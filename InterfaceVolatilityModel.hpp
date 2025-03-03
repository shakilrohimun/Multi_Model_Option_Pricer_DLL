#ifndef INTERFACEVOLATILITYMODEL_HPP
#define INTERFACEVOLATILITYMODEL_HPP

/**
 * @file IVolatilityModel.hpp
 * @brief Interface pour les modèles de volatilité.
 */

 /**
  * @brief Interface pour un modèle de volatilité.
  *
  * Permet de définir la méthode de calcul ou d'estimation de la volatilité à un instant donné.
  */

#include "pch.h"

class IVolatilityModel {
public:
    /**
     * @brief Destructeur virtuel.
     */
    virtual ~IVolatilityModel() = default;

    /**
     * @brief Calcule la volatilité pour une maturité donnée.
     * @param t Temps ou maturité pour laquelle la volatilité doit être calculée.
     * @return La volatilité calculée.
     */
    virtual double getVolatility(double t) const = 0;
};

#endif // IVOLATILITYMODEL_HPP
