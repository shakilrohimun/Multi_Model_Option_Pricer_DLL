/**
 * @file BlackScholesPricer.cpp
 * @brief Implementation of the BlackScholesPricer class using the Black-Scholes formula.
 *
 * This file implements the BlackScholesPricer class which derives from the IOptionPricer interface.
 * It calculates the price and Greeks (delta, gamma, vega, theta, and rho) of a European option
 * using the Black-Scholes formula with continuous dividends.
 */

#include "pch.h"
#include "BlackScholesPricer.hpp"
#include "Option.hpp"
#include "DateConverter.hpp"  // For date conversion functions
#include <cmath>
#include <stdexcept>

#ifndef M_SQRT1_2
#define M_SQRT1_2 0.70710678118654752440
#endif

#ifndef M_PI
#define M_PI 3.14159265358979323846 // Definition of the constant PI
#endif

 /**
  * @brief Utility function to compute the cumulative distribution function (CDF)
  *        of the standard normal distribution.
  * @param x The value at which to compute the CDF.
  * @return The probability that the standard normal variable is less than or equal to x.
  */
static double norm_cdf(double x) {
    return 0.5 * std::erfc(-x * M_SQRT1_2);
}

/**
 * @brief Utility function to compute the probability density function (PDF)
 *        of the standard normal distribution.
 * @param x The value at which to compute the PDF.
 * @return The density at x.
 */
static double norm_pdf(double x) {
    return (1.0 / std::sqrt(2 * M_PI)) * std::exp(-0.5 * x * x);
}

/**
 * @brief Default constructor for BlackScholesPricer.
 *
 * Initializes the pricer using default configuration parameters.
 */
BlackScholesPricer::BlackScholesPricer() {
    // Default constructor; no explicit configuration provided.
}

/**
 * @brief Constructs a BlackScholesPricer with a custom pricing configuration.
 *
 * This constructor initializes the BlackScholesPricer using the provided PricingConfiguration object.
 * The configuration parameters (such as calculation date, maturity, risk-free rate, and yield curve)
 * are stored and used in the pricing calculations.
 *
 * @param config A PricingConfiguration object containing custom parameters.
 */
BlackScholesPricer::BlackScholesPricer(const PricingConfiguration& config)
    : config_(config)
{
    // Configuration parameters are now stored in config_
}

/**
 * @brief Destructor for BlackScholesPricer.
 */
BlackScholesPricer::~BlackScholesPricer() {
    // No dynamic cleanup required.
}

/**
 * @brief Computes the option price using the Black-Scholes formula.
 *
 * This method calculates the price of a European option using the Black-Scholes formula.
 * It retrieves the maturity and risk-free rate from the configuration and adjusts the effective
 * time to maturity based on the calculation (continuation) date if provided. It then computes
 * an effective risk-free rate by averaging the rates from the yield curve (if available). If no
 * yield curve data is loaded, the default risk-free rate is used.
 *
 * @param opt The option to be priced.
 * @return The computed option price.
 * @throw std::runtime_error if the option is not European.
 */
double BlackScholesPricer::price(const Option& opt) const {
    if (opt.getOptionStyle() != Option::OptionStyle::European) {
        throw std::runtime_error("BlackScholesPricer supports only European options.");
    }

    double S = opt.getUnderlying();      // Underlying price
    double K = opt.getStrike();          // Strike price
    double sigma = opt.getVolatility();  // Volatility
    double q = opt.getDividend();        // Continuous dividend yield

    // Retrieve maturity and default risk-free rate from configuration.
    double T = config_.maturity;
    double r_default = config_.riskFreeRate;

    // Adjust effective time to maturity using the calculation date if provided.
    double T_effective = T;
    if (!config_.calculationDate.empty()) {
        auto calcDate = DateConverter::parseDate(config_.calculationDate);
        auto today = std::chrono::system_clock::now();
        double offset = DateConverter::yearsBetween(calcDate, today);
        T_effective = T - offset;
    }

    // Determine effective risk-free rate using the yield curve if available.
    double effective_r = r_default;

    double d1 = (std::log(S / K) + (effective_r - q + 0.5 * sigma * sigma) * T_effective) / (sigma * std::sqrt(T_effective));
    double d2 = d1 - sigma * std::sqrt(T_effective);

    if (opt.getOptionType() == Option::OptionType::Call) {
        return S * std::exp(-q * T_effective) * norm_cdf(d1) - K * std::exp(-effective_r * T_effective) * norm_cdf(d2);
    }
    else {
        return K * std::exp(-effective_r * T_effective) * norm_cdf(-d2) - S * std::exp(-q * T_effective) * norm_cdf(-d1);
    }
}

/**
 * @brief Computes the Greeks of the option using the Black-Scholes model.
 *
 * This method calculates the following Greeks for a European option:
 * - Delta: Sensitivity of the price with respect to the underlying.
 * - Gamma: Sensitivity of Delta with respect to the underlying.
 * - Vega: Sensitivity with respect to volatility.
 * - Theta: Sensitivity with respect to time.
 * - Rho: Sensitivity with respect to the risk-free rate.
 *
 * Maturity and risk-free rate are taken from the configuration, and the effective time to maturity
 * is adjusted by the calculation (continuation) date if provided. The effective risk-free rate is
 * computed by averaging the yield curve data (if available) or using the default value.
 *
 * @param opt The option to evaluate.
 * @return A Greeks structure containing the calculated values.
 * @throw std::runtime_error if the option is not European.
 */
Greeks BlackScholesPricer::computeGreeks(const Option& opt) const {
    if (opt.getOptionStyle() != Option::OptionStyle::European) {
        throw std::runtime_error("BlackScholesPricer supports only European options.");
    }

    double S = opt.getUnderlying();
    double K = opt.getStrike();
    double sigma = opt.getVolatility();
    double q = opt.getDividend();

    double T = config_.maturity;
    double r_default = config_.riskFreeRate;

    // Adjust effective time to maturity using the calculation date if provided.
    double T_effective = T;
    if (!config_.calculationDate.empty()) {
        auto calcDate = DateConverter::parseDate(config_.calculationDate);
        auto today = std::chrono::system_clock::now();
        double offset = DateConverter::yearsBetween(calcDate, today);
        T_effective = T - offset;
    }

    // Determine effective risk-free rate using the yield curve if available.
    double effective_r = r_default;

    double d1 = (std::log(S / K) + (effective_r - q + 0.5 * sigma * sigma) * T_effective) / (sigma * std::sqrt(T_effective));
    double d2 = d1 - sigma * std::sqrt(T_effective);

    Greeks greeks;
    if (opt.getOptionType() == Option::OptionType::Call) {
        greeks.delta = std::exp(-q * T_effective) * norm_cdf(d1);
    }
    else {
        greeks.delta = -std::exp(-q * T_effective) * norm_cdf(-d1);
    }

    greeks.gamma = std::exp(-q * T_effective) * norm_pdf(d1) / (S * sigma * std::sqrt(T_effective));
    greeks.vega = S * std::exp(-q * T_effective) * norm_pdf(d1) * std::sqrt(T_effective);

    if (opt.getOptionType() == Option::OptionType::Call) {
        greeks.theta = -(S * sigma * std::exp(-q * T_effective) * norm_pdf(d1)) / (2 * std::sqrt(T_effective))
            - effective_r * K * std::exp(-effective_r * T_effective) * norm_cdf(d2)
            + q * S * std::exp(-q * T_effective) * norm_cdf(d1);
    }
    else {
        greeks.theta = -(S * sigma * std::exp(-q * T_effective) * norm_pdf(d1)) / (2 * std::sqrt(T_effective))
            + effective_r * K * std::exp(-effective_r * T_effective) * norm_cdf(-d2)
            - q * S * std::exp(-q * T_effective) * norm_cdf(-d1);
    }

    if (opt.getOptionType() == Option::OptionType::Call) {
        greeks.rho = K * T_effective * std::exp(-effective_r * T_effective) * norm_cdf(d2);
    }
    else {
        greeks.rho = -K * T_effective * std::exp(-effective_r * T_effective) * norm_cdf(-d2);
    }

    greeks.theta = -greeks.theta;

    return greeks;
}
