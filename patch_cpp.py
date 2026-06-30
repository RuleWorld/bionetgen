import sys

filepath = 'src/engine/HybridModelGenerator.cpp'
with open(filepath, 'r') as f:
    content = f.read()

old_impl = """const BNGcore::PatternGraph& HybridModelGenerator::getPatternGraph(const std::string& pattern) const {
    auto it = patternGraphCache_.find(pattern);
    if (it != patternGraphCache_.end()) {
        return it->second;
    }

    auto& mutableModel = const_cast<ast::Model&>(model_);
    antlr4::ANTLRInputStream input(pattern);
    BNGLexer lexer(&input);
    antlr4::CommonTokenStream tokens(&lexer);
    BNGParser parser(&tokens);
    auto* species = parser.species_def();
    if (parser.getNumberOfSyntaxErrors() != 0) {
        throw std::runtime_error("Syntax error in pattern: " + pattern);
    }
    auto graph = bng::parser::buildPatternGraph(species, mutableModel, true);
    return patternGraphCache_.emplace(pattern, std::move(graph)).first->second;
}"""

new_impl = """const BNGcore::PatternGraph& HybridModelGenerator::getPatternGraph(const std::string& pattern) const {
    {
        std::lock_guard<std::mutex> lock(cacheMutex_);
        auto it = patternGraphCache_.find(pattern);
        if (it != patternGraphCache_.end()) {
            return it->second;
        }
    }

    auto& mutableModel = const_cast<ast::Model&>(model_);
    antlr4::ANTLRInputStream input(pattern);
    BNGLexer lexer(&input);
    antlr4::CommonTokenStream tokens(&lexer);
    BNGParser parser(&tokens);
    auto* species = parser.species_def();
    if (parser.getNumberOfSyntaxErrors() != 0) {
        throw std::runtime_error("Syntax error in pattern: " + pattern);
    }
    auto graph = bng::parser::buildPatternGraph(species, mutableModel, true);

    std::lock_guard<std::mutex> lock(cacheMutex_);
    return patternGraphCache_.emplace(pattern, std::move(graph)).first->second;
}"""

content = content.replace(old_impl, new_impl)

with open(filepath, 'w') as f:
    f.write(content)
