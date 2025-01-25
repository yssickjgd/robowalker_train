/**
 * @file drv_adc.h
 * @author yssickjgd (1345578933@qq.com)
 * @brief 仿照SCUT-Robotlab改写的ADC初始化与配置流程
 * @version 0.1
 * @date 2023-08-29 0.1 23赛季定稿
 *
 * @copyright USTC-RoboWalker (c) 2023
 *
 */

#ifndef DRV_ADC_H
#define DRV_ADC_H

/* Includes ------------------------------------------------------------------*/

#include "stm32f4xx_hal.h"
#include "adc.h"

/* Exported macros -----------------------------------------------------------*/

// ADC存储字节数, 与扫描内容有关
#define SAMPLE_BUFFER_SIZE 4

/* Exported types ------------------------------------------------------------*/

/**
 * @brief ADC采样信息结构体
 *
 */
struct Struct_ADC_Manage_Object
{
    ADC_HandleTypeDef *ADC_Handler;
    uint16_t ADC_Data[SAMPLE_BUFFER_SIZE];
};

/* Exported variables --------------------------------------------------------*/

extern Struct_ADC_Manage_Object ADC1_Manage_Object;
extern Struct_ADC_Manage_Object ADC2_Manage_Object;
extern Struct_ADC_Manage_Object ADC3_Manage_Object;

/* Exported function declarations --------------------------------------------*/

void ADC_Init(ADC_HandleTypeDef *hadc, uint16_t Sample_Number);

#endif

/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/
