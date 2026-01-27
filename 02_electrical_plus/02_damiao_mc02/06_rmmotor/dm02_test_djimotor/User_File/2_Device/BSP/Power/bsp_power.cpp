/**
 * @file bsp_power.cpp
 * @author yssickjgd (1345578933@qq.com)
 * @brief 板载电源, 包含两个24V和一个5V输出
 * @version 0.1
 * @date 2025-08-15 0.1 新建文档
 *
 * @copyright USTC-RoboWalker (c) 2025
 *
 */

/* Includes ------------------------------------------------------------------*/

#include "bsp_power.h"

/* Private macros ------------------------------------------------------------*/

/* Private types -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

Class_Power BSP_Power;

/* Private function declarations ---------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/

/**
 * @brief 初始化电源
 *
 * @param __DC24_0 DC24V_0输出
 * @param __DC24_1 DC24V_1输出
 * @param __DC5 DC5V输出
 */
void Class_Power::Init(const bool &__DC24_0_Status, const bool &__DC24_1_Status, const bool &__DC5_Status)
{
    ADC_Manage_Object = &ADC1_Manage_Object;

    HAL_GPIO_WritePin(DC24_0__OUTPUT_GPIO_Port, DC24_0__OUTPUT_Pin, __DC24_0_Status ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(DC24_1__OUTPUT_GPIO_Port, DC24_1__OUTPUT_Pin, __DC24_1_Status ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(DC5__OUTPUT_GPIO_Port, DC5__OUTPUT_Pin, __DC5_Status ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/