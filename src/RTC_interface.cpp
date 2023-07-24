/*
* A generic interface for reading from and writing to an RTC.
* Currently only supports the DS3231, but I will add other chips
* as I buy them ;-)
* 
* Theory of Operation:
*  - TODO: stored timezone and daylight savings offsets are passed to the class constructor by the KeeperOfTime module
*  - new UTC timestamp, timezone, and/or daylight savings offsets are passed to the class
*  - the new values are stored in a temporary struct
*  - when commitChanges() is called, the values are combined to create a local timestamp
*  - (if a new UTC timestamp isn't given, the old one is used instead)
*  - the class will attempt to write the new local timestamp to the chip
*  - if the write is successful, the new offsets will be saved as instance variables.
*      commitChanges() will return true, and the KeeperOfTime module should save the new offsets to file
* 
* There is a known bug where a timestamp for 1/3/2024 (a leap year)
* gets interpreted as 2/3/2024
*/

#include "RTC_interface.h"
