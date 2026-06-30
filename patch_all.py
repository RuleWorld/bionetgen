import re

with open('src/ast/MacroBNGModel.cpp', 'r') as f:
    content = f.read()

# Optimize activ_sit
old_activ_sit = """void MacroBNGModel::activ_sit(int /*typrul*/, std::string& reac, std::string& prod,
                               std::vector<std::string>& mreac) {
    while (!reac.empty()) {
        // Match first (...) group
        std::regex re_paren("[\\\\(].*?[)]");
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
}"""

new_activ_sit = """void MacroBNGModel::activ_sit(int /*typrul*/, std::string& reac, std::string& prod,
                               std::vector<std::string>& mreac) {
    while (!reac.empty()) {
        auto paren_open = reac.find('(');
        if (paren_open == std::string::npos) break;
        auto paren_close = reac.find(')', paren_open);
        if (paren_close == std::string::npos) break;

        std::string name = reac.substr(0, paren_open);
        std::string r1 = reac.substr(0, paren_close + 1);

        reac = reac.substr(paren_close + 1);
        auto non_sep = reac.find_first_not_of(";+.");
        if (non_sep != std::string::npos) {
            reac = reac.substr(non_sep);
        } else {
            reac.clear();
        }

        mreac.push_back(r1);
    }
}"""

content = content.replace(old_activ_sit, new_activ_sit)


# Optimize hash_sor
old_hash_sor = """            // Find matching molecule in product
            std::string p1;
            {
                std::string qname = quotemeta(name);
                std::regex re_prod(qname + "[\\\\(].*?[)]");
                std::smatch pm;
                if (std::regex_search(prod, pm, re_prod)) {
                    p1 = pm[0].str();
                    // Remove the match from prod
                    prod = pm.prefix().str() + pm.suffix().str();
                }
            }"""

new_hash_sor = """            // Find matching molecule in product
            std::string p1;
            {
                std::string match_prefix = name + "(";
                auto pos = prod.find(match_prefix);
                if (pos != std::string::npos) {
                    auto end_pos = prod.find(')', pos);
                    if (end_pos != std::string::npos) {
                        p1 = prod.substr(pos, end_pos - pos + 1);
                        prod.erase(pos, end_pos - pos + 1);
                    }
                }
            }"""

content = content.replace(old_hash_sor, new_hash_sor)


# Optimize delsites
old_delsites = """    std::map<std::string, size_t> rr1s;
    for (size_t j = 0; j < rr1out.size(); ++j) {
        if (rr1out[j].empty()) continue;
        // Create sort key: replace bond labels with #,
        std::string sortKey = rr1out[j];
        // Replace !xxx, and !xxx) patterns with #,
        std::regex bondPat("![^,)]+([,)])");
        sortKey = std::regex_replace(sortKey, bondPat, "#,");
        rr1s[sortKey] = j;
    }"""

new_delsites = """    std::map<std::string, size_t> rr1s;
    for (size_t j = 0; j < rr1out.size(); ++j) {
        if (rr1out[j].empty()) continue;
        // Create sort key: replace bond labels with #,
        std::string sortKey;
        sortKey.reserve(rr1out[j].size());
        for (size_t k = 0; k < rr1out[j].size(); ++k) {
            if (rr1out[j][k] == '!') {
                sortKey.push_back('#');
                sortKey.push_back(',');
                ++k; // skip '!'
                while (k < rr1out[j].size() && rr1out[j][k] != ',' && rr1out[j][k] != ')') {
                    ++k;
                }
                // The regex replaced "![^,)]+([,)])" with "#,"
                // Meaning the trailing , or ) is LOST in the replacement.
                // So we do NOT append the matched , or ) to sortKey.
                // We just continue. Note: if k is at the end, that's fine.
                // If k is at , or ), we've skipped it. We shouldn't process it in the next loop iteration either,
                // wait, the for loop will increment k again!
                // Ah! We need to make sure k ends up at the correct place.
                // If we found a , or ), the regex consumed it. So in the next loop iteration, k should be AFTER it.
                // Since the for loop does ++k, we leave k at the , or ), and the loop will increment past it.
            } else {
                sortKey.push_back(rr1out[j][k]);
            }
        }
        rr1s[sortKey] = j;
    }"""

content = content.replace(old_delsites, new_delsites)

# Optimize string manipulations in pre_macr and elsewhere
old_replace1 = """        // Check for "begin <block>"
        std::string trimmed = line;
        trimmed.erase(0, trimmed.find_first_not_of(" \\t"));
        std::smatch m;
        std::regex re_begin("^begin\\\\s+(.*)");
        if (std::regex_search(trimmed, m, re_begin)) {
            std::string name = m[1].str();"""

new_replace1 = """        // Check for "begin <block>"
        std::string trimmed = line;
        trimmed.erase(0, trimmed.find_first_not_of(" \\t"));
        std::smatch m;
        if (trimmed.compare(0, 6, "begin ") == 0 || trimmed.compare(0, 6, "begin\\t") == 0) {
            std::string name = trimmed.substr(6);"""

content = content.replace(old_replace1, new_replace1)

old_replace2 = """            else if (std::regex_match(name, std::regex("^reaction[_ ]rules$"))) {
                int nerr_count = 0;
                for (auto& [entry, lno] : block_dat) {
                    std::string str1ing = entry;
                    std::string nam1e;
                    // Check for user-named rule "name: ..."
                    std::smatch rm;
                    if (std::regex_search(str1ing, rm, std::regex("^\\\\s*([^:].*)[:]\\\\s*"))) {
                        nam1e = rm[1].str();
                        std::cerr << "ERROR 1002   rules  (user) name=" << nam1e << "\\n";
                        return "ERROR 1002: named rules not supported";
                    } else if (std::regex_search(str1ing, rm, std::regex("^\\\\s*(\\\\d+)\\\\s+"))) {
                        nam1e = rm[1].str();
                        str1ing = rm.suffix().str();
                    }"""

new_replace2 = """            else if (name == "reaction rules" || name == "reaction_rules") {
                int nerr_count = 0;
                for (auto& [entry, lno] : block_dat) {
                    std::string str1ing = entry;
                    std::string nam1e;

                    // strip leading whitespace
                    size_t start = str1ing.find_first_not_of(" \\t");
                    if (start != std::string::npos) {
                        str1ing = str1ing.substr(start);
                    }

                    // Check for user-named rule "name: ..."
                    auto colon_pos = str1ing.find(':');
                    if (colon_pos != std::string::npos) {
                        nam1e = str1ing.substr(0, colon_pos);
                        std::cerr << "ERROR 1002   rules  (user) name=" << nam1e << "\\n";
                        return "ERROR 1002: named rules not supported";
                    } else {
                        // strip numeric index
                        size_t first_space = str1ing.find_first_of(" \\t");
                        if (first_space != std::string::npos) {
                            bool is_num = true;
                            for (size_t i = 0; i < first_space; ++i) {
                                if (!std::isdigit(str1ing[i])) {
                                    is_num = false;
                                    break;
                                }
                            }
                            if (is_num) {
                                nam1e = str1ing.substr(0, first_space);
                                str1ing = str1ing.substr(first_space);
                                size_t next_non_space = str1ing.find_first_not_of(" \\t");
                                if (next_non_space != std::string::npos) {
                                    str1ing = str1ing.substr(next_non_space);
                                } else {
                                    str1ing.clear();
                                }
                            }
                        }
                    }"""

content = content.replace(old_replace2, new_replace2)

old_replace3 = """            else if (name == "observables") {
                for (auto& [entry, lno] : block_dat) {
                    std::string str1ing = entry;
                    // Strip leading numeric index
                    std::smatch rm;
                    if (std::regex_search(str1ing, rm, std::regex("^\\\\s*\\\\d+\\\\s+"))) {
                        str1ing = rm.suffix().str();
                    }
                    obser1vable_.push_back(str1ing);
                }"""

new_replace3 = """            else if (name == "observables") {
                for (auto& [entry, lno] : block_dat) {
                    std::string str1ing = entry;
                    // Strip leading numeric index
                    size_t start = str1ing.find_first_not_of(" \\t");
                    if (start != std::string::npos) {
                        size_t first_space = str1ing.find_first_of(" \\t", start);
                        if (first_space != std::string::npos) {
                            bool is_num = true;
                            for (size_t i = start; i < first_space; ++i) {
                                if (!std::isdigit(str1ing[i])) {
                                    is_num = false;
                                    break;
                                }
                            }
                            if (is_num) {
                                size_t next_non_space = str1ing.find_first_not_of(" \\t", first_space);
                                if (next_non_space != std::string::npos) {
                                    str1ing = str1ing.substr(next_non_space);
                                } else {
                                    str1ing.clear();
                                }
                            }
                        }
                    }
                    obser1vable_.push_back(str1ing);
                }"""

content = content.replace(old_replace3, new_replace3)

old_replace4 = """        // Check for action lines like "generate_network" / "simulate"
        else if (std::regex_search(trimmed, m, std::regex("^([A-Za-z][^(]*)"))) {
            gene1rate_.push_back(line);
        }"""

new_replace4 = """        // Check for action lines like "generate_network" / "simulate"
        else if (!trimmed.empty() && std::isalpha(trimmed[0])) {
            gene1rate_.push_back(line);
        }"""

content = content.replace(old_replace4, new_replace4)

old_replace5 = """        // Strip leading label "name: ..."
        {
            std::smatch m;
            if (std::regex_search(entry, m, std::regex("^\\\\s*([^:].*)[:]\\\\s*"))) {
                name = m[1].str();
                std::cerr << "ERROR 1001 block species      (user) name=" << name << "\\n";
                return;
            } else {
                // Strip leading numeric index
                std::regex_replace(entry, std::regex("^\\\\s*\\\\d+\\\\s+"), "");
                std::smatch rm;
                if (std::regex_search(entry, rm, std::regex("^\\\\s*\\\\d+\\\\s+"))) {
                    entry = rm.suffix().str();
                }
            }
        }"""

new_replace5 = """        // Strip leading label "name: ..."
        {
            size_t start = entry.find_first_not_of(" \\t");
            if (start != std::string::npos) {
                auto colon_pos = entry.find(':', start);
                if (colon_pos != std::string::npos) {
                    name = entry.substr(start, colon_pos - start);
                    std::cerr << "ERROR 1001 block species      (user) name=" << name << "\\n";
                    return;
                } else {
                    // Strip leading numeric index
                    size_t first_space = entry.find_first_of(" \\t", start);
                    if (first_space != std::string::npos) {
                        bool is_num = true;
                        for (size_t i = start; i < first_space; ++i) {
                            if (!std::isdigit(entry[i])) {
                                is_num = false;
                                break;
                            }
                        }
                        if (is_num) {
                            size_t next_non_space = entry.find_first_not_of(" \\t", first_space);
                            if (next_non_space != std::string::npos) {
                                entry = entry.substr(next_non_space);
                            } else {
                                entry.clear();
                            }
                        }
                    }
                }
            }
        }"""

content = content.replace(old_replace5, new_replace5)


old_replace6 = """        // Extract molecule(site,site,...) patterns
        // Repeatedly match (...) groups
        std::regex re_paren("([\\\\(])(.*?)([\\\\)])");
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
        }"""

new_replace6 = """        // Extract molecule(site,site,...) patterns
        // Repeatedly match (...) groups
        std::string remaining = spec_entry;
        while (true) {
            auto paren_open = remaining.find('(');
            if (paren_open == std::string::npos) break;
            auto paren_close = remaining.find(')', paren_open);
            if (paren_close == std::string::npos) break;

            std::string prefix_str = remaining.substr(0, paren_open);
            // Get the molecule name: last segment after '.'
            auto dot_pos = prefix_str.rfind('.');
            if (dot_pos != std::string::npos) {
                name = prefix_str.substr(dot_pos + 1);
            } else {
                name = prefix_str;
            }

            if (nm_site.find(name) == nm_site.end()) {
                std::string inside = remaining.substr(paren_open + 1, paren_close - paren_open - 1);  // contents inside parentheses
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

            remaining = remaining.substr(paren_close + 1);
            // Also strip "name." prefix from remaining
            if (!remaining.empty() && remaining[0] == '.') {
                remaining = remaining.substr(1);
            }
        }"""

content = content.replace(old_replace6, new_replace6)

old_replace7 = """        // If starts with digit followed by ';', strip the leading number
        std::smatch m;
        if (std::regex_search(line, m, std::regex("^\\\\d+?;"))) {
            line = ";" + m.suffix().str();
        } else {
            line = ";" + line;
        }"""

new_replace7 = """        // If starts with digit followed by ';', strip the leading number
        size_t semi_pos = line.find(';');
        if (semi_pos != std::string::npos && semi_pos > 0) {
            bool is_all_digits = true;
            for (size_t i = 0; i < semi_pos; ++i) {
                if (!std::isdigit(line[i])) {
                    is_all_digits = false;
                    break;
                }
            }
            if (is_all_digits) {
                line = ";" + line.substr(semi_pos + 1);
            } else {
                line = ";" + line;
            }
        } else {
            line = ";" + line;
        }"""

content = content.replace(old_replace7, new_replace7)

with open('src/ast/MacroBNGModel.cpp', 'w') as f:
    f.write(content)
