#ifndef DATECONVERTER_HPP
#define DATECONVERTER_HPP

#include "pch.h"
#include <string>
#include <chrono>

/**
 * @brief Utility class for date conversions.
 *
 * This class provides functions to convert between date strings and time points,
 * as well as to compute time differences in years.
 */
class DateConverter {
public:
    /**
     * @brief Retrieves today's date as a string in the format "YYYY-MM-DD".
     * @return A string representing the current date.
     */
    static std::string getTodayDate();

    /**
     * @brief Parses an ISO 8601 date string into a std::chrono::system_clock::time_point.
     * @param dateStr The date string in the format "YYYY-MM-DD".
     * @return A time_point representing the parsed date.
     * @throw std::runtime_error if the date cannot be parsed.
     */
    static std::chrono::system_clock::time_point parseDate(const std::string& dateStr);

    /**
     * @brief Computes the difference in years between two time points.
     * @param start The start time point.
     * @param end The end time point.
     * @return The time difference in years as a double.
     */
    static double yearsBetween(const std::chrono::system_clock::time_point& start,
        const std::chrono::system_clock::time_point& end);
};

#endif // DATECONVERTER_HPP
