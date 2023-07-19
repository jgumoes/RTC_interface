#include "MockWire.h"

MockWire::MockWire(uint8_t seconds, uint8_t minutes, uint8_t hours,
    uint8_t dayOfWeek, uint8_t date, uint8_t month, uint8_t years)
{
  buffer[0] = seconds;
  buffer[1] = minutes;
  buffer[2] = hours;
  buffer[3] = dayOfWeek;
  buffer[4] = date;
  buffer[5] = month;
  buffer[6] = years;
}

MockWire::MockWire(){ MockWire(0, 0, 0, 0, 0, 0, 0); }

/*
 * This method has been copied from RTC_interface, where
 * it has passed its tests.
 */
uint8_t decToBcd(uint8_t val) { return ( ((val/10) << 4) + (val%10) ); }

void MockWire::convertToBcd(){
  buffer[0] = decToBcd(buffer[0]);
  buffer[1] = decToBcd(buffer[1]);
  buffer[2] = decToBcd(buffer[2]);
  buffer[3] = decToBcd(buffer[3]);
  buffer[4] = decToBcd(buffer[4]);
  buffer[5] = decToBcd(buffer[5]);
  buffer[6] = decToBcd(buffer[6]);
}

uint8_t MockWire::requestFrom(int address, int size){
  if(size >= 13){ return false; }
  readSize = size;
  readIndex = 0;
  return true;
}

uint8_t MockWire::read(){
  if(readIndex >= readSize){
    return 0;
  }
  readIndex++;
  return buffer[readIndex-1];
}

void MockWire::write(int value){
  if(!wireAddressSet){ throw "Wire address hasn't been set!"; }
  if(!registerAddressSet){
    // first write() call sets the register address
    registerAddressSet = true;
    readIndex = value;
  }
  else{
    buffer[readIndex] = value;
    readIndex++;
  }
}

void MockWire::beginTransmission(int address){
  wireAddressSet = true;
  readIndex = 0;
}

uint8_t MockWire::endTransmission(){
  wireAddressSet = false;
  registerAddressSet = false;
  readIndex = 0;
  return 0;
}