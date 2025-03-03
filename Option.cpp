/**
 * @file Option.cpp
 * @brief Impl�mentation de la classe Option et de la structure Greeks pour le pricing d'options.
 *
 * Ce fichier contient l'impl�mentation compl�te de la classe Option, utilis�e pour repr�senter
 * une option financi�re avec ses param�tres de base (sous-jacent, strike, volatilit�, dividende,
 * type et style). Les m�thodes getter et setter permettent de manipuler ces param�tres.
 */

#include "pch.h"
#include "Option.hpp"

 /**
  * @brief Constructeur par d�faut.
  *
  * Initialise l'option avec des valeurs par d�faut :
  * - sous-jacent = 0.0
  * - strike = 0.0
  * - volatilit� = 0.0
  * - dividende = 0.0
  * - type = Call
  * - style = European
  */
Option::Option()
    : underlying_(0.0), strike_(0.0), volatility_(0.0), dividend_(0.0),
    type_(OptionType::Call), style_(OptionStyle::European)
{
    // Constructeur par d�faut : aucune action suppl�mentaire requise.
}

/**
 * @brief Constructeur param�tr�.
 *
 * Initialise l'option avec les param�tres fournis.
 *
 * @param underlying Prix du sous-jacent.
 * @param strike Prix d'exercice.
 * @param volatility Volatilit� du sous-jacent.
 * @param dividend Dividende �ventuel.
 * @param optionType Type de l'option (Call ou Put).
 * @param optionStyle Style de l'option (Europ�enne ou Am�ricaine).
 */
Option::Option(double underlying, double strike, double volatility, double dividend,
    OptionType optionType, OptionStyle optionStyle)
    : underlying_(underlying), strike_(strike), volatility_(volatility), dividend_(dividend),
    type_(optionType), style_(optionStyle)
{
    // Initialisation avec les param�tres fournis.
}

/**
 * @brief Destructeur virtuel.
 *
 * Aucun nettoyage sp�cifique n'est requis car aucune allocation dynamique n'est utilis�e.
 */
Option::~Option()
{
    // Destructeur : rien � lib�rer.
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
 * @brief Obtient la volatilit� du sous-jacent.
 * @return La volatilit�.
 */
double Option::getVolatility() const {
    return volatility_;
}

/**
 * @brief Obtient le dividende �ventuel de l'option.
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
 * @brief Obtient le style de l'option (Europ�enne ou Am�ricaine).
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
 * @brief Modifie la volatilit� du sous-jacent.
 * @param volatility Nouvelle volatilit�.
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
 * @param optionStyle Nouveau style (Europ�enne ou Am�ricaine).
 */
void Option::setOptionStyle(OptionStyle optionStyle) {
    style_ = optionStyle;
}
