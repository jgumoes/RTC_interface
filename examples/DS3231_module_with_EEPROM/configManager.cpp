#include <Wire.h>
#include <Arduino.h>
#include "configManager.h"

#define EEPROM_ADDRESS 0x57

#define CONFIG_ADDRESS 501

const uint8_t* rtcConfigsAddress = reinterpret_cast<uint8_t*>(CONFIG_ADDRESS);

ConfigManagerClass::ConfigManagerClass(/* args */)
{
}


RTCConfigsStruct ConfigManagerClass::getRTCConfigs(){
  RTCConfigsStruct configs;
  Wire.beginTransmission(EEPROM_ADDRESS);
  Wire.write(rtcConfigsAddress[0]);
  Wire.write(rtcConfigsAddress[1]);
  uint8_t status = Wire.endTransmission();
  Wire.requestFrom(EEPROM_ADDRESS, sizeof(configs));
  Wire.readBytes(reinterpret_cast<uint8_t*>(&configs), sizeof(configs));

  Serial.print("\tread status: "); Serial.println(status);
  Serial.print("\tread DST: "); Serial.println(configs.DST);
  Serial.print("\tread timezone: "); Serial.println(configs.timezone);
  return configs;
}
  
uint8_t ConfigManagerClass::setRTCConfigs(RTCConfigsStruct configs){
  Wire.beginTransmission(EEPROM_ADDRESS);
  Wire.write(rtcConfigsAddress[0]);
  Wire.write(rtcConfigsAddress[1]);
  Wire.write(reinterpret_cast<uint8_t*>(&configs), sizeof(configs));
  uint8_t status = Wire.endTransmission();
  Serial.print("write status: "); Serial.println(status);
  return true;
}