#ifndef PRICERFACTORY_HPP
#define PRICERFACTORY_HPP

/**
 * @file PricerFactory.hpp
 * @brief D�claration de la factory pour cr�er des moteurs de pricing.
 */

#include "pch.h"
#include <memory>
#include <string>
#include "InterfaceOptionPricer.hpp"
#include "PricingConfiguration.hpp"

 /**
  * @brief Enum�ration des types de pricers.
  */
enum class PricerType {
    BlackScholes, /**< Pricer utilisant la formule de Black-Scholes. */
    Binomial,     /**< Pricer bas� sur la m�thode binomiale. */
    CrankNicolson,/**< Pricer utilisant la m�thode des diff�rences finies de Crank-Nicolson. */
    MonteCarlo    /**< Pricer bas� sur la simulation par Monte Carlo. */
};

/**
 * @brief Factory pour cr�er des instances de moteurs de pricing.
 *
 * Cette classe fournit une m�thode statique permettant de cr�er dynamiquement un pricer en fonction du type demand�.
 */
class PricerFactory {
public:
    /**
     * @brief Cr�e un moteur de pricing selon le type sp�cifi�.
     * @param type Le type de pricer � cr�er.
     * @return Un pointeur unique vers une instance de IOptionPricer.
     */
    static std::unique_ptr<IOptionPricer> createPricer(PricerType type);

    /**
    * @brief Creates a pricing engine according to the specified type and configuration.
    *
    * This overload allows the user to specify additional configuration parameters
    * (such as maturity, risk-free rate, discretization settings, etc.) via a
    * PricingConfiguration object.
    *
    * @param type The type of pricer to create (BlackScholes, Binomial, CrankNicolson, MonteCarlo).
    * @param config A PricingConfiguration object containing additional parameters.
    * @return A std::unique_ptr<IOptionPricer> pointing to the created instance.
    * @throw std::invalid_argument if the pricer type is unknown.
    */
    static std::unique_ptr<IOptionPricer> createPricer(PricerType type, const PricingConfiguration& config);

};

#endif // PRICERFACTORY_HPP
