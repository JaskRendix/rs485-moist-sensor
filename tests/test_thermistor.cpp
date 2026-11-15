#include <gtest/gtest.h>

extern "C" {
#include "../src/thermistor.h"
}

TEST(Thermistor, ClampHot) {
  EXPECT_EQ(thermistorLsbToTemperature(0), 850);

  EXPECT_EQ(thermistorLsbToTemperature(129), 850);
}

TEST(Thermistor, ClampCold) {
  EXPECT_EQ(thermistorLsbToTemperature(1023), -200);

  EXPECT_EQ(thermistorLsbToTemperature(892), -200);
}

TEST(Thermistor, ExactTablePoints) {
  EXPECT_EQ(thermistorLsbToTemperature(129), 850);
  EXPECT_EQ(thermistorLsbToTemperature(146), 800);
  EXPECT_EQ(thermistorLsbToTemperature(165), 750);
  EXPECT_EQ(thermistorLsbToTemperature(186), 700);
  EXPECT_EQ(thermistorLsbToTemperature(210), 650);
  EXPECT_EQ(thermistorLsbToTemperature(236), 600);
  EXPECT_EQ(thermistorLsbToTemperature(267), 550);
  EXPECT_EQ(thermistorLsbToTemperature(300), 500);
  EXPECT_EQ(thermistorLsbToTemperature(337), 450);
  EXPECT_EQ(thermistorLsbToTemperature(376), 400);
  EXPECT_EQ(thermistorLsbToTemperature(419), 350);
  EXPECT_EQ(thermistorLsbToTemperature(464), 300);
  EXPECT_EQ(thermistorLsbToTemperature(511), 251);
  EXPECT_EQ(thermistorLsbToTemperature(559), 201);
  EXPECT_EQ(thermistorLsbToTemperature(608), 150);
  EXPECT_EQ(thermistorLsbToTemperature(656), 100);
  EXPECT_EQ(thermistorLsbToTemperature(703), 50);
  EXPECT_EQ(thermistorLsbToTemperature(748), 0);
  EXPECT_EQ(thermistorLsbToTemperature(789), -50);
  EXPECT_EQ(thermistorLsbToTemperature(828), -100);
  EXPECT_EQ(thermistorLsbToTemperature(862), -150);
  EXPECT_EQ(thermistorLsbToTemperature(892), -200);
}

TEST(Thermistor, Monotonic) {
  long previous = thermistorLsbToTemperature(0);

  for (int adc = 1; adc < 1024; adc++) {
    long current = thermistorLsbToTemperature(adc);

    EXPECT_LE(current, previous) << "ADC=" << adc;

    previous = current;
  }
}

TEST(Thermistor, Around25CPoint) {
  EXPECT_GE(thermistorLsbToTemperature(510), thermistorLsbToTemperature(511));

  EXPECT_GE(thermistorLsbToTemperature(511), thermistorLsbToTemperature(512));
}

TEST(Thermistor, NeverExceedsRange) {
  for (int adc = 0; adc < 1024; adc++) {
    long t = thermistorLsbToTemperature(adc);

    EXPECT_LE(t, 850);
    EXPECT_GE(t, -200);
  }
}

TEST(Thermistor, FullDomain) {
  for (int adc = 0; adc < 1024; adc++) {
    ASSERT_NO_FATAL_FAILURE(thermistorLsbToTemperature(adc));
  }
}
