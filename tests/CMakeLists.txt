

add_subdirectory("${PROJECT_SOURCE_DIR}/googletest" "googletest")
add_subdirectory("${PROJECT_SOURCE_DIR}/tests/mocks" "MockWire")
include_directories(MockWire PUBLIC "${PROJECT_SOURCE_DIR}/tests/mocks")
link_directories("${PROJECT_SOURCE_DIR}/tests/mocks")

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# macro copied from https://cliutils.gitlab.io/modern-cmake/chapters/testing/googletest.html
macro(package_add_test TESTNAME)
    # create an executable in which the tests will be stored
    add_executable(${TESTNAME} ${ARGN})
    # link the Google test infrastructure, mocking library, and a default main function to
    # the test executable.  Remove g_test_main if writing your own main function.
    target_link_libraries(${TESTNAME} MockWire MockConfigManager gtest gmock gtest_main RTC_interface)
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

package_add_test(
  RTC_interface_tests
  RTC_interface_tests.cpp
)

package_add_test(
  BCD_conversion_tests
  BCD_conversion_tests.cpp
)

package_add_test(
  getTime_tests
  getTime_tests.cpp
)

package_add_test(
  setTime_tests
  setTime_tests.cpp
)

package_add_test(
  Mock_tests
  Mock_tests.cpp
)