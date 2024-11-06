#pragma once

#include "stm32l4xx.h"

//This version of the code uses simple bit-banging
//Prons - power-efficient - does not need additional peripherals
//Cons - enforces delays - needs disabling and enabling interrupts with each measurement

//This library presumes that Timer 6 is set to 1MHz, to change used timer modify .c file
//You can get 1MHz eg. by setting HSI to 80MHz and prescaler to 79 (tested on Nucleo)

//This version of code may break things if there are too many interrupts at once

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

//Delay
//Assumes the timer 6 is set to 1MHz
void delay_us(uint32_t us);
