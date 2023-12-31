#include <gtest/gtest.h>
#include "./mocks/MockWire.h"
#include "./mocks/MockConfigManager.h"

#define USE_DS3231
#include "RTC_interface.h"


TEST(decToBcd_converts_correctly, for_0_to_99){
  MockWire emptyWire;
  MockConfigManager config;
  // RTCInterfaceClass<MockWire, MockConfigManager> RTC(emptyWire, config);
  // RTC.begin();
  for(int tens = 0; tens < 10; tens++){
    for(int ones = 0; ones < 10; ones++){
      ASSERT_EQ(decToBcd((tens * 10) + ones), (tens << 4) | ones);
    }
  }
}

TEST(bcd2bin_converts_correctly, for_0_to_99){
  MockWire emptyWire;
  MockConfigManager config;
  // RTCInterfaceClass<MockWire, MockConfigManager> RTC(emptyWire, config);
  // RTC.begin();
  for(int tens = 0; tens < 10; tens++){
    for(int ones = 0; ones < 10; ones++){
      ASSERT_EQ(bcdToDec((tens << 4) | ones), (tens * 10) + ones);
    }
  }
}