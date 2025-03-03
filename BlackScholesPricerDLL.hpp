#ifndef BLACK_SCHOLES_PRICER_DLL_HPP
#define BLACK_SCHOLES_PRICER_DLL_HPP

#ifdef BLACK_SCHOLES_PRICER_DLL_EXPORTS
#define BLACK_SCHOLES_API __declspec(dllexport)
#else
#define BLACK_SCHOLES_API __declspec(dllimport)
#endif

#include "pch.h"

extern "C" {

    // Function to compute option price using Black-Scholes
    BLACK_SCHOLES_API double __stdcall PriceOption(
        double S, double K, double T, double r, double sigma, double q,
        int optionType, int optionStyle, const char* calculationDate);

    // Function to compute the Greeks of an option using Black-Scholes
    BLACK_SCHOLES_API void __stdcall ComputeOptionGreeks(
        double S, double K, double T, double r, double sigma, double q,
        int optionType, int optionStyle, const char* calculationDate,
        double* delta, double* gamma, double* vega, double* theta, double* rho);

}

#endif // BLACK_SCHOLES_PRICER_DLL_HPP
