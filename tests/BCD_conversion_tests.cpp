#include <gtest/gtest.h>
#include "./mocks/MockWire.h"

#define USE_DS3231
#include "RTC_interface.h"

MockWire emptyWire = MockWire();
RTCInterfaceClass<MockWire> RTC = RTCInterfaceClass(emptyWire);


TEST(decToBcd, converts_correctly){
  for(int tens = 0; tens < 10; tens++){
    for(int ones = 0; ones < 10; ones++){
      ASSERT_EQ(RTC.decToBcd((tens * 10) + ones), (tens << 4) | ones);
    }
  }
}

TEST(bcd2bin, converts_correctly){
  for(int tens = 0; tens < 10; tens++){
    for(int ones = 0; ones < 10; ones++){
      ASSERT_EQ(RTC.bcd2bin((tens << 4) | ones), (tens * 10) + ones);
    }
  }
}