/*
* A generic interface for reading from and writing to an RTC.
* Currently only supports the DS3231, but I will add other chips
* if I buy them (i probably won't ;-) lol)
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
*/

#ifndef RTC_interface_h
#define RTC_interface_h

#ifdef native_env
  #include <stdint.h>
  // #include <iostream>
#endif
#include <memory>
#include "structs.h"

#define USE_DS3231  // TODO: remove once more chipsets are supported

// define which RTC chipset is being used
#ifdef USE_DS3231
  #define CLOCK_ADDRESS 0x68
#endif

#define SECONDS_BETWEEN_1970_2000 946684800

#define BCDMask 0b00001111

const uint8_t monthDays[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
void convertFromLocalTimestamp(uint64_t time, DateTimeStruct *datetime);
uint64_t convertToLocalTimestamp(DateTimeStruct *datetime);

uint8_t bcdToDec (uint8_t val);
uint8_t decToBcd(uint8_t val);

/*
 * Handles interfacing with the RTC.
 * Gets and sets the current time, sets the alarm, and sets RTC settings.
 *
 * Changing the time:
 * the user must use setX(y) to change the intended values,
 * then call commitUpdates() to write the changes to the RTC chip
 */
template <typename WireClassDependancy, typename ConfigManagerDependancy>
class RTCInterfaceClass{
  public:
    std::shared_ptr<WireClassDependancy> _wire;
    std::shared_ptr<ConfigManagerDependancy> _configManager;
 
    RTCInterfaceClass(std::shared_ptr<WireClassDependancy> WireClass, std::shared_ptr<ConfigManagerDependancy> configManager)
      : _wire(WireClass), _configManager(configManager) {
    }

    void begin(){
      RTCConfigsStruct configVals = _configManager->getRTCConfigs();
      _timeZoneSecs = configVals.timezone;
      _DSTOffsetSecs = configVals.DST;
      setTo24hr();
    }

    DateTimeStruct datetime;
    // const uint8_t monthDays[12] = {31,28,31,30,31,30,31,31,30,31,30,31};

    /*
     * Returns the local timestamp in seconds since the 2000 epoch
     */
    uint64_t getLocalTimestamp();
    
    /*
     * Returns the UTC timestamp, without timezone and DST adjustments
     */
    uint64_t getUTCTimestamp(){ return getLocalTimestamp() - _timeZoneSecs - _DSTOffsetSecs; }

    DateTimeStruct getDatetime(){fetchDatetime();return datetime;}

    /*
     * Set pending timezone, comforming to Device Time Service.
     * call commitUpdates() to commit the new timezone.
     * @param seconds timezone offset in 15 minute increments
     */
    void setTimezoneOffset(int32_t seconds){
      pendingUpdates.timezone = seconds;
      pendingUpdates.timezonePending = true;
    }

    /*
     * @return the timezone offset in seconds
     */
    int32_t getTimezoneOffset(){return _timeZoneSecs;}
    
    /*
     * @param seconds DST offset in seconds
     */
    void setDSTOffset(uint16_t seconds){
      pendingUpdates.DST = seconds;
      pendingUpdates.DSTPending = true;
    }

    uint16_t getDSTOffset(){return _DSTOffsetSecs;}

    /*
     * Sets the time with a UTC timestamp and offsets, and writes changes to RTC chip
     * @param time UTC timestamp in seconds
     * @param timezone offset in seconds
     * @param DST offset in seconds
     * @return if the write was successful
     */
    bool setUTCTimestamp(uint64_t time, int32_t timezone, uint16_t dst){
      setTimezoneOffset(timezone);
      setDSTOffset(dst);
      return setUTCTimestamp(time);
    };
    
    /*
     * Sets the time using a UTC timestamp
     * @param time the UTC timestamp in seconds
     * @note the timezone and dst offsets must be set BEFORE calling this function
     * @return if the write was successful
     */
    bool setUTCTimestamp(uint64_t time){
      return setLocalTimestamp(time + _timeZoneSecs + _DSTOffsetSecs);
    };

    /*
     * set the time using a local timestamp.
     * @param time the local timestamp in seconds
     * @note the timezone and dst offsets must be set BEFORE calling this function
     * @return if the write was successful
     */
    bool setLocalTimestamp(uint64_t time){
      pendingUpdates.timestamp = time;
      pendingUpdates.timestampPending = true;
      return commitUpdates();
    };

    /*
     * sends pending updates to the RTC and saves the offsets to file.
     * Resets pending updates buffer on success.
     * @returns if commit was successful
     */
    bool commitUpdates();

    /*
     * resets all stored values before performing read/write operations
     */
    void resetDatetime();

    /*
     * enables a square wave at a preset frequency
     * @param f frequency of the square wave. use the SquareWaveFrequencies enum
     */
    void enableSquarewave(SquareWaveFrequencies f){
      if(f == SquareWaveFrequencies::off){
        disableSquareware();
      }
      else{
        transmit2Bytes(0x0E, 0b01000000 | static_cast<uint8_t>(f) << 3);
      }
    }

    /*
     * disables the square wave
     */
    void disableSquareware(){
        transmit2Bytes(0x0E, 0b00000000);
    }

#ifdef USE_BCD_TIME
    BCDTimeStruct BCDTime;
    BCDDateStruct BCDDate;

    BCDTimeStruct getBCDTime();
    BCDDateStruct getBCDDate();
#endif

  private:
    int32_t _timeZoneSecs = 0;    // timezone offset in seconds
    uint16_t _DSTOffsetSecs = 0;  // daylight savings offset in seconds

    /*
     * Fetchs the time from the RTC chipset, and unpacks it into a datetime object
     */
    void fetchDatetime();

    uint64_t localTimestamp;
    PendingUpdatesStruct pendingUpdates;

    /*
     * resets pending updates after updates have been made
     */
    void resetPendingUpdates();

    /*
     * Writes a local timestamp to the RTC chip.
     * @param time local timestamp in seconds
     * @return if operation was performed without any errors
     */
    bool updateLocalTimestamp(uint64_t time){
      // datetime.readReady = false;
      resetDatetime();
      convertFromLocalTimestamp(time, &datetime);
      return transmitDatetime();
    };

    /*
     * Transmits the dateTime struct to the RTC chip.
     * @param time local timestamp in seconds
     * @return if operation was performed without any errors
     */
    bool transmitDatetime();

    /*
     * sets the rtc to use 24 hour time
     * NOTE: this method is likely to only be called once
     * in a projects lifetime.
     */
    void setTo24hr();

    /*
     * transmits a single byte, AKA the register address to be read from
     * note: device address is set by CLOCK_ADDRESS
     * @return if operation was performed without any errors
     */
    bool transmitByte(uint8_t registerAddress);

    /*
     * transmits 2 byte packets, AKA the register address and the value
     * note: device address is set by CLOCK_ADDRESS
     * @return if operation was performed without any errors
     */
    bool transmit2Bytes(uint8_t registerAddress, uint8_t value);
};

/* Public Class Methods */

template <typename WireClassDependancy, typename ConfigManagerDependancy>
uint64_t RTCInterfaceClass<WireClassDependancy, ConfigManagerDependancy>::getLocalTimestamp(){
  fetchDatetime();
  datetime.readReady = true;
  localTimestamp = (datetime.years * 365) + ((datetime.years + 3)/4);      // years to days

  uint16_t days = datetime.date - 1 + (!(datetime.years % 4) && (datetime.month > 2)); // remove a day to 0 index the days, but
                                                                                          // add a day if it's a leap year and after Feb
  for(int i = 0; i < datetime.month - 1; i++){ days += monthDays[i];}

  localTimestamp = (localTimestamp + days) * 24;      // days to hours
  localTimestamp = (localTimestamp + datetime.hours) * 60;     // hours to minutes
  localTimestamp = (localTimestamp + datetime.minutes) * 60;   // minutes to seconds
  localTimestamp += datetime.seconds;                          // add the seconds
  return localTimestamp;
}

template <typename WireClassDependancy, typename ConfigManagerDependancy>
bool RTCInterfaceClass<WireClassDependancy, ConfigManagerDependancy>::commitUpdates(){
  uint64_t timestamp = (pendingUpdates.timestampPending) ? pendingUpdates.timestamp : getLocalTimestamp();  // set a new time or update the current one
  if(pendingUpdates.DSTPending){
    timestamp += pendingUpdates.DST - _DSTOffsetSecs; // replace DST offset with incoming offset
    _DSTOffsetSecs = pendingUpdates.DST;
  }
  if(pendingUpdates.timezonePending){
    timestamp += pendingUpdates.timezone - _timeZoneSecs; // replace timezone offset with incoming offset
    _timeZoneSecs = pendingUpdates.timezone;
  }
  bool res = updateLocalTimestamp(timestamp);
  if(res){
    resetPendingUpdates();
    RTCConfigsStruct configs;
    configs.DST = _DSTOffsetSecs;
    configs.timezone = _timeZoneSecs;
    return _configManager->setRTCConfigs(configs);
  }
  return res;
}

template <typename WireClassDependancy, typename ConfigManagerDependancy>
void RTCInterfaceClass<WireClassDependancy, ConfigManagerDependancy>::resetDatetime(){
  datetime.date = 0;
  datetime.dayOfWeek = 0;
  datetime.hours = 0;
  datetime.minutes = 0;
  datetime.month = 0;
  datetime.seconds = 0;
  datetime.years = 0;
}

#ifdef USE_BCD_TIME
template <typename WireClassDependancy, typename ConfigManagerDependancy>
BCDTimeStruct RTCInterfaceClass<WireClassDependancy, ConfigManagerDependancy>::getBCDTime(){
  BCDTime.readReady = false;
  transmitByte(0);

  _wire->requestFrom(CLOCK_ADDRESS, 3);
  uint8_t seconds = _wire->read();
  uint8_t minutes = _wire->read();
  uint8_t hours = _wire->read();

  uint8_t hoursMask = 0b00111111;
  if(hours & 0b01000000){
    // if mode is 12 hours, bit 5 is am/pm and should be ignored
    hoursMask = (hoursMask >> 1);
  }
  hours &= hoursMask; // ignore bit 6 (12/24 hr)

  BCDTime.seconds_1 = seconds & BCDMask;
  BCDTime.seconds_10 = (seconds >> 4) & BCDMask;
  BCDTime.minutes_1 = minutes & BCDMask;
  BCDTime.minutes_10 = (minutes >> 4) & BCDMask;
  BCDTime.hours_1 = hours & BCDMask;
  BCDTime.hours_10 = (hours >> 4);
  BCDTime.readReady = true;
  return BCDTime;
}

template <typename WireClassDependancy, typename ConfigManagerDependancy>
BCDDateStruct RTCInterfaceClass<WireClassDependancy, ConfigManagerDependancy>::getBCDDate(){
  BCDDate.readReady = false;
  transmitByte(0x03);

  _wire->requestFrom(CLOCK_ADDRESS, 4);
  uint8_t dayOfWeek = _wire->read();
  uint8_t date = _wire->read();
  uint8_t month = _wire->read() & 0b01111111; // we don't need the century bit
  uint8_t year = _wire->read();
  
  BCDDate.dayOfWeek = dayOfWeek;
  BCDDate.date_1 = date & BCDMask;
  BCDDate.date_10 = (date >> 4) & BCDMask;
  BCDDate.month_1 = month & BCDMask;
  BCDDate.month_10 = (month >> 4) & BCDMask;
  BCDDate.year_1 = year & BCDMask;
  BCDDate.year_10 = (year >> 4) & BCDMask;
  BCDDate.readReady = true;
  return BCDDate;
}
#endif

/* Private Class Methods */

template <typename WireClassDependancy, typename ConfigManagerDependancy>
void RTCInterfaceClass<WireClassDependancy, ConfigManagerDependancy>::fetchDatetime(){
  resetDatetime();
  transmitByte(0);

  _wire->requestFrom(CLOCK_ADDRESS, 13);
  datetime.seconds = bcdToDec(_wire->read());
  datetime.minutes = bcdToDec(_wire->read());
  datetime.hours = bcdToDec(_wire->read());
  datetime.dayOfWeek = bcdToDec(_wire->read());
  datetime.date = bcdToDec(_wire->read());
  datetime.month = bcdToDec(_wire->read() & 0b00011111);
  datetime.years = bcdToDec(_wire->read()); // years since midnight 2000
  for(int i = 8; i < 13; i++){
    _wire->read();
  }
}

template <typename WireClassDependancy, typename ConfigManagerDependancy>
void RTCInterfaceClass<WireClassDependancy, ConfigManagerDependancy>::resetPendingUpdates(){
  pendingUpdates.timezonePending = 0;
  pendingUpdates.timezone = 0;
  pendingUpdates.DSTPending = 0;
  pendingUpdates.DST = 0;
  pendingUpdates.timestampPending = 0;
  pendingUpdates.timestamp = 0;
}

template <typename WireClassDependancy, typename ConfigManagerDependancy>
bool RTCInterfaceClass<WireClassDependancy, ConfigManagerDependancy>::transmitDatetime(){
  bool anyErrors = 0;
  anyErrors |= !transmit2Bytes(0x00, decToBcd(datetime.seconds));
  anyErrors |= !transmit2Bytes(0x01, decToBcd(datetime.minutes));
  anyErrors |= !transmit2Bytes(0x02, decToBcd(datetime.hours));
  anyErrors |= !transmit2Bytes(0x03, datetime.dayOfWeek);
  anyErrors |= !transmit2Bytes(0x04, decToBcd(datetime.date));
  anyErrors |= !transmit2Bytes(0x05, decToBcd(datetime.month));
  anyErrors |= !transmit2Bytes(0x06, decToBcd(datetime.years));

  return !anyErrors;
}

template <typename WireClassDependancy, typename ConfigManagerDependancy>
void RTCInterfaceClass<WireClassDependancy, ConfigManagerDependancy>::setTo24hr(){
  uint8_t hours_register;
  fetchDatetime();  // get the time right now
  hours_register = decToBcd(datetime.hours);  // convert datetime.hours back into its register format

  const uint8_t BIT_FLAG_12_HR = 0b01000000;
  if(hours_register & BIT_FLAG_12_HR){
    // if set to 12 hour time
    uint8_t trueHours = decToBcd(datetime.hours) - BIT_FLAG_12_HR;  // convert hours back to the register value and remove the 12 hour flag

    const uint8_t IS_PM_FLAG = 0b00100000;
    if((trueHours & IS_PM_FLAG) > 0){
      trueHours -= (IS_PM_FLAG - 0b00010010); // remove PM flag and add 12 in BCD
    }
    trueHours = bcdToDec(trueHours);
    datetime.hours = trueHours % 24;
    
    transmitDatetime();
  }
}

template <typename WireClassDependancy, typename ConfigManagerDependancy>
bool RTCInterfaceClass<WireClassDependancy, ConfigManagerDependancy>::transmitByte(uint8_t registerAddress){
  _wire->beginTransmission(CLOCK_ADDRESS);
  _wire->write(registerAddress);
  if(_wire->endTransmission() != 0){
    return false;
  }
  return true;
}

template <typename WireClassDependancy, typename ConfigManagerDependancy>
bool RTCInterfaceClass<WireClassDependancy, ConfigManagerDependancy>::transmit2Bytes(uint8_t registerAddress, uint8_t value){
  _wire->beginTransmission(CLOCK_ADDRESS);
  _wire->write(registerAddress);
  _wire->write(value);
  if(_wire->endTransmission() != 0){
    return false;
  }
  return true;
}

#endif
