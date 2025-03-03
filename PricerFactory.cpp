/**
 * @file PricerFactory.cpp
 * @brief Impl�mentation de la factory pour cr�er des moteurs de pricing.
 *
 * Ce fichier contient l'impl�mentation compl�te de la classe PricerFactory qui fournit
 * une m�thode statique pour cr�er dynamiquement une instance de moteur de pricing en fonction
 * du type sp�cifi� (Black-Scholes, Binomial, Crank-Nicolson, ou Monte Carlo).
 */

#include "pch.h"
#include "PricerFactory.hpp"
#include "BlackScholesPricer.hpp"
#include "BinomialPricer.hpp"
#include "CrankNicolsonPricer.hpp"
#include "MonteCarloPricer.hpp"
#include <memory>
#include <stdexcept>

 /**
  * @brief Cr�e un moteur de pricing selon le type sp�cifi�.
  *
  * En fonction de l'�num�ration pass�e en param�tre, cette m�thode retourne un pointeur
  * unique vers une instance concr�te d'IOptionPricer.
  *
  * @param type Le type de pricer � cr�er (BlackScholes, Binomial, CrankNicolson, MonteCarlo).
  * @return Un std::unique_ptr<IOptionPricer> pointant vers l'instance cr��e.
  * @throw std::invalid_argument Si le type de pricer n'est pas reconnu.
  */
std::unique_ptr<IOptionPricer> PricerFactory::createPricer(PricerType type) {
    switch (type) {
    case PricerType::BlackScholes:
        return std::make_unique<BlackScholesPricer>();
    case PricerType::Binomial:
        return std::make_unique<BinomialPricer>();
    case PricerType::CrankNicolson:
        return std::make_unique<CrankNicolsonPricer>();
    case PricerType::MonteCarlo:
        return std::make_unique<MonteCarloPricer>();
    default:
        throw std::invalid_argument("Type de pricer inconnu.");
    }
}

std::unique_ptr<IOptionPricer> PricerFactory::createPricer(PricerType type, const PricingConfiguration& config) {
    switch (type) {
    case PricerType::BlackScholes:
        return std::make_unique<BlackScholesPricer>(config);
    case PricerType::Binomial:
        return std::make_unique<BinomialPricer>(config);
    case PricerType::CrankNicolson:
        return std::make_unique<CrankNicolsonPricer>(config);
    case PricerType::MonteCarlo:
        return std::make_unique<MonteCarloPricer>(config);
    default:
        throw std::invalid_argument("Unknown pricer type.");
    }
}

