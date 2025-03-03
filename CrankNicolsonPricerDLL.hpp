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

    // Function to compute the option price using the Crank-Nicolson model.
    // Parameters:
    //  S: Underlying price
    //  K: Strike price
    //  T: Maturity (in years)
    //  r: Risk-free rate
    //  sigma: Volatility
    //  q: Dividend yield
    //  optionType: 0 = Call, 1 = Put
    //  optionStyle: 0 = European, 1 = American
    //  calculationDate: A null-terminated string in "YYYY-MM-DD"; if empty, today's date is used.
    //  crankTimeSteps: Number of time discretization steps
    //  crankSpotSteps: Number of spot discretization steps
    //  S_max: Maximum underlying asset price (if 0, default is computed inside the model)
    CRANK_NICOLSON_API double __stdcall PriceOptionCrankNicolson(
        double S, double K, double T, double r, double sigma, double q,
        int optionType, int optionStyle, const char* calculationDate,
        int crankTimeSteps, int crankSpotSteps, double S_max);

    // Function to compute the Greeks using the Crank-Nicolson model.
    // The computed Greeks (delta, gamma, vega, theta, rho) are written to the pointers provided.
    CRANK_NICOLSON_API void __stdcall ComputeOptionGreeksCrankNicolson(
        double S, double K, double T, double r, double sigma, double q,
        int optionType, int optionStyle, const char* calculationDate,
        int crankTimeSteps, int crankSpotSteps, double S_max,
        double* delta, double* gamma, double* vega, double* theta, double* rho);

#ifdef __cplusplus
}
#endif

#endif // CRANK_NICOLSON_PRICER_DLL_HPP
