/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
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
#include "can.h"
#include "dma.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "drv_bsp.h"

#include "dvc_serialplot.h"
#include "dvc_motor.h"
#include "dvc_motor_dm.h"

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

Class_Serialplot serialplot;
Class_Motor_DM_Normal motor_j4310;
// Class_Motor_C620 motor_3508;
// Class_Motor_GM6020 motor_6020;

// float Target_Angle_3508, Now_Angle_3508, Target_Omega_3508, Now_Omega_3508;
// float Target_Angle_6020, Now_Angle_6020, Target_Omega_6020, Now_Omega_6020, Target_Current_6020, Now_Current_6020;

uint32_t Counter = 0;

static char Variable_Assignment_List[][SERIALPLOT_RX_VARIABLE_ASSIGNMENT_MAX_LENGTH] = {
    //电机调PID
    "po",
    "io",
    "do",
    "fo",
};

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
        // case (0x201):
        // {
        //     motor_3508.CAN_RxCpltCallback(Rx_Buffer->Data);
        // }
        // break;
        // case (0x205):
        // {
        //     motor_6020.CAN_RxCpltCallback(Rx_Buffer->Data);
        // }
        // break;
        case (0x00):
        {
            motor_j4310.CAN_RxCpltCallback(Rx_Buffer->Data);
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
    serialplot.UART_RxCpltCallback(Buffer);
    switch (serialplot.Get_Variable_Index())
    {
        // 电机调PID
        // case(0):
        // {
        //     motor_3508.PID_Omega.Set_K_P(serialplot.Get_Variable_Value());
        // }
        // break;
        // case(1):
        // {
        //     motor_3508.PID_Omega.Set_K_I(serialplot.Get_Variable_Value());
        // }
        // break;
        // case(2):
        // {
        //     motor_3508.PID_Omega.Set_K_D(serialplot.Get_Variable_Value());
        // }
        // break;
        // case(3):
        // {
        //     motor_3508.PID_Omega.Set_K_F(serialplot.Get_Variable_Value());
        // }
        // break;
    }
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
  MX_CAN1_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

    BSP_Init(BSP_DC24_LU_ON | BSP_DC24_LD_ON | BSP_DC24_RU_ON | BSP_DC24_RD_ON);
    CAN_Init(&hcan1, CAN_Motor_Call_Back);
    UART_Init(&huart2, UART_Serialplot_Call_Back, SERIALPLOT_RX_VARIABLE_ASSIGNMENT_MAX_LENGTH);

    serialplot.Init(&huart2, 6, (char **)Variable_Assignment_List);

    // motor_3508.PID_Omega.Init(1200.0f, 3000.0f, 0.0f, 0.0f, 2500.0f, 2500.0f);
    // motor_3508.Init(&hcan1, CAN_Motor_ID_0x201, Control_Method_OMEGA);

    // motor_6020.PID_Torque.Init(0.8f, 100.0f, 0.0f, 0.0f, 30000.0f, 30000.0f);
    // motor_6020.PID_Omega.Init(500.0f, 2000.0f, 0.0f, 0.0f, 1000.0f, 1000.0f);
    // motor_6020.PID_Angle.Init(12.0f, 0.0f, 0.0f, 0.0f, 4.0f * PI, 4.0f * PI);
    // motor_6020.Init(&hcan1, CAN_Motor_ID_0x205, Control_Method_ANGLE);

    motor_j4310.Init(&hcan1, 0x00, 0x01, Motor_DM_Control_Method_NORMAL_EMIT);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
    while (1)
    {
        Counter++;
        // motor_3508.Set_Target_Omega(5.0f * motor_6020.Get_Now_Angle());
        // motor_6020.Set_Target_Angle(0.0f);
        motor_j4310.Set_Control_Angle((Counter / 3000) % 2 == 0 ? 0.0f : PI);
        motor_j4310.Set_Control_Omega(PI);
        motor_j4310.Set_Control_Current(5.0f);
        motor_j4310.TIM_Send_PeriodElapsedCallback();

        // 保持存活
        static uint32_t Counter_KeepAlive = 0;
        if (Counter_KeepAlive++ > 100)
        {
            Counter_KeepAlive = 0;
            
            motor_j4310.TIM_Alive_PeriodElapsedCallback();
        }

        // 串口绘图显示内容
        
        float status = motor_j4310.Get_Control_Status();
        float control_angle = motor_j4310.Get_Control_Angle();
        float now_angle = motor_j4310.Get_Now_Angle();
        float control_omega = motor_j4310.Get_Control_Omega();
        float now_omega = motor_j4310.Get_Now_Omega();
        float control_torque = motor_j4310.Get_Control_Torque();
        float now_torque = motor_j4310.Get_Now_Torque();
        float temperature_mos = motor_j4310.Get_Now_MOS_Temperature();
        float temperature_rotor = motor_j4310.Get_Now_Rotor_Temperature();
        float k_p = motor_j4310.Get_K_P();
        float k_d = motor_j4310.Get_K_D();
        serialplot.Set_Data(11, &status, &control_angle, &now_angle, &control_omega, &now_omega, &control_torque, &now_torque, &temperature_mos, &temperature_rotor, &k_p, &k_d);
        serialplot.TIM_Write_PeriodElapsedCallback();

        //输出数据到电机
        // motor_3508.TIM_PID_PeriodElapsedCallback();
        // motor_6020.TIM_PID_PeriodElapsedCallback();

        //通信设备回调数据
        TIM_CAN_PeriodElapsedCallback();
        TIM_UART_PeriodElapsedCallback();

        //延时1ms
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
  RCC_OscInitStruct.PLL.PLLN = 180;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
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
