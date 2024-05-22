/**
 * @file Calendar.h
 * @brief Header file for the Calendar interface class, which provides methods for managing dates.
 *
 * This class provides an abstract interface for managing dates, including setting holidays,
 * adding and removing days, and checking if a given date is a holiday or valid.
 * Subclasses of Calendar can implement specific calendar systems, such as Gregorian or Julian.
 *
 * @author Binu Melit Devassy
 * @date 2024-05-20
 */

#ifndef CALENDAR_H
#define CALENDAR_H

#include "Date.h"

namespace Workday {
    class Calendar {
    public:
        /**
         * @brief Default constructor.
         * Constructs a Calendar object with default date values.
         */
        Calendar() : date_() {}

        /**
         * @brief Parameterized constructor.
         * Constructs a Calendar object with the specified date values.
         *
         * @param year The year component of the date.
         * @param month The month component of the date.
         * @param day The day component of the date.
         * @param hour The hour component of the date.
         * @param minute The minute component of the date.
         */
        Calendar(int year, int month, int day, int hour, int minute)
            : date_(year, month, day, hour, minute) {}

        /**
         * @brief Sets the date components of the calendar.
         *
         * @param year The year component of the date.
         * @param month The month component of the date.
         * @param day The day component of the date.
         * @param hour The hour component of the date.
         * @param minute The minute component of the date.
         */
        void setDate(int year, int month, int day, int hour, int minute) {
            date_.setDate(year, month, day, hour, minute);
        }

        /**
         * @brief Gets the date string representation.
         *
         * @return A string representing the date in "YYYY-MM-DD" format.
         */
        std::string getDate() const {
            return date_.getDate();
        }

        /**
         * @brief Sets a holiday on the calendar.
         * Pure virtual function to be implemented by subclasses.
         *
         * @param date The date of the holiday.
         */
        virtual void setHoliday(const Date& date) = 0;

        /**
         * @brief Sets a recurring holiday on the calendar.
         * Pure virtual function to be implemented by subclasses.
         *
         * @param date The date of the recurring holiday.
         */
        virtual void setRecurringHoliday(const Date& date) = 0;

        /**
         * @brief Adds a day to the given date.
         * Pure virtual function to be implemented by subclasses.
         *
         * @param date_i The date to which a day will be added.
         */
        virtual void addDay(Date& date_i) const = 0;

        /**
         * @brief Removes a day from the given date.
         * Pure virtual function to be implemented by subclasses.
         *
         * @param date_i The date from which a day will be removed.
         */
        virtual void removeDay(Date& date_i) const = 0;

        /**
         * @brief Checks if the given date is a holiday.
         * Pure virtual function to be implemented by subclasses.
         *
         * @param date The date to check.
         * @return True if the date is a holiday, otherwise false.
         */
        virtual bool isHoliday(const Date& date) const = 0;

        /**
         * @brief Checks if the given date is valid.
         * Pure virtual function to be implemented by subclasses.
         *
         * @param date The date to check.
         * @return True if the date is valid, otherwise false.
         */
        virtual bool isValidDate(const Date& date) const = 0;

        /**
         * @brief Virtual destructor.
         * Destructor to ensure proper cleanup when deleting subclasses.
         */
        virtual ~Calendar() = default;

    protected:
        Date date_; ///< The date managed by the calendar.
    };
}// namespace Workday

#endif // CALENDAR_H
