#ifndef INTERFACEOPTIONPRICER_HPP
#define INTERFACEOPTIONPRICER_HPP

/**
 * @file IOptionPricer.hpp
 * @brief Abstract interface for option pricing engines.
 *
 * This header file declares the IOptionPricer interface, which serves as the base
 * for all option pricing engine implementations. It enforces a standard contract for
 * computing both the price and the Greeks (sensitivity measures) of an option.
 *
 * @details Implementations of this interface must provide concrete definitions for
 * calculating the option price and its Greeks, ensuring consistency across different
 * pricing models. This interface is critical in a modular pricing system where various
 * numerical methods can be interchanged.
 */

#include "pch.h"   // Precompiled header for faster compilation.
#include "Option.hpp" // Defines the Option class and the Greeks structure used in pricing.

 /**
  * @brief Interface for an option pricing engine.
  *
  * The IOptionPricer interface specifies the necessary methods for pricing options and
  * computing their associated Greeks. Any pricing engine that derives from this interface
  * must implement these methods to allow for consistent integration within the pricing system.
  *
  * @note The interface uses pure virtual functions to enforce implementation in derived classes.
  */
class IOptionPricer {
public:
    /**
     * @brief Virtual destructor.
     *
     * Ensures that derived classes can clean up resources appropriately when an
     * object is deleted through a pointer to IOptionPricer.
     *
     * @details Declaring the destructor as virtual is essential in a polymorphic
     * base class to prevent resource leaks and undefined behavior during object destruction.
     */
    virtual ~IOptionPricer() = default;

    /**
     * @brief Computes the price of the option.
     *
     * This pure virtual function calculates the theoretical price of a given option.
     * The implementation must use the relevant pricing model to derive a numerical value
     * based on the option's parameters.
     *
     * @param opt A constant reference to an Option object that contains all necessary data
     *            (such as underlying price, strike, volatility, dividend yield, and maturity).
     * @return The calculated price of the option as a double.
     *
     * @details The pricing method should consider the specific model assumptions and may involve
     * numerical methods such as finite differences, binomial trees, or closed-form solutions.
     */
    virtual double price(const Option& opt) const = 0;

    /**
     * @brief Computes the Greeks of the option.
     *
     * This pure virtual function calculates the sensitivity measures (Greeks) of an option.
     * The Greeks include Delta, Gamma, Vega, Theta, and Rho, which quantify the option's
     * responsiveness to changes in underlying parameters.
     *
     * @param opt A constant reference to an Option object containing the necessary parameters for evaluation.
     * @return A Greeks structure containing the calculated values for each sensitivity measure.
     *
     * @details The computation of Greeks is vital for risk management and hedging strategies.
     * Implementations should use appropriate numerical or analytical methods to accurately estimate
     * these derivatives.
     */
    virtual Greeks computeGreeks(const Option& opt) const = 0;
};

#endif // IOPTIONPRICER_HPP
