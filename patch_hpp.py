import sys

filepath = 'src/engine/HybridModelGenerator.hpp'
with open(filepath, 'r') as f:
    content = f.read()

if '<mutex>' not in content:
    content = content.replace('#include <unordered_map>', '#include <mutex>\n#include <unordered_map>')

if 'std::mutex cacheMutex_;' not in content:
    content = content.replace('mutable std::unordered_map<std::string, BNGcore::PatternGraph> patternGraphCache_;',
                              'mutable std::unordered_map<std::string, BNGcore::PatternGraph> patternGraphCache_;\n    mutable std::mutex cacheMutex_;')

with open(filepath, 'w') as f:
    f.write(content)
