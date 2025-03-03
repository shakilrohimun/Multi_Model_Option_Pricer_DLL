#include "pch.h"
#include "BinomialPricerDLL.hpp"
#include "BinomialPricer.hpp"     // Doit contenir la déclaration de la classe BinomialPricer
#include "Option.hpp"
#include "PricingConfiguration.hpp"
#include "DateConverter.hpp"
#include <stdexcept>
#include <cstring>
#include <cmath>

extern "C" {

    double __stdcall PriceOptionBinomial(
        double S, double K, double T, double r, double sigma, double q,
        int optionType, int optionStyle, const char* calculationDate,
        int binomialSteps)
    {
        try {
            PricingConfiguration config;
            // Utiliser la date du jour si aucun paramètre n'est fourni.
            if (calculationDate == nullptr || strlen(calculationDate) == 0)
                config.calculationDate = DateConverter::getTodayDate();
            else
                config.calculationDate = calculationDate;

            config.maturity = T;
            config.riskFreeRate = r;
            // Recharger la yield curve à chaque appel (chemin à adapter si nécessaire)
            config.yieldCurve.loadFromFile("C:\\Users\\shaki\\OneDrive\\Bureau\\YieldCurveData.txt");

            // Spécifique au modèle binomial : nombre d'étapes de l'arbre.
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

    void __stdcall ComputeOptionGreeksBinomial(
        double S, double K, double T, double r, double sigma, double q,
        int optionType, int optionStyle, const char* calculationDate,
        int binomialSteps,
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
            // Recharge la yield curve à chaque appel (chemin à adapter)
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
