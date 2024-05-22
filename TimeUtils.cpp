/**
 * @file TimeUtils.cpp
 * @brief Implementation file for the TimeUtils class which provides utility functions
 * for time-related operations.
 *
 * This file contains the implementation of methods to perform time subtraction, addition,
 * and conversion between time representations.
 *
 * @author Binu Melit Devassy
 * @date 2024-05-20
 *
 * @license MIT License
 */

#include "TimeUtils.h"

namespace Workday {

    // **Subtracts two time tuples (hours, minutes) and returns the resulting time difference (hours, minutes)**
    std::tuple<int, int> TimeUtils::subtractTime(std::tuple<int, int> larger, std::tuple<int, int> smaller) {
        int diff(convertToMinutes(larger) - convertToMinutes(smaller));
        if (diff < 0) {
            diff += MINUTES_IN_DAY;  // Add total minutes in a day to handle negative values
        }
        return{ diff / MINUTES_IN_HOUR,diff % MINUTES_IN_HOUR }; //hours, minutes
    }

    // **Adds two time tuples (hours, minutes) and returns the resulting time sum (hours, minutes)**
    std::tuple<int, int> TimeUtils::addTime(std::tuple<int, int> left, std::tuple<int, int> right) {
        int total(convertToMinutes(left) + convertToMinutes(right));
        return{ total / MINUTES_IN_HOUR,total % MINUTES_IN_HOUR }; //hours, minutes
    }

    // **Subtracts minutes from a larger integer and returns the resulting time (hours, minutes)**
    std::tuple<int, int> TimeUtils::subtractMinutes(int larger, int smaller) {
        int diff(larger - smaller);
        if (diff < 0) {
            diff += MINUTES_IN_DAY;  // Add total minutes in a day to handle negative values
        }
        return{ diff / MINUTES_IN_HOUR,diff % MINUTES_IN_HOUR }; //hours, minutes
    }

    // **Adds minutes to a larger integer and returns the resulting time (hours, minutes)**
    std::tuple<int, int> TimeUtils::addMinutes(int left, int right) {
        int total(left + right);
        return{ total / MINUTES_IN_HOUR, total % MINUTES_IN_HOUR }; //hours, minutes
    }
    
    // **Converts a time tuple (hours, minutes) to total minutes**
    int TimeUtils::convertToMinutes(std::tuple<int, int> time_i) {
        const auto [hours, minutes] = time_i;
        return hours * MINUTES_IN_HOUR + minutes;
    }

} // namespace Workday
