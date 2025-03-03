#pragma once
// The #pragma once directive ensures that this header file is included only once in a single compilation,
// preventing duplicate definitions and reducing compile times.

#define NOMINMAX 
// Define NOMINMAX to disable the definition of the min and max macros in the Windows headers.
// This prevents conflicts with the standard library's std::min and std::max functions,
// ensuring that the C++ standard library functions work as expected.

#define WIN32_LEAN_AND_MEAN             
// Define WIN32_LEAN_AND_MEAN to exclude rarely-used services from the Windows headers.
// This macro reduces the size of the Win32 header files by excluding certain APIs (e.g., Cryptography, DDE, RPC, Shell),
// which can improve compile times and reduce potential naming conflicts.

#include <windows.h>
// Include the primary Windows API header.
// This header provides declarations for all of the Windows API functions, data types, and macros used for developing Windows applications.
// It is essential for accessing Windows system functionality and interacting with the operating system.
