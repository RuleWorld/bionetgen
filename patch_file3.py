import sys

def modify():
    with open('src/io/NetWriter.cpp', 'r') as f:
        content = f.read()

    # Search for the start of write method
    insert_pos = content.find('void NetWriter::write(std::ostream& out, ast::Model& model, const engine::GeneratedNetwork& network) {')

    if insert_pos == -1:
        print("Function not found")
        return

    insertion = """
    // Cache for parsed observable patterns to avoid re-parsing and re-building graphs
    std::unordered_map<std::string, std::pair<BNGcore::PatternGraph, std::string>> parsedObservableCache;
"""

    content = content[:insert_pos + content[insert_pos:].find('{') + 1] + insertion + content[insert_pos + content[insert_pos:].find('{') + 1:]

    # Replace occurrences in write
    search_str = """            for (const auto& patternText : observable.getPatterns()) {
                auto [pattern, patternCompartment] = parseObservablePatternWithCompartment(patternText, model);"""

    replace_str = """            for (const auto& patternText : observable.getPatterns()) {
                auto cacheIt = parsedObservableCache.find(patternText);
                if (cacheIt == parsedObservableCache.end()) {
                    cacheIt = parsedObservableCache.emplace(patternText, parseObservablePatternWithCompartment(patternText, model)).first;
                }
                auto& pattern = cacheIt->second.first;
                auto& patternCompartment = cacheIt->second.second;"""

    content = content.replace(search_str, replace_str)

    with open('src/io/NetWriter.cpp', 'w') as f:
        f.write(content)

modify()
