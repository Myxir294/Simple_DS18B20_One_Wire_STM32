#include "wire.h"
#include "gpio.h"
#include "tim.h"

//This version of the code uses simple bit-banging
//Prons - power-efficient - does not need additional peripherals
//Cons - enforces delays - needs disabling and enabling interrupts with each measurement

//This version of code may break things if there are too many interrupts at once

HAL_StatusTypeDef wire_init(void)
{
  return HAL_TIM_Base_Start(&htim6);
}

void delay_us(uint32_t us) // for delays lower than 1 ms
{
  __HAL_TIM_SET_COUNTER(&htim6, 0); //has to be set to 1MHz
  while (__HAL_TIM_GET_COUNTER(&htim6) < us)
  {
	  //do nothing
  }
}

HAL_StatusTypeDef wire_reset(void)
{
  __disable_irq();

  int slave_answer;

  HAL_GPIO_WritePin(DS_GPIO_Port, DS_Pin, GPIO_PIN_RESET);
  delay_us(480); //Master starts with 480us of low logic state
  HAL_GPIO_WritePin(DS_GPIO_Port, DS_Pin, GPIO_PIN_SET);

  delay_us(70);	 //Then checks state of the bus after 70us
  slave_answer = HAL_GPIO_ReadPin(DS_GPIO_Port, DS_Pin);
  delay_us(410);

  __enable_irq();

  if (slave_answer == 0)
  {
    return HAL_OK;
  }
  else
  {
    return HAL_ERROR;
  }
}

static int read_bit(void)
{
  __disable_irq();
  int slave_answer;
  HAL_GPIO_WritePin(DS_GPIO_Port, DS_Pin, GPIO_PIN_RESET);
  delay_us(6);
  HAL_GPIO_WritePin(DS_GPIO_Port, DS_Pin, GPIO_PIN_SET);
  delay_us(9);
  slave_answer = HAL_GPIO_ReadPin(DS_GPIO_Port, DS_Pin);
  delay_us(55);
  __enable_irq();
  return slave_answer;
}


//reads a byte - merges bits (quite useful)
//starts from LSB to MSB
uint8_t wire_read(void)
{
  uint8_t value = 0;
  int i;
  for (i = 0; i < 8; i++)
  {
	value = value >> 1;  //to start from MSB -> <<
    if (read_bit())
    {
      value = value | 0x80; //to start from MBS -> 0x01
    }
  }
  return value;
}


//Sends a bit
static void write_bit(int value)
{
  __disable_irq();
  if (value)
  {
    HAL_GPIO_WritePin(DS_GPIO_Port, DS_Pin, GPIO_PIN_RESET);
    delay_us(6);
    HAL_GPIO_WritePin(DS_GPIO_Port, DS_Pin, GPIO_PIN_SET);
    delay_us(64);
  }
  else
  {
    HAL_GPIO_WritePin(DS_GPIO_Port, DS_Pin, GPIO_PIN_RESET);
    delay_us(60);
    HAL_GPIO_WritePin(DS_GPIO_Port, DS_Pin, GPIO_PIN_SET);
    delay_us(10);
  }
  __enable_irq();
}


//Sends a byte
void wire_write(uint8_t byte)
{
  int i;
  for (i = 0; i < 8; i++)
  {
    write_bit(byte & 0x01); //bit by bit
    byte = byte >> 1;
  }
}

uint8_t static byte_crc(uint8_t crc, uint8_t byte) //calculates CRC for a byte
{
  int i;
  for (i = 0; i < 8; i++)
  {
    uint8_t b = crc ^ byte;
    crc = crc >> 1;
    if (b & 0x01)
    {
      crc = crc ^ 0x8c;
    }
    byte = byte >> 1;
  }
  return crc;
}

uint8_t wire_crc(const uint8_t* data, int len) //calculates CRC for a data block
{
  int i;
  uint8_t crc = 0;
  for (i = 0; i < len; i++)
  {
	  crc = byte_crc(crc, data[i]);
  }
  return crc;
}
