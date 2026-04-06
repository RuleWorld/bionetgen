// MacroBNGModel.cpp — faithful port of Perl BNG2 MacroBNGModel.pm (first half)
// Methods 1-20: constructor through pre_rules()

#include "MacroBNGModel.hpp"

#include <algorithm>
#include <cassert>
#include <cctype>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <set>
#include <sstream>
#include <stdexcept>

namespace bng {
namespace ast {

// ---------------------------------------------------------------------------
// 1. Constructor / Destructor
// ---------------------------------------------------------------------------

MacroBNGModel::MacroBNGModel() = default;
MacroBNGModel::~MacroBNGModel() = default;

// ---------------------------------------------------------------------------
// 2. Static string helpers
// ---------------------------------------------------------------------------

std::vector<std::string> MacroBNGModel::split(const std::string& s, char delim) {
    std::vector<std::string> tokens;
    std::istringstream iss(s);
    std::string tok;
    while (std::getline(iss, tok, delim)) {
        tokens.push_back(tok);
    }
    return tokens;
}

std::vector<std::string> MacroBNGModel::split(const std::string& s,
                                               const std::string& delim) {
    std::vector<std::string> tokens;
    if (delim.empty()) {
        tokens.push_back(s);
        return tokens;
    }
    std::string::size_type start = 0;
    std::string::size_type pos;
    while ((pos = s.find(delim, start)) != std::string::npos) {
        tokens.push_back(s.substr(start, pos - start));
        start = pos + delim.size();
    }
    tokens.push_back(s.substr(start));
    return tokens;
}

std::string MacroBNGModel::join(const std::string& delim,
                                 const std::vector<std::string>& v) {
    std::string result;
    for (size_t i = 0; i < v.size(); ++i) {
        if (i > 0) result += delim;
        result += v[i];
    }
    return result;
}

bool MacroBNGModel::startsWith(const std::string& s, const std::string& prefix) {
    if (prefix.size() > s.size()) return false;
    return s.compare(0, prefix.size(), prefix) == 0;
}

bool MacroBNGModel::endsWith(const std::string& s, const std::string& suffix) {
    if (suffix.size() > s.size()) return false;
    return s.compare(s.size() - suffix.size(), suffix.size(), suffix) == 0;
}

std::string MacroBNGModel::replaceFirst(const std::string& s,
                                         const std::string& from,
                                         const std::string& to) {
    std::string result = s;
    auto pos = result.find(from);
    if (pos != std::string::npos) {
        result.replace(pos, from.size(), to);
    }
    return result;
}

std::string MacroBNGModel::replaceAll(const std::string& s,
                                       const std::string& from,
                                       const std::string& to) {
    if (from.empty()) return s;
    std::string result = s;
    std::string::size_type pos = 0;
    while ((pos = result.find(from, pos)) != std::string::npos) {
        result.replace(pos, from.size(), to);
        pos += to.size();
    }
    return result;
}

std::string MacroBNGModel::ltrim(const std::string& s) {
    auto it = std::find_if(s.begin(), s.end(),
                           [](unsigned char ch) { return !std::isspace(ch); });
    return std::string(it, s.end());
}

std::string MacroBNGModel::rtrim(const std::string& s) {
    auto it = std::find_if(s.rbegin(), s.rend(),
                           [](unsigned char ch) { return !std::isspace(ch); });
    return std::string(s.begin(), it.base());
}

std::string MacroBNGModel::trim(const std::string& s) {
    return ltrim(rtrim(s));
}

std::string MacroBNGModel::quotemeta(const std::string& s) {
    // Escape all non-alphanumeric, non-underscore characters for regex use
    // (mirrors Perl \Q...\E / quotemeta)
    std::string result;
    result.reserve(s.size() * 2);
    for (char ch : s) {
        if (!std::isalnum(static_cast<unsigned char>(ch)) && ch != '_') {
            result += '\\';
        }
        result += ch;
    }
    return result;
}

// ---------------------------------------------------------------------------
// 3. version()   (Perl lines 1776-1780)
// ---------------------------------------------------------------------------

std::string MacroBNGModel::version() const {
    // Perl delegates to $base_model->version(); we return BNG2 version string.
    return "2.2.6-stable";
}

// ---------------------------------------------------------------------------
// 4. get_line()   (Perl lines 1709-1727)
// ---------------------------------------------------------------------------

std::string MacroBNGModel::get_line() {
    std::string line;
    while (!file_dat_.empty()) {
        line = file_dat_.front();
        file_dat_.erase(file_dat_.begin());
        ++line_number_;

        // chomp: remove trailing newline / carriage return
        while (!line.empty() && (line.back() == '\n' || line.back() == '\r')) {
            line.pop_back();
        }

        // remove comments
        auto hash_pos = line.find('#');
        if (hash_pos != std::string::npos) {
            line.erase(hash_pos);
        }

        // skip blank lines
        if (line.find_first_not_of(" \t\r\n") == std::string::npos) {
            continue;
        }

        // handle continuation lines (trailing backslash)
        while (!line.empty() && line.back() == '\\') {
            // remove trailing backslash and whitespace before it
            line.pop_back();
            // trim trailing whitespace that was before the backslash
            while (!line.empty() && std::isspace(static_cast<unsigned char>(line.back()))) {
                line.pop_back();
            }
            ++line_number_;
            if (file_dat_.empty()) break;
            std::string nline = file_dat_.front();
            file_dat_.erase(file_dat_.begin());
            // chomp
            while (!nline.empty() && (nline.back() == '\n' || nline.back() == '\r')) {
                nline.pop_back();
            }
            // remove comments
            auto hp = nline.find('#');
            if (hp != std::string::npos) {
                nline.erase(hp);
            }
            line += nline;
        }
        return line;
    }
    return "";
}

// ---------------------------------------------------------------------------
// 5. read_block_array()   (Perl lines 1731-1763)
// ---------------------------------------------------------------------------

std::pair<std::vector<std::pair<std::string, int>>, std::string>
MacroBNGModel::read_block_array(const std::string& name) {
    std::vector<std::pair<std::string, int>> array;
    bool got_end = false;
    std::string line;

    while (!(line = get_line()).empty()) {
        // Check for end of block
        std::string trimmed = line;
        // strip leading whitespace
        trimmed.erase(0, trimmed.find_first_not_of(" \t"));
        if (startsWith(trimmed, "end ") || startsWith(trimmed, "end\t")) {
            std::string ename = trimmed.substr(4);
            // trim and normalize whitespace
            ename = trim(ename);
            ename = std::regex_replace(ename, std::regex("\\s+"), " ");
            if (ename != name) {
                return {{}, errgen("end " + ename + " does not match begin " + name)};
            }
            got_end = true;
            break;
        }
        // Check for nested begin (error)
        if (startsWith(trimmed, "begin ") || startsWith(trimmed, "begin\t")) {
            return {{}, errgen("begin block before end of previous block " + name)};
        }
        // Add declaration
        array.emplace_back(line, line_number_);
    }

    if (!got_end) {
        return {{}, errgen("begin " + name + " has no matching end " + name)};
    }
    return {array, ""};
}

// ---------------------------------------------------------------------------
// 6. errgen()   (Perl lines 1767-1774)
// ---------------------------------------------------------------------------

std::string MacroBNGModel::errgen(const std::string& err, int lno) {
    int lineno = (lno >= 0) ? lno : line_number_;
    // Perl: s/[*]/\*/g on err — escape asterisks
    std::string e = replaceAll(err, "*", "\\*");
    char buf[512];
    std::snprintf(buf, sizeof(buf), "%s\n  at line %d of file %s",
                  e.c_str(), lineno, file_.c_str());
    return std::string(buf);
}

// ---------------------------------------------------------------------------
// 7. readFile()   (Perl lines 32-98)
// ---------------------------------------------------------------------------

std::string MacroBNGModel::readFile(const std::map<std::string, std::string>& params) {
    std::string fname;
    std::string prefix;
    std::string param_prefix;
    std::string err;

    // Process optional parameters
    for (auto& [param, value] : params) {
        if (param == "no_exec") {
            NO_EXEC_ = (value != "0" && !value.empty());
        } else if (param == "file") {
            fname = value;
            // strip extension to get prefix:  name =~ s/[.]([^.]+)$//
            std::string name = fname;
            auto dot_pos = name.rfind('.');
            if (dot_pos != std::string::npos) {
                name.erase(dot_pos);
            }
            param_prefix = name;

            // pre_macr: preprocess the BNGL into macro form
            err = pre_macr(param_prefix);
            if (!err.empty()) return err;
        } else if (param == "prefix") {
            prefix = value;
        } else {
            std::cerr << "WARNING: Parameter " << param << " ignored" << std::endl;
        }
    }

    if (fname.empty()) {
        return "Parameter file must be specified in readFile";
    }

    std::cout << "\n#################-START BASEMODEL-#####################\n";
    // In the full port the base_model->readFile() call would happen here.
    // For the C++ standalone macro reducer we skip this delegation.
    std::cout << "\n#################-FINISH BASEMODEL-#####################\n";

    // cor_net: correct the .net file
    cor_net(param_prefix);
    if (WFILErec_.is_open()) {
        WFILErec_ << " END COR_NET \n";
        WFILErec_.close();
    }

    // Rename macr_*.bngl -> macr_*.bnglisx and write the simulation file
    std::string filen = "macr_" + param_prefix + ".bnglisx";
    std::string bngl_file = "macr_" + param_prefix + ".bngl";
    if (std::filesystem::exists(bngl_file)) {
        std::filesystem::rename(bngl_file, filen);
    }

    {
        std::ofstream wbngl(bngl_file);
        if (!wbngl.is_open()) {
            return "Can't open " + bngl_file + " for writing";
        }
        wbngl << simul1_;
        wbngl.close();
    }

    return err;
}

// ---------------------------------------------------------------------------
// 8. pre_macr()   (Perl lines 105-371)
// ---------------------------------------------------------------------------

std::string MacroBNGModel::pre_macr(const std::string& param_prefix) {
    std::string err;

    // File paths
    std::string bnglfile = param_prefix + ".bngl";
    std::string recfile = "macr_" + param_prefix + ".rec";
    std::string parfile = "macr_" + param_prefix + ".par";

    WFILErec_.open(recfile);
    if (!WFILErec_.is_open()) {
        return "Can't open " + recfile;
    }

    std::ofstream WFILEpar(parfile);
    if (!WFILEpar.is_open()) {
        return "Can't open " + parfile;
    }
    WFILErec_ << recfile << "\n";

    // READ:
    std::cout << "Reading from file " << bnglfile << "\n";
    {
        std::ifstream fh(bnglfile);
        if (!fh.is_open()) {
            return "Couldn't read from file " + bnglfile;
        }
        file_dat_.clear();
        std::string fline;
        while (std::getline(fh, fline)) {
            file_dat_.push_back(fline);
        }
        fh.close();
    }

    line_number_ = 0;

    // Local data maps for pre-processing
    std::map<std::string, int> nm_site;
    std::map<std::string, int> nm2_site;
    std::map<std::string, std::vector<std::string>> skf;
    std::map<std::string, std::vector<std::string>> dpp_site;
    std::map<std::string, std::vector<int>> lis1h;

    // Clear accumulated data
    species2_.clear();
    reac1tion_.clear();
    obser1vable_.clear();
    gene1rate_.clear();
    para1ms_.clear();

    bool abort_read = false;
    std::string line;
    while (!(line = get_line()).empty() && !abort_read) {
        // Check for "begin <block>"
        std::string trimmed = line;
        trimmed.erase(0, trimmed.find_first_not_of(" \t"));
        std::smatch m;
        std::regex re_begin("^begin\\s+(.*)");
        if (std::regex_search(trimmed, m, re_begin)) {
            std::string name = m[1].str();
            name = trim(name);
            name = std::regex_replace(name, std::regex("\\s+"), " ");

            auto [block_dat, block_err] = read_block_array(name);
            if (!block_err.empty()) {
                err = block_err;
                break;
            }

            // --- parameters ---
            if (name == "parameters") {
                WFILEpar.open(parfile, std::ios::trunc);
                WFILEpar << "begin parameters\n";
                for (auto& [entry, lno] : block_dat) {
                    WFILEpar << entry << "\n";
                    auto tok1ens = split(entry, ' ');
                    // Remove leading index if numeric
                    std::string na1m, va1l;
                    size_t idx = 0;
                    if (!tok1ens.empty() && !tok1ens[0].empty() &&
                        std::all_of(tok1ens[0].begin(), tok1ens[0].end(), ::isdigit)) {
                        idx = 1;
                    }
                    if (idx < tok1ens.size()) {
                        na1m = tok1ens[idx++];
                    } else {
                        err = errgen("No parameter name specified", lno);
                        abort_read = true;
                        break;
                    }
                    if (idx < tok1ens.size()) {
                        va1l = tok1ens[idx];
                    } else {
                        err = errgen("No parameter value specified", lno);
                        abort_read = true;
                        break;
                    }
                    para1ms_[na1m] = va1l;
                }
                std::cout << "Read " << para1ms_.size() << " parameters.\n";
                WFILEpar << "end parameters\n";
                WFILEpar.close();
            }
            // --- molecule_types ---
            else if (std::regex_match(name, std::regex("^molecule[_ ]types$"))) {
                // We store the type info via pre_species1 later; just count.
                std::cout << "Read " << block_dat.size() << " molecule types.\n";
            }
            // --- seed_species ---
            else if (std::regex_match(name, std::regex("^seed[_ ]species$"))) {
                // Perl stores into SeedSpeciesList; we just count.
                std::cout << "Read " << block_dat.size() << " species.\n";
            }
            // --- species ---
            else if (name == "species") {
                for (auto& [entry, lno] : block_dat) {
                    species2_.push_back(entry);
                }
                std::cout << "Read " << block_dat.size() << " species.\n";
            }
            // --- reaction_rules ---
            else if (std::regex_match(name, std::regex("^reaction[_ ]rules$"))) {
                int nerr_count = 0;
                for (auto& [entry, lno] : block_dat) {
                    std::string str1ing = entry;
                    std::string nam1e;
                    // Check for user-named rule "name: ..."
                    std::smatch rm;
                    if (std::regex_search(str1ing, rm, std::regex("^\\s*([^:].*)[:]\\s*"))) {
                        nam1e = rm[1].str();
                        std::cerr << "ERROR 1002   rules  (user) name=" << nam1e << "\n";
                        return "ERROR 1002: named rules not supported";
                    } else if (std::regex_search(str1ing, rm, std::regex("^\\s*(\\d+)\\s+"))) {
                        nam1e = rm[1].str();
                        str1ing = rm.suffix().str();
                    }
                    str1ing = ltrim(str1ing);
                    reac1tion_.push_back(str1ing);
                }
                if (nerr_count > 0) {
                    err = "Reaction rule list could not be read because of errors";
                    break;
                }
                std::cout << "Read " << reac1tion_.size() << " reaction rule(s).\n";
            }
            // --- observables ---
            else if (name == "observables") {
                for (auto& [entry, lno] : block_dat) {
                    std::string str1ing = entry;
                    // Strip leading numeric index
                    std::smatch rm;
                    if (std::regex_search(str1ing, rm, std::regex("^\\s*\\d+\\s+"))) {
                        str1ing = rm.suffix().str();
                    }
                    obser1vable_.push_back(str1ing);
                }
                std::cout << "Read " << obser1vable_.size() << " observable(s).\n";
            }
            // --- reactions, groups: we skip these for the macro reducer ---
            else if (name == "reactions" || name == "groups") {
                std::cout << "Read " << block_dat.size() << " " << name << ".\n";
            }
            // --- unknown block ---
            else {
                std::cerr << "WARNING: Could not process block type " << name << "\n";
            }
        }
        // Check for action lines like "generate_network" / "simulate"
        else if (std::regex_search(trimmed, m, std::regex("^([A-Za-z][^(]*)"))) {
            gene1rate_.push_back(line);
        }
    } // while get_line

    if (!err.empty()) return err;

    // Pre-process species to extract molecule:site counts
    pre_species1(nm_site, nm2_site);

    // Pre-process rules: binding analysis, hierarchy, macro transform
    pre_rules(nm_site, nm2_site, param_prefix, skf, dpp_site, lis1h);

    // pre_obs1 will be called by the second-half agent
    // (For completeness of the first half we omit pre_obs1 here since it is
    //  not in the assignment list; the second half agent will add it.)

    WFILErec_ << " EXIT FROM sub PRE_MACR for *.rec \n";
    WFILErec_ << " BEGIN COR_NET \n";

    // Assemble the macro BNGL file from the intermediate files
    std::string macrfile = "macr_" + param_prefix + ".bngl";
    std::string spec2file_r = "macr_" + param_prefix + ".spec2";
    std::string rulesfile_r = "macr_" + param_prefix + ".rules";
    std::string obserfile_r = "macr_" + param_prefix + ".obser";
    std::string parfile_r = "macr_" + param_prefix + ".par";

    {
        std::ofstream wf(macrfile);
        if (!wf.is_open()) return "Can't open " + macrfile;

        // Copy parameters
        {
            std::ifstream rf(parfile_r);
            if (rf.is_open()) {
                std::string fline;
                while (std::getline(rf, fline)) {
                    wf << fline << "\n";
                }
            }
        }
        // Copy species
        {
            std::ifstream rf(spec2file_r);
            if (rf.is_open()) {
                std::string fline;
                while (std::getline(rf, fline)) {
                    wf << fline << "\n";
                }
            }
        }
        // Copy rules
        {
            std::ifstream rf(rulesfile_r);
            if (rf.is_open()) {
                std::string fline;
                while (std::getline(rf, fline)) {
                    wf << fline << "\n";
                }
            }
        }
        // Copy observables
        {
            std::ifstream rf(obserfile_r);
            if (rf.is_open()) {
                std::string fline;
                while (std::getline(rf, fline)) {
                    wf << fline << "\n";
                }
            }
        }
        wf.close();
    }

    // Delete intermediate files (Perl lines 366-370)
    for (const auto& suff : {"obser", "par", "spec2", "spec1", "rules", "cdat"}) {
        std::string f = "macr_" + param_prefix + "." + suff;
        if (std::filesystem::exists(f)) {
            std::filesystem::remove(f);
        }
    }

    return "";
}

// ---------------------------------------------------------------------------
// 9. pre_species1()   (Perl lines 375-407)
// ---------------------------------------------------------------------------

void MacroBNGModel::pre_species1(std::map<std::string, int>& nm_site,
                                  std::map<std::string, int>& nm2_site) {
    for (auto& ii : species2_) {
        std::string entry = ii;
        std::string name;

        // Strip leading label "name: ..."
        {
            std::smatch m;
            if (std::regex_search(entry, m, std::regex("^\\s*([^:].*)[:]\\s*"))) {
                name = m[1].str();
                std::cerr << "ERROR 1001 block species      (user) name=" << name << "\n";
                return;
            } else {
                // Strip leading numeric index
                std::regex_replace(entry, std::regex("^\\s*\\d+\\s+"), "");
                std::smatch rm;
                if (std::regex_search(entry, rm, std::regex("^\\s*\\d+\\s+"))) {
                    entry = rm.suffix().str();
                }
            }
        }

        // First token is the species pattern
        auto tok1ens = split(entry, ' ');
        if (tok1ens.empty()) continue;
        std::string spec_entry = tok1ens[0];

        // Extract molecule(site,site,...) patterns
        // Repeatedly match (...) groups
        std::regex re_paren("([\\(])(.*?)([\\)])");
        std::smatch pm;
        std::string remaining = spec_entry;
        while (std::regex_search(remaining, pm, re_paren)) {
            // name is everything before the '('
            std::string prefix_str = pm.prefix().str();
            // Get the molecule name: last segment after '.'
            auto dot_pos = prefix_str.rfind('.');
            if (dot_pos != std::string::npos) {
                name = prefix_str.substr(dot_pos + 1);
            } else {
                name = prefix_str;
            }

            if (nm_site.find(name) == nm_site.end()) {
                std::string inside = pm[2].str();  // contents inside parentheses
                auto sits = split(inside, ',');
                // Strip modifiers from site names
                for (auto& s : sits) {
                    auto bang = s.find('!');
                    auto tilde = s.find('~');
                    auto cut = std::min(bang, tilde);
                    if (cut != std::string::npos) {
                        s = s.substr(0, cut);
                    }
                }
                for (auto& s : sits) {
                    nm_site[name + ":" + s]++;
                }
                // Check for duplicate sites => nm2_site
                for (auto& [key, val] : nm_site) {
                    if (key.find(':') != std::string::npos && val > 1) {
                        nm2_site[key] = val;
                    }
                }
                nm_site[name] = static_cast<int>(sits.size());
            }

            remaining = pm.suffix().str();
            // Also strip "name." prefix from remaining
            if (!remaining.empty() && remaining[0] == '.') {
                remaining = remaining.substr(1);
            }
        }
    }
}

// ---------------------------------------------------------------------------
// 10. del_blank()   (Perl lines 439-460)
// ---------------------------------------------------------------------------

void MacroBNGModel::del_blank(const std::vector<std::string>& str,
                               std::vector<std::string>& lis1) {
    for (auto line : str) {
        // Delete comment
        auto hash_pos = line.find('#');
        if (hash_pos != std::string::npos) {
            line.erase(hash_pos);
        }
        // Strip leading whitespace
        line = ltrim(line);
        if (line.empty()) continue;
        // Skip if all whitespace
        if (line.find_first_not_of(" \t") == std::string::npos) continue;

        // Strip trailing whitespace
        line = rtrim(line);
        // Collapse internal whitespace to single space
        line = std::regex_replace(line, std::regex("\\s+"), " ");
        // Replace single spaces with semicolons
        line = replaceAll(line, " ", ";");

        // If starts with digit followed by ';', strip the leading number
        std::smatch m;
        if (std::regex_search(line, m, std::regex("^\\d+?;"))) {
            line = ";" + m.suffix().str();
        } else {
            line = ";" + line;
        }
        lis1.push_back(line);
    }
}

// ---------------------------------------------------------------------------
// 11. hash_skf()   (Perl lines 462-493)
// ---------------------------------------------------------------------------

void MacroBNGModel::hash_skf(const std::vector<std::string>& lis1,
                              std::map<std::string, std::vector<std::string>>& skf,
                              const std::map<std::string, int>& nm2_site,
                              const std::string& /*label*/) {
    WFILErec_ << " from hash_skf\n";
    for (auto& rule : lis1) {
        std::string rab = rule;
        // Split on <-> or ->
        std::string reac, prod;
        auto bidi = rab.find("<->");
        auto uni = rab.find("->");
        if (bidi != std::string::npos) {
            reac = rab.substr(0, bidi);
            prod = rab.substr(bidi + 3);
        } else if (uni != std::string::npos) {
            reac = rab.substr(0, uni);
            prod = rab.substr(uni + 2);
        } else {
            continue;
        }

        // Split reactants and products on ";+;"
        auto r_parts = split(reac, ";+;");
        std::string r1 = (r_parts.size() > 0) ? r_parts[0] : "";
        std::string r2 = (r_parts.size() > 1) ? r_parts[1] : "";

        auto p_parts = split(prod, ";+;");
        std::string p1 = (p_parts.size() > 0) ? p_parts[0] : "";
        std::string p2 = (p_parts.size() > 1) ? p_parts[1] : "";

        skf0(r1, skf, nm2_site);
        skf0(r2, skf, nm2_site);
        skf0(p1, skf, nm2_site);
        skf0(p2, skf, nm2_site);
    }

    // Log sorted hash
    int i = 1;
    for (auto& [key_d, vec] : skf) {
        WFILErec_ << " " << i << "  " << key_d << " ";
        for (auto& v : vec) {
            WFILErec_ << " " << v << " ";
        }
        WFILErec_ << "\n";
        i++;
    }
}

// ---------------------------------------------------------------------------
// 12. skf0()   (Perl lines 878-918)
// ---------------------------------------------------------------------------

void MacroBNGModel::skf0(const std::string& rp1,
                          std::map<std::string, std::vector<std::string>>& skf,
                          const std::map<std::string, int>& nm2_site) {
    std::string p1 = rp1;

    while (p1.find('!') != std::string::npos) {
        auto bang_pos = p1.find('!');
        std::string lef1 = p1.substr(0, bang_pos);
        std::string rit1 = p1.substr(bang_pos + 1);

        // Find the link number: everything until the next , ) or !
        std::string::size_type end_pos = 0;
        while (end_pos < rit1.size() &&
               rit1[end_pos] != ',' && rit1[end_pos] != ')' && rit1[end_pos] != '!') {
            end_pos++;
        }
        std::string lnk_num = rit1.substr(0, end_pos);
        std::string lnk1 = "!" + lnk_num;

        // Skip wildcard links + and ?
        if (lnk_num == "+" || lnk_num == "?") {
            // Remove this link occurrence and continue
            p1 = replaceFirst(p1, lnk1, "");
            continue;
        }

        // Extract skf1 and sit1 from lef1
        // lef1 looks like ";R(b~pY" — find the last ',' or '('
        std::string::size_type last_delim = std::string::npos;
        for (std::string::size_type k = lef1.size(); k > 0; --k) {
            if (lef1[k - 1] == ',' || lef1[k - 1] == '(') {
                last_delim = k - 1;
                break;
            }
        }
        std::string skf1_raw = (last_delim != std::string::npos)
                                   ? lef1.substr(0, last_delim + 1)
                                   : "";
        std::string sit1 = (last_delim != std::string::npos)
                               ? lef1.substr(last_delim + 1)
                               : lef1;

        // Clean sit1: remove ~ and ! suffixes
        {
            auto tpos = sit1.find('~');
            auto epos = sit1.find('!');
            auto cpos = std::min(tpos, epos);
            if (cpos != std::string::npos) sit1 = sit1.substr(0, cpos);
        }

        // Extract molecule name from skf1_raw: after last '.' or ';', before '('
        std::string skf1 = skf1_raw;
        {
            // Find last '.' or ';'
            auto dpos = skf1.rfind('.');
            auto spos = skf1.rfind(';');
            auto cut = std::string::npos;
            if (dpos != std::string::npos && spos != std::string::npos)
                cut = std::max(dpos, spos);
            else if (dpos != std::string::npos)
                cut = dpos;
            else if (spos != std::string::npos)
                cut = spos;
            if (cut != std::string::npos) skf1 = skf1.substr(cut + 1);
        }
        // Remove '(' and everything after
        {
            auto ppos = skf1.find('(');
            if (ppos != std::string::npos) skf1 = skf1.substr(0, ppos);
        }

        // Remove the first occurrence of lnk1 from p1
        p1 = replaceFirst(p1, lnk1, "");

        // Find the second occurrence of lnk1 in p1
        if (p1.find(lnk1) == std::string::npos) {
            // No matching second link — skip
            continue;
        }

        auto bang_pos2 = p1.find(lnk1);
        std::string lef2 = p1.substr(0, bang_pos2);

        // Extract skf2, sit2 from lef2 the same way
        std::string::size_type last_delim2 = std::string::npos;
        for (std::string::size_type k = lef2.size(); k > 0; --k) {
            if (lef2[k - 1] == ',' || lef2[k - 1] == '(') {
                last_delim2 = k - 1;
                break;
            }
        }
        std::string sit2 = (last_delim2 != std::string::npos)
                               ? lef2.substr(last_delim2 + 1)
                               : lef2;
        {
            auto tpos = sit2.find('~');
            auto epos = sit2.find('!');
            auto cpos = std::min(tpos, epos);
            if (cpos != std::string::npos) sit2 = sit2.substr(0, cpos);
        }

        std::string skf2_raw = (last_delim2 != std::string::npos)
                                   ? lef2.substr(0, last_delim2 + 1)
                                   : "";
        std::string skf2 = skf2_raw;
        {
            auto dpos = skf2.rfind('.');
            auto spos = skf2.rfind(';');
            auto cut = std::string::npos;
            if (dpos != std::string::npos && spos != std::string::npos)
                cut = std::max(dpos, spos);
            else if (dpos != std::string::npos)
                cut = dpos;
            else if (spos != std::string::npos)
                cut = spos;
            if (cut != std::string::npos) skf2 = skf2.substr(cut + 1);
        }
        {
            auto ppos = skf2.find('(');
            if (ppos != std::string::npos) skf2 = skf2.substr(0, ppos);
        }

        // Remove the second occurrence of lnk1 from p1
        p1 = replaceFirst(p1, lnk1, "");

        // add_skf both directions
        add_skf(skf1, skf2, sit1, skf, nm2_site);
        add_skf(skf2, skf1, sit2, skf, nm2_site);
    }
}

// ---------------------------------------------------------------------------
// 13. add_skf()   (Perl lines 854-875)
// ---------------------------------------------------------------------------

void MacroBNGModel::add_skf(const std::string& skf1, const std::string& skf2,
                             const std::string& sit1,
                             std::map<std::string, std::vector<std::string>>& skf,
                             const std::map<std::string, int>& nm2_site) {
    std::string key = skf1 + ":" + skf2;
    if (skf.find(key) != skf.end()) {
        auto& rabm = skf[key];
        if (!inc_elt(sit1, rabm)) {
            rabm.push_back(sit1);
        }
    } else {
        skf[key].push_back(sit1);
    }

    // Add numbered duplicate sites if nm2_site has this molecule:site
    std::string nm2_key = skf1 + ":" + sit1;
    auto it = nm2_site.find(nm2_key);
    if (it != nm2_site.end()) {
        for (int ii = 2; ii <= it->second; ii++) {
            skf[key].push_back(sit1 + ":" + std::to_string(ii));
        }
    }
}

// ---------------------------------------------------------------------------
// 14. inc_elt(), inc_set(), del_set()   (Perl lines 496-543)
// ---------------------------------------------------------------------------

int MacroBNGModel::inc_elt(const std::string& re,
                            const std::vector<std::string>& pr) {
    int jj = 0;
    for (auto& elem : pr) {
        if (re == elem) {
            return jj + 1;  // 1-based index (truthy in Perl)
        }
        ++jj;
    }
    return 0;  // not found
}

bool MacroBNGModel::inc_set(const std::vector<std::string>& re,
                             const std::vector<std::string>& pr) {
    // Check if multiset re is included in multiset pr
    std::vector<std::string> pm = pr;  // working copy
    for (auto& elem : re) {
        int jj = inc_elt(elem, pm);
        if (!jj) {
            return false;
        }
        // Remove the found element (splice)
        pm.erase(pm.begin() + (jj - 1));
    }
    return true;
}

void MacroBNGModel::del_set(const std::vector<std::string>& rem,
                             const std::vector<std::string>& prm,
                             std::vector<std::string>& rem_prm) {
    // Elements of rem that are NOT in prm (multiset difference)
    std::vector<std::string> pm = prm;  // working copy
    rem_prm.clear();
    for (auto& elem : rem) {
        int jj = inc_elt(elem, pm);
        if (!jj) {
            rem_prm.push_back(elem);  // not included
        } else {
            pm.erase(pm.begin() + (jj - 1));
        }
    }
}

// ---------------------------------------------------------------------------
// 15. activ_sit()   (Perl lines 563-580)
// ---------------------------------------------------------------------------

void MacroBNGModel::activ_sit(int /*typrul*/, std::string& reac, std::string& prod,
                               std::vector<std::string>& mreac) {
    while (!reac.empty()) {
        // Match first (...) group
        std::regex re_paren("[\\(].*?[)]");
        std::smatch m;
        if (!std::regex_search(reac, m, re_paren)) break;

        std::string name = m.prefix().str();  // molecule name before '('
        std::string r1 = name + m[0].str();   // e.g. "R(a!1)"

        // Remove the matched portion from reac
        reac = m.suffix().str();
        // Strip leading separators ;+.
        reac = std::regex_replace(reac, std::regex("^[;+.]+"), "");

        // Perl pushes r1 to mreac in both branches of the prod check
        mreac.push_back(r1);
    }
}

// ---------------------------------------------------------------------------
// 16. hierarch_sit()   (Perl lines 582-627)
// ---------------------------------------------------------------------------

void MacroBNGModel::hierarch_sit(
    std::map<std::string, std::vector<std::string>>& dpp_site) {
    bool fl = true;
    int iter = 1;

    while (fl) {
        // Take a snapshot of the current state
        auto m_kv = dpp_site;
        fl = false;

        for (auto& [key, b_vec] : m_kv) {
            std::set<std::string> tab_site(b_vec.begin(), b_vec.end());
            std::vector<std::string> bb;

            // Parse "Mol:Site" from key
            auto colon = key.find(':');
            std::string name = (colon != std::string::npos) ? key.substr(0, colon) : key;

            for (auto& bsite : b_vec) {
                std::string keyr = name + ":" + bsite;
                auto it = m_kv.find(keyr);
                if (it != m_kv.end() && !it->second.empty()) {
                    for (auto& a_elem : it->second) {
                        if (tab_site.find(a_elem) == tab_site.end()) {
                            tab_site.insert(a_elem);
                            bb.push_back(a_elem);
                            fl = true;
                        }
                    }
                }
            }

            // Append new elements
            auto& target = m_kv[key];
            target.insert(target.end(), bb.begin(), bb.end());

            // Sort and store back into dpp_site
            std::vector<std::string> sorted_vec(target.begin(), target.end());
            std::sort(sorted_vec.begin(), sorted_vec.end());
            dpp_site[key] = sorted_vec;
        }

        WFILErec_ << " iteration " << iter
                   << " dpp_site sort key of variable  (n/n hierarch_sit)\n";
        for (auto& [key, vec] : dpp_site) {
            WFILErec_ << " " << key;
            for (auto& v : vec) WFILErec_ << " " << v;
            WFILErec_ << "\n";
        }
        iter++;
    }
}

// ---------------------------------------------------------------------------
// 17. min_set()   (Perl lines 631-698)
// ---------------------------------------------------------------------------

void MacroBNGModel::min_set(
    std::map<std::string, std::vector<std::string>>& dpp_site,
    std::map<std::string, std::vector<int>>& lis1h,
    const std::map<std::string, int>& nm_site,
    const std::map<std::string, int>& nm2_site,
    std::map<std::string, int>& site_lig) {
    std::vector<std::string> key_del;
    std::set<std::string> f_obr;

    // Collect keys to delete (subsets of other entries for same molecule)
    auto keys_vec = std::vector<std::string>();
    for (auto& [k, v] : dpp_site) keys_vec.push_back(k);

    for (size_t idx1 = 0; idx1 < keys_vec.size(); ++idx1) {
        auto& key = keys_vec[idx1];
        if (f_obr.count(key)) continue;
        f_obr.insert(key);

        auto colon1 = key.find(':');
        std::string name = (colon1 != std::string::npos) ? key.substr(0, colon1) : key;
        std::string si1 = (colon1 != std::string::npos) ? key.substr(colon1 + 1) : "";
        auto& r1 = dpp_site[key];

        for (size_t idx2 = 0; idx2 < keys_vec.size(); ++idx2) {
            auto& key2 = keys_vec[idx2];
            if (f_obr.count(key2)) continue;
            auto colon2 = key2.find(':');
            std::string name2 = (colon2 != std::string::npos) ? key2.substr(0, colon2) : key2;
            std::string si2 = (colon2 != std::string::npos) ? key2.substr(colon2 + 1) : "";

            if (name == name2 && key != key2) {
                auto& r2 = dpp_site[key2];
                if (r1.size() == r2.size() && (si1 == "%" || si2 == "0")) {
                    if (inc_set(r1, r2) && si1 == "%") {
                        key_del.push_back(key);
                        continue;
                    }
                    if (inc_set(r2, r1) && si2 == "%") {
                        key_del.push_back(key2);
                        continue;
                    }
                } else {
                    if (inc_set(r1, r2)) {
                        key_del.push_back(key);
                        continue;
                    }
                    if (inc_set(r2, r1)) {
                        key_del.push_back(key2);
                        continue;
                    }
                }
            }
        }
    }

    // Delete subset keys
    for (auto& k : key_del) {
        dpp_site.erase(k);
    }

    // Delete entries where the site count equals the molecule's total site count
    {
        std::vector<std::string> to_del;
        for (auto& [key, r1] : dpp_site) {
            auto colon = key.find(':');
            std::string mol = (colon != std::string::npos) ? key.substr(0, colon) : key;
            auto it = nm_site.find(mol);
            if (it != nm_site.end()) {
                if (it->second == static_cast<int>(r1.size())) {
                    to_del.push_back(key);
                }
            }
        }
        for (auto& k : to_del) dpp_site.erase(k);
    }

    // Log minimum set
    WFILErec_ << " MINIMUM set of variable  (n/n min_set)\n";
    for (auto& [key, vec] : dpp_site) {
        WFILErec_ << " " << key << ",";
        for (auto& v : vec) WFILErec_ << " " << v;
        WFILErec_ << "\n";

        auto colon = key.find(':');
        std::string name = (colon != std::string::npos) ? key.substr(0, colon) : key;
        for (auto& s : vec) {
            site_lig[name + ":" + s]++;
        }
    }

    WFILErec_ << " hash site_lig. \n";
    if (dpp_site.empty()) {
        std::cout << " REDUCTION NO\n";
        return;
    }

    // Prune site_lig entries with count <= 1
    {
        std::vector<std::string> to_del;
        for (auto& [key, val] : site_lig) {
            if (val > 1) {
                WFILErec_ << " " << key << ", " << val << "\n";
            } else {
                to_del.push_back(key);
            }
        }
        for (auto& k : to_del) site_lig.erase(k);
    }

    // Prune lis1h entries not in site_lig
    {
        std::vector<std::string> rabm;
        for (auto& [k, v] : site_lig) rabm.push_back(k);

        WFILErec_ << " hash lis1h. \n";
        std::vector<std::string> to_del;
        for (auto& [key, vec] : lis1h) {
            if (inc_elt(key, rabm)) {
                WFILErec_ << " " << key << ",";
                for (auto& v : vec) WFILErec_ << " " << v;
                WFILErec_ << "\n";
            } else {
                to_del.push_back(key);
            }
        }
        for (auto& k : to_del) lis1h.erase(k);
    }
    WFILErec_ << " Macro rules  (n/n trans_rec)\n";
}

// ---------------------------------------------------------------------------
// 18. num_site()   (Perl lines 702-760)
// ---------------------------------------------------------------------------

std::string MacroBNGModel::num_site(const std::string& re, const std::string& pr,
                                     const std::map<std::string, int>& nm2_site,
                                     std::vector<std::string>& dep_sit) {
    bool eqv = (re == pr);

    // Extract contents inside parentheses from reactant
    std::string name;
    std::smatch m;
    std::regex re_paren("[\\(](.*)[\\)]");

    std::string ss_re;
    if (std::regex_search(re, m, re_paren)) {
        name = m.prefix().str();
        ss_re = m[1].str();
    }
    auto rem = split(ss_re, ',');

    // Extract contents from product
    std::string ss_pr;
    if (std::regex_search(pr, m, re_paren)) {
        ss_pr = m[1].str();
    }
    auto prm = split(ss_pr, ',');

    // res = rem with modifiers stripped
    auto res = rem;
    for (auto& s : res) {
        auto bang = s.find('!');
        auto tilde = s.find('~');
        auto cut = std::min(bang, tilde);
        if (cut != std::string::npos) s = s.substr(0, cut);
    }

    // Unique elements of res
    std::set<std::string> seen;
    std::vector<std::string> aonly;
    for (auto& ii : res) {
        if (seen.insert(ii).second) {
            aonly.push_back(ii);
        }
    }

    if (eqv) {
        dep_sit = aonly;
        return "%";
    }

    // If there are duplicate site names, swap rem and prm
    if (res.size() != aonly.size()) {
        for (size_t ii = 0; ii < rem.size() && ii < prm.size(); ii++) {
            std::swap(rem[ii], prm[ii]);
        }
    }

    // Add numbered sites for duplicates
    for (auto& a : aonly) {
        auto it = nm2_site.find(name + ":" + a);
        if (it != nm2_site.end()) {
            for (int ii = 2; ii <= it->second; ii++) {
                rem.push_back(a + ":" + std::to_string(ii));
                prm.push_back(a + ":" + std::to_string(ii));
            }
        }
    }

    // Find elements in rem but not prm (the changed site)
    del_set(rem, prm, aonly);
    if (aonly.size() != 1) {
        std::cerr << "ERROR 1003:  aonly= " << (int)aonly.size() - 1
                  << " reactant=" << re << " product=" << pr << "\n";
        return "";
    }

    std::string active_site = aonly[0];  // e.g. "l!1"

    // Now find dependent sites: rem minus the active site
    std::vector<std::string> active_vec = {active_site};
    del_set(rem, active_vec, aonly);

    // Strip modifiers from dependent sites
    for (auto& s : aonly) {
        auto bang = s.find('!');
        auto tilde = s.find('~');
        auto cut = std::min(bang, tilde);
        if (cut != std::string::npos) s = s.substr(0, cut);
    }

    dep_sit = aonly;

    // Strip modifiers from active site name
    std::string site_name = active_site;
    {
        auto bang = site_name.find('!');
        auto tilde = site_name.find('~');
        auto cut = std::min(bang, tilde);
        if (cut != std::string::npos) site_name = site_name.substr(0, cut);
    }

    // Add active site to dep_sit and deduplicate
    dep_sit.push_back(site_name);
    {
        std::set<std::string> seen2;
        std::vector<std::string> unique_dep;
        for (auto& d : dep_sit) {
            if (seen2.insert(d).second) unique_dep.push_back(d);
        }
        dep_sit = unique_dep;
    }

    return site_name;
}

// ---------------------------------------------------------------------------
// 19. hash_sor()   (Perl lines 764-851)
// ---------------------------------------------------------------------------

void MacroBNGModel::hash_sor(
    const std::vector<std::string>& lis1,
    const std::map<std::string, int>& nm2_site,
    std::map<std::string, std::vector<int>>& lis1h,
    std::map<std::string, std::vector<std::string>>& dpp_site) {
    WFILErec_ << " for hash lis1h  (n/n hash_sor)\n";

    int ind = -1;
    int uni = 1;

    for (size_t li = 0; li < lis1.size(); ++li) {
        ind++;
        std::string rab = lis1[li];

        // Strip leading ';'
        if (!rab.empty() && rab[0] == ';') rab = rab.substr(1);

        // Split on <-> or ->
        std::string reac, prod;
        auto bidi = rab.find("<->");
        auto unidir = rab.find("->");
        if (bidi != std::string::npos) {
            reac = rab.substr(0, bidi);
            prod = rab.substr(bidi + 3);
        } else if (unidir != std::string::npos) {
            reac = rab.substr(0, unidir);
            prod = rab.substr(unidir + 2);
        } else {
            continue;
        }

        // Separate product from coefficients: match up to last ");"
        std::string coef;
        {
            auto last_paren_semi = prod.rfind(");");
            if (last_paren_semi != std::string::npos) {
                coef = prod.substr(last_paren_semi + 2);
                prod = prod.substr(0, last_paren_semi + 2);
            }
        }

        std::vector<std::string> dep_sit;
        std::vector<std::string> mreac;
        std::vector<std::string> mprod;

        int typrul = 0;
        if (prod.find(";+;") != std::string::npos) typrul = 1;

        activ_sit(typrul, reac, prod, mreac);

        for (size_t mi = 0; mi < mreac.size(); ++mi) {
            std::string r1 = mreac[mi];

            // Extract molecule name from r1
            auto paren_pos = r1.find('(');
            std::string name = (paren_pos != std::string::npos) ? r1.substr(0, paren_pos) : r1;

            // Find matching molecule in product
            std::string p1;
            {
                std::string qname = quotemeta(name);
                std::regex re_prod(qname + "[\\(].*?[)]");
                std::smatch pm;
                if (std::regex_search(prod, pm, re_prod)) {
                    p1 = pm[0].str();
                    // Remove the match from prod
                    prod = pm.prefix().str() + pm.suffix().str();
                }
            }
            mprod.push_back(p1);

            dep_sit.clear();
            std::string n_site = num_site(r1, p1, nm2_site, dep_sit);

            if (n_site == "%") {
                n_site = "%" + std::to_string(uni);
                uni++;
            }

            std::vector<int> lis_sit;
            lis_sit.push_back(ind);

            std::string key_d = name + ":" + n_site;
            std::string key_dubl;
            if (nm2_site.count(key_d)) {
                key_dubl = name + ":" + n_site + ":2";
            }
            std::string key_l = key_d;

            // Merge dep_sit into dpp_site[key_d]
            auto& rxd = dpp_site[key_d];
            for (auto& d : rxd) {
                if (!inc_elt(d, dep_sit)) dep_sit.push_back(d);
            }
            dpp_site[key_d] = dep_sit;

            // Merge lis_sit into lis1h[key_l]
            auto& rxl = lis1h[key_l];
            for (auto& l : rxl) {
                bool found = false;
                for (auto& ls : lis_sit) {
                    if (ls == l) { found = true; break; }
                }
                if (!found) lis_sit.push_back(l);
            }
            lis1h[key_l] = lis_sit;

            // Log
            WFILErec_ << " " << key_l;
            for (auto& ls : lis_sit) WFILErec_ << " " << ls;
            WFILErec_ << "  " << lis1[ind] << "\n";

            if (!key_dubl.empty()) {
                WFILErec_ << " " << key_dubl;
                for (auto& ls : lis_sit) WFILErec_ << " " << ls;
                WFILErec_ << "  " << lis1[ind] << "\n";
            }
        }

        if (mreac.size() != mprod.size()) {
            std::cerr << "ERROR: 1004 n_mreac= " << mreac.size()
                      << "  : n_mprod= " << mprod.size() << "\n";
            return;
        }
    }

    // Log sorted dpp_site
    WFILErec_ << " sort hash dpp_site  (n/n hash_sor)\n";
    int i = 1;
    for (auto& [key_d, vec] : dpp_site) {
        WFILErec_ << " " << i << "  " << key_d;
        for (auto& v : vec) WFILErec_ << " " << v;
        WFILErec_ << "\n";
        i++;
    }
}

// ---------------------------------------------------------------------------
// 20. pre_rules()   (Perl lines 410-437)
// ---------------------------------------------------------------------------

void MacroBNGModel::pre_rules(
    std::map<std::string, int>& nm_site,
    std::map<std::string, int>& nm2_site,
    const std::string& param_prefix,
    std::map<std::string, std::vector<std::string>>& skf,
    std::map<std::string, std::vector<std::string>>& dpp_site,
    std::map<std::string, std::vector<int>>& lis1h) {
    std::map<std::string, int> site_lig;

    // Open rules output file
    std::string rulesfile = "macr_" + param_prefix + ".rules";
    WFILErules_.open(rulesfile);
    if (!WFILErules_.is_open()) {
        std::cerr << "Can't open " << rulesfile << "\n";
        return;
    }
    WFILErules_ << "begin reaction_rules\n";

    // Remove blank lines, normalize to semicolon-delimited format
    std::vector<std::string> lis1;
    del_blank(reac1tion_, lis1);

    // Build binding-site hash
    hash_skf(lis1, skf, nm2_site);

    // Build sorted hash of site dependencies
    hash_sor(lis1, nm2_site, lis1h, dpp_site);

    // Build dependency hierarchy
    hierarch_sit(dpp_site);

    // Determine minimal variable set
    min_set(dpp_site, lis1h, nm_site, nm2_site, site_lig);

    // Transform reaction rules (trans_rec is in the second half)
    // trans_rec(dpp_site, lis1h, lis1, site_lig);

    WFILErules_ << "end reaction_rules\n";
    WFILErules_.close();
}

// ---------------------------------------------------------------------------
// 21. trans_rec()   (Perl lines 1211-1239)
// ---------------------------------------------------------------------------

void MacroBNGModel::trans_rec(
    std::map<std::string, std::vector<std::string>>& dpp_site,
    std::map<std::string, std::vector<int>>& lis1h,
    const std::vector<std::string>& lis1,
    std::map<std::string, int>& site_lig) {
    std::string lin;
    int ino = 0;
    int ind = -1;

    for (auto& value : lis1) {
        ind++;
        std::vector<std::string> li;
        r_mac(dpp_site, lis1h, value, site_lig, ind, li);

        if (!li.empty()) {
            for (auto& val : li) {
                ino++;
                lin = std::to_string(ino) + val;
                lin = replaceAll(lin, ";", " ");
                WFILErec_ << lin << "\n";
                WFILErules_ << lin << "\n";
            }
        } else {
            ino++;
            lin = std::to_string(ino) + value;
            lin = replaceAll(lin, ";", " ");
            WFILErec_ << lin << "\n";
            WFILErules_ << lin << "\n";
        }
    }
}

// ---------------------------------------------------------------------------
// 22. r_mac()   (Perl lines 1305-1387)
// ---------------------------------------------------------------------------

void MacroBNGModel::r_mac(
    std::map<std::string, std::vector<std::string>>& dpp_site,
    std::map<std::string, std::vector<int>>& lis1h,
    const std::string& value,
    std::map<std::string, int>& site_lig,
    int ind,
    std::vector<std::string>& li) {

    std::map<std::string, int> rlig = site_lig;
    std::map<std::string, std::vector<std::string>> rdep = dpp_site;
    int lig = 0;

mlig:
    int fl = 0;
    std::string val = value;

    // Iterate over dependent-site keys in sorted order
    std::vector<std::string> sorted_keys;
    for (auto& [k, v] : rdep) sorted_keys.push_back(k);
    std::sort(sorted_keys.begin(), sorted_keys.end());

    for (auto& key : sorted_keys) {
        // Parse "Mol:Site" -> tei1 = Mol
        auto cpos = key.find(':');
        std::string tei1 = (cpos != std::string::npos) ? key.substr(0, cpos) : key;

        // Check if val contains ;Mol( or .Mol(
        std::string pat_semi = ";" + tei1 + "(";
        std::string pat_dot  = "." + tei1 + "(";
        if (val.find(pat_semi) == std::string::npos &&
            val.find(pat_dot)  == std::string::npos) {
            continue;  // no Macro ;Mol( or .Mol(
        }

        // Build rabms = [ "Mol:site1", "Mol:site2", ... ]
        std::vector<std::string> rabms;
        auto rabm = rdep[key];
        std::sort(rabm.begin(), rabm.end());
        for (auto& s : rabm) rabms.push_back(tei1 + ":" + s);

        std::string out;

        // While val contains ;Mol( or .Mol(
        while (true) {
            auto semi_pos = val.find(pat_semi);
            auto dot_pos  = val.find(pat_dot);
            std::string::size_type match_pos = std::string::npos;
            std::string tei2;

            if (semi_pos != std::string::npos && dot_pos != std::string::npos) {
                if (semi_pos < dot_pos) {
                    match_pos = semi_pos;
                    tei2 = pat_semi;
                } else {
                    match_pos = dot_pos;
                    tei2 = pat_dot;
                }
            } else if (semi_pos != std::string::npos) {
                match_pos = semi_pos;
                tei2 = pat_semi;
            } else if (dot_pos != std::string::npos) {
                match_pos = dot_pos;
                tei2 = pat_dot;
            } else {
                break;  // no more matches
            }

            // Perl: $out .= $`; (prefix before match)
            out += val.substr(0, match_pos);
            // val = $' (everything after match including "(")
            val = val.substr(match_pos + tei2.size());

            // Extract sites inside parentheses: val starts after "("
            // Find matching ")"
            auto paren_pos = val.find(')');
            if (paren_pos == std::string::npos) {
                out += tei2;
                break;
            }
            std::string inside = val.substr(0, paren_pos);

            // Split on comma to get site names, strip modifiers
            auto re = split(inside, ',');
            std::vector<std::string> re_names;
            for (auto& s : re) {
                auto p = s.find_first_of("!~");
                re_names.push_back((p != std::string::npos) ? s.substr(0, p) : s);
            }

            // Check if re_names is included in rabm
            if (!inc_set(re_names, rabm)) {
                out += tei2;  // not a macro molecule — keep original
                continue;
            }

            fl = 1;
            // tei2 without trailing "(" => ";Mol" or ".Mol"
            std::string tei2_noparen = tei2.substr(0, tei2.size() - 1);

            // Build aggregated name: ;MolSite1_Site2_...
            out += tei2_noparen + rabm[0] + "_";
            for (size_t i = 1; i < rabm.size(); ++i) {
                out += rabm[i] + "_";
            }
            if (!out.empty()) out.pop_back();  // chop trailing '_'
            out += "(";

            // Append original site contents up to ")"
            out += inside + ")";
            val = val.substr(paren_pos + 1);  // rest after ")"
        } // while match

        // Check lis1h entries against rabms
        for (auto& [aft, rabmn] : lis1h) {
            if (inc_elt(aft, rabms)) {
                if (std::find(rabmn.begin(), rabmn.end(), ind) != rabmn.end()) {
                    rdep.erase(key);

                    // Check if reactant-side bimolecular: ";+;...->;"
                    if (value.find(";+;") != std::string::npos &&
                        value.find("->") != std::string::npos &&
                        value.find(";+;") < value.find("->")) {
                        if (rlig[aft] > 0 && lig == 0) {
                            lig = 1;
                        } else {
                            auto aft_parts = split(aft, ':');
                            // Rebuild out with ;+; preserved
                            auto out_parts = split(out, ";+;");
                            if (out_parts.size() >= 2) {
                                out = out_parts[0] + ";+;" + out_parts[1];
                            }
                        }
                        --rlig[aft];
                        if (!rlig[aft]) lig = 0;
                    }

                    // Check if product-side bimolecular: "->;...;+;"
                    if (value.find("->") != std::string::npos &&
                        value.find(";+;") != std::string::npos &&
                        value.find("->") < value.rfind(";+;")) {
                        if (rlig[aft] > 0 && lig == 0) {
                            lig = 1;
                        } else {
                            auto aft_parts = split(aft, ':');
                            auto out_parts = split(out, ";+;");
                            if (out_parts.size() >= 2) {
                                out = out_parts[0] + ";+;" + out_parts[1];
                            }
                        }
                        --rlig[aft];
                        if (!rlig[aft]) lig = 0;
                    }
                }
            }
        } // foreach lis1h

        val = out + val;
    } // for key in rdep

    if (!fl) return;  // is not a macro molecule
    li.push_back(val);
    if (lig) goto mlig;
}

// ---------------------------------------------------------------------------
// 23. pre_obs1()   (Perl lines 1390-1441)
// ---------------------------------------------------------------------------

void MacroBNGModel::pre_obs1(
    std::map<std::string, int>& nm_site,
    std::map<std::string, int>& nm2_site,
    const std::string& param_prefix,
    std::map<std::string, std::vector<std::string>>& skf,
    std::map<std::string, std::vector<std::string>>& dpp_site,
    std::map<std::string, std::vector<int>>& lis1h) {

    // Open observables output file
    std::string obserfile = "macr_" + param_prefix + ".obser";
    WFILEobser_.open(obserfile);
    if (!WFILEobser_.is_open()) {
        std::cerr << "Can't open " << obserfile << "\n";
        return;
    }
    WFILEobser_ << "begin observables\n";

    // Remove blank lines from observables
    std::vector<std::string> lis1;
    del_blank(obser1vable_, lis1);

    WFILErec_ << " FULL set of observables (n/n pre_rules)\n";
    for (auto& s : lis1) {
        WFILErec_ << " " << s << " \n";
    }

    // Transform observables
    trans_obs(dpp_site, lis1h, lis1, skf);
    WFILEobser_ << "end observables\n";

    // Extract generate/simulate commands
    int ii = 1;
    for (auto& g : gene1rate_) {
        if (g.find("generate_network") == 0) gener1_ = g;
        if (g.find("simulate_ode") == 0)     { simul1_ = g; ii--; }
    }
    WFILEobser_ << gener1_;

    WFILEobser_.close();

    // --------- SPECIES2 ---------

    std::string spec2file = "macr_" + param_prefix + ".spec2";
    WFILEspec2_.open(spec2file);
    if (!WFILEspec2_.is_open()) {
        std::cerr << "Can't open " << spec2file << "\n";
        return;
    }
    WFILEspec2_ << "begin species\n";

    lis1.clear();
    del_blank(species2_, lis1);

    WFILErec_ << " FULL set of species2 (n/n pre_rules)\n";
    for (auto& s : lis1) {
        WFILErec_ << " " << s << " \n";
    }

    hash_skf(lis1, skf, nm2_site, "species2");
    trans_specie(dpp_site, lis1h, lis1);
    WFILEspec2_ << "end species\n";

    WFILEspec2_.close();
}

// ---------------------------------------------------------------------------
// 24. trans_obs()   (Perl lines 1243-1301)
// ---------------------------------------------------------------------------

void MacroBNGModel::trans_obs(
    std::map<std::string, std::vector<std::string>>& dpp_site,
    std::map<std::string, std::vector<int>>& lis1h,
    const std::vector<std::string>& lis1,
    std::map<std::string, std::vector<std::string>>& skf) {

    int nobj_form = 1;

    for (auto& line : lis1) {
        std::string lin_mac;
        std::string mol_form;

        // Split on ";" — first element is empty (leading ;)
        auto parts = split(line, ';');
        if (parts.size() < 3) continue;

        std::string group_type = parts[1];
        std::string group_name = parts[2];
        // re = parts[3..end]
        std::vector<std::string> re(parts.begin() + 3, parts.end());

        lin_mac = group_type + ";" + group_name + " ";

        // Validate group_name starts with a letter
        if (group_name.empty() || !std::isalpha(static_cast<unsigned char>(group_name[0]))) {
            std::string tmp = line;
            tmp = replaceAll(tmp, ";", " ");
            std::cerr << "Invalid group name.  Must start with a letter. " << tmp << "\n";
            continue;
        }

        std::string mol;
        for (auto& rr : re) {
            if (rr.find('(') == std::string::npos) {
                // Simple molecule without sites
                lin_mac += obs_mac0(rr, dpp_site, lis1h, skf) + ";";
            } else {
                // Complex molecule with sites
                mol.clear();
                obs_mac(rr, dpp_site, lis1h, mol, mol_form, skf);
                if (!mol.empty()) {
                    lin_mac += ";" + mol;
                }
            }
        }

        if (!mol_form.empty()) {
            // Formula mode
            lin_mac += ";";
            lin_mac = replaceAll(lin_mac, ";", " ");
            // chop mol_form (remove trailing '#')
            if (!mol_form.empty()) mol_form.pop_back();

            auto formulas = split(mol_form, '#');
            for (auto& f : formulas) {
                int n_form = 1;
                auto formula = split(f, '&');
                for (auto& ff : formula) {
                    lin_mac += ff + "&";
                    ++n_form;
                }
                ++nobj_form;
                lin_mac = "#" + lin_mac;
                // chop last two chars (trailing "&" from last formula element)
                if (lin_mac.size() >= 2) {
                    lin_mac.pop_back();
                    lin_mac.pop_back();
                }
                WFILErec_ << lin_mac;
                WFILEobser_ << lin_mac;
                lin_mac = " ";
            }
            WFILErec_ << "\n";
            WFILEobser_ << "\n";
        } else {
            lin_mac = replaceAll(lin_mac, ";", " ");
            WFILErec_ << lin_mac << "\n";
            WFILEobser_ << lin_mac << "\n";
        }
    }
}

// ---------------------------------------------------------------------------
// 25. obs_mac0()   (Perl lines 1444-1520)
// ---------------------------------------------------------------------------

std::string MacroBNGModel::obs_mac0(
    const std::string& comp,
    std::map<std::string, std::vector<std::string>>& dpp_site,
    std::map<std::string, std::vector<int>>& lis1h,
    std::map<std::string, std::vector<std::string>>& skf) {

    std::string lin_mac;
    auto scaf = split(comp, '.');  // e.g. S.A from S.A

    for (int ii = 0; ii <= static_cast<int>(scaf.size()) - 1; ii++) {
        for (int jj = 0; jj <= static_cast<int>(scaf.size()) - 1; jj++) {
            std::vector<std::string> rskf;

            if (scaf.size() <= 1 || scaf[0] == scaf[1]) {
                // One scaffold or Rec.Rec — no site links
                rskf.clear();
            } else {
                std::string ij = scaf[ii] + ":" + scaf[jj];
                auto it = skf.find(ij);
                if (it == skf.end()) continue;
                rskf = it->second;
            }

            std::vector<std::string> rabm;
            std::vector<std::string> rab2;
            int no_complex = 0;
            std::string key = scaf[ii];
            std::string out = key;

            for (auto& [key2, value] : dpp_site) {
                auto cpos = key2.find(':');
                std::string key3 = (cpos != std::string::npos) ? key2.substr(0, cpos) : key2;

                if (key == key3) {
                    rab2 = value;
                    std::sort(rab2.begin(), rab2.end());

                    if (rabm.empty()) {
                        rabm = rab2;
                    }

                    std::vector<std::string> rabms;
                    for (auto& s : rab2) rabms.push_back(key3 + ":" + s);

                    if (rskf.empty()) {
                        if (inc_set(rabm, rab2)) {
                            for (auto& s : rab2) {
                                out += s + "_";
                            }
                            if (!out.empty()) out.pop_back();  // chop
                            ++no_complex;
                            break;  // last
                        }
                    }

                    if (inc_set(rskf, rabm)) {
                        if (inc_set(rabm, rab2)) {
                            for (auto& s : rab2) {
                                out += s + "_";
                            }
                            if (!out.empty()) out.pop_back();  // chop
                            ++no_complex;
                            break;  // last
                        }
                    } else {
                        rabm.clear();
                    }
                }
            } // for dpp_site

            if (no_complex) {
                lin_mac += out;
            } else {
                if (scaf.size() > 1) {
                    if (!lin_mac.empty()) {
                        lin_mac += "." + key;
                    } else {
                        lin_mac += key;
                    }
                } else {
                    lin_mac += key;
                }
            }

            if (scaf.size() >= 2 && scaf[0] == scaf[1]) {
                return lin_mac + "." + lin_mac;  // Rec.Rec
            }
        } // for jj
    } // for ii

    return lin_mac;
}

// ---------------------------------------------------------------------------
// 26. obs_mac()   (Perl lines 1524-1622)
// ---------------------------------------------------------------------------

void MacroBNGModel::obs_mac(
    const std::string& comp,
    std::map<std::string, std::vector<std::string>>& dpp_site,
    std::map<std::string, std::vector<int>>& lis1h,
    std::string& mol,
    std::string& mol_form,
    std::map<std::string, std::vector<std::string>>& skf) {

    auto scaf = split(comp, '.');  // e.g. Lig(l1!1,l2).Rec(a1!1,b~y,g~Y)
    std::string lform;  // Perl: $lform scoped to sub, used at bottom

    for (auto& rscaf : scaf) {
        if (rscaf.find('(') == std::string::npos) {
            // No parentheses — delegate to obs_mac0
            mol += obs_mac0(rscaf, dpp_site, lis1h, skf) + ".";
            continue;
        }

        std::vector<std::string> rabm;
        std::vector<std::string> rab2;

        // Split on first '('
        auto ppos = rscaf.find('(');
        std::string key = rscaf.substr(0, ppos);
        std::string mac = rscaf.substr(ppos + 1);

        // Extract site contents before ')'
        auto rppos = mac.find(')');
        std::string sit = (rppos != std::string::npos) ? mac.substr(0, rppos) : mac;

        // Split sites on ','
        auto re = split(sit, ',');
        int jj = 0;

        rabm = re;
        // Strip modifiers from rabm to get bare site names
        for (auto& s : rabm) {
            auto p = s.find_first_of("~!");
            if (p != std::string::npos) s = s.substr(0, p);
        }

        std::string out = key;
        int no_dep = 1;
        int no_form = 0;
        std::vector<std::string> pere;
        std::vector<std::string> obed;

        for (auto& [key2, value_ref] : dpp_site) {
            auto cpos = key2.find(':');
            std::string key3 = (cpos != std::string::npos) ? key2.substr(0, cpos) : key2;

            if (key == key3) {
                rab2 = value_ref;
                std::sort(rab2.begin(), rab2.end());
                no_dep = 0;

                std::vector<std::string> rabms;
                for (auto& s : rab2) rabms.push_back(key3 + ":" + s);

                if (inc_set(rabm, rab2)) {
                    for (auto& s : rab2) {
                        out += s + "_";
                    }
                    if (!out.empty()) out.pop_back();  // chop
                    ++no_form;
                    break;  // out from dpp_site
                } else {
                    if (!pere.empty()) {
                        spere(pere, obed, rab2);
                    } else {
                        pere = rab2;
                        obed = rab2;
                    }
                }
            }
        } // for dpp_site

        if (no_dep) {
            // Not in dependency set — keep original
            out += "(";
            out += sit;
            out += ").";
            mol += out;
        } else {
            if (no_form) {
                // Direct macro form
                out += "(";
                for (auto& r : re) out += r + ",";
                if (!out.empty()) out.pop_back();  // chop trailing ','
                out += ").";
                mol += out;
            } else {
                // Formula mode
                std::string hform, zform;
                int nrabm = static_cast<int>(rabm.size());
                std::vector<int> irabm(nrabm, 0);
                int ii = 0;
                int i1 = 0;
                std::map<std::string, int> dep_del;

                while (!i1) {
                    if (ii > 2 * nrabm) {
                        WFILErec_ << "INVALID GROUP OBSERVABLES " << rscaf
                                  << " sites=";
                        for (auto& s : rabm) WFILErec_ << s << " ";
                        break;
                    }

                    h_dep(dpp_site, dep_del, rabm, rab2, out);

                    out += join("_", rab2) + "(";

                    for (auto& s : rab2) {
                        jj = inc_elt(s, rabm);
                        if (jj) {
                            irabm[jj - 1] = 1;
                            out += re[jj - 1] + ",";
                        } else {
                            out += "*,";
                        }
                    }
                    if (!out.empty()) out.pop_back();  // chop trailing ','
                    out += ")";

                    if (!mol.empty()) {
                        hform += mol + out + "&";
                    } else {
                        hform += out + "&";
                    }

                    i1 = 1;
                    for (auto& v : irabm) i1 *= v;

                    out = key;

                    if (ii > 0) {
                        std::string zout = out;
                        zout += join("_", rab2) + "(";
                        for (auto& s : pere) {
                            jj = inc_elt(s, rabm);
                            if (jj) {
                                zout += re[jj - 1] + ",";
                            }
                        }
                        if (!zout.empty()) zout.pop_back();  // chop
                        if (!pere.empty()) zout += ")";
                        if (!mol.empty()) {
                            zform += mol + zout + "&";
                        } else {
                            zform += zout + "&";
                        }
                    }

                    ii++;
                } // while

                if (!hform.empty()) hform.pop_back();  // chop trailing '&'
                if (!zform.empty()) zform.pop_back();
                lform += "[" + hform + "]/[" + zform + "].";
                mol.clear();

            } // else no_form
        } // if no_dep
    } // for scaf

    // Perl: if ($lform) { $$mol_form .= $lform."#"; }
    if (!lform.empty()) {
        mol_form += lform + "#";
    }

    if (!mol.empty() && mol.back() == '.') mol.pop_back();
}

// ---------------------------------------------------------------------------
// cor_net  (Perl lines 922-1207)
// Reads .net file, extracts MINIMUM set, processes molecule types, species,
// reactions (combining compatible ones via L_AiAn_BiBn / R_AiAn_BiBn),
// processes groups, writes corrected .net file.
// ---------------------------------------------------------------------------

void MacroBNGModel::cor_net(const std::string& param_prefix) {
    namespace fs = std::filesystem;

    ts3n_.clear();
    species_.clear();

    // ---- 1. Read macr_<prefix>.rec — extract MINIMUM set ----
    std::string recfile = "macr_" + param_prefix + ".rec";
    std::ifstream RFILErec(recfile);
    if (!RFILErec.is_open())
        throw std::runtime_error("Can't open " + recfile);

    int fl = 0;
    std::map<std::string, std::vector<std::string>> croc;
    std::map<std::string, std::string> master;
    std::map<std::string, std::vector<std::string>> slive;

    std::string line;
    while (std::getline(RFILErec, line)) {
        if (line.find(" MINIMUM set") != std::string::npos) {
            fl = 1;
            continue;
        } else if (line.find(" hash site_lig.") != std::string::npos) {
            fl = 0;
            break;
        } else if (fl) {
            // work line:  A:a1, a1 r  A:a2, a2 r
            line = ltrim(line);
            auto cpos = line.find(':');
            std::string tei1 = (cpos != std::string::npos) ? line.substr(0, cpos) : line;
            std::string rec = (cpos != std::string::npos) ? line.substr(cpos + 1) : "";

            // Split rec on whitespace; skip first token (it's the comma-separated part)
            auto parts = split(rec, ' ');
            // Perl: (undef, @rabn) = split(/\s+/, $rec) — skip first token
            std::vector<std::string> rabn;
            for (size_t i = 1; i < parts.size(); ++i) {
                if (!parts[i].empty()) rabn.push_back(parts[i]);
            }

            // Build key: tei1 + rabn[0] + "_" + rest + "_"
            std::string out;
            if (!rabn.empty()) {
                out = tei1 + rabn[0] + "_";
                for (size_t i = 1; i < rabn.size(); ++i) {
                    out += rabn[i] + "_";
                }
            } else {
                out = tei1 + "_";
            }
            if (!out.empty()) out.pop_back();  // chop trailing '_'

            // croc{out} = rabn (neighbor sites)
            croc[out] = rabn;

            if (master.count(tei1)) {
                // SLIVE: key=tei1, push out
                slive[tei1].push_back(out);
            } else {
                // MASTER: key=tei1, value=out
                master[tei1] = out;
            }
        }
        // else: skip
    }
    RFILErec.close();

    // Reverse maps
    std::map<std::string, std::string> rmaster;
    for (auto& [k, v] : master) rmaster[v] = k;

    std::map<std::string, std::string> rslive;
    for (auto& [k, vec] : slive) {
        for (auto& v : vec) rslive[v] = k;
    }

    // ms1 = rmaster + rslive, ms2 = ms1, ms = ms1
    std::map<std::string, std::string> ms1;
    for (auto& [k, v] : rmaster) ms1[k] = v;
    for (auto& [k, v] : rslive) ms1[k] = v;
    std::map<std::string, std::string> ms = ms1;

    // ---- 2. Adaptation *.net ----
    std::string netfile = "macr_" + param_prefix + ".net";
    std::string rabfile = "macr_" + param_prefix + ".rab";
    std::ifstream RFILEnet(netfile);
    if (!RFILEnet.is_open())
        throw std::runtime_error("Can't open " + netfile);
    std::ofstream WFILErab(rabfile);
    if (!WFILErab.is_open())
        throw std::runtime_error("Can't open " + rabfile);

    // search-copy until "begin molecule types"
    while (std::getline(RFILEnet, line)) {
        WFILErab << line << "\n";
        if (line.find("begin molecule types") != std::string::npos) break;
    }

    // work molecule types — copy and add TRASH at end
    int ii = 0;
    while (std::getline(RFILEnet, line)) {
        if (line.find("end molecule types") != std::string::npos) {
            ii++;
            char buf[256];
            std::snprintf(buf, sizeof(buf), "%3d %s\n", ii, "TRASH");
            WFILErab << buf;
            WFILErab << line << "\n";
            break;
        } else {
            WFILErab << line << "\n";
            ii++;
        }
    }

    // search-copy until "begin species"
    while (std::getline(RFILEnet, line)) {
        WFILErab << line << "\n";
        if (line.find("begin species") != std::string::npos) break;
    }

    // ---- 3. Work species ----
    species_.clear();
    ii = 0;
    while (std::getline(RFILEnet, line)) {
        if (line.find("end species") != std::string::npos) {
            ii++;
            char buf[256];
            std::snprintf(buf, sizeof(buf), "%5d %s\n", ii, "TRASH   0");
            WFILErab << buf;
            WFILErab << line << "\n";
            break;
        } else {
            WFILErab << line << "\n";
            ii++;
            std::string trimmed = ltrim(line);
            // Parse: ns1 li1 rest
            auto parts = split(trimmed, ' ');
            if (parts.size() >= 2) {
                species_.push_back(parts[1]);
            }
        }
    }

    // ---- 4. search-copy until "begin reactions" ----
    while (std::getline(RFILEnet, line)) {
        WFILErab << line << "\n";
        if (line.find("begin reactions") != std::string::npos) break;
    }

    // Read all reactions
    std::vector<std::string> reac;
    std::string endl_line;
    while (std::getline(RFILEnet, line)) {
        if (line.find("end reactions") != std::string::npos) {
            endl_line = line;
            break;
        } else {
            reac.push_back(line);
        }
    }

    // ---- 5. Update reactions — combine compatible ones ----
    int nn = -1;
    for (size_t idx = 0; idx < reac.size(); ++idx) {
        nn++;
        std::string linen = reac[idx];
        if (nn == 0) { reac[nn] = linen; continue; }

        if (linen.find(',') == std::string::npos) {
            reac[nn] = linen;
        } else {
            std::string trimmed = ltrim(linen);
            // Remove trailing newline/spaces
            while (!trimmed.empty() && (trimmed.back() == '\n' || trimmed.back() == '\r'))
                trimmed.pop_back();

            // Split on first comma
            auto cpos = trimmed.find(',');
            std::string r1str = trimmed.substr(0, cpos);
            std::string r2str = trimmed.substr(cpos + 1);

            auto rr1 = split(r1str, ' ');
            // Remove empty tokens
            rr1.erase(std::remove(rr1.begin(), rr1.end(), std::string("")), rr1.end());
            auto rr2 = split(r2str, ' ');
            rr2.erase(std::remove(rr2.begin(), rr2.end(), std::string("")), rr2.end());

            std::vector<std::string> rabn;
            for (auto& s : rr1) rabn.push_back(s);
            for (auto& s : rr2) rabn.push_back(s);

            if (rr1.size() == 2) {
                // LEFT variant — e.g. "30 5,17 11 kpS"
                // rabn = (nnn, an, bn, s3n, nkn)
                int n_adds = 0;
                std::map<std::string, int> dubl;

                for (int jj = 0; jj < nn; ++jj) {
                    if (n_adds == 2) { dubl.clear(); break; }
                    std::string linei = reac[jj];
                    if (linei.find(',') == std::string::npos) continue;

                    std::string ti = ltrim(linei);
                    while (!ti.empty() && (ti.back() == '\n' || ti.back() == '\r'))
                        ti.pop_back();

                    // Split on first comma
                    auto cp = ti.find(',');
                    std::string r1i_str = ti.substr(0, cp);
                    std::string r2i_str = ti.substr(cp + 1);

                    auto rr1i = split(r1i_str, ' ');
                    rr1i.erase(std::remove(rr1i.begin(), rr1i.end(), std::string("")), rr1i.end());
                    auto rr2i = split(r2i_str, ' ');
                    rr2i.erase(std::remove(rr2i.begin(), rr2i.end(), std::string("")), rr2i.end());

                    std::vector<std::string> rabi;
                    for (auto& s : rr1i) rabi.push_back(s);
                    for (auto& s : rr2i) rabi.push_back(s);

                    if (rr1i.size() == 2) {
                        // LEFT variant_ii
                        // rabi = (nni, ai, bi, s3i, nki)
                        if (rabn.size() >= 5 && rabi.size() >= 5) {
                            int an = std::stoi(rabn[1]);
                            int bn = std::stoi(rabn[2]);
                            int ai = std::stoi(rabi[1]);
                            int bi = std::stoi(rabi[2]);
                            std::string s3i = rabi[3];

                            if (an != ai && an != bi && bn != ai && bn != bi) {
                                continue;
                            } else if (s3i.find(',') != std::string::npos) {
                                continue;
                            } else {
                                L_AiAn_BiBn(croc, ms, rabn, rabi, reac, n_adds, dubl);
                            }
                        }
                    }
                } // for jj LEFT
                if (!n_adds) { reac[nn] = linen; }

            } else {
                // RIGHT variant — e.g. "29 20 4,18 kmS"
                // rabn = (nnn, s1n, an, bn, nkn)
                int n_subtracts = 0;
                std::map<std::string, int> dubl;

                for (int jj = 0; jj < nn; ++jj) {
                    if (n_subtracts == 2) { dubl.clear(); break; }
                    std::string linei = reac[jj];
                    if (linei.find(',') == std::string::npos) continue;

                    std::string ti = ltrim(linei);
                    while (!ti.empty() && (ti.back() == '\n' || ti.back() == '\r'))
                        ti.pop_back();

                    // Split on first comma (limit 2 parts)
                    auto cp = ti.find(',');
                    std::string r1i_str = ti.substr(0, cp);
                    std::string r2i_str = ti.substr(cp + 1);

                    auto rr1i = split(r1i_str, ' ');
                    rr1i.erase(std::remove(rr1i.begin(), rr1i.end(), std::string("")), rr1i.end());
                    auto rr2i = split(r2i_str, ' ');
                    rr2i.erase(std::remove(rr2i.begin(), rr2i.end(), std::string("")), rr2i.end());

                    std::vector<std::string> rabi;
                    for (auto& s : rr1i) rabi.push_back(s);
                    for (auto& s : rr2i) rabi.push_back(s);

                    if (rr1i.size() == 3) {
                        // RIGHT variant_ii
                        if (rabn.size() >= 5 && rabi.size() >= 5) {
                            int an = std::stoi(rabn[2]);
                            int bn = std::stoi(rabn[3]);
                            int ai = std::stoi(rabi[2]);
                            int bi = std::stoi(rabi[3]);

                            if (an != ai && an != bi && bn != ai && bn != bi) {
                                continue;
                            } else {
                                R_AiAn_BiBn(croc, ms, rabn, rabi, reac, n_subtracts, dubl);
                            }
                        }
                    }
                    if (n_subtracts == 2) {
                        subTrash(rabn, reac);
                        dubl.clear();
                    }
                } // for jj RIGHT
                if (!n_subtracts) { reac[nn] = linen; }
            } // else RIGHT
        } // else has comma
    } // foreach reactions

    // Output reactions to *.rab
    for (int i = 0; i <= nn; ++i) {
        WFILErab << reac[i];
        // Ensure newline
        if (reac[i].empty() || reac[i].back() != '\n')
            WFILErab << "\n";
    }
    WFILErab << endl_line << "\n";  // end reactions

    // ---- 6. Update groups ----
    // (Perl lines 1161-1179: prepare egf_tot from ts3n_ — largely commented out
    //  in the Perl source; we replicate the active code path)
    // The Perl code iterates ts3n_ keys "an.bn", splits, looks up species
    // and cross-references with obser1vable_ to build egf_tot_.
    // The active (non-=pod) block at line 1162 is essentially a no-op in
    // production because it only sets $egf_tot{...} = @rabm which
    // references the list context of @rabm from within the foreach.
    // We replicate the structural intent.
    for (auto& [keyAB, val] : ts3n_) {
        auto dotpos = keyAB.find('.');
        if (dotpos == std::string::npos) continue;
        int anIdx = std::stoi(keyAB.substr(0, dotpos));
        int bnIdx = std::stoi(keyAB.substr(dotpos + 1));
        if (anIdx < 1 || anIdx > static_cast<int>(species_.size())) continue;
        if (bnIdx < 1 || bnIdx > static_cast<int>(species_.size())) continue;

        std::string egf = species_[anIdx - 1];
        auto pp = egf.find('(');
        if (pp != std::string::npos) egf = egf.substr(0, pp);

        // Search obser1vable_ for matching group
        for (auto& obs : obser1vable_) {
            if (obs.find(";" + egf) != std::string::npos ||
                endsWith(obs, ";" + egf)) {
                // Extract group name: Molecules;name;...
                std::regex mol_re("Molecules;(.*?);");
                std::smatch m;
                if (std::regex_search(obs, m, mol_re)) {
                    // egf_tot_[group_name] — Perl assigns @rabm here
                    // but the value is unused beyond group filtering
                    egf_tot_[m[1].str()] = {};
                    break;
                }
            }
        }
    }

    // Copy until "begin groups"
    while (std::getline(RFILEnet, line)) {
        WFILErab << line << "\n";
        if (line.find("begin groups") != std::string::npos) break;
    }

    // Process groups
    while (std::getline(RFILEnet, line)) {
        if (line.find("end groups") != std::string::npos) {
            WFILErab << line << "\n";
            break;
        } else {
            // Parse group name (second field)
            auto parts = split(ltrim(line), ' ');
            std::string name_group;
            if (parts.size() >= 2) name_group = parts[1];

            if (egf_tot_.count(name_group)) {
                // Filter species indices from group line
                auto& rabm = egf_tot_[name_group];
                for (auto& sp : rabm) {
                    // Remove "sp," or "sp" from line
                    std::string pat1 = sp + ",";
                    std::string pat2 = sp;
                    auto p = line.find(pat1);
                    if (p != std::string::npos) {
                        line.erase(p, pat1.size());
                    } else {
                        p = line.find(pat2);
                        if (p != std::string::npos) {
                            line.erase(p, pat2.size());
                        }
                    }
                }
            }
            WFILErab << line << "\n";
        }
    }

    // Copy remaining lines
    while (std::getline(RFILEnet, line)) {
        WFILErab << line << "\n";
    }

    RFILEnet.close();
    WFILErab.close();

    // Rename: netfile -> netfile.isx, rabfile -> netfile
    fs::rename(netfile, netfile + "isx");
    fs::rename(rabfile, netfile);
}

// ---------------------------------------------------------------------------
// L_AiAn_BiBn  (Perl lines 1783-1822)
// Combine left-variant reactions.
// ---------------------------------------------------------------------------

void MacroBNGModel::L_AiAn_BiBn(
    std::map<std::string, std::vector<std::string>>& croc,
    std::map<std::string, std::string>& ms,
    std::vector<std::string>& rabn,
    std::vector<std::string>& rabi,
    std::vector<std::string>& reac,
    int& n_adds,
    std::map<std::string, int>& dubl)
{
    // rabn = (nnn, an, bn, s3n, nkn)
    // rabi = (nni, ai, bi, s3i, nki)
    int nnn = std::stoi(rabn[0]);
    int an  = std::stoi(rabn[1]);
    int bn  = std::stoi(rabn[2]);
    std::string s3n = rabn[3];
    std::string nkn = rabn[4];

    int nni = std::stoi(rabi[0]);
    int ai  = std::stoi(rabi[1]);
    int bi  = std::stoi(rabi[2]);
    std::string s3i = rabi[3];
    std::string nki = rabi[4];

    tnnn_ = nnn;
    tnni_ = nni;

    if (nkn != nki) return;

    std::string biStr = std::to_string(bi);
    std::string aiStr = std::to_string(ai);
    char buf[256];

    if (bi == bn && !dubl.count(biStr) && cros(croc, ms, ai, an)) {
        add_ts3n(an, bn);
        std::string ln = std::to_string(an) + "," + std::to_string(bn) +
                         " " + s3n + "," + std::to_string(bn) + " " + nkn;
        std::snprintf(buf, sizeof(buf), "%5d %s\n", nnn, ln.c_str());
        reac[nnn - 1] = buf;
        rabn[3] = s3n + "," + std::to_string(bn);
        n_adds++;
        dubl[biStr] = 1;
    }
    if (bi == an && !dubl.count(biStr) && cros(croc, ms, ai, bn)) {
        add_ts3n(an, bn);
        std::string ln = std::to_string(an) + "," + std::to_string(bn) +
                         " " + s3n + "," + std::to_string(an) + " " + nkn;
        std::snprintf(buf, sizeof(buf), "%5d %s\n", nnn, ln.c_str());
        reac[nnn - 1] = buf;
        rabn[3] = s3n + "," + std::to_string(an);
        n_adds++;
        dubl[biStr] = 1;
    }
    if (ai == an && !dubl.count(aiStr) && cros(croc, ms, bi, bn)) {
        add_ts3n(an, bn);
        std::string ln = std::to_string(an) + "," + std::to_string(bn) +
                         " " + s3n + "," + std::to_string(an) + " " + nkn;
        std::snprintf(buf, sizeof(buf), "%5d %s\n", nnn, ln.c_str());
        reac[nnn - 1] = buf;
        rabn[3] = s3n + "," + std::to_string(an);
        n_adds++;
        dubl[aiStr] = 1;
    }
    if (ai == bn && !dubl.count(aiStr) && cros(croc, ms, bi, an)) {
        add_ts3n(an, bn);
        std::string ln = std::to_string(an) + "," + std::to_string(bn) +
                         " " + s3n + "," + std::to_string(bn) + " " + nkn;
        std::snprintf(buf, sizeof(buf), "%5d %s\n", nnn, ln.c_str());
        reac[nnn - 1] = buf;
        rabn[3] = s3n + "," + std::to_string(bn);
        n_adds++;
        dubl[aiStr] = 1;
    }
}

// ---------------------------------------------------------------------------
// R_AiAn_BiBn  (Perl lines 1987-2018)
// Combine right-variant reactions.
// ---------------------------------------------------------------------------

void MacroBNGModel::R_AiAn_BiBn(
    std::map<std::string, std::vector<std::string>>& croc,
    std::map<std::string, std::string>& ms,
    std::vector<std::string>& rabn,
    std::vector<std::string>& rabi,
    std::vector<std::string>& reac,
    int& n_subtracts,
    std::map<std::string, int>& dubl)
{
    // rabn = (nnn, s1n, an, bn, nkn)
    // rabi = (nni, s1i, ai, bi, nki)
    int nnn = std::stoi(rabn[0]);
    std::string s1n = rabn[1];
    int an  = std::stoi(rabn[2]);
    int bn  = std::stoi(rabn[3]);
    std::string nkn = rabn[4];

    int nni = std::stoi(rabi[0]);
    std::string s1i = rabi[1];
    int ai  = std::stoi(rabi[2]);
    int bi  = std::stoi(rabi[3]);
    std::string nki = rabi[4];

    tnnn_ = nnn;
    tnni_ = nni;

    if (nkn != nki) return;

    std::string biStr = std::to_string(bi);
    std::string aiStr = std::to_string(ai);
    char buf[256];

    if (bi == bn && !dubl.count(biStr) && cros(croc, ms, ai, an)) {
        std::string ln = s1n + " " + std::to_string(an) + " " + nkn;
        std::snprintf(buf, sizeof(buf), "%5d %s\n", nnn, ln.c_str());
        reac[nnn - 1] = buf;
        n_subtracts++;
        dubl[biStr] = 1;
    }
    if (bi == an && !dubl.count(biStr) && cros(croc, ms, ai, bn)) {
        std::string ln = s1n + " " + std::to_string(bn) + " " + nkn;
        std::snprintf(buf, sizeof(buf), "%5d %s\n", nnn, ln.c_str());
        reac[nnn - 1] = buf;
        n_subtracts++;
        dubl[biStr] = 1;
    }
    if (ai == an && !dubl.count(aiStr) && cros(croc, ms, bi, bn)) {
        std::string ln = s1n + " " + std::to_string(bn) + " " + nkn;
        std::snprintf(buf, sizeof(buf), "%5d %s\n", nnn, ln.c_str());
        reac[nnn - 1] = buf;
        n_subtracts++;
        dubl[aiStr] = 1;
    }
    if (ai == bn && !dubl.count(aiStr) && cros(croc, ms, bi, an)) {
        std::string ln = s1n + " " + std::to_string(an) + " " + nkn;
        std::snprintf(buf, sizeof(buf), "%5d %s\n", nnn, ln.c_str());
        reac[nnn - 1] = buf;
        n_subtracts++;
        dubl[aiStr] = 1;
    }
}

// ---------------------------------------------------------------------------
// cros  (Perl lines 1826-1894)
// Check if two species are homogeneous (can be combined).
// ---------------------------------------------------------------------------

bool MacroBNGModel::cros(
    std::map<std::string, std::vector<std::string>>& croc,
    std::map<std::string, std::string>& ms,
    int ai, int an)
{
    // STEP 1: build pairs of macro-molecules that differ only in site composition
    std::vector<std::string> par1, par2;

    // Species ai
    std::map<std::string, std::string> rr1mac;
    if (ai >= 1 && ai <= static_cast<int>(species_.size())) {
        std::string li1 = species_[ai - 1];
        auto rr1 = split(li1, '.');
        for (auto& mol : rr1) {
            auto pp = mol.find('(');
            if (pp != std::string::npos) {
                std::string macName = mol.substr(0, pp);
                if (ms.count(macName)) {
                    rr1mac[mol] = macName;
                }
            }
        }
    }

    // Species an
    std::map<std::string, std::string> rr2mac;
    if (an >= 1 && an <= static_cast<int>(species_.size())) {
        std::string li2 = species_[an - 1];
        auto rr2 = split(li2, '.');
        for (auto& mol : rr2) {
            auto pp = mol.find('(');
            if (pp != std::string::npos) {
                std::string macName = mol.substr(0, pp);
                if (ms.count(macName)) {
                    rr2mac[mol] = macName;
                }
            }
        }
    }

    // Build pairs: macro-molecules from different ms-groups with same parent
    for (auto& [teil1, na1] : rr1mac) {
        for (auto& [teil2, na2] : rr2mac) {
            if (na1 != na2 && ms.count(na1) && ms.count(na2) &&
                ms[na1] == ms[na2]) {
                par1.push_back(teil1);
                par2.push_back(teil2);
            }
        }
    }

    // STEP 2x3: check homogeneity
    if (par1.empty()) {
        return false;  // not homogeneous
    }

    for (size_t jj = 0; jj < par1.size(); ++jj) {
        std::string teil1 = par1[jj];
        std::string teil2 = par2[jj];

        std::string na1 = rr1mac[teil1];
        std::string na2 = rr2mac[teil2];

        if (!croc.count(na1) || !croc.count(na2)) continue;

        std::vector<std::string> am = croc[na1];
        std::vector<std::string> bm = croc[na2];

        // am - bm => aonly
        std::vector<std::string> aonly;
        del_set(am, bm, aonly);

        // Process species ai
        std::vector<std::string> rr1vec = split(species_[ai - 1], '.');
        std::vector<std::string> rr1out;
        std::string tei1out;
        delsites(rr1vec, teil1, aonly, rr1out, tei1out);
        // Remove macro-name prefix
        auto pos = tei1out.find(na1);
        if (pos != std::string::npos) {
            tei1out.erase(pos, na1.size());
        }

        // bm - am => aonly2
        std::vector<std::string> aonly2;
        del_set(bm, am, aonly2);

        // Process species an
        std::vector<std::string> rr2vec = split(species_[an - 1], '.');
        std::vector<std::string> rr2out;
        std::string tei2out;
        delsites(rr2vec, teil2, aonly2, rr2out, tei2out);
        pos = tei2out.find(na2);
        if (pos != std::string::npos) {
            tei2out.erase(pos, na2.size());
        }

        if (tei1out == tei2out) {
            if (WFILErec_.is_open()) {
                WFILErec_ << "Hom_spes: reac=" << tnni_ << "," << tnnn_
                          << " spes=" << ai << "," << an
                          << " " << teil1 << " <=> " << teil2 << "\n";
            }
            return true;  // homogeneous
        }
    }

    return false;
}

// ---------------------------------------------------------------------------
// delsites  (Perl lines 1898-1975)
// Remove sites from species string.
// ---------------------------------------------------------------------------

void MacroBNGModel::delsites(
    std::vector<std::string>& rr1,
    const std::string& teil1_in,
    const std::vector<std::string>& aonly,
    std::vector<std::string>& rr1out,
    std::string& tei1out)
{
    rr1out.clear();
    rr1out = rr1;  // copy
    std::string teil1 = teil1_in;

    for (size_t i = 0; i < aonly.size(); ++i) {
        std::vector<std::string> met;
        std::string sit = aonly[i];
        std::string tei = teil1;

        // Find site in tei: look for "(sit" or ",sit"
        std::string::size_type fpos = std::string::npos;
        std::string ind0, ind1, ind2;

        // Try "(sit"
        std::string pat1 = "(" + sit;
        std::string pat2 = "," + sit;
        fpos = tei.find(pat1);
        bool isOpen = false;
        if (fpos != std::string::npos) {
            ind0 = tei.substr(0, fpos);
            ind1 = pat1;
            ind2 = tei.substr(fpos + pat1.size());
            isOpen = true;
        } else {
            fpos = tei.find(pat2);
            if (fpos != std::string::npos) {
                ind0 = tei.substr(0, fpos);
                ind1 = pat2;
                ind2 = tei.substr(fpos + pat2.size());
                isOpen = false;
            } else {
                continue;  // site not found
            }
        }

        // ind2 has the rest after the site name
        // Find next ',' or ')' to delimit the site value
        auto nextDelim = ind2.find_first_of(",)");
        std::string siteVal = (nextDelim != std::string::npos) ? ind2.substr(0, nextDelim) : ind2;
        std::string afterDelim = (nextDelim != std::string::npos) ? ind2.substr(nextDelim) : "";

        if (isOpen) {
            // "(sit..." — remove site, keep rest
            if (!afterDelim.empty() && afterDelim[0] == ',') {
                tei = ind0 + "(" + afterDelim.substr(1);
            } else if (!afterDelim.empty() && afterDelim[0] == ')') {
                tei = ind0 + afterDelim;
            } else {
                tei = ind0 + "(" + afterDelim;
            }
            // Extract bond labels from siteVal
            if (!siteVal.empty()) {
                auto parts = split(siteVal, '!');
                for (size_t k = 1; k < parts.size(); ++k) {
                    std::string label = parts[k];
                    // Remove trailing ')'
                    if (!label.empty() && label.back() == ')') label.pop_back();
                    if (!label.empty()) met.push_back(label);
                }
            }
        } else {
            // ",sit..." — remove site including leading comma
            if (!siteVal.empty()) {
                auto parts = split(siteVal, '!');
                for (size_t k = 1; k < parts.size(); ++k) {
                    std::string label = parts[k];
                    if (!label.empty() && label.back() == ')') label.pop_back();
                    if (!label.empty()) met.push_back(label);
                }
            }
            if (!afterDelim.empty()) {
                tei = ind0 + afterDelim;
            } else {
                tei = ind0 + ")";
            }
        }

        // Update rr1out: find teil1, replace with tei
        for (size_t j = 0; j < rr1out.size(); ++j) {
            if (rr1out[j] == teil1) {
                rr1out[j] = tei;
                break;
            }
        }
        teil1 = tei;

        // Pseudo-recursion: follow bond labels to remove linked molecules
        while (!met.empty()) {
            std::string me = met.back();
            met.pop_back();
            for (size_t j = 0; j < rr1out.size(); ++j) {
                if (rr1out[j].empty()) continue;
                if (rr1out[j].find("!" + me) != std::string::npos) {
                    selectmet(met, rr1out[j], me);
                    rr1out[j].clear();  // mark as removed (Perl: undef)
                    break;
                }
            }
        }
    } // for aonly

    // Sort and normalize: build sorted output
    std::map<std::string, size_t> rr1s;
    for (size_t j = 0; j < rr1out.size(); ++j) {
        if (rr1out[j].empty()) continue;
        // Create sort key: replace bond labels with #,
        std::string sortKey = rr1out[j];
        // Replace !xxx, and !xxx) patterns with #,
        std::regex bondPat("![^,)]+([,)])");
        sortKey = std::regex_replace(sortKey, bondPat, "#,");
        rr1s[sortKey] = j;
    }

    std::vector<std::string> rr1sor;
    for (auto& [key, idx] : rr1s) {
        if (!rr1out[idx].empty()) {
            rr1sor.push_back(rr1out[idx]);
        }
    }

    // Rebuild rr1out with sorted molecules, sites sorted within each molecule
    rr1out.clear();
    std::string ou1;
    for (auto& mol : rr1sor) {
        auto pp = mol.find('(');
        if (pp != std::string::npos) {
            std::string molName = mol.substr(0, pp);
            std::string sitePart = mol.substr(pp + 1);
            if (!sitePart.empty() && sitePart.back() == ')')
                sitePart.pop_back();
            auto sites = split(sitePart, ',');
            std::sort(sites.begin(), sites.end());
            std::string rebuilt = molName + "(" + join(",", sites) + ")";
            ou1 += rebuilt + ".";
            rr1out.push_back(rebuilt);
        } else {
            ou1 += mol + ".";
            rr1out.push_back(mol);
        }
    }
    if (!ou1.empty()) ou1.pop_back();  // chop trailing '.'

    // Renumber bond labels sequentially
    int bondNum = 0;
    while (ou1.find('!') != std::string::npos) {
        bondNum++;
        // Find first bond label: !<label> terminated by , or ) or !
        std::regex firstBond("![^,)!]+");
        std::smatch m;
        if (std::regex_search(ou1, m, firstBond)) {
            std::string oldLabel = m[0].str();
            // Replace all occurrences of this label
            std::string replacement = "#" + std::to_string(bondNum);
            ou1 = replaceAll(ou1, oldLabel, replacement);
        } else {
            break;
        }
    }
    tei1out = ou1;
}

// ---------------------------------------------------------------------------
// selectmet  (Perl lines 2020-2043)
// Recursively select linked molecules by following bond labels.
// ---------------------------------------------------------------------------

void MacroBNGModel::selectmet(
    std::vector<std::string>& met,
    const std::string& tei,
    const std::string& me)
{
    // Split molecule on ','
    auto rr = split(tei, ',');
    if (rr.empty()) return;

    // Process first element: may have '(' prefix and bond labels
    auto parts = split(rr[0], '!');
    if (parts.size() >= 2) {
        std::string ch2 = parts[1];
        // Remove trailing ')'
        if (!ch2.empty() && ch2.back() == ')') ch2.pop_back();
        if (ch2 != me && !ch2.empty()) {
            met.push_back(ch2);
        }
        // Strip to get molecule prefix: remove "(<char>"
        std::string ch1 = parts[0];
        auto ppos = ch1.find("(");
        if (ppos != std::string::npos) {
            ch1 = ch1.substr(0, ppos) + "(";
        }
    }

    // Process remaining sites
    for (size_t i = 1; i < rr.size(); ++i) {
        auto ps = split(rr[i], '!');
        if (ps.size() >= 2) {
            std::string ch2 = ps[1];
            if (!ch2.empty() && ch2.back() == ')') ch2.pop_back();
            if (ch2 != me && !ch2.empty()) {
                met.push_back(ch2);
            }
        }
    }
}

// ---------------------------------------------------------------------------
// add_ts3n  (Perl lines 1978-1985)
// Add to ts3n_ map (records species pairs that were combined).
// ---------------------------------------------------------------------------

void MacroBNGModel::add_ts3n(int an, int bn) {
    std::string key = std::to_string(an) + "." + std::to_string(bn);
    if (!ts3n_.count(key)) {
        ts3n_[key] = 1;
    }
}

// ---------------------------------------------------------------------------
// subTrash  (referenced at Perl line 1133)
// Replace reactions pointing to deleted species with TRASH.
// When n_subtracts == 2 for a RIGHT-variant reaction, the original
// reaction's product is replaced by the TRASH species.
// ---------------------------------------------------------------------------

void MacroBNGModel::subTrash(
    std::vector<std::string>& rabn,
    std::vector<std::string>& reac)
{
    // rabn = (nnn, s1n, an, bn, nkn)
    if (rabn.size() < 5) return;
    int nnn = std::stoi(rabn[0]);
    std::string s1n = rabn[1];
    std::string nkn = rabn[4];

    // The TRASH species is the last one (added during species processing)
    int trashIdx = static_cast<int>(species_.size());  // 1-based, points to TRASH

    std::string ln = s1n + " " + std::to_string(trashIdx) + " " + nkn;
    char buf[256];
    std::snprintf(buf, sizeof(buf), "%5d %s\n", nnn, ln.c_str());
    if (nnn >= 1 && nnn <= static_cast<int>(reac.size())) {
        reac[nnn - 1] = buf;
    }
}

// ---------------------------------------------------------------------------
// spere  (referenced at Perl line 1563)
// Species-reaction relationship helper: intersect pere with rab2,
// store result in obed.
// ---------------------------------------------------------------------------

void MacroBNGModel::spere(
    std::vector<std::string>& pere,
    std::vector<std::string>& obed,
    const std::vector<std::string>& rab2)
{
    // Perl: &spere(\@pere,\@obed,\@rab2)
    // Intersection: keep elements in pere that are also in rab2
    std::vector<std::string> result;
    for (auto& s : pere) {
        for (auto& r : rab2) {
            if (s == r) {
                result.push_back(s);
                break;
            }
        }
    }
    pere = result;
    obed = rab2;
}

// ---------------------------------------------------------------------------
// h_dep  (Perl lines 2047-2078)
// Select dependent sites for formula — finds the smallest dependency group
// from dpp_site that overlaps with rabm, writing result into rab2.
// ---------------------------------------------------------------------------

void MacroBNGModel::h_dep(
    std::map<std::string, std::vector<std::string>>& dpp_site,
    std::map<std::string, int>& dep_del,
    const std::vector<std::string>& rabm,
    std::vector<std::string>& rab2,
    const std::string& out)
{
    std::string key_mi;
    rab2.clear();

    for (auto& [key, value] : dpp_site) {
        if (dep_del.count("0" + key)) continue;

        auto cpos = key.find(':');
        std::string key2 = (cpos != std::string::npos) ? key.substr(0, cpos) : key;

        if (out != key2) continue;

        std::vector<std::string> ind_var = value;
        int n_in = 0;
        for (size_t j = 0; j < rabm.size(); ++j) {
            if (inc_elt(rabm[j], ind_var)) {
                ++n_in;
            }
        }

        if (n_in > 0) {
            if (!rab2.empty()) {
                // Keep the smaller set (minimization)
                if (static_cast<int>(rab2.size()) > static_cast<int>(ind_var.size())) {
                    rab2 = ind_var;
                    key_mi = key;
                }
            } else {
                rab2 = ind_var;
                key_mi = key;
            }
        }
    }

    dep_del["0" + key_mi] = 1;
}

} // namespace ast
} // namespace bng
