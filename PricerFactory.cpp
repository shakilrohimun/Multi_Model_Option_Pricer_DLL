/**
 * @file PricerFactory.cpp
 * @brief Implementation of the factory for creating pricing engines.
 *
 * This file contains the complete implementation of the PricerFactory class which provides
 * a static method to dynamically create an instance of a pricing engine based on the specified type
 * (BlackScholes, Binomial, CrankNicolson, or MonteCarlo).
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
  * @brief Creates a pricing engine based on the specified type.
  *
  * Depending on the enumeration passed as a parameter, this method returns a unique pointer
  * to a concrete instance of IOptionPricer.
  *
  * @param type The type of pricer to create (BlackScholes, Binomial, CrankNicolson, MonteCarlo).
  * @return A std::unique_ptr<IOptionPricer> pointing to the created instance.
  * @throw std::invalid_argument if the pricer type is not recognized.
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
        throw std::invalid_argument("Unknown pricer type.");
    }
}

/**
 * @brief Creates a pricing engine with a specific configuration.
 *
 * This overload allows the user to specify additional configuration parameters via a
 * PricingConfiguration object.
 *
 * @param type The type of pricer to create (BlackScholes, Binomial, CrankNicolson, MonteCarlo).
 * @param config A PricingConfiguration object containing additional parameters.
 * @return A std::unique_ptr<IOptionPricer> pointing to the created instance.
 * @throw std::invalid_argument if the pricer type is not recognized.
 */
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
