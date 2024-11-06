/*
 * ds18b20.h
 *
 *  Created on: 22 sie 2024
 *      Author: Mikolaj
 */

#ifndef INC_DS18B20_H_
#define INC_DS18B20_H_

#include "stm32l4xx.h"

#define DS18B20_ROM_CODE_SIZE		8

//This library presumes that Timer 6 is set to 1MHz, to change used timer modify .c file
//You can get 1MHz eg. by setting HSI to 80MHz and prescaler to 79 (tested on Nucleo)

//This version of code may not work properly if there are too many interrupts at once

// 1-Wire init
// return - HAL_OK/HAL_ERROR
HAL_StatusTypeDef ds18b20_init(void);

//Read address of a sensor and calculate CRC
// rom_code - variable to store read data
// return - HAL_OK/HAL_ERROR
HAL_StatusTypeDef ds18b20_read_address(uint8_t* rom_code);

// Initialize temperature measurement
// rom_code - device address or NULL if not used (eg. when there is only one sensor)
// return - HAL_OK/HAL_ERROR
HAL_StatusTypeDef ds18b20_start_measure(const uint8_t* rom_code);

// Read temperature data
// rom_code - device address or NULL if not used (eg. when there is only one sensor)
// return - temperature in Celsius degrees
float ds18b20_get_temp(const uint8_t* rom_code);

#endif /* INC_DS18B20_H_ */
