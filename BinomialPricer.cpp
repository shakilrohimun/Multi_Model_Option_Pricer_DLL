/**
 * @file BinomialPricer.cpp
 * @brief Implementation of the BinomialPricer class using the CRR method.
 *
 * This file implements the option pricing model using a binomial tree.
 * It supports pricing for both European and American options.
 * In addition to the basic option parameters, this pricer uses additional configuration
 * parameters (e.g., calculation date, number of steps, maturity, risk-free rate, and yield curve)
 * from a PricingConfiguration object.
 */

#include "pch.h"
#include "BinomialPricer.hpp"
#include "Option.hpp"
#include <cmath>
#include <vector>
#include <algorithm>
#include <stdexcept>

 // The BinomialPricer class utilizes the Cox-Ross-Rubinstein (CRR) model to simulate the evolution
 // of the underlying asset's price through a recombining binomial tree. This implementation supports both
 // European and American options, and leverages a variable risk-free rate by interpolating values from a yield curve.

 /**
  * @brief Default constructor, using default configuration values.
  *
  * Constructs a BinomialPricer instance with default configuration parameters.
  * The default configuration is obtained from the PricingConfiguration's default constructor.
  */
BinomialPricer::BinomialPricer()
    : config_() // Default configuration from PricingConfiguration's default constructor
{
    // No additional initialization required.
}

/**
 * @brief Constructor with pricing configuration.
 *
 * Constructs a BinomialPricer instance using the provided PricingConfiguration object.
 * The configuration parameters (e.g., calculation date, number of steps, maturity, etc.) are stored internally
 * and used in subsequent pricing computations.
 *
 * @param config A PricingConfiguration object containing custom pricing parameters.
 */
BinomialPricer::BinomialPricer(const PricingConfiguration& config)
    : config_(config)
{
    // The configuration parameters are now stored in config_.
}

/**
 * @brief Destructor.
 *
 * Destroys the BinomialPricer instance. No explicit dynamic memory cleanup is required.
 */
BinomialPricer::~BinomialPricer() {
    // No dynamic cleanup is required.
}

/**
 * @brief Computes the option price using the binomial CRR model.
 *
 * This function calculates the price of an option using a binomial tree constructed according to the CRR method.
 * The number of steps in the tree is determined by config_.binomialSteps. For each time step, a local risk-free rate is
 * obtained via interpolation from the yield curve, allowing for a variable rate across the tree.
 *
 * The procedure involves:
 * - Computing the up (u) and down (d) factors based on the volatility and time increment.
 * - Determining the risk-neutral probability (p) using a constant risk-free rate for the forward simulation.
 * - Calculating the terminal payoffs for each final node.
 * - Performing backward induction to discount the payoffs to present value, with adjustments for American options.
 *
 * @param opt The Option object to be priced, which contains parameters such as underlying price, strike, volatility,
 *            dividend yield, option type (Call/Put), and option style (European/American).
 * @return The computed option price as a double.
 * @throw std::runtime_error if the computed risk-neutral probability is invalid.
 */
double BinomialPricer::price(const Option& opt) const {
    // Retrieve basic option parameters.
    double S = opt.getUnderlying();
    double K = opt.getStrike();
    double sigma = opt.getVolatility();
    double q = opt.getDividend();
    // Retrieve the maturity and the default risk-free rate from the pricing configuration.
    double T = config_.maturity;
    double r_const = config_.riskFreeRate; // fallback if yield curve is not loaded

    // Retrieve the number of steps from the configuration.
    int N = config_.binomialSteps;
    double dt = T / N;

    // Compute the up (u) and down (d) factors using the CRR model.
    double u = std::exp(sigma * std::sqrt(dt));
    double d = 1.0 / u;
    // Calculate the risk-neutral probability using the constant risk-free rate.
    double p = (std::exp((r_const - q) * dt) - d) / (u - d);
    if (p < 0.0 || p > 1.0) {
        throw std::runtime_error("Invalid risk-neutral probability in the binomial model.");
    }

    // Create a vector to store the terminal payoffs.
    std::vector<double> prices(N + 1);
    for (int j = 0; j <= N; ++j) {
        double S_j = S * std::pow(u, j) * std::pow(d, N - j);
        if (opt.getOptionType() == Option::OptionType::Call) {
            prices[j] = std::max(S_j - K, 0.0);
        }
        else { // Put option
            prices[j] = std::max(K - S_j, 0.0);
        }
    }

    // Backward induction through the binomial tree with a variable interest rate.
    for (int i = N - 1; i >= 0; --i) {
        // Compute normalized time for the current step (i/N).
        double t_norm = static_cast<double>(i) / N;
        // Obtain the local risk-free rate via the yield curve.
        // If the yield curve is not loaded, it returns the default risk-free rate.
        double r_local = config_.yieldCurve.getRate(t_norm);
        // Compute the discount factor using the local rate.
        double discountFactor = std::exp(-r_local * dt);
        // Compute the local risk-neutral probability using the local rate.
        double p_local = (std::exp((r_local - q) * dt) - d) / (u - d);

        for (int j = 0; j <= i; ++j) {
            // Calculate the expected continuation value at the current node.
            double continuation = discountFactor * (p_local * prices[j + 1] + (1.0 - p_local) * prices[j]);
            if (opt.getOptionStyle() == Option::OptionStyle::American) {
                // For American options, compare the continuation value with the immediate exercise value.
                double S_i = S * std::pow(u, j) * std::pow(d, i - j);
                double intrinsic = 0.0;
                if (opt.getOptionType() == Option::OptionType::Call) {
                    intrinsic = std::max(S_i - K, 0.0);
                }
                else {
                    intrinsic = std::max(K - S_i, 0.0);
                }
                prices[j] = std::max(continuation, intrinsic);
            }
            else {
                // For European options, only the continuation value is considered.
                prices[j] = continuation;
            }
        }
    }

    // The price at the root of the tree is the computed option price.
    return prices[0];
}

/**
 * @brief Computes the Greeks of the option using finite differences applied to the binomial model.
 *
 * This function estimates the sensitivity measures (Greeks) by perturbing the input parameters and
 * recalculating the option price. Finite difference approximations are used as follows:
 * - Delta and Gamma are computed by perturbing the underlying price.
 * - Vega is computed by perturbing the volatility.
 * - Theta is computed by reducing the maturity by one day.
 * - Rho is computed by perturbing the risk-free rate (or yield curve).
 *
 * @param opt The Option object to be evaluated.
 * @return A Greeks structure containing the calculated values for Delta, Gamma, Vega, Theta, and Rho.
 */
Greeks BinomialPricer::computeGreeks(const Option& opt) const {
    double h = 0.01 * opt.getUnderlying();

    // Compute Delta and Gamma by perturbing the underlying asset price.
    Option opt_up = opt;
    Option opt_down = opt;
    opt_up.setUnderlying(opt.getUnderlying() + h);
    opt_down.setUnderlying(opt.getUnderlying() - h);
    double price_up = price(opt_up);
    double price_down = price(opt_down);
    double basePrice = price(opt);
    double delta = (price_up - price_down) / (2 * h);
    double gamma = (price_up - 2 * basePrice + price_down) / (h * h);

    // Compute Vega by perturbing the volatility.
    double volStep = 0.01;
    Option opt_vol_up = opt;
    Option opt_vol_down = opt;
    opt_vol_up.setVolatility(opt.getVolatility() + volStep);
    opt_vol_down.setVolatility(opt.getVolatility() - volStep);
    double vega = (price(opt_vol_up) - price(opt_vol_down)) / (2 * volStep);

    // --- Theta Implementation ---
    // Estimate Theta by reducing the maturity by one day (converted to years).
    double dt_small = 1.0 / 365.0; // One day in years
    PricingConfiguration config_time = config_;
    config_time.maturity = config_.maturity - dt_small;
    BinomialPricer pricer_time(config_time);
    double price_T_down = pricer_time.price(opt);
    double theta = (basePrice - price_T_down) / dt_small;

    // --- Modified Implementation of Rho ---
    // Estimate Rho by perturbing the risk-free rate or the yield curve uniformly.
    double rStep = 0.001;
    PricingConfiguration config_r_up = config_;
    PricingConfiguration config_r_down = config_;

    if (!config_.yieldCurve.getData().empty()) {
        // Shift all rates in the yield curve uniformly by rStep.
        YieldCurve yc_up;
        YieldCurve yc_down;
        for (const auto& pt : config_.yieldCurve.getData()) {
            yc_up.addRatePoint(pt.maturity, pt.rate + rStep);
            yc_down.addRatePoint(pt.maturity, pt.rate - rStep);
        }
        config_r_up.yieldCurve = yc_up;
        config_r_down.yieldCurve = yc_down;
    }
    else {
        // If no yield curve data is available, adjust the constant risk-free rate.
        config_r_up.riskFreeRate = config_.riskFreeRate + rStep;
        config_r_down.riskFreeRate = config_.riskFreeRate - rStep;
    }

    BinomialPricer pricer_r_up(config_r_up);
    BinomialPricer pricer_r_down(config_r_down);
    double price_r_up = pricer_r_up.price(opt);
    double price_r_down = pricer_r_down.price(opt);
    double rho = (price_r_up - price_r_down) / (2 * rStep);

    // Populate the Greeks structure with the computed values.
    Greeks g;
    g.delta = delta;
    g.gamma = gamma;
    g.vega = vega;
    g.theta = theta;
    g.rho = rho;
    return g;
}

/**
 * @brief Sets the pricing configuration.
 *
 * Updates the internal configuration parameters used by the BinomialPricer.
 * This method allows the user to modify settings such as the number of steps, maturity,
 * risk-free rate, and yield curve data, which will affect subsequent pricing computations.
 *
 * @param config A PricingConfiguration object containing the new configuration parameters.
 */
void BinomialPricer::setConfiguration(const PricingConfiguration& config) {
    config_ = config;
}

/**
 * @brief Gets the current pricing configuration.
 *
 * Retrieves the PricingConfiguration object currently stored in the BinomialPricer.
 * This function is useful for verifying the active configuration and for debugging purposes.
 *
 * @return A copy of the current PricingConfiguration.
 */
PricingConfiguration BinomialPricer::getConfiguration() const {
    return config_;
}
