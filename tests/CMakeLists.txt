# include(FetchContent)
# FetchContent_Declare(
#   googletest
#   URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
# )

add_subdirectory("${PROJECT_SOURCE_DIR}/googletest" "googletest")
add_subdirectory("${PROJECT_SOURCE_DIR}/tests/mocks" "MockWire")
include_directories(MockWire PUBLIC "${PROJECT_SOURCE_DIR}/tests/mocks")
link_directories("${PROJECT_SOURCE_DIR}/tests/mocks")

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
# FetchContent_MakeAvailable(googletest)

# add_executable(
#   RTC_interface_tests
#   RTC_interface_tests.cpp
# )

# target_link_libraries(
#   RTC_interface_tests
#   RTC_interface
#   GTest::gtest_main
# )

macro(package_add_test TESTNAME)
    # create an executable in which the tests will be stored
    add_executable(${TESTNAME} ${ARGN})
    # link the Google test infrastructure, mocking library, and a default main function to
    # the test executable.  Remove g_test_main if writing your own main function.
    target_link_libraries(${TESTNAME} MockWire gtest gmock gtest_main RTC_interface)
    # gtest_discover_tests replaces gtest_add_tests,
    # see https://cmake.org/cmake/help/v3.10/module/GoogleTest.html for more options to pass to it
    gtest_discover_tests(${TESTNAME}
        # set a working directory so your project root so that you can find test data via paths relative to the project root
        WORKING_DIRECTORY ${PROJECT_DIR}
        PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_DIR}"
    )
    set_target_properties(${TESTNAME} PROPERTIES FOLDER tests)
endmacro()

include(GoogleTest)
# gtest_discover_tests(RTC_interface_tests)

package_add_test(
  RTC_interface_tests
  RTC_interface_tests.cpp
)

package_add_test(
  BCD_conversion_tests
  BCD_conversion_tests.cpp
)

