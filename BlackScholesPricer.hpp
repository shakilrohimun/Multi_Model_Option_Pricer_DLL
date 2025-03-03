#ifndef BLACKSCHOLESPRICER_HPP
#define BLACKSCHOLESPRICER_HPP

/**
 * @file BlackScholesPricer.hpp
 * @brief Declaration of the BlackScholesPricer class.
 *
 * This class implements the pricing of European options using the Black-Scholes formula.
 * It calculates both the option price and its Greeks.
 */

#include "pch.h"
#include "InterfaceOptionPricer.hpp"
#include "PricingConfiguration.hpp"

 /**
  * @brief Class that implements the Black-Scholes pricing model.
  */
class BlackScholesPricer : public IOptionPricer {
public:
    /**
     * @brief Default constructor.
     */
    BlackScholesPricer();

    /**
     * @brief Constructor with pricing configuration.
     *
     * Constructs a BlackScholesPricer using the provided PricingConfiguration object.
     * The configuration parameters (such as calculation date, maturity, and risk-free rate)
     * are stored and used in the pricing calculations.
     *
     * @param config A PricingConfiguration object containing custom parameters.
     */
    BlackScholesPricer(const PricingConfiguration& config);

    /**
     * @brief Destructor.
     */
    virtual ~BlackScholesPricer();

    /**
     * @brief Computes the price of the option using the Black-Scholes formula.
     *
     * This method calculates the price of a European option using the Black-Scholes formula.
     * The calculation uses the option parameters and the configuration parameters (maturity
     * and risk-free rate).
     *
     * @param opt The option to be priced.
     * @return The computed option price.
     * @throw std::runtime_error if the option is not European.
     */
    virtual double price(const Option& opt) const override;

    /**
     * @brief Computes the Greeks of the option using the Black-Scholes model.
     *
     * This method computes the following Greeks:
     * - Delta: sensitivity with respect to the underlying price.
     * - Gamma: sensitivity of Delta with respect to the underlying price.
     * - Vega: sensitivity with respect to volatility.
     * - Theta: sensitivity with respect to time.
     * - Rho: sensitivity with respect to the risk-free rate.
     *
     * The classical Black-Scholes formulas are used, with maturity and risk-free rate
     * taken from the configuration.
     *
     * @param opt The option to evaluate.
     * @return A Greeks structure containing the calculated values.
     * @throw std::runtime_error if the option is not European.
     */
    virtual Greeks computeGreeks(const Option& opt) const override;

private:
    PricingConfiguration config_; ///< Additional configuration parameters for the Black-Scholes model.
};

#endif // BLACKSCHOLESPRICER_HPP
