/**
 * @file alg_quaternion.cpp
 * @author yssickjgd (1345578933@qq.com)
 * @brief 四元数计算相关支持库
 * @version 0.1
 * @date 2025-08-22 0.1 新建文档
 *
 * @copyright USTC-RoboWalker (c) 2025
 *
 */

#include "alg_quaternion.h"

/* Private macros ------------------------------------------------------------*/

/* Private types -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function declarations ---------------------------------------------*/

/**brief 获取零四元数
 *
 */
Class_Quaternion_f32 Namespace_ALG_Quaternion::Zero()
{
    Class_Quaternion_f32 result;
    result[0] = 0.0f;
    result[1] = 0.0f;
    result[2] = 0.0f;
    result[3] = 0.0f;
    return (result);
}

/**
 * @brief 获取单位实四元数
 *
 */
Class_Quaternion_f32 Namespace_ALG_Quaternion::Unit_Real()
{
    Class_Quaternion_f32 result;
    result[0] = 1.0f;
    result[1] = 0.0f;
    result[2] = 0.0f;
    result[3] = 0.0f;
    return (result);
}

/**
 * @brief 获取单位虚四元数X
 *
 */
Class_Quaternion_f32 Namespace_ALG_Quaternion::Unit_Imaginary_X()
{
    Class_Quaternion_f32 result;
    result[0] = 1.0f;
    result[1] = 0.0f;
    result[2] = 0.0f;
    result[3] = 0.0f;
    return (result);
}

/**
 * @brief 获取单位虚四元数Y
 *
 */
Class_Quaternion_f32 Namespace_ALG_Quaternion::Unit_Imaginary_Y()
{
    Class_Quaternion_f32 result;
    result[0] = 0.0f;
    result[1] = 0.0f;
    result[2] = 1.0f;
    result[3] = 0.0f;
    return (result);
}

/**
 * @brief 获取单位虚四元数Z
 *
 */
Class_Quaternion_f32 Namespace_ALG_Quaternion::Unit_Imaginary_Z()
{
    Class_Quaternion_f32 result;
    result[0] = 0.0f;
    result[1] = 0.0f;
    result[2] = 0.0f;
    result[3] = 1.0f;
    return (result);
}

/**brief 根据三维向量获取纯四元数
 *
 * @param Vector 三维向量
 * @return Class_Quaternion_f32 纯四元数
 */
Class_Quaternion_f32 Namespace_ALG_Quaternion::From_Vector(const Class_Matrix_f32<3, 1> &Vector)
{
    Class_Quaternion_f32 result;
    result[0] = 0.0f;
    result[1] = Vector[0][0];
    result[2] = Vector[1][0];
    result[3] = Vector[2][0];
    return (result);
}

/**
 * @brief 根据旋转矩阵获取四元数
 *
 * @param Rotation_Matrix 3x3旋转矩阵
 * @return Class_Quaternion_f32 四元数
 */
Class_Quaternion_f32 Namespace_ALG_Quaternion::From_Rotation_Matrix(const Class_Matrix_f32<3, 3> &Rotation_Matrix)
{
    Class_Quaternion_f32 result;
    float trace = Rotation_Matrix.Get_Trace();
    if (trace > 0.0f)
    {
        // 矩阵迹是正的

        float s = sqrtf(trace + 1.0f) * 2.0f;
        result[0] = 0.25f * s;

        result[1] = (Rotation_Matrix[2][1] - Rotation_Matrix[1][2]) / s;
        result[2] = (Rotation_Matrix[0][2] - Rotation_Matrix[2][0]) / s;
        result[3] = (Rotation_Matrix[1][0] - Rotation_Matrix[0][1]) / s;
    }
    else if ((Rotation_Matrix[0][0] >= Rotation_Matrix[1][1]) && (Rotation_Matrix[0][0] >= Rotation_Matrix[2][2]))
    {
        // r00是最大值

        float s = sqrtf(1.0f + Rotation_Matrix[0][0] - Rotation_Matrix[1][1] - Rotation_Matrix[2][2]) * 2.0f;

        result[0] = (Rotation_Matrix[2][1] - Rotation_Matrix[1][2]) / s;
        result[1] = 0.25f * s;
        result[2] = (Rotation_Matrix[0][1] + Rotation_Matrix[1][0]) / s;
        result[3] = (Rotation_Matrix[0][2] + Rotation_Matrix[2][0]) / s;
    }
    else if (Rotation_Matrix[1][1] >= Rotation_Matrix[2][2] && Rotation_Matrix[1][1] >= Rotation_Matrix[0][0])
    {
        // r11是最大值

        float s = sqrtf(1.0f + Rotation_Matrix[1][1] - Rotation_Matrix[0][0] - Rotation_Matrix[2][2]) * 2.0f;

        result[0] = (Rotation_Matrix[0][2] - Rotation_Matrix[2][0]) / s;
        result[1] = (Rotation_Matrix[0][1] + Rotation_Matrix[1][0]) / s;
        result[2] = 0.25f * s;
        result[3] = (Rotation_Matrix[1][2] + Rotation_Matrix[2][1]) / s;
    }
    else if (Rotation_Matrix[2][2] >= Rotation_Matrix[0][0] && Rotation_Matrix[2][2] >= Rotation_Matrix[1][1])
    {
        // r22是最大值

        float s = sqrtf(1.0f + Rotation_Matrix[2][2] - Rotation_Matrix[0][0] - Rotation_Matrix[1][1]) * 2.0f;

        result[0] = (Rotation_Matrix[1][0] - Rotation_Matrix[0][1]) / s;
        result[1] = (Rotation_Matrix[0][2] + Rotation_Matrix[2][0]) / s;
        result[2] = (Rotation_Matrix[1][2] + Rotation_Matrix[2][1]) / s;
        result[3] = 0.25f * s;
    }
    else
    {
        // 其他情况, 返回单位实四元数
        result = Namespace_ALG_Quaternion::Unit_Real();
    }

    return (result);
}

/**
 * @brief 根据旋转轴与旋转角度获取四元数
 *
 * @param Axis 旋转轴, 单位向量
 * @param Angle 旋转角度, 单位弧度
 * @return Class_Quaternion_f32 四元数
 */
Class_Quaternion_f32 Namespace_ALG_Quaternion::From_Axis_Angle(const Class_Matrix_f32<3, 1> &Axis, const float &Angle)
{
    Class_Quaternion_f32 result;
    float half_angle = Angle * 0.5f;
    float cos_half_angle = arm_cos_f32(half_angle);
    float sin_half_angle = arm_sin_f32(half_angle);
    result[0] = cos_half_angle;
    result[1] = Axis[0][0] * sin_half_angle;
    result[2] = Axis[1][0] * sin_half_angle;
    result[3] = Axis[2][0] * sin_half_angle;
    return (result);
}

/* Function prototypes -------------------------------------------------------*/

/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/