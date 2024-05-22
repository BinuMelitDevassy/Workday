/**
 * @file Date.cpp
 * @brief Implementation file for the Date class, representing a date and time.
 *
 * This file provides the implementation of the Date class, including setting date components,
 * retrieving date strings, getting date and time as tuples,
 * determining the day of the week, and generating an invalid date.
 *
 * @author Binu Melit Devassy
 * @date 2024-05-20
 *
 * @license MIT License
 */

#include "Date.h"
#include <iomanip>
#include <sstream>

namespace Workday {
    
    // **Default constructor - initializes all components to 0**
    Date::Date() : year_(0), month_(0), day_(0), hour_(0), minute_(0) {}

    // **Constructor with all components specified**
    Date::Date(int year, int month, int day, int hour, int minute)
        : year_(year), month_(month), day_(day), hour_(hour), minute_(minute) {}

    // **Sets the date components**
    void Date::setDate(int year, int month, int day, int hour, int minute) {
        year_ = year;
        month_ = month;
        day_ = day;
        hour_ = hour;
        minute_ = minute;
    }

    // **Gets the date as a string (YYYY-MM-DD format)**
    std::string Date::getDate() const {
        std::ostringstream oss;
        oss << std::setw(4) << std::setfill('0') << year_ << "-"
            << std::setw(2) << std::setfill('0') << month_ << "-"
            << std::setw(2) << std::setfill('0') << day_ << " ";
        return oss.str();
    }

    // **Gets the date and time as a string (YYYY-MM-DD HH:MM format)**
    std::string Date::getDateAndTime() const {
        std::ostringstream oss;
        oss << std::setw(4) << std::setfill('0') << year_ << "-"
            << std::setw(2) << std::setfill('0') << month_ << "-"
            << std::setw(2) << std::setfill('0') << day_ << " "
            << std::setw(2) << std::setfill('0') << hour_ << ":"
            << std::setw(2) << std::setfill('0') << minute_;
        return oss.str();
    }


    // **Gets the day of the week (0-Sunday, 6-Saturday)**
    // using  Sakamoto, Lachman, Keith and Craver methode to calculate day
    // logic explanation canbe found here https://shorturl.at/j4IyT
    int Date::dayOfWeek() const {

        // not magic numbers! numbers are from Sakamoto, Lachman, Keith and Craver algo
        // used static to avoid re-initialization
        static int t[] = { 0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4 };
        int y = year_; //dont want to modify the member variable
        y -= month_ < 3;
        return (y + y / 4 - y / 100 +
            y / 400 + t[month_ - 1] + day_) % 7;
    }


} // namespace Workday

