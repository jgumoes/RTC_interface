#ifndef __HELPERS_H__
#define __HELPERS_H__

#define ONES(decimal) ((decimal) % 10)
#define TENS(decimal) ((decimal) / 10)

#define AS_BCD(decimal) (TENS(decimal) << 4) | ONES(decimal)
#define AS_DEC(bcd) ((((bcd) >> 4) * 10) + ((bcd) & 0b00001111))

// #include <stdio.h>
#include<iostream>
#define PRINT_AS_INT(title, value) std::cout << title << ": " << static_cast<int>(value) << std::endl

#include <algorithm>
#define COPY_ARRAY(inputArray, destinationArray) std::copy(std::begin(inputArray), std::end(inputArray), std::begin(destinationArray))

#define PRINT_ARRAY(name, array, length) std::cout << name << ":" << std::endl; \
  for(int i = 0; i < length; i++){\
    std::cout << '\t';\
    PRINT_AS_INT(i, array[i]);\
  }

// passes if expectedValue - 1 <= testValue <= expectedValue + 1
#define EXPECT_CLOSE_TO(testValue, expectedValue) EXPECT_TRUE(testValue >= (expectedValue - 1)); EXPECT_TRUE(testValue <= (expectedValue + 1))

#endif