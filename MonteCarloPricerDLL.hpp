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

    // Fonction pour calculer le prix d'une option à l'aide du modèle Monte Carlo.
    // Paramètres :
    //  S: Prix de l'actif sous-jacent
    //  K: Prix d'exercice
    //  T: Maturité (en années)
    //  r: Taux sans risque
    //  sigma: Volatilité
    //  q: Rendement du dividende
    //  optionType: 0 = Call, 1 = Put
    //  optionStyle: 0 = Européenne, 1 = Américaine
    //  calculationDate: Date de calcul au format "YYYY-MM-DD" (si vide, la date du jour est utilisée)
    //  mcNumPaths: Nombre de trajectoires de simulation
    //  mcTimeStepsPerPath: Nombre de pas de temps par trajectoire
    MONTE_CARLO_PRICER_API double __stdcall PriceOptionMonteCarlo(
        double S, double K, double T, double r, double sigma, double q,
        int optionType, int optionStyle, const char* calculationDate,
        int mcNumPaths, int mcTimeStepsPerPath);

    // Fonction pour calculer les Greeks à l'aide du modèle Monte Carlo.
    // Les valeurs calculées (delta, gamma, vega, theta, rho) sont renvoyées via les pointeurs fournis.
    MONTE_CARLO_PRICER_API void __stdcall ComputeOptionGreeksMonteCarlo(
        double S, double K, double T, double r, double sigma, double q,
        int optionType, int optionStyle, const char* calculationDate,
        int mcNumPaths, int mcTimeStepsPerPath,
        double* delta, double* gamma, double* vega, double* theta, double* rho);

#ifdef __cplusplus
}
#endif

#endif // MONTE_CARLO_PRICER_DLL_HPP
