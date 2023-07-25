#include "MockWire.h"
#include "../helpers.h"

MockWire::MockWire(uint8_t seconds, uint8_t minutes, uint8_t hours,
    uint8_t dayOfWeek, uint8_t date, uint8_t month, uint8_t years)
{
  mockBuffer[0] = seconds;
  mockBuffer[1] = minutes;
  mockBuffer[2] = hours;
  mockBuffer[3] = dayOfWeek;
  mockBuffer[4] = date;
  mockBuffer[5] = month;
  mockBuffer[6] = years;

  readSize = 0;
  readIndex = 0;
}

MockWire::MockWire(){ MockWire(0, 0, 0, 0, 0, 0, 0); }

void MockWire::convertToBcd(){
  mockBuffer[0] = AS_BCD(mockBuffer[0]);
  mockBuffer[1] = AS_BCD(mockBuffer[1]);
  mockBuffer[2] = AS_BCD(mockBuffer[2]);
  mockBuffer[3] = AS_BCD(mockBuffer[3]);
  mockBuffer[4] = AS_BCD(mockBuffer[4]);
  mockBuffer[5] = AS_BCD(mockBuffer[5]);
  mockBuffer[6] = AS_BCD(mockBuffer[6]);
}

void MockWire::incrementSeconds()
{
  const uint8_t leapYearAdjust = ((mockBuffer[5] == 2) && (mockBuffer[6] % 4 == 0));
  const uint8_t maxValues[13] = {
    59,
    59,
    23,
    7, //dayOfWeek
    monthDays[AS_DEC(mockBuffer[5]) - 1] + leapYearAdjust,
    12,
    99
  };

  uint8_t index = 0;
  while(incrementValue(index, maxValues[index])){
    index++;
    if(index == 3){
      // its different for day-of-week
      incrementValue(index, maxValues[index]);
      index++;
    }
  }
}

bool MockWire::incrementValue(uint8_t index, uint8_t maxValue)
{
  const uint8_t resetValues[7] = {0,0,0,0,1,1,0};
  uint8_t decValue = AS_DEC(mockBuffer[index]) + 1;
  if(decValue > maxValue){
    mockBuffer[index] = resetValues[index];
    return true;
  }
  mockBuffer[index] = AS_BCD(decValue);
  return false;
}

void MockWire::makeDatetimeBuffer()
{
  for(int i = 0; i < 7; i++){
    datetimeBuffer[i] = mockBuffer[i];
  }
}

uint8_t MockWire::requestFrom(int address, int size){
  if(size >= 13){ return false; }
  readSize = size + readIndex;
  // readIndex = 0;
  return true;
}

uint8_t MockWire::read(){
  if(readIndex >= readSize){
    return 0;
  }
  uint8_t readValue = mockBuffer[readIndex];
  readIndex++;
  readHasBeenCalledTimes++;
  if((readHasBeenCalledTimes == changeBufferAfterRead) && (changeBufferAfterRead > 0)){ COPY_ARRAY(nextMockBuffer, mockBuffer); }
  return readValue;
}

void MockWire::write(int value){
  if(!wireAddressSet){ throw "Wire address hasn't been set!"; }
  writeHasBeenCalledTimes += 1;
  if(!registerAddressSet){
    // first write() call sets the register address
    registerAddressSet = true;
    readIndex = value;
  }
  else{
    writeHasModifiedBuffer = true;
    mockBuffer[readIndex] = value;
    readIndex++;
  }
}

void MockWire::beginTransmission(int address){
  wireAddressSet = true;
  // readIndex = 0;
}

uint8_t MockWire::endTransmission(){
  wireAddressSet = false;
  registerAddressSet = false;
  // readIndex = 0;
  return 0;
}