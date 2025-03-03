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

 /// Default constructor, using default configuration values.
BinomialPricer::BinomialPricer()
    : config_() // Default configuration from PricingConfiguration's default constructor
{
    // No additional initialization required.
}

/// Constructor with pricing configuration.
BinomialPricer::BinomialPricer(const PricingConfiguration& config)
    : config_(config)
{
    // The configuration parameters are now stored in config_.
}

/// Destructor.
BinomialPricer::~BinomialPricer() {
    // No dynamic cleanup is required.
}

/**
 * @brief Computes the option price using the binomial CRR model.
 *
 * This function calculates the price of an option using a binomial tree.
 * The number of steps in the tree is taken from the configuration (config_.binomialSteps).
 *
 * Instead of using a constant risk-free rate, the backward induction uses a variable rate
 * obtained via interpolation from the yield curve stored in the configuration.
 * For each time step, the local rate is obtained by:
 *   r_local = config_.yieldCurve.getRate(t_norm)
 * where t_norm is the normalized time (between 0 and 1).
 *
 * @param opt The option to be priced.
 * @return The computed option price.
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

    // Compute the up and down factors using the CRR model.
    double u = std::exp(sigma * std::sqrt(dt));
    double d = 1.0 / u;
    // For the forward simulation, use the constant risk-free rate.
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

    // Backward induction through the binomial tree with variable interest rate.
    for (int i = N - 1; i >= 0; --i) {
        // Compute normalized time for the current step (i/N).
        double t_norm = static_cast<double>(i) / N;
        // Obtain the local risk-free rate via the yield curve.
        // If the yield curve is not loaded, it should return the default risk-free rate.
        double r_local = config_.yieldCurve.getRate(t_norm);
        // Compute the discount factor using the local rate.
        double discountFactor = std::exp(-r_local * dt);
        // Compute the local risk-neutral probability using the local rate.
        double p_local = (std::exp((r_local - q) * dt) - d) / (u - d);

        for (int j = 0; j <= i; ++j) {
            double continuation = discountFactor * (p_local * prices[j + 1] + (1.0 - p_local) * prices[j]);
            if (opt.getOptionStyle() == Option::OptionStyle::American) {
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
                prices[j] = continuation;
            }
        }
    }

    return prices[0];
}

/**
 * @brief Computes the Greeks of the option using finite differences applied to the binomial model.
 *
 * This function estimates the Greeks (Delta, Gamma, Vega, Theta, and Rho)
 * by perturbing the input parameters and recalculating the option price.
 *
 * @param opt The option to evaluate.
 * @return A Greeks structure containing the calculated values.
 */
Greeks BinomialPricer::computeGreeks(const Option& opt) const {
    double h = 0.01 * opt.getUnderlying();

    // Compute Delta and Gamma via finite differences for the underlying price.
    Option opt_up = opt;
    Option opt_down = opt;
    opt_up.setUnderlying(opt.getUnderlying() + h);
    opt_down.setUnderlying(opt.getUnderlying() - h);
    double price_up = price(opt_up);
    double price_down = price(opt_down);
    double basePrice = price(opt);
    double delta = (price_up - price_down) / (2 * h);
    double gamma = (price_up - 2 * basePrice + price_down) / (h * h);

    // Compute Vega via finite differences for volatility.
    double volStep = 0.01;
    Option opt_vol_up = opt;
    Option opt_vol_down = opt;
    opt_vol_up.setVolatility(opt.getVolatility() + volStep);
    opt_vol_down.setVolatility(opt.getVolatility() - volStep);
    double vega = (price(opt_vol_up) - price(opt_vol_down)) / (2 * volStep);

    // --- Ajout de l'implémentation de Theta ---
    double dt_small = 1.0 / 365.0; // 1 jour en années
    PricingConfiguration config_time = config_;
    config_time.maturity = config_.maturity - dt_small;
    BinomialPricer pricer_time(config_time);
    double price_T_down = pricer_time.price(opt);
    double theta = (basePrice - price_T_down) / dt_small;

    // --- Implémentation modifiée de Rho ---
    double rStep = 0.001;
    PricingConfiguration config_r_up = config_;
    PricingConfiguration config_r_down = config_;

    if (!config_.yieldCurve.getData().empty()) {
        // Décalage de tous les taux de la courbe pour refléter une variation uniforme
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
        config_r_up.riskFreeRate = config_.riskFreeRate + rStep;
        config_r_down.riskFreeRate = config_.riskFreeRate - rStep;
    }

    BinomialPricer pricer_r_up(config_r_up);
    BinomialPricer pricer_r_down(config_r_down);
    double price_r_up = pricer_r_up.price(opt);
    double price_r_down = pricer_r_down.price(opt);
    double rho = (price_r_up - price_r_down) / (2 * rStep);


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
 * Allows updating the configuration parameters used by the BinomialPricer.
 *
 * @param config A PricingConfiguration object containing the new parameters.
 */
void BinomialPricer::setConfiguration(const PricingConfiguration& config) {
    config_ = config;
}

/**
 * @brief Gets the current pricing configuration.
 *
 * @return The current PricingConfiguration object.
 */
PricingConfiguration BinomialPricer::getConfiguration() const {
    return config_;
}
