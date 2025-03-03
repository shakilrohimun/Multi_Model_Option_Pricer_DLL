#include "pch.h"
#include "CrankNicolsonPricerDLL.hpp"
#include "CrankNicolsonPricer.hpp"
#include "Option.hpp"
#include "PricingConfiguration.hpp"
#include "DateConverter.hpp"
#include <stdexcept>
#include <cstring>
#include <cmath>

extern "C" {

    /**
     * @brief Computes the option price using the Crank-Nicolson model.
     *
     * This function sets up the pricing configuration, reloads the yield curve data,
     * and creates a CrankNicolsonPricer instance to compute the option price.
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
     *   crankTimeSteps - Number of time steps for discretization.
     *   crankSpotSteps - Number of spatial steps for discretization.
     *   S_max - Maximum underlying price (if zero, it is computed from S and K).
     *
     * @return The computed option price, or -1.0 if an error occurs.
     */
    double __stdcall PriceOptionCrankNicolson(
        double S, double K, double T, double r, double sigma, double q,
        int optionType, int optionStyle, const char* calculationDate,
        int crankTimeSteps, int crankSpotSteps, double S_max)
    {
        try {
            PricingConfiguration config;
            // If calculationDate is empty, use today's date.
            if (calculationDate == nullptr || strlen(calculationDate) == 0)
                config.calculationDate = DateConverter::getTodayDate();
            else
                config.calculationDate = calculationDate;

            config.maturity = T;
            config.riskFreeRate = r;
            // Reload the yield curve on each call:
            config.yieldCurve.loadFromFile("C:\\Users\\shaki\\OneDrive\\Bureau\\YieldCurveData.txt");

            config.crankTimeSteps = crankTimeSteps;
            config.crankSpotSteps = crankSpotSteps;
            config.S_max = S_max;

            CrankNicolsonPricer pricer(config);

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
     * @brief Computes the Greeks of the option using the Crank-Nicolson model.
     *
     * This function sets up the pricing configuration, reloads the yield curve data,
     * and creates a CrankNicolsonPricer instance to compute the Greeks.
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
     *   crankTimeSteps - Number of time steps for discretization.
     *   crankSpotSteps - Number of spatial steps for discretization.
     *   S_max - Maximum underlying price (if zero, it is computed from S and K).
     *   delta, gamma, vega, theta, rho - Pointers to store the computed Greeks.
     */
    void __stdcall ComputeOptionGreeksCrankNicolson(
        double S, double K, double T, double r, double sigma, double q,
        int optionType, int optionStyle, const char* calculationDate,
        int crankTimeSteps, int crankSpotSteps, double S_max,
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
            // Reload the yield curve on each call:
            config.yieldCurve.loadFromFile("C:\\Users\\shaki\\OneDrive\\Bureau\\YieldCurveData.txt");
            config.crankTimeSteps = crankTimeSteps;
            config.crankSpotSteps = crankSpotSteps;
            config.S_max = S_max;

            CrankNicolsonPricer pricer(config);

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
