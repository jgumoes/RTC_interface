#ifndef MOCKWIRE_H
#define MOCKWIRE_H

#include "testing_types.h"

class MockWire
{
public:
/*
 * instantiate the class with pre-set time values.
 * call convertToBcd() to convert them to the format
 * expected from DS3231
 */
  MockWire(uint8_t seconds, uint8_t minutes, uint8_t hours,
      uint8_t day, uint8_t date, uint8_t month, uint8_t years);

  /*
   * instantiate the class without any pre-set values
   */
  MockWire();
  // uint8_t seconds;
  // uint8_t minutes;
  // uint8_t hours;
  // uint8_t day;      // day of the week
  // uint8_t date;
  // uint8_t month;
  // uint8_t years;

  uint8_t buffer[13];
  uint8_t bytesToRead;
  uint8_t readSize;
  uint8_t readIndex;

  bool wireAddressSet = false;
  bool registerAddressSet = false;

  // void convertToBcd();

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