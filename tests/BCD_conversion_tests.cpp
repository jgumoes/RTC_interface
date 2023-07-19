#include <gtest/gtest.h>
#include "./mocks/MockWire.h"

#define USE_DS3231
#include "RTC_interface.h"

MockWire emptyWire = MockWire();
RTCInterfaceClass<MockWire> RTC = RTCInterfaceClass(emptyWire);

// decToBcd tests

TEST(decToBcd, converts_9){
  EXPECT_EQ(RTC.decToBcd(9), 9);
}

TEST(decToBcd, converts_10){
  EXPECT_EQ(RTC.decToBcd(10), 1 << 4);
}

TEST(decToBcd, converts_49){
  EXPECT_EQ(RTC.decToBcd(49), 9 | (4 << 4));
}

TEST(decToBcd, converts_50){
  EXPECT_EQ(RTC.decToBcd(50), (5 << 4));
}

TEST(decToBcd, converts_59){
  EXPECT_EQ(RTC.decToBcd(59), 9 | (5 << 4));
}

// bcd2bin tests

TEST(bcd2bin, converts_9){
  EXPECT_EQ(RTC.bcd2bin(9), 9);
}

TEST(bcd2bin, converts_10){
  EXPECT_EQ(RTC.bcd2bin(1 << 4), 10);
}

TEST(bcd2bin, converts_49){
  EXPECT_EQ(RTC.bcd2bin(9 | (4 << 4)), 49);
}

TEST(bcd2bin, converts_50){
  EXPECT_EQ(RTC.bcd2bin((5 << 4)), 50);
}

TEST(bcd2bin, converts_59){
  EXPECT_EQ(RTC.bcd2bin(9 | (5 << 4)), 59);
}
