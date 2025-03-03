/**
 * @file YieldCurve.cpp
 * @brief Implementation of the YieldCurve class for managing the interest rate curve.
 *
 * This file implements the YieldCurve class, which allows adding rate points,
 * performing linear interpolation to obtain an interest rate for a given maturity,
 * and loading the rate data from a text file.
 */

#include "pch.h"
#include "YieldCurve.hpp"
#include <stdexcept>
#include <fstream>
#include <sstream>

void YieldCurve::addRatePoint(double maturity, double rate) {
    data_.push_back({ maturity, rate });
}

double YieldCurve::getRate(double t) const {
    if (data_.empty()) {
        throw std::runtime_error("YieldCurve is empty");
    }

    // If t is less than or equal to the first point, return its rate.
    if (t <= data_.front().maturity) {
        return data_.front().rate;
    }
    // If t is greater than or equal to the last point, return its rate.
    if (t >= data_.back().maturity) {
        return data_.back().rate;
    }

    // Traverse the vector to find the interval containing t.
    for (size_t i = 1; i < data_.size(); ++i) {
        if (t < data_[i].maturity) {
            const RatePoint& p0 = data_[i - 1];
            const RatePoint& p1 = data_[i];
            // Linear interpolation factor.
            double factor = (t - p0.maturity) / (p1.maturity - p0.maturity);
            return p0.rate + factor * (p1.rate - p0.rate);
        }
    }

    // Default return (should not be reached).
    return data_.back().rate;
}

const std::vector<RatePoint>& YieldCurve::getData() const {
    return data_;
}

void YieldCurve::loadFromFile(const std::string& filename) {
    std::ifstream infile(filename);
    if (!infile) {
        throw std::runtime_error("Cannot open file: " + filename);
    }

    std::string line;
    while (std::getline(infile, line)) {
        if (line.empty()) continue;  // Skip empty lines.
        std::istringstream iss(line);
        double maturity, rate;
        if (!(iss >> maturity >> rate)) {
            throw std::runtime_error("Invalid format in file: " + line);
        }
        addRatePoint(maturity, rate);
    }
}
