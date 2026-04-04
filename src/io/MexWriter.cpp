#include "MexWriter.hpp"

#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cctype>
#include <cmath>
#include <map>
#include <set>

#include <antlr4-runtime.h>
#include "generated/BNGLexer.h"
#include "generated/BNGParser.h"
#include "parser/PatternGraphBuilder.hpp"
#include "core/Ullmann.hpp"

namespace bng::io {

std::string MexWriter::write(const ast::Model& model, const engine::GeneratedNetwork& network) {
    Options opts;
    return write(model, network, opts);
}

std::string MexWriter::write(const ast::Model& model, const engine::GeneratedNetwork& network, const Options& opts) {
    std::ostringstream out;

    const std::string modelName = model.getModelName();
    const std::size_t nSpecies = network.species.size();
    const std::size_t nRxns = network.reactions.size();

    // Compute observable groups and stoichiometry
    auto groups = computeObservableGroups(model, network);
    auto stoich = computeStoichiometry(network, nSpecies);

    // Collect parameter info
    std::vector<std::string> paramNames;
    std::vector<double> paramValues;
    for (const auto& param : model.getParameters().all()) {
        paramNames.push_back(param.getName());
        paramValues.push_back(param.getValue());
    }
    const std::size_t nParams = paramNames.size();

    // Detect fixed species
    std::vector<bool> fixedSpecies(nSpecies, false);
    for (std::size_t i = 0; i < nSpecies; ++i) {
        fixedSpecies[i] = network.species.get(i).isConstant();
    }

    // ===== Header comment =====
    out << "/*\n";
    out << "**   " << modelName << "_mex.c\n";
    out << "**\n";
    out << "**   MATLAB MEX implementation of BioNetGen model '" << modelName << "'.\n";
    out << "**\n";
    out << "**   Compile in MATLAB with:\n";
    out << "**     mex " << modelName << "_mex.c\n";
    out << "**\n";
    out << "**   Usage in MATLAB:\n";
    out << "**     [species_out, observables_out] = " << modelName << "_mex(timepoints, species_init, parameters)\n";
    out << "**\n";
    out << "**   Uses a built-in RK4 integrator (no external solver dependencies).\n";
    out << "*/\n\n";

    // ===== Includes =====
    out << "#include \"mex.h\"\n";
    out << "#include <math.h>\n";
    out << "#include <string.h>\n";
    out << "#include <stdlib.h>\n\n";

    // ===== Problem dimensions =====
    out << "/* Problem Dimensions */\n";
    out << "#define N_PARAMETERS   " << nParams << "\n";
    out << "#define N_EXPRESSIONS  " << nParams << "\n";
    out << "#define N_OBSERVABLES  " << groups.size() << "\n";
    out << "#define N_RATELAWS     " << nRxns << "\n";
    out << "#define N_SPECIES      " << nSpecies << "\n\n";

    // ===== calc_expressions =====
    out << "/* Calculate expressions */\n";
    out << "static void calc_expressions(double *expressions, const double *parameters)\n";
    out << "{\n";
    for (std::size_t i = 0; i < nParams; ++i) {
        out << "    expressions[" << i << "] = parameters[" << i << "];";
        out << "   /* " << paramNames[i] << " */\n";
    }
    out << "}\n\n";

    // ===== calc_observables =====
    out << "/* Calculate observables */\n";
    out << "static void calc_observables(double *observables, const double *species, const double *expressions)\n";
    out << "{\n";
    for (std::size_t g = 0; g < groups.size(); ++g) {
        if (groups[g].entries.empty()) {
            out << "    observables[" << g << "] = 0;";
        } else {
            out << "    observables[" << g << "] = ";
            bool first = true;
            for (const auto& [specIdx, weight] : groups[g].entries) {
                if (!first) out << " + ";
                if (weight > 1) {
                    out << weight << "*";
                }
                out << "species[" << specIdx << "]";
                first = false;
            }
            out << ";";
        }
        out << "   /* " << groups[g].name << " */\n";
    }
    out << "}\n\n";

    // ===== calc_ratelaws =====
    out << "/* Calculate ratelaws */\n";
    out << "static void calc_ratelaws(double *ratelaws, const double *species, const double *expressions, const double *observables)\n";
    out << "{\n";
    for (std::size_t r = 0; r < nRxns; ++r) {
        const auto& rxn = network.reactions.all()[r];
        out << "    ratelaws[" << r << "] = ";

        const std::string& rateLaw = rxn.getRateLaw();
        std::string cleanRate = rateLaw;
        auto localPos = cleanRate.find("|local:");
        if (localPos != std::string::npos) {
            cleanRate = cleanRate.substr(0, localPos);
        }

        double statFactor = rxn.getFactor();

        bool isNumeric = false;
        double rateVal = 0.0;
        try {
            std::size_t pos = 0;
            rateVal = std::stod(cleanRate, &pos);
            if (pos == cleanRate.size()) isNumeric = true;
        } catch (...) {}

        if (isNumeric) {
            double combined = statFactor * rateVal;
            out << combined;
        } else {
            std::string rateRef = convertRateToC(cleanRate, paramNames);
            if (std::abs(statFactor - 1.0) > 1e-12) {
                out << statFactor << "*" << rateRef;
            } else {
                out << rateRef;
            }
        }

        // Multiply by reactant concentrations
        for (const auto idx : rxn.getReactants()) {
            out << "*species[" << idx << "]";
        }

        out << ";\n";
    }
    out << "}\n\n";

    // ===== calc_species_deriv =====
    out << "/* Calculate species derivatives */\n";
    out << "static void calc_species_deriv(double t, const double *species, double *dydt,\n";
    out << "                               const double *expressions)\n";
    out << "{\n";
    out << "    double observables[N_OBSERVABLES];\n";
    out << "    double ratelaws[N_RATELAWS];\n\n";
    out << "    /* calculate observables */\n";
    out << "    calc_observables(observables, species, expressions);\n\n";
    out << "    /* calculate ratelaws */\n";
    out << "    calc_ratelaws(ratelaws, species, expressions, observables);\n\n";
    out << "    /* calculate derivatives */\n";

    for (std::size_t i = 0; i < nSpecies; ++i) {
        out << "    dydt[" << i << "] = ";
        if (fixedSpecies[i]) {
            out << "0; /* fixed species */\n";
            continue;
        }

        bool found = false;
        for (const auto& entry : stoich) {
            if (entry.speciesIndex == i) {
                found = true;
                bool first = true;
                for (const auto& [rxnIdx, coeff] : entry.rxnCoeffs) {
                    if (first) {
                        if (coeff == 1) {
                            out << "ratelaws[" << rxnIdx << "]";
                        } else if (coeff == -1) {
                            out << "-ratelaws[" << rxnIdx << "]";
                        } else {
                            out << coeff << "*ratelaws[" << rxnIdx << "]";
                        }
                    } else {
                        if (coeff == 1) {
                            out << " + ratelaws[" << rxnIdx << "]";
                        } else if (coeff == -1) {
                            out << " - ratelaws[" << rxnIdx << "]";
                        } else if (coeff > 0) {
                            out << " + " << coeff << "*ratelaws[" << rxnIdx << "]";
                        } else {
                            out << " - " << (-coeff) << "*ratelaws[" << rxnIdx << "]";
                        }
                    }
                    first = false;
                }
                break;
            }
        }
        if (!found) {
            out << "0";
        }
        out << ";\n";
    }

    out << "}\n\n";

    // ===== RK4 integrator =====
    out << "/* Built-in RK4 integrator */\n";
    out << "static void rk4_step(double t, double dt, double *y, double *y_out,\n";
    out << "                     int n, const double *expressions)\n";
    out << "{\n";
    out << "    int i;\n";
    out << "    double *k1, *k2, *k3, *k4, *ytmp;\n\n";
    out << "    k1   = (double *)mxMalloc(n * sizeof(double));\n";
    out << "    k2   = (double *)mxMalloc(n * sizeof(double));\n";
    out << "    k3   = (double *)mxMalloc(n * sizeof(double));\n";
    out << "    k4   = (double *)mxMalloc(n * sizeof(double));\n";
    out << "    ytmp = (double *)mxMalloc(n * sizeof(double));\n\n";
    out << "    /* k1 */\n";
    out << "    calc_species_deriv(t, y, k1, expressions);\n\n";
    out << "    /* k2 */\n";
    out << "    for (i = 0; i < n; i++) ytmp[i] = y[i] + 0.5 * dt * k1[i];\n";
    out << "    calc_species_deriv(t + 0.5 * dt, ytmp, k2, expressions);\n\n";
    out << "    /* k3 */\n";
    out << "    for (i = 0; i < n; i++) ytmp[i] = y[i] + 0.5 * dt * k2[i];\n";
    out << "    calc_species_deriv(t + 0.5 * dt, ytmp, k3, expressions);\n\n";
    out << "    /* k4 */\n";
    out << "    for (i = 0; i < n; i++) ytmp[i] = y[i] + dt * k3[i];\n";
    out << "    calc_species_deriv(t + dt, ytmp, k4, expressions);\n\n";
    out << "    /* combine */\n";
    out << "    for (i = 0; i < n; i++)\n";
    out << "        y_out[i] = y[i] + (dt / 6.0) * (k1[i] + 2.0*k2[i] + 2.0*k3[i] + k4[i]);\n\n";
    out << "    mxFree(k1);\n";
    out << "    mxFree(k2);\n";
    out << "    mxFree(k3);\n";
    out << "    mxFree(k4);\n";
    out << "    mxFree(ytmp);\n";
    out << "}\n\n";

    // ===== Adaptive RK4 driver =====
    out << "/* Adaptive RK4 driver: integrate from t0 to tf with sub-stepping */\n";
    out << "static void integrate_rk4(double t0, double tf, double *y, int n,\n";
    out << "                          const double *expressions, int max_steps)\n";
    out << "{\n";
    out << "    double dt = (tf - t0) / (double)max_steps;\n";
    out << "    double t  = t0;\n";
    out << "    double *y_new = (double *)mxMalloc(n * sizeof(double));\n";
    out << "    int step;\n\n";
    out << "    for (step = 0; step < max_steps; step++) {\n";
    out << "        if (t + dt > tf) dt = tf - t;\n";
    out << "        rk4_step(t, dt, y, y_new, n, expressions);\n";
    out << "        memcpy(y, y_new, n * sizeof(double));\n";
    out << "        t += dt;\n";
    out << "        if (t >= tf) break;\n";
    out << "    }\n\n";
    out << "    mxFree(y_new);\n";
    out << "}\n\n";

    // ===== mexFunction gateway =====
    out << "/*\n";
    out << "** Gateway function\n";
    out << "**   [species_out, observables_out] = " << modelName << "_mex(timepoints, species_init, parameters)\n";
    out << "**\n";
    out << "**   timepoints   : 1 x T or T x 1 double array\n";
    out << "**   species_init : 1 x " << nSpecies << " or " << nSpecies << " x 1 double array\n";
    out << "**   parameters   : 1 x " << nParams << " or " << nParams << " x 1 double array\n";
    out << "**\n";
    out << "**   species_out     : " << nSpecies << " x T double array\n";
    out << "**   observables_out : " << groups.size() << " x T double array\n";
    out << "*/\n";
    out << "void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])\n";
    out << "{\n";
    out << "    int i, j, n_timepoints, n_substeps;\n";
    out << "    double *timepoints, *species_init, *parameters;\n";
    out << "    double *species_out, *observables_out;\n";
    out << "    double *y;\n";
    out << "    double expressions[N_EXPRESSIONS];\n";
    out << "    double observables[N_OBSERVABLES];\n\n";

    // Input validation
    out << "    /* Validate inputs */\n";
    out << "    if (nrhs < 3)\n";
    out << "        mexErrMsgTxt(\"Three inputs required: timepoints, species_init, parameters.\");\n";
    out << "    if (nlhs > 2)\n";
    out << "        mexErrMsgTxt(\"Too many output arguments (max 2).\");\n\n";

    out << "    /* Get input pointers */\n";
    out << "    timepoints   = mxGetPr(prhs[0]);\n";
    out << "    species_init = mxGetPr(prhs[1]);\n";
    out << "    parameters   = mxGetPr(prhs[2]);\n\n";

    out << "    n_timepoints = (int)mxGetNumberOfElements(prhs[0]);\n\n";

    out << "    /* Validate dimensions */\n";
    out << "    if (mxGetNumberOfElements(prhs[1]) != N_SPECIES)\n";
    out << "        mexErrMsgTxt(\"species_init must have " << nSpecies << " elements.\");\n";
    out << "    if (mxGetNumberOfElements(prhs[2]) != N_PARAMETERS)\n";
    out << "        mexErrMsgTxt(\"parameters must have " << nParams << " elements.\");\n";
    out << "    if (n_timepoints < 2)\n";
    out << "        mexErrMsgTxt(\"timepoints must have at least 2 elements.\");\n\n";

    // Create output arrays
    out << "    /* Create output arrays */\n";
    out << "    plhs[0] = mxCreateDoubleMatrix(N_SPECIES, n_timepoints, mxREAL);\n";
    out << "    species_out = mxGetPr(plhs[0]);\n";
    out << "    if (nlhs > 1) {\n";
    out << "        plhs[1] = mxCreateDoubleMatrix(N_OBSERVABLES, n_timepoints, mxREAL);\n";
    out << "        observables_out = mxGetPr(plhs[1]);\n";
    out << "    } else {\n";
    out << "        observables_out = NULL;\n";
    out << "    }\n\n";

    // Initialize
    out << "    /* Calculate expressions (constant, do once) */\n";
    out << "    calc_expressions(expressions, parameters);\n\n";

    out << "    /* Copy initial conditions */\n";
    out << "    y = (double *)mxMalloc(N_SPECIES * sizeof(double));\n";
    out << "    for (i = 0; i < N_SPECIES; i++) y[i] = species_init[i];\n\n";

    out << "    /* Store initial state */\n";
    out << "    for (i = 0; i < N_SPECIES; i++)\n";
    out << "        species_out[i] = y[i];  /* column 0 */\n";
    out << "    if (observables_out) {\n";
    out << "        calc_observables(observables, y, expressions);\n";
    out << "        for (i = 0; i < N_OBSERVABLES; i++)\n";
    out << "            observables_out[i] = observables[i];\n";
    out << "    }\n\n";

    // Integration loop
    out << "    /* Number of RK4 sub-steps per output interval */\n";
    out << "    n_substeps = " << opts.maxNumSteps << ";\n\n";
    out << "    /* Integrate to each timepoint */\n";
    out << "    for (j = 1; j < n_timepoints; j++) {\n";
    out << "        integrate_rk4(timepoints[j-1], timepoints[j], y, N_SPECIES,\n";
    out << "                      expressions, n_substeps);\n\n";
    out << "        /* Store species (column-major: row i, col j -> index j*N_SPECIES + i) */\n";
    out << "        for (i = 0; i < N_SPECIES; i++)\n";
    out << "            species_out[j * N_SPECIES + i] = y[i];\n\n";
    out << "        /* Store observables */\n";
    out << "        if (observables_out) {\n";
    out << "            calc_observables(observables, y, expressions);\n";
    out << "            for (i = 0; i < N_OBSERVABLES; i++)\n";
    out << "                observables_out[j * N_OBSERVABLES + i] = observables[i];\n";
    out << "        }\n";
    out << "    }\n\n";

    out << "    mxFree(y);\n";
    out << "}\n";

    return out.str();
}

std::string MexWriter::convertRateToC(const std::string& rate, const std::vector<std::string>& paramNames) {
    std::string result = rate;

    // Replace parameter names with expressions[i] - longer names first
    std::vector<std::pair<std::size_t, std::string>> sorted;
    for (std::size_t i = 0; i < paramNames.size(); ++i) {
        sorted.push_back({i, paramNames[i]});
    }
    std::sort(sorted.begin(), sorted.end(), [](const auto& a, const auto& b) {
        return a.second.size() > b.second.size();
    });

    for (const auto& [idx, name] : sorted) {
        std::string paramRef = "expressions[" + std::to_string(idx) + "]";
        std::size_t pos = 0;
        while ((pos = result.find(name, pos)) != std::string::npos) {
            bool validStart = (pos == 0 || (!std::isalnum(static_cast<unsigned char>(result[pos - 1])) && result[pos - 1] != '_'));
            bool validEnd = (pos + name.length() >= result.length() ||
                           (!std::isalnum(static_cast<unsigned char>(result[pos + name.length()])) && result[pos + name.length()] != '_'));
            if (validStart && validEnd) {
                result.replace(pos, name.length(), paramRef);
                pos += paramRef.length();
            } else {
                pos += name.length();
            }
        }
    }

    return result;
}

std::vector<MexWriter::ObservableGroup> MexWriter::computeObservableGroups(
    const ast::Model& model, const engine::GeneratedNetwork& network) {
    std::vector<ObservableGroup> groups;
    auto& mutableModel = const_cast<ast::Model&>(model);

    for (const auto& observable : model.getObservables()) {
        ObservableGroup group;
        group.name = observable.getName();
        group.type = observable.getType();

        for (std::size_t speciesIndex = 0; speciesIndex < network.species.size(); ++speciesIndex) {
            std::size_t weight = 0;

            for (const auto& patternText : observable.getPatterns()) {
                try {
                    antlr4::ANTLRInputStream input(patternText);
                    BNGLexer lexer(&input);
                    antlr4::CommonTokenStream tokens(&lexer);
                    BNGParser parser(&tokens);
                    auto* species = parser.species_def();

                    if (parser.getNumberOfSyntaxErrors() == 0) {
                        const auto pattern = bng::parser::buildPatternGraph(species, mutableModel);
                        BNGcore::UllmannSGIso matcher(pattern,
                            network.species.get(speciesIndex).getSpeciesGraph().getGraph());
                        BNGcore::List<BNGcore::Map> maps;
                        weight += matcher.find_maps(maps);
                    }
                } catch (...) {}
            }

            if (observable.getType() == "Species" && weight > 0) {
                weight = 1;
            }

            if (weight > 0) {
                group.entries.push_back({speciesIndex, weight});
            }
        }

        groups.push_back(std::move(group));
    }

    return groups;
}

std::vector<MexWriter::StoichEntry> MexWriter::computeStoichiometry(
    const engine::GeneratedNetwork& network, std::size_t nSpecies) {
    std::map<std::size_t, std::map<std::size_t, int>> stoichMap;

    for (std::size_t r = 0; r < network.reactions.size(); ++r) {
        const auto& rxn = network.reactions.all()[r];
        for (const auto idx : rxn.getReactants()) {
            stoichMap[idx][r] -= 1;
        }
        for (const auto idx : rxn.getProducts()) {
            stoichMap[idx][r] += 1;
        }
    }

    std::vector<StoichEntry> result;
    for (auto& [specIdx, rxnMap] : stoichMap) {
        StoichEntry entry;
        entry.speciesIndex = specIdx;
        for (auto& [rxnIdx, coeff] : rxnMap) {
            if (coeff != 0) {
                entry.rxnCoeffs.push_back({rxnIdx, coeff});
            }
        }
        if (!entry.rxnCoeffs.empty()) {
            result.push_back(std::move(entry));
        }
    }

    return result;
}

} // namespace bng::io
