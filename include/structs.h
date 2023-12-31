#ifndef _STRUCTS_H_
#define _STRUCTS_H_

#include <stdint.h>

struct DateTimeStruct{
  uint8_t seconds;
  uint8_t minutes;
  uint8_t hours;
  uint8_t dayOfWeek;  // 1 to 7
  uint8_t date;     // 1 to monthDays[month - 1] (Feb can be 29)
  uint8_t month;    // 1 to 12
  uint8_t years;    // 0 = 2000
  bool readReady = false; // if true, struct is suitable for reading
};

struct PendingUpdatesStruct{
  bool timezonePending = false;
  int32_t timezone = 0;
  bool DSTPending = false;
  uint16_t DST = 0;
  bool timestampPending = false;
  uint32_t timestamp = 0; // incoming timestamps will always be UTC as per BLE DTS specification
};

// holds the config values
struct RTCConfigsStruct{
   int32_t timezone = 0;  // timezone offset in seconds
   uint16_t DST = 0;      // daylight savings offset in seconds
};

enum class SquareWaveFrequencies: uint8_t {
  hz_1 = 0,     // 1 Hz
  hz_1024 = 1,  // 1.024 kHz
  hz_4096 = 2,  // 4.096 kHz
  hz_8192 = 3,  // 8.192 kHz
  off = 4       // no square wave
};

#ifdef USE_BCD_TIME // don't need this in the progmem if we're not going to use it
  struct BCDTimeStruct{
    uint8_t seconds_1;
    uint8_t seconds_10;
    uint8_t minutes_1;
    uint8_t minutes_10;
    uint8_t hours_1;
    uint8_t hours_10;
    bool readReady;
  };

  struct BCDDateStruct
  {
    uint8_t dayOfWeek;
    uint8_t date_1;
    uint8_t date_10;
    uint8_t month_1;
    uint8_t month_10;
    uint8_t year_1;
    uint8_t year_10;
    bool readReady;
  };
  
#endif

#endif