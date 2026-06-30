import re

with open('src/actions/ActionDispatch.cpp', 'r') as f:
    content = f.read()

# Insert validation functions
validate_fn = """
void validateNFSimPath(const std::string& path) {
    if (path.empty()) return;

    // Ensure the executable is named NFsim or NFsim.exe to prevent arbitrary command execution
    std::filesystem::path p(path);
    std::string filename = p.filename().string();
    if (filename != "NFsim" && filename != "NFsim.exe") {
        throw std::runtime_error("Invalid nfsim_exec path: executable must be named 'NFsim' or 'NFsim.exe'");
    }
}

void validateNumeric(const std::string& arg, const std::string& argName) {
    if (arg.empty()) return;

    // Ensure the argument is a valid number (integer or float)
    try {
        size_t pos = 0;
        std::stod(arg, &pos);
        if (pos != arg.length()) {
            throw std::invalid_argument("not fully parsed");
        }
    } catch (...) {
        throw std::runtime_error("Invalid argument for NFSim parameter '" + argName + "': must be numeric");
    }
}
"""

insert_pos = content.find("std::string lowercase(std::string value) {")
if insert_pos != -1:
    content = content[:insert_pos] + validate_fn + "\n" + content[insert_pos:]
else:
    print("Failed to find insertion point")

nfsim_read_start = content.find('const auto tEnd = stripQuotes(readArgument(action, "t_end", "10"));')
if nfsim_read_start != -1:
    validation_code = """
            validateNumeric(tEnd, "t_end");
            validateNumeric(nSteps, "n_steps");
            if (!seedText.empty()) validateNumeric(seedText, "seed");
            validateNFSimPath(nfsimExec);
"""

    print_pos = content.find("if (verbose) {", nfsim_read_start)
    if print_pos != -1:
        content = content[:print_pos] + validation_code + "\n            " + content[print_pos:]
    else:
        print("Failed to find print statement")
else:
    print("Failed to find read start")

with open('src/actions/ActionDispatch.cpp', 'w') as f:
    f.write(content)
