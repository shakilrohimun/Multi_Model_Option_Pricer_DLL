#ifndef INTERFACEOPTIONPRICER_HPP
#define INTERFACEOPTIONPRICER_HPP

/**
 * @file IOptionPricer.hpp
 * @brief Interface abstraite pour les moteurs de pricing d'options.
 */

#include "pch.h"
#include "Option.hpp"

 /**
  * @brief Interface pour un moteur de pricing d'options.
  *
  * Cette interface définit les méthodes nécessaires pour calculer le prix et les greeks d'une option.
  */
class IOptionPricer {
public:
    /**
     * @brief Destructeur virtuel.
     */
    virtual ~IOptionPricer() = default;

    /**
     * @brief Calcule le prix de l'option.
     * @param opt L'option à évaluer.
     * @return Le prix calculé de l'option.
     */
    virtual double price(const Option& opt) const = 0;

    /**
     * @brief Calcule les greeks de l'option.
     * @param opt L'option à évaluer.
     * @return Une structure Greeks contenant les greeks calculés.
     */
    virtual Greeks computeGreeks(const Option& opt) const = 0;
};

#endif // IOPTIONPRICER_HPP
