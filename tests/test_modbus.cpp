#include <gtest/gtest.h>

extern "C" {
#include "../src/modbus_utils.h"
}

TEST(Modbus, RegisterToBytesSingle) {
  uint16_t input[1] = {0x1234};
  uint8_t output[2] = {};

  intToModbusRegister(input, output, 1);

  EXPECT_EQ(output[0], 0x12);
  EXPECT_EQ(output[1], 0x34);
}

TEST(Modbus, RegisterToBytesMultiple) {
  uint16_t input[3] = {0x1234, 0xABCD, 0x0001};

  uint8_t output[6] = {};

  intToModbusRegister(input, output, 3);

  EXPECT_EQ(output[0], 0x12);
  EXPECT_EQ(output[1], 0x34);

  EXPECT_EQ(output[2], 0xAB);
  EXPECT_EQ(output[3], 0xCD);

  EXPECT_EQ(output[4], 0x00);
  EXPECT_EQ(output[5], 0x01);
}

TEST(Modbus, BytesToRegisterSingle) {
  uint8_t input[2] = {0x12, 0x34};

  uint16_t output[1] = {};

  modbusRegisterToInt(input, output, 1);

  EXPECT_EQ(output[0], 0x1234);
}

TEST(Modbus, BytesToRegisterMultiple) {
  uint8_t input[6] = {0x12, 0x34, 0xAB, 0xCD, 0x00, 0x01};

  uint16_t output[3] = {};

  modbusRegisterToInt(input, output, 3);

  EXPECT_EQ(output[0], 0x1234);
  EXPECT_EQ(output[1], 0xABCD);
  EXPECT_EQ(output[2], 0x0001);
}

TEST(Modbus, RoundTrip) {
  uint16_t source[4] = {0x0000, 0x1234, 0xABCD, 0xFFFF};

  uint8_t bytes[8] = {};
  uint16_t result[4] = {};

  intToModbusRegister(source, bytes, 4);

  modbusRegisterToInt(bytes, result, 4);

  for (int i = 0; i < 4; i++) {
    EXPECT_EQ(source[i], result[i]);
  }
}

TEST(Modbus, CRCGeneration) {
  uint8_t frame[8] = {0x01, 0x03, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00};

  EXPECT_EQ(crc16(frame, 5), 0);

  EXPECT_EQ(frame[6], 0x84);
  EXPECT_EQ(frame[7], 0x0A);
}

TEST(Modbus, CRCValidation) {
  uint8_t frame[8] = {0x01, 0x03, 0x00, 0x00, 0x00, 0x01, 0x84, 0x0A};

  EXPECT_EQ(crc16(frame, 5), 1);
}

TEST(Modbus, CRCCorruption) {
  uint8_t frame[8] = {0x01, 0x03, 0x00, 0x00, 0x00, 0x02, 0x84, 0x0A};

  EXPECT_EQ(crc16(frame, 5), 0);
}

TEST(Modbus, ZeroRegister) {
  uint16_t input[1] = {0};

  uint8_t bytes[2];

  intToModbusRegister(input, bytes, 1);

  EXPECT_EQ(bytes[0], 0);
  EXPECT_EQ(bytes[1], 0);
}

TEST(Modbus, MaxRegister) {
  uint16_t input[1] = {0xFFFF};

  uint8_t bytes[2];

  intToModbusRegister(input, bytes, 1);

  EXPECT_EQ(bytes[0], 0xFF);
  EXPECT_EQ(bytes[1], 0xFF);
}

TEST(Modbus, RoundTripManyValues) {
  for (uint32_t value = 0; value <= 0xFFFF; value += 137) {
    uint16_t in[1] = {static_cast<uint16_t>(value)};

    uint8_t bytes[2];
    uint16_t out[1];

    intToModbusRegister(in, bytes, 1);
    modbusRegisterToInt(bytes, out, 1);

    EXPECT_EQ(in[0], out[0]);
  }
}

TEST(Modbus, CRCAppendThenValidate) {
  uint8_t frame[8] = {0x01, 0x03, 0x00, 0x00, 0x00, 0x01, 0, 0};

  EXPECT_EQ(crc16(frame, 5), 0);

  EXPECT_EQ(crc16(frame, 5), 1);
}

TEST(Modbus, Endianness) {
  uint16_t value = 0x55AA;
  uint8_t bytes[2];

  intToModbusRegister(&value, bytes, 1);

  EXPECT_EQ(bytes[0], 0x55);
  EXPECT_EQ(bytes[1], 0xAA);
}

TEST(Modbus, CRCDetectsSingleBitErrors) {
  uint8_t frame[8] = {0x01, 0x03, 0x00, 0x00, 0x00, 0x01, 0x84, 0x0A};

  for (int byte = 0; byte < 6; byte++) {
    for (int bit = 0; bit < 8; bit++) {
      uint8_t copy[8];

      memcpy(copy, frame, sizeof(frame));

      copy[byte] ^= (1 << bit);

      EXPECT_EQ(crc16(copy, 5), 0);
    }
  }
}
