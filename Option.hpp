#ifndef OPTION_HPP
#define OPTION_HPP

/**
 * @file Option.hpp
 * @brief Declaration of the Option class and the Greeks structure for option pricing.
 *
 * This file contains the declaration of the Option class, which represents a financial derivative,
 * and the Greeks structure, which stores various sensitivity measures (Greeks) used in option pricing.
 * These measures help in assessing how the option's value changes in response to variations in underlying parameters.
 */

#include "pch.h"
#include <string>

 /**
  * @brief Structure grouping the Greeks of an option.
  *
  * The Greeks quantify the sensitivity of the option's price to changes in different market parameters:
  * - Delta: Sensitivity to changes in the underlying asset's price.
  * - Gamma: Rate of change of Delta with respect to the underlying asset's price.
  * - Vega: Sensitivity to changes in the underlying asset's volatility.
  * - Theta: Sensitivity to the passage of time (time decay).
  * - Rho: Sensitivity to changes in the risk-free interest rate.
  */
struct Greeks {
    double delta; /**< Sensitivity of the option's price with respect to the underlying asset's price. */
    double gamma; /**< Sensitivity of Delta with respect to changes in the underlying asset's price. */
    double vega;  /**< Sensitivity of the option's price to changes in volatility. */
    double theta; /**< Sensitivity of the option's price with respect to time decay. */
    double rho;   /**< Sensitivity of the option's price with respect to changes in the risk-free interest rate. */
};

/**
 * @brief Class representing a financial option.
 *
 * The Option class encapsulates the essential parameters needed for option pricing,
 * including the underlying asset's price, strike price, volatility, dividend yield (if any),
 * option type (Call or Put), and option style (European or American).
 *
 * It provides methods to access (getters) and modify (setters) these parameters,
 * enabling flexibility in option pricing calculations.
 */
class Option {
public:
    /**
     * @brief Enumeration for the type of option.
     *
     * Defines the possible types of options:
     * - Call: Option to buy the underlying asset.
     * - Put: Option to sell the underlying asset.
     */
    enum class OptionType {
        Call, /**< Call option: grants the right to buy the underlying asset. */
        Put   /**< Put option: grants the right to sell the underlying asset. */
    };

    /**
     * @brief Enumeration for the style of the option.
     *
     * Specifies whether an option is European or American:
     * - European: Can only be exercised at maturity.
     * - American: Can be exercised at any time up to maturity.
     */
    enum class OptionStyle {
        European, /**< European option: exercise is allowed only at expiration. */
        American  /**< American option: exercise is allowed at any time before expiration. */
    };

    /**
     * @brief Default constructor.
     *
     * Constructs an Option object with default parameters. The default values are typically
     * set to zero or other appropriate defaults as determined by the implementation.
     */
    Option();

    /**
     * @brief Parameterized constructor.
     *
     * Constructs an Option object with the specified parameters.
     *
     * @param underlying The price of the underlying asset.
     * @param strike The strike price of the option.
     * @param volatility The volatility of the underlying asset.
     * @param dividend The dividend yield (if applicable) of the underlying asset.
     * @param optionType The type of the option (Call or Put).
     * @param optionStyle The style of the option (European or American).
     */
    Option(double underlying, double strike, double volatility, double dividend,
        OptionType optionType, OptionStyle optionStyle);

    /**
     * @brief Virtual destructor.
     *
     * Ensures that resources are properly released when an Option object is destroyed.
     */
    virtual ~Option();

    /**
     * @brief Retrieves the price of the underlying asset.
     *
     * @return The current price of the underlying asset.
     */
    double getUnderlying() const;

    /**
     * @brief Retrieves the strike price of the option.
     *
     * @return The strike price.
     */
    double getStrike() const;

    /**
     * @brief Retrieves the volatility of the underlying asset.
     *
     * @return The volatility.
     */
    double getVolatility() const;

    /**
     * @brief Retrieves the dividend yield.
     *
     * @return The dividend yield.
     */
    double getDividend() const;

    /**
     * @brief Retrieves the type of the option.
     *
     * @return The option type (Call or Put).
     */
    OptionType getOptionType() const;

    /**
     * @brief Retrieves the style of the option.
     *
     * @return The option style (European or American).
     */
    OptionStyle getOptionStyle() const;

    /**
     * @brief Sets the price of the underlying asset.
     *
     * @param underlying The new price of the underlying asset.
     */
    void setUnderlying(double underlying);

    /**
     * @brief Sets the strike price of the option.
     *
     * @param strike The new strike price.
     */
    void setStrike(double strike);

    /**
     * @brief Sets the volatility of the underlying asset.
     *
     * @param volatility The new volatility.
     */
    void setVolatility(double volatility);

    /**
     * @brief Sets the dividend yield.
     *
     * @param dividend The new dividend yield.
     */
    void setDividend(double dividend);

    /**
     * @brief Sets the type of the option.
     *
     * @param optionType The new option type (Call or Put).
     */
    void setOptionType(OptionType optionType);

    /**
     * @brief Sets the style of the option.
     *
     * @param optionStyle The new option style (European or American).
     */
    void setOptionStyle(OptionStyle optionStyle);

private:
    double underlying_;   /**< The price of the underlying asset. */
    double strike_;       /**< The strike price of the option. */
    double volatility_;   /**< The volatility of the underlying asset. */
    double dividend_;     /**< The dividend yield, if applicable. */
    OptionType type_;     /**< The type of the option (Call or Put). */
    OptionStyle style_;   /**< The style of the option (European or American). */
};

#endif // OPTION_HPP
