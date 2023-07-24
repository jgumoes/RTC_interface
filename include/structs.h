#ifndef _STRUCTS_H_
#define _STRUCTS_H_

#include <stdint.h>

struct DateTimeStruct{
  uint8_t seconds;
  uint8_t minutes;
  uint8_t hours;
  uint8_t dayOfWeek;
  uint8_t date;
  uint8_t month;
  uint8_t years;
  bool readReady; // if true, struct is suitable for reading
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
   int32_t timezone = 0;
   uint16_t DST = 0;
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