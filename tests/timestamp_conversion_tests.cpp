#include <gtest/gtest.h>
#include "RTC_interface.h"

// TEST(convert_To_Local_Timestamp_Test, during_british_winter_time_2023){
//   DateTimeStruct datetime;
//   datetime.seconds = 2;
//   datetime.minutes = 59;
//   datetime.hours = 10;
//   datetime.dayOfWeek = 2;
//   datetime.date = 7;
//   datetime.month = 2;
//   datetime.years = 23;
//   EXPECT_EQ(convertToLocalTimestamp(&datetime), 729082742);
// }

TEST(convert_To_Local_Timestamp_Test, during_british_summer_time_2023){
  DateTimeStruct datetime;
  datetime.seconds = 21;
  datetime.minutes = 13;
  datetime.hours = 17;
  datetime.dayOfWeek = 3;
  datetime.date = 19;
  datetime.month = 7;
  datetime.years = 23;
  EXPECT_EQ(convertToLocalTimestamp(&datetime), 743102001);
}

TEST(convert_To_Local_Timestamp_month_test, all_of_january_24){
  uint32_t timestamp = 757453508;
  const uint32_t secondsInDay = 60*60*24;
  DateTimeStruct datetime;
  datetime.seconds = 8;
  datetime.minutes = 45;
  datetime.hours = 19;
  // datetime.dayOfWeek = ;
  datetime.date = 1;
  datetime.month = 1;
  datetime.years = 24;

  for(uint8_t date = 1; date <= monthDays[3]; date++){
    datetime.date = date;
    ASSERT_EQ(convertToLocalTimestamp(&datetime), timestamp);
    timestamp += secondsInDay;
  }
}

TEST(convert_To_Local_Timestamp_month_test, all_of_febuary_24){
  uint32_t timestamp = 760131908;
  const uint32_t secondsInDay = 60*60*24;
  DateTimeStruct datetime;
  datetime.seconds = 8;
  datetime.minutes = 45;
  datetime.hours = 19;
  // datetime.dayOfWeek = ;
  datetime.date = 1;
  datetime.month = 2;
  datetime.years = 24;

  for(uint8_t date = 1; date <= monthDays[3]; date++){
    datetime.date = date;
    ASSERT_EQ(convertToLocalTimestamp(&datetime), timestamp);
    timestamp += secondsInDay;
  }
}

TEST(convert_To_Local_Timestamp_month_test, all_of_march_24){
  uint32_t timestamp = 762637508;
  const uint32_t secondsInDay = 60*60*24;
  DateTimeStruct datetime;
  datetime.seconds = 8;
  datetime.minutes = 45;
  datetime.hours = 19;
  // datetime.dayOfWeek = ;
  datetime.date = 1;
  datetime.month = 3;
  datetime.years = 24;

  for(uint8_t date = 1; date <= monthDays[3]; date++){
    datetime.date = date;
    ASSERT_EQ(convertToLocalTimestamp(&datetime), timestamp);
    timestamp += secondsInDay;
  }
}

TEST(convert_To_Local_Timestamp_month_test, all_of_april_24){
  uint32_t timestamp = 765315908;
  const uint32_t secondsInDay = 60*60*24;
  DateTimeStruct datetime;
  datetime.seconds = 8;
  datetime.minutes = 45;
  datetime.hours = 19;
  // datetime.dayOfWeek = ;
  datetime.date = 1;
  datetime.month = 4;
  datetime.years = 24;

  for(uint8_t date = 1; date <= monthDays[4]; date++){
    datetime.date = date;
    ASSERT_EQ(convertToLocalTimestamp(&datetime), timestamp);
    timestamp += secondsInDay;
  }
}

TEST(convert_To_Local_Timestamp_month_test, all_of_may_24){
  uint32_t timestamp = 767907908;
  const uint32_t secondsInDay = 60*60*24;
  DateTimeStruct datetime;
  datetime.seconds = 8;
  datetime.minutes = 45;
  datetime.hours = 19;
  // datetime.dayOfWeek = ;
  datetime.date = 1;
  datetime.month = 5;
  datetime.years = 24;

  for(uint8_t date = 1; date <= monthDays[5]; date++){
    datetime.date = date;
    ASSERT_EQ(convertToLocalTimestamp(&datetime), timestamp);
    timestamp += secondsInDay;
  }
}

TEST(convert_To_Local_Timestamp_month_test, all_of_may_20){
  uint32_t timestamp = 641681362;
  const uint32_t secondsInDay = 60*60*24;
  DateTimeStruct datetime;
  datetime.seconds = 22;
  datetime.minutes = 49;
  datetime.hours = 20;
  // datetime.dayOfWeek = ;
  datetime.date = 1;
  datetime.month = 5;
  datetime.years = 20;

  for(uint8_t date = 1; date <= monthDays[5]; date++){
    datetime.date = date;
    ASSERT_EQ(convertToLocalTimestamp(&datetime), timestamp);
    timestamp += secondsInDay;
  }
}

TEST(convert_To_Local_Timestamp_month_test, all_of_may_21){
  uint32_t timestamp = 673217362;
  const uint32_t secondsInDay = 60*60*24;
  DateTimeStruct datetime;
  datetime.seconds = 22;
  datetime.minutes = 49;
  datetime.hours = 20;
  // datetime.dayOfWeek = ;
  datetime.date = 1;
  datetime.month = 5;
  datetime.years = 21;

  for(uint8_t date = 1; date <= monthDays[5]; date++){
    datetime.date = date;
    ASSERT_EQ(convertToLocalTimestamp(&datetime), timestamp);
    timestamp += secondsInDay;
  }
}

uint32_t convertToLocalTimestamp_helper(DateTimeStruct datetime){
  DateTimeStruct datetimeClone = datetime;
  return convertToLocalTimestamp(&datetime);
}

#define QUICKTEST(testName, expectedLocalTimestamp, t_seconds, t_minutes, t_hours, t_dayOfWeek, t_date, t_month, t_years, t_timezone, t_DST)\
TEST(convert_To_Local_Timestamp_returns_correct_time , testName ){\
  DateTimeStruct datetime;\
  datetime.seconds = t_seconds;\
  datetime.minutes = t_minutes;\
  datetime.hours = t_hours;\
  datetime.dayOfWeek = t_dayOfWeek;\
  datetime.date = t_date;\
  datetime.month = t_month;\
  datetime.years = t_years;\
  EXPECT_EQ(convertToLocalTimestamp_helper(datetime), expectedLocalTimestamp);\
}
QUICKTEST(at_the_beginning_of_time, 0, 0, 0, 0, 6, 1, 1, 0, 0, 0)

QUICKTEST(during_british_winter_time_2023, 729082742, 2, 59, 10, 2, 7, 2, 23, 0, 0)

QUICKTEST(during_british_summer_time_2023, 743102001, 21, 13, 17, 3, 19, 7, 23, 0, 60*60)

QUICKTEST(feb_28_2023, 730925880, 0, 58, 18, 2, 28, 2, 23, 0, 0)

QUICKTEST(mar_1_2023, 731007913, 13, 45, 17, 3, 1, 3, 23, 0, 0)

QUICKTEST(feb_28_2024, 762461880, 0, 58, 18, 3, 28, 2, 24, 0, 0)

QUICKTEST(feb_29_2024, 762548280, 0, 58, 18, 4, 29, 2, 24, 0, 0)

QUICKTEST(mar_1_2024, 762630313, 13, 45, 17, 5, 1, 3, 24, 0, 0)
QUICKTEST(mar_2_2024, 762716713, 13, 45, 17, 6, 2, 3, 24, 0, 0)
QUICKTEST(mar_3_2024, 762803113, 13, 45, 17, 7, 3, 3, 24, 0, 0)
QUICKTEST(mar_4_2024, 762889513, 13, 45, 17, 1, 4, 3, 24, 0, 0)
QUICKTEST(mar_5_2024, 762975913, 13, 45, 17, 2, 5, 3, 24, 0, 0)
QUICKTEST(mar_6_2024, 763062313, 13, 45, 17, 3, 6, 3, 24, 0, 0)
QUICKTEST(mar_7_2024, 763148713, 13, 45, 17, 4, 7, 3, 24, 0, 0)

QUICKTEST(april_6_24, 765740713, 13, 45, 17, 6, 6, 4, 24, 0, 60*60)
QUICKTEST(may_10_2024, 768678313, 13, 45, 17, 5, 10, 5, 24, 0, 60*60)
QUICKTEST(june_10_2024, 771356713, 13, 45, 17, 1, 10, 6, 24, 0, 60*60)
QUICKTEST(july_10_2024, 773948713, 13, 45, 17, 3, 10, 7, 24, 0, 60*60)
QUICKTEST(aug_10_2024, 776627113, 13, 45, 17, 6, 10, 8, 24, 0, 60*60)
QUICKTEST(sep_10_2024, 779305513, 13, 45, 17, 2, 10, 9, 24, 0, 60*60)
QUICKTEST(oct_10_2024, 781897513, 13, 45, 17, 4, 10, 10, 24, 0, 0)
QUICKTEST(nov_10_2024, 784575913, 13, 45, 17, 7, 10, 11, 24, 0, 0)
QUICKTEST(dec_10_2024, 787167913, 13, 45, 17, 2, 10, 12, 24, 0, 0)

QUICKTEST(feb_6_2005, 161049598, 58, 59, 23, 7, 6, 2, 5, 0, 0)

QUICKTEST(june_30_2023_1hr_dst, 804600317, 17, 5, 12, 1, 30, 6, 25, 0, 60*60)

QUICKTEST(july_31_25_1hr_dst_minus_8hr_timezone, 807244446, 6, 34, 2, 4, 31, 7, 25, -8*60*60, 60*60)

QUICKTEST(nov_30_26_plus_3hrs_timezone, 849321246, 6, 34, 2, 1, 30, 11, 26, 3*60*60, 0)


QUICKTEST(last_second_of_22, 725846399, 59, 59, 23, 6, 31, 12, 22, 0, 0)
QUICKTEST(jan_1_23_midnight, 725846400, 0, 0, 0, 7, 1, 1, 23, 0, 0)

// QUICKTEST(mar_7_2024, 763148713, 13, 45, 17, 4, 7, 3, 24, 0, 0)
QUICKTEST(april_1_24, 765308713, 13, 45, 17, 1, 1, 4, 24, 0, 60*60)