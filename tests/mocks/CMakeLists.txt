add_library(MockWire MockWire.cpp)
add_library(MockConfigManager MockConfigManager.cpp)

target_include_directories(MockWire PUBLIC
  "${PROJECT_SOURCE_DIR}/tests/mocks"
)

target_include_directories(MockConfigManager PUBLIC
  "${PROJECT_SOURCE_DIR}/tests/mocks"
)

#  MockConfigManager uses structs from RTC_interface as a type,
# this line lets MockConfigManager know that RTC_interface exists
target_link_libraries(MockConfigManager RTC_interface)
