#include "BNGAstVisitor.hpp"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include <antlr4-runtime.h>

#include "generated/BNGLexer.h"
#include "generated/BNGParser.h"
#include "PatternGraphBuilder.hpp"

namespace bng::parser {

namespace {

std::string toLower(std::string value) {
    std::transform(value.begin(), value.end(), value.begin(), [](unsigned char c) {
        return static_cast<char>(std::tolower(c));
    });
    return value;
}

std::string stripQuotes(const std::string& value) {
    if (value.size() >= 2 && ((value.front() == '"' && value.back() == '"') || (value.front() == '\'' && value.back() == '\''))) {
        return value.substr(1, value.size() - 2);
    }
    return value;
}

std::string extractTrailingCompartment(BNGParser::Species_defContext* ctx) {
    if (ctx == nullptr) {
        return {};
    }
    const std::string text = ctx->getText();
    const auto at = text.rfind('@');
    if (at == std::string::npos || at + 1 >= text.size()) {
        return {};
    }
    const auto scope = text.find("::", at + 1);
    if (scope != std::string::npos) {
        return text.substr(at + 1, scope - (at + 1));
    }
    return text.substr(at + 1);
}

double parseNumberLiteral(std::string text) {
    std::replace(text.begin(), text.end(), 'D', 'E');
    std::replace(text.begin(), text.end(), 'd', 'e');
    return std::stod(text);
}

double evaluateExpression(const ast::Expression& expression, ast::ParameterList& parameters) {
    parameters.evaluateAll();
    return expression.evaluate([&](const std::string& dependency) {
        return parameters.evaluate(dependency);
    });
}

std::vector<std::string> collectParamNames(BNGParser::Param_listContext* ctx) {
    std::vector<std::string> names;
    if (ctx == nullptr) {
        return names;
    }
    for (auto* token : ctx->STRING()) {
        names.push_back(token->getText());
    }
    return names;
}

std::vector<std::string> collectStateNames(BNGParser::State_listContext* ctx) {
    std::vector<std::string> names;
    if (ctx == nullptr) {
        return names;
    }
    for (auto* state : ctx->state_name()) {
        names.push_back(state->getText());
    }
    return names;
}

std::string getMoleculeName(BNGParser::Molecule_defContext* ctx) {
    if (ctx->STRING() != nullptr) {
        return ctx->STRING()->getText();
    }
    if (ctx->keyword_as_mol_name() != nullptr) {
        return ctx->keyword_as_mol_name()->getText();
    }
    return ctx->getText();
}

std::string getComponentName(BNGParser::Component_defContext* ctx) {
    if (ctx->STRING() != nullptr) {
        return ctx->STRING()->getText();
    }
    if (ctx->INT() != nullptr) {
        return ctx->INT()->getText();
    }
    if (ctx->keyword_as_component_name() != nullptr) {
        return ctx->keyword_as_component_name()->getText();
    }
    return ctx->getText();
}

template <typename ContextT, typename ChildT>
ast::Expression buildLeftAssociativeExpression(
    ContextT* ctx,
    const std::function<ast::Expression(ChildT*)>& buildChild) {

    auto children = ctx->template getRuleContexts<ChildT>();
    ast::Expression expr = buildChild(children.front());
    for (std::size_t childIndex = 1, opIndex = 1; childIndex < ctx->children.size(); childIndex += 2, ++opIndex) {
        const std::string op = ctx->children[childIndex]->getText();
        expr = ast::Expression::binary(op, std::move(expr), buildChild(children[opIndex]));
    }
    return expr;
}

ast::Expression buildExpression(BNGParser::ExpressionContext* ctx);

ast::Expression buildPrimary(BNGParser::Primary_exprContext* ctx);

ast::Expression buildFunction(BNGParser::Function_callContext* ctx) {
    std::vector<ast::Expression> args;
    if (auto* list = ctx->expression_list()) {
        for (auto* expr : list->expression()) {
            args.push_back(buildExpression(expr));
        }
    }
    return ast::Expression::function(toLower(ctx->children.front()->getText()), std::move(args));
}

ast::Expression buildObservableRef(BNGParser::Observable_refContext* ctx) {
    std::vector<ast::Expression> args;
    if (auto* list = ctx->expression_list()) {
        for (auto* expr : list->expression()) {
            args.push_back(buildExpression(expr));
        }
    }
    return ast::Expression::observableRef(ctx->STRING()->getText(), std::move(args));
}

ast::Expression buildLiteral(BNGParser::LiteralContext* ctx) {
    if (ctx->INT() != nullptr) {
        return ast::Expression::number(parseNumberLiteral(ctx->INT()->getText()));
    }
    if (ctx->FLOAT() != nullptr) {
        return ast::Expression::number(parseNumberLiteral(ctx->FLOAT()->getText()));
    }
    if (ctx->PI() != nullptr) {
        return ast::Expression::number(std::acos(-1.0));
    }
    if (ctx->EULERIAN() != nullptr) {
        return ast::Expression::number(std::exp(1.0));
    }
    throw std::runtime_error("Unsupported literal expression");
}

ast::Expression buildPrimary(BNGParser::Primary_exprContext* ctx) {
    if (ctx->expression() != nullptr) {
        return buildExpression(ctx->expression());
    }
    if (ctx->function_call() != nullptr) {
        return buildFunction(ctx->function_call());
    }
    if (ctx->observable_ref() != nullptr) {
        return buildObservableRef(ctx->observable_ref());
    }
    if (ctx->literal() != nullptr) {
        return buildLiteral(ctx->literal());
    }
    if (ctx->arg_name() != nullptr) {
        return ast::Expression::identifier(ctx->arg_name()->getText());
    }
    throw std::runtime_error("Unsupported primary expression");
}

ast::Expression buildUnary(BNGParser::Unary_exprContext* ctx) {
    auto expr = buildPrimary(ctx->primary_expr());
    if (ctx->PLUS() != nullptr) {
        return ast::Expression::unary("+", std::move(expr));
    }
    if (ctx->MINUS() != nullptr) {
        return ast::Expression::unary("-", std::move(expr));
    }
    if (ctx->EMARK() != nullptr) {
        return ast::Expression::unary("!", std::move(expr));
    }
    if (ctx->TILDE() != nullptr) {
        return ast::Expression::unary("~", std::move(expr));
    }
    return expr;
}

ast::Expression buildPower(BNGParser::Power_exprContext* ctx) {
    return buildLeftAssociativeExpression<BNGParser::Power_exprContext, BNGParser::Unary_exprContext>(
        ctx, [](auto* child) { return buildUnary(child); });
}

ast::Expression buildMultiplicative(BNGParser::Multiplicative_exprContext* ctx) {
    return buildLeftAssociativeExpression<BNGParser::Multiplicative_exprContext, BNGParser::Power_exprContext>(
        ctx, [](auto* child) { return buildPower(child); });
}

ast::Expression buildAdditive(BNGParser::Additive_exprContext* ctx) {
    return buildLeftAssociativeExpression<BNGParser::Additive_exprContext, BNGParser::Multiplicative_exprContext>(
        ctx, [](auto* child) { return buildMultiplicative(child); });
}

ast::Expression buildEquality(BNGParser::Equality_exprContext* ctx) {
    return buildLeftAssociativeExpression<BNGParser::Equality_exprContext, BNGParser::Relational_exprContext>(
        ctx, [](auto* child) { return buildAdditive(child->additive_expr()); });
}

ast::Expression buildAnd(BNGParser::And_exprContext* ctx) {
    return buildLeftAssociativeExpression<BNGParser::And_exprContext, BNGParser::Equality_exprContext>(
        ctx, [](auto* child) { return buildEquality(child); });
}

ast::Expression buildOr(BNGParser::Or_exprContext* ctx) {
    return buildLeftAssociativeExpression<BNGParser::Or_exprContext, BNGParser::And_exprContext>(
        ctx, [](auto* child) { return buildAnd(child); });
}

ast::Expression buildExpression(BNGParser::ExpressionContext* ctx) {
    return buildOr(ctx->conditional_expr()->or_expr());
}

ast::Action buildActionFromArgs(const std::string& name, BNGParser::Action_argsContext* args) {
    ast::Action action;
    action.name = name;
    if (args == nullptr || args->action_arg_list() == nullptr) {
        return action;
    }

    for (auto* arg : args->action_arg_list()->action_arg()) {
        action.arguments[arg->arg_name()->getText()] = arg->action_arg_value()->getText();
    }
    return action;
}

ast::Expression parseExpressionImpl(const std::string& exprText) {
    antlr4::ANTLRInputStream input(exprText);
    BNGLexer lexer(&input);
    antlr4::CommonTokenStream tokens(&lexer);
    BNGParser parser(&tokens);

    auto* exprCtx = parser.expression();
    if (parser.getNumberOfSyntaxErrors() != 0) {
        throw std::runtime_error("Failed to parse expression: " + exprText);
    }

    return buildExpression(exprCtx);
}

} // namespace

BNGAstVisitor::BNGAstVisitor()
    : currentModel_(std::make_unique<ast::Model>()) {}

std::unique_ptr<ast::Model> BNGAstVisitor::takeModel() {
    return std::move(currentModel_);
}

std::any BNGAstVisitor::visitProg(BNGParser::ProgContext* ctx) {
    visitChildren(ctx);
    currentModel_->getParameters().evaluateAll();
    return {};
}

std::any BNGAstVisitor::visitVersion_def(BNGParser::Version_defContext* ctx) {
    currentModel_->setVersion(stripQuotes(ctx->quoted_string()->getText()));
    return {};
}

std::any BNGAstVisitor::visitSubstance_def(BNGParser::Substance_defContext* ctx) {
    currentModel_->setSubstanceUnits(stripQuotes(ctx->quoted_string()->getText()));
    return {};
}

std::any BNGAstVisitor::visitSet_option(BNGParser::Set_optionContext* ctx) {
    const auto strings = ctx->quoted_string();
    const auto values = ctx->action_arg_value();
    for (std::size_t i = 0; i < strings.size() && i < values.size(); ++i) {
        currentModel_->setOption(stripQuotes(strings[i]->getText()), values[i]->getText());
    }
    return {};
}

std::any BNGAstVisitor::visitSet_model_name(BNGParser::Set_model_nameContext* ctx) {
    currentModel_->setModelName(stripQuotes(ctx->quoted_string()->getText()));
    return {};
}

std::any BNGAstVisitor::visitParameter_def(BNGParser::Parameter_defContext* ctx) {
    const auto names = ctx->param_name();
    if (names.empty() || ctx->expression() == nullptr) {
        return {};
    }

    currentModel_->addParameter(ast::Parameter(names.back()->getText(), buildExpression(ctx->expression())));
    return {};
}

std::any BNGAstVisitor::visitFunction_def(BNGParser::Function_defContext* ctx) {
    if (ctx->expression() == nullptr || ctx->STRING().empty()) {
        return {};
    }

    const auto strings = ctx->STRING();
    currentModel_->addFunction(ast::Function(
        strings.front()->getText(),
        collectParamNames(ctx->param_list()),
        buildExpression(ctx->expression())));
    return {};
}

std::any BNGAstVisitor::visitCompartment_def(BNGParser::Compartment_defContext* ctx) {
    const auto names = ctx->STRING();
    if (names.empty() || ctx->INT() == nullptr || ctx->expression() == nullptr) {
        return {};
    }

    const auto expr = buildExpression(ctx->expression());
    currentModel_->addCompartment(ast::Compartment(
        names.front()->getText(),
        evaluateExpression(expr, currentModel_->getParameters()),
        std::stoi(ctx->INT()->getText()),
        names.size() > 1 ? names.back()->getText() : std::string {}));
    return {};
}

std::any BNGAstVisitor::visitMolecule_type_def(BNGParser::Molecule_type_defContext* ctx) {
    if (ctx->molecule_def() == nullptr) {
        return {};
    }

    std::vector<ast::ComponentType> components;
    if (auto* componentList = ctx->molecule_def()->component_def_list()) {
        for (auto* component : componentList->component_def()) {
            if (component == nullptr) {
                continue;
            }
            components.push_back(ast::ComponentType {
                getComponentName(component),
                collectStateNames(component->state_list()),
            });
        }
    }

    currentModel_->addMoleculeType(ast::MoleculeType(
        getMoleculeName(ctx->molecule_def()),
        std::move(components),
        ctx->POPULATION() != nullptr));
    return {};
}

std::any BNGAstVisitor::visitSeed_species_def(BNGParser::Seed_species_defContext* ctx) {
    if (ctx->species_def() == nullptr) {
        return {};
    }

    std::string compartment;
    if (ctx->AT() != nullptr && !ctx->STRING().empty()) {
        compartment = ctx->STRING().back()->getText();
    }
    if (compartment.empty()) {
        compartment = extractTrailingCompartment(ctx->species_def());
    }

    currentModel_->addSeedSpecies(ast::SeedSpecies(
        ctx->species_def()->getText(),
        ctx->expression() != nullptr ? buildExpression(ctx->expression()) : ast::Expression::number(0.0),
        ctx->DOLLAR() != nullptr,
        std::move(compartment),
        buildPatternGraph(ctx->species_def(), *currentModel_, false)));
    return {};
}

std::any BNGAstVisitor::visitEnergy_pattern_def(BNGParser::Energy_pattern_defContext* ctx) {
    if (ctx->species_def() == nullptr || ctx->expression() == nullptr) {
        return {};
    }

    currentModel_->addEnergyPattern(ast::EnergyPattern(
        ctx->STRING() != nullptr ? ctx->STRING()->getText() : std::string(),
        ctx->species_def()->getText(),
        buildExpression(ctx->expression()),
        ast::SpeciesGraph(buildPatternGraph(ctx->species_def(), *currentModel_, true))));
    return {};
}

std::any BNGAstVisitor::visitObservable_def(BNGParser::Observable_defContext* ctx) {
    if (ctx->observable_pattern_list() == nullptr || ctx->STRING().empty()) {
        return {};
    }

    std::vector<std::string> patterns;
    for (auto* pattern : ctx->observable_pattern_list()->observable_pattern()) {
        patterns.push_back(pattern->getText());
    }

    const auto names = ctx->STRING();
    const std::string type = ctx->observable_type() != nullptr ? ctx->observable_type()->getText() : "Molecules";
    currentModel_->addObservable(ast::Observable(names.back()->getText(), type, std::move(patterns)));
    return {};
}

std::any BNGAstVisitor::visitReaction_rule_def(BNGParser::Reaction_rule_defContext* ctx) {
    std::vector<std::string> reactants;
    std::vector<std::string> products;
    std::vector<ast::Expression> rates;
    std::vector<std::string> modifiers;

    for (auto* species : ctx->reactant_patterns()->species_def()) {
        reactants.push_back(species->getText());
    }
    for (auto* species : ctx->product_patterns()->species_def()) {
        products.push_back(species->getText());
    }
    for (auto* expr : ctx->rate_law()->expression()) {
        rates.push_back(buildExpression(expr));
    }
    for (auto* modifier : ctx->rule_modifiers()) {
        modifiers.push_back(modifier->getText());
    }

    // Check if any molecule in reactant/product patterns has a scope prefix (%x::)
    bool hasScopePrefix = false;
    for (auto* speciesDef : ctx->reactant_patterns()->species_def()) {
        for (auto* molPat : speciesDef->molecule_pattern()) {
            if (molPat->scope_prefix() != nullptr) {
                hasScopePrefix = true;
                break;
            }
        }
        if (hasScopePrefix) break;
    }
    if (!hasScopePrefix) {
        for (auto* speciesDef : ctx->product_patterns()->species_def()) {
            for (auto* molPat : speciesDef->molecule_pattern()) {
                if (molPat->scope_prefix() != nullptr) {
                    hasScopePrefix = true;
                    break;
                }
            }
            if (hasScopePrefix) break;
        }
    }

    currentModel_->addReactionRule(ast::ReactionRule(
        "R" + std::to_string(currentModel_->getReactionRules().size() + 1),
        ctx->label_def() != nullptr ? ctx->label_def()->getText() : std::string(),
        std::move(reactants),
        std::move(products),
        std::move(rates),
        std::move(modifiers),
        ctx->reaction_sign()->BI_REACTION_SIGN() != nullptr,
        [&] {
            std::vector<ast::SpeciesGraph> patterns;
            patterns.reserve(ctx->reactant_patterns()->species_def().size());
            for (auto* species : ctx->reactant_patterns()->species_def()) {
                auto sg = ast::SpeciesGraph(buildPatternGraph(species, *currentModel_, true));
                sg.setCompartment(extractSpeciesCompartment(species));
                sg.setCompartmentIsPrefix(isSpeciesCompartmentPrefix(species));
                patterns.emplace_back(sg);
            }
            return patterns;
        }(),
        [&] {
            std::vector<ast::SpeciesGraph> patterns;
            patterns.reserve(ctx->product_patterns()->species_def().size());
            for (auto* species : ctx->product_patterns()->species_def()) {
                auto sg = ast::SpeciesGraph(buildPatternGraph(species, *currentModel_, true));
                sg.setCompartment(extractSpeciesCompartment(species));
                sg.setCompartmentIsPrefix(isSpeciesCompartmentPrefix(species));
                patterns.emplace_back(sg);
            }
            return patterns;
        }()));
    if (hasScopePrefix) {
        auto& rules = currentModel_->getReactionRules();
        if (!rules.empty()) {
            rules.back().setHasScopePrefix(true);
        }
    }
    return {};
}

void BNGAstVisitor::addAction(const std::string& name, BNGParser::Action_argsContext* args) {
    currentModel_->addAction(buildActionFromArgs(name, args));
}

std::any BNGAstVisitor::visitGenerate_network_cmd(BNGParser::Generate_network_cmdContext* ctx) {
    addAction("generate_network", ctx->action_args());
    return {};
}

std::any BNGAstVisitor::visitSimulate_cmd(BNGParser::Simulate_cmdContext* ctx) {
    addAction(toLower(ctx->children.front()->getText()), ctx->action_args());
    return {};
}

std::any BNGAstVisitor::visitWrite_cmd(BNGParser::Write_cmdContext* ctx) {
    addAction(toLower(ctx->children.front()->getText()), ctx->action_args());
    return {};
}

std::any BNGAstVisitor::visitSet_cmd(BNGParser::Set_cmdContext* ctx) {
    ast::Action action;
    action.name = toLower(ctx->children.front()->getText());
    action.arguments["target"] = stripQuotes(ctx->quoted_string()->getText());
    action.arguments["value"] = ctx->action_arg_value()->getText();
    currentModel_->addAction(std::move(action));
    return {};
}

std::any BNGAstVisitor::visitOther_action_cmd(BNGParser::Other_action_cmdContext* ctx) {
    if (ctx->action_args() != nullptr) {
        addAction(toLower(ctx->children.front()->getText()), ctx->action_args());
    } else {
        ast::Action action;
        action.name = toLower(ctx->children.front()->getText());
        if (ctx->action_arg_value() != nullptr) {
            action.arguments["value"] = ctx->action_arg_value()->getText();
        }
        currentModel_->addAction(std::move(action));
    }
    return {};
}

std::unique_ptr<ast::Model> parseModel(const std::string& sourceText) {
    antlr4::ANTLRInputStream input(sourceText);
    BNGLexer lexer(&input);
    antlr4::CommonTokenStream tokens(&lexer);
    BNGParser parser(&tokens);
    auto* tree = parser.prog();
    if (parser.getNumberOfSyntaxErrors() != 0) {
        throw std::runtime_error("Cannot build model from source with syntax errors");
    }

    BNGAstVisitor visitor;
    visitor.visit(tree);
    return visitor.takeModel();
}

ast::Expression parseExpression(const std::string& exprText) {
    return parseExpressionImpl(exprText);
}

} // namespace bng::parser
