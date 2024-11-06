/*
 * ds18b20.c
 *
 *  Created on: 22 sie 2024
 *      Author: Mikolaj
 */

#include <string.h>
#include "ds18b20.h"
#include "wire.h"

#define DS18B20_SCRATCHPAD_SIZE    9

#define DS18B20_READ_ROM           0x33
#define DS18B20_MATCH_ROM          0x55 //used if address is necessary eg. if there are multiple sensors
#define DS18B20_SKIP_ROM           0xCC //used when address can be skipped

#define DS18B20_CONVERT_T          0x44
#define DS18B20_READ_SCRATCHPAD    0xBE

HAL_StatusTypeDef ds18b20_init(void)
{
  return wire_init();
}

HAL_StatusTypeDef ds18b20_read_address(uint8_t* rom_code)
{
  int i;
  uint8_t crc;

  if (wire_reset() != HAL_OK) // Reset 1 Wire
  {
    return HAL_ERROR;
  }

  wire_write(DS18B20_READ_ROM); //Sends READ ROM command

  for (i = 0; i < DS18B20_ROM_CODE_SIZE; i++) //Reads the ROM from the device
  {
    rom_code[i] = wire_read();
  }

  crc = wire_crc(rom_code, DS18B20_ROM_CODE_SIZE - 1); //Checks CRC
  if (rom_code[DS18B20_ROM_CODE_SIZE - 1] == crc)
  {
    return HAL_OK;
  }
  else
  {
    return HAL_ERROR;
  }
}

//Sends cmd and skip rom or match rom before the cmd
static HAL_StatusTypeDef send_cmd(const uint8_t* rom_code, uint8_t cmd)
{
  int i;

  if (wire_reset() != HAL_OK) {return HAL_ERROR;}

  if (!rom_code) //if there is no ROM CODE, that is address is not used
  {
    wire_write(DS18B20_SKIP_ROM);
  }
  else	//Address is used
  {
    wire_write(DS18B20_MATCH_ROM); //Sends ROM CODE before the command
    for (i = 0; i < DS18B20_ROM_CODE_SIZE; i++)
    {
      wire_write(rom_code[i]);
    }
  }
  wire_write(cmd); //Sends the actual command
  return HAL_OK;
}

//Starts measurement
HAL_StatusTypeDef ds18b20_start_measure(const uint8_t* rom_code)
{
  return send_cmd(rom_code, DS18B20_CONVERT_T);
}


//Reads measurement data
static HAL_StatusTypeDef ds18b20_read_scratchpad(const uint8_t* rom_code, uint8_t* scratchpad)
{
  int i;
  uint8_t crc;

  if (send_cmd(rom_code, DS18B20_READ_SCRATCHPAD) != HAL_OK)
  {
    return HAL_ERROR;
  }

  for (i = 0; i < DS18B20_SCRATCHPAD_SIZE; i++) //read and write data to the buffer
  {
    scratchpad[i] = wire_read();
  }

  crc = wire_crc(scratchpad, DS18B20_SCRATCHPAD_SIZE - 1); //checks CRC

  if (scratchpad[DS18B20_SCRATCHPAD_SIZE - 1] == crc)
  {
    return HAL_OK;
  }
  else
  {
    return HAL_ERROR;
  }
}

//Reads temperature, basically combines the functions above
float ds18b20_get_temp(const uint8_t* rom_code)
{
  uint8_t scratchpad[DS18B20_SCRATCHPAD_SIZE];
  int16_t temp;

  if (ds18b20_read_scratchpad(rom_code, scratchpad) != HAL_OK)
  {
	  return 85.0f; //default value - the measurement failed
  }
  else
  {
	  //copies sizeof(temp) bytes from the scratchpad and merge it into "temp"
	  memcpy(&temp, &scratchpad[0], sizeof(temp));

	  return temp / 16.0f; //divide by 16 to get the actual temperature value
  }
}



