#ifndef __WS2812_H__
#define __WS2812_H__

#include "main.h"

extern TIM_HandleTypeDef htim1;

#define LED_NUM 60
#define WS_BIT0 30  // 0码高电平计数 (30/72MHz = 417ns)
#define WS_BIT1 60  // 1码高电平计数 (60/72MHz = 833ns)
#define RESET_CYCLES 50

extern uint8_t led_data[LED_NUM * 3];

void ws2812_init(void);
void ws2812_set_color(uint8_t led_index, uint8_t red, uint8_t green, uint8_t blue);
void ws2812_show(uint8_t *data, uint16_t length);

#endif
