/**
 * @file Date.h
 * @brief Header file for the Date class, representing a date and time.
 *
 * This class provides functionality to represent and manipulate dates and times,
 * including setting date components, retrieving date strings, getting date and time as tuples,
 * converting to std::tm, and determining the day of the week.
 * It also includes a function to generate an invalid date with all values set to -1.
 *
 * @author Binu Melit Devassy
 * @date 2024-05-20
 *
 * @license MIT License
 */

#ifndef DATE_H
#define DATE_H

#include <string>
#include <tuple>

namespace Workday {
    class Date {
    public:
        /**
         * @brief Default constructor.
         * Constructs a Date object with all components set to 0.
         */
        Date();

        /**
         * @brief Parameterized constructor.
         * Constructs a Date object with the specified date and time components.
         *
         * @param year The year component of the date.
         * @param month The month component of the date.
         * @param day The day component of the date.
         * @param hour The hour component of the date.
         * @param minute The minute component of the date.
         */
        Date(int year, int month, int day, int hour, int minute);

        /**
         * @brief Sets the date and time components of the Date object.
         *
         * @param year The year component of the date.
         * @param month The month component of the date.
         * @param day The day component of the date.
         * @param hour The hour component of the date.
         * @param minute The minute component of the date.
         */
        void setDate(int year, int month, int day, int hour, int minute);

        /**
         * @brief Returns the date string representation in "YYYY-MM-DD" format.
         *
         * @return A string representing the date.
         */
        std::string getDate() const;

        /**
         * @brief Returns the date and time string representation in "YYYY-MM-DD HH:MM" format.
         *
         * @return A string representing the date and time.
         */
        std::string getDateAndTime() const;

        /**
         * @brief Returns the hour and minute components as a tuple.
         *
         * @return A tuple containing the hour and minute components.
         */
        std::tuple<int, int> getTime() const {
            return std::make_tuple(hour_, minute_);
        }


        /**
         * @brief Returns the year component of the date.
         *
         * @return The year component.
         */
        int getYear() const {
            return year_;
        }

        /**
         * @brief Returns the month component of the date.
         *
         * @return The month component.
         */
        int getMonth() const {
            return month_;
        }

        /**
         * @brief Returns the day component of the date.
         *
         * @return The day component.
         */
        int getDay() const {
            return day_;
        }

        /**
         * @brief Returns the hour component of the date.
         *
         * @return The hour component.
         */
        int getHours() const {
            return hour_;
        }

        /**
         * @brief Returns the minute component of the date.
         *
         * @return The minute component.
         */
        int getMinutes() const {
            return minute_;
        }

        /**
         * @brief Determines the day of the week.
         *
         * @return The day of the week, where 0 = Sunday, 1 = Monday, ..., 6 = Saturday.
         */
        int dayOfWeek() const;

        /**
         * @brief Generates an invalid date with all components set to -1.
         *
         * @return An invalid Date object.
         */
        Date generateInvalidDate() const {
            return Date(-1, -1, -1, -1, -1);
        }

    private:
        int year_;   ///< The year component of the date.
        int month_;  ///< The month component of the date.
        int day_;    ///< The day component of the date.
        int hour_;   ///< The hour component of the date.
        int minute_; ///< The minute component of the date.
    };
} // namespace Workday
#endif // DATE_H
