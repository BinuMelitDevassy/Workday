#define GTEST_USE_OWN_TR1_TUPLE 1
#include <gtest/gtest.h>
#include "WorkdayCalendar.h"
#include <thread>

using namespace Workday;

// Fixture for WorkdayCalendar tests
class WorkdayCalendarTest : public ::testing::Test {
protected:
    // Set up the test fixture
    void SetUp() override {
        // Initialize a workday calendar
        workday_calendar = new WorkdayCalendar();
    }

    // Tear down the test fixture
    void TearDown() override {
        // Clean up resources
        delete workday_calendar;
    }

    // Pointer to the WorkdayCalendar object
    WorkdayCalendar* workday_calendar;
};

// Test case for setting workday start and stop times - error cases
TEST_F(WorkdayCalendarTest, SetWorkdayStartAndStop_Error_Cases) {
    // Define start and stop times
    Date start_time(2024, -5, 20, 8, 0); // invalid date
    Date stop_time(-2024, 5, 20, 17, 0); // in valid date

    // Set workday start and stop times
    workday_calendar->setWorkdayStartAndStop(start_time, stop_time);

    // Check if the start and stop times are set to nullptr
    EXPECT_EQ(workday_calendar->getWorkdayStart(), nullptr);
    EXPECT_EQ(workday_calendar->getWorkdayStop(), nullptr);
}

// Test case for setting workday start and stop times
TEST_F(WorkdayCalendarTest, SetWorkdayStartAndStop) {
    // Define start and stop times
    Date start_time(2024, 5, 20, 8, 0); // May 20, 2024, 8:00 AM
    Date stop_time(2024, 5, 20, 17, 0); // May 20, 2024, 5:00 PM

    // Set workday start and stop times
    workday_calendar->setWorkdayStartAndStop(start_time, stop_time);

    // Check if the start and stop times are correctly set
    EXPECT_EQ(workday_calendar->getWorkdayStart()->getDateAndTime(), start_time.getDateAndTime());
    EXPECT_EQ(workday_calendar->getWorkdayStop()->getDateAndTime(), stop_time.getDateAndTime());
}


// Test case for setting a recurring holiday - error cases
TEST_F(WorkdayCalendarTest, Set_Holiday_RecuringHoliday_Error) {
    // Define a working day
    Date any_date(2024, 5, 21, 0, 0); // May 27, 2024

    // Check if the day is a working day
    EXPECT_FALSE(workday_calendar->isHoliday(any_date));

    // Check if the day is a working day
    EXPECT_FALSE(workday_calendar->isHoliday(any_date));
}

// Test case for setting a holiday and recuring holidaye
TEST_F(WorkdayCalendarTest, Set_Holiday_RecuringHoliday) {
    // Define a holiday date
    Date holiday_date(2024, 5, 27, 0, 0); // May 27, 2024

    // Define a recurring holiday date
    Date recurring_holiday_date(2024, 12, 25, 0, 0); // December 25, 2024

    // Set the recurring holiday
    workday_calendar->setRecurringHoliday(recurring_holiday_date);

    // Set the holiday
    workday_calendar->setHoliday(holiday_date);

    // Check if the holiday is correctly set
    EXPECT_TRUE(workday_calendar->isHoliday(holiday_date));

    // Check if the recurring holiday is correctly set
    EXPECT_TRUE(workday_calendar->isHoliday(recurring_holiday_date));
}

// Test case for calculating workday increments error case
TEST_F(WorkdayCalendarTest, CalculateWorkdayIncrement_error) {

    //Calling getWorkdayIncrement without setting satrt and stop of workday
    // Define start date and increment
    Date start_date(2024, 5, 20, 8, 0); // May 20, 2024, 8:00 AM
    float increment = 3.5; // Increment by 3.5 workdays

    // Calculate the incremented date
    Date incremented_date = workday_calendar->getWorkdayIncrement(start_date, increment);

    // Check if the calculated incremented date matches the invalid date
    EXPECT_EQ(incremented_date.getDateAndTime(), incremented_date.generateInvalidDate().getDateAndTime());
}

struct CalculateWorkdayIncrement {
    Date workdayStart;
    Date workdayStop;
    Date holiday;
    Date recuringHoliday;
    Date startDate;
    Date expectedReturnDate; // expected return date after increment
    float increment;
    // boolean to check corresponding functions need call
    bool setHolday;
    bool setRecHolday;

    friend std::ostream& operator<<(std::ostream& os, const CalculateWorkdayIncrement& obj) {
        return os
            << "start date : " << obj.startDate.getDateAndTime()
            << "increment : " << obj.increment;
    }

};

// Name generator function.
std::string TestNameGenerator(const testing::TestParamInfo<CalculateWorkdayIncrement>& info) {
    std::stringstream ss;
    ss << "TestCase_" << info.index;
    return ss.str();
}

struct CalculateWorkdayIncrementTest : WorkdayCalendarTest, testing::WithParamInterface<CalculateWorkdayIncrement> {
    CalculateWorkdayIncrementTest() {
    }
};

TEST_P(CalculateWorkdayIncrementTest, WorkdayIncrementTest) {
    auto as = GetParam();
    workday_calendar->setWorkdayStartAndStop(as.workdayStart, as.workdayStop);
    if (as.setHolday) {
        workday_calendar->setHoliday(as.holiday);
    }

    if (as.setRecHolday) {
        workday_calendar->setRecurringHoliday(as.recuringHoliday);
    }

    Date returnDate = workday_calendar->getWorkdayIncrement(as.startDate, as.increment);
    EXPECT_EQ(returnDate.getDateAndTime(), as.expectedReturnDate.getDateAndTime());
}

//Use below dates as start and end of work day
Date startWorkday = Date(2004, 1, 1, 8, 0);
Date stopWorkday = Date(2004, 1, 1, 16, 0);
Date invalid = stopWorkday.generateInvalidDate();
INSTANTIATE_TEST_SUITE_P(default, CalculateWorkdayIncrementTest,
    testing::Values( // trying given test cases
        CalculateWorkdayIncrement{ startWorkday,stopWorkday, invalid, invalid, Date(2004, 1, 1, 15, 07), Date(2004, 1, 2, 9, 07),
            0.25, false, false },
        CalculateWorkdayIncrement{ startWorkday,stopWorkday, invalid, invalid, Date(2004, 1, 1, 16, 00), Date(2004, 1, 2, 12, 00),
            0.5, false, false },
        CalculateWorkdayIncrement{ startWorkday,stopWorkday, Date(2004, 5, 27, 0, 0), Date(2004, 5, 17, 0, 0),
            Date(2004, 5, 24, 19, 03), Date(2004, 7, 27, 13, 47),  44.723656, true, true },
        CalculateWorkdayIncrement{ startWorkday,stopWorkday, Date(2004, 5, 27, 0, 0), Date(2004, 5, 17, 0, 0),
            Date(2004, 5, 24, 8, 03), Date(2004, 6, 10, 14, 18),  12.782709, true, true },
        CalculateWorkdayIncrement{ startWorkday,stopWorkday, Date(2004, 5, 27, 0, 0), Date(2004, 5, 17, 0, 0),
            Date(2004, 5, 24, 7, 03), Date(2004, 6, 4, 10, 12),  8.276628, true, true },
        CalculateWorkdayIncrement{ startWorkday,stopWorkday, Date(2004, 5, 27, 0, 0), Date(2004, 5, 17, 0, 0),
            Date(2004, 5, 24, 18, 03), Date(2004, 5, 13, 10, 02),  -6.7470217, true, true },
        CalculateWorkdayIncrement{ startWorkday,stopWorkday, Date(2004, 5, 27, 0, 0), Date(2004, 5, 17, 0, 0),
        Date(2004, 5, 24, 18, 05), Date(2004, 5, 14, 12, 00),  -5.5, true, true },
        //leap year
        CalculateWorkdayIncrement{ startWorkday,stopWorkday, Date(2004, 5, 27, 0, 0), Date(2004, 5, 17, 0, 0),
            Date(2024, 2, 28, 9, 0), Date(2024, 2, 29, 9, 0),  1, true, true },
        CalculateWorkdayIncrement{ startWorkday,stopWorkday, Date(2004, 5, 27, 0, 0), Date(2004, 5, 17, 0, 0),
            Date(2024, 3, 1, 9, 0), Date(2024, 2, 29, 9, 0),  -1, true, true },
        // zero increment
        CalculateWorkdayIncrement{ startWorkday,stopWorkday, Date(2004, 5, 27, 0, 0), Date(2004, 5, 17, 0, 0),
            Date(2024, 3, 1, 9, 0), Date(2024, 3, 1, 9, 0),  0, true, true },
        // Increment starting from a weekend
        CalculateWorkdayIncrement{ startWorkday, stopWorkday, invalid, invalid,
            Date(2024, 5, 11, 9, 0), Date(2024, 5, 14, 8, 0), 1, false, false },
        // Increment that crosses a holiday
        CalculateWorkdayIncrement{ startWorkday, stopWorkday, Date(2024, 7, 4, 0, 0), invalid,
            Date(2024, 7, 3, 9, 0), Date(2024, 7, 5, 9, 0), 1, true, false },
        // Increment that crosses multiple holidays
        CalculateWorkdayIncrement{ startWorkday, stopWorkday, Date(2024, 7, 4, 0, 0), Date(2024, 12, 25, 0, 0),
            Date(2024, 7, 3, 9, 0), Date(2024, 7, 9, 9, 0), 3, true, false },
        // Negative increment starting from a weekend
        CalculateWorkdayIncrement{ startWorkday, stopWorkday, invalid, invalid, 
            Date(2024, 5, 11, 9, 0), Date(2024, 5, 9, 16, 0), -1, false, false },
        // Negative increment that crosses a holiday
        CalculateWorkdayIncrement{ startWorkday, stopWorkday, Date(2024, 7, 4, 0, 0), invalid, 
            Date(2024, 7, 5, 9, 0), Date(2024, 7, 3, 9, 0), -1, true, false },
        // Negative increment that crosses multiple holidays
        CalculateWorkdayIncrement{ startWorkday, stopWorkday, Date(2024, 7, 4, 0, 0), Date(2024, 12, 25, 0, 0), 
            Date(2024, 7, 8, 9, 0), Date(2024, 7, 2, 9, 0), -3, true, false },
        // Increment starting late in the day
        CalculateWorkdayIncrement{ startWorkday, stopWorkday, invalid, invalid, 
            Date(2024, 7, 1, 15, 0), Date(2024, 7, 2, 15, 0), 1, false, false },
        // Increment that starts before workday start time
        CalculateWorkdayIncrement{ startWorkday, stopWorkday, invalid, invalid, 
            Date(2024, 7, 1, 7, 0), Date(2024, 7, 1, 12, 0), 0.5, false, false },
        // Increment that results in exact end of workday
        CalculateWorkdayIncrement{ startWorkday, stopWorkday, invalid, invalid, 
            Date(2024, 7, 1, 8, 0), Date(2024, 7, 2, 8, 0), 1, false, false },
        // Increment on a holiday - start fron begining of next workday
        CalculateWorkdayIncrement{ startWorkday, stopWorkday, Date(2024, 7, 4, 0, 0), invalid, 
            Date(2024, 7, 4, 9, 0), Date(2024, 7, 8, 8, 0), 1, true, false },
        // Increment that spans to the next year
        CalculateWorkdayIncrement{ startWorkday, stopWorkday, invalid, invalid, 
            Date(2024, 12, 30, 9, 0), Date(2025, 1, 2, 9, 0), 3, false, false }
),TestNameGenerator);


// Main function to run tests
int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}


