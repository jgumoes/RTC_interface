
#ifndef RTC_interface_h
#define RTC_interface_h

#include <stdint.h>

#define USE_DS3231  // TODO: remove once more chipsets are supported

// define which RTC chipset is being used
#ifdef USE_DS3231
  #define CLOCK_ADDRESS 0x68
#endif

#define SECONDS_BETWEEN_1970_2000 946684800;

#define BCDMask 0b00001111

struct DateTimeStruct{
  uint16_t seconds;
  uint16_t minutes;
  uint16_t hours;
  uint16_t dayOfWeek;
  uint16_t date;
  uint16_t month;
  uint16_t years;
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

#ifdef USE_BCD_TIME
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

/*
 * Handles interfacing with the RTC.
 * Gets and sets the current time, sets the alarm, and sets RTC settings.
 *
 * Changing the time:
 * the user must use setX(y) to change the intended values,
 * then call commitUpdates() to write the changes to the RTC chip
 */
template <typename WireClassDependancy>
class RTCInterfaceClass{
  public:
    WireClassDependancy Wire;
 
    RTCInterfaceClass(const WireClassDependancy& WireClass){
      Wire = WireClass;
      setTo24hr();
    }

    DateTimeStruct datetime;
    const uint8_t monthDays[12] = {31,28,31,30,31,30,31,31,30,31,30,31};

#ifdef USE_BCD_TIME
    BCDTimeStruct BCDTime;
    BCDDateStruct BCDDate;
#endif
    /*
     * Returns the local timestamp in seconds since the 2000 epoch
     */
    uint32_t getLocalTimestamp(){
      fetchDatetime();
      datetime.readReady = true;
      // TODO: can this be improved?
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
    
    /*
     * Returns the UTC timestamp, without timezone and DST ajustments
     */
    uint32_t getUTCTimestamp(){
      return getLocalTimestamp() - _timeZoneSecs - _DSTOffsetSecs;
    }

    DateTimeStruct getDatetime(){
      fetchDatetime();
      return datetime;
    }

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
    int32_t getTimezoneOffset(){
      return _timeZoneSecs;
    }
    
    /*
     * @param seconds DST offset in seconds
     */
    void setDSTOffset(uint16_t seconds){
      pendingUpdates.DST = seconds;
      pendingUpdates.DSTPending = true;
    }

    uint16_t getDSTOffset(){
      return _DSTOffsetSecs;
    }

    /*
     * Sets the time with a UTC timestamp and offsets, and writes changes to RTC chip
     * @param time UTC timestamp in seconds
     * @param timezone offset in seconds
     * @param DST offset in seconds
     * @return if the write was successful
     */
    bool setUTCTimestamp(uint32_t time, int32_t timezone, uint16_t dst){
      setTimezoneOffset(timezone);
      setDSTOffset(dst);
      setUTCTimestamp(time);
      return commitUpdates();
    }
    
    /*
     * Sets the time using a UTC timezone
     * @param time the UTC timestamp in seconds
     * @note the timezone and dst offsets must be set BEFORE calling this function
     * @return if the write was successful
     */
    bool setUTCTimestamp(uint32_t time){
      pendingUpdates.timestamp = time;
      pendingUpdates.timestampPending = true;
      return commitUpdates();
    }

    /*
     * sends pending updates to the RTC and saves the offsets to file.
     * Resets pending updates buffer on success.
     * @returns if commit was successful
     */
    bool commitUpdates(){
      uint32_t timestamp = (pendingUpdates.timestampPending) ? pendingUpdates.timestamp : getUTCTimestamp();  // set a new time or update the current one
      timestamp += (pendingUpdates.DSTPending) ? pendingUpdates.DST : _DSTOffsetSecs;                         // add the new DST offset or use the current one
      timestamp += (pendingUpdates.timezonePending) ? pendingUpdates.timezone : _timeZoneSecs;                // add the new timezone offset or use the current one
      bool res = updateLocalTimestamp(timestamp + _timeZoneSecs + _DSTOffsetSecs);
      if(res){
        if(pendingUpdates.DSTPending){ _DSTOffsetSecs = pendingUpdates.DST; }
        if(pendingUpdates.timezonePending){ _timeZoneSecs = pendingUpdates.timezonePending; }
      }
      resetPendingUpdates();
      return res;
    }

    /*
    * convert a binary-coded decimal into a regular binary integer
    */
    uint8_t bcd2bin (uint8_t val) { return val - 6 * (val >> 4); }

    /*
     * convets a regular integer to binary-coded decimal
     */
    uint8_t decToBcd(uint8_t val) { return ( ((val/10) << 4) + (val%10) ); }
    
    /*
     * breaks a 2000 epoch timestamp into datetime values
     * @param time local timestamp in seconds
     */
    void convertLocalTimestamp(uint32_t time){
      datetime.readReady = false;
      resetDatetime();
      datetime.seconds = time % 60;   // i.e. divide by number of minutes and take remainder
      time /= 60;             // time is now in minutes
      datetime.minutes = time % 60;   // i.e. divide by number of hours and take remainder
      time /= 60;             // time is now in hours
      datetime.hours = time % 24;     // i.e. divide by number of days and take remainder
      time /= 24;             // time is now in days since 1/1/2000
      datetime.dayOfWeek = time % 7;
      datetime.dayOfWeek += 7 * !datetime.dayOfWeek;  // if day is 0, set it to 7
      datetime.years = ((4 * time) / 1461);
      time -= (datetime.years * 365.25) - 1;   // time is now in day of the year
                                                // +1 because day of the year isn't zero-indexed

      int leapDay = (!(datetime.years % 4) && (time > 59)); // knock of a day if its a leap year and after Feb 29
      time -= leapDay;
      int i = 0;
      while (time > monthDays[i]){
          time -= monthDays[i];
          i++;
      }
      datetime.date = time + leapDay;
      datetime.month = i + 1;
    }

    /*
     * resets all stored values before performing read/write operations
     */
    void resetDatetime(){
      datetime.date = 0;
      datetime.dayOfWeek = 0;
      datetime.hours = 0;
      datetime.minutes = 0;
      datetime.month = 0;
      datetime.seconds = 0;
      datetime.years = 0;
    }

#ifdef USE_BCD_TIME
    BCDTimeStruct getBCDTime(){
      BCDTime.readReady = false;
      transmitByte(0);

      Wire.requestFrom(CLOCK_ADDRESS, 3);
      uint8_t seconds = Wire.read();
      uint8_t minutes = Wire.read();
      uint8_t hours = Wire.read();

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
      BCDTime.hours_10 = (hours >> 4) & 1;
      BCDTime.readReady = true;
      return BCDTime;
    }

    BCDDateStruct getBCDDate(){
      BCDDate.readReady = false;
      transmitByte(0x03);

      Wire.requestFrom(CLOCK_ADDRESS, 4);
      uint8_t dayOfWeek = Wire.read();
      uint8_t date = Wire.read();
      uint8_t month = Wire.read() & 0b01111111; // we don't need the century bit
      uint8_t year = Wire.read();
      
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

  private:
    int32_t _timeZoneSecs = 0;    // timezone offset in seconds
    uint16_t _DSTOffsetSecs = 0;  // daylight savings offset in seconds

    /*
     * Fetchs the time from the RTC chipset, and unpacks it into a datetime object
     */
    void fetchDatetime(){
      resetDatetime();
      transmitByte(0);

      Wire.requestFrom(CLOCK_ADDRESS, 7);
      datetime.seconds = bcd2bin(Wire.read());
      datetime.minutes = bcd2bin(Wire.read());
      datetime.hours = bcd2bin(Wire.read() & 0b00011111);
      datetime.dayOfWeek = bcd2bin(Wire.read());
      datetime.date = bcd2bin(Wire.read());
      datetime.month = bcd2bin(Wire.read() & 0b00011111);
      datetime.years = bcd2bin(Wire.read()); // years since midnight 2000
    }

    uint32_t localTimestamp;
    PendingUpdatesStruct pendingUpdates;

    /*
     * resets pending updates after updates have been made
     */
    void resetPendingUpdates(){
      pendingUpdates.timezonePending = 0;
      pendingUpdates.timezone = 0;
      pendingUpdates.DSTPending = 0;
      pendingUpdates.DST = 0;
      pendingUpdates.timestampPending = 0;
      pendingUpdates.timestamp = 0;
    }

    /*
     * Writes a local timestamp to the RTC chip.
     * @param time local timestamp in seconds
     * @return if operation was performed without any errors
     */
    bool updateLocalTimestamp(uint32_t time){
      bool anyErrors = 0;
      convertLocalTimestamp(time);
      anyErrors |= !transmit2Bytes(0x00, decToBcd(datetime.seconds));
      anyErrors |= !transmit2Bytes(0x01, decToBcd(datetime.minutes));
      anyErrors |= !transmit2Bytes(0x02, decToBcd(datetime.hours));
      anyErrors |= !transmit2Bytes(0x03, datetime.dayOfWeek);
      anyErrors |= !transmit2Bytes(0x04, decToBcd(datetime.date));
      anyErrors |= !transmit2Bytes(0x05, decToBcd(datetime.month));
      anyErrors |= !transmit2Bytes(0x06, decToBcd(datetime.years));

      return !anyErrors;
    }

    /*
     * sets the rtc to use 24 hour time
     * NOTE: as it sets the rtc to the same time it just read,
     * there's a non-zero chance the hour could change and set
     * the time back by up to a minute
     */
    void setTo24hr(){
      // TODO: there should be an internal check to make sure time doesn't go backwards
      uint8_t temp_buffer;
      transmitByte(0x02);
      Wire.requestFrom(CLOCK_ADDRESS, 1);
      temp_buffer = Wire.read();

      if(temp_buffer & 0b01000000){
        // if set to 12 hour time
        temp_buffer = temp_buffer & 0b10111111;
        transmit2Bytes(0x02, temp_buffer);
      }
    }

    /*
     * transmits a single byte, AKA the register address to be read from
     * note: device address is set by CLOCK_ADDRESS
     * @return if operation was performed without any errors
     */
    bool transmitByte(uint8_t registerAddress){
      Wire.beginTransmission(CLOCK_ADDRESS);
      Wire.write(registerAddress);
      if(Wire.endTransmission() != 0){
        return false;
      }
      return true;
    }

    /*
     * transmits 2 byte packets, AKA the register address and the value
     * note: device address is set by CLOCK_ADDRESS
     * @return if operation was performed without any errors
     */
    bool transmit2Bytes(uint8_t registerAddress, uint8_t value){
      Wire.beginTransmission(CLOCK_ADDRESS);
      Wire.write(registerAddress);
      Wire.write(value);
      if(Wire.endTransmission() != 0){
        return false;
      }
      return true;
    }
};

// extern RTCInterfaceClass RTCInterface;
#endif
