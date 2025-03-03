#ifndef YIELDCURVE_HPP
#define YIELDCURVE_HPP

/**
 * @file YieldCurve.hpp
 * @brief Declaration of the YieldCurve class for managing the interest rate curve.
 */

#include "pch.h"
#include <vector>
#include <string>

 /**
  * @brief Structure representing a point on the yield curve.
  */
struct RatePoint {
    double maturity; /**< Maturity fraction (for example, from 0 to 1). */
    double rate;     /**< Associated interest rate. */
};

/**
 * @brief Class for managing the interest rate curve.
 *
 * This class stores a series of points (maturity, rate) and provides methods
 * for interpolating the interest rate for a given time. It also allows loading
 * the data from a text file. Each line in the file should contain two numbers:
 * the maturity (between 0 and 1) and the corresponding interest rate.
 */
class YieldCurve {
public:
    /**
     * @brief Adds a rate point to the curve.
     * @param maturity The maturity fraction (between 0 and 1).
     * @param rate The interest rate associated with this maturity.
     */
    void addRatePoint(double maturity, double rate);

    /**
     * @brief Returns the interpolated interest rate for a given maturity.
     * @param t The desired maturity (between 0 and 1).
     * @return The interpolated interest rate.
     * @throw std::runtime_error if the curve is empty.
     */
    double getRate(double t) const;

    /**
     * @brief Provides access to the underlying data.
     * @return A constant reference to the vector of rate points.
     */
    const std::vector<RatePoint>& getData() const;

    /**
     * @brief Loads rate data from a text file.
     *
     * The file should contain, on each line, two numbers separated by whitespace:
     * the maturity (a fraction between 0 and 1) and the associated interest rate.
     *
     * @param filename The path to the text file to load.
     * @throw std::runtime_error if the file cannot be opened or if the format is invalid.
     */
    void loadFromFile(const std::string& filename);

private:
    std::vector<RatePoint> data_; /**< Storage for the rate points. */
};

#endif // YIELDCURVE_HPP
