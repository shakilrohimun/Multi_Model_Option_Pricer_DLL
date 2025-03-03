#ifndef MONTECARLOPRICER_HPP
#define MONTECARLOPRICER_HPP

/**
 * @file MonteCarloPricer.hpp
 * @brief Declaration of the MonteCarloPricer class.
 *
 * This class implements option pricing using Monte Carlo simulation.
 * For European options, a standard Monte Carlo simulation is used.
 * For American options, the Longstaff-Schwartz method (least-squares regression)
 * is employed to determine optimal early exercise.
 */

#include "pch.h"
#include "InterfaceOptionPricer.hpp"
#include "PricingConfiguration.hpp"


 /**
  * @brief Class that implements the Monte Carlo pricing model.
  */
class MonteCarloPricer : public IOptionPricer {
public:
    /**
     * @brief Default constructor.
     */
    MonteCarloPricer();

    /**
     * @brief Destructor.
     */
    virtual ~MonteCarloPricer();

    /**
     * @brief Computes the price of the option using Monte Carlo simulation.
     * @param opt The option to be priced.
     * @return The computed option price.
     */
    virtual double price(const Option& opt) const override;

    /**
     * @brief Computes the Greeks of the option using Monte Carlo simulation.
     * @param opt The option to evaluate.
     * @return A Greeks structure containing the calculated values.
     */
    virtual Greeks computeGreeks(const Option& opt) const override;

    /**
    * @brief Constructor with pricing configuration.
    * @param config A PricingConfiguration structure containing additional parameters,
    *               such as the calculation date, maturity, risk-free rate, number of paths,
    *               and time steps per path.
    */
    MonteCarloPricer(const PricingConfiguration& config);

private:
    PricingConfiguration config_; ///< Additional configuration parameters for the Monte Carlo model.

};

#endif // MONTECARLOPRICER_HPP
