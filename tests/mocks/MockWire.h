#ifndef MOCKWIRE_H
#define MOCKWIRE_H

#include <stdint.h>

class MockWire
{
public:
/*
 * instantiate the class with pre-set time values.
 * call convertToBcd() to convert them to the format
 * expected from DS3231
 */
  MockWire(uint8_t seconds, uint8_t minutes, uint8_t hours,
      uint8_t dayOfWeek, uint8_t date, uint8_t month, uint8_t years);

  /*
   * instantiate the class without any pre-set values
   */
  MockWire();

  uint8_t mockBuffer[13];
  uint8_t bytesToRead;
  uint8_t readSize;
  uint8_t readIndex;

  uint8_t writeHasBeenCalledTimes = 0;
  bool writeHasModifiedBuffer = false;
  uint8_t readHasBeenCalledTimes = 0;
  uint8_t changeBufferAfterRead = 0;  // change the buffer before the nth time read has been called. i.e. =0 won't change the buffer, =1 will change the buffer after the first time read() is called, 2 for the 2nd time, etc.

  uint8_t nextMockBuffer[13];
  
  bool wireAddressSet = false;
  bool registerAddressSet = false;

  void convertToBcd();

  const uint8_t monthDays[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
  /*
   * Advance the buffer by one second
   */
  void incrementSeconds();
  /*
   * increment a value in the buffer.
   * Example: for seconds, .incrementValue(0, 60);
   * @param index the index in the buffer to increment
   * @param maxValue buffer value ticks over if this value is reached
   * @return true if the value ticks over
   */
  bool incrementValue(uint8_t index, uint8_t maxValue);

  uint8_t datetimeBuffer[7];
  void makeDatetimeBuffer();



  /*
   * returns true if size is less than 13, otherwise false
   */
  uint8_t requestFrom(int address, int size);
  uint8_t read();
  void write(int value);

  void beginTransmission(int address);
  uint8_t endTransmission();
  
};

#endif