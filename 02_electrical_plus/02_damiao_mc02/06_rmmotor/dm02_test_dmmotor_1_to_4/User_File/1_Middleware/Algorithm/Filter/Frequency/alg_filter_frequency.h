/**
 * @file alg_filter_frequency.h
 * @author yssickjgd (1345578933@qq.com)
 * @brief 频率滤波器
 * @version 1.1
 * @date 2023-08-29 0.1 23赛季定稿
 * @date 2023-09-25 1.1 可自定义滤波器阶数
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef ALG_FILTER_FREQUENCY_H
#define ALG_FILTER_FREQUENCY_H

/* Includes ------------------------------------------------------------------*/

#include "1_Middleware/Algorithm/Basic/alg_basic.h"

/* Exported macros -----------------------------------------------------------*/

extern const float FREQUENCY_FILTER_DEFAULT_SAMPLING_FREQUENCY;

/* Exported types ------------------------------------------------------------*/

/**
 * @brief 滤波器类型
 *
 */
enum Enum_Filter_Frequency_Type
{
    Filter_Frequency_Type_LOWPASS = 0,
    Filter_Frequency_Type_HIGHPASS,
    Filter_Frequency_Type_BANDPASS,
    Filter_Frequency_Type_BANDSTOP,
};

/**
 * @brief Reusable, Frequency滤波器算法
 *
 */
template<uint32_t Filter_Frequency_Order = 50>
class Class_Filter_Frequency
{
public:
    void Init(const float &__Value_Constrain_Low = 0.0f, const float &__Value_Constrain_High = 1.0f, const Enum_Filter_Frequency_Type &__Filter_Frequency_Type = Filter_Frequency_Type_LOWPASS, const float &__Frequency_Low = 0.0f, const float &__Frequency_High = FREQUENCY_FILTER_DEFAULT_SAMPLING_FREQUENCY / 2.0f, const float &__Sampling_Frequency = FREQUENCY_FILTER_DEFAULT_SAMPLING_FREQUENCY);

    inline float Get_Out() const;

    inline void Set_Now(const float &__Now);

    void TIM_Calculate_PeriodElapsedCallback();

protected:
    // 初始化相关常量

    // 输入限幅
    float Value_Constrain_Low;
    float Value_Constrain_High;

    // 滤波器类型
    Enum_Filter_Frequency_Type Filter_Frequency_Type;
    // 滤波器特征低频
    float Frequency_Low;
    // 滤波器特征高频
    float Frequency_High;
    // 滤波器采样频率
    float Sampling_Frequency;

    // 常量

    // 内部变量

    // 卷积系统函数向量
    float System_Function[Filter_Frequency_Order + 1]{};

    // 输入信号向量
    float Input_Signal[Filter_Frequency_Order + 1]{};

    // 新数据指示向量
    uint8_t Signal_Flag = 0;

    // 读变量

    // 输出值
    float Out = 0;

    // 写变量

    // 读写变量

    // 内部函数
};

/* Exported variables --------------------------------------------------------*/

/* Exported function declarations --------------------------------------------*/

/**
 * @brief 初始化滤波器
 *
 * @tparam Filter_Frequency_Order 滤波器阶数
 * @param __Value_Constrain_Low 滤波器最小值, 全0不限制
 * @param __Value_Constrain_High 滤波器最大值, 全0不限制
 * @param __Filter_Frequency_Type 滤波器类型
 * @param __Frequency_Low 滤波器特征低频, 非高通有效
 * @param __Frequency_High 滤波器特征高频, 非低通有效
 * @param __Sampling_Frequency 滤波器采样频率
 */
template<uint32_t Filter_Frequency_Order>
void Class_Filter_Frequency<Filter_Frequency_Order>::Init(const float &__Value_Constrain_Low, const float &__Value_Constrain_High, const Enum_Filter_Frequency_Type &__Filter_Frequency_Type, const float &__Frequency_Low, const float &__Frequency_High, const float &__Sampling_Frequency)
{
    Value_Constrain_Low = __Value_Constrain_Low;
    Value_Constrain_High = __Value_Constrain_High;
    Filter_Frequency_Type = __Filter_Frequency_Type;
    Frequency_Low = __Frequency_Low;
    Frequency_High = __Frequency_High;
    Sampling_Frequency = __Sampling_Frequency;

    // 将所有计算所得值进行softmax操作成和为1的值
    float system_function_sum = 0.0f;
    // 特征低角速度
    float omega_low;
    // 特征高角速度
    float omega_high;

    omega_low = 2.0f * PI * Frequency_Low / Sampling_Frequency;
    omega_high = 2.0f * PI * Frequency_High / Sampling_Frequency;

    // 计算滤波器系统

    switch (Filter_Frequency_Type)
    {
    case (Filter_Frequency_Type_LOWPASS):
    {
        for (int i = 0; i < Filter_Frequency_Order + 1; i++)
        {
            System_Function[i] = omega_low / PI * Basic_Math_Sinc(((float) (i) - Filter_Frequency_Order / 2.0f) * omega_low);
        }

        break;
    }
    case (Filter_Frequency_Type_HIGHPASS):
    {
        for (int i = 0; i < Filter_Frequency_Order + 1; i++)
        {
            System_Function[i] = Basic_Math_Sinc(((float) (i) - Filter_Frequency_Order / 2.0f) * PI) - omega_high / PI * Basic_Math_Sinc(((float) (i) - Filter_Frequency_Order / 2.0f) * omega_high);
        }

        break;
    }
    case (Filter_Frequency_Type_BANDPASS):
    {
        for (int i = 0; i < Filter_Frequency_Order + 1; i++)
        {
            System_Function[i] = omega_high / PI * Basic_Math_Sinc(((float) (i) - Filter_Frequency_Order / 2.0f) * omega_high) - omega_low / PI * Basic_Math_Sinc(((float) (i) - Filter_Frequency_Order / 2.0f) * omega_low);
        }

        break;
    }
    case (Filter_Frequency_Type_BANDSTOP):
    {
        for (int i = 0; i < Filter_Frequency_Order + 1; i++)
        {
            System_Function[i] = Basic_Math_Sinc(((float) (i) - Filter_Frequency_Order / 2.0f) * PI) + omega_low / PI * Basic_Math_Sinc(((float) (i) - Filter_Frequency_Order / 2.0f) * omega_low) - omega_high / PI * Basic_Math_Sinc(((float) (i) - Filter_Frequency_Order / 2.0f) * omega_high);
        }

        break;
    }
    }

    for (int i = 0; i < Filter_Frequency_Order + 1; i++)
    {
        system_function_sum += System_Function[i];
    }

    for (int i = 0; i < Filter_Frequency_Order + 1; i++)
    {
        System_Function[i] /= system_function_sum;
    }
}

/**
 * @brief 滤波器调整值, 周期与采样周期相同
 *
 * @tparam Filter_Frequency_Order 滤波器阶数
 */
template<uint32_t Filter_Frequency_Order>
void Class_Filter_Frequency<Filter_Frequency_Order>::TIM_Calculate_PeriodElapsedCallback()
{
    Out = 0.0f;

    // 执行卷积操作
    for (int i = 0; i < Filter_Frequency_Order + 1; i++)
    {
        Out += System_Function[i] * Input_Signal[(Signal_Flag + i) % (Filter_Frequency_Order + 1)];
    }
}

/**
 * @brief 获取输出值
 *
 * @return float 输出值
 */
template<uint32_t Filter_Frequency_Order>
inline float Class_Filter_Frequency<Filter_Frequency_Order>::Get_Out() const
{
    return (Out);
}

/**
 * @brief 设定当前值
 *
 * @param __Now 当前值
 */
template<uint32_t Filter_Frequency_Order>
inline void Class_Filter_Frequency<Filter_Frequency_Order>::Set_Now(const float &__Now)
{
    float now_value;

    // 输入限幅, 全0为不限制
    if (Value_Constrain_Low != 0.0f || Value_Constrain_High != 0.0f)
    {
        now_value = Basic_Math_Constrain(__Now, Value_Constrain_Low, Value_Constrain_High);
    }

    // 将当前值放入被卷积的信号中
    Input_Signal[Signal_Flag] = now_value;
    Signal_Flag++;

    // 若越界则轮回
    if (Signal_Flag == Filter_Frequency_Order + 1)
    {
        Signal_Flag = 0;
    }
}

#endif

/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/