/**
 * @file CrankNicolsonPricer.cpp
 * @brief Implementation of the CrankNicolsonPricer class using the Crank-Nicolson method.
 *
 * This file implements the CrankNicolsonPricer class which derives from the IOptionPricer interface.
 * It solves the Black-Scholes partial differential equation by discretizing time and the underlying asset price.
 * The method supports both European and American options (with a projection step for early exercise).
 * It uses additional configuration parameters (maturity, risk-free rate, discretization settings, S_max, and calculationDate)
 * provided via a PricingConfiguration object. The effective time to maturity is adjusted by subtracting the
 * time offset (in years) computed from the calculationDate using DateConverter.
 */

#include "pch.h"
#include "CrankNicolsonPricer.hpp"
#include "Option.hpp"
#include "DateConverter.hpp" // For date conversion functions
#include <vector>
#include <cmath>
#include <algorithm>
#include <stdexcept>

 /**
  * @brief Default constructor of CrankNicolsonPricer.
  */
CrankNicolsonPricer::CrankNicolsonPricer() {
    // No explicit configuration provided; default configuration is assumed.
}

/**
 * @brief Destructor of CrankNicolsonPricer.
 */
CrankNicolsonPricer::~CrankNicolsonPricer() {
    // No dynamic cleanup required.
}

/**
 * @brief Constructs a CrankNicolsonPricer with a custom pricing configuration.
 *
 * This constructor initializes the CrankNicolsonPricer using the provided PricingConfiguration object.
 * The configuration parameters (such as calculation date, maturity, risk-free rate, time discretization,
 * spatial discretization, and S_max) are stored internally and used in the pricing calculations.
 *
 * @param config A PricingConfiguration object containing custom parameters.
 */
CrankNicolsonPricer::CrankNicolsonPricer(const PricingConfiguration& config)
    : config_(config)
{
    // Configuration parameters are now stored in config_
}

/**
 * @brief Computes the option price using the Crank-Nicolson method.
 *
 * This method solves the Black-Scholes PDE by discretizing time and the underlying asset price.
 * It uses configuration parameters for maturity, risk-free rate, and discretization. If a calculation date
 * is provided, the effective time to maturity is computed as:
 *
 *    T_effective = T - offset,
 *
 * where offset is the number of years between config_.calculationDate and today.
 *
 * Moreover, at each time step the local risk-free rate is determined by interpolating the yield curve.
 * If the yield curve is empty, the default risk-free rate from config_ is used.
 *
 * @param opt The option to be priced.
 * @return The computed option price.
 */
double CrankNicolsonPricer::price(const Option& opt) const {
    // Option parameters
    double S0 = opt.getUnderlying();    // Underlying price
    double K = opt.getStrike();         // Strike price
    double sigma = opt.getVolatility(); // Volatility
    double q = opt.getDividend();       // Continuous dividend yield

    // Retrieve maturity and default risk-free rate from configuration.
    double T = config_.maturity;
    double r_default = config_.riskFreeRate;

    // Adjust effective time to maturity using calculationDate if provided.
    double T_effective = T;
    if (!config_.calculationDate.empty()) {
        auto calcDate = DateConverter::parseDate(config_.calculationDate);
        auto today = std::chrono::system_clock::now();
        double offset = DateConverter::yearsBetween(calcDate, today);
        T_effective = T - offset;
    }

    // Retrieve discretization parameters.
    const int M = config_.crankSpotSteps;  // Number of spatial steps
    const int N = config_.crankTimeSteps;    // Number of time steps
    double Smax = (config_.S_max > 0.0) ? config_.S_max : std::max(3.0 * K, 3.0 * S0);
    double dS = Smax / M;
    double dt = T_effective / N;           // Use effective time here

    // Build spatial grid.
    std::vector<double> S(M + 1);
    for (int j = 0; j <= M; ++j) {
        S[j] = j * dS;
    }

    // Terminal condition: payoff at maturity.
    std::vector<double> V(M + 1, 0.0);
    if (opt.getOptionType() == Option::OptionType::Call) {
        for (int j = 0; j <= M; ++j) {
            V[j] = std::max(S[j] - K, 0.0);
        }
    }
    else { // Put option
        for (int j = 0; j <= M; ++j) {
            V[j] = std::max(K - S[j], 0.0);
        }
    }

    // Temporary vector for current time step.
    std::vector<double> newV(M + 1, 0.0);
    // Vectors for the tridiagonal system.
    std::vector<double> a(M - 1, 0.0); // Coefficient for V_{j-1}^{n+1}
    std::vector<double> b(M - 1, 0.0); // Coefficient for V_{j}^{n+1}
    std::vector<double> c(M - 1, 0.0); // Coefficient for V_{j+1}^{n+1}
    std::vector<double> d_vec(M - 1, 0.0); // Right-hand side

    // Backward induction loop (n from N-1 to 0)
    for (int n = N - 1; n >= 0; --n) {
        double t = n * dt;
        // Compute normalized time (for yield curve interpolation).
        // Here, we define normTime such that normTime = 1 at t = 0 (start) and 0 at t = T_effective (maturity)
        double normTime = (T_effective - t) / T_effective;
        // Obtain local risk-free rate from yield curve (if available); otherwise, use default.
        double r_local = (!config_.yieldCurve.getData().empty()) ? config_.yieldCurve.getRate(normTime) : r_default;

        // Boundary conditions at time t.
        if (opt.getOptionType() == Option::OptionType::Call) {
            newV[0] = 0.0;
            newV[M] = Smax - K * std::exp(-r_local * (T_effective - t));
        }
        else {
            newV[0] = K * std::exp(-r_local * (T_effective - t));
            newV[M] = 0.0;
        }

        // Form the tridiagonal system for interior nodes j = 1 to M-1.
        for (int j = 1; j < M; ++j) {
            double S_j = S[j];
            // Use local risk-free rate in coefficients.
            double A = 0.5 * dt * (0.5 * sigma * sigma * S_j * S_j / (dS * dS) - (r_local - q) * S_j / (2 * dS));
            double B = 1.0 + 0.5 * dt * (sigma * sigma * S_j * S_j / (dS * dS) + r_local);
            double C = 0.5 * dt * (0.5 * sigma * sigma * S_j * S_j / (dS * dS) + (r_local - q) * S_j / (2 * dS));

            // Explicit part coefficients.
            double D_coef = A;
            double E_coef = 1.0 - 0.5 * dt * (sigma * sigma * S_j * S_j / (dS * dS) + r_local);
            double F = C;

            a[j - 1] = -A;
            b[j - 1] = B;
            c[j - 1] = -C;
            d_vec[j - 1] = D_coef * V[j - 1] + E_coef * V[j] + F * V[j + 1];
        }

        // Adjust right-hand side for boundary conditions.
        d_vec[0] -= (-a[0]) * newV[0];
        d_vec[M - 2] -= (-c[M - 2]) * newV[M];

        // Solve the tridiagonal system using the Thomas algorithm.
        std::vector<double> c_prime(M - 1, 0.0);
        std::vector<double> d_prime(M - 1, 0.0);

        c_prime[0] = c[0] / b[0];
        d_prime[0] = d_vec[0] / b[0];

        for (int j = 1; j < M - 1; ++j) {
            double m = b[j] - a[j] * c_prime[j - 1];
            c_prime[j] = c[j] / m;
            d_prime[j] = (d_vec[j] - a[j] * d_prime[j - 1]) / m;
        }

        newV[M - 1] = d_prime[M - 2];
        for (int j = M - 2; j >= 1; --j) {
            newV[j] = d_prime[j - 1] - c_prime[j - 1] * newV[j + 1];
        }

        // Update V for the next time step.
        for (int j = 0; j <= M; ++j) {
            V[j] = newV[j];
        }

        // Projection step for American options: update value with intrinsic payoff.
        if (opt.getOptionStyle() == Option::OptionStyle::American) {
            for (int j = 0; j <= M; ++j) {
                double payoff = 0.0;
                if (opt.getOptionType() == Option::OptionType::Call) {
                    payoff = std::max(S[j] - K, 0.0);
                }
                else {
                    payoff = std::max(K - S[j], 0.0);
                }
                V[j] = std::max(V[j], payoff);
            }
        }
    }

    // Linear interpolation to obtain the option price at S0.
    double price = 0.0;
    if (S0 <= 0) {
        price = V[0];
    }
    else if (S0 >= Smax) {
        price = V[M];
    }
    else {
        int j = static_cast<int>(S0 / dS);
        double weight = (S0 - S[j]) / dS;
        price = V[j] * (1.0 - weight) + V[j + 1] * weight;
    }

    return price;
}

/**
 * @brief Computes the Greeks of the option using finite differences applied to the Crank-Nicolson pricer.
 *
 * The Greeks (Delta, Gamma, Vega, Theta, and Rho) are estimated by perturbing the input parameters
 * and recalculating the option price.
 *
 * @param opt The option to evaluate.
 * @return A Greeks structure containing the calculated values.
 */
Greeks CrankNicolsonPricer::computeGreeks(const Option& opt) const {
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
    // Utiliser un pas de temps d'un jour (en années)
    double CranktimeStep = 1.0 / 365.0;
    // Créer une nouvelle configuration avec la maturité réduite de timeStep
    PricingConfiguration config_T_down = config_;
    config_T_down.maturity = config_.maturity - CranktimeStep;
    // Recalculer le prix avec cette configuration modifiée
    CrankNicolsonPricer pricer_T_down(config_T_down);
    double price_T_down = pricer_T_down.price(opt);
    // Calculer Theta avec une différence finie arrière
    double theta = (price_T_down - basePrice) / (-CranktimeStep);


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
    CrankNicolsonPricer pricer_r_up(config_r_up);
    CrankNicolsonPricer pricer_r_down(config_r_down);
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
