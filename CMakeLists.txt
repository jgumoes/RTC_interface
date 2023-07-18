cmake_minimum_required(VERSION 3.14)
project(my_project)

# GoogleTest requires at least C++14
# platformio requires at least C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

enable_testing()
include(Googletest)
add_subdirectory(tests)
add_subdirectory(src)