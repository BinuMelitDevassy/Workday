/**
 * @file WorkdayGregorianCalendar.h
 * @brief Header file for the Workday::GregorianCalendar class, managing Gregorian calendar calculations
 * including holidays and workdays.
 *
 * This class provides functionality to manage Gregorian calendar-specific calculations,
 * including setting holidays, checking for holidays, adding and removing days, and validating dates.
 *
 * @author Binu Melit Devassy 
 * @date 2024-05-20
 *
 * @license MIT License
 */

#ifndef WORKDAY_GREGORIAN_CALENDAR_H
#define WORKDAY_GREGORIAN_CALENDAR_H

#include "Calendar.h"
#include <set>

namespace Workday {

    /**
     * @class GregorianCalendar
     * @brief Manages Gregorian calendar calculations including holidays and workdays.
     */
    class GregorianCalendar : public Calendar {
    public:
        /**
         * @brief Default constructor.
         */
        GregorianCalendar();

        /**
         * @brief Parameterized constructor.
         * @param year Year component.
         * @param month Month component.
         * @param day Day component.
         * @param hour Hour component.
         * @param minute Minute component.
         */
        GregorianCalendar(int year, int month, int day, int hour, int minute);

        /**
         * @brief Default destructor.
         */
        virtual ~GregorianCalendar() = default;

        /**
         * @brief Sets a holiday on the specified date.
         * @param date The date to set as a holiday.
         */
        void setHoliday(const Date& date) override;

        /**
         * @brief Sets a recurring holiday on the specified date.
         * @param date The date to set as a recurring holiday.
         */
        void setRecurringHoliday(const Date& date) override;

        /**
         * @brief Checks if the specified date is a holiday.
         * @param date The date to check.
         * @return True if the date is a holiday, false otherwise.
         */
        bool isHoliday(const Date& date) const override;

        /**
         * @brief Checks if the specified date is valid.
         * @param date The date to check.
         * @return True if the date is valid, false otherwise.
         */
        bool isValidDate(const Date& date) const override;

    private:
        std::set<std::string> holidays_; /**< Set of holiday dates. */
        std::set<std::pair<int, int>> recurring_holidays_; /**< Set of recurring holiday dates. */

        /**
         * @brief Checks if the specified year is a leap year.
         * @param year The year to check.
         * @return True if the year is a leap year, false otherwise.
         */
        bool isLeapYear(int year) const;

        /**
         * @brief Returns the number of days in the specified month of the specified year.
         * @param year The year.
         * @param month The month.
         * @return The number of days in the month.
         */
        int daysInMonth(int year, int month) const;

        /**
         * @brief Adds a day to the specified date, considering month and year transitions.
         * @param date_i The date to add a day to.
         */
        void addDay(Date& date_i) const override;

        /**
         * @brief Removes a day from the specified date, considering month and year transitions.
         * @param date The date to remove a day from.
         */
        void removeDay(Date& date) const override;
    };

} // namespace Workday

#endif // WORKDAY_GREGORIAN_CALENDAR_H
