#ifndef MODBUS_UTILS_H
#define MODBUS_UTILS_H

#include <stdint.h>

uint8_t crc16(volatile uint8_t *ptrToArray, uint8_t inputSize);

void intToModbusRegister(volatile uint16_t *inreg, volatile uint8_t *outreg,
                         uint8_t amount);

void modbusRegisterToInt(volatile uint8_t *inreg, volatile uint16_t *outreg,
                         uint8_t amount);

#endif