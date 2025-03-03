/**
 * @file MonteCarloPricer.cpp
 * @brief Implementation of the MonteCarloPricer class using Monte Carlo simulation.
 *
 * This file implements the option pricing model using Monte Carlo simulation.
 * For European options, a standard simulation of geometric Brownian motion is used.
 * For American options, the Longstaff-Schwartz method (least-squares regression)
 * is applied to determine the optimal early exercise.
 * The simulation parameters (maturity, risk-free rate, number of paths, time steps per path)
 * are taken from a PricingConfiguration object. If a calculation date is provided,
 * the effective time to maturity is adjusted as T_effective = T - offset.
 * The risk-free rate is obtained at each time step by interpolating the yield curve.
 */

#include "pch.h"
#include "MonteCarloPricer.hpp"
#include "Option.hpp"
#include "DateConverter.hpp"   // For date conversion functions
#include <vector>
#include <cmath>
#include <random>
#include <algorithm>
#include <numeric>
#include <stdexcept>

 /// New constructor: initialize with a pricing configuration.
MonteCarloPricer::MonteCarloPricer(const PricingConfiguration& config)
    : config_(config)
{
    // The configuration parameters are now stored in config_
}

/// Default constructor using the default configuration.
MonteCarloPricer::MonteCarloPricer()
    : config_() // Default configuration from PricingConfiguration's default constructor
{
    // No additional initialization is required.
}

/// Destructor.
MonteCarloPricer::~MonteCarloPricer() {
    // No dynamic cleanup is required.
}

/**
 * @brief Computes the option price using Monte Carlo simulation.
 *
 * For European options, a standard simulation of geometric Brownian motion is used.
 * For American options, the Longstaff-Schwartz algorithm is applied to determine the optimal exercise.
 *
 * The simulation parameters (maturity, risk-free rate, number of simulation paths, and number of time steps)
 * are obtained from the PricingConfiguration object. If a calculation date is provided,
 * the effective time to maturity is adjusted as:
 *
 *    T_effective = T - offset,
 *
 * where offset is the number of years between config_.calculationDate and today.
 *
 * In the forward simulation, the local risk-free rate at each step is obtained by:
 *    r_local = (yieldCurve is available) ? yieldCurve.getRate(t_norm) : default riskFreeRate,
 * where t_norm is the normalized time (current time / T_effective).
 *
 * @param opt The option to be priced.
 * @return The computed option price.
 */
double MonteCarloPricer::price(const Option& opt) const {
    // Retrieve option parameters.
    double S0 = opt.getUnderlying();
    double K = opt.getStrike();
    double sigma = opt.getVolatility();
    double q = opt.getDividend();

    // Retrieve simulation parameters from configuration.
    double T = config_.maturity;           // Configured maturity
    double r_default = config_.riskFreeRate; // Default risk-free rate
    const int NPaths = config_.mcNumPaths;   // Number of simulation paths
    const int NSteps = config_.mcTimeStepsPerPath; // Number of time steps per path

    // Adjust effective time to maturity based on the calculation date.
    double T_effective = T;
    if (!config_.calculationDate.empty()) {
        auto calcDate = DateConverter::parseDate(config_.calculationDate);
        auto today = std::chrono::system_clock::now();
        double offset = DateConverter::yearsBetween(calcDate, today);
        if (offset >= T) {
            throw std::runtime_error("Effective maturity is negative. Check the calculation date.");
        }
        T_effective = T - offset;
    }
    double dt = T_effective / NSteps;

    // Initialize random number generator with a fixed seed for reproducibility.
    std::mt19937 rng(42);
    std::normal_distribution<double> norm(0.0, 1.0);

    if (opt.getOptionStyle() == Option::OptionStyle::European) {
        // --- Standard Monte Carlo simulation for European options ---
        double sumPayoff = 0.0;
        for (int i = 0; i < NPaths; i++) {
            double S = S0;
            double disc = 1.0;
            // Simulate one price path using GBM with variable interest rate.
            for (int j = 0; j < NSteps; j++) {
                double t_current = j * dt;
                double normTime = t_current / T_effective; // normalized time in [0,1]
                double r_local = (config_.yieldCurve.getData().empty()) ? r_default : config_.yieldCurve.getRate(normTime);
                double Z = norm(rng);
                S *= std::exp((r_local - q - 0.5 * sigma * sigma) * dt + sigma * std::sqrt(dt) * Z);
                disc *= std::exp(-r_local * dt);
            }
            double payoff = (opt.getOptionType() == Option::OptionType::Call) ? std::max(S - K, 0.0)
                : std::max(K - S, 0.0);
            sumPayoff += payoff * disc; // Already discounted along the path
        }
        double priceMC = sumPayoff / NPaths;
        return priceMC;
    }
    else {
        // --- Monte Carlo simulation for American options using Longstaff-Schwartz ---
        // Build a grid to store simulated paths.
        std::vector<std::vector<double>> paths(NPaths, std::vector<double>(NSteps + 1, 0.0));
        for (int i = 0; i < NPaths; i++) {
            paths[i][0] = S0;
            for (int j = 1; j <= NSteps; j++) {
                double t_current = (j - 1) * dt;
                double normTime = t_current / T_effective;
                double r_local = (config_.yieldCurve.getData().empty()) ? r_default : config_.yieldCurve.getRate(normTime);
                double Z = norm(rng);
                paths[i][j] = paths[i][j - 1] * std::exp((r_local - q - 0.5 * sigma * sigma) * dt + sigma * std::sqrt(dt) * Z);
            }
        }
        // Initialize cash flows at maturity.
        std::vector<double> cashFlow(NPaths, 0.0);
        for (int i = 0; i < NPaths; i++) {
            cashFlow[i] = (opt.getOptionType() == Option::OptionType::Call) ?
                std::max(paths[i][NSteps] - K, 0.0) :
                std::max(K - paths[i][NSteps], 0.0);
        }
        // Vector to record exercise time (initially set to maturity).
        std::vector<int> exerciseTime(NPaths, NSteps);

        // Backward induction using Longstaff-Schwartz.
        for (int t = NSteps - 1; t >= 1; t--) {
            std::vector<int> inTheMoneyIndices;
            std::vector<double> X; // Underlying prices for in-the-money paths.
            std::vector<double> Y; // Discounted cash flows for in-the-money paths.
            for (int i = 0; i < NPaths; i++) {
                double intrinsic = (opt.getOptionType() == Option::OptionType::Call) ?
                    std::max(paths[i][t] - K, 0.0) :
                    std::max(K - paths[i][t], 0.0);
                if (intrinsic > 0.0 && exerciseTime[i] == NSteps) {
                    inTheMoneyIndices.push_back(i);
                    X.push_back(paths[i][t]);
                    // Compute discount factor from t to exerciseTime using variable rates.
                    double disc = 1.0;
                    for (int k = t; k < exerciseTime[i]; k++) {
                        double normTime_k = ((T_effective - k * dt) / T_effective);
                        double r_local = (config_.yieldCurve.getData().empty()) ? r_default : config_.yieldCurve.getRate(normTime_k);
                        disc *= std::exp(-r_local * dt);
                    }
                    Y.push_back(cashFlow[i] * disc);
                }
            }
            if (!inTheMoneyIndices.empty()) {
                int n = inTheMoneyIndices.size();
                double sum1 = 0.0, sumX = 0.0, sumX2 = 0.0, sumX3 = 0.0, sumX4 = 0.0;
                double sumY = 0.0, sumXY = 0.0, sumX2Y = 0.0;
                for (int k = 0; k < n; k++) {
                    double x = X[k];
                    double y = Y[k];
                    sum1 += 1.0;
                    sumX += x;
                    sumX2 += x * x;
                    sumX3 += x * x * x;
                    sumX4 += x * x * x * x;
                    sumY += y;
                    sumXY += x * y;
                    sumX2Y += x * x * y;
                }
                double D = sum1 * (sumX2 * sumX4 - sumX3 * sumX3)
                    - sumX * (sumX * sumX4 - sumX2 * sumX3)
                    + sumX2 * (sumX * sumX3 - sumX2 * sumX2);
                if (std::abs(D) < 1e-10) {
                    continue;
                }
                double a0 = (sumY * (sumX2 * sumX4 - sumX3 * sumX3)
                    - sumX * (sumXY * sumX4 - sumX3 * sumX2Y)
                    + sumX2 * (sumXY * sumX3 - sumX2 * sumX2Y)) / D;
                double a1 = (sum1 * (sumXY * sumX4 - sumX3 * sumX2Y)
                    - sumY * (sumX * sumX4 - sumX2 * sumX3)
                    + sumX2 * (sumX * sumX2Y - sumX2 * sumXY)) / D;
                double a2 = (sum1 * (sumX2 * sumX2Y - sumX3 * sumXY)
                    - sumX * (sumX * sumX2Y - sumX2 * sumXY)
                    + sumY * (sumX * sumX3 - sumX2 * sumX2)) / D;

                for (int idx = 0; idx < n; idx++) {
                    int i = inTheMoneyIndices[idx];
                    double x = paths[i][t];
                    double continuation = a0 + a1 * x + a2 * x * x;
                    double immediate = (opt.getOptionType() == Option::OptionType::Call) ?
                        std::max(x - K, 0.0) : std::max(K - x, 0.0);
                    if (immediate > continuation) {
                        cashFlow[i] = immediate;
                        exerciseTime[i] = t;
                    }
                }
            }
        }
        // Final discounting from time 0 to exerciseTime for each path using variable rates.
        double sumPayoffs = 0.0;
        for (int i = 0; i < NPaths; i++) {
            double disc = 1.0;
            for (int k = 0; k < exerciseTime[i]; k++) {
                double normTime_k = (T_effective - k * dt) / T_effective;
                double r_local = (config_.yieldCurve.getData().empty()) ? r_default : config_.yieldCurve.getRate(normTime_k);
                disc *= std::exp(-r_local * dt);
            }
            sumPayoffs += cashFlow[i] * disc;
        }
        return sumPayoffs / NPaths;
    }
}

/**
 * @brief Computes the Greeks of the option using finite differences applied to the Monte Carlo pricer.
 *
 * This function estimates the Greeks (Delta, Gamma, Vega, Theta, and Rho) by perturbing the input parameters
 * and recalculating the option price.
 *
 * @param opt The option to evaluate.
 * @return A Greeks structure containing the calculated values.
 */
Greeks MonteCarloPricer::computeGreeks(const Option& opt) const {
    double h = 0.01 * opt.getUnderlying();
    double volStep = 0.01;
    double rStep = 0.001;
    double timeStep = 1.0 / 365.0; // One day

    double basePrice = price(opt);

    // --- Delta ---
    Option opt_up = opt;
    Option opt_down = opt;
    opt_up.setUnderlying(opt.getUnderlying() + h);
    opt_down.setUnderlying(opt.getUnderlying() - h);
    double price_up = price(opt_up);
    double price_down = price(opt_down);
    double delta = (price_up - price_down) / (2 * h);

    // --- Gamma ---
    double gamma = (price_up - 2 * basePrice + price_down) / (h * h);

    // --- Vega ---
    Option opt_vol_up = opt;
    Option opt_vol_down = opt;
    opt_vol_up.setVolatility(opt.getVolatility() + volStep);
    opt_vol_down.setVolatility(opt.getVolatility() - volStep);
    double vega = (price(opt_vol_up) - price(opt_vol_down)) / (2 * volStep);

    // --- Theta ---
    // Création d'une nouvelle configuration avec une maturité légèrement réduite
    PricingConfiguration config_T_down = config_;
    config_T_down.maturity = config_.maturity - timeStep;

    // Création d'un nouveau pricer avec cette configuration
    MonteCarloPricer pricer_T_down(config_T_down);
    double price_T_down = pricer_T_down.price(opt);

    // Approximation par différences finies avec un décalage négatif
    double theta = (price_T_down - basePrice) / (-timeStep);  // On divise par -timeStep car Theta est généralement négatif



    // --- Rho ---
    PricingConfiguration config_r_up = config_;
    PricingConfiguration config_r_down = config_;
    if (!config_.yieldCurve.getData().empty()) {
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
    MonteCarloPricer pricer_r_up(config_r_up);
    MonteCarloPricer pricer_r_down(config_r_down);
    double price_r_up = pricer_r_up.price(opt);
    double price_r_down = pricer_r_down.price(opt);
    double rho = (price_r_up - price_r_down) / (2 * rStep);


    Greeks greeks;
    greeks.delta = delta;
    greeks.gamma = gamma;
    greeks.vega = vega;
    greeks.theta = theta;
    greeks.rho = rho;
    return greeks;
}
