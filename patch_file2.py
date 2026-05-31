import sys

def modify():
    with open('src/io/NetWriter.cpp', 'r') as f:
        content = f.read()

    # Replace occurrences in buildDerivedRateParams
    search_str = """                                            auto pattern = parseObservablePattern(patternText, const_cast<ast::Model&>(model));
                                            BNGcore::UllmannSGIso matcher(pattern, speciesGraph);"""

    replace_str = """                                            auto cacheIt = parsedObservableCache.find(patternText);
                                            if (cacheIt == parsedObservableCache.end()) {
                                                cacheIt = parsedObservableCache.emplace(patternText, parseObservablePattern(patternText, const_cast<ast::Model&>(model))).first;
                                            }
                                            BNGcore::UllmannSGIso matcher(cacheIt->second, speciesGraph);"""

    content = content.replace(search_str, replace_str)

    with open('src/io/NetWriter.cpp', 'w') as f:
        f.write(content)

modify()
