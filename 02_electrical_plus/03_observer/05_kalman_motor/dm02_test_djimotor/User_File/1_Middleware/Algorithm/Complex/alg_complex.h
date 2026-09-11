/**
 * @file alg_complex.h
 * @author yssickjgd (1345578933@qq.com)
 * @brief 复数计算相关支持库
 * @version 0.1
 * @date 2025-08-22 0.1 新建文档
 *
 * @copyright USTC-RoboWalker (c) 2025
 *
 */

#ifndef ALG_COMPLEX_H
#define ALG_COMPLEX_H

/* Includes ------------------------------------------------------------------*/

#include "1_Middleware/Algorithm/Matrix/alg_matrix.h"

/* Exported macros -----------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/**
 * @brief Reusable, 复数运算, 由于不涉及批处理因此用硬算版本即可
 *
 */
class Class_Complex_f32 : public Class_Matrix_f32<2, 1>
{
public:
    // 构造函数
    using Class_Matrix_f32<2, 1>::Class_Matrix_f32;

    Class_Complex_f32(float Real = 1.0f, float Imaginary = 0.0f)
    {
        Data[0] = Real;
        Data[1] = Imaginary;
    }

    Class_Complex_f32(Class_Matrix_f32<2, 1> Matrix)
    {
        Data[0] = Matrix[0][0];
        Data[1] = Matrix[1][0];
    }

    // 析构函数
    ~Class_Complex_f32() = default;

    // 赋值函数
    using Class_Matrix_f32<2, 1>::operator=;

    inline float &operator[](const int &Index)
    {
        return (Data[Index]);
    }

    using Class_Matrix_f32<2, 1>::operator+;

    using Class_Matrix_f32<2, 1>::operator-;

    inline Class_Complex_f32 operator*(const float &Value) const
    {
        Class_Complex_f32 result;
        result.Data[0] = Data[0] * Value;
        result.Data[1] = Data[1] * Value;
        return (result);
    }

    inline friend Class_Complex_f32 operator*(const float &Value, const Class_Complex_f32 &Complex)
    {
        Class_Complex_f32 result;
        result.Data[0] = Complex.Data[0] * Value;
        result.Data[1] = Complex.Data[1] * Value;
        return (result);
    }

    inline Class_Complex_f32 operator*(const Class_Complex_f32 &Complex) const
    {
        Class_Complex_f32 result;
        float c0c0 = Data[0] * Complex.Data[0];
        float c0c1 = Data[0] * Complex.Data[1];
        float c1c0 = Data[1] * Complex.Data[0];
        float c1c1 = Data[1] * Complex.Data[1];
        result.Data[0] = c0c0 - c1c1;
        result.Data[1] = c0c1 + c1c0;
        return (result);
    }

    using Class_Matrix_f32<2, 1>::operator/;

    inline Class_Complex_f32 operator/(const Class_Complex_f32 &Complex) const
    {
        Class_Complex_f32 result;
        Class_Complex_f32 divisor = Complex;
        float modulus_square = divisor.Data[0] * divisor.Data[0] + divisor.Data[1] * divisor.Data[1];
        if (modulus_square <= Matrix_Compare_Epsilon)
        {
            return (*this);
        }
        modulus_square = 1.0f / modulus_square;
        float c0c0 = Data[0] * divisor.Data[0];
        float c0c1 = Data[0] * -divisor.Data[1];
        float c1c0 = Data[1] * divisor.Data[0];
        float c1c1 = Data[1] * -divisor.Data[1];
        result.Data[0] = (c0c0 - c1c1) * modulus_square;
        result.Data[1] = (c0c1 + c1c0) * modulus_square;
        return (result);
    }

    using Class_Matrix_f32<2, 1>::operator+=;

    using Class_Matrix_f32<2, 1>::operator-=;

    inline Class_Complex_f32 &operator*=(const float &Value)
    {
        Data[0] *= Value;
        Data[1] *= Value;
        return (*this);
    }

    inline Class_Complex_f32 &operator*=(const Class_Complex_f32 &Complex)
    {
        float c0c0 = Data[0] * Complex.Data[0];
        float c0c1 = Data[0] * Complex.Data[1];
        float c1c0 = Data[1] * Complex.Data[0];
        float c1c1 = Data[1] * Complex.Data[1];
        Data[0] = c0c0 - c1c1;
        Data[1] = c0c1 + c1c0;
        return (*this);
    }

    using Class_Matrix_f32<2, 1>::operator/=;

    inline Class_Complex_f32 &operator/=(const Class_Complex_f32 &Complex)
    {
        Class_Complex_f32 divisor = Complex;
        float modulus_square = divisor.Data[0] * divisor.Data[0] + divisor.Data[1] * divisor.Data[1];
        if (modulus_square <= Matrix_Compare_Epsilon)
        {
            return (*this);
        }
        modulus_square = 1.0f / modulus_square;
        float c0c0 = Data[0] * divisor.Data[0];
        float c0c1 = Data[0] * -divisor.Data[1];
        float c1c0 = Data[1] * divisor.Data[0];
        float c1c1 = Data[1] * -divisor.Data[1];
        Data[0] = (c0c0 - c1c1) * modulus_square;
        Data[1] = (c0c1 + c1c0) * modulus_square;
        return (*this);
    }

    using Class_Matrix_f32<2, 1>::operator==;

    using Class_Matrix_f32<2, 1>::operator!=;

    inline float Get_Real() const;

    inline float Get_Imaginary() const;

    inline Class_Complex_f32 Get_Conjugate() const;

    inline Class_Complex_f32 Get_Inverse() const;

    using Class_Matrix_f32<2, 1>::Get_Modulus;

    inline float Get_Angle() const;

    using Class_Matrix_f32<2, 1>::Get_Normalization;

    inline Class_Matrix_f32<2, 2> Get_Self_Matrix() const;

    inline Class_Matrix_f32<2, 2> Get_Rotation_Matrix() const;

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

namespace Namespace_ALG_Complex
{
    Class_Complex_f32 Zero();

    Class_Complex_f32 Unit_Real();

    Class_Complex_f32 Unit_Imaginary();

    Class_Complex_f32 Angle_Unit(const float &Angle);

    Class_Complex_f32 Polar(const float &Modulus, const float &Angle);
}

/* Exported function declarations --------------------------------------------*/

/**
 * @brief 获取复数的实部
 *
 * @return float 实部
 */
inline float Class_Complex_f32::Get_Real() const
{
    return (Data[0]);
}

/**
 * @brief 获取复数的虚部
 *
 * @return float 虚部
 */
inline float Class_Complex_f32::Get_Imaginary() const
{
    return (Data[1]);
}

/**
 * @brief 获取复数的共轭复数
 *
 * @return Class_Complex_f32 共轭复数
 */
inline Class_Complex_f32 Class_Complex_f32::Get_Conjugate() const
{
    Class_Complex_f32 result;
    result.Data[0] = Data[0];
    result.Data[1] = -Data[1];
    return (result);
}

/**
 * @brief 获取复数的逆元
 *
 * @return Class_Complex_f32 倒数
 */
inline Class_Complex_f32 Class_Complex_f32::Get_Inverse() const
{
    Class_Complex_f32 result;
    float modulus_square = Data[0] * Data[0] + Data[1] * Data[1];
    if (modulus_square <= Matrix_Compare_Epsilon)
    {
        return (Class_Complex_f32(1.0f, 0.0f));
    }
    modulus_square = 1.0f / modulus_square;
    result.Data[0] = Data[0] * modulus_square;
    result.Data[1] = -Data[1] * modulus_square;
    return (result);
}

/**
 * @brief 获取复数的幅角, 范围[-pi, pi]
 *
 * @return float 幅角
 */
inline float Class_Complex_f32::Get_Angle() const
{
    return (atan2f(Data[1], Data[0]));
}

/**
 * @brief 获取复数对应的2x2矩阵表示
 *
 * @return Class_Matrix_f32<2, 2> 2x2矩阵
 */
inline Class_Matrix_f32<2, 2> Class_Complex_f32::Get_Self_Matrix() const
{
    Class_Matrix_f32 < 2, 2 > result;
    result[0][0] = Data[0];
    result[0][1] = -Data[1];
    result[1][0] = Data[1];
    result[1][1] = Data[0];
    return (result);
}

/**
 * @brief 获取复数对应的旋转矩阵表示
 *
 * @return Class_Matrix_f32<2, 2> 2x2旋转矩阵
 */
inline Class_Matrix_f32<2, 2> Class_Complex_f32::Get_Rotation_Matrix() const
{
    Class_Matrix_f32 < 2, 2 > result;
    float modulus_square = Data[0] * Data[0] + Data[1] * Data[1];
    if (modulus_square <= Matrix_Compare_Epsilon)
    {
        return (Namespace_ALG_Matrix::Identity<2, 2>());
    }
    float modulus = 1.0f / sqrtf(modulus_square);
    float cos_theta = Data[0] * modulus;
    float sin_theta = Data[1] * modulus;
    result[0][0] = cos_theta;
    result[0][1] = -sin_theta;
    result[1][0] = sin_theta;
    result[1][1] = cos_theta;
    return (result);
}

#endif

/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/