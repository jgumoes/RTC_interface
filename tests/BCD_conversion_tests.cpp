#include <gtest/gtest.h>
#include "./mocks/MockWire.h"

#define USE_DS3231
#include "RTC_interface.h"

MockWire emptyWire = MockWire();
RTCInterfaceClass<MockWire> RTC = RTCInterfaceClass(emptyWire);

TEST(decToBcd, converts_9){
  EXPECT_EQ(RTC.decToBcd(9), 9);
}

TEST(decToBcd, converts_10){
  EXPECT_EQ(RTC.decToBcd(10), 10);
}