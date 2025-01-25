/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.c
  * @brief   This file provides code for the configuration
  *          of all used GPIO pins.
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
#include "gpio.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins as
        * Analog
        * Input
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOI_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, BoardA_Pin_J2_Pin|BoardA_Pin_J1_Pin|BoardA_Pin_K1_Pin|BoardA_Pin_K2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOI, BoardA_Pin_Y_Pin|BoardA_Pin_X_Pin|BoardA_Pin_W_Pin|BoardA_Pin_Z_Pin
                          |BoardA_Pin_Q2_Pin|BoardA_Pin_A_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOF, BoardA_Pin_I2_Pin|BoardA_Pin_I1_Pin|BoardA_Pin_Q1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOH, BoardA_DC24_RD_Pin|BoardA_DC24_RU_Pin|BoardA_DC24_LD_Pin|BoardA_DC24_LU_Pin
                          |BoardA_Pin_B_Pin|BoardA_Pin_C_Pin|BoardA_Pin_D_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOG, BoardA_LED_8_Pin|BoardA_LED_7_Pin|BoardA_LED_6_Pin|BoardA_LED_5_Pin
                          |BoardA_LED_4_Pin|BoardA_LED_2_Pin|BoardA_LED_1_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOF, BoardA_MPU6500_CS_Pin|BoardA_LED_G_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(BoardA_LED_3_GPIO_Port, BoardA_LED_3_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, BoardA_Pin_E_Pin|BoardA_Pin_F_Pin|BoardA_Pin_G_Pin|BoardA_Pin_H_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, BoardA_Pin_N2_Pin|BoardA_Pin_O2_Pin|BoardA_Pin_L1_Pin|BoardA_Pin_M1_Pin
                          |BoardA_Pin_N1_Pin|BoardA_Pin_O1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, BoardA_Pin_T_Pin|BoardA_Pin_S_Pin|BoardA_Pin_P2_Pin|BoardA_Pin_P1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(BoardA_LED_R_GPIO_Port, BoardA_LED_R_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, BoardA_Pin_M2_Pin|BoardA_Pin_L2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : BoardA_Pin_J2_Pin BoardA_Pin_J1_Pin BoardA_Pin_K1_Pin BoardA_LED_R_Pin
                           BoardA_Pin_K2_Pin */
  GPIO_InitStruct.Pin = BoardA_Pin_J2_Pin|BoardA_Pin_J1_Pin|BoardA_Pin_K1_Pin|BoardA_LED_R_Pin
                          |BoardA_Pin_K2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : BoardA_Pin_Y_Pin BoardA_Pin_X_Pin BoardA_Pin_W_Pin BoardA_Pin_Z_Pin
                           BoardA_Pin_Q2_Pin BoardA_Pin_A_Pin */
  GPIO_InitStruct.Pin = BoardA_Pin_Y_Pin|BoardA_Pin_X_Pin|BoardA_Pin_W_Pin|BoardA_Pin_Z_Pin
                          |BoardA_Pin_Q2_Pin|BoardA_Pin_A_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);

  /*Configure GPIO pins : BoardA_Pin_I2_Pin BoardA_Pin_I1_Pin BoardA_Pin_Q1_Pin BoardA_LED_G_Pin */
  GPIO_InitStruct.Pin = BoardA_Pin_I2_Pin|BoardA_Pin_I1_Pin|BoardA_Pin_Q1_Pin|BoardA_LED_G_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /*Configure GPIO pins : BoardA_DC24_RD_Pin BoardA_DC24_RU_Pin BoardA_DC24_LD_Pin BoardA_DC24_LU_Pin
                           BoardA_Pin_B_Pin BoardA_Pin_C_Pin BoardA_Pin_D_Pin */
  GPIO_InitStruct.Pin = BoardA_DC24_RD_Pin|BoardA_DC24_RU_Pin|BoardA_DC24_LD_Pin|BoardA_DC24_LU_Pin
                          |BoardA_Pin_B_Pin|BoardA_Pin_C_Pin|BoardA_Pin_D_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

  /*Configure GPIO pins : BoardA_LED_8_Pin BoardA_LED_7_Pin BoardA_LED_6_Pin BoardA_LED_5_Pin
                           BoardA_LED_4_Pin BoardA_LED_3_Pin BoardA_LED_2_Pin BoardA_LED_1_Pin */
  GPIO_InitStruct.Pin = BoardA_LED_8_Pin|BoardA_LED_7_Pin|BoardA_LED_6_Pin|BoardA_LED_5_Pin
                          |BoardA_LED_4_Pin|BoardA_LED_3_Pin|BoardA_LED_2_Pin|BoardA_LED_1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  /*Configure GPIO pin : BoardA_MPU6500_CS_Pin */
  GPIO_InitStruct.Pin = BoardA_MPU6500_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(BoardA_MPU6500_CS_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : BoardA_Pin_E_Pin BoardA_Pin_F_Pin BoardA_Pin_G_Pin BoardA_Pin_H_Pin */
  GPIO_InitStruct.Pin = BoardA_Pin_E_Pin|BoardA_Pin_F_Pin|BoardA_Pin_G_Pin|BoardA_Pin_H_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : BoardA_Pin_N2_Pin BoardA_Pin_O2_Pin BoardA_Pin_L1_Pin BoardA_Pin_M1_Pin
                           BoardA_Pin_N1_Pin BoardA_Pin_O1_Pin */
  GPIO_InitStruct.Pin = BoardA_Pin_N2_Pin|BoardA_Pin_O2_Pin|BoardA_Pin_L1_Pin|BoardA_Pin_M1_Pin
                          |BoardA_Pin_N1_Pin|BoardA_Pin_O1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : BoardA_Pin_T_Pin BoardA_Pin_S_Pin BoardA_Pin_P2_Pin BoardA_Pin_P1_Pin */
  GPIO_InitStruct.Pin = BoardA_Pin_T_Pin|BoardA_Pin_S_Pin|BoardA_Pin_P2_Pin|BoardA_Pin_P1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : BoardA_Pin_M2_Pin BoardA_Pin_L2_Pin */
  GPIO_InitStruct.Pin = BoardA_Pin_M2_Pin|BoardA_Pin_L2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 2 */

/* USER CODE END 2 */
