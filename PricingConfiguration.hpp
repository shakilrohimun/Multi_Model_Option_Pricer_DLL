#ifndef PRICINGCONFIGURATION_HPP
#define PRICINGCONFIGURATION_HPP

/**
 * @file PricingConfiguration.hpp
 * @brief Declaration of the PricingConfiguration structure.
 *
 * This file defines the PricingConfiguration structure which encapsulates additional
 * input parameters for the pricing models. It includes both common parameters and model-specific
 * parameters to allow maximum user flexibility.
 */

#include "pch.h"
#include <string>
#include "YieldCurve.hpp"  // Include the yield curve header

 /**
  * @brief Structure holding pricing configuration parameters.
  *
  * This structure contains additional input parameters for the pricing models.
  * It provides flexibility by allowing customization of numerical discretization, simulation parameters,
  * and variable interest rates via a yield curve.
  */
struct PricingConfiguration {
    // Common parameter: Calculation date (in ISO 8601 format, e.g., "2025-02-19").
    // If not specified, the current date is assumed.
    std::string calculationDate;

    // New common parameters:
    double maturity;      ///< Maturity in years (default: 1.0)
    double riskFreeRate;  ///< Default risk-free interest rate (default: 0.0 if no yield curve data is available)

    // Yield curve for variable interest rates.
    // If loaded with data, it is used to interpolate the risk-free rate; otherwise, riskFreeRate is used.
    YieldCurve yieldCurve;

    // Black-Scholes parameters:
    // (No additional parameters beyond the calculation date are required.)

    // Binomial model parameters:
    // Number of steps in the binomial tree.
    int binomialSteps;

    // Crank-Nicolson model parameters:
    // Number of time steps in the discretization.
    int crankTimeSteps;
    // Number of spatial (spot) steps in the discretization.
    int crankSpotSteps;
    // Upper limit for the underlying asset price (S_max).
    // If set to 0.0, it can be computed from the underlying or strike.
    double S_max;

    // Monte Carlo model parameters:
    // Number of simulation paths.
    int mcNumPaths;
    // Number of time steps per simulation path.
    int mcTimeStepsPerPath;

    /**
     * @brief Default constructor with default parameter values.
     *
     * The default values are chosen to provide a reasonable starting point.
     * Note that the risk-free rate is set to 0.0 by default; if no yield curve is loaded,
     * this value will be used.
     */
    PricingConfiguration()
        : calculationDate(""), // Empty string means default to current date
        maturity(1.0),
        riskFreeRate(2.0),
        binomialSteps(100),
        crankTimeSteps(100),
        crankSpotSteps(100),
        S_max(0.0), // 0.0 indicates S_max should be computed if needed
        mcNumPaths(10000),
        mcTimeStepsPerPath(100)
    {}
};

#endif // PRICINGCONFIGURATION_HPP
