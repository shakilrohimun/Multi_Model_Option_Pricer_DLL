#ifndef BINOMIALPRICER_HPP
#define BINOMIALPRICER_HPP

/**
 * @file BinomialPricer.hpp
 * @brief Declaration of the BinomialPricer class.
 *
 * This class implements the option pricing model using the binomial CRR method.
 * It is capable of pricing both European and American options.
 *
 * In addition to the basic option parameters (underlying, strike, volatility, dividend,
 * option type and style), this pricer allows the user to specify extra configuration
 * parameters via a PricingConfiguration structure. These additional parameters include:
 * - Calculation date (if not provided, the current date is assumed).
 * - Number of steps (N) for the binomial tree.
 *
 * This design provides maximum flexibility and separation of concerns.
 */

#include "pch.h"
#include "InterfaceOptionPricer.hpp"
#include "PricingConfiguration.hpp"

class BinomialPricer : public IOptionPricer {
public:
    /**
     * @brief Default constructor.
     *
     * Constructs a BinomialPricer using default configuration values.
     */
    BinomialPricer();

    /**
     * @brief Constructor with pricing configuration.
     *
     * @param config A PricingConfiguration structure containing additional parameters,
     *               such as the calculation date and the number of steps (N).
     */
    BinomialPricer(const PricingConfiguration& config);

    /**
     * @brief Destructor.
     */
    virtual ~BinomialPricer();

    /**
     * @brief Computes the option price using the binomial CRR model.
     *
     * This function calculates the price of an option using the binomial tree approach,
     * with the number of steps specified in the pricing configuration.
     *
     * @param opt The option to be priced.
     * @return The computed option price.
     */
    virtual double price(const Option& opt) const override;

    /**
     * @brief Computes the Greeks of the option using finite differences applied to the binomial model.
     *
     * This function estimates the option Greeks (Delta, Gamma, Vega, Theta, and Rho)
     * by perturbing the input parameters and recalculating the option price.
     *
     * @param opt The option to evaluate.
     * @return A Greeks structure containing the calculated values.
     */
    virtual Greeks computeGreeks(const Option& opt) const override;

    /**
     * @brief Sets the pricing configuration.
     *
     * Allows updating the configuration parameters used by the BinomialPricer.
     *
     * @param config A PricingConfiguration structure containing the new parameters.
     */
    void setConfiguration(const PricingConfiguration& config);

    /**
     * @brief Gets the current pricing configuration.
     *
     * @return The current PricingConfiguration structure.
     */
    PricingConfiguration getConfiguration() const;

private:
    PricingConfiguration config_; ///< Additional configuration parameters for the Binomial model.
};

#endif // BINOMIALPRICER_HPP
