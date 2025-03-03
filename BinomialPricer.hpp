#ifndef BINOMIALPRICER_HPP
#define BINOMIALPRICER_HPP

/**
 * @file BinomialPricer.hpp
 * @brief Declaration and detailed documentation of the BinomialPricer class.
 *
 * This file defines the BinomialPricer class, which implements the option pricing model
 * using the binomial Cox-Ross-Rubinstein (CRR) method. The class is designed to price
 * both European and American style options.
 *
 * The pricing model leverages a binomial tree approach that discretizes the underlying asset's
 * price evolution over a finite number of time steps. This method provides an efficient and
 * flexible framework for option pricing, particularly when dealing with early exercise features
 * (in the case of American options) or when a high degree of accuracy is required.
 *
 * Additional configuration parameters can be supplied via a PricingConfiguration structure.
 * These parameters include, but are not limited to:
 * - Calculation date: If not explicitly provided, the current date is assumed.
 * - Number of steps (N): Determines the granularity of the binomial tree; more steps generally yield
 *   a more accurate pricing result at the cost of additional computational effort.
 *
 * The overall design emphasizes flexibility and separation of concerns by isolating configuration
 * details from the core pricing logic. This makes it easier to adjust model parameters and to extend
 * the class functionality if needed.
 *
 * Detailed documentation for each method is provided below, ensuring that users of this class have
 * a comprehensive understanding of its functionality and usage.
 */

#include "pch.h"
#include "InterfaceOptionPricer.hpp"
#include "PricingConfiguration.hpp"

 /**
  * @class BinomialPricer
  * @brief A concrete implementation of the IOptionPricer interface using the binomial CRR model.
  *
  * The BinomialPricer class offers functionality for pricing options by constructing a recombining
  * binomial tree. It supports the pricing of various option types and styles, and also provides methods
  * for calculating option sensitivities (Greeks) using finite difference approximations.
  *
  * The class inherits from the IOptionPricer interface, ensuring that it conforms to a standard
  * contract for option pricing engines within the DLL pricing project.
  */
class BinomialPricer : public IOptionPricer {
public:
    /**
     * @brief Default constructor.
     *
     * Constructs a BinomialPricer instance using default configuration values. This constructor
     * sets up the pricer with a standard configuration that is suitable for most basic pricing
     * scenarios without the need for user-specified parameters.
     *
     * @details The default configuration is typically determined by either internal constants or
     * by the default constructor of the PricingConfiguration class. This ensures that even without
     * explicit configuration, the pricer is immediately ready for use.
     */
    BinomialPricer();

    /**
     * @brief Constructor with pricing configuration.
     *
     * Creates a BinomialPricer instance with a user-specified pricing configuration.
     *
     * @param config A PricingConfiguration structure containing additional parameters, such as:
     *               - Calculation date: The specific date at which the option pricing should be evaluated.
     *               - Number of steps (N): The number of time steps in the binomial tree.
     *
     * @details This constructor is particularly useful when different market conditions or
     * experimental configurations are required. By allowing the injection of a custom configuration,
     * it offers flexibility to tailor the binomial tree construction and subsequent pricing results.
     */
    BinomialPricer(const PricingConfiguration& config);

    /**
     * @brief Virtual destructor.
     *
     * Ensures proper cleanup of resources when a BinomialPricer object is destroyed.
     *
     * @details Although the current implementation may not allocate dynamic resources explicitly,
     * the destructor is declared virtual to guarantee that derived classes can override it if necessary.
     * This follows best practices in C++ for inheritance and polymorphism.
     */
    virtual ~BinomialPricer();

    /**
     * @brief Computes the option price using the binomial CRR model.
     *
     * This method implements the core pricing functionality by constructing a binomial tree based
     * on the provided option parameters and the current pricing configuration. The tree is built
     * according to the Cox-Ross-Rubinstein method, which ensures that the underlying asset's price
     * evolution is modeled accurately over discrete time intervals.
     *
     * @param opt The option to be priced, which contains essential data such as the strike price,
     *            underlying asset price, volatility, dividend yield, and maturity.
     * @return The computed option price as a double value.
     *
     * @details The method performs the following high-level steps:
     *  - Initializes the binomial tree parameters using the configuration settings (e.g., number of steps).
     *  - Iteratively calculates the asset price at each node of the tree.
     *  - Determines the option payoff at each final node and performs backward induction to compute
     *    the present value.
     *  - For American options, the method includes a check for early exercise at each node.
     */
    virtual double price(const Option& opt) const override;

    /**
     * @brief Computes the option Greeks using finite difference approximations.
     *
     * This method calculates the sensitivity measures (Greeks) for the given option by perturbing
     * the underlying parameters and recomputing the option price. The Greeks typically computed include:
     * - Delta: Sensitivity to changes in the underlying asset price.
     * - Gamma: Sensitivity of Delta with respect to changes in the underlying asset price.
     * - Vega: Sensitivity to changes in volatility.
     * - Theta: Sensitivity to changes in time to maturity.
     * - Rho: Sensitivity to changes in the risk-free interest rate.
     *
     * @param opt The option for which the Greeks are to be computed.
     * @return A Greeks structure containing the computed values for each sensitivity measure.
     *
     * @details Finite difference methods are used by slightly adjusting one parameter at a time while
     * keeping the others constant. The difference in the computed option prices is then used to approximate
     * the derivative with respect to that parameter. This method is crucial for risk management and
     * for understanding how small changes in market conditions can impact the option price.
     */
    virtual Greeks computeGreeks(const Option& opt) const override;

    /**
     * @brief Sets or updates the pricing configuration for the pricer.
     *
     * This method allows the user to modify the configuration parameters that govern the behavior
     * of the BinomialPricer. By updating the configuration, the user can adapt the pricing engine to
     * different market scenarios or testing conditions.
     *
     * @param config A PricingConfiguration structure containing the new configuration values.
     *
     * @details Upon invocation, the internal state of the pricer is updated with the new settings.
     * This may include adjustments to the calculation date, number of steps in the binomial tree, and
     * other parameters that affect the pricing algorithm.
     */
    void setConfiguration(const PricingConfiguration& config);

    /**
     * @brief Retrieves the current pricing configuration.
     *
     * Returns the PricingConfiguration structure that is currently in use by the pricer.
     *
     * @return The current PricingConfiguration.
     *
     * @details This method is useful for verifying the pricer's current state and for debugging
     * purposes. It allows external components or users to inspect the parameters that will be used
     * in subsequent pricing computations.
     */
    PricingConfiguration getConfiguration() const;

private:
    /**
     * @brief Internal storage for the pricing configuration.
     *
     * This member variable holds additional configuration parameters that are used by the binomial
     * pricing algorithm. These include the calculation date, number of steps, and any other parameters
     * encapsulated within the PricingConfiguration structure.
     *
     * @details The configuration is critical for controlling the behavior of the pricing model.
     * It ensures that the option pricing is consistent with the user's expectations and the prevailing
     * market conditions. Modifying this configuration directly impacts the results of both the option
     * pricing and the Greeks computations.
     */
    PricingConfiguration config_;
};

#endif // BINOMIALPRICER_HPP
