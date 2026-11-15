find_package(GTest REQUIRED)

include(GoogleTest)

add_executable(
    thermistor_tests
    test_thermistor.cpp
    ../src/thermistor.c
)

target_include_directories(
    thermistor_tests
    PRIVATE
    ../src
)

target_link_libraries(
    thermistor_tests
    GTest::gtest
    GTest::gtest_main
)

gtest_discover_tests(thermistor_tests)

add_executable(
    modbus_tests
    test_modbus.cpp
    ../src/modbus_utils.c
)

target_include_directories(
    modbus_tests
    PRIVATE
    ../src
)

target_link_libraries(
    modbus_tests
    GTest::gtest
    GTest::gtest_main
)

gtest_discover_tests(modbus_tests)