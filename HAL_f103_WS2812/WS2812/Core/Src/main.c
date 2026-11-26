/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stm32f1xx_hal_gpio.h"
#include "ws2812.h"
#include "TM1652.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
typedef StaticTask_t osStaticThreadDef_t;
typedef StaticSemaphore_t osStaticMutexDef_t;
typedef StaticSemaphore_t osStaticSemaphoreDef_t;
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim1;
DMA_HandleTypeDef hdma_tim1_ch1;

UART_HandleTypeDef huart1;
DMA_HandleTypeDef hdma_usart1_tx;

/* Definitions for UartTask */
osThreadId_t UartTaskHandle;
uint32_t UartTaskBuffer[ 64 ];
osStaticThreadDef_t UartTaskControlBlock;
const osThreadAttr_t UartTask_attributes = {
  .name = "UartTask",
  .cb_mem = &UartTaskControlBlock,
  .cb_size = sizeof(UartTaskControlBlock),
  .stack_mem = &UartTaskBuffer[0],
  .stack_size = sizeof(UartTaskBuffer),
  .priority = (osPriority_t) osPriorityHigh,
};
/* Definitions for ws2812_Task */
osThreadId_t ws2812_TaskHandle;
uint32_t ws2812_TaskBuffer[ 64 ];
osStaticThreadDef_t ws2812_TaskControlBlock;
const osThreadAttr_t ws2812_Task_attributes = {
  .name = "ws2812_Task",
  .cb_mem = &ws2812_TaskControlBlock,
  .cb_size = sizeof(ws2812_TaskControlBlock),
  .stack_mem = &ws2812_TaskBuffer[0],
  .stack_size = sizeof(ws2812_TaskBuffer),
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for TreansmitMutex */
osMutexId_t TreansmitMutexHandle;
osStaticMutexDef_t TreansmitMutexControlBlock;
const osMutexAttr_t TreansmitMutex_attributes = {
  .name = "TreansmitMutex",
  .cb_mem = &TreansmitMutexControlBlock,
  .cb_size = sizeof(TreansmitMutexControlBlock),
};
/* Definitions for UartTxSemaphore */
osSemaphoreId_t UartTxSemaphoreHandle;
osStaticSemaphoreDef_t UartTxSemaphoreControlBlock;
const osSemaphoreAttr_t UartTxSemaphore_attributes = {
  .name = "UartTxSemaphore",
  .cb_mem = &UartTxSemaphoreControlBlock,
  .cb_size = sizeof(UartTxSemaphoreControlBlock),
};
/* Definitions for PWMTxSemaphore */
osSemaphoreId_t PWMTxSemaphoreHandle;
osStaticSemaphoreDef_t PWMTxSemaphoreControlBlock;
const osSemaphoreAttr_t PWMTxSemaphore_attributes = {
  .name = "PWMTxSemaphore",
  .cb_mem = &PWMTxSemaphoreControlBlock,
  .cb_size = sizeof(PWMTxSemaphoreControlBlock),
};
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_TIM1_Init(void);
static void MX_USART1_UART_Init(void);
void UartTaskEntryFunction(void *argument);
void ws2812_Task_EntryFunction(void *argument);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_TIM1_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();
  /* Create the mutex(es) */
  /* creation of TreansmitMutex */
  TreansmitMutexHandle = osMutexNew(&TreansmitMutex_attributes);

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* creation of UartTxSemaphore */
  UartTxSemaphoreHandle = osSemaphoreNew(1, 1, &UartTxSemaphore_attributes);

  /* creation of PWMTxSemaphore */
  PWMTxSemaphoreHandle = osSemaphoreNew(1, 1, &PWMTxSemaphore_attributes);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of UartTask */
  UartTaskHandle = osThreadNew(UartTaskEntryFunction, NULL, &UartTask_attributes);

  /* creation of ws2812_Task */
  ws2812_TaskHandle = osThreadNew(ws2812_Task_EntryFunction, NULL, &ws2812_Task_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 0;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 89;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 0;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */
  HAL_TIM_MspPostInit(&htim1);

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 19200;
  huart1.Init.WordLength = UART_WORDLENGTH_9B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_ODD;
  huart1.Init.Mode = UART_MODE_TX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel2_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel2_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel2_IRQn);
  /* DMA1_Channel4_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel4_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel4_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LEDState_Pin|NumDisplay_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);

  /*Configure GPIO pins : KEY1_Pin KEY2_Pin KEY3_Pin */
  GPIO_InitStruct.Pin = KEY1_Pin|KEY2_Pin|KEY3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : LEDState_Pin NumDisplay_Pin */
  GPIO_InitStruct.Pin = LEDState_Pin|NumDisplay_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : LED_Pin */
  GPIO_InitStruct.Pin = LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED_GPIO_Port, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
    // 判断是不是我们的 TIM1 通道 1 发完了
    if (htim->Instance == TIM1)
    {
        HAL_TIM_PWM_Stop_DMA(&htim1, TIM_CHANNEL_1);
        
        // 如果用 RTOS 信号量，在这里释放
        osSemaphoreRelease(PWMTxSemaphoreHandle); 
    }
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    // 判断是不是 USART1 发完了
    if (huart->Instance == USART1)
    {
        // 到了这里，说明 TM1652 的两包数据（地址+数据）已经完全变成了物理波形发出去了。
        
        // 1. 如果用 RTOS 信号量，在这里释放
        osSemaphoreRelease(UartTxSemaphoreHandle);
    }
}

/* USER CODE END 4 */

/* USER CODE BEGIN Header_UartTaskEntryFunction */
/**
  * @brief  Function implementing the UartTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_UartTaskEntryFunction */
void UartTaskEntryFunction(void *argument)
{
  /* USER CODE BEGIN 5 */
  osMutexAcquire(TreansmitMutexHandle, osWaitForever); // 获取互斥锁，确保独占访问
  HAL_GPIO_WritePin(NumDisplay_GPIO_Port, NumDisplay_Pin, GPIO_PIN_RESET);
  TM1652_setNumber(&huart1, ADDRESS_GR1, 1); // 发地址 + 数字 '1'

  osMutexRelease(TreansmitMutexHandle); // 释放互斥锁

  // osSemaphoreAcquire(UartTxSemaphoreHandle, 100); // 等待发送完成信号量

  osDelay(10);                      // 给芯片一点点反应时间

  TM1652_setNumber(&huart1, ADDRESS_GR2, 2); // 发地址 + 数字 '2'

  osMutexRelease(TreansmitMutexHandle); // 释放互斥锁

  // osSemaphoreAcquire(UartTxSemaphoreHandle, 100); // 等待发送完成信号量

  osDelay(10);                      // 给芯片一点点反应时间

  TM1652_setNumber(&huart1, ADDRESS_GR3, 3); // 发地址 + 数字 '3'

  osMutexRelease(TreansmitMutexHandle); // 释放互斥锁

  // osSemaphoreAcquire(UartTxSemaphoreHandle, 100); // 等待发送完成信号量

  osDelay(10);                      // 给芯片一点点反应时间

  TM1652_setNumber(&huart1, ADDRESS_GR4, 4); // 发地址 + 数字 '4'

  osMutexRelease(TreansmitMutexHandle); // 释放互斥锁

  // osSemaphoreAcquire(UartTxSemaphoreHandle, 100); // 等待发送完成信号量

  osDelay(10);                      // 给芯片一点点反应时间

  TM1652_Init(&huart1);               // 初始化，配置模式并拉高 >3ms
  
  osDelay(5);                       // 锁存必须的 3ms 空闲帧
    
  TM1652_setDisplay(&huart1, DISPLAY_DUTY_8, DISPLAY_CURRENT_8, DISPLAY_SG8_GR5); // 真正打开显示
  osDelay(5);
  /* Infinite loop */
  for(;;)
  {
    osDelay(1000);
  }
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_ws2812_Task_EntryFunction */
/**
* @brief Function implementing the ws2812_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_ws2812_Task_EntryFunction */
void ws2812_Task_EntryFunction(void *argument)
{
  /* USER CODE BEGIN ws2812_Task_EntryFunction */
  ws2812_init();
  for(uint8_t i=0; i<20; i++){
    ws2812_set_color(i, 255, 0, 0);
  }
  for(uint8_t i=20; i<40; i++){
    ws2812_set_color(i, 0, 255, 0);
  }
  for(int i = 40; i < LED_NUM; i++){
    ws2812_set_color(i, 0, 0, 255); // Set all LEDs to blue
  }
  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
  osMutexAcquire(TreansmitMutexHandle, osWaitForever); // 获取互斥锁，确保独占访问
  ws2812_show(led_data, LED_NUM);
  osMutexRelease(TreansmitMutexHandle); // 释放互斥锁
  osSemaphoreAcquire(PWMTxSemaphoreHandle, 100); // 等待发送完成信号量
  /* Infinite loop */
  uint8_t index = 0;
  // uint8_t red, green, blue;
  for(;;)
  {
    if(++index == 60){
      index = 0;
    }
    // red = (index + 20) % 60;
    // green = (index + 40) % 60;
    // blue = index;
    ws2812_set_color((index + 20) % 60, 255, 0, 0);
    ws2812_set_color((index + 40) % 60, 0, 255, 0);
    ws2812_set_color(index, 0, 0, 255);
    ws2812_show(led_data, LED_NUM);
    osDelay(200);
  }
  /* USER CODE END ws2812_Task_EntryFunction */
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM2 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM2)
  {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
