A generic interface for reading from and writing to an RTC over I2C.
Currently only supports the DS3231, but I will add other chips
as I buy them ;-). Doesn't yet support alarms.

Theory of Operation:
 - TODO: stored timezone and daylight savings offsets are passed to the class constructor by the KeeperOfTime module
 - new UTC timestamp, timezone, and/or daylight savings offsets are passed to the class
 - the new values are stored in a temporary struct
 - when commitChanges() is called, the values are combined to create a local timestamp
 - (if a new UTC timestamp isn't given, the old one is used instead)
 - the class will attempt to write the new local timestamp to the chip
 - if the write is successful, the new offsets will be saved as instance variables. commitChanges() will return true, and the KeeperOfTime module should save the new offsets to file

There is a known bug where a timestamp for 1/3/2024 (a leap year gets interpreted as 2/3/2024)

# Usage

This library isn't dependent on any particular I2C library, but it is built around the Wire library for esp boards. 

```C++
#include <Wire.h>
#include "RTC_interface.h"

RTCInterfaceClass<TwoWire> RTC = RTCInterfaceClass(Wire);
```

# Testing

Testing is done using Googletest and built through CMake. Setting up CMake has been a very humbling experience.

## setup
make sure Googletest is installed:
```
git submodule add https://github.com/google/googletest
```
make the build folder:
```
cmake -S . -B build
```
CMake is set up to only see the tests directory if testing is enabled. Additionally, RTC_interface uses types common to microcontrollers, but not used in core C++ (i.e. uint8_t). Enabling the tests will selectively include local definitions to make the tests work, but will break an embedded project.
enable the tests:
```
cmake -DBUILD_TESTS=ON -B build
```

## running tests
build the project
```
cmake --build build
```
run the tests
```
cd build; ctest
```