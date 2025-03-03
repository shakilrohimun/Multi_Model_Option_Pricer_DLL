#include "pch.h"
#include "DateConverter.hpp"
#include <chrono>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <stdexcept>

/**
 * @brief Retrieves today's date as a string in the format "YYYY-MM-DD".
 *
 * This function obtains the current system time, converts it to local time,
 * and formats it as a string.
 *
 * @return A string representing today's date.
 */
std::string DateConverter::getTodayDate() {
    // Get the current time point from the system clock.
    auto now = std::chrono::system_clock::now();
    // Convert the time point to a time_t object.
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    // Convert time_t to a tm struct representing local time.
    std::tm local_tm;
#if defined(_MSC_VER) || defined(__MINGW32__)
    localtime_s(&local_tm, &now_time);
#else
    localtime_r(&now_time, &local_tm);
#endif
    // Format the time using an ostringstream.
    std::ostringstream oss;
    oss << std::put_time(&local_tm, "%Y-%m-%d");
    return oss.str();
}

/**
 * @brief Parses an ISO 8601 date string into a time_point.
 *
 * This function converts a date string in the "YYYY-MM-DD" format into a
 * std::chrono::system_clock::time_point.
 *
 * @param dateStr The date string to parse.
 * @return A time_point corresponding to the parsed date.
 * @throw std::runtime_error if parsing fails.
 */
std::chrono::system_clock::time_point DateConverter::parseDate(const std::string& dateStr) {
    std::tm tm = {};
    std::istringstream ss(dateStr);
    ss >> std::get_time(&tm, "%Y-%m-%d");
    if (ss.fail()) {
        throw std::runtime_error("Failed to parse date: " + dateStr);
    }
    std::time_t time = std::mktime(&tm);
    return std::chrono::system_clock::from_time_t(time);
}

/**
 * @brief Computes the difference in years between two time points.
 *
 * This function calculates the number of years between the start and end time points.
 * It first converts the time difference to days and then divides by 365.25.
 *
 * @param start The start time point.
 * @param end The end time point.
 * @return The difference in years.
 */
double DateConverter::yearsBetween(const std::chrono::system_clock::time_point& start,
    const std::chrono::system_clock::time_point& end) {
    auto diff = end - start;
    // Convert duration to days (using 24 hours per day)
    auto days = std::chrono::duration_cast<std::chrono::hours>(diff).count() / 24.0;
    return days / 365.25;
}
