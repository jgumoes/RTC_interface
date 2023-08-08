/*
 * The ZS-042 module is a DS3231 breakout with on-board 24C32 eeprom chip.
 * This example includes a configManager that stores the offsets on the eeprom chip.
 * You can set and read the eeprom values over serial.
 */
#include <Arduino.h>
#include <Wire.h>

#include "RTC_interface.h"
#include "configManager.h"

ConfigManagerClass ConfigManager;
RTCInterfaceClass<TwoWire, ConfigManagerClass> RTC = RTCInterfaceClass<TwoWire, ConfigManagerClass>(Wire, ConfigManager);

const unsigned long timeInterval = 1000;
unsigned long timerValue = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("Starting sketch...");
  Wire.begin();
  Wire.setClock(400000);
  RTC.begin();
  RTC.setUTCTimestamp(744845856);
  Serial.println("setup complete");
}

void readSerial(){
  Serial.println("reading serial input:");
  String buffer = Serial.readStringUntil('\n');
  Serial.print("recieved buffer: "); Serial.println(buffer);
  int splitIndex = buffer.indexOf(':');

  String instruction = buffer.substring(0, splitIndex);
  instruction.toLowerCase();
  Serial.print("recieved instruction: "); Serial.println(instruction);

  if(splitIndex == -1){
    if(instruction == "read configs" || instruction == "configs"){
      Serial.println("eeprom configs: ");
      ConfigManager.getRTCConfigs();
      Serial.println("RTC configs: ");
      Serial.print("\tDST: "); Serial.println(RTC.getDSTOffset());
      Serial.print("\ttimezone: "); Serial.println(RTC.getTimezoneOffset());
    }
    return;
  }

  String svalue = buffer.substring(splitIndex + 1);
  svalue.trim();
  Serial.print("value (string):"); Serial.print(svalue); Serial.println(";");
  
  if(instruction == "timezone" || instruction == "tz"){
    int32_t ivalue = svalue.toInt();
    RTC.setTimezoneOffset(ivalue);
    Serial.print("recieved value: "); Serial.println(ivalue);
  }
  else if (instruction == "dst")
  {
    uint16_t ivalue = svalue.toInt();
    RTC.setDSTOffset(ivalue);
    Serial.print("recieved value: "); Serial.println(ivalue);
  }
  else if (instruction == "time" || instruction == "timestamp" || instruction == "local timestamp" || instruction == "local")
  {
    uint32_t ivalue = svalue.toInt();
    RTC.setLocalTimestamp(ivalue);
    Serial.print("recieved value: "); Serial.println(ivalue);
  }
  else if (instruction == "utc time" || instruction == "utc timestamp" || instruction == "utc")
  {
    uint32_t ivalue = svalue.toInt();
    RTC.setUTCTimestamp(ivalue);
    Serial.print("recieved value: "); Serial.println(ivalue);
  }
  else {
    return;
  }
  RTC.commitUpdates();
  Serial.println("read complete");
}

void loop() {
  if (millis() - timerValue > timeInterval){
    Serial.print("current timestamp: "); Serial.println(RTC.getLocalTimestamp());
    timerValue = millis();
  }

  if (Serial.available() > 0) {
    readSerial();
  }
  delay(50);
}
