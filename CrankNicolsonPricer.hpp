#ifndef CRANKNICOLSONPRICER_HPP
#define CRANKNICOLSONPRICER_HPP

/**
 * @file CrankNicolsonPricer.hpp
 * @brief Declaration of the CrankNicolsonPricer class.
 *
 * This class implements option pricing using the Crank-Nicolson finite difference method.
 * It solves the Black-Scholes partial differential equation by discretizing the time and
 * underlying asset price.
 */

#include "pch.h"
#include "InterfaceOptionPricer.hpp"
#include "PricingConfiguration.hpp"


 /**
  * @brief Class that implements the Crank-Nicolson pricing model.
  */
class CrankNicolsonPricer : public IOptionPricer {
public:
    /**
     * @brief Default constructor.
     */
    CrankNicolsonPricer();

    /**
     * @brief Destructor.
     */
    virtual ~CrankNicolsonPricer();

    /**
     * @brief Computes the price of the option using the Crank-Nicolson method.
     * @param opt The option to be priced.
     * @return The computed option price.
     */
    virtual double price(const Option& opt) const override;

    /**
     * @brief Computes the Greeks of the option using the Crank-Nicolson method.
     * @param opt The option to evaluate.
     * @return A Greeks structure containing the calculated values.
     */
    virtual Greeks computeGreeks(const Option& opt) const override;

    /**
    * @brief Constructor with pricing configuration.
    * @param config A PricingConfiguration structure containing additional parameters,
    *               such as the calculation date, maturity, risk-free rate, time steps, spot steps, and S_max.
    */
    CrankNicolsonPricer(const PricingConfiguration& config);

private:
    PricingConfiguration config_; ///< Additional configuration parameters for the Crank-Nicolson model.
};

#endif // CRANKNICOLSONPRICER_HPP
