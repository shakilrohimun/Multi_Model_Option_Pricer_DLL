#ifndef DATECONVERTER_HPP
#define DATECONVERTER_HPP

#include "pch.h"
#include <string>
#include <chrono>

/**
 * @brief A utility class for performing date conversions and calculations.
 *
 * This class provides static methods to convert between date strings and
 * std::chrono time points, as well as to compute time differences expressed in years.
 * The functions in this class are designed to handle dates in the ISO 8601 format ("YYYY-MM-DD")
 * and leverage the C++ standard library's <chrono> facilities to represent and manipulate time.
 *
 * @note All methods in this class are static and can be used without instantiating an object.
 *
 * @see std::chrono::system_clock, std::string
 */
class DateConverter {
public:
    /**
     * @brief Retrieves the current system date.
     *
     * This function obtains the current date from the system clock and returns it as a string formatted
     * according to the ISO 8601 standard ("YYYY-MM-DD"). It uses the facilities provided by the C++ standard library
     * to convert the current time to a human-readable format.
     *
     * @return A std::string representing today's date in the format "YYYY-MM-DD".
     *
     * @remark Useful for timestamping, logging, or as a default value for date-related calculations.
     */
    static std::string getTodayDate();

    /**
     * @brief Converts an ISO 8601 formatted date string to a time point.
     *
     * This function parses a date string provided in the ISO 8601 format ("YYYY-MM-DD") and converts it into a
     * std::chrono::system_clock::time_point. This time point can then be used for further date and time calculations
     * within the application.
     *
     * @param dateStr A std::string containing the date in the format "YYYY-MM-DD".
     * @return A std::chrono::system_clock::time_point corresponding to the parsed date.
     *
     * @throw std::runtime_error If the input date string is invalid or does not conform to the "YYYY-MM-DD" format.
     *
     * @note The use of ISO 8601 format ensures consistency and avoids ambiguity, particularly in international contexts.
     */
    static std::chrono::system_clock::time_point parseDate(const std::string& dateStr);

    /**
     * @brief Calculates the time difference between two dates in years.
     *
     * This function computes the difference between two std::chrono::system_clock::time_point values and returns the result
     * as a double representing the number of years between them. The calculation is based on the exact difference in days,
     * converting that difference into fractional years.
     *
     * @param start A std::chrono::system_clock::time_point representing the start date.
     * @param end A std::chrono::system_clock::time_point representing the end date.
     * @return A double indicating the time difference in years.
     *
     * @remark The result may include fractional years to accurately represent partial year differences, which is particularly
     * useful in financial or scientific applications where precise time intervals are critical.
     */
    static double yearsBetween(const std::chrono::system_clock::time_point& start,
        const std::chrono::system_clock::time_point& end);
};

#endif // DATECONVERTER_HPP
