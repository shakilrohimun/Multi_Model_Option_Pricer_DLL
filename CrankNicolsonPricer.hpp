#ifndef CRANKNICOLSONPRICER_HPP
#define CRANKNICOLSONPRICER_HPP

/**
 * @file CrankNicolsonPricer.hpp
 * @brief Declaration and detailed documentation of the CrankNicolsonPricer class.
 *
 * This header file declares the CrankNicolsonPricer class, which implements an option pricing model
 * using the Crank-Nicolson finite difference method. This numerical technique solves the Black-Scholes
 * partial differential equation (PDE) by discretizing both time and the underlying asset price. The method
 * is particularly effective for pricing options when an analytical solution is either unavailable or impractical.
 *
 * The class supports the computation of both the option price and its sensitivity measures (Greeks), which are
 * vital for risk management and hedging strategies. Additional configuration parameters can be specified via a
 * PricingConfiguration object, which includes settings such as the calculation date, option maturity, risk-free rate,
 * the number of time steps, spot steps, and the maximum asset price (S_max) used in the finite difference grid.
 */

#include "pch.h"                   ///< Precompiled header for faster compilation.
#include "InterfaceOptionPricer.hpp" ///< Interface defining the contract for option pricing models.
#include "PricingConfiguration.hpp"  ///< Contains configuration parameters used for pricing models.

 /**
  * @brief A class that implements the Crank-Nicolson finite difference pricing model.
  *
  * The CrankNicolsonPricer class inherits from the IOptionPricer interface and provides methods to compute
  * the price of an option as well as its Greeks using the Crank-Nicolson method. This numerical method is used
  * to approximate the solution of the Black-Scholes PDE by constructing a finite difference grid over both time
  * and the underlying asset price.
  *
  * @details The Crank-Nicolson method is known for its stability and accuracy, making it a popular choice for numerical
  * option pricing. By leveraging a configurable grid defined through the PricingConfiguration object, the pricer can be
  * tailored to a variety of market conditions and computational requirements.
  */
class CrankNicolsonPricer : public IOptionPricer {
public:
    /**
     * @brief Default constructor for CrankNicolsonPricer.
     *
     * Constructs an instance of the CrankNicolsonPricer using default configuration parameters. This allows for
     * immediate usage with a standard set of parameters that are defined internally or via the default constructor
     * of the PricingConfiguration class.
     *
     * @details The default configuration ensures that the model is set up with reasonable defaults for general pricing
     * scenarios, reducing the need for explicit parameter specification in common cases.
     */
    CrankNicolsonPricer();

    /**
     * @brief Destructor for CrankNicolsonPricer.
     *
     * Ensures that all resources allocated by an instance of CrankNicolsonPricer are properly released upon destruction.
     *
     * @details The destructor is declared virtual to facilitate proper cleanup in scenarios where the class might be inherited.
     * This conforms to C++ best practices for managing dynamic resources and avoiding memory leaks.
     */
    virtual ~CrankNicolsonPricer();

    /**
     * @brief Computes the price of an option using the Crank-Nicolson finite difference method.
     *
     * This method calculates the theoretical price of an option by discretizing the time to maturity and the underlying asset
     * price into a grid and then solving the Black-Scholes PDE numerically using the Crank-Nicolson scheme.
     *
     * @param opt A constant reference to an Option object that contains all necessary parameters (such as strike price, underlying price,
     *            volatility, dividend yield, and maturity) required for pricing.
     * @return A double representing the computed option price.
     *
     * @details The Crank-Nicolson method applies an implicit finite difference technique that averages the explicit and implicit
     * discretizations, thereby enhancing numerical stability and accuracy. This is particularly useful for handling the boundary
     * conditions and ensuring convergence of the solution.
     */
    virtual double price(const Option& opt) const override;

    /**
     * @brief Computes the Greeks of the option using the Crank-Nicolson method.
     *
     * This method calculates the sensitivity measures, known as the Greeks (Delta, Gamma, Vega, Theta, and Rho), by applying
     * finite difference approximations to the numerical solution obtained via the Crank-Nicolson method.
     *
     * @param opt A constant reference to an Option object containing the details required for pricing.
     * @return A Greeks structure containing the calculated sensitivity measures.
     *
     * @details The calculation of the Greeks involves perturbing the input parameters slightly and re-solving the finite
     * difference equations. The differences in the computed option prices are then used to estimate the partial derivatives
     * with respect to the underlying variables. These values are essential for risk management and for constructing hedging strategies.
     */
    virtual Greeks computeGreeks(const Option& opt) const override;

    /**
     * @brief Constructs a CrankNicolsonPricer with a custom pricing configuration.
     *
     * This constructor allows for the initialization of a CrankNicolsonPricer object with a user-specified PricingConfiguration.
     * This configuration can include parameters such as the calculation date, option maturity, risk-free rate, number of time steps,
     * spot steps, and the maximum underlying asset price (S_max).
     *
     * @param config A PricingConfiguration structure containing the custom configuration parameters for the pricing model.
     *
     * @details Using a custom configuration enables the pricer to be adapted to different market conditions and numerical accuracy
     * requirements. This flexibility is particularly valuable when high precision is needed or when exploring various scenario analyses.
     */
    CrankNicolsonPricer(const PricingConfiguration& config);

private:
    /**
     * @brief Holds additional configuration parameters for the Crank-Nicolson model.
     *
     * This private member variable stores a PricingConfiguration object that encapsulates the key parameters affecting
     * the numerical solution of the Black-Scholes PDE. These parameters include the calculation date, option maturity, risk-free
     * rate, and the grid settings such as time steps, spot steps, and S_max.
     *
     * @details The configuration parameters directly influence the resolution and accuracy of the finite difference grid, and
     * therefore the resulting option pricing and Greeks calculations. Adjusting these parameters allows for a trade-off
     * between computational cost and precision.
     */
    PricingConfiguration config_;
};

#endif // CRANKNICOLSONPRICER_HPP
