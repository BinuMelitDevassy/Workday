/**
 * @file WorkdayGregorianCalendar.cpp
 * @brief Implementation file for the GregorianCalendar class which manages
 * Gregorian calendar-specific calculations and holiday management.
 *
 * This file contains the implementation of methods to handle holidays,
 * validate dates, and perform date manipulations such as adding or removing days.
 * 
 * @author Binu Melit Devassy
 * @date 2024-05-20
 *
 * @license MIT License
 */

#include "GregorianCalendar.h"

namespace Workday {

    // **Default constructor - calls base class constructor**
    GregorianCalendar::GregorianCalendar() : Calendar() {}

    // **Constructor with specific date and time arguments**
    GregorianCalendar::GregorianCalendar(int year, int month, int day, int hour, int minute)
        : Calendar(year, month, day, hour, minute) {}

    // **Adds a one-time holiday by storing the date string**
    void GregorianCalendar::setHoliday(const Date& date) {
        if (isValidDate(date)) {
            holidays_.insert(date.getDate());
        }
    }

    // **Adds a recurring holiday by storing month and day as a pair**
    void GregorianCalendar::setRecurringHoliday(const Date& date) {
        if (isValidDate(date)) {
            recurring_holidays_.insert(std::make_pair(date.getMonth(), date.getDay()));
        }
    }

    // **Checks for weekends, then one-time and recurring holidays**
    bool GregorianCalendar::isHoliday(const Date& date) const {
        int day_of_week = date.dayOfWeek();
        // Check for Saturday (0) or Sunday (6)
        if (day_of_week == 0 || day_of_week == 6) {
            return true;
        }

        // Check for one-time holidays
        if (holidays_.count(date.getDate()) > 0) {
            return true;
        }

        // Check for recurring holidays
        if (recurring_holidays_.count(std::make_pair(date.getMonth(), date.getDay())) > 0) {
            return true;
        }
        return false;
    }

    // **Validates year, month, day, hour, and minute ranges**
    bool GregorianCalendar::isValidDate(const Date& date) const {
        int year = date.getYear();
        int month = date.getMonth();
        int day = date.getDay();
        int hour = date.getHours();
        int minute = date.getMinutes();

        //checking for valid date
        if (year < 0 || month < 1 || month > 12 || day < 1 || day > daysInMonth(year, month)) {
            return false;
        }

        //checking for valid time
        if (hour < 0 || hour >= 24 || minute < 0 || minute >= 60) {
            return false;
        }

        return true;
    }

    // **Standard Gregorian leap year check**
    bool GregorianCalendar::isLeapYear(int year) const {
        if (year % 4 != 0) return false;
        if (year % 100 != 0) return true;
        if (year % 400 == 0) return true;
        return false;
    }

    // **Calculates number of days in a month considering leap years**
    int GregorianCalendar::daysInMonth(int year, int month) const {
        switch (month) {
        case 1: case 3: case 5: case 7: case 8: case 10: case 12:
            return 31;
        case 4: case 6: case 9: case 11:
            return 30;
        case 2:
            return isLeapYear(year) ? 29 : 28;
        default:
            return 30;
        }
    }

    // **Increments date (year, month, day) handling rollovers**
    void GregorianCalendar::addDay(Date& date_i) const {
        int newYear = date_i.getYear();
        int newMonth = date_i.getMonth();
        int newDay = date_i.getDay() + 1;

        if (newDay > daysInMonth(newYear, newMonth)) {
            newDay = 1;
            newMonth++;
            if (newMonth > 12) {
                newMonth = 1;
                newYear++;
            }
        }

        date_i.setDate(newYear, newMonth, newDay, date_i.getHours(), date_i.getMinutes());
    }

    // **decrements date (year, month, day) handling rollovers**
    void GregorianCalendar::removeDay(Date& date) const {
        int newYear = date.getYear();
        int newMonth = date.getMonth();
        int newDay = date.getDay() - 1;

        if (newDay < 1) {
            newMonth--;
            if (newMonth < 1) {
                newMonth = 12;
                newYear--;
            }
            newDay = daysInMonth(newYear, newMonth);
        }

        date.setDate(newYear, newMonth, newDay, date.getHours(), date.getMinutes());
    }

} // namespace Workday
