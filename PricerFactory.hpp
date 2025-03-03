#ifndef PRICERFACTORY_HPP
#define PRICERFACTORY_HPP

/**
 * @file PricerFactory.hpp
 * @brief Declaration of the factory for creating pricing engines.
 *
 * This header defines the PricerFactory class, which provides static methods to dynamically
 * instantiate different types of option pricing engines based on the specified pricer type.
 * The factory encapsulates the creation logic, thereby promoting a clean separation of concerns
 * and ease of extension.
 */

#include "pch.h"
#include <memory>
#include <string>
#include "InterfaceOptionPricer.hpp"
#include "PricingConfiguration.hpp"

 /**
  * @brief Enumeration of pricer types.
  *
  * The PricerType enumeration defines the various pricing engine implementations that are supported.
  * Each enumerated value corresponds to a specific pricing model used to compute option prices and Greeks.
  */
enum class PricerType {
    BlackScholes, /**< Pricing engine using the Black-Scholes formula. */
    Binomial,     /**< Pricing engine based on the binomial method. */
    CrankNicolson,/**< Pricing engine using the Crank-Nicolson finite difference method. */
    MonteCarlo    /**< Pricing engine based on Monte Carlo simulation. */
};

/**
 * @brief Factory for creating pricing engine instances.
 *
 * The PricerFactory class provides static methods to create instances of pricing engines
 * (implementations of IOptionPricer) based on the specified pricer type and configuration.
 * This design pattern promotes flexibility and scalability in the option pricing system.
 */
class PricerFactory {
public:
    /**
     * @brief Creates a pricing engine according to the specified type.
     *
     * This static method instantiates and returns a unique pointer to a pricing engine
     * based solely on the pricer type provided.
     *
     * @param type The type of pricing engine to create, as defined in the PricerType enumeration.
     * @return A std::unique_ptr<IOptionPricer> pointing to the newly created pricing engine instance.
     *
     * @note Use the returned object to compute option prices and Greeks using the selected pricing model.
     */
    static std::unique_ptr<IOptionPricer> createPricer(PricerType type);

    /**
     * @brief Creates a pricing engine with a specified configuration.
     *
     * This overload allows the caller to provide additional configuration parameters via a
     * PricingConfiguration object. This enables customization of the pricing engine, such as specifying
     * maturity, risk-free rate, discretization settings, and other relevant parameters.
     *
     * @param type The type of pricing engine to create (BlackScholes, Binomial, CrankNicolson, MonteCarlo).
     * @param config A PricingConfiguration object containing additional parameters for customizing the engine.
     * @return A std::unique_ptr<IOptionPricer> pointing to the newly created pricing engine instance.
     *
     * @throw std::invalid_argument if the pricer type is unknown.
     */
    static std::unique_ptr<IOptionPricer> createPricer(PricerType type, const PricingConfiguration& config);
};

#endif // PRICERFACTORY_HPP
