#include <iostream>
#include <fstream>
#include <string>

int main() {
    std::ifstream in("src/io/NetWriter.cpp");
    std::string content((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    std::string search = "auto pattern = parseObservablePattern(patternText, const_cast<ast::Model&>(model));";

    std::cout << "Occurrences of search: " << content.find(search) << std::endl;
    return 0;
}
