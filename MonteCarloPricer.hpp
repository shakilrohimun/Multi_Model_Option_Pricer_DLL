#ifndef MONTECARLOPRICER_HPP
#define MONTECARLOPRICER_HPP

/**
 * @file MonteCarloPricer.hpp
 * @brief Declaration and detailed documentation of the MonteCarloPricer class.
 *
 * This header file declares the MonteCarloPricer class, which implements option pricing using
 * Monte Carlo simulation techniques. The class simulates numerous potential future price paths
 * of the underlying asset to compute the expected payoff of the option.
 *
 * For European options, a standard Monte Carlo simulation is used. For American options,
 * the Longstaff-Schwartz method (which employs least-squares regression) is utilized to
 * determine the optimal early exercise strategy.
 *
 * This approach is particularly useful when closed-form solutions are not available,
 * providing a flexible and robust method for pricing a variety of option types.
 */

#include "pch.h"                      // Precompiled header for common system and project includes.
#include "InterfaceOptionPricer.hpp"  // Interface definition for option pricing engines.
#include "PricingConfiguration.hpp"   // Contains configuration parameters for pricing models.

 /**
  * @brief Class implementing the Monte Carlo pricing model.
  *
  * The MonteCarloPricer class derives from the IOptionPricer interface and provides concrete
  * implementations for calculating both the option price and its Greeks using Monte Carlo simulation.
  *
  * @details The class supports two primary simulation methods:
  *          - A standard Monte Carlo simulation for European options.
  *          - The Longstaff-Schwartz method for American options, which uses least-squares regression
  *            to determine the optimal early exercise strategy.
  *
  * The simulation parameters such as the number of simulation paths and the time steps per path are
  * configurable via the PricingConfiguration structure, allowing the user to balance precision and performance.
  */
class MonteCarloPricer : public IOptionPricer {
public:
    /**
     * @brief Default constructor for MonteCarloPricer.
     *
     * Constructs a MonteCarloPricer object using default simulation parameters.
     *
     * @details The default constructor initializes the pricing model with standard parameters,
     *          which are either defined internally or via the default settings in the PricingConfiguration class.
     */
    MonteCarloPricer();

    /**
     * @brief Destructor for MonteCarloPricer.
     *
     * Destroys the MonteCarloPricer object and frees any resources allocated during its lifetime.
     *
     * @details The destructor is declared virtual to ensure that resources are properly released
     *          in any derived classes, following C++ best practices for polymorphic base classes.
     */
    virtual ~MonteCarloPricer();

    /**
     * @brief Computes the price of the option using Monte Carlo simulation.
     *
     * This method simulates multiple future price paths for the underlying asset and calculates the
     * expected payoff of the option by discounting and averaging the results from all simulated paths.
     *
     * @param opt A constant reference to an Option object containing parameters such as the underlying asset price,
     *            strike price, volatility, dividend yield, and maturity.
     * @return The computed option price as a double.
     *
     * @details For American options, the Longstaff-Schwartz algorithm is employed to handle early exercise,
     *          while for European options, the option payoff is directly calculated at maturity.
     */
    virtual double price(const Option& opt) const override;

    /**
     * @brief Computes the Greeks of the option using Monte Carlo simulation.
     *
     * This method calculates the sensitivity measures (Greeks) of the option's price relative to changes in
     * various parameters, such as the underlying asset price, volatility, time, and interest rate.
     *
     * @param opt A constant reference to an Option object containing the required option parameters.
     * @return A Greeks structure encapsulating the computed sensitivity measures (Delta, Gamma, Vega, Theta, and Rho).
     *
     * @details The Greeks are typically computed using finite difference methods or other numerical techniques
     *          within the context of Monte Carlo simulation. These measures are crucial for risk management and hedging.
     */
    virtual Greeks computeGreeks(const Option& opt) const override;

    /**
     * @brief Constructs a MonteCarloPricer with a specific pricing configuration.
     *
     * This constructor allows for the initialization of a MonteCarloPricer object using a user-defined
     * PricingConfiguration, which may include parameters such as the calculation date, option maturity,
     * risk-free rate, number of simulation paths, and time steps per path.
     *
     * @param config A PricingConfiguration structure containing custom simulation parameters.
     *
     * @details Customizing the pricing configuration enables users to optimize the trade-off between simulation
     *          precision and computational performance based on their specific requirements.
     */
    MonteCarloPricer(const PricingConfiguration& config);

private:
    /**
     * @brief Stores additional configuration parameters for the Monte Carlo pricing model.
     *
     * This member variable holds a PricingConfiguration object that contains various simulation parameters,
     * including calculation date, risk-free rate, number of simulation paths, and time steps per path.
     *
     * @details The configuration parameters directly influence the accuracy and efficiency of the Monte Carlo simulation,
     *          allowing for tailored pricing calculations based on the desired level of precision.
     */
    PricingConfiguration config_;
};

#endif // MONTECARLOPRICER_HPP
