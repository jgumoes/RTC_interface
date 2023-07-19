#include <gtest/gtest.h>
#include "./mocks/MockWire.h"

#define USE_DS3231
#include "RTC_interface.h"

TEST(getLocalTimestamp_gets_the_correct_timestamp, at_the_beginning_of_time){
  MockWire wire = MockWire(0, 0, 0, 6, 1, 1, 0);  // set to midnight, 1/1/2000
  RTCInterfaceClass<MockWire> RTC = RTCInterfaceClass(wire);
  EXPECT_EQ(RTC.getLocalTimestamp(), 0);
}

TEST(getLocalTimestamp_gets_the_correct_timestamp, during_british_winter_time_2023){
  MockWire wire = MockWire(2, 59, 10, 2, 7, 2, 23);
  wire.convertToBcd();
  RTCInterfaceClass<MockWire> RTC = RTCInterfaceClass(wire);
  EXPECT_EQ(RTC.getLocalTimestamp(), 729082742);
}

TEST(getLocalTimestamp_gets_the_correct_timestamp, during_british_summer_time_2023){
  MockWire wire = MockWire(21, 13, 17, 3, 19, 7, 23);
  wire.convertToBcd();
  RTCInterfaceClass<MockWire> RTC = RTCInterfaceClass(wire);
  EXPECT_EQ(RTC.getLocalTimestamp(), 743102001);
}

TEST(getLocalTimestamp_gets_the_correct_timestamp, feb_28_2023){
  MockWire wire = MockWire(0, 58, 18, 2, 28, 2, 23);
  wire.convertToBcd();
  RTCInterfaceClass<MockWire> RTC = RTCInterfaceClass(wire);
  EXPECT_EQ(RTC.getLocalTimestamp(), 730925880);
}

TEST(getLocalTimestamp_gets_the_correct_timestamp, mar_1_2023){
  MockWire wire = MockWire(13, 45, 17, 3, 1, 3, 23);
  wire.convertToBcd();
  RTCInterfaceClass<MockWire> RTC = RTCInterfaceClass(wire);
  EXPECT_EQ(RTC.getLocalTimestamp(), 731007913);
}

TEST(getLocalTimestamp_gets_the_correct_timestamp, feb_28_2024){
  MockWire wire = MockWire(0, 58, 18, 3, 28, 2, 24);
  wire.convertToBcd();
  RTCInterfaceClass<MockWire> RTC = RTCInterfaceClass(wire);
  EXPECT_EQ(RTC.getLocalTimestamp(), 762461880);
}

TEST(getLocalTimestamp_gets_the_correct_timestamp, feb_29_2024){
  MockWire wire = MockWire(0, 58, 18, 4, 29, 2, 24);
  wire.convertToBcd();
  RTCInterfaceClass<MockWire> RTC = RTCInterfaceClass(wire);
  EXPECT_EQ(RTC.getLocalTimestamp(), 762548280);
}

TEST(getLocalTimestamp_gets_the_correct_timestamp, mar_1_2024){
  MockWire wire = MockWire(13, 45, 17, 5, 1, 3, 24);
  wire.convertToBcd();
  RTCInterfaceClass<MockWire> RTC = RTCInterfaceClass(wire);
  EXPECT_EQ(RTC.getLocalTimestamp(), 762630313);
}