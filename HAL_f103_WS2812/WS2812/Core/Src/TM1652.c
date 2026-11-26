#include "TM1652.h"


static uint8_t TM1652_Data[2] = {0x00, 0x00};

static uint8_t TM1652_TreanslateNumber(uint8_t number)
{
    switch (number)
    {
    case 0:
        return NUMBER_0;
    case 1:
        return NUMBER_1;
    case 2:
        return NUMBER_2;
    case 3:
        return NUMBER_3;
    case 4:
        return NUMBER_4;
    case 5:
        return NUMBER_5;
    case 6:
        return NUMBER_6;
    case 7:
        return NUMBER_7;
    case 8:
        return NUMBER_8;
    case 9:
        return NUMBER_9;
    default:
        return 0x00; // Return blank for invalid input
    }
}

void TM1652_SendCommand(UART_HandleTypeDef *huart, uint8_t *data, uint16_t len)
{
    HAL_UART_Transmit_DMA(huart, data, len);
    // while (huart->gState != HAL_UART_STATE_READY);
}

void TM1652_setNumber(UART_HandleTypeDef *huart, uint8_t GR, uint8_t number){
    TM1652_Data[0] = GR;
    TM1652_Data[1] = TM1652_TreanslateNumber(number);
    TM1652_SendCommand(huart, TM1652_Data, 2);
}

void TM1652_setDisplay(UART_HandleTypeDef *huart, uint8_t duty, uint8_t current, uint8_t sg_gr){
    TM1652_Data[0] = ADDRESS_DISPLAY;
    TM1652_Data[1] = duty | current | sg_gr;
    TM1652_SendCommand(huart, TM1652_Data, 2);
}

void TM1652_Init(UART_HandleTypeDef *huart){
    TM1652_setDisplay(huart, DISPLAY_DUTY_8, DISPLAY_CURRENT_8, DISPLAY_SG8_GR5);
}

void TM1652_ShowNumber(UART_HandleTypeDef *huart, uint8_t number){
    uint8_t digits[6];
    for(int i = 0; i < 6; i++){
        digits[i] = number % 10;
        number /= 10;
    }
    TM1652_setNumber(huart, ADDRESS_GR1, digits[0]);
    TM1652_setNumber(huart, ADDRESS_GR2, digits[1]);
    TM1652_setNumber(huart, ADDRESS_GR3, digits[2]);
    TM1652_setNumber(huart, ADDRESS_GR4, digits[3]);
    TM1652_setNumber(huart, ADDRESS_GR5, digits[4]);
    TM1652_setNumber(huart, ADDRESS_GR6, digits[5]);
}
