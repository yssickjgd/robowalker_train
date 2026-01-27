/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "4_Task/tsk_config_and_callback.h"

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define DC24_1__OUTPUT_Pin GPIO_PIN_13
#define DC24_1__OUTPUT_GPIO_Port GPIOC
#define DC24_0__OUTPUT_Pin GPIO_PIN_14
#define DC24_0__OUTPUT_GPIO_Port GPIOC
#define DC5__OUTPUT_Pin GPIO_PIN_15
#define DC5__OUTPUT_GPIO_Port GPIOC
#define BMI088_ACCEL__SPI_CS_Pin GPIO_PIN_0
#define BMI088_ACCEL__SPI_CS_GPIO_Port GPIOC
#define BMI088_RX__SPI_MOSI_Pin GPIO_PIN_1
#define BMI088_RX__SPI_MOSI_GPIO_Port GPIOC
#define BMI088_TX__SPI_MISO_Pin GPIO_PIN_2
#define BMI088_TX__SPI_MISO_GPIO_Port GPIOC
#define BMI088_GYRO__SPI_CS_Pin GPIO_PIN_3
#define BMI088_GYRO__SPI_CS_GPIO_Port GPIOC
#define SERVO_3__PWM_Pin GPIO_PIN_0
#define SERVO_3__PWM_GPIO_Port GPIOA
#define W25Q64_HOLD__OSPI_IO3_Pin GPIO_PIN_1
#define W25Q64_HOLD__OSPI_IO3_GPIO_Port GPIOA
#define SERVO_2__PWM_Pin GPIO_PIN_2
#define SERVO_2__PWM_GPIO_Port GPIOA
#define W25Q64_WP__OSPI_IO2_Pin GPIO_PIN_3
#define W25Q64_WP__OSPI_IO2_GPIO_Port GPIOA
#define WS2812_RX__SPI_MOSI_Pin GPIO_PIN_7
#define WS2812_RX__SPI_MOSI_GPIO_Port GPIOA
#define BAT_VOLTAGE__ADC_Pin GPIO_PIN_4
#define BAT_VOLTAGE__ADC_GPIO_Port GPIOC
#define W25Q64_SO__OSPI_IO1_Pin GPIO_PIN_0
#define W25Q64_SO__OSPI_IO1_GPIO_Port GPIOB
#define HEATER__PWM_Pin GPIO_PIN_1
#define HEATER__PWM_GPIO_Port GPIOB
#define W25Q64_CLK__OSPI_CLK_Pin GPIO_PIN_2
#define W25Q64_CLK__OSPI_CLK_GPIO_Port GPIOB
#define SERVO_1__PWM_Pin GPIO_PIN_9
#define SERVO_1__PWM_GPIO_Port GPIOE
#define BMI088_ACCEL__INTERRUPT_Pin GPIO_PIN_10
#define BMI088_ACCEL__INTERRUPT_GPIO_Port GPIOE
#define BMI088_ACCEL__INTERRUPT_EXTI_IRQn EXTI15_10_IRQn
#define W25Q64_CS__OSPI_NCS_Pin GPIO_PIN_11
#define W25Q64_CS__OSPI_NCS_GPIO_Port GPIOE
#define BMI088_GYRO__INTERRUPT_Pin GPIO_PIN_12
#define BMI088_GYRO__INTERRUPT_GPIO_Port GPIOE
#define BMI088_GYRO__INTERRUPT_EXTI_IRQn EXTI15_10_IRQn
#define SERVO_0__PWM_Pin GPIO_PIN_13
#define SERVO_0__PWM_GPIO_Port GPIOE
#define BMI088_CLK__SPI_CLK_Pin GPIO_PIN_13
#define BMI088_CLK__SPI_CLK_GPIO_Port GPIOB
#define BUZZER__PWM_Pin GPIO_PIN_15
#define BUZZER__PWM_GPIO_Port GPIOB
#define W25Q64_SI__OSPI_IO0_Pin GPIO_PIN_11
#define W25Q64_SI__OSPI_IO0_GPIO_Port GPIOD
#define KEY__INPUT_Pin GPIO_PIN_15
#define KEY__INPUT_GPIO_Port GPIOA
#define SBUS_TX__UART_RX_Pin GPIO_PIN_2
#define SBUS_TX__UART_RX_GPIO_Port GPIOD

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
