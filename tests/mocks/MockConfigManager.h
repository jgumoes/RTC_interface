#ifndef _MOCK_CONFIG_MANAGER_H_
#define _MOCK_CONFIG_MANAGER_H_

#include <stdint.h>
// #include "structs.h"
#include "RTC_interface.h"

class MockConfigManager
{
public:
  int32_t timezone = 0;
  uint16_t DST = 0;

/*
 * initialse class with set values
 */
  MockConfigManager(int32_t timezone, uint16_t DST) : timezone(timezone), DST(DST){};

  MockConfigManager(){
    timezone = 0; DST = 0;
  };

  RTCConfigsStruct getRTCConfigs();
  bool setRTCConfigs(RTCConfigsStruct configs);

  bool setConfigResult = true;
};

#endif