/**
 * @file Option.cpp
 * @brief Implémentation de la classe Option et de la structure Greeks pour le pricing d'options.
 *
 * Ce fichier contient l'implémentation complète de la classe Option, utilisée pour représenter
 * une option financière avec ses paramètres de base (sous-jacent, strike, volatilité, dividende,
 * type et style). Les méthodes getter et setter permettent de manipuler ces paramètres.
 */

#include "pch.h"
#include "Option.hpp"

 /**
  * @brief Constructeur par défaut.
  *
  * Initialise l'option avec des valeurs par défaut :
  * - sous-jacent = 0.0
  * - strike = 0.0
  * - volatilité = 0.0
  * - dividende = 0.0
  * - type = Call
  * - style = European
  */
Option::Option()
    : underlying_(0.0), strike_(0.0), volatility_(0.0), dividend_(0.0),
    type_(OptionType::Call), style_(OptionStyle::European)
{
    // Constructeur par défaut : aucune action supplémentaire requise.
}

/**
 * @brief Constructeur paramétré.
 *
 * Initialise l'option avec les paramètres fournis.
 *
 * @param underlying Prix du sous-jacent.
 * @param strike Prix d'exercice.
 * @param volatility Volatilité du sous-jacent.
 * @param dividend Dividende éventuel.
 * @param optionType Type de l'option (Call ou Put).
 * @param optionStyle Style de l'option (Européenne ou Américaine).
 */
Option::Option(double underlying, double strike, double volatility, double dividend,
    OptionType optionType, OptionStyle optionStyle)
    : underlying_(underlying), strike_(strike), volatility_(volatility), dividend_(dividend),
    type_(optionType), style_(optionStyle)
{
    // Initialisation avec les paramètres fournis.
}

/**
 * @brief Destructeur virtuel.
 *
 * Aucun nettoyage spécifique n'est requis car aucune allocation dynamique n'est utilisée.
 */
Option::~Option()
{
    // Destructeur : rien à libérer.
}

/**
 * @brief Obtient le prix du sous-jacent.
 * @return La valeur du sous-jacent.
 */
double Option::getUnderlying() const {
    return underlying_;
}

/**
 * @brief Obtient le prix d'exercice (strike) de l'option.
 * @return Le strike de l'option.
 */
double Option::getStrike() const {
    return strike_;
}

/**
 * @brief Obtient la volatilité du sous-jacent.
 * @return La volatilité.
 */
double Option::getVolatility() const {
    return volatility_;
}

/**
 * @brief Obtient le dividende éventuel de l'option.
 * @return Le dividende.
 */
double Option::getDividend() const {
    return dividend_;
}

/**
 * @brief Obtient le type de l'option (Call ou Put).
 * @return Le type de l'option.
 */
Option::OptionType Option::getOptionType() const {
    return type_;
}

/**
 * @brief Obtient le style de l'option (Européenne ou Américaine).
 * @return Le style de l'option.
 */
Option::OptionStyle Option::getOptionStyle() const {
    return style_;
}

/**
 * @brief Modifie le prix du sous-jacent.
 * @param underlying Nouveau prix du sous-jacent.
 */
void Option::setUnderlying(double underlying) {
    underlying_ = underlying;
}

/**
 * @brief Modifie le prix d'exercice (strike) de l'option.
 * @param strike Nouveau strike.
 */
void Option::setStrike(double strike) {
    strike_ = strike;
}

/**
 * @brief Modifie la volatilité du sous-jacent.
 * @param volatility Nouvelle volatilité.
 */
void Option::setVolatility(double volatility) {
    volatility_ = volatility;
}

/**
 * @brief Modifie le dividende de l'option.
 * @param dividend Nouveau dividende.
 */
void Option::setDividend(double dividend) {
    dividend_ = dividend;
}

/**
 * @brief Modifie le type de l'option.
 * @param optionType Nouveau type (Call ou Put).
 */
void Option::setOptionType(OptionType optionType) {
    type_ = optionType;
}

/**
 * @brief Modifie le style de l'option.
 * @param optionStyle Nouveau style (Européenne ou Américaine).
 */
void Option::setOptionStyle(OptionStyle optionStyle) {
    style_ = optionStyle;
}
