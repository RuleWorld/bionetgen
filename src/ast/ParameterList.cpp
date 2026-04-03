#include "ParameterList.hpp"

#include <stdexcept>
#include <utility>

namespace bng::ast {

void ParameterList::add(Parameter parameter) {
    const auto& name = parameter.getName();
    const auto existing = indexByName_.find(name);
    if (existing != indexByName_.end()) {
        parameters_[existing->second] = std::move(parameter);
        return;
    }

    indexByName_[name] = parameters_.size();
    parameters_.push_back(std::move(parameter));
}

bool ParameterList::contains(const std::string& name) const {
    return indexByName_.find(name) != indexByName_.end();
}

const Parameter& ParameterList::get(const std::string& name) const {
    const auto iter = indexByName_.find(name);
    if (iter == indexByName_.end()) {
        throw std::runtime_error("Unknown parameter '" + name + "'");
    }
    return parameters_[iter->second];
}

const std::vector<Parameter>& ParameterList::all() const {
    return parameters_;
}

std::vector<Parameter>& ParameterList::all() {
    return parameters_;
}

std::size_t ParameterList::size() const {
    return parameters_.size();
}

void ParameterList::evaluateAll(double t) {
    for (auto& parameter : parameters_) {
        parameter.clearValue();
    }
    std::unordered_map<std::string, bool> visiting;
    for (std::size_t i = 0; i < parameters_.size(); ++i) {
        evaluateIndex(i, visiting, t);
    }
}

double ParameterList::evaluate(const std::string& name, double t) const {
    const auto iter = indexByName_.find(name);
    if (iter == indexByName_.end()) {
        throw std::runtime_error("Unknown parameter '" + name + "'");
    }

    if (parameters_[iter->second].hasValue()) {
        return parameters_[iter->second].getValue();
    }

    std::unordered_map<std::string, bool> visiting;
    return evaluateIndex(iter->second, visiting, t);
}

double ParameterList::evaluateIndex(std::size_t index, std::unordered_map<std::string, bool>& visiting, double t) const {
    auto& parameter = parameters_[index];
    if (parameter.hasValue()) {
        return parameter.getValue();
    }

    const auto& name = parameter.getName();
    if (visiting[name]) {
        throw std::runtime_error("Circular parameter dependency detected at '" + name + "'");
    }

    visiting[name] = true;
    const double value = parameter.getExpression().evaluate(
        [&](const std::string& dependency) {
            const auto iter = indexByName_.find(dependency);
            if (iter == indexByName_.end()) {
                throw std::runtime_error(
                    "Unknown parameter dependency '" + dependency + "' referenced by '" + name + "'");
            }
            return evaluateIndex(iter->second, visiting, t);
        },
        t);
    visiting[name] = false;

    parameter.setValue(value);
    return value;
}

} // namespace bng::ast
