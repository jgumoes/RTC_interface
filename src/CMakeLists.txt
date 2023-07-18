add_library(RTC_interface RTC_interface.cpp)



target_include_directories(RTC_interface PUBLIC
  "${PROJECT_SOURCE_DIR}/include"
  "${PROJECT_SOURCE_DIR}/src"
  )