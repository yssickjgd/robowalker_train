/**
 * @file dvc_sampler.h
 * @author yssickjgd (1345578933@qq.com)
 * @brief ADC采样器
 * @version 0.1
 * @date 2023-08-29 0.1 23赛季定稿
 *
 * @copyright USTC-RoboWalker (c) 2023
 *
 */

#ifndef DVC_SAMPLER_H
#define DVC_SAMPLER_H

/* Includes ------------------------------------------------------------------*/

#include "1_Middleware/2_Algorithm/Filter/alg_filter.h"
#include "1_Middleware/1_Driver/ADC/drv_adc.h"

/* Exported macros -----------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/**
 * @brief 采样器的滤波器
 *
 */
enum Enum_Sampler_Filter
{
    Sampler_Filter_NULL = 0,
    Sampler_Filter_FOURIER,
};

/**
 * @brief Reusable, 采样器, 后续可以增加多对一采样并匹配滤波等算法
 *
 */
template<uint32_t Filter_Fourier_Order>
class Class_Sampler
{
public:
    //滤波器算法
    Class_Filter_Fourier<Filter_Fourier_Order> Filter_Fourier;

    void Init(ADC_HandleTypeDef *hadc, uint16_t __Sampler_Serial, Enum_Sampler_Filter __Sampler_Filter);

    inline float Get_Value();

    void TIM_Sampler_PeriodElapsedCallback();

protected:
    //初始化相关常量

    //绑定的UART
    Struct_ADC_Manage_Object *ADC_Manage_Object;
    //绑定的采样通道
    uint16_t Sampler_Serial;
    //采用的滤波器
    Enum_Sampler_Filter Sampler_Filter;
    //对应采样数据指针
    uint16_t *ADC_Value;

    //常量

    //内部变量

    //读变量

    float Value = 0.0f;

    //写变量

    //读写变量

    //内部函数
};

/* Exported variables --------------------------------------------------------*/

/* Exported function declarations --------------------------------------------*/

/**
 * @brief 获取采样值, 归化到0~1
 *
 * @return float 采样值, 归化到0~1
 */
template<uint32_t Filter_Fourier_Order>
inline float Class_Sampler<Filter_Fourier_Order>::Get_Value()
{
    return (Value);
}

#endif

/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/
