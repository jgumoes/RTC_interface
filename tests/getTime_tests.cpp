#include <gtest/gtest.h>
#include "./mocks/MockWire.h"

#define USE_DS3231
#define USE_BCD_TIME
#include "RTC_interface.h"

#include "helpers.h"

#define GET_LOCAL_TIMESTAMP_TEST(testName, expectedResult, seconds, minutes, hours, dayOfWeek, date, month, years)TEST(getLocalTimestamp_gets_the_correct_timestamp, testName){ \
  MockWire wire = MockWire(seconds, minutes, hours, dayOfWeek, date, month, years); \
  wire.convertToBcd(); \
  PRINT_ARRAY("wire mock mockBuffer", wire.mockBuffer, 7); \
  RTCInterfaceClass<MockWire> RTC = RTCInterfaceClass(wire); \
  EXPECT_EQ(RTC.getLocalTimestamp(), expectedResult); \
}

#define GET_BCD_DATETIME_TEST(test_name_var, seconds, minutes, hours, dayOfWeekVal, date, month, year) TEST(getBCDX_gets_the_correct_time_and_date, test_name_var){ \
  MockWire wire = MockWire(AS_BCD(seconds), AS_BCD(minutes), AS_BCD(hours), AS_BCD(dayOfWeekVal), AS_BCD(date), AS_BCD(month), AS_BCD(year)); \
  RTCInterfaceClass<MockWire> RTC = RTCInterfaceClass(wire); \
  BCDTimeStruct bcdTime = RTC.getBCDTime(); \
\
  EXPECT_EQ(bcdTime.seconds_1, ONES(seconds)); \
  EXPECT_EQ(bcdTime.seconds_10, TENS(seconds)); \
  EXPECT_EQ(bcdTime.minutes_1, ONES(minutes)); \
  EXPECT_EQ(bcdTime.minutes_10, TENS(minutes)); \
  EXPECT_EQ(bcdTime.hours_1, ONES(hours)); \
  EXPECT_EQ(bcdTime.hours_10, TENS(hours)); \
 \
  BCDDateStruct bcdDate = RTC.getBCDDate(); \
  EXPECT_EQ(bcdDate.dayOfWeek, dayOfWeekVal); \
  EXPECT_EQ(bcdDate.date_1, ONES(date)); \
  EXPECT_EQ(bcdDate.date_10, TENS(date)); \
  EXPECT_EQ(bcdDate.month_1, ONES(month)); \
  EXPECT_EQ(bcdDate.month_10, TENS(month)); \
  EXPECT_EQ(bcdDate.year_1, ONES(year)); \
  EXPECT_EQ(bcdDate.year_10, TENS(year)); \
}

#define QUICKTEST(testName, expectedLocalTimestamp, seconds, minutes, hours, dayOfWeek, date, month, years)\
 GET_LOCAL_TIMESTAMP_TEST(testName, expectedLocalTimestamp, seconds, minutes, hours, dayOfWeek, date, month, years);\
 GET_BCD_DATETIME_TEST(testName, seconds, minutes, hours, dayOfWeek, date, month, years);


TEST(define_BCDMask, is_correct){
  EXPECT_EQ(BCDMask, 0b00001111);
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