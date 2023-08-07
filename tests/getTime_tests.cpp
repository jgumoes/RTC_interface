#define USE_DS3231
#define USE_BCD_TIME

#include <gtest/gtest.h>
#include "./mocks/MockWire.h"
#include "./mocks/MockConfigManager.h"

#include "RTC_interface.h"

#include "helpers.h"

#define GET_LOCAL_TIMESTAMP_TEST(testName, expectedLocalTimestamp, seconds, minutes, hours, dayOfWeek, date, month, years, timezone, DST)TEST(getLocalTimestamp_gets_the_correct_timestamp, testName){ \
  MockWire wire = MockWire(seconds, minutes, hours, dayOfWeek, date, month, years); \
  wire.convertToBcd(); \
  MockConfigManager config = MockConfigManager(timezone, DST); \
  RTCInterfaceClass<MockWire, MockConfigManager> RTC = RTCInterfaceClass(wire, config); \
  RTC.begin(); \
  EXPECT_EQ(RTC.getLocalTimestamp(), expectedLocalTimestamp); \
}

#define GET_BCD_DATETIME_TEST(test_name_var, seconds, minutes, hours, dayOfWeekVal, date, month, year, timezone, DST) TEST(getBCDX_gets_the_correct_time_and_date, test_name_var){ \
  MockWire wire = MockWire(AS_BCD(seconds), AS_BCD(minutes), AS_BCD(hours), AS_BCD(dayOfWeekVal), AS_BCD(date), AS_BCD(month), AS_BCD(year)); \
  MockConfigManager config = MockConfigManager(timezone, DST); \
  RTCInterfaceClass<MockWire, MockConfigManager> RTC = RTCInterfaceClass(wire, config); \
  RTC.begin(); \
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

#define GET_UTC_TIMESTAMP_TEST(testName, expectedLocalTimestamp, seconds, minutes, hours, dayOfWeek, date, month, years, timezone, DST)TEST(getUTCTimestamp_gets_the_correct_timestamp, testName){ \
  MockWire wire = MockWire(seconds, minutes, hours, dayOfWeek, date, month, years); \
  wire.convertToBcd(); \
  MockConfigManager config = MockConfigManager(timezone, DST); \
  RTCInterfaceClass<MockWire, MockConfigManager> RTC = RTCInterfaceClass(wire, config); \
  RTC.begin(); \
  EXPECT_EQ(RTC.getUTCTimestamp(), expectedLocalTimestamp - DST - timezone); \
  EXPECT_EQ(RTC.getDSTOffset(), DST); \
  EXPECT_EQ(RTC.getTimezoneOffset(), timezone); \
}

#define QUICKTEST(testName, expectedLocalTimestamp, seconds, minutes, hours, dayOfWeek, date, month, years, timezone, DST)\
 GET_LOCAL_TIMESTAMP_TEST(testName, expectedLocalTimestamp, seconds, minutes, hours, dayOfWeek, date, month, years, timezone, DST);\
 GET_BCD_DATETIME_TEST(testName, seconds, minutes, hours, dayOfWeek, date, month, years, timezone, DST); \
 GET_UTC_TIMESTAMP_TEST(testName, expectedLocalTimestamp, seconds, minutes, hours, dayOfWeek, date, month, years, timezone, DST);


TEST(define_BCDMask, is_correct){
  EXPECT_EQ(BCDMask, 0b00001111);
}

QUICKTEST(at_the_beginning_of_time, 0, 0, 0, 0, 6, 1, 1, 0, 0, 0)

QUICKTEST(during_british_winter_time_2023, 729082742, 2, 59, 10, 2, 7, 2, 23, 0, 0)

QUICKTEST(during_british_summer_time_2023, 743102001, 21, 13, 17, 3, 19, 7, 23, 0, 60*60)

QUICKTEST(feb_28_2023, 730925880, 0, 58, 18, 2, 28, 2, 23, 0, 0)

QUICKTEST(mar_1_2023, 731007913, 13, 45, 17, 3, 1, 3, 23, 0, 0)

QUICKTEST(feb_28_2024, 762461880, 0, 58, 18, 3, 28, 2, 24, 0, 0)

QUICKTEST(feb_29_2024, 762548280, 0, 58, 18, 4, 29, 2, 24, 0, 0)

QUICKTEST(mar_1_2024, 762630313, 13, 45, 17, 5, 1, 3, 24, 0, 0)

QUICKTEST(feb_6_2005, 161049599, 59, 59, 23, 7, 6, 2, 5, 0, 0)

QUICKTEST(june_30_2023_1hr_dst, 804600317, 17, 5, 12, 1, 30, 6, 25, 0, 60*60)

QUICKTEST(july_31_25_1hr_dst_minus_8hr_timezone, 807244446, 6, 34, 2, 4, 31, 7, 25, -8*60*60, 60*60)

QUICKTEST(nov_30_26_plus_3hrs_timezone, 849321246, 6, 34, 2, 1, 30, 11, 26, 3*60*60, 0)
