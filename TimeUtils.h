/**
 * @file TimeUtils.h
 * @brief Header file for the Workday::TimeUtils class, providing utility functions for time calculations.
 *
 * This class provides static utility functions for time calculations, including addition and subtraction
 * of time represented in hours and minutes, as well as conversion of time to minutes.
 *
 * @author Binu Melit Devassy
 * @date 2024-05-20
 *
 * @license MIT License
 */

#ifndef TIME_UTILS_H
#define TIME_UTILS_H

#include <tuple>

namespace Workday {

    //constants decleration
    const int HOURS_IN_DAY = 24;
    const int MINUTES_IN_HOUR = 60;
    const int MINUTES_IN_DAY = MINUTES_IN_HOUR * HOURS_IN_DAY;
    const int SECONDS_IN_HOUR = 3600;
    const int SECONDS_IN_MINUTE = 60;

    /**
     * @class TimeUtils
     * @brief Provides utility functions for time calculations.
     */
    class TimeUtils {
    public:
        /**
         * @brief Subtracts two time values represented as tuples of hours and minutes.
         * @param larger The larger time value.
         * @param smaller The smaller time value.
         * @return A tuple representing the difference in hours and minutes.
         */
        static std::tuple<int, int> subtractTime(std::tuple<int, int> larger, std::tuple<int, int> smaller);

        /**
         * @brief Adds two time values represented as tuples of hours and minutes.
         * @param left The first time value.
         * @param right The second time value.
         * @return A tuple representing the sum in hours and minutes.
         */
        static std::tuple<int, int> addTime(std::tuple<int, int> left, std::tuple<int, int> right);

        /**
         * @brief Subtracts two time values represented in minutes.
         * @param larger The larger time value in minutes.
         * @param smaller The smaller time value in minutes.
         * @return A tuple representing the difference in hours and minutes.
         */
        static std::tuple<int, int> subtractMinutes(int larger, int smaller);

        /**
         * @brief Adds two time values represented in minutes.
         * @param left The first time value in minutes.
         * @param right The second time value in minutes.
         * @return A tuple representing the sum in hours and minutes.
         */
        static std::tuple<int, int> addMinutes(int left, int right);

        /**
         * @brief Converts a time value represented as a tuple of hours and minutes to minutes.
         * @param time_i The time value as a tuple of hours and minutes.
         * @return The time value in minutes.
         */
        static int convertToMinutes(std::tuple<int, int> time_i);
    };

} // namespace Workday

#endif // TIME_UTILS_H
