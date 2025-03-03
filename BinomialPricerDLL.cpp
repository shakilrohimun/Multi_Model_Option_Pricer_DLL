#include "pch.h"
#include "BinomialPricerDLL.hpp"
#include "BinomialPricer.hpp"     // Must contain the declaration of the BinomialPricer class
#include "Option.hpp"
#include "PricingConfiguration.hpp"
#include "DateConverter.hpp"
#include <stdexcept>
#include <cstring>
#include <cmath>

extern "C" {

    /**
     * @brief Computes the price of an option using the binomial model.
     *
     * This function sets up the pricing configuration, loads the yield curve data, creates a BinomialPricer instance,
     * and then computes the option price using the provided parameters.
     *
     * @param S The underlying asset price.
     * @param K The strike price.
     * @param T The maturity (in years).
     * @param r The risk-free interest rate.
     * @param sigma The volatility of the underlying asset.
     * @param q The dividend yield.
     * @param optionType An integer representing the option type (0 for Call, 1 for Put).
     * @param optionStyle An integer representing the option style (0 for European, 1 for American).
     * @param calculationDate A C-string representing the calculation date in "YYYY-MM-DD" format.
     *                        If empty, today's date is used.
     * @param binomialSteps The number of steps in the binomial tree.
     * @return The computed option price, or -1.0 if an error occurs.
     */
    double __stdcall PriceOptionBinomial(
        double S, double K, double T, double r, double sigma, double q,
        int optionType, int optionStyle, const char* calculationDate,
        int binomialSteps)
    {
        try {
            PricingConfiguration config;
            // Use today's date if no calculation date is provided.
            if (calculationDate == nullptr || strlen(calculationDate) == 0)
                config.calculationDate = DateConverter::getTodayDate();
            else
                config.calculationDate = calculationDate;

            config.maturity = T;
            config.riskFreeRate = r;
            // Reload the yield curve on each call (adjust the path if necessary)
            config.yieldCurve.loadFromFile("C:\\Users\\shaki\\OneDrive\\Bureau\\YieldCurveData.txt");

            // Specific to the binomial model: number of steps in the tree.
            config.binomialSteps = binomialSteps;

            BinomialPricer pricer(config);

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
     * @brief Computes the Greeks of an option using the binomial model.
     *
     * This function sets up the pricing configuration, loads the yield curve data, creates a BinomialPricer instance,
     * and computes the Greeks (delta, gamma, vega, theta, rho) by applying finite differences.
     *
     * @param S The underlying asset price.
     * @param K The strike price.
     * @param T The maturity (in years).
     * @param r The risk-free interest rate.
     * @param sigma The volatility of the underlying asset.
     * @param q The dividend yield.
     * @param optionType An integer representing the option type (0 for Call, 1 for Put).
     * @param optionStyle An integer representing the option style (0 for European, 1 for American).
     * @param calculationDate A C-string representing the calculation date in "YYYY-MM-DD" format.
     *                        If empty, today's date is used.
     * @param binomialSteps The number of steps in the binomial tree.
     * @param delta Pointer to store the computed delta.
     * @param gamma Pointer to store the computed gamma.
     * @param vega Pointer to store the computed vega.
     * @param theta Pointer to store the computed theta.
     * @param rho Pointer to store the computed rho.
     */
    void __stdcall ComputeOptionGreeksBinomial(
        double S, double K, double T, double r, double sigma, double q,
        int optionType, int optionStyle, const char* calculationDate,
        int binomialSteps,
        double* delta, double* gamma, double* vega, double* theta, double* rho)
    {
        try {
            PricingConfiguration config;
            // Use today's date if no calculation date is provided.
            if (calculationDate == nullptr || strlen(calculationDate) == 0)
                config.calculationDate = DateConverter::getTodayDate();
            else
                config.calculationDate = calculationDate;

            config.maturity = T;
            config.riskFreeRate = r;
            // Reload the yield curve on each call (adjust the path if necessary)
            config.yieldCurve.loadFromFile("C:\\Users\\shaki\\OneDrive\\Bureau\\YieldCurveData.txt");

            config.binomialSteps = binomialSteps;

            BinomialPricer pricer(config);

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
