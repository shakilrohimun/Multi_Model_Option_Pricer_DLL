#ifndef YIELDCURVE_HPP
#define YIELDCURVE_HPP

/**
 * @file YieldCurve.hpp
 * @brief Declaration of the YieldCurve class and RatePoint structure for managing the interest rate curve.
 *
 * This file defines the RatePoint structure and the YieldCurve class, which provide a mechanism to store and manage
 * interest rate data as a function of time (maturity). The YieldCurve class offers functionalities such as adding rate points,
 * interpolating interest rates for a given maturity, retrieving stored data, and loading rate data from an external file.
 */

#include "pch.h"      // Precompiled header for improved compilation performance.
#include <vector>     // Standard vector container used for storing rate points.
#include <string>     // Provides std::string for handling text data, such as filenames.

 /**
  * @brief Structure representing a point on the yield curve.
  *
  * A RatePoint encapsulates a specific maturity value and its associated interest rate.
  * The maturity is typically represented as a fraction (e.g., between 0 and 1) to denote the time in years.
  */
struct RatePoint {
    double maturity; /**< The maturity fraction, typically within the range [0, 1], representing the time to maturity in years. */
    double rate;     /**< The interest rate corresponding to the given maturity. */
};

/**
 * @brief Class for managing the interest rate curve.
 *
 * The YieldCurve class manages a collection of RatePoint objects, which together form an interest rate curve.
 * It provides methods to add new rate points, interpolate the interest rate for a specified maturity, access the underlying data,
 * and load rate data from a file. This is essential for financial models that require variable interest rates,
 * such as when discounting cash flows or pricing interest rate-sensitive derivatives.
 *
 * @details The interpolation is typically performed using linear interpolation between the nearest known rate points.
 * The loadFromFile function expects a text file where each line contains two numbers separated by whitespace:
 * the maturity (as a fraction between 0 and 1) and the corresponding interest rate.
 */
class YieldCurve {
public:
    /**
     * @brief Adds a rate point to the yield curve.
     *
     * This method appends a new RatePoint to the internal storage, thereby extending the yield curve with additional data.
     *
     * @param maturity The maturity fraction (between 0 and 1) representing the time to maturity in years.
     * @param rate The interest rate associated with the given maturity.
     */
    void addRatePoint(double maturity, double rate);

    /**
     * @brief Interpolates and returns the interest rate for a given maturity.
     *
     * This method computes the interest rate corresponding to a specified maturity by performing interpolation on the stored rate points.
     *
     * @param t The desired maturity (between 0 and 1) for which the interest rate is requested.
     * @return The interpolated interest rate as a double.
     * @throw std::runtime_error if the yield curve is empty.
     */
    double getRate(double t) const;

    /**
     * @brief Retrieves the underlying rate point data.
     *
     * This method provides read-only access to the vector that stores all the rate points of the yield curve.
     *
     * @return A constant reference to the vector of RatePoint objects.
     */
    const std::vector<RatePoint>& getData() const;

    /**
     * @brief Loads rate data from a text file.
     *
     * This method reads rate points from a file, where each line should contain two numbers separated by whitespace:
     * the maturity (a fraction between 0 and 1) and the associated interest rate. The loaded rate points are then added
     * to the yield curve.
     *
     * @param filename The path to the text file containing the rate data.
     * @throw std::runtime_error if the file cannot be opened or if the file format is invalid.
     */
    void loadFromFile(const std::string& filename);

private:
    std::vector<RatePoint> data_; /**< Internal storage for the rate points that form the yield curve. */
};

#endif // YIELDCURVE_HPP
