/**
 * @file alg_quaternion.h
 * @author yssickjgd (1345578933@qq.com)
 * @brief 四元数计算相关支持库
 * @version 0.1
 * @date 2025-08-22 0.1 新建文档
 *
 * @copyright USTC-RoboWalker (c) 2025
 *
 */

#ifndef ALG_QUATERNION_H
#define ALG_QUATERNION_H

/* Includes ------------------------------------------------------------------*/

#include "1_Middleware/Algorithm/Matrix/alg_matrix.h"

/* Exported macros -----------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/**
 * @brief Reusable, 四元数运算
 *
 */
class Class_Quaternion_f32 : public Class_Matrix_f32<4, 1>
{
public:
    // 构造函数
    using Class_Matrix_f32<4, 1>::Class_Matrix_f32;

    Class_Quaternion_f32(float Real = 1.0f, float Imaginary_X = 0.0f, float Imaginary_Y = 0.0f, float Imaginary_Z = 0.0f)
    {
        Data[0] = Real;
        Data[1] = Imaginary_X;
        Data[2] = Imaginary_Y;
        Data[3] = Imaginary_Z;
    }

    Class_Quaternion_f32(Class_Matrix_f32<4, 1> Matrix)
    {
        Data[0] = Matrix[0][0];
        Data[1] = Matrix[1][0];
        Data[2] = Matrix[2][0];
        Data[3] = Matrix[3][0];
    }

    // 析构函数
    ~Class_Quaternion_f32() = default;

    // 赋值函数
    using Class_Matrix_f32<4, 1>::operator=;

    inline float &operator[](const int &Index)
    {
        return (Data[Index]);
    }

    using Class_Matrix_f32<4, 1>::operator+;

    using Class_Matrix_f32<4, 1>::operator-;

    inline Class_Quaternion_f32 operator*(const float &Value) const
    {
        Class_Quaternion_f32 result;
        result.Data[0] = Data[0] * Value;
        result.Data[1] = Data[1] * Value;
        result.Data[2] = Data[2] * Value;
        result.Data[3] = Data[3] * Value;
        return (result);
    }

    inline friend Class_Quaternion_f32 operator*(const float &Value, const Class_Quaternion_f32 &Quaternion)
    {
        Class_Quaternion_f32 result;
        result.Data[0] = Quaternion.Data[0] * Value;
        result.Data[1] = Quaternion.Data[1] * Value;
        result.Data[2] = Quaternion.Data[2] * Value;
        result.Data[3] = Quaternion.Data[3] * Value;
        return (result);
    }

    inline Class_Quaternion_f32 operator*(const Class_Quaternion_f32 &Quaternion) const
    {
        Class_Quaternion_f32 result;
        float q0q0 = Data[0] * Quaternion.Data[0];
        float q0q1 = Data[0] * Quaternion.Data[1];
        float q0q2 = Data[0] * Quaternion.Data[2];
        float q0q3 = Data[0] * Quaternion.Data[3];
        float q1q0 = Data[1] * Quaternion.Data[0];
        float q1q1 = Data[1] * Quaternion.Data[1];
        float q1q2 = Data[1] * Quaternion.Data[2];
        float q1q3 = Data[1] * Quaternion.Data[3];
        float q2q0 = Data[2] * Quaternion.Data[0];
        float q2q1 = Data[2] * Quaternion.Data[1];
        float q2q2 = Data[2] * Quaternion.Data[2];
        float q2q3 = Data[2] * Quaternion.Data[3];
        float q3q0 = Data[3] * Quaternion.Data[0];
        float q3q1 = Data[3] * Quaternion.Data[1];
        float q3q2 = Data[3] * Quaternion.Data[2];
        float q3q3 = Data[3] * Quaternion.Data[3];
        result.Data[0] = q0q0 - q1q1 - q2q2 - q3q3;
        result.Data[1] = q0q1 + q1q0 + q2q3 - q3q2;
        result.Data[2] = q0q2 - q1q3 + q2q0 + q3q1;
        result.Data[3] = q0q3 + q1q2 - q2q1 + q3q0;
        return (result);
    }

    using Class_Matrix_f32<4, 1>::operator/;

    inline Class_Quaternion_f32 operator/(const Class_Quaternion_f32 &Quaternion) const
    {
        Class_Quaternion_f32 result;
        Class_Quaternion_f32 divisor = Quaternion;
        float modulus_square = divisor.Data[0] * divisor.Data[0] + divisor.Data[1] * divisor.Data[1] + divisor.Data[2] * divisor.Data[2] + divisor.Data[3] * divisor.Data[3];
        if (modulus_square <= Matrix_Compare_Epsilon)
        {
            return (*this);
        }
        modulus_square = 1.0f / modulus_square;
        float q0q0 = Data[0] * divisor.Data[0];
        float q0q1 = Data[0] * -divisor.Data[1];
        float q0q2 = Data[0] * -divisor.Data[2];
        float q0q3 = Data[0] * -divisor.Data[3];
        float q1q0 = Data[1] * divisor.Data[0];
        float q1q1 = Data[1] * -divisor.Data[1];
        float q1q2 = Data[1] * -divisor.Data[2];
        float q1q3 = Data[1] * -divisor.Data[3];
        float q2q0 = Data[2] * divisor.Data[0];
        float q2q1 = Data[2] * -divisor.Data[1];
        float q2q2 = Data[2] * -divisor.Data[2];
        float q2q3 = Data[2] * -divisor.Data[3];
        float q3q0 = Data[3] * divisor.Data[0];
        float q3q1 = Data[3] * -divisor.Data[1];
        float q3q2 = Data[3] * -divisor.Data[2];
        float q3q3 = Data[3] * -divisor.Data[3];
        result.Data[0] = (q0q0 - q1q1 - q2q2 - q3q3) * modulus_square;
        result.Data[1] = (q0q1 + q1q0 + q2q3 - q3q2) * modulus_square;
        result.Data[2] = (q0q2 - q1q3 + q2q0 + q3q1) * modulus_square;
        result.Data[3] = (q0q3 + q1q2 - q2q1 + q3q0) * modulus_square;
        return (result);
    }

    using Class_Matrix_f32<4, 1>::operator+=;

    using Class_Matrix_f32<4, 1>::operator-=;

    inline Class_Quaternion_f32 &operator*=(const float &Value)
    {
        Data[0] *= Value;
        Data[1] *= Value;
        Data[2] *= Value;
        Data[3] *= Value;
        return (*this);
    }

    inline Class_Quaternion_f32 &operator*=(const Class_Quaternion_f32 &Quaternion)
    {
        float q0q0 = Data[0] * Quaternion.Data[0];
        float q0q1 = Data[0] * Quaternion.Data[1];
        float q0q2 = Data[0] * Quaternion.Data[2];
        float q0q3 = Data[0] * Quaternion.Data[3];
        float q1q0 = Data[1] * Quaternion.Data[0];
        float q1q1 = Data[1] * Quaternion.Data[1];
        float q1q2 = Data[1] * Quaternion.Data[2];
        float q1q3 = Data[1] * Quaternion.Data[3];
        float q2q0 = Data[2] * Quaternion.Data[0];
        float q2q1 = Data[2] * Quaternion.Data[1];
        float q2q2 = Data[2] * Quaternion.Data[2];
        float q2q3 = Data[2] * Quaternion.Data[3];
        float q3q0 = Data[3] * Quaternion.Data[0];
        float q3q1 = Data[3] * Quaternion.Data[1];
        float q3q2 = Data[3] * Quaternion.Data[2];
        float q3q3 = Data[3] * Quaternion.Data[3];
        Data[0] = q0q0 - q1q1 - q2q2 - q3q3;
        Data[1] = q0q1 + q1q0 + q2q3 - q3q2;
        Data[2] = q0q2 - q1q3 + q2q0 + q3q1;
        Data[3] = q0q3 + q1q2 - q2q1 + q3q0;
        return (*this);
    }

    inline Class_Quaternion_f32 &operator/=(const Class_Quaternion_f32 &Quaternion)
    {
        Class_Quaternion_f32 divisor = Quaternion;
        float modulus_square = divisor.Data[0] * divisor.Data[0] + divisor.Data[1] * divisor.Data[1] + divisor.Data[2] * divisor.Data[2] + divisor.Data[3] * divisor.Data[3];
        if (modulus_square <= Matrix_Compare_Epsilon)
        {
            return (*this);
        }
        modulus_square = 1.0f / modulus_square;
        float q0q0 = Data[0] * divisor.Data[0];
        float q0q1 = Data[0] * -divisor.Data[1];
        float q0q2 = Data[0] * -divisor.Data[2];
        float q0q3 = Data[0] * -divisor.Data[3];
        float q1q0 = Data[1] * divisor.Data[0];
        float q1q1 = Data[1] * -divisor.Data[1];
        float q1q2 = Data[1] * -divisor.Data[2];
        float q1q3 = Data[1] * -divisor.Data[3];
        float q2q0 = Data[2] * divisor.Data[0];
        float q2q1 = Data[2] * -divisor.Data[1];
        float q2q2 = Data[2] * -divisor.Data[2];
        float q2q3 = Data[2] * -divisor.Data[3];
        float q3q0 = Data[3] * divisor.Data[0];
        float q3q1 = Data[3] * -divisor.Data[1];
        float q3q2 = Data[3] * -divisor.Data[2];
        float q3q3 = Data[3] * -divisor.Data[3];
        Data[0] = (q0q0 - q1q1 - q2q2 - q3q3) * modulus_square;
        Data[1] = (q0q1 + q1q0 + q2q3 - q3q2) * modulus_square;
        Data[2] = (q0q2 - q1q3 + q2q0 + q3q1) * modulus_square;
        Data[3] = (q0q3 + q1q2 - q2q1 + q3q0) * modulus_square;
        return (*this);
    }

    using Class_Matrix_f32<4, 1>::operator==;

    using Class_Matrix_f32<4, 1>::operator!=;

    inline float Get_Real() const;

    inline Class_Matrix_f32<3, 1> Get_Imaginary() const;

    inline Class_Quaternion_f32 Get_Conjugate() const;

    inline Class_Quaternion_f32 Get_Inverse() const;

    using Class_Matrix_f32<4, 1>::Get_Modulus;

    inline Class_Matrix_f32<3, 1> Get_Euler_Angle() const;

    using Class_Matrix_f32<4, 1>::Get_Normalization;

    inline Class_Matrix_f32<4, 4> Get_Self_Matrix() const;

    inline Class_Matrix_f32<3, 3> Get_Rotation_Matrix() const;

    inline Class_Matrix_f32<4, 1> Get_Axis_Angle() const;

protected:
    // 初始化相关常量

    // 常量

    // 内部变量

    // 读变量

    // 写变量

    // 读写变量

    // 内部函数
};

/* Exported variables --------------------------------------------------------*/

namespace Namespace_ALG_Quaternion
{
    Class_Quaternion_f32 Zero();

    Class_Quaternion_f32 Unit_Real();

    Class_Quaternion_f32 Unit_Imaginary_X();

    Class_Quaternion_f32 Unit_Imaginary_Y();

    Class_Quaternion_f32 Unit_Imaginary_Z();

    Class_Quaternion_f32 From_Vector(const Class_Matrix_f32<3, 1> &Vector);

    Class_Quaternion_f32 From_Euler_Angle(const Class_Matrix_f32<3, 1> &Euler_Angle);

    Class_Quaternion_f32 From_Rotation_Matrix(const Class_Matrix_f32<3, 3> &Rotation_Matrix);

    Class_Quaternion_f32 From_Axis_Angle(const Class_Matrix_f32<3, 1> &Axis, const float &Angle);
}

/* Exported function declarations --------------------------------------------*/

/**
 * @brief 获取四元数的实部
 *
 * @return float 实部
 */
inline float Class_Quaternion_f32::Get_Real() const
{
    return (Data[0]);
}

/**
 * @brief 获取四元数的虚部
 *
 * @return Class_Matrix_f32<3, 1> 虚部
 */
inline Class_Matrix_f32<3, 1> Class_Quaternion_f32::Get_Imaginary() const
{
    return (Class_Matrix_f32 < 3, 1 > (&Data[1]));
}

/**
 * @brief 获取四元数的共轭四元数
 *
 * @return Class_Quaternion_f32 共轭四元数
 */
inline Class_Quaternion_f32 Class_Quaternion_f32::Get_Conjugate() const
{
    Class_Quaternion_f32 result;
    result.Data[0] = Data[0];
    result.Data[1] = -Data[1];
    result.Data[2] = -Data[2];
    result.Data[3] = -Data[3];
    return (result);
}

/**
 * @brief 获取四元数的逆元
 *
 * @return Class_Quaternion_f32 逆四元数
 */
inline Class_Quaternion_f32 Class_Quaternion_f32::Get_Inverse() const
{
    Class_Quaternion_f32 result;
    float modulus_square = Data[0] * Data[0] + Data[1] * Data[1] + Data[2] * Data[2] + Data[3] * Data[3];
    if (modulus_square <= Matrix_Compare_Epsilon)
    {
        return (Class_Quaternion_f32(1.0f, 0.0f, 0.0f, 0.0f));
    }
    modulus_square = 1.0f / modulus_square;
    result.Data[0] = Data[0] * modulus_square;
    result.Data[1] = -Data[1] * modulus_square;
    result.Data[2] = -Data[2] * modulus_square;
    result.Data[3] = -Data[3] * modulus_square;
    return (result);
}

/**
 * @brief 获取四元数对应的欧拉角, Z-Y-X顺序
 *
 * @return Class_Matrix_f32<3, 1> 欧拉角, Yaw-Pitch-Roll顺序
 * 注意, 这只是用矩阵形式存储, 不可参与矩阵计算
 */
inline Class_Matrix_f32<3, 1> Class_Quaternion_f32::Get_Euler_Angle() const
{
    Class_Matrix_f32 < 3, 1 > result;
    float modulus_square = Data[0] * Data[0] + Data[1] * Data[1] + Data[2] * Data[2] + Data[3] * Data[3];
    if (modulus_square <= Matrix_Compare_Epsilon)
    {
        return (result);
    }
    float modulus = 1.0f / sqrtf(modulus_square);
    float q0 = Data[0] * modulus;
    float q1 = Data[1] * modulus;
    float q2 = Data[2] * modulus;
    float q3 = Data[3] * modulus;
    result[0][0] = atan2f(2.0f * (q0 * q3 + q1 * q2), 1.0f - 2.0f * (q2 * q2 + q3 * q3));
    float sin_pitch = 2.0f * (q0 * q2 - q3 * q1);
    if (fabs(sin_pitch) >= 1.0f)
    {
        if (sin_pitch > 0.0f)
        {
            result[1][0] = PI / 2.0f;
        }
        else
        {
            result[1][0] = -PI / 2.0f;
        }
    }
    else
    {
        result[1][0] = asinf(sin_pitch);
    }
    result[2][0] = atan2f(2.0f * (q0 * q1 + q2 * q3), 1.0f - 2.0f * (q1 * q1 + q2 * q2));
    return (result);
}

/**
 * @brief 获取四元数的自乘矩阵
 *
 * @return Class_Matrix_f32<4, 4> 自乘矩阵
 */
inline Class_Matrix_f32<4, 4> Class_Quaternion_f32::Get_Self_Matrix() const
{
    Class_Matrix_f32 < 4, 4 > result;
    result[0][0] = Data[0];
    result[0][1] = -Data[1];
    result[0][2] = -Data[2];
    result[0][3] = -Data[3];
    result[1][0] = Data[1];
    result[1][1] = Data[0];
    result[1][2] = -Data[3];
    result[1][3] = Data[2];
    result[2][0] = Data[2];
    result[2][1] = Data[3];
    result[2][2] = Data[0];
    result[2][3] = -Data[1];
    result[3][0] = Data[3];
    result[3][1] = -Data[2];
    result[3][2] = Data[1];
    result[3][3] = Data[0];
    return (result);
}

/**
 * @brief 获取四元数对应的旋转矩阵
 *
 * @return Class_Matrix_f32<3, 3> 旋转矩阵
 */
inline Class_Matrix_f32<3, 3> Class_Quaternion_f32::Get_Rotation_Matrix() const
{
    Class_Matrix_f32 < 3, 3 > result;
    float modulus_square = Data[0] * Data[0] + Data[1] * Data[1] + Data[2] * Data[2] + Data[3] * Data[3];
    if (modulus_square <= Matrix_Compare_Epsilon)
    {
        return (Namespace_ALG_Matrix::Identity<3, 3>());
    }
    float modulus = 1.0f / sqrtf(modulus_square);
    float q0 = Data[0] * modulus;
    float q1 = Data[1] * modulus;
    float q2 = Data[2] * modulus;
    float q3 = Data[3] * modulus;
    float q0q1 = q0 * q1;
    float q0q2 = q0 * q2;
    float q0q3 = q0 * q3;
    float q1q1 = q1 * q1;
    float q1q2 = q1 * q2;
    float q1q3 = q1 * q3;
    float q2q2 = q2 * q2;
    float q2q3 = q2 * q3;
    float q3q3 = q3 * q3;
    result[0][0] = 1.0f - 2.0f * (q2q2 + q3q3);
    result[0][1] = 2.0f * (q1q2 - q0q3);
    result[0][2] = 2.0f * (q1q3 + q0q2);
    result[1][0] = 2.0f * (q1q2 + q0q3);
    result[1][1] = 1.0f - 2.0f * (q1q1 + q3q3);
    result[1][2] = 2.0f * (q2q3 - q0q1);
    result[2][0] = 2.0f * (q1q3 - q0q2);
    result[2][1] = 2.0f * (q2q3 + q0q1);
    result[2][2] = 1.0f - 2.0f * (q1q1 + q2q2);
    return (result);
}

/**
 * @brief 获取四元数对应的轴角表示, Axis-Angle顺序, 轴与x轴正方向夹角为正
 *
 * @return Class_Matrix_f32<4, 1> 轴角表示, Axis-Angle顺序
 * 注意, 这只是用矩阵形式存储, 不可参与矩阵计算
 */
inline Class_Matrix_f32<4, 1> Class_Quaternion_f32::Get_Axis_Angle() const
{
    Class_Matrix_f32 < 4, 1 > result;
    float modulus_square = Data[0] * Data[0] + Data[1] * Data[1] + Data[2] * Data[2] + Data[3] * Data[3];
    if (modulus_square <= Matrix_Compare_Epsilon)
    {
        result[0][0] = 1.0f;
        return (result);
    }
    float modulus = 1.0f / sqrtf(modulus_square);
    float q0 = Data[0] * modulus;
    float q1 = Data[1] * modulus;
    float q2 = Data[2] * modulus;
    float q3 = Data[3] * modulus;
    result[3][0] = 2.0f * acosf(q0);
    float sin_half_angle = sqrtf(1.0f - q0 * q0);
    if (sin_half_angle <= Matrix_Compare_Epsilon)
    {
        result[0][0] = 1.0f;
        result[1][0] = 0.0f;
        result[2][0] = 0.0f;
    }
    else
    {
        float inv_sin_half_angle = 1.0f / sin_half_angle;
        if (q1 < 0.0f)
        {
            inv_sin_half_angle = -inv_sin_half_angle;
        }
        result[0][0] = q1 * inv_sin_half_angle;
        result[1][0] = q2 * inv_sin_half_angle;
        result[2][0] = q3 * inv_sin_half_angle;
    }
    return (result);
}

#endif

/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/