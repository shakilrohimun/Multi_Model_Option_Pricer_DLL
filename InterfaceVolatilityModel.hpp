#ifndef INTERFACEVOLATILITYMODEL_HPP
#define INTERFACEVOLATILITYMODEL_HPP

/**
 * @file IVolatilityModel.hpp
 * @brief Abstract interface for volatility models.
 *
 * This file declares the IVolatilityModel interface, which defines the contract for volatility
 * models. Implementations of this interface are required to provide a method for calculating or
 * estimating the volatility at a given time or maturity. This flexibility allows for different
 * approaches such as constant volatility, time-dependent volatility, or even stochastic volatility.
 *
 * The interface serves as a foundation for integrating various volatility models into a pricing
 * framework, ensuring that each model can be used interchangeably.
 */

#include "pch.h" // Precompiled header for improved compilation times.

class IVolatilityModel {
public:
    /**
     * @brief Virtual destructor.
     *
     * Ensures proper cleanup of derived objects when deleted through a pointer to IVolatilityModel.
     *
     * @details The virtual destructor is essential in a polymorphic interface to prevent resource
     * leaks and undefined behavior during object destruction.
     */
    virtual ~IVolatilityModel() = default;

    /**
     * @brief Calculates the volatility at a given time or maturity.
     *
     * This pure virtual function must be implemented by any concrete volatility model. It computes
     * the volatility based on the specific time (or maturity) provided, allowing for the evaluation
     * of dynamic market conditions.
     *
     * @param t The time (or maturity) at which the volatility should be calculated.
     * @return The computed volatility as a double.
     *
     * @details The parameter t can represent the time until an option's maturity or any relevant
     * time interval depending on the model's design. The implementation might involve analytical
     * formulas or numerical methods tailored to the characteristics of the specific volatility model.
     */
    virtual double getVolatility(double t) const = 0;
};

#endif // INTERFACEVOLATILITYMODEL_HPP
