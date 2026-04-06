#pragma once
#include <string>
#include <vector>

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

    // Topology methods (Perl Compartment.pm parity)
    bool isSurface() const { return dimension_ == 2; }
    bool isVolume() const { return dimension_ == 3; }
    bool isInside(const std::string& other) const { return parent_ == other; }
    bool isOutside(const std::string& other, const std::vector<Compartment>& all) const;
    bool isAdjacent(const std::string& other, const std::vector<Compartment>& all) const;
    // Returns +1 if this is inside other, -1 if other is inside this, 0 if no containment
    int adjacency(const std::string& other, const std::vector<Compartment>& all) const;

private:
    std::string name_;
    double volume_;
    int dimension_;
    std::string parent_;
};

} // namespace ast
} // namespace bng
