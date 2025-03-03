#ifndef BLACK_SCHOLES_PRICER_DLL_HPP
#define BLACK_SCHOLES_PRICER_DLL_HPP

#ifdef BLACK_SCHOLES_PRICER_DLL_EXPORTS
#define BLACK_SCHOLES_API __declspec(dllexport)
#else
#define BLACK_SCHOLES_API __declspec(dllimport)
#endif

#include "pch.h"

extern "C" {

    /**
     * @brief Calculates the price of an option using the Black-Scholes formula.
     *
     * This function computes the theoretical price of a European option based on the Black-Scholes model.
     * It evaluates the necessary parameters using cumulative distribution functions for the standard normal distribution.
     *
     * @param S The current price of the underlying asset.
     * @param K The strike price of the option.
     * @param T The time to maturity in years.
     * @param r The annualized risk-free interest rate.
     * @param sigma The annualized volatility of the underlying asset.
     * @param q The dividend yield of the underlying asset.
     * @param optionType An integer indicating the type of option (0 for Call, 1 for Put).
     * @param optionStyle An integer representing the style of the option (0 for European; note that the Black-Scholes model is applicable only to European options).
     * @param calculationDate A string representing the calculation date in "YYYY-MM-DD" format. If an empty string is provided, the current date is assumed.
     * @return The computed option price as a double.
     *
     * @details The Black-Scholes model is based on several assumptions including constant volatility and interest rates, as well as a log-normal distribution of returns.
     * It is widely used for pricing European options where early exercise is not permitted.
     */
    BLACK_SCHOLES_API double __stdcall PriceOption(
        double S, double K, double T, double r, double sigma, double q,
        int optionType, int optionStyle, const char* calculationDate);

    /**
     * @brief Computes the Greeks of an option using the Black-Scholes model.
     *
     * This function calculates the sensitivity measures (Greeks) that describe how the price of an option
     * responds to changes in various underlying parameters. The Greeks provided include:
     * - Delta: Sensitivity of the option price to changes in the underlying asset's price.
     * - Gamma: Sensitivity of Delta with respect to changes in the underlying asset's price.
     * - Vega: Sensitivity of the option price to changes in volatility.
     * - Theta: Sensitivity of the option price to the passage of time.
     * - Rho: Sensitivity of the option price to changes in the risk-free interest rate.
     *
     * @param S The current price of the underlying asset.
     * @param K The strike price of the option.
     * @param T The time to maturity in years.
     * @param r The risk-free interest rate.
     * @param sigma The volatility of the underlying asset.
     * @param q The dividend yield of the underlying asset.
     * @param optionType An integer indicating the type of option (0 for Call, 1 for Put).
     * @param optionStyle An integer representing the option style (0 for European, as the Black-Scholes model is only valid for European options).
     * @param calculationDate A string representing the calculation date in "YYYY-MM-DD" format. If empty, the current date is used.
     * @param delta Pointer to a double where the computed Delta will be stored.
     * @param gamma Pointer to a double where the computed Gamma will be stored.
     * @param vega Pointer to a double where the computed Vega will be stored.
     * @param theta Pointer to a double where the computed Theta will be stored.
     * @param rho Pointer to a double where the computed Rho will be stored.
     *
     * @return This function does not return a value; the computed Greeks are output through the pointer parameters.
     *
     * @details The function uses analytical derivatives of the Black-Scholes formula to compute the Greeks.
     * These measures are critical for traders and risk managers to evaluate the risk exposure of an option position.
     */
    BLACK_SCHOLES_API void __stdcall ComputeOptionGreeks(
        double S, double K, double T, double r, double sigma, double q,
        int optionType, int optionStyle, const char* calculationDate,
        double* delta, double* gamma, double* vega, double* theta, double* rho);

}

#endif // BLACK_SCHOLES_PRICER_DLL_HPP
