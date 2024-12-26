/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
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
#include "adc.h"
#include "can.h"
#include "dma.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "1_Middleware/1_Driver/BSP/drv_djiboarda.h"

#include "2_Device/Motor/Motor_DJI/dvc_motor_dji.h"
#include "2_Device/Serialplot/dvc_serialplot.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

bool init_finished = 0;

Class_Serialplot serialplot;
Class_Motor_DJI_GM6020 motor;

uint32_t Counter = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/**
 * @brief CAN报文回调函数
 *
 * @param Rx_Buffer CAN接收的信息结构体
 */
void CAN_Motor_Call_Back(Struct_CAN_Rx_Buffer *Rx_Buffer)
{
  switch (Rx_Buffer->Header.StdId)
  {
  case (0x205):
  {
    motor.CAN_RxCpltCallback(Rx_Buffer->Data);
  }
  }
}

/**
 * @brief HAL库UART接收DMA空闲中断
 *
 * @param huart UART编号
 * @param Size 长度
 */
void UART_Serialplot_Call_Back(uint8_t *Buffer, uint16_t Length)
{

}

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
  MX_CAN2_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_TIM5_Init();
  MX_TIM12_Init();
  MX_CAN1_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();
  MX_USART6_UART_Init();
  MX_UART7_Init();
  MX_UART8_Init();
  MX_ADC1_Init();
  MX_SPI5_Init();
  /* USER CODE BEGIN 2 */

  BSP_Init(BSP_DC24_LU_ON | BSP_DC24_LD_ON | BSP_DC24_RU_ON | BSP_DC24_RD_ON);
  CAN_Init(&hcan1, CAN_Motor_Call_Back);
  UART_Init(&huart2, UART_Serialplot_Call_Back, SERIALPLOT_RX_VARIABLE_ASSIGNMENT_MAX_LENGTH);

  serialplot.Init(&huart2, Serialplot_Checksum_8_ENABLE, NULL);

  motor.PID_Angle.Init(12.0f, 0.0f, 0.0f, 0.0f, 7.0f * PI, 7.0f * PI);
  motor.PID_Omega.Init(0.03f, 0.3f, 0.0f, 0.0f, 3.0f, 3.0f);
  // motor.PID_Current.Init(1.0f, 5000.0f, 0.0f, 0.0f, 24.0f, 24.0f);
  motor.Init(&hcan1, Motor_DJI_ID_0x205, Motor_DJI_Control_Method_ANGLE, 0, Motor_DJI_GM6020_Driver_Version_2023);

  init_finished = 1;

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    Counter++;
    motor.Set_Target_Angle(PI * sin(6.0f * Counter / 1000.0f));
    motor.Set_Feedforward_Omega(PI * 6.0f * cos(6.0f * Counter / 1000.0f));

    // 保持存活
    static uint32_t Counter_KeepAlive = 0;
    if (Counter_KeepAlive++ > 100)
    {
      Counter_KeepAlive = 0;

      motor.TIM_100ms_Alive_PeriodElapsedCallback();
    }

    // 串口绘图显示内容

    float target_angle = motor.Get_Target_Angle();
    float now_angle = motor.Get_Now_Angle();
    float target_omega = motor.Get_Target_Omega() + motor.Get_Feedforward_Omega();
    float now_omega = motor.Get_Now_Omega();
    float target_current = motor.Get_Target_Current() + motor.Get_Feedforward_Current();
    float now_current = motor.Get_Now_Current();
    serialplot.Set_Data(6, &target_angle, &now_angle, &target_omega, &now_omega, &target_current, &now_current);
    serialplot.TIM_1ms_Write_PeriodElapsedCallback();

    // 电机控制
    motor.TIM_Calculate_PeriodElapsedCallback();

    // 通信设备回调数据
    TIM_1ms_CAN_PeriodElapsedCallback();

    // 延时1ms
    HAL_Delay(0);
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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 6;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

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

#ifdef  USE_FULL_ASSERT
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
