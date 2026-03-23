#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "Parameter.hpp"

namespace bng::ast {

class ParameterList {
public:
    void add(Parameter parameter);
    bool contains(const std::string& name) const;
    const Parameter& get(const std::string& name) const;
    const std::vector<Parameter>& all() const;
    std::vector<Parameter>& all();
    std::size_t size() const;

    void evaluateAll(double t = 0.0);
    double evaluate(const std::string& name, double t = 0.0);

private:
    double evaluateIndex(std::size_t index, std::unordered_map<std::string, bool>& visiting, double t);

    std::vector<Parameter> parameters_;
    std::unordered_map<std::string, std::size_t> indexByName_;
};

} // namespace bng::ast
