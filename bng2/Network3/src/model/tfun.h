/*
 * tfun.h
 *
 * Table function (TFUN) implementation for BioNetGen
 * Supports piecewise linear and step function interpolation
 */

#ifndef TFUN_H_
#define TFUN_H_

#include <vector>
#include <string>
#include <stdexcept>

namespace BNG {

// Interpolation methods
enum TfunMethod {
    LINEAR,  // Piecewise linear interpolation (default)
    STEP     // Step function (left-continuous)
};

/**
 * @class Tfun
 * @brief Table function with interpolation
 *
 * Stores tabular data (x, y pairs) and provides interpolation.
 * Supports both linear and step function interpolation.
 *
 * Extrapolation: Returns first/last value outside data range (constant)
 */
class Tfun {
public:
    /**
     * @brief Constructor
     * @param x_vals X values (index) - must be strictly increasing
     * @param y_vals Y values (function values)
     * @param method Interpolation method (LINEAR or STEP)
     * @throws std::runtime_error if data is invalid
     */
    Tfun(const std::vector<double>& x_vals,
         const std::vector<double>& y_vals,
         TfunMethod method = LINEAR);

    /**
     * @brief Evaluate function at given x value
     * @param x Index value
     * @return Interpolated function value
     */
    double evaluate(double x) const;

    // Getters
    const std::vector<double>& getXVals() const { return x_vals_; }
    const std::vector<double>& getYVals() const { return y_vals_; }
    TfunMethod getMethod() const { return method_; }
    size_t size() const { return x_vals_.size(); }

private:
    std::vector<double> x_vals_;  // Index values (strictly increasing)
    std::vector<double> y_vals_;  // Function values
    TfunMethod method_;           // Interpolation method

    // Cached interval for performance
    mutable size_t last_interval_;

    /**
     * @brief Find interval containing x using binary search
     * @param x Value to locate
     * @return Index i such that x_vals_[i] <= x < x_vals_[i+1]
     */
    size_t findInterval(double x) const;

    /**
     * @brief Linear interpolation between two points
     * @param x Value to interpolate at
     * @param i Interval index (x is in [x_vals_[i], x_vals_[i+1]))
     * @return Interpolated value
     */
    double linearInterpolate(double x, size_t i) const;

    /**
     * @brief Step function interpolation
     * @param x Value to interpolate at
     * @param i Interval index
     * @return Step value (y_vals_[i])
     */
    double stepInterpolate(double x, size_t i) const;

    /**
     * @brief Validate data consistency
     * @throws std::runtime_error if data is invalid
     */
    void validate() const;
};

} // namespace BNG

#endif // TFUN_H_
