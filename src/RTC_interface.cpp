#include "RTC_interface.h"

/**
 * @brief breaks down a local timestamp into a DateTimeStruct. doesn't adjust for
 * DST or timezones
 * 
 * @param time 
 * @param datetime 
 */
void convertFromLocalTimestamp(uint32_t time, DateTimeStruct *datetime){
  datetime->seconds = time % 60;   // i.e. divide by number of minutes and take remainder
  time /= 60;             // time is now in minutes
  datetime->minutes = time % 60;   // i.e. divide by number of hours and take remainder
  time /= 60;             // time is now in hours
  datetime->hours = time % 24;     // i.e. divide by number of days and take remainder
  time /= 24;             // time is now in days since 1/1/2000
  datetime->dayOfWeek = (time + 6) % 7;  // epoch starts on a saturday
  datetime->dayOfWeek += 7 * !datetime->dayOfWeek;  // if day is 0, set it to 7
  datetime->years = ((4 * time) / 1461);
  time -= (datetime->years * 365.25) - 1;   // time is now in day of the year
                                            // +1 because day of the year isn't zero-indexed

  uint8_t leapDay = (!(datetime->years % 4) && (time > 59)); // knock of a day if its a leap year and after Feb 28
  time -= leapDay;
  uint8_t i = 0;
  while (time > monthDays[i]){
      time -= monthDays[i];
      i++;
  }
  datetime->month = i + 1;
  datetime->date = time + (leapDay && (datetime->month == 2)); // add the leap day back if it's actually Feb 29
}

uint32_t convertToLocalTimestamp(DateTimeStruct *datetime){
  uint32_t timestamp = datetime->years * 365.25;  // in days. adds an extra day on a leap year, which we might not want

  uint16_t dayOfYear = datetime->date;
  for(uint8_t month = 1; month < datetime->month; month++){
    dayOfYear += monthDays[month - 1];
  }

  if((datetime->years % 4 == 0) && (datetime->month <= 2)){
    dayOfYear -= 1; // handle leap days
  }
  
  timestamp += dayOfYear;
  timestamp = (timestamp * 24) + datetime->hours;
  timestamp = (timestamp * 60) + datetime->minutes;
  timestamp = (timestamp * 60) + datetime->seconds;

  return timestamp;
}

/*
 * convert a binary-coded decimal into a regular binary integer
 */
uint8_t bcdToDec (uint8_t val) { return (val & 0b00001111) + ((val >> 4) * 10); }
// uint8_t bcdToDec (uint8_t val) { return val - 6 * (val >> 4); }

/*
 * convets a regular integer to binary-coded decimal
 */
uint8_t decToBcd(uint8_t val) { return ( ((val/10) << 4) + (val%10) ); }