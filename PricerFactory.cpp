/**
 * @file PricerFactory.cpp
 * @brief Implémentation de la factory pour créer des moteurs de pricing.
 *
 * Ce fichier contient l'implémentation complète de la classe PricerFactory qui fournit
 * une méthode statique pour créer dynamiquement une instance de moteur de pricing en fonction
 * du type spécifié (Black-Scholes, Binomial, Crank-Nicolson, ou Monte Carlo).
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
  * @brief Crée un moteur de pricing selon le type spécifié.
  *
  * En fonction de l'énumération passée en paramètre, cette méthode retourne un pointeur
  * unique vers une instance concrète d'IOptionPricer.
  *
  * @param type Le type de pricer à créer (BlackScholes, Binomial, CrankNicolson, MonteCarlo).
  * @return Un std::unique_ptr<IOptionPricer> pointant vers l'instance créée.
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

