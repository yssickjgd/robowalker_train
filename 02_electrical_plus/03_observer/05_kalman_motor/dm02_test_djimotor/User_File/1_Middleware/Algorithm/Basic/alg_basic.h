/**
 * @file alg_basic.h
 * @author yssickjgd 1345578933@qq.com
 * @brief 一些极其简易的数学
 * @version 0.1
 * @date 2023-08-29 0.1 23赛季定稿
 * @date 2023-11-10 1.1 修改成cpp
 * @date 2025-09-23 2.1 引入NaN判断
 *
 * @copyright Copyright (c) 2023-2025
 *
 */

#ifndef ALG_BASIC_H
#define ALG_BASIC_H

/* Includes ------------------------------------------------------------------*/

#include "arm_math.h"
#include <float.h>

/* Exported macros -----------------------------------------------------------*/

extern const float BASIC_MATH_RPM_TO_RADPS;
extern const float BASIC_MATH_DEG_TO_RAD;
extern const float BASIC_MATH_CELSIUS_TO_KELVIN;

/* Exported types ------------------------------------------------------------*/

/* Exported variables --------------------------------------------------------*/

/* Exported function declarations --------------------------------------------*/

void Basic_Math_Boolean_Logical_Not(bool *Value);

void Basic_Math_Endian_Reverse_16(void *Address);

uint16_t Basic_Math_Endian_Reverse_16(void *Source, void *Destination);

void Basic_Math_Endian_Reverse_32(void *Address);

uint32_t Basic_Math_Endian_Reverse_32(void *Source, void *Destination);

uint8_t Basic_Math_Sum_8(const uint8_t *Address, uint32_t Length);

uint16_t Basic_Math_Sum_16(const uint16_t *Address, uint32_t Length);

uint32_t Basic_Math_Sum_32(const uint32_t *Address, uint32_t Length);

float Basic_Math_Sinc(float x);

int32_t Basic_Math_Float_To_Int(float x, float Float_1, float Float_2, int32_t Int_1, int32_t Int_2);

float Basic_Math_Int_To_Float(int32_t x, int32_t Int_1, int32_t Int_2, float Float_1, float Float_2);

bool Basic_Math_Is_Invalid_Float(float x);

float Basic_Math_Modulus_Normalization(float x, float modulus);

/**
 * @brief 限幅函数
 *
 * @tparam Type 类型
 * @param x 传入数据
 * @param Min 最小值
 * @param Max 最大值
 * @return 输出值
 */
template<typename Type>
Type Basic_Math_Constrain(Type x, Type Min, Type Max)
{
    if (x < Min)
    {
        x = Min;
    }
    else if (x > Max)
    {
        x = Max;
    }
    return (x);
}

/**
 * @brief 限幅函数
 *
 * @tparam Type 类型
 * @param x 传入数据
 * @param Min 最小值
 * @param Max 最大值
 * @return 输出值
 */
template<typename Type>
Type Basic_Math_Constrain(Type *x, Type Min, Type Max)
{
    if (*x < Min)
    {
        *x = Min;
    }
    else if (*x > Max)
    {
        *x = Max;
    }
    return (*x);
}

/**
 * @brief 求绝对值
 *
 * @tparam Type 类型
 * @param x 传入数据
 * @return Type x的绝对值
 */
template<typename Type>
Type Basic_Math_Abs(Type x)
{
    return ((x > 0) ? x : -x);
}

#endif

/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/