#ifndef INTERFACEVOLATILITYMODEL_HPP
#define INTERFACEVOLATILITYMODEL_HPP

/**
 * @file IVolatilityModel.hpp
 * @brief Interface pour les mod�les de volatilit�.
 */

 /**
  * @brief Interface pour un mod�le de volatilit�.
  *
  * Permet de d�finir la m�thode de calcul ou d'estimation de la volatilit� � un instant donn�.
  */

#include "pch.h"

class IVolatilityModel {
public:
    /**
     * @brief Destructeur virtuel.
     */
    virtual ~IVolatilityModel() = default;

    /**
     * @brief Calcule la volatilit� pour une maturit� donn�e.
     * @param t Temps ou maturit� pour laquelle la volatilit� doit �tre calcul�e.
     * @return La volatilit� calcul�e.
     */
    virtual double getVolatility(double t) const = 0;
};

#endif // IVOLATILITYMODEL_HPP
