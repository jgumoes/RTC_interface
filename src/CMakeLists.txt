message(STATUS "BUILD_TESTS is switched ${BUILD_TESTS}")
if(BUILD_TESTS)
  add_compile_definitions(USE_TESTING_TYPES)
endif()

add_library(RTC_interface RTC_interface.cpp)

target_include_directories(RTC_interface PUBLIC
  "${PROJECT_SOURCE_DIR}/include"
  "${PROJECT_SOURCE_DIR}/src"
  )

if(BUILD_TESTS)
  target_include_directories(RTC_interface PUBLIC "${PROJECT_SOURCE_DIR}/tests/mocks")
  endif()
