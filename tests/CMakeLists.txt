find_package(GTest REQUIRED)

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

include(GoogleTest)

gtest_discover_tests(
    thermistor_tests
)