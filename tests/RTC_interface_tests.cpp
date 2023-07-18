#define USE_TESTING_TYPES

#include <gtest/gtest.h>
#include "./mocks/MockWire.h"

#define USE_DS3231
#include "RTC_interface.h"

TEST(CLOCK_ADDRESS_test, BasicAssertions){
  EXPECT_EQ(CLOCK_ADDRESS, 0x68);
}

// TEST(fetchTime, returns_the_correct_time){
//   MockWire wire = MockWire()
// }