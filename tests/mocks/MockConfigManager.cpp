#include "MockConfigManager.h"
#include "../helpers.h"

// MockConfigManager::MockConfigManager(int32_t initialTimezone, uint16_t initialDST)
// {
//   timezone = initialTimezone;
//   DST = initialDST;
// }

RTCConfigsStruct MockConfigManager::getRTCConfigs(){
  RTCConfigsStruct output;
  output.DST = DST;
  output.timezone = timezone;
  return output;
}

bool MockConfigManager::setRTCConfigs(RTCConfigsStruct configs){
  timezone = configs.timezone;
  DST = configs.DST;
  return setConfigResult;
}