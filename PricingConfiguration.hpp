#ifndef PRICINGCONFIGURATION_HPP
#define PRICINGCONFIGURATION_HPP

/**
 * @file PricingConfiguration.hpp
 * @brief Declaration of the PricingConfiguration structure.
 *
 * This file defines the PricingConfiguration structure which encapsulates additional
 * input parameters required by the pricing models. It provides a flexible framework by
 * including both common parameters and model-specific parameters, thereby allowing users
 * to tailor the pricing engine to various market conditions and simulation requirements.
 */

#include "pch.h"
#include <string>
#include "YieldCurve.hpp"  // Include the header file that defines the YieldCurve class for handling variable interest rates.

 /**
  * @brief Structure holding pricing configuration parameters.
  *
  * This structure is used to store additional input parameters for the pricing models.
  * It offers flexibility by allowing customization of numerical discretization, simulation settings,
  * and even variable interest rates via a yield curve. The parameters defined here are used across
  * different pricing models, such as Black-Scholes, Binomial, Crank-Nicolson, and Monte Carlo, to control
  * aspects like maturity, risk-free rate, discretization steps, and simulation paths.
  */
struct PricingConfiguration {
    // Common parameter: Calculation date provided in ISO 8601 format (e.g., "2025-02-19").
    // If this parameter is not specified (i.e., an empty string), the current system date is assumed.
    std::string calculationDate;

    // Additional common parameters:
    double maturity;      ///< The time to maturity of the option in years (default value: 1.0).
    double riskFreeRate;  ///< The default risk-free interest rate used for discounting (default value: 2.0).
    ///< This value is used when yield curve data is not available or loaded.

// Yield curve for handling variable interest rates.
// If yield curve data is provided, it will be used to interpolate the risk-free rate dynamically.
// Otherwise, the static riskFreeRate value will be applied for pricing calculations.
    YieldCurve yieldCurve;

    // Black-Scholes parameters:
    // (No additional parameters beyond the common calculation date are required for Black-Scholes.)

    // Binomial model parameters:
    // Number of steps in the binomial tree used for discretizing the option's life.
    int binomialSteps;

    // Crank-Nicolson model parameters:
    // Number of time steps used to discretize the time dimension in the finite difference grid.
    int crankTimeSteps;
    // Number of spatial (spot) steps used to discretize the underlying asset price in the grid.
    int crankSpotSteps;
    // Upper limit for the underlying asset price (S_max) considered in the finite difference grid.
    // If set to 0.0, S_max will be computed automatically based on the underlying asset price or strike price.
    double S_max;

    // Monte Carlo model parameters:
    // Number of simulation paths to generate in the Monte Carlo simulation.
    int mcNumPaths;
    // Number of time steps per simulation path, which determines the granularity of the simulation.
    int mcTimeStepsPerPath;

    /**
     * @brief Default constructor with default parameter values.
     *
     * The default values are chosen to provide a reasonable starting configuration for option pricing models.
     * For example, if the calculation date is not specified, an empty string is used to indicate that the current
     * date should be applied. The default maturity is set to 1.0 year, and the default risk-free rate is set to 2.0.
     *
     * Other model-specific parameters, such as the number of binomial steps, Crank-Nicolson time and spatial steps,
     * and Monte Carlo simulation settings, are also initialized to typical default values.
     */
    PricingConfiguration()
        : calculationDate(""), // An empty string indicates that the current date should be used.
        maturity(1.0),
        riskFreeRate(2.0),
        binomialSteps(100),
        crankTimeSteps(100),
        crankSpotSteps(100),
        S_max(0.0), // A value of 0.0 signals that S_max should be computed automatically if necessary.
        mcNumPaths(10000),
        mcTimeStepsPerPath(100)
    {}
};

#endif // PRICINGCONFIGURATION_HPP
