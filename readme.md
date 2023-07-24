A generic interface for reading from and writing to an RTC over I2C. This was originally written as part of an implementation of the Device Time Service as specified by the Bluetooth Special Interest Group, and as such includes features required for the service (i.e. getting a UTC timestamp).

Currently only supports the DS3231, but I will add other chips
as I buy them ;-). Doesn't yet support alarms.

In the future, I'd like to use a chipset with on-board eeprom for storing the timezone and dst offsets, but hobbyist RTC modules will need access to some kind of storage. This will probably be in the form of a filesystem interface class.

There is a known bug where a timestamp for 1/3/2024 (a leap year) gets interpreted as 2/3/2024

# Theory of Operation

## Reading time

The normal process of reading time is:
 - TODO: the class loads stored timezone and daylight savings offsets from storage and holds as instance variables
 - local time is read from the chipset in whichever form it uses, and stores the decimal values in a struct
 - the DateTimeStruct can be read directly. DateTimeStruct::readReady will be true if the read operation has completed (useful for RTOS)
 - if a local timestamp is required, the DateTimeStruct values are combined into seconds since midnight 1/1/2000. SECONDS_BETWEEN_1970_2000 can be added if the 1970 epoch is preferred.

Alternatively, the seconds, minutes and hours can be read as binary-coded decimals. This is useful for driving 7-segment displays, and the reduced computations is especially useful when bit-banging a one-wire interface.
 - only seconds, minutes, and hours are read from the chip.
 - the 1s and 10s values are stored separately in a BCDTimeStruct

Use `#define USE_BCD_TIME` if reading in BCD form is desired.

## Updating time, timezone, and/or daylight savings

 - new UTC timestamp, timezone, and/or daylight savings offsets are passed to the class
 - the new values are stored in a temporary struct
 - when commitChanges() is called, the values are combined to create a local timestamp
 - (if a new UTC timestamp isn't given, the old one is used instead)
 - the class will attempt to write the new local timestamp to the chip
 - if the write is successful, the new offsets will be saved to storage.

# Usage

While this library isn't dependent on any particular I2C library, it is built around the Wire library for esp boards, which I believe was made to match the built-in Wire library for Arduino.

```C++
#define USE_BCD_TIME  // only define if reading values in BCD format is desired.

#include <Wire.h>
#include "RTC_interface.h"

RTCInterfaceClass<TwoWire> RTC = RTCInterfaceClass(Wire);
```

This library only supports 24 hour time, but if you really want to display 12 hour, you could do:
```c++
uint8_t hours24 = (BCDTime.hours_10 << 4) + (BCDTime.hours_1 > 2);
bool isPm = hours24 > 12
if(isPm){ hours24 -= 12 }
uint8_t hours_1 = hours24 % 10;
uint8_t hours_10 = hours24 / 10;
```

## Application Notes

 * the time is stored as the local time. Timezone and DST offsets are removed to create a UTC timestamp
 * the timestamps are in seconds since midnight 2000. If the 1970 epoch is required, the user must do the conversion themselves.
 * The timezone and DST values are stored and handled as seconds. They must be converted to seconds before trying to set them.
 * This library is not dependant on any particular I2C class. The user must specify the class they are using.
 * The library also does not depend on any particular class or library for handling configs. However, the config class is required to have the methods `RTCConfigsStruct ConfigClass::getRTCConfigs()` and `bool ConfigClass::setRTCConfigs(RTCConfigsStruct configs)`.
 * It's recommended that the user make their own class that handles all the configs for their application, that is responsible for reading and writing configs from the file system of choice.

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
CMake is set up to only see the tests directory if testing is enabled. It will also include optional extras (i.e. reading time in BCD format).
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