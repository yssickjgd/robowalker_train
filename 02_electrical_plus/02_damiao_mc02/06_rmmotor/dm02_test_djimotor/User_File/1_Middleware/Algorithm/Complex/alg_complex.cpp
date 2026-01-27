/**
 * @file alg_complex.cpp
 * @author yssickjgd (1345578933@qq.com)
 * @brief 复数计算相关支持库
 * @version 0.1
 * @date 2025-08-22 0.1 新建文档
 *
 * @copyright USTC-RoboWalker (c) 2025
 *
 */

#include "alg_complex.h"

/* Private macros ------------------------------------------------------------*/

/* Private types -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function declarations ---------------------------------------------*/

/**
 * @brief 获取零复数
 *
 */
Class_Complex_f32 Namespace_ALG_Complex::Zero()
{
    Class_Complex_f32 result;
    result[0] = 0.0f;
    result[1] = 0.0f;
    return (result);
}

/**
 * @brief 获取单位实复数
 *
 */
Class_Complex_f32 Namespace_ALG_Complex::Unit_Real()
{
    Class_Complex_f32 result;
    result[0] = 1.0f;
    result[1] = 0.0f;
    return (result);
}

/**
 * @brief 获取单位虚复数
 *
 */
Class_Complex_f32 Namespace_ALG_Complex::Unit_Imaginary()
{
    Class_Complex_f32 result;
    result[0] = 0.0f;
    result[1] = 1.0f;
    return (result);
}

/**
 * @brief 获取模长为1的旋转复数
 *
 * @param Angle 旋转角度, 单位: 弧度
 */
Class_Complex_f32 Namespace_ALG_Complex::Angle_Unit(const float &Angle)
{
    Class_Complex_f32 result;
    result[0] = arm_cos_f32(Angle);
    result[1] = arm_sin_f32(Angle);
    return (result);
}

/**
 * @brief 获取根据极坐标的表示来定义的复数
 *
 * @param Modulus 复数模长
 * @param Angle 复数幅角
 */
Class_Complex_f32 Namespace_ALG_Complex::Polar(const float &Modulus, const float &Angle)
{
    Class_Complex_f32 result;
    result[0] = Modulus * arm_cos_f32(Angle);
    result[1] = Modulus * arm_sin_f32(Angle);
    return (result);
}

/* Function prototypes -------------------------------------------------------*/

/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/