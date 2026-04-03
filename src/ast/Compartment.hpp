#pragma once
#include <string>

namespace bng {
namespace ast {

class Compartment {
public:
    Compartment(std::string name, double volume, int dimension = 3, std::string parent = {});
    
    std::string getName() const;
    double getVolume() const;
    void setVolume(double volume);
    int getDimension() const;
    const std::string& getParent() const;

private:
    std::string name_;
    double volume_;
    int dimension_;
    std::string parent_;
};

} // namespace ast
} // namespace bng
