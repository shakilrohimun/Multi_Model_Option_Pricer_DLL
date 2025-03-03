#ifndef MONTE_CARLO_PRICER_DLL_HPP
#define MONTE_CARLO_PRICER_DLL_HPP

#ifdef MONTE_CARLO_PRICER_DLL_EXPORTS
#define MONTE_CARLO_PRICER_API __declspec(dllexport)
#else
#define MONTE_CARLO_PRICER_API __declspec(dllimport)
#endif

#include "pch.h"

#ifdef __cplusplus
extern "C" {
#endif

    /**
     * @brief Computes the price of an option using the Monte Carlo simulation model.
     *
     * This function calculates the theoretical price of an option by generating multiple simulated
     * price paths for the underlying asset. For European options, the payoff is evaluated at maturity,
     * while for American options, simulation techniques that allow for early exercise decisions are employed.
     *
     * @param S The current price of the underlying asset.
     * @param K The strike price of the option.
     * @param T The time to maturity (in years).
     * @param r The risk-free interest rate.
     * @param sigma The volatility of the underlying asset.
     * @param q The dividend yield of the underlying asset.
     * @param optionType An integer indicating the type of option (0 for Call, 1 for Put).
     * @param optionStyle An integer indicating the style of the option (0 for European, 1 for American).
     * @param calculationDate A null-terminated string representing the calculation date in "YYYY-MM-DD" format.
     *                        If empty, the current date is used.
     * @param mcNumPaths The number of simulation paths to generate.
     * @param mcTimeStepsPerPath The number of time steps for each simulation path.
     * @return The computed option price as a double.
     */
    MONTE_CARLO_PRICER_API double __stdcall PriceOptionMonteCarlo(
        double S, double K, double T, double r, double sigma, double q,
        int optionType, int optionStyle, const char* calculationDate,
        int mcNumPaths, int mcTimeStepsPerPath);

    /**
     * @brief Computes the Greeks of an option using the Monte Carlo simulation model.
     *
     * This function calculates key sensitivity measures (Greeks) such as Delta, Gamma, Vega, Theta, and Rho
     * by simulating numerous price paths for the underlying asset. The computed values are output via the
     * pointer parameters provided.
     *
     * @param S The current price of the underlying asset.
     * @param K The strike price of the option.
     * @param T The time to maturity (in years).
     * @param r The risk-free interest rate.
     * @param sigma The volatility of the underlying asset.
     * @param q The dividend yield of the underlying asset.
     * @param optionType An integer representing the type of option (0 for Call, 1 for Put).
     * @param optionStyle An integer representing the option style (0 for European, 1 for American).
     * @param calculationDate A null-terminated string in "YYYY-MM-DD" format representing the calculation date.
     *                        If empty, the current date is used.
     * @param mcNumPaths The number of simulation paths to generate.
     * @param mcTimeStepsPerPath The number of time steps per simulation path.
     * @param delta Pointer to a double where the computed Delta will be stored.
     * @param gamma Pointer to a double where the computed Gamma will be stored.
     * @param vega Pointer to a double where the computed Vega will be stored.
     * @param theta Pointer to a double where the computed Theta will be stored.
     * @param rho Pointer to a double where the computed Rho will be stored.
     */
    MONTE_CARLO_PRICER_API void __stdcall ComputeOptionGreeksMonteCarlo(
        double S, double K, double T, double r, double sigma, double q,
        int optionType, int optionStyle, const char* calculationDate,
        int mcNumPaths, int mcTimeStepsPerPath,
        double* delta, double* gamma, double* vega, double* theta, double* rho);

#ifdef __cplusplus
}
#endif

#endif // MONTE_CARLO_PRICER_DLL_HPP
