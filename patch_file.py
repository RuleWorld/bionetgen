import sys

def modify():
    with open('src/io/NetWriter.cpp', 'r') as f:
        content = f.read()

    # Search for the start of buildDerivedRateParams
    insert_pos = content.find('std::unordered_map<std::string, DerivedRateInfo> NetWriter::buildDerivedRateParams')

    if insert_pos == -1:
        print("Function not found")
        return

    insertion = """
    // Cache for parsed observable patterns to avoid re-parsing and re-building graphs
    std::unordered_map<std::string, BNGcore::PatternGraph> parsedObservableCache;
"""

    content = content[:insert_pos + content[insert_pos:].find('{') + 1] + insertion + content[insert_pos + content[insert_pos:].find('{') + 1:]

    # Replace occurrences in buildDerivedRateParams
    search_str = """                                    auto pattern = parseObservablePattern(patternText, const_cast<ast::Model&>(model));
                                    BNGcore::UllmannSGIso matcher(pattern, speciesGraph);"""

    replace_str = """                                    auto cacheIt = parsedObservableCache.find(patternText);
                                    if (cacheIt == parsedObservableCache.end()) {
                                        cacheIt = parsedObservableCache.emplace(patternText, parseObservablePattern(patternText, const_cast<ast::Model&>(model))).first;
                                    }
                                    BNGcore::UllmannSGIso matcher(cacheIt->second, speciesGraph);"""

    content = content.replace(search_str, replace_str)

    with open('src/io/NetWriter.cpp', 'w') as f:
        f.write(content)

modify()
