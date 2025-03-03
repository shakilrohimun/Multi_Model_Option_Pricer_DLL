/**
 * @file Option.cpp
 * @brief Implementation de la classe Option et de la structure Greeks pour le pricing d'options.
 *
 * Ce fichier contient l'implementation complete de la classe Option, utilisee pour representer
 * une option financiere avec ses parametres de base (sous-jacent, strike, volatilite, dividende,
 * type et style). Les methodes getter et setter permettent de manipuler ces parametres.
 */

#include "pch.h"
#include "Option.hpp"

 /**
  * @brief Constructeur par defaut.
  *
  * Initialise l'option avec des valeurs par defaut :
  * - sous-jacent = 0.0
  * - strike = 0.0
  * - volatilite = 0.0
  * - dividende = 0.0
  * - type = Call
  * - style = European
  */
Option::Option()
    : underlying_(0.0), strike_(0.0), volatility_(0.0), dividend_(0.0),
    type_(OptionType::Call), style_(OptionStyle::European)
{
    // Constructeur par defaut : aucune action supplementaire requise.
}

/**
 * @brief Constructeur parametre.
 *
 * Initialise l'option avec les parametres fournis.
 *
 * @param underlying Prix du sous-jacent.
 * @param strike Prix d'exercice.
 * @param volatility Volatilite du sous-jacent.
 * @param dividend Dividende eventuel.
 * @param optionType Type de l'option (Call ou Put).
 * @param optionStyle Style de l'option (European ou American).
 */
Option::Option(double underlying, double strike, double volatility, double dividend,
    OptionType optionType, OptionStyle optionStyle)
    : underlying_(underlying), strike_(strike), volatility_(volatility), dividend_(dividend),
    type_(optionType), style_(optionStyle)
{
    // Initialisation avec les parametres fournis.
}

/**
 * @brief Destructeur virtuel.
 *
 * Aucun nettoyage specifique n'est requis car aucune allocation dynamique n'est utilisee.
 */
Option::~Option()
{
    // Destructeur : rien a liberer.
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
 * @brief Obtient la volatilite du sous-jacent.
 * @return La volatilite.
 */
double Option::getVolatility() const {
    return volatility_;
}

/**
 * @brief Obtient le dividende eventuel de l'option.
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
 * @brief Obtient le style de l'option (European ou American).
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
 * @brief Modifie la volatilite du sous-jacent.
 * @param volatility Nouvelle volatilite.
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
 * @param optionStyle Nouveau style (European ou American).
 */
void Option::setOptionStyle(OptionStyle optionStyle) {
    style_ = optionStyle;
}
