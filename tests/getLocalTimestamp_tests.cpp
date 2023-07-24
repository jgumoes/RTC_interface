#include <gtest/gtest.h>
#include "./mocks/MockWire.h"

#define USE_DS3231
#include "RTC_interface.h"

#include "helpers.h"

#define QUICKTEST(testName, expectedResult, seconds, minutes, hours, dayOfWeek, date, month, years)TEST(getLocalTimestamp_gets_the_correct_timestamp, testName){ \
  MockWire wire = MockWire(seconds, minutes, hours, dayOfWeek, date, month, years); \
  wire.convertToBcd(); \
  PRINT_ARRAY("wire mock mockBuffer", wire.mockBuffer, 7); \
  RTCInterfaceClass<MockWire> RTC = RTCInterfaceClass(wire); \
  EXPECT_EQ(RTC.getLocalTimestamp(), expectedResult); \
}

QUICKTEST(at_the_beginning_of_time, 0, 0, 0, 0, 6, 1, 1, 0)

QUICKTEST(during_british_winter_time_2023, 729082742, 2, 59, 10, 2, 7, 2, 23)

QUICKTEST(during_british_summer_time_2023, 743102001, 21, 13, 17, 3, 19, 7, 23)

QUICKTEST(feb_28_2023, 730925880, 0, 58, 18, 2, 28, 2, 23)

QUICKTEST(mar_1_2023, 731007913, 13, 45, 17, 3, 1, 3, 23)

QUICKTEST(feb_28_2024, 762461880, 0, 58, 18, 3, 28, 2, 24)

QUICKTEST(feb_29_2024, 762548280, 0, 58, 18, 4, 29, 2, 24)

QUICKTEST(mar_1_2024, 762630313, 13, 45, 17, 5, 1, 3, 24)

QUICKTEST(feb_6_2005, 161049599, 59, 59, 23, 1, 6, 2, 5)