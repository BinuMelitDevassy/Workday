/**
 * @file WorkdayCalendar.cpp
 * @brief Implementation file for the WorkdayCalendar class which manages workday calculations considering holidays and
 * work hours.
 *
 * This file contains the implementation of methods to manage workday calculations, including setting workday start and stop times,
 * handling holidays, incrementing or decrementing workdays, and calculating dates after a specified number of workdays.
 *
 * @author Binu Melit Devassy
 * @date 2024-05-20
 *
 * @license MIT License
 */

#include "WorkdayCalendar.h"
#include "Date.h"
#include "TimeUtils.h"
#include "GregorianCalendar.h"
#include "logger.h"

namespace Workday{

    // **Constructor**
    WorkdayCalendar::WorkdayCalendar():
        calendar_(std::make_unique<GregorianCalendar>()),
        workday_start_(nullptr), workday_stop_(nullptr),
        workday_duration_(nullptr){}

    // **Sets workday start and stop times**
    void WorkdayCalendar::setWorkdayStartAndStop(const Date& start, const Date& stop) {

        try {
            std::lock_guard<std::mutex> lock(mtx_);  // Acquires a lock on the mutex for thread safety

            //check the incoming date is valid
            if (!calendar_->isValidDate(start)) {
                // return invalid date
                Logger::getInstance().logInfo("Invalid startdate", LOG_LOCATION);
                workday_start_ = nullptr;
                workday_stop_ = nullptr;
                return;
            }

            //check the incoming date is valid
            if (!calendar_->isValidDate(stop)) {
                // return invalid date
                Logger::getInstance().logInfo("Invalid stopdate", LOG_LOCATION);
                workday_start_ = nullptr;
                workday_stop_ = nullptr;
                return;
            }

            workday_start_ = std::make_unique<Date>(start.getYear(), start.getMonth(), start.getDay(), start.getHours(),
                start.getMinutes());  // Creates a unique_ptr to a new Date object with start time
            workday_stop_ = std::make_unique<Date>(stop.getYear(), stop.getMonth(), stop.getDay(), stop.getHours(),
                stop.getMinutes());   // Creates a unique_ptr to a new Date object with stop time
            updateWorkingDuration();  // Updates the workday duration after setting start and stop times
        }
        catch (std::exception e) {
            Logger::getInstance().logError(e.what(), LOG_LOCATION);
        }
    }

    // **Sets a one-time holiday**
    void WorkdayCalendar::setHoliday(const Date& date) {
        try {
            std::lock_guard<std::mutex> lock(mtx_);
            calendar_->setHoliday(date);
        }
        catch (std::exception e) {
            Logger::getInstance().logError(e.what(), LOG_LOCATION);
        }
    }

    // **Sets a recurring holiday**
    void WorkdayCalendar::setRecurringHoliday(const Date& date) {
        try {
            std::lock_guard<std::mutex> lock(mtx_);
            calendar_->setRecurringHoliday(date);
        }
        catch (std::exception e) {
            Logger::getInstance().logError(e.what(), LOG_LOCATION);
        }
    }

    // **Increments or decrements a work week**
    void WorkdayCalendar::incrementWorkWeek(Date& startDate, bool decrement) {
        for (int i = 0; i < WORKWEEK_DURATION; ++i) {
            incrementWorkDay(startDate, decrement);
        }
    }

    // **Increments or decrements a work day considering holidays**
    void WorkdayCalendar::incrementWorkDay(Date& startDate, bool decrement) {
        if (decrement) {
            calendar_->removeDay(startDate);
        }
        else {
            calendar_->addDay(startDate);
        }
        // Skips holidays until a non-holiday workday is found
        while (calendar_->isHoliday(startDate)) {
            if (decrement) {
                calendar_->removeDay(startDate);
            }
            else {
                calendar_->addDay(startDate);
            }
        }
    }

    // **Adds remaining minutes to a date within workday limits**
    void WorkdayCalendar::addRemainingMinutes(int minutes, Date& current) {
        // Convert workday start and stop times to minutes for easier comparison
        int stop_minutes = TimeUtils::convertToMinutes(workday_stop_->getTime());
        int start_minutes = TimeUtils::convertToMinutes(workday_start_->getTime());
        int current_minutes = TimeUtils::convertToMinutes(current.getTime());

        // Check if current time is past workday stop time
        // If so, reset to next workday start and update current_minutes
        if (current_minutes >= stop_minutes) {
            incrementWorkDay(current);
            current.setDate(current.getYear(), current.getMonth(), current.getDay(),
                workday_start_->getHours(), workday_start_->getMinutes());
            current_minutes = TimeUtils::convertToMinutes(current.getTime());
        }
        // Check if current time is before workday start time
        // If so, reset to workday start and update current_minutes
        else if (current_minutes < start_minutes) {
            current.setDate(current.getYear(), current.getMonth(), current.getDay(),
                workday_start_->getHours(), workday_start_->getMinutes());
            current_minutes = TimeUtils::convertToMinutes(current.getTime());
        }

        // If adding minutes keeps the time within workday limits, add them directly
        if ((current_minutes + minutes) <= stop_minutes) {
            auto [hours, mins] = TimeUtils::addMinutes(current_minutes, minutes);
            current.setDate(current.getYear(), current.getMonth(), current.getDay(), hours, mins);
        }
        else {
            // If adding minutes goes past workday stop, handle overflow
            incrementWorkDay(current);
            int remaining_minutes = (current_minutes + minutes) - stop_minutes;
            auto [hours, mins] = TimeUtils::addMinutes(start_minutes, remaining_minutes);
            current.setDate(current.getYear(), current.getMonth(), current.getDay(), hours, mins);
        }
    }

    // **Function to remove remaining minutes within workday limits**
    void WorkdayCalendar::removeRemainingMinutes(int minutes, Date& current) {
        // Convert workday start and stop times to minutes for easier comparison
        int stop_minutes = TimeUtils::convertToMinutes(workday_stop_->getTime());
        int start_minutes = TimeUtils::convertToMinutes(workday_start_->getTime());
        int current_minutes = TimeUtils::convertToMinutes(current.getTime());

        // Check if current time is past workday stop time
        // If so, reset to workday stop and update current_minutes
        if (current_minutes >= stop_minutes) {
            current.setDate(current.getYear(), current.getMonth(), current.getDay(),
                workday_stop_->getHours(), workday_stop_->getMinutes());
            current_minutes = TimeUtils::convertToMinutes(current.getTime());
        }
        // Check if current time is before workday start time
        // If so, decrement to previous workday stop and update current_minutes
        else if (current_minutes < start_minutes) {
            incrementWorkDay(current, true);
            current.setDate(current.getYear(), current.getMonth(), current.getDay(),
                workday_stop_->getHours(), workday_stop_->getMinutes());
            current_minutes = TimeUtils::convertToMinutes(current.getTime());
        }

        // If subtracting minutes keeps the time within workday limits, subtract them directly
        if ((current_minutes - minutes) >= start_minutes) {
            auto [hours, mins] = TimeUtils::subtractMinutes(current_minutes, minutes);
            current.setDate(current.getYear(), current.getMonth(), current.getDay(), hours, mins);
        }
        else {
            // If subtracting minutes goes before workday start, handle underflow
            incrementWorkDay(current, true);
            int remaining_minutes = start_minutes - (current_minutes - minutes);
            auto [hours, mins] = TimeUtils::subtractMinutes(stop_minutes, remaining_minutes);
            current.setDate(current.getYear(), current.getMonth(), current.getDay(), hours, mins);
        }
    }

    // **Function to calculate a date after incrementing by workdays**
    Date WorkdayCalendar::getWorkdayIncrement(const Date& startDate, float incrementInWorkdays) {

        try {
            //check the incoming date is valid
            if (!calendar_->isValidDate(startDate)) {
                // return invalid date
                Logger::getInstance().logInfo("Invalid startdate", LOG_LOCATION);
                return startDate.generateInvalidDate();
            }

            //check workday start,stop and duration  are valid
            if (!workday_start_ || !workday_stop_ || !workday_duration_ || !calendar_){
                Logger::getInstance().logInfo("Invalid workday param", LOG_LOCATION);
                // return invalid date
                return startDate.generateInvalidDate();
            }

            //check calendar valid
            if (!calendar_) {
                Logger::getInstance().logInfo("Invalid calendar", LOG_LOCATION);
                // return invalid date
                return startDate.generateInvalidDate();
            }

            bool decrement = incrementInWorkdays < 0;
            // Check if increment is negative, handle decrement case separately
            if (decrement) {
                // Convert to positive value for calculation
                incrementInWorkdays = -incrementInWorkdays;
            }

            // Initialize variables
            Date current = startDate;
            long workdayInMinutes = TimeUtils::convertToMinutes(workday_duration_->getTime());
            long workDay_IncrementInMinutes = static_cast<long>(incrementInWorkdays * workdayInMinutes);

            // Calculate number of workdays and workweeks from the total increment
            int workDays = workDay_IncrementInMinutes / workdayInMinutes;
            int workWeeks = workDays / WORKWEEK_DURATION;

            //move to first workday
            // Skips holidays until a non-holiday workday is found
            while (calendar_->isHoliday(current)) {
                if (decrement) {
                    calendar_->removeDay(current);
                    current.setDate(current.getYear(), current.getMonth(), current.getDay(),
                        workday_stop_->getHours(), workday_stop_->getMinutes());
                }
                else {
                    calendar_->addDay(current);
                    current.setDate(current.getYear(), current.getMonth(), current.getDay(),
                        workday_start_->getHours(), workday_start_->getMinutes());
                }
            }

            // Iterate through workweeks, incrementing by workweeks at a time
            while (workWeeks-- > 0) {
                incrementWorkWeek(current, decrement);
            }

            // Calculate remaining workdays after processing workweeks
            int remainingWorkDays = workDays % WORKWEEK_DURATION;

            // Iterate through remaining workdays, incrementing by a day at a time
            while (remainingWorkDays-- > 0) {
                incrementWorkDay(current, decrement);
            }

            // Calculate remaining minutes after processing whole workdays
            int remaining_minutes = workDay_IncrementInMinutes % workdayInMinutes;
            // Handle remaining minutes based on increment direction (add or remove)
            if (decrement) {
                removeRemainingMinutes(remaining_minutes, current);
            }
            else {
                addRemainingMinutes(remaining_minutes, current);
            }
            return current;
        }
        catch (std::exception e) {
            Logger::getInstance().logError(e.what(), LOG_LOCATION);
            return startDate.generateInvalidDate();
        }
    }

    // **Function to update the workday duration**
    void WorkdayCalendar::updateWorkingDuration() {
        // Calculate the difference between workday stop and start time
        auto [hours, mins] = TimeUtils::subtractTime(workday_stop_->getTime(), workday_start_->getTime());
        // Create a new Date object to store the workday duration (0 year, month, day)
        workday_duration_ = std::make_unique<Date>(0, 0, 0, hours, mins);
    }

} // namespace Workday
