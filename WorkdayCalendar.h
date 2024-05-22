/**
 * @file WorkdayCalendar.h
 * @brief Header file for the WorkdayCalendar class which manages workday calculations
 * considering holidays and work hours.
 *
 * This class provides functionality to set workday start and stop times, designate holidays,
 * calculate workday increments, and manage the overall workday calendar. It ensures that
 * operations are thread-safe using a mutex.
 *
 * @author Binu Melit Devassy
 * @date 2024-05-20
 *
 * @license MIT License
 */

#ifndef WORKDAY_CALENDAR_H
#define WORKDAY_CALENDAR_H

#include "Calendar.h"
#include "Date.h"
#include <mutex>

namespace Workday{

    const int WORKWEEK_DURATION = 5;

    /**
     * @class WorkdayCalendar
     * @brief A class to manage workday calculations considering holidays and work hours.
     */
    class WorkdayCalendar {
    public:
        /**
         * @brief Constructor for WorkdayCalendar.
         */
        WorkdayCalendar();

        /**
         * @brief Sets the start and stop times for the working day.
         * @param start The start time of the working day.
         * @param stop The stop time of the working day.
         */
        void setWorkdayStartAndStop(const Date& start, const Date& stop);

        /**
         * @brief Sets a specific date as a holiday.
         * @param date The date to be set as a holiday.
         */
        void setHoliday(const Date& date);

        /**
         * @brief Sets a recurring holiday on the same date every year.
         * @param date The date to be set as a recurring holiday.
         */
        void setRecurringHoliday(const Date& date);

        /**
         * @brief Calculates the date after incrementing the specified number of workdays.
         * @param startDate The start date from which to calculate.
         * @param incrementInWorkdays The number of workdays to increment (can be negative for decrement).
         * @return The calculated date after the increment or invalid date if anything goes wrong.
         */
        Date getWorkdayIncrement(const Date& startDate, float incrementInWorkdays);

        /**
         * @brief Returns the workday start
         */
        Date* getWorkdayStart() {
            return workday_start_.get();
        }

        /**
         * @brief Returns the workday end
         */
        Date* getWorkdayStop() {
            return workday_stop_.get();
        }

        /**
         * @brief Returns true if it is a holiday
         */
        bool isHoliday(Date date_i) {
            return calendar_->isHoliday(date_i);
        }

    private:
        /**
         * @brief Increments or decrements the given date by a workweek.
         * @param startDate The date to be incremented or decremented.
         * @param decrement Indicates if the date should be decremented.
         */
        void incrementWorkWeek(Date& startDate, bool decrement = false);

        /**
         * @brief Increments or decrements the given date by a workday.
         * @param startDate The date to be incremented or decremented.
         * @param decrement Indicates if the date should be decremented.
         */
        void incrementWorkDay(Date& startDate, bool decrement = false);

        /**
         * @brief Adds the remaining minutes to the given date.
         * @param minutes The number of minutes to add.
         * @param current The current date to which minutes are added.
         */
        void addRemainingMinutes(int minutes, Date& current);

        /**
         * @brief Removes the remaining minutes from the given date.
         * @param minutes The number of minutes to remove.
         * @param current The current date from which minutes are removed.
         */
        void removeRemainingMinutes(int minutes, Date& current);

        /**
         * @brief Updates the duration of the working day based on start and stop times.
         */
        void updateWorkingDuration();

    private:
        //variables holding workday start,stop & duration
        std::unique_ptr<Date> workday_start_;
        std::unique_ptr<Date> workday_stop_;
        std::unique_ptr<Date> workday_duration_;
        std::unique_ptr<Calendar> calendar_;
        std::mutex mtx_;  ///< Mutex for thread safety
    };

} // namespace Workday

#endif // WORKDAY_CALENDAR_H
