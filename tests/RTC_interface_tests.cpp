#define USE_TESTING_TYPES

#include <gtest/gtest.h>
#include "./mocks/MockWire.h"
#include "./mocks/MockConfigManager.h"

#define USE_DS3231
#include "RTC_interface.h"

#include "helpers.h"

TEST(CLOCK_ADDRESS_test, BasicAssertions){
  EXPECT_EQ(CLOCK_ADDRESS, 0x68);
}

#define BIT_FLAG_12_HR 0b01000000
#define IS_PM_FLAG 0b00100000

// NOTE: RTCInterfaceClass::setTo24hr() is private, but
// should be called in the class constructor
TEST(setTo24hr_test, sets_RTC_to_24_hrs){
  MockWire wire = MockWire(0, 8, 7 | BIT_FLAG_12_HR, 2, 6, 2, 5);
  MockConfigManager config = MockConfigManager(0, 0);
  RTCInterfaceClass<MockWire, MockConfigManager> RTC = RTCInterfaceClass(wire, config);
  RTC.begin();
  EXPECT_FALSE((wire.mockBuffer[2] & BIT_FLAG_12_HR) > 0);
  EXPECT_EQ(RTC.getLocalTimestamp(), 160988880);
  EXPECT_TRUE(wire.writeHasBeenCalledTimes > 0); // this is more to test the MockWire behaviour for the next test
}

TEST(setTo24hr_test, doesnt_write_if_already_set_to_24hr){
  MockWire wire = MockWire(0, 8, 7, 2, 6, 2, 5);
  MockConfigManager config = MockConfigManager(0, 0);
  RTCInterfaceClass<MockWire, MockConfigManager> RTC = RTCInterfaceClass(wire, config);
  RTC.begin();
  EXPECT_FALSE(wire.writeHasModifiedBuffer);
}

TEST(setTo24hr_test, doesnt_lose_time_during_hour_changeover){
  MockWire wire = MockWire(AS_BCD(59), AS_BCD(59), 7 | BIT_FLAG_12_HR, 2, 6, 2, 5);
  wire.changeBufferAfterRead = true;
  COPY_ARRAY(wire.mockBuffer, wire.nextMockBuffer);
  wire.nextMockBuffer[0] = 0; wire.nextMockBuffer[1] = 0; wire.nextMockBuffer[2] = 8;
  wire.changeBufferAfterRead = 7;
  MockConfigManager config = MockConfigManager(0, 0);
  RTCInterfaceClass<MockWire, MockConfigManager> RTC = RTCInterfaceClass(wire, config);
  EXPECT_CLOSE_TO(RTC.getLocalTimestamp(), 160992000);
}

TEST(setTo24hr_test, doesnt_lose_time_during_day_changeover){
  MockWire wire = MockWire(AS_BCD(59), AS_BCD(59), AS_BCD(11) + IS_PM_FLAG +  BIT_FLAG_12_HR, 2, 6, 2, 5);
  wire.changeBufferAfterRead = true;
  COPY_ARRAY(wire.mockBuffer, wire.nextMockBuffer);
  wire.nextMockBuffer[0] = 0; wire.nextMockBuffer[1] = 0; wire.nextMockBuffer[2] = 0; wire.nextMockBuffer[4] = 7;
  wire.changeBufferAfterRead = 8;
  MockConfigManager config = MockConfigManager(0, 0);
  RTCInterfaceClass<MockWire, MockConfigManager> RTC = RTCInterfaceClass(wire, config);
  RTC.begin();
  EXPECT_CLOSE_TO(RTC.getLocalTimestamp(), 161049600);
  // EXPECT_EQ(RTC.getLocalTimestamp(), 161049599);
}

TEST(config_values, are_loaded_when_RTC_initiates){
  MockWire wire = MockWire(0, 8, 7, 2, 6, 2, 5);
  MockConfigManager config = MockConfigManager(7 * 60 * 60, 1 * 60 * 60);
  RTCInterfaceClass<MockWire, MockConfigManager> RTC = RTCInterfaceClass(wire, config);
  RTC.begin();
  EXPECT_EQ(RTC.getDSTOffset(), 1 * 60 * 60);
  EXPECT_EQ(RTC.getTimezoneOffset(), 7 * 60 * 60);
}