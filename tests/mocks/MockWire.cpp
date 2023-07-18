#include "MockWire.h"

MockWire::MockWire(uint8_t seconds, uint8_t minutes, uint8_t hours,
    uint8_t day, uint8_t date, uint8_t month, uint8_t years)
{
  buffer[0] = seconds;
  buffer[1] = minutes;
  buffer[2] = hours;
  buffer[3] = day;
  buffer[4] = date;
  buffer[5] = month;
  buffer[6] = years;
}

MockWire::MockWire(){ MockWire(0, 0, 0, 0, 0, 0, 0); }

// byte decToBcd(byte val) { return ( ((val/10) << 4) + (val%10) ); }

// void MockWire::convertToBcd(){
//   seconds = decToBcd(seconds);
//   minutes = decToBcd(minutes);
//   hours = decToBcd(hours);
//   day = decToBcd(day);
//   date = decToBcd(date);
//   month = decToBcd(month);
//   years = decToBcd(years);
// }

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