#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "Rxn.hpp"

namespace bng::ast {

class RxnList {
public:
    bool add(Rxn reaction);
    bool containsLabel(const std::string& label) const;
    std::size_t size() const;
    const std::vector<Rxn>& all() const;

private:
    std::vector<Rxn> reactions_;
    std::unordered_map<std::string, std::size_t> indexBySignature_;
};

} // namespace bng::ast
