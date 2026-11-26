#include "ws2812.h"

uint8_t led_data[LED_NUM * 3]; // Array to hold the color data for each LED
static uint16_t ws_buffer[LED_NUM * 24 + RESET_CYCLES]; // Array to hold the PWM data for each LED

void ws2812_init(void){
    // Initialize the LED data array to all off (black)
    for (int i = 0; i < LED_NUM * 3; i++) {
        led_data[i] = 0;
    }
}

void ws2812_set_color(uint8_t led_index, uint8_t red, uint8_t green, uint8_t blue){
    if (led_index < LED_NUM) {
        uint16_t offset = led_index * 3;
        led_data[offset] = green;
        led_data[offset + 1] = red;
        led_data[offset + 2] = blue;
    }
}

void ws2812_show(uint8_t *data, uint16_t length){
    // 定义临时存储LED的颜色数据
    uint32_t color;
    // 分别定义LED索引、位索引和缓冲区索引
    uint16_t led_index, bit_index, buffer_index = 0;

    for(led_index = 0; led_index < length; led_index++){
        // 取出当前第led_index个LED的颜色数据，按顺序为绿色、红色、蓝色
        color = (data[led_index * 3] << 16) | (data[led_index * 3 + 1] << 8) | data[led_index * 3 + 2];
        
        // 将颜色数据转换为PWM信号，存入ws_buffer中
        for(bit_index = 0; bit_index < 24; bit_index++){
            if(color & 0x800000){
                ws_buffer[buffer_index++] = WS_BIT1;
            } else {
                ws_buffer[buffer_index++] = WS_BIT0;
            }
            color <<= 1;
        }
    }
    // 添加复位信号
    for(uint8_t i = 0; i < RESET_CYCLES; i++){
        ws_buffer[buffer_index++] = 0; // 添加复位信号
    }
    // 启动DMA传输，将ws_buffer中的数据发送到WS2812
    HAL_TIM_PWM_Start_DMA(&htim1, TIM_CHANNEL_1, (uint32_t*)ws_buffer, buffer_index);
}
