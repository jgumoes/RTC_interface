#ifndef __HELPERS_H__
#define __HELPERS_H__

#define ONES(decimal) (decimal % 10)
#define TENS(decimal) (decimal / 10)

#define AS_BCD(decimal) (TENS(decimal) << 4) | ONES(decimal)

#include <stdio.h>
#define PRINT_AS_INT(title, value) std::cout << title << ": " << static_cast<int>(value) << std::endl;

#include <algorithm>

#define PRINT_ARRAY(name, array, length) std::cout << name << ":" << std::endl; \
  for(int i = 0; i < length; i++){\
    std::cout << '\t';\
    int thisArray[length];\
    std::copy(std::begin(array), std::end(array), std::begin(thisArray));\
    PRINT_AS_INT(i, thisArray[i]);\
  }

#endif