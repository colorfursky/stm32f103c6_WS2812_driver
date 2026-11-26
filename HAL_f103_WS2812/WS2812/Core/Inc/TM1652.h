#ifndef __TM1652_H__
#define __TM1652_H__

#include "main.h"
#include "stm32f1xx_hal_uart.h"

#define ADDRESS_DISPLAY 0x18

#define NUMBER_1 0x06   // 0b00000110
#define NUMBER_2 0x5B   // 0b01011011
#define NUMBER_3 0x4F   // 0b01001111
#define NUMBER_4 0x66   // 0b01100110
#define NUMBER_5 0x6D   // 0b01101101
#define NUMBER_6 0x7D   // 0b01111101
#define NUMBER_7 0x07   // 0b00000111
#define NUMBER_8 0x7F   // 0b01111111
#define NUMBER_9 0x6F   // 0b01101111
#define NUMBER_0 0x3F   // 0b00111111

#define ADDRESS_GR1 0x08
#define ADDRESS_GR2 0x88
#define ADDRESS_GR3 0x48
#define ADDRESS_GR4 0xC8
#define ADDRESS_GR5 0x28
#define ADDRESS_GR6 0xA8

#define DISPLAY_OFF 0x00
#define DISPLAY_DUTY_1 0x80
#define DISPLAY_DUTY_2 0x40
#define DISPLAY_DUTY_3 0xC0
#define DISPLAY_DUTY_4 0x20
#define DISPLAY_DUTY_5 0xA0
#define DISPLAY_DUTY_6 0x60
#define DISPLAY_DUTY_7 0xE0
#define DISPLAY_DUTY_8 0x10
#define DISPLAY_DUTY_9 0x90
#define DISPLAY_DUTY_10 0x50
#define DISPLAY_DUTY_11 0xD0
#define DISPLAY_DUTY_12 0x30
#define DISPLAY_DUTY_13 0xB0
#define DISPLAY_DUTY_14 0x70
#define DISPLAY_DUTY_15 0xF0

#define DISPLAY_CURRENT_1 0x00
#define DISPLAY_CURRENT_2 0x08
#define DISPLAY_CURRENT_3 0x04
#define DISPLAY_CURRENT_4 0x0C
#define DISPLAY_CURRENT_5 0x02
#define DISPLAY_CURRENT_6 0x0A
#define DISPLAY_CURRENT_7 0x06
#define DISPLAY_CURRENT_8 0x0E

#define DISPLAY_SG8_GR5 0x00
#define DISPLAY_SG7_GR6 0x01

void TM1652_SendCommand(UART_HandleTypeDef *huart, uint8_t *data, uint16_t len);
void TM1652_setNumber(UART_HandleTypeDef *huart, uint8_t GR, uint8_t number);
void TM1652_setDisplay(UART_HandleTypeDef *huart, uint8_t duty, uint8_t current, uint8_t sg_gr);

void TM1652_Init(UART_HandleTypeDef *huart);
void TM1652_ShowNumber(UART_HandleTypeDef *huart, uint8_t number);

#endif /* __TM1652_H__ */
