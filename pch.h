// pch.h : This is a precompiled header file.
// The files listed below are compiled only once, which improves build performance for subsequent builds.
// This also enhances IntelliSense performance, including code completion and various navigation features.
// However, all files listed here will be recompiled if any one of them is updated between builds.
// Do not add frequently modified files here, as that will negate the performance benefits.

#ifndef PCH_HPP
#define PCH_HPP

// Include the framework header.
// This header typically includes fundamental definitions and libraries required by the project.
#include "framework.h"

// Standard library headers that are frequently used throughout the project.
// Including these in the precompiled header speeds up compilation by compiling them only once.
#include <iostream>   // Provides input/output stream functionality.
#include <string>     // Supports usage of std::string for string manipulation.
#include <vector>     // Enables the use of the std::vector container.
#include <memory>     // Contains utilities for dynamic memory management (e.g., smart pointers).
#include <chrono>     // Provides facilities for time manipulation and measurement.
#include <stdexcept>  // Includes standard exception types.
#include <cmath>      // Provides common mathematical functions.

// Common project headers used in option pricing modules.
// These headers contain declarations for classes and functions critical to the pricing application.
#include "Option.hpp"               // Defines the Option class and associated structures.
#include "PricingConfiguration.hpp" // Contains configuration parameters for pricing models.
#include "YieldCurve.hpp"           // Provides classes and functions to manage yield curves.
#include "DateConverter.hpp"        // Offers utility functions for converting and handling dates.
#include "InterfaceOptionPricer.hpp"// Declares the interface for option pricing engines.

#endif // PCH_HPP
