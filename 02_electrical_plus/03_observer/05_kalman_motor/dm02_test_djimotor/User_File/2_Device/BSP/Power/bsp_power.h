/**
 * @file bsp_power.h
 * @author yssickjgd (1345578933@qq.com)
 * @brief 板载电源, 包含两个24V和一个5V输出
 * @version 0.1
 * @date 2025-08-15 0.1 新建文档
 *
 * @copyright USTC-RoboWalker (c) 2025
 *
 */

#ifndef BSP_POWER_H
#define BSP_POWER_H

/* Includes ------------------------------------------------------------------*/

#include "1_Middleware/Driver/ADC/drv_adc.h"
#include "gpio.h"
#include "stm32h7xx_hal.h"

/* Exported macros -----------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/**
 * @brief Specialized, 板载电源相关, 包括两个24V输出和一个5V输出以及一个电源检测器
 *
 */
class Class_Power
{
public:
    void Init(const bool &__DC24_0_Status = false, const bool &__DC24_1_Status = false, const bool &__DC5_Status = false);

    inline float Get_Power_Voltage() const;

    inline void Set_DC24_0(const bool &__Status);

    inline void Set_DC24_1(const bool &__Status);

    inline void Set_DC5(const bool &__Status);

protected:
    // 初始化相关常量

    // 绑定的ADC
    Struct_ADC_Manage_Object *ADC_Manage_Object;
    // 绑定的ADC缓冲区偏移量
    uint16_t ADC_Buffer_Offset = 0;

    // 常量

    // 内部变量

    // 读变量

    // 写变量

    // 读写变量

    // 内部函数
};

/* Exported variables --------------------------------------------------------*/

extern Class_Power BSP_Power;

/* Exported function declarations --------------------------------------------*/

/**
 * @brief 获取电源电压
 *
 * @return float 电源电压, 单位V
 */
inline float Class_Power::Get_Power_Voltage() const
{
    // 读取ADC值, 偏移量为0
    uint32_t adc_value = ADC_Manage_Object->ADC_Data[ADC_Buffer_Offset];
    // 计算电压, 假设ADC分辨率为16位,
    float voltage = ((float) (adc_value) / (float) (1 << 16)) * 3.3f * 11.0f;
    // 返回电压值
    return (voltage);
}

/**
 * @brief 设置DC24V_0输出
 *
 * @param __DC24_0 true: 输出, false: 不输出
 */
inline void Class_Power::Set_DC24_0(const bool &__Status)
{
    HAL_GPIO_WritePin(DC24_0__OUTPUT_GPIO_Port, DC24_0__OUTPUT_Pin, __Status ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

/**
 * @brief 设置DC24V_1输出
 *
 * @param __DC24_1 true: 输出, false: 不输出
 */
inline void Class_Power::Set_DC24_1(const bool &__Status)
{
    HAL_GPIO_WritePin(DC24_1__OUTPUT_GPIO_Port, DC24_1__OUTPUT_Pin, __Status ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

/**
 * @brief 设置DC5V输出
 *
 * @param __DC5 true: 输出, false: 不输出
 */
inline void Class_Power::Set_DC5(const bool &__Status)
{
    HAL_GPIO_WritePin(DC5__OUTPUT_GPIO_Port, DC5__OUTPUT_Pin, __Status ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

#endif

/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/