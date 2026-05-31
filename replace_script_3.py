import re

with open("src/io/NetWriter.cpp", "r") as f:
    content = f.read()

def replacer(match):
    indent = match.group(1)
    replacement_str = match.group(2)
    return f"""{indent}{{
{indent}    std::string::size_type pos = 0;
{indent}    std::string::size_type next_pos = funcBody.find(pat, pos);
{indent}    if (next_pos != std::string::npos) {{
{indent}        std::string result;
{indent}        result.reserve(funcBody.length());
{indent}        std::string rep = {replacement_str}; // evaluate once
{indent}        while (next_pos != std::string::npos) {{
{indent}            result.append(funcBody, pos, next_pos - pos);
{indent}            result.append(rep);
{indent}            pos = next_pos + pat.length();
{indent}            next_pos = funcBody.find(pat, pos);
{indent}        }}
{indent}        result.append(funcBody, pos, funcBody.length() - pos);
{indent}        funcBody = std::move(result);
{indent}    }}
{indent}}}"""

pattern = re.compile(
    r'^(\s*)\{\n'
    r'\s*std::string result;\n'
    r'\s*result\.reserve\(funcBody\.length\(\)\);\n'
    r'\s*std::string::size_type pos = 0;\n'
    r'\s*std::string::size_type next_pos = 0;\n'
    r'\s*while \(\(next_pos = funcBody\.find\(pat, pos\)\) != std::string::npos\) \{\n'
    r'\s*result\.append\(funcBody, pos, next_pos - pos\);\n'
    r'\s*result\.append\((.*?)\);\n'
    r'\s*pos = next_pos \+ pat\.length\(\);\n'
    r'\s*\}\n'
    r'\s*result\.append\(funcBody, pos, funcBody\.length\(\) - pos\);\n'
    r'\s*funcBody = std::move\(result\);\n'
    r'\s*\}',
    re.MULTILINE
)

new_content = pattern.sub(replacer, content)

with open("src/io/NetWriter.cpp", "w") as f:
    f.write(new_content)

print(f"Replaced {len(pattern.findall(content))} occurrences.")
