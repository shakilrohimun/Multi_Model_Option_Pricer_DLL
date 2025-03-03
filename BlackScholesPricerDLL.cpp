#include "pch.h"
#include "BlackScholesPricerDLL.hpp"
#include "BlackScholesPricer.hpp"
#include "Option.hpp"
#include "PricingConfiguration.hpp"
#include "DateConverter.hpp"
#include <stdexcept>
#include <cstring>
#include <cmath>

extern "C" {

    // Parameters:
    // S: Underlying price
    // K: Strike price
    // T: Maturity (in years)
    // r: Risk-free rate (constant)
    // sigma: Volatility
    // q: Dividend yield
    // optionType: 0 for Call, 1 for Put
    // optionStyle: 0 for European, 1 for American (only European supported here)
    // calculationDate: A null-terminated string (YYYY-MM-DD). If empty, today's date is used.
    double __stdcall PriceOption(
        double S, double K, double T, double r, double sigma, double q,
        int optionType, int optionStyle, const char* calculationDate)
    {
        try {
            PricingConfiguration config;
            if (calculationDate == nullptr || strlen(calculationDate) == 0)
                config.calculationDate = DateConverter::getTodayDate();
            else
                config.calculationDate = calculationDate;

            config.maturity = T;
            config.riskFreeRate = r;

            BlackScholesPricer pricer(config);
            Option opt(S, K, sigma, q,
                (optionType == 0 ? Option::OptionType::Call : Option::OptionType::Put),
                (optionStyle == 0 ? Option::OptionStyle::European : Option::OptionStyle::American));

            if (opt.getOptionStyle() != Option::OptionStyle::European)
                throw std::runtime_error("BlackScholesPricer supports only European options.");

            return pricer.price(opt);
        }
        catch (const std::exception& ex) {
            return -1.0;
        }
    }

    // Computes the Greeks of the option. The computed values are written to the pointers provided.
    // If an error occurs, the pointers are set to NAN.
    void __stdcall ComputeOptionGreeks(
        double S, double K, double T, double r, double sigma, double q,
        int optionType, int optionStyle, const char* calculationDate,
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

            BlackScholesPricer pricer(config);
            Option opt(S, K, sigma, q,
                (optionType == 0 ? Option::OptionType::Call : Option::OptionType::Put),
                (optionStyle == 0 ? Option::OptionStyle::European : Option::OptionStyle::American));

            if (opt.getOptionStyle() != Option::OptionStyle::European)
                throw std::runtime_error("BlackScholesPricer supports only European options.");

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
