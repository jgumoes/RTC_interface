add_library(MockWire MockWire.cpp)

target_include_directories(MockWire PUBLIC
  "${PROJECT_SOURCE_DIR}/tests/mocks"
  )