import re

with open('src/ast/PopulationMappingRule.cpp', 'r') as f:
    content = f.read()

old_pm = """    // Check for TotalRate attribute (Perl lines 233-235)
    bool totalRate = false;
    {
        std::regex totalRateRe("(^|\\\\s)TotalRate(\\\\s|$)");
        if (std::regex_search(str, totalRateRe)) {
            totalRate = true;
            str = std::regex_replace(str, totalRateRe, " ");
            trim(str);
        }
    }"""

new_pm = """    // Check for TotalRate attribute (Perl lines 233-235)
    bool totalRate = false;
    {
        auto pos = str.find("TotalRate");
        if (pos != std::string::npos) {
            bool left_ok = (pos == 0 || std::isspace(str[pos - 1]));
            bool right_ok = (pos + 9 == str.size() || std::isspace(str[pos + 9]));
            if (left_ok && right_ok) {
                totalRate = true;
                str.replace(pos, 9, " ");
                trim(str);
            }
        }
    }"""

content = content.replace(old_pm, new_pm)

with open('src/ast/PopulationMappingRule.cpp', 'w') as f:
    f.write(content)
