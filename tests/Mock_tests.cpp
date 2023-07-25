/*
 * This file is only for testing the functionality of Mock classes and helper macros.
 */

#define USE_DS3231
#define USE_BCD_TIME

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "./mocks/MockWire.h"
#include "./mocks/MockConfigManager.h"

#include "helpers.h"

TEST(AS_BCD_converts_correctly, for_0_to_99){
  MockWire emptyWire;
  MockConfigManager config;
  RTCInterfaceClass<MockWire, MockConfigManager> RTC(emptyWire, config);
  for(int tens = 0; tens < 10; tens++){
    for(int ones = 0; ones < 10; ones++){
      uint8_t expectedResult = (tens << 4) | ones;
      uint8_t actualResult = AS_BCD((tens * 10) + ones);
      ASSERT_EQ(actualResult, expectedResult);
    }
  }
}

TEST(AS_DEC_converts_correctly, for_0_to_99){
  MockWire emptyWire;
  MockConfigManager config;
  RTCInterfaceClass<MockWire, MockConfigManager> RTC(emptyWire, config);
  for(int tens = 0; tens < 10; tens++){
    for(int ones = 0; ones < 10; ones++){
      uint8_t expectedResult = (tens * 10) + ones;
      uint8_t actualResult = AS_DEC((tens << 4) | ones);
      ASSERT_EQ(actualResult, expectedResult);
    }
  }
}

using namespace testing;

TEST(MockWire_incrementSeconds, can_increment_seconds){
  MockWire wire = MockWire(AS_BCD(8), AS_BCD(16), AS_BCD(17), AS_BCD(3), AS_BCD(20), AS_BCD(9), AS_BCD(23));
  wire.incrementSeconds();
  wire.makeDatetimeBuffer();
  const uint8_t resultBuffer[7] = {AS_BCD(9), AS_BCD(16), AS_BCD(17), AS_BCD(3), AS_BCD(20), AS_BCD(9), AS_BCD(23)};
  EXPECT_THAT(wire.datetimeBuffer, ElementsAreArray(resultBuffer, 7));
}

TEST(MockWire_incrementSeconds, can_increment_minutes){
  MockWire wire = MockWire(AS_BCD(59), AS_BCD(16), AS_BCD(17), AS_BCD(3), AS_BCD(20), AS_BCD(9), AS_BCD(23));
  wire.incrementSeconds();
  wire.makeDatetimeBuffer();
  const uint8_t resultBuffer[7] = {AS_BCD(0), AS_BCD(17), AS_BCD(17), AS_BCD(3), AS_BCD(20), AS_BCD(9), AS_BCD(23)};
  EXPECT_THAT(wire.datetimeBuffer, ElementsAreArray(resultBuffer, 7));
}

TEST(MockWire_incrementSeconds, can_increment_hours){
  MockWire wire = MockWire(AS_BCD(59), AS_BCD(59), AS_BCD(17), AS_BCD(3), AS_BCD(20), AS_BCD(9), AS_BCD(23));
  wire.incrementSeconds();
  wire.makeDatetimeBuffer();
  const uint8_t resultBuffer[7] = {AS_BCD(0), AS_BCD(0), AS_BCD(18), AS_BCD(3), AS_BCD(20), AS_BCD(9), AS_BCD(23)};
  EXPECT_THAT(wire.datetimeBuffer, ElementsAreArray(resultBuffer, 7));
}

TEST(MockWire_incrementSeconds, can_increment_days){
  MockWire wire = MockWire(AS_BCD(59), AS_BCD(59), AS_BCD(23), AS_BCD(3), AS_BCD(20), AS_BCD(9), AS_BCD(23));
  wire.incrementSeconds();
  wire.makeDatetimeBuffer();
  const uint8_t resultBuffer[7] = {AS_BCD(0), AS_BCD(0), AS_BCD(0), AS_BCD(4), AS_BCD(21), AS_BCD(9), AS_BCD(23)};
  EXPECT_THAT(wire.datetimeBuffer, ElementsAreArray(resultBuffer, 7));
}

TEST(MockWire_incrementSeconds, can_increment_months){
  MockWire wire = MockWire(AS_BCD(59), AS_BCD(59), AS_BCD(23), AS_BCD(3), AS_BCD(31), AS_BCD(1), AS_BCD(23));
  wire.incrementSeconds();
  wire.makeDatetimeBuffer();
  const uint8_t resultBuffer[7] = {AS_BCD(0), AS_BCD(0), AS_BCD(0), AS_BCD(4), AS_BCD(1), AS_BCD(2), AS_BCD(23)};
  EXPECT_THAT(wire.datetimeBuffer, ElementsAreArray(resultBuffer, 7));
}

TEST(MockWire_incrementSeconds, can_increment_feb_28_23){
  MockWire wire = MockWire(AS_BCD(59), AS_BCD(59), AS_BCD(23), AS_BCD(3), AS_BCD(28), AS_BCD(2), AS_BCD(23));
  wire.incrementSeconds();
  wire.makeDatetimeBuffer();
  const uint8_t resultBuffer[7] = {AS_BCD(0), AS_BCD(0), AS_BCD(0), AS_BCD(4), AS_BCD(1), AS_BCD(3), AS_BCD(23)};
  EXPECT_THAT(wire.datetimeBuffer, ElementsAreArray(resultBuffer, 7));
}

TEST(MockWire_incrementSeconds, can_increment_feb_28_24){
  MockWire wire = MockWire(AS_BCD(59), AS_BCD(59), AS_BCD(23), AS_BCD(3), AS_BCD(28), AS_BCD(2), AS_BCD(24));
  wire.incrementSeconds();
  wire.makeDatetimeBuffer();
  const uint8_t resultBuffer[7] = {AS_BCD(0), AS_BCD(0), AS_BCD(0), AS_BCD(4), AS_BCD(29), AS_BCD(2), AS_BCD(24)};
  EXPECT_THAT(wire.datetimeBuffer, ElementsAreArray(resultBuffer, 7));
}

TEST(MockWire_incrementSeconds, can_increment_feb_29_24){
  MockWire wire = MockWire(AS_BCD(59), AS_BCD(59), AS_BCD(23), AS_BCD(3), AS_BCD(29), AS_BCD(2), AS_BCD(24));
  wire.incrementSeconds();
  wire.makeDatetimeBuffer();
  const uint8_t resultBuffer[7] = {AS_BCD(0), AS_BCD(0), AS_BCD(0), AS_BCD(4), AS_BCD(1), AS_BCD(3), AS_BCD(24)};
  EXPECT_THAT(wire.datetimeBuffer, ElementsAreArray(resultBuffer, 7));
}

TEST(MockWire_incrementSeconds, can_increment_years){
  MockWire wire = MockWire(AS_BCD(59), AS_BCD(59), AS_BCD(23), AS_BCD(3), AS_BCD(31), AS_BCD(12), AS_BCD(23));
  wire.incrementSeconds();
  wire.makeDatetimeBuffer();
  const uint8_t resultBuffer[7] = {AS_BCD(0), AS_BCD(0), AS_BCD(0), AS_BCD(4), AS_BCD(1), AS_BCD(1), AS_BCD(24)};
  EXPECT_THAT(wire.datetimeBuffer, ElementsAreArray(resultBuffer, 7));
}
