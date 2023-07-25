#define USE_DS3231
#define USE_BCD_TIME

#include <gtest/gtest.h>
#include "./mocks/MockWire.h"
#include "./mocks/MockConfigManager.h"

#include "RTC_interface.h"

#include "helpers.h"

void setLocalTimestampTest(uint32_t localTimestamp, int32_t timezone, uint16_t DST, uint8_t dayOfWeek){
  MockWire wire = MockWire(); MockConfigManager config = MockConfigManager(timezone, DST);
  RTCInterfaceClass<MockWire, MockConfigManager> RTC = RTCInterfaceClass(wire, config);
  RTC.setLocalTimestamp(localTimestamp);
  wire.incrementSeconds();  // RTC should have written the time to wire. By incrementing the second value,
  EXPECT_EQ(RTC.getLocalTimestamp(), localTimestamp + 1);  // we can check this behaviour
  EXPECT_EQ(RTC.getUTCTimestamp(), localTimestamp + 1 - timezone - DST);
  EXPECT_EQ(RTC.datetime.dayOfWeek, dayOfWeek);
}

void setUTCTimestampTest(uint32_t localTimestamp, int32_t timezone, uint16_t DST, uint8_t dayOfWeek){
  MockWire wire = MockWire(); MockConfigManager config = MockConfigManager(timezone, DST);
  RTCInterfaceClass<MockWire, MockConfigManager> RTC = RTCInterfaceClass(wire, config);
  RTC.setUTCTimestamp(localTimestamp - timezone - DST);
  wire.incrementSeconds();  // RTC should have written the time to wire. By incrementing the second value,
  EXPECT_EQ(RTC.getLocalTimestamp(), localTimestamp + 1);  // we can check this behaviour
  EXPECT_EQ(RTC.getUTCTimestamp(), localTimestamp + 1 - timezone - DST);
  EXPECT_EQ(RTC.datetime.dayOfWeek, dayOfWeek);
}

void changeTimezoneTest(uint32_t localTimestamp, int32_t initialTimezone, uint16_t DST){
  MockWire wire = MockWire(); MockConfigManager config = MockConfigManager(initialTimezone, DST);
  RTCInterfaceClass<MockWire, MockConfigManager> RTC = RTCInterfaceClass(wire, config);
  uint32_t UTCTimestamp = localTimestamp - DST - initialTimezone;
  RTC.setUTCTimestamp(UTCTimestamp);
  for(int i = -12*4; i <= 14*4; i++){
    int32_t timezoneOffset = i*15*60;
    RTC.setTimezoneOffset(timezoneOffset);
    RTC.commitUpdates();
    EXPECT_EQ(RTC.getLocalTimestamp(), UTCTimestamp + DST + timezoneOffset);
    EXPECT_EQ(RTC.getUTCTimestamp(), UTCTimestamp);
  }
}

void changeDSTTest(uint32_t localTimestamp, int32_t timezone, uint16_t initialDST){
  MockWire wire = MockWire(); MockConfigManager config = MockConfigManager(timezone, initialDST);
  RTCInterfaceClass<MockWire, MockConfigManager> RTC = RTCInterfaceClass(wire, config);
  uint32_t UTCTimestamp = localTimestamp - initialDST - timezone;
  RTC.setUTCTimestamp(UTCTimestamp);
  uint16_t possibleDST[4] = { 0, 30*60, 60*60, 2*60*60};
  for(uint8_t i = 0; i < 4; i++){
    uint16_t DSTOffset = possibleDST[i];
    RTC.setDSTOffset(DSTOffset);
    RTC.commitUpdates();
    EXPECT_EQ(RTC.getLocalTimestamp(), UTCTimestamp + timezone + DSTOffset);
    EXPECT_EQ(RTC.getUTCTimestamp(), UTCTimestamp);
  }
}


#define QUICKTEST(testName, expectedLocalTimestamp, seconds, minutes, hours, dayOfWeek, date, month, years, timezone, DST)\
TEST(local_timestamp_can_be_set, testName){\
  setLocalTimestampTest(expectedLocalTimestamp, timezone, DST, dayOfWeek);\
}\
TEST(UTC_timestamp_can_be_set, testName){\
  setUTCTimestampTest(expectedLocalTimestamp, timezone, DST, dayOfWeek);\
}\
TEST(timezone_can_be_set, testName){\
  changeTimezoneTest(expectedLocalTimestamp, timezone, DST);\
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
