/**
 * @file alg_matrix.cpp
 * @author yssickjgd (1345578933@qq.com)
 * @brief 矩阵计算相关支持库
 * @version 0.1
 * @date 2025-08-21 0.1 新建文档
 *
 * @copyright USTC-RoboWalker (c) 2025
 *
 */

#include "alg_matrix.h"

/* Private macros ------------------------------------------------------------*/

/* Private types -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

// 矩阵比较大小时允许的最大误差
float Matrix_Compare_Epsilon = 1e-6f;

/* Private function declarations ---------------------------------------------*/

/**
 * @brief 获取2D空间的X轴单位向量
 *
 */
Class_Matrix_f32<2, 1> Namespace_ALG_Matrix::Axis_X_2d()
{
    Class_Matrix_f32<2, 1> result;
    result[0][0] = 1.0f;
    result[1][0] = 0.0f;
    return (result);
}

/**
 * @brief 获取2D空间的Y轴单位向量
 *
 */
Class_Matrix_f32<2, 1> Namespace_ALG_Matrix::Axis_Y_2d()
{
    Class_Matrix_f32<2, 1> result;
    result[0][0] = 0.0f;
    result[1][0] = 1.0f;
    return (result);
}

/**
 * @brief 获取X轴单位向量
 *
 */
Class_Matrix_f32<3, 1> Namespace_ALG_Matrix::Axis_X_3d()
{
    Class_Matrix_f32<3, 1> result;
    result[0][0] = 1.0f;
    result[1][0] = 0.0f;
    result[2][0] = 0.0f;
    return (result);
}

/**
 * @brief 获取Y轴单位向量
 *
 */
Class_Matrix_f32<3, 1> Namespace_ALG_Matrix::Axis_Y_3d()
{
    Class_Matrix_f32<3, 1> result;
    result[0][0] = 0.0f;
    result[1][0] = 1.0f;
    result[2][0] = 0.0f;
    return (result);
}

/**
 * @brief 获取Z轴单位向量
 *
 */
Class_Matrix_f32<3, 1> Namespace_ALG_Matrix::Axis_Z_3d()
{
    Class_Matrix_f32<3, 1> result;
    result[0][0] = 0.0f;
    result[1][0] = 0.0f;
    result[2][0] = 1.0f;
    return (result);
}

/**
 * @brief 获取二维空间的旋转矩阵
 *
 * @param Angle 旋转角度, 单位弧度
 * @return Class_Matrix_f32<2, 2> 旋转矩阵
 */
Class_Matrix_f32<2, 2> Namespace_ALG_Matrix::From_Angle(const float &Angle)
{
    Class_Matrix_f32<2, 2> result;
    float tmp_cos, tmp_sin;
    tmp_cos = arm_cos_f32(Angle);
    tmp_sin = arm_sin_f32(Angle);
    result[0][0] = tmp_cos;
    result[0][1] = -tmp_sin;
    result[1][0] = tmp_sin;
    result[1][1] = tmp_cos;
    return (result);
}

/**
 * @brief 获取三维空间的旋转矩阵
 *
 * @param Angle 旋转角度, 单位弧度
 * @param Axis 旋转轴, 单位向量
 * @return Class_Matrix_f32<3, 3> 旋转矩阵
 */
Class_Matrix_f32<3, 3> Namespace_ALG_Matrix::From_Axis_Angle(const float &Angle, const Class_Matrix_f32<3, 1> &Axis)
{
    Class_Matrix_f32<3, 3> result;
    float tmp_cos, tmp_sin;
    tmp_cos = arm_cos_f32(Angle);
    tmp_sin = arm_sin_f32(Angle);
    result[0][0] = tmp_cos + Axis[0][0] * Axis[0][0] * (1.0f - tmp_cos);
    result[0][1] = Axis[0][0] * Axis[1][0] * (1.0f - tmp_cos) - Axis[2][0] * tmp_sin;
    result[0][2] = Axis[0][0] * Axis[2][0] * (1.0f - tmp_cos) + Axis[1][0] * tmp_sin;
    result[1][0] = Axis[1][0] * Axis[0][0] * (1.0f - tmp_cos) + Axis[2][0] * tmp_sin;
    result[1][1] = tmp_cos + Axis[1][0] * Axis[1][0] * (1.0f - tmp_cos);
    result[1][2] = Axis[1][0] * Axis[2][0] * (1.0f - tmp_cos) - Axis[0][0] * tmp_sin;
    result[2][0] = Axis[2][0] * Axis[0][0] * (1.0f - tmp_cos) - Axis[1][0] * tmp_sin;
    result[2][1] = Axis[2][0] * Axis[1][0] * (1.0f - tmp_cos) + Axis[0][0] * tmp_sin;
    result[2][2] = tmp_cos + Axis[2][0] * Axis[2][0] * (1.0f - tmp_cos);
    return (result);
}

/**
 * @brief 获取三维空间的旋转矩阵
 *
 * @param Yaw 偏航角, 单位弧度
 * @param Pitch 俯仰角, 单位弧度
 * @param Roll 横滚角, 单位弧度
 * @return Class_Matrix_f32<3, 3> 旋转矩阵
 */
Class_Matrix_f32<3, 3> Namespace_ALG_Matrix::From_Euler_Angle(const float &Yaw, const float &Pitch, const float &Roll)
{
    Class_Matrix_f32<3, 3> result;
    float cy = arm_cos_f32(Yaw);
    float sy = arm_sin_f32(Yaw);
    float cp = arm_cos_f32(Pitch);
    float sp = arm_sin_f32(Pitch);
    float cr = arm_cos_f32(Roll);
    float sr = arm_sin_f32(Roll);

    result[0][0] = cy * cp;
    result[0][1] = cy * sp * sr - sy * cr;
    result[0][2] = cy * sp * cr + sy * sr;

    result[1][0] = sy * cp;
    result[1][1] = sy * sp * sr + cy * cr;
    result[1][2] = sy * sp * cr - cy * sr;

    result[2][0] = -sp;
    result[2][1] = cp * sr;
    result[2][2] = cp * cr;

    return (result);
}

/**
 * @brief 计算两个三维列向量的叉乘
 *
 * @param Vector_1 向量1
 * @param Vector_2 向量2
 * @return Class_Matrix_f32<3, 1> 叉乘结果
 */
Class_Matrix_f32<3, 1> Namespace_ALG_Matrix::Operator_Cross_3d(const Class_Matrix_f32<3, 1> &Vector_1, const Class_Matrix_f32<3, 1> &Vector_2)
{
    Class_Matrix_f32<3, 1> result;
    result[0][0] = Vector_1[1][0] * Vector_2[2][0] - Vector_1[2][0] * Vector_2[1][0];
    result[1][0] = Vector_1[2][0] * Vector_2[0][0] - Vector_1[0][0] * Vector_2[2][0];
    result[2][0] = Vector_1[0][0] * Vector_2[1][0] - Vector_1[1][0] * Vector_2[0][0];
    return (result);
}

/**
 * @brief 计算两个二维列向量的叉乘
 *
 * @param Vector_1 向量1
 * @param Vector_2 向量2
 * @return float 叉乘结果
 */
float Namespace_ALG_Matrix::Operator_Cross_2d(const Class_Matrix_f32<2, 1> &Vector_1, const Class_Matrix_f32<2, 1> &Vector_2)
{
    float result;
    result = Vector_1[0][0] * Vector_2[1][0] - Vector_1[1][0] * Vector_2[0][0];
    return (result);
}

/* Function prototypes -------------------------------------------------------*/

/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/
