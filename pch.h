// pch.h : Il s'agit d'un fichier d'en-tête précompilé.
// Les fichiers listés ci-dessous sont compilés une seule fois, ce qui améliore les performances de génération des futures builds.
// Cela affecte également les performances d'IntelliSense, notamment la complétion du code et de nombreuses fonctionnalités de navigation du code.
// Toutefois, les fichiers listés ici sont TOUS recompilés si l'un d'entre eux est mis à jour entre les builds.
// N'ajoutez pas de fichiers fréquemment mis à jour ici, car cela annule les gains de performance.

#ifndef PCH_HPP
#define PCH_HPP

// Include the framework header
#include "framework.h"

// Standard library headers used frequently across the project
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <chrono>
#include <stdexcept>
#include <cmath>

// Common project headers for option pricing
#include "Option.hpp"
#include "PricingConfiguration.hpp"
#include "YieldCurve.hpp"
#include "DateConverter.hpp"
#include "InterfaceOptionPricer.hpp"

#endif // PCH_HPP

