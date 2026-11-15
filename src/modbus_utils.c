#include "modbus_utils.h"

uint8_t crc16(volatile uint8_t *ptrToArray, uint8_t inputSize) {
  uint16_t out = 0xFFFF;
  uint16_t carry;
  unsigned char n;

  inputSize++;

  for (int l = 0; l < inputSize; l++) {
    out ^= ptrToArray[l];

    for (n = 0; n < 8; n++) {
      carry = out & 1;
      out >>= 1;

      if (carry) {
        out ^= 0xA001;
      }
    }
  }

  if ((ptrToArray[inputSize] == out % 256) &&
      (ptrToArray[inputSize + 1] == out / 256)) {
    return 1;
  } else {
    ptrToArray[inputSize] = out % 256;
    ptrToArray[inputSize + 1] = out / 256;

    return 0;
  }
}

void intToModbusRegister(volatile uint16_t *inreg, volatile uint8_t *outreg,
                         uint8_t amount) {
  for (uint8_t c = 0; c < amount; c++) {
    *(outreg + c * 2) = (uint8_t)(*(inreg + c) >> 8);

    *(outreg + 1 + c * 2) = (uint8_t)(*(inreg + c));
  }
}

void modbusRegisterToInt(volatile uint8_t *inreg, volatile uint16_t *outreg,
                         uint8_t amount) {
  for (uint8_t c = 0; c < amount; c++) {
    *(outreg + c) = (*(inreg + c * 2) << 8) + *(inreg + 1 + c * 2);
  }
}