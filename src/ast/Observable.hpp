#pragma once

#include <string>
#include <vector>

namespace bng::ast {

class Observable {
public:
    Observable(std::string name, std::string type, std::vector<std::string> patterns);

    const std::string& getName() const;
    const std::string& getType() const;
    const std::vector<std::string>& getPatterns() const;

private:
    std::string name_;
    std::string type_;
    std::vector<std::string> patterns_;
};

} // namespace bng::ast
