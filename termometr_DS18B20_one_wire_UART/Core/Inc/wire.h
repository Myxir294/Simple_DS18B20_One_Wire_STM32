#pragma once

#include "stm32l4xx.h"

//This version of the code uses UART instead of bit-banging
//Prons - lowers the usage of the CPU, allows for more interrupts, simpler, no delays
//Cons - requires free UART port, may increase power consumption

//Still uses only one wire

// 1-Wire init
// Starts the counter used to count delay values
// return - HAL_OK/HAL_ERROR, depending on the counter state
HAL_StatusTypeDef wire_init(void);

// Sends "reset" command via 1-Wire
// return - HAL_OK/HAL_ERROR, depending on the counter state
HAL_StatusTypeDef wire_reset(void);

// Read data via 1-Wire
// Takes 8 bits and combine them into 1 byte
// return - read byte
uint8_t wire_read(void);

// Send data via 1-Wire
// byte - sent byte
void wire_write(uint8_t byte);

// Calculates CRC
// data - data block to calculate crc from
// len - length of data
uint8_t wire_crc(const uint8_t* data, int len);
