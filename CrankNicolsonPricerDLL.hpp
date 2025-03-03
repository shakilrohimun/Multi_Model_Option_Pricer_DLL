#ifndef CRANK_NICOLSON_PRICER_DLL_HPP
#define CRANK_NICOLSON_PRICER_DLL_HPP

#ifdef CRANK_NICOLSON_PRICER_DLL_EXPORTS
#define CRANK_NICOLSON_API __declspec(dllexport)
#else
#define CRANK_NICOLSON_API __declspec(dllimport)
#endif

#include "pch.h"

#ifdef __cplusplus
extern "C" {
#endif

    /**
     * @brief Computes the option price using the Crank-Nicolson finite difference method.
     *
     * This function calculates the theoretical price of an option by numerically solving the
     * Black-Scholes partial differential equation using the Crank-Nicolson scheme. It discretizes
     * both the time to maturity and the underlying asset price into a finite grid and applies an
     * implicit method that offers a balance between stability and accuracy.
     *
     * @param S The current underlying asset price.
     * @param K The strike price of the option.
     * @param T The time to maturity in years.
     * @param r The risk-free interest rate, expressed as an annual rate.
     * @param sigma The volatility of the underlying asset, expressed as an annual rate.
     * @param q The dividend yield of the underlying asset.
     * @param optionType An integer representing the type of the option (0 for Call, 1 for Put).
     * @param optionStyle An integer representing the option style (0 for European, 1 for American).
     *                    Note: The handling of American options may require special treatment.
     * @param calculationDate A null-terminated string representing the calculation date in "YYYY-MM-DD" format.
     *                        If empty, the current date is used as the default.
     * @param crankTimeSteps The number of time discretization steps to be used in the finite difference grid.
     * @param crankSpotSteps The number of spot (underlying asset price) discretization steps to be used in the grid.
     * @param S_max The maximum underlying asset price considered in the grid. If set to 0, a default value is computed internally.
     * @return The computed option price as a double.
     *
     * @details The Crank-Nicolson method is an implicit finite difference technique that averages the results of
     * the explicit and implicit schemes. This method is particularly effective for option pricing as it provides
     * stability even with larger time steps, making it suitable for a wide range of applications in financial modeling.
     */
    CRANK_NICOLSON_API double __stdcall PriceOptionCrankNicolson(
        double S, double K, double T, double r, double sigma, double q,
        int optionType, int optionStyle, const char* calculationDate,
        int crankTimeSteps, int crankSpotSteps, double S_max);

    /**
     * @brief Computes the Greeks of an option using the Crank-Nicolson method.
     *
     * This function calculates the sensitivity measures (Greeks) of an option's price with respect to changes
     * in various parameters by employing the Crank-Nicolson finite difference method. The computed Greeks include:
     * Delta, Gamma, Vega, Theta, and Rho.
     *
     * @param S The current underlying asset price.
     * @param K The strike price of the option.
     * @param T The time to maturity in years.
     * @param r The risk-free interest rate.
     * @param sigma The volatility of the underlying asset.
     * @param q The dividend yield of the underlying asset.
     * @param optionType An integer indicating the type of the option (0 for Call, 1 for Put).
     * @param optionStyle An integer indicating the option style (0 for European, 1 for American).
     * @param calculationDate A null-terminated string in "YYYY-MM-DD" format representing the calculation date.
     *                        If empty, the current date is assumed.
     * @param crankTimeSteps The number of time discretization steps used in the finite difference grid.
     * @param crankSpotSteps The number of spot discretization steps used in the grid.
     * @param S_max The maximum underlying asset price considered in the grid. If set to 0, a default value is computed internally.
     * @param delta Pointer to a double where the computed Delta (sensitivity to the underlying price) will be stored.
     * @param gamma Pointer to a double where the computed Gamma (rate of change of Delta) will be stored.
     * @param vega Pointer to a double where the computed Vega (sensitivity to volatility) will be stored.
     * @param theta Pointer to a double where the computed Theta (sensitivity to time decay) will be stored.
     * @param rho Pointer to a double where the computed Rho (sensitivity to the risk-free rate) will be stored.
     *
     * @return This function does not return a value; the computed Greeks are output via the provided pointers.
     *
     * @details The function applies finite difference approximations on the grid produced by the Crank-Nicolson method
     * to evaluate the partial derivatives of the option price with respect to the input parameters. These Greeks are essential
     * for risk management, hedging strategies, and understanding how the option's price is affected by market conditions.
     */
    CRANK_NICOLSON_API void __stdcall ComputeOptionGreeksCrankNicolson(
        double S, double K, double T, double r, double sigma, double q,
        int optionType, int optionStyle, const char* calculationDate,
        int crankTimeSteps, int crankSpotSteps, double S_max,
        double* delta, double* gamma, double* vega, double* theta, double* rho);

#ifdef __cplusplus
}
#endif

#endif // CRANK_NICOLSON_PRICER_DLL_HPP
