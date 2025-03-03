#ifndef BINOMIAL_PRICER_DLL_HPP
#define BINOMIAL_PRICER_DLL_HPP

#ifdef BINOMIAL_PRICER_DLL_EXPORTS
#define BINOMIAL_PRICER_API __declspec(dllexport)
#else
#define BINOMIAL_PRICER_API __declspec(dllimport)
#endif

#include "pch.h"

#ifdef __cplusplus
extern "C" {
#endif

    // Fonction pour calculer le prix d'une option � l'aide du mod�le binomial (m�thode CRR).
    // Param�tres :
    //  S: Prix de l'actif sous-jacent
    //  K: Prix d'exercice
    //  T: Maturit� (en ann�es)
    //  r: Taux sans risque
    //  sigma: Volatilit�
    //  q: Rendement du dividende
    //  optionType: 0 = Call, 1 = Put
    //  optionStyle: 0 = Europ�enne, 1 = Am�ricaine
    //  calculationDate: Date de calcul au format "YYYY-MM-DD" (si vide, la date du jour est utilis�e)
    //  binomialSteps: Nombre d'�tapes de l'arbre binomial
    BINOMIAL_PRICER_API double __stdcall PriceOptionBinomial(
        double S, double K, double T, double r, double sigma, double q,
        int optionType, int optionStyle, const char* calculationDate,
        int binomialSteps);

    // Fonction pour calculer les Greeks � l'aide du mod�le binomial.
    // Les valeurs calcul�es (delta, gamma, vega, theta, rho) sont renvoy�es via les pointeurs fournis.
    BINOMIAL_PRICER_API void __stdcall ComputeOptionGreeksBinomial(
        double S, double K, double T, double r, double sigma, double q,
        int optionType, int optionStyle, const char* calculationDate,
        int binomialSteps,
        double* delta, double* gamma, double* vega, double* theta, double* rho);

#ifdef __cplusplus
}
#endif

#endif // BINOMIAL_PRICER_DLL_HPP
