#include "pch.h"
#include "MonteCarloPricerDLL.hpp"
#include "MonteCarloPricer.hpp"   // This header must contain the declaration of the MonteCarloPricer class
#include "Option.hpp"
#include "PricingConfiguration.hpp"
#include "DateConverter.hpp"
#include <stdexcept>
#include <cstring>
#include <cmath>

extern "C" {

    /**
     * @brief Computes the option price using the Monte Carlo simulation model.
     *
     * This function sets up the pricing configuration, reloads the yield curve data,
     * and creates a MonteCarloPricer instance to compute the option price.
     *
     * Parameters:
     *   S - Underlying price.
     *   K - Strike price.
     *   T - Maturity (in years).
     *   r - Risk-free rate (constant).
     *   sigma - Volatility.
     *   q - Dividend yield.
     *   optionType - 0 for Call, 1 for Put.
     *   optionStyle - 0 for European, 1 for American.
     *   calculationDate - A null-terminated string (YYYY-MM-DD). If empty, today's date is used.
     *   mcNumPaths - Number of simulation paths.
     *   mcTimeStepsPerPath - Number of time steps per simulation path.
     *
     * @return The computed option price, or -1.0 if an error occurs.
     */
    double __stdcall PriceOptionMonteCarlo(
        double S, double K, double T, double r, double sigma, double q,
        int optionType, int optionStyle, const char* calculationDate,
        int mcNumPaths, int mcTimeStepsPerPath)
    {
        try {
            PricingConfiguration config;
            // If no date is specified, use today's date.
            if (calculationDate == nullptr || strlen(calculationDate) == 0)
                config.calculationDate = DateConverter::getTodayDate();
            else
                config.calculationDate = calculationDate;

            config.maturity = T;
            config.riskFreeRate = r;
            // Reload the yield curve at each call (adjust the path if necessary)
            config.yieldCurve.loadFromFile("C:\\Users\\shaki\\OneDrive\\Bureau\\YieldCurveData.txt");

            // Monte Carlo specific parameters.
            config.mcNumPaths = mcNumPaths;
            config.mcTimeStepsPerPath = mcTimeStepsPerPath;

            MonteCarloPricer pricer(config);

            Option opt(S, K, sigma, q,
                (optionType == 0 ? Option::OptionType::Call : Option::OptionType::Put),
                (optionStyle == 0 ? Option::OptionStyle::European : Option::OptionStyle::American));

            double price = pricer.price(opt);
            return price;
        }
        catch (const std::exception& ex) {
            return -1.0;
        }
    }

    /**
     * @brief Computes the Greeks of the option using the Monte Carlo simulation model.
     *
     * This function sets up the pricing configuration, reloads the yield curve data,
     * and creates a MonteCarloPricer instance to compute the Greeks.
     * The computed values are written to the pointers provided.
     *
     * Parameters:
     *   S - Underlying price.
     *   K - Strike price.
     *   T - Maturity (in years).
     *   r - Risk-free rate (constant).
     *   sigma - Volatility.
     *   q - Dividend yield.
     *   optionType - 0 for Call, 1 for Put.
     *   optionStyle - 0 for European, 1 for American.
     *   calculationDate - A null-terminated string (YYYY-MM-DD). If empty, today's date is used.
     *   mcNumPaths - Number of simulation paths.
     *   mcTimeStepsPerPath - Number of time steps per simulation path.
     *   delta, gamma, vega, theta, rho - Pointers to store the computed Greeks.
     */
    void __stdcall ComputeOptionGreeksMonteCarlo(
        double S, double K, double T, double r, double sigma, double q,
        int optionType, int optionStyle, const char* calculationDate,
        int mcNumPaths, int mcTimeStepsPerPath,
        double* delta, double* gamma, double* vega, double* theta, double* rho)
    {
        try {
            PricingConfiguration config;
            if (calculationDate == nullptr || strlen(calculationDate) == 0)
                config.calculationDate = DateConverter::getTodayDate();
            else
                config.calculationDate = calculationDate;

            config.maturity = T;
            config.riskFreeRate = r;
            // Reload the yield curve at each call (adjust the path if necessary)
            config.yieldCurve.loadFromFile("C:\\Users\\shaki\\OneDrive\\Bureau\\YieldCurveData.txt");

            config.mcNumPaths = mcNumPaths;
            config.mcTimeStepsPerPath = mcTimeStepsPerPath;

            MonteCarloPricer pricer(config);

            Option opt(S, K, sigma, q,
                (optionType == 0 ? Option::OptionType::Call : Option::OptionType::Put),
                (optionStyle == 0 ? Option::OptionStyle::European : Option::OptionStyle::American));

            Greeks g = pricer.computeGreeks(opt);
            if (delta) *delta = g.delta;
            if (gamma) *gamma = g.gamma;
            if (vega)  *vega = g.vega;
            if (theta) *theta = g.theta;
            if (rho)   *rho = g.rho;
        }
        catch (const std::exception& ex) {
            if (delta) *delta = NAN;
            if (gamma) *gamma = NAN;
            if (vega)  *vega = NAN;
            if (theta) *theta = NAN;
            if (rho)   *rho = NAN;
        }
    }

} // extern "C"
