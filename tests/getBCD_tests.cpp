#include <gtest/gtest.h>
#include "./mocks/MockWire.h"

#include <stdio.h>

#define USE_DS3231
#define USE_BCD_TIME
#include "RTC_interface.h"

#include "helpers.h"

#define QUICKTEST(test_name_var, seconds, minutes, hours, dayOfWeekVal, date, month, year) TEST(getBCDX_gets_the_correct_time_and_date, test_name_var){ \
  MockWire wire = MockWire(AS_BCD(seconds), AS_BCD(minutes), AS_BCD(hours), AS_BCD(dayOfWeekVal), AS_BCD(date), AS_BCD(month), AS_BCD(year)); \
  RTCInterfaceClass<MockWire> RTC = RTCInterfaceClass(wire); \
  BCDTimeStruct bcdTime = RTC.getBCDTime(); \
  PRINT_AS_INT("seconds_1", bcdTime.seconds_1)\
  PRINT_AS_INT("seconds_10", bcdTime.seconds_10)\
  PRINT_AS_INT("minutes_1", bcdTime.minutes_1)\
  PRINT_AS_INT("minutes_10", bcdTime.minutes_10)\
  PRINT_AS_INT("hours_1", bcdTime.hours_1)\
  PRINT_AS_INT("hours_10", bcdTime.hours_10)\
  PRINT_ARRAY("wire mock mockBuffer", wire.mockBuffer, 7)\
\
  EXPECT_EQ(bcdTime.seconds_1, ONES(seconds)); \
  EXPECT_EQ(bcdTime.seconds_10, TENS(seconds)); \
  EXPECT_EQ(bcdTime.minutes_1, ONES(minutes)); \
  EXPECT_EQ(bcdTime.minutes_10, TENS(minutes)); \
  EXPECT_EQ(bcdTime.hours_1, ONES(hours)); \
  EXPECT_EQ(bcdTime.hours_10, TENS(hours)); \
 \
  BCDDateStruct bcdDate = RTC.getBCDDate(); \
  PRINT_AS_INT("dayOfWeek", bcdDate.dayOfWeek)\
  PRINT_AS_INT("date_1", bcdDate.date_1)\
  PRINT_AS_INT("date_10", bcdDate.date_10)\
  PRINT_AS_INT("month_1", bcdDate.month_1)\
  PRINT_AS_INT("month_10", bcdDate.month_10)\
  PRINT_AS_INT("year_1", bcdDate.year_1)\
  PRINT_AS_INT("year_10", bcdDate.year_10)\
  EXPECT_EQ(bcdDate.dayOfWeek, dayOfWeekVal); \
  EXPECT_EQ(bcdDate.date_1, ONES(date)); \
  EXPECT_EQ(bcdDate.date_10, TENS(date)); \
  EXPECT_EQ(bcdDate.month_1, ONES(month)); \
  EXPECT_EQ(bcdDate.month_10, TENS(month)); \
  EXPECT_EQ(bcdDate.year_1, ONES(year)); \
  EXPECT_EQ(bcdDate.year_10, TENS(year)); \
}


TEST(define_BCDMask, is_correct){
  EXPECT_EQ(BCDMask, 0b00001111);
}

QUICKTEST(at_the_beginning_of_time, 0, 0, 0, 6, 1, 1, 0)

QUICKTEST(during_british_winter_time_2023, 2, 59, 10, 2, 7, 2, 23)

QUICKTEST(during_british_summer_time_2023, 21, 13, 17, 3, 19, 7, 23)

QUICKTEST(feb_28_2023, 0, 58, 18, 2, 28, 2, 23)

QUICKTEST(mar_1_2023, 13, 45, 17, 3, 1, 3, 23)

QUICKTEST(feb_28_2024, 0, 58, 18, 3, 28, 2, 24)

QUICKTEST(feb_29_2024, 0, 58, 18, 4, 29, 2, 24)

QUICKTEST(mar_1_2024, 13, 45, 17, 5, 1, 3, 24)