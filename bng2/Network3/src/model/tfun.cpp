/*
 * tfun.cpp
 *
 * Table function (TFUN) implementation
 */

#include "tfun.h"
#include <algorithm>
#include <sstream>
#include <cmath>

namespace BNG {

Tfun::Tfun(const std::vector<double>& x_vals,
           const std::vector<double>& y_vals,
           TfunMethod method)
    : x_vals_(x_vals)
    , y_vals_(y_vals)
    , method_(method)
    , last_interval_(0)
{
    validate();
}

void Tfun::validate() const
{
    // Must have at least 2 data points
    if (x_vals_.size() < 2) {
        throw std::runtime_error(
            "tfun requires at least 2 data points, found " +
            std::to_string(x_vals_.size()));
    }

    // Arrays must have same length
    if (x_vals_.size() != y_vals_.size()) {
        throw std::runtime_error(
            "tfun x and y arrays must have same length: " +
            std::to_string(x_vals_.size()) + " vs " +
            std::to_string(y_vals_.size()));
    }

    // Verify strict monotonicity (x values must be increasing)
    for (size_t i = 0; i < x_vals_.size() - 1; i++) {
        if (x_vals_[i] >= x_vals_[i+1]) {
            std::ostringstream oss;
            oss << "tfun x values must be strictly increasing. "
                << "At index " << i << ": "
                << x_vals_[i] << " >= " << x_vals_[i+1];
            throw std::runtime_error(oss.str());
        }
    }
}

double Tfun::evaluate(double x) const
{
    // Extrapolation: constant (return first/last value)
    if (x <= x_vals_.front()) {
        return y_vals_.front();
    }
    if (x >= x_vals_.back()) {
        return y_vals_.back();
    }

    // Find interval containing x
    size_t i = findInterval(x);

    // Interpolate based on method
    switch (method_) {
        case LINEAR:
            return linearInterpolate(x, i);
        case STEP:
            return stepInterpolate(x, i);
        default:
            throw std::runtime_error("Unknown tfun interpolation method");
    }
}

size_t Tfun::findInterval(double x) const
{
    // Check cached interval first (optimization for sequential access)
    if (last_interval_ < x_vals_.size() - 1) {
        if (x >= x_vals_[last_interval_] && x < x_vals_[last_interval_ + 1]) {
            return last_interval_;
        }
    }

    // Binary search for interval containing x
    // Find first element > x, then back up one
    auto it = std::upper_bound(x_vals_.begin(), x_vals_.end(), x);

    // upper_bound returns iterator to first element > x
    // We want the interval [i, i+1) containing x
    size_t i = std::distance(x_vals_.begin(), it);

    if (i > 0) {
        i--;  // Back up to get x_vals_[i] <= x < x_vals_[i+1]
    }

    // Cache for next call
    last_interval_ = i;

    return i;
}

double Tfun::linearInterpolate(double x, size_t i) const
{
    // Linear interpolation: y = y1 + (y2-y1)*(x-x1)/(x2-x1)
    double x1 = x_vals_[i];
    double x2 = x_vals_[i+1];
    double y1 = y_vals_[i];
    double y2 = y_vals_[i+1];

    double t = (x - x1) / (x2 - x1);  // Parameter in [0,1]

    return y1 + t * (y2 - y1);
}

double Tfun::stepInterpolate(double x, size_t i) const
{
    // Step function: return y[i] for x in [x[i], x[i+1])
    // This is left-continuous
    return y_vals_[i];
}

} // namespace BNG
