/**
 * @file dvc_sampler.cpp
 * @author yssickjgd (1345578933@qq.com)
 * @brief ADC采样器
 * @version 0.1
 * @date 2023-08-29 0.1 23赛季定稿
 *
 * @copyright USTC-RoboWalker (c) 2023
 *
 */

/* Includes ------------------------------------------------------------------*/

#include "dvc_sampler.h"

/* Private macros ------------------------------------------------------------*/

/* Private types -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function declarations ---------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/

/**
 * @brief 采样器初始化
 * 
 * @param hadc 指定的ADC
 * @param __Sampler_Serial 采样点序列号, 从0开始
 * @param __Sampler_Filter 采用的滤波器
 */
template<uint32_t Filter_Fourier_Order>
void Class_Sampler<Filter_Fourier_Order>::Init(ADC_HandleTypeDef *hadc, uint16_t __Sampler_Serial, Enum_Sampler_Filter __Sampler_Filter)
{
    if (hadc->Instance == ADC1)
    {
        ADC_Manage_Object = &ADC1_Manage_Object;
    }
    else if (hadc->Instance == ADC2)
    {
        ADC_Manage_Object = &ADC2_Manage_Object;
    }
    else if (hadc->Instance == ADC3)
    {
        ADC_Manage_Object = &ADC3_Manage_Object;
    }
    Sampler_Serial = __Sampler_Serial;
    Sampler_Filter = __Sampler_Filter;

    ADC_Value = &ADC_Manage_Object->ADC_Data[Sampler_Serial];
}

/**
 * @brief TIM定时器中断计算滤波器回调函数
 *
 */
template<uint32_t Filter_Fourier_Order>
void Class_Sampler<Filter_Fourier_Order>::TIM_Sampler_PeriodElapsedCallback()
{
    //选择滤波器, 若没有则直接输出
    switch (Sampler_Filter)
    {
    case (Sampler_Filter_NULL):
    {
        Value = *ADC_Value / 4096.0f;
    }
        break;
    case (Sampler_Filter_FOURIER):
    {
        Filter_Fourier.Set_Now(*ADC_Value / 4096.0f);
        Filter_Fourier.TIM_Calculate_PeriodElapsedCallback();
        Value = Filter_Fourier.Get_Out();
    }
        break;
    }
}

/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/
