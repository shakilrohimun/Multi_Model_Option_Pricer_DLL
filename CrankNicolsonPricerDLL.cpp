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
            // Recharge la yield curve à chaque appel :
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
            // Recharge la yield curve à chaque appel :
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
