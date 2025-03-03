#ifndef OPTION_HPP
#define OPTION_HPP

/**
 * @file Option.hpp
 * @brief D�claration de la classe Option et de la structure Greeks pour le pricing d'options.
 */

#include "pch.h"
#include <string>

 /**
  * @brief Structure regroupant les greeks d'une option.
  */
struct Greeks {
    double delta; /**< Sensibilit� par rapport au prix du sous-jacent. */
    double gamma; /**< Sensibilit� de delta par rapport au prix du sous-jacent. */
    double vega;  /**< Sensibilit� par rapport � la volatilit�. */
    double theta; /**< Sensibilit� par rapport au temps. */
    double rho;   /**< Sensibilit� par rapport au taux d'int�r�t. */
};

/**
 * @brief Classe repr�sentant une option financi�re.
 *
 * Cette classe encapsule les param�tres essentiels d'une option,
 * tels que le prix du sous-jacent, le strike, la volatilit�,
 * le dividende �ventuel, le type (Call/Put) et le style (Europ�enne/Americaine).
 */
class Option {
public:
    /**
     * @brief Enum�ration pour le type d'option.
     */
    enum class OptionType {
        Call, /**< Option d'achat. */
        Put   /**< Option de vente. */
    };

    /**
     * @brief Enum�ration pour le style de l'option.
     */
    enum class OptionStyle {
        European, /**< Option europ�enne. */
        American  /**< Option am�ricaine. */
    };

    /**
     * @brief Constructeur par d�faut.
     */
    Option();

    /**
     * @brief Constructeur param�tr�.
     * @param underlying Prix du sous-jacent.
     * @param strike Prix d'exercice.
     * @param volatility Volatilit� du sous-jacent.
     * @param dividend Dividende �ventuel.
     * @param optionType Type de l'option (Call ou Put).
     * @param optionStyle Style de l'option (Europ�enne ou Am�ricaine).
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
     * @brief Obtient la volatilit�.
     * @return La volatilit� du sous-jacent.
     */
    double getVolatility() const;

    /**
     * @brief Obtient le dividende �ventuel.
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
     * @return Le style (Europ�enne ou Am�ricaine).
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
     * @brief Modifie la volatilit�.
     * @param volatility Nouvelle volatilit�.
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
     * @param optionStyle Nouveau style (Europ�enne ou Am�ricaine).
     */
    void setOptionStyle(OptionStyle optionStyle);

private:
    double underlying_;   /**< Prix du sous-jacent. */
    double strike_;       /**< Prix d'exercice de l'option. */
    double volatility_;   /**< Volatilit� du sous-jacent. */
    double dividend_;     /**< Dividende �ventuel. */
    OptionType type_;     /**< Type de l'option (Call/Put). */
    OptionStyle style_;   /**< Style de l'option (Europ�enne/Americaine). */
};

#endif // OPTION_HPP
