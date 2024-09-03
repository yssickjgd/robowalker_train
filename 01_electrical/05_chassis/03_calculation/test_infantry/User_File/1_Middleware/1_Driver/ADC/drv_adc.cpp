/**
 * @file drv_adc.cpp
 * @author yssickjgd (1345578933@qq.com)
 * @brief 仿照SCUT-Robotlab改写的ADC初始化与配置流程
 * @version 0.1
 * @date 2023-08-29 0.1 23赛季定稿
 *
 * @copyright USTC-RoboWalker (c) 2023
 *
 */

/* Includes ------------------------------------------------------------------*/

#include "drv_adc.h"

/* Private macros ------------------------------------------------------------*/

/* Private types -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

Struct_ADC_Manage_Object ADC1_Manage_Object = {0};
Struct_ADC_Manage_Object ADC2_Manage_Object = {0};
Struct_ADC_Manage_Object ADC3_Manage_Object = {0};

/* Private function declarations ---------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/

/**
 * @brief 初始化ADC
 *
 * @param hadc ADC编号
 * @param Sample_Number 一周期被采样的数据数量, 通道数 * 每个通道的传输数
 */
void ADC_Init(ADC_HandleTypeDef *hadc, uint16_t Sample_Number)
{
    if (hadc->Instance == ADC1)
    {
        ADC1_Manage_Object.ADC_Handler = hadc;
        HAL_ADC_Start_DMA(hadc, (uint32_t *) &ADC1_Manage_Object.ADC_Data, Sample_Number);
    }
    else if (hadc->Instance == ADC2)
    {
        ADC2_Manage_Object.ADC_Handler = hadc;
        HAL_ADC_Start_DMA(hadc, (uint32_t *) &ADC2_Manage_Object.ADC_Data, Sample_Number);
    }
    else if (hadc->Instance == ADC3)
    {
        ADC3_Manage_Object.ADC_Handler = hadc;
        HAL_ADC_Start_DMA(hadc, (uint32_t *) &ADC3_Manage_Object.ADC_Data, Sample_Number);
    }
}

/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/
