#ifndef OPTION_HPP
#define OPTION_HPP

/**
 * @file Option.hpp
 * @brief Déclaration de la classe Option et de la structure Greeks pour le pricing d'options.
 */

#include "pch.h"
#include <string>

 /**
  * @brief Structure regroupant les greeks d'une option.
  */
struct Greeks {
    double delta; /**< Sensibilité par rapport au prix du sous-jacent. */
    double gamma; /**< Sensibilité de delta par rapport au prix du sous-jacent. */
    double vega;  /**< Sensibilité par rapport à la volatilité. */
    double theta; /**< Sensibilité par rapport au temps. */
    double rho;   /**< Sensibilité par rapport au taux d'intérêt. */
};

/**
 * @brief Classe représentant une option financière.
 *
 * Cette classe encapsule les paramètres essentiels d'une option,
 * tels que le prix du sous-jacent, le strike, la volatilité,
 * le dividende éventuel, le type (Call/Put) et le style (Européenne/Americaine).
 */
class Option {
public:
    /**
     * @brief Enumération pour le type d'option.
     */
    enum class OptionType {
        Call, /**< Option d'achat. */
        Put   /**< Option de vente. */
    };

    /**
     * @brief Enumération pour le style de l'option.
     */
    enum class OptionStyle {
        European, /**< Option européenne. */
        American  /**< Option américaine. */
    };

    /**
     * @brief Constructeur par défaut.
     */
    Option();

    /**
     * @brief Constructeur paramétré.
     * @param underlying Prix du sous-jacent.
     * @param strike Prix d'exercice.
     * @param volatility Volatilité du sous-jacent.
     * @param dividend Dividende éventuel.
     * @param optionType Type de l'option (Call ou Put).
     * @param optionStyle Style de l'option (Européenne ou Américaine).
     */
    Option(double underlying, double strike, double volatility, double dividend,
        OptionType optionType, OptionStyle optionStyle);

    /**
     * @brief Destructeur virtuel.
     */
    virtual ~Option();

    /**
     * @brief Obtient le prix du sous-jacent.
     * @return La valeur du sous-jacent.
     */
    double getUnderlying() const;

    /**
     * @brief Obtient le prix d'exercice.
     * @return Le strike de l'option.
     */
    double getStrike() const;

    /**
     * @brief Obtient la volatilité.
     * @return La volatilité du sous-jacent.
     */
    double getVolatility() const;

    /**
     * @brief Obtient le dividende éventuel.
     * @return Le dividende.
     */
    double getDividend() const;

    /**
     * @brief Obtient le type de l'option.
     * @return Le type (Call ou Put).
     */
    OptionType getOptionType() const;

    /**
     * @brief Obtient le style de l'option.
     * @return Le style (Européenne ou Américaine).
     */
    OptionStyle getOptionStyle() const;

    /**
     * @brief Modifie le prix du sous-jacent.
     * @param underlying Nouveau prix du sous-jacent.
     */
    void setUnderlying(double underlying);

    /**
     * @brief Modifie le prix d'exercice.
     * @param strike Nouveau strike.
     */
    void setStrike(double strike);

    /**
     * @brief Modifie la volatilité.
     * @param volatility Nouvelle volatilité.
     */
    void setVolatility(double volatility);

    /**
     * @brief Modifie le dividende.
     * @param dividend Nouveau dividende.
     */
    void setDividend(double dividend);

    /**
     * @brief Modifie le type de l'option.
     * @param optionType Nouveau type (Call ou Put).
     */
    void setOptionType(OptionType optionType);

    /**
     * @brief Modifie le style de l'option.
     * @param optionStyle Nouveau style (Européenne ou Américaine).
     */
    void setOptionStyle(OptionStyle optionStyle);

private:
    double underlying_;   /**< Prix du sous-jacent. */
    double strike_;       /**< Prix d'exercice de l'option. */
    double volatility_;   /**< Volatilité du sous-jacent. */
    double dividend_;     /**< Dividende éventuel. */
    OptionType type_;     /**< Type de l'option (Call/Put). */
    OptionStyle style_;   /**< Style de l'option (Européenne/Americaine). */
};

#endif // OPTION_HPP
