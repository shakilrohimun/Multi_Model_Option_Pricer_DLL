#ifndef BLACKSCHOLESPRICER_HPP
#define BLACKSCHOLESPRICER_HPP

/**
 * @file BlackScholesPricer.hpp
 * @brief Declaration and comprehensive documentation of the BlackScholesPricer class.
 *
 * This header file declares the BlackScholesPricer class, which implements the analytical pricing
 * model for European options using the Black-Scholes formula. The class provides methods for computing
 * both the option price and its sensitivity measures (Greeks) such as Delta, Gamma, Vega, Theta, and Rho.
 *
 * The Black-Scholes formula is a widely recognized model in quantitative finance, relying on the
 * assumption of a log-normal distribution of the underlying asset's returns with constant volatility
 * and interest rates. It is applicable exclusively to European style options.
 *
 * The class integrates with a PricingConfiguration object to allow for flexible specification of
 * market parameters, including but not limited to the risk-free interest rate, the option maturity,
 * and the calculation date. This facilitates easy adaptation to different market scenarios and risk
 * management strategies.
 */

#include "pch.h"                     // Precompiled header for speeding up compilation.
#include "InterfaceOptionPricer.hpp" // Interface ensuring consistency among different pricing models.
#include "PricingConfiguration.hpp"  // Provides the structure to configure pricing parameters.

 /**
  * @brief A class that implements the Black-Scholes pricing model for European options.
  *
  * The BlackScholesPricer class is derived from the IOptionPricer interface, ensuring that it provides
  * the standard methods for option pricing and computing Greeks. This class encapsulates the Black-Scholes
  * formula logic and integrates market configuration parameters via the PricingConfiguration object.
  *
  * @details The Black-Scholes model assumes that the underlying asset follows a geometric Brownian motion
  * with constant drift and volatility. This analytical model is used to compute the theoretical price of
  * European options and their sensitivity to various market factors, thus playing a crucial role in risk
  * management and hedging.
  */
class BlackScholesPricer : public IOptionPricer {
public:
    /**
     * @brief Default constructor for BlackScholesPricer.
     *
     * Constructs an instance of the BlackScholesPricer using default pricing configuration values.
     *
     * @details The default constructor initializes the pricer with a standard set of parameters which
     * may be defined internally or by the default constructor of the PricingConfiguration class. This
     * allows for immediate usage in scenarios where custom market parameters are not needed.
     */
    BlackScholesPricer();

    /**
     * @brief Constructor with custom pricing configuration.
     *
     * Initializes a BlackScholesPricer instance with user-defined pricing parameters.
     *
     * @param config A PricingConfiguration object that contains custom parameters such as the calculation
     *               date, risk-free interest rate, and option maturity.
     *
     * @details This constructor facilitates the customization of the pricing model to reflect specific
     * market conditions or experimental setups. By providing a custom configuration, users can ensure that
     * the pricer aligns with their desired assumptions and constraints.
     */
    BlackScholesPricer(const PricingConfiguration& config);

    /**
     * @brief Virtual destructor for BlackScholesPricer.
     *
     * Ensures that the resources allocated for an instance of BlackScholesPricer are properly released.
     *
     * @details The destructor is declared virtual to support proper cleanup in case of inheritance,
     * following best practices in object-oriented design. This guarantees that if a derived class
     * overrides the destructor, it will be called appropriately.
     */
    virtual ~BlackScholesPricer();

    /**
     * @brief Computes the price of a European option using the Black-Scholes formula.
     *
     * This method calculates the theoretical price of a European option based on the Black-Scholes model.
     * It utilizes both the option parameters (e.g., underlying price, strike price, volatility) and the
     * configuration parameters (e.g., risk-free rate, maturity) provided in the PricingConfiguration.
     *
     * @param opt A constant reference to an Option object that encapsulates the essential option parameters.
     * @return A double representing the calculated option price.
     * @throw std::runtime_error if the provided option is not European, as the Black-Scholes model is
     *        applicable only to European style options.
     *
     * @details The pricing method involves computing intermediate variables (such as d1 and d2) that are
     * central to the Black-Scholes formula. These variables are then used to evaluate the cumulative
     * distribution functions of the standard normal distribution, yielding the option price.
     */
    virtual double price(const Option& opt) const override;

    /**
     * @brief Computes the Greeks for a European option using the Black-Scholes model.
     *
     * This method calculates various sensitivity measures (Greeks) of the option's price to changes in
     * underlying market parameters. The Greeks computed include:
     * - Delta: Rate of change of the option price with respect to changes in the underlying asset's price.
     * - Gamma: Rate of change of Delta with respect to changes in the underlying asset's price.
     * - Vega: Sensitivity of the option price to changes in volatility.
     * - Theta: Rate of decline of the option price with respect to time.
     * - Rho: Sensitivity of the option price to changes in the risk-free interest rate.
     *
     * @param opt A constant reference to an Option object containing the details required for pricing.
     * @return A Greeks structure that encapsulates the computed values of Delta, Gamma, Vega, Theta, and Rho.
     * @throw std::runtime_error if the option is not European, since the Black-Scholes model is not designed
     *        for American options.
     *
     * @details The method leverages the classical analytical formulas for each Greek, which involve
     * differentiating the Black-Scholes pricing equation with respect to the underlying parameters. This
     * provides critical insights into how the option price reacts to market movements, supporting effective
     * risk management and hedging strategies.
     */
    virtual Greeks computeGreeks(const Option& opt) const override;

private:
    /**
     * @brief Holds the pricing configuration parameters.
     *
     * This private member variable stores an instance of the PricingConfiguration object, which includes
     * essential market parameters such as the risk-free rate, calculation date, and option maturity. These
     * parameters are used throughout the pricing and Greeks computation methods to ensure that the model
     * accurately reflects current market conditions.
     *
     * @details The configuration can either be set to default values or be customized by the user during
     * object construction. It serves as a centralized repository for inputs that are critical to the functioning
     * of the Black-Scholes pricing algorithm, promoting consistency and ease of maintenance.
     */
    PricingConfiguration config_;
};

#endif // BLACKSCHOLESPRICER_HPP
