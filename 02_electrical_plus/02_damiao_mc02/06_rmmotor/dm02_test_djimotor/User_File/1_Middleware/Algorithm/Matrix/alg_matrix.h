/**
 * @file alg_matrix.h
 * @author yssickjgd (1345578933@qq.com)
 * @brief 矩阵计算相关支持库
 * @version 0.1
 * @date 2025-08-21 0.1 新建文档
 *
 * @copyright USTC-RoboWalker (c) 2025
 *
 */

#ifndef ALG_MATRIX_H
#define ALG_MATRIX_H

/* Includes ------------------------------------------------------------------*/

#include "1_Middleware/Algorithm/Basic/alg_basic.h"

/* Exported macros -----------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

extern float Matrix_Compare_Epsilon;

/**
 * @brief Reusable, 矩阵运算
 *
 */
template<int row, int column>
class Class_Matrix_f32
{
public:
    // 矩阵数据区域
    float Data[row * column] = {0.0f};

    // 构造函数
    Class_Matrix_f32()
    {
        for (int i = 0; i < row * column; i++)
        {
            Data[i] = 0.0f;
        }
    }

    Class_Matrix_f32(const float __Data[row * column])
    {
        memcpy(Data, __Data, sizeof(float) * row * column);
    }

    // 复制构造函数
    Class_Matrix_f32(const Class_Matrix_f32<row, column> &Matrix)
    {
        memcpy(Data, Matrix.Data, sizeof(float) * row * column);
    }

    // 移动构造函数
    Class_Matrix_f32(Class_Matrix_f32<row, column> &&Matrix) noexcept
    {
        memcpy(Data, Matrix.Data, sizeof(float) * row * column);
    }

    // 析构函数
    ~Class_Matrix_f32() = default;

    // 复制赋值函数
    Class_Matrix_f32<row, column> &operator=(const Class_Matrix_f32<row, column> &Matrix)
    {
        if (this != &Matrix)
        {
            memcpy(Data, Matrix.Data, sizeof(float) * row * column);
        }
        return (*this);
    }

    // 移动赋值函数
    Class_Matrix_f32<row, column> &operator=(Class_Matrix_f32<row, column> &&Matrix) noexcept
    {
        memcpy(Data, Matrix.Data, sizeof(float) * row * column);
        return (*this);
    }

    inline float *operator[](const int &Index)
    {
        return (&Data[Index * column]);
    }

    inline const float *operator[](const int &Index) const
    {
        return (&Data[Index * column]);
    }

    inline Class_Matrix_f32<row, column> operator+() const
    {
        return (*this);
    }

    inline Class_Matrix_f32<row, column> operator+(const float &Value) const
    {
        Class_Matrix_f32<row, column> result;
        for (int i = 0; i < row * column; i++)
        {
            result.Data[i] = Data[i] + Value;
        }
        return (result);
    }

    inline Class_Matrix_f32<row, column> operator+(const Class_Matrix_f32<row, column> &Matrix) const
    {
        Class_Matrix_f32<row, column> result;
        for (int i = 0; i < row * column; i++)
        {
            result.Data[i] = Data[i] + Matrix.Data[i];
        }
        return (result);
    }

    inline Class_Matrix_f32<row, column> operator-() const
    {
        Class_Matrix_f32<row, column> result;
        for (int i = 0; i < row * column; i++)
        {
            result.Data[i] = -Data[i];
        }
        return (result);
    }

    inline Class_Matrix_f32<row, column> operator-(const float &Value) const
    {
        Class_Matrix_f32<row, column> result;
        for (int i = 0; i < row * column; i++)
        {
            result.Data[i] = Data[i] - Value;
        }
        return (result);
    }

    inline Class_Matrix_f32<row, column> operator-(const Class_Matrix_f32<row, column> &Matrix) const
    {
        Class_Matrix_f32<row, column> result;
        for (int i = 0; i < row * column; i++)
        {
            result.Data[i] = Data[i] - Matrix.Data[i];
        }
        return (result);
    }

    inline Class_Matrix_f32<row, column> operator*(const float &Value) const
    {
        Class_Matrix_f32<row, column> result;
        for (int i = 0; i < row * column; i++)
        {
            result.Data[i] = Data[i] * Value;
        }
        return (result);
    }

    inline friend Class_Matrix_f32<row, column> operator*(const float &Value, const Class_Matrix_f32<row, column> &Matrix)
    {
        Class_Matrix_f32<row, column> result;
        for (int i = 0; i < row * column; i++)
        {
            result.Data[i] = Value * Matrix.Data[i];
        }
        return (result);
    }

    template<int tmp>
    inline Class_Matrix_f32<row, tmp> operator*(const Class_Matrix_f32<column, tmp> &Matrix)
    {
        Class_Matrix_f32<row, tmp> result;
        for (int i = 0; i < row; i++)
        {
            for (int k = 0; k < column; k++)
            {
                float data_ik = Data[i * column + k];
                int result_index = i * tmp;
                int matrix_index = k * tmp;
                for (int j = 0; j < tmp; j++)
                {
                    result.Data[result_index + j] += data_ik * Matrix.Data[matrix_index + j];
                }
            }
        }
        return (result);
    }

    inline Class_Matrix_f32<row, column> operator/(const float &Value) const
    {
        if (fabs(Value) <= Matrix_Compare_Epsilon)
        {
            return (*this);
        }
        Class_Matrix_f32<row, column> result;
        for (int i = 0; i < row * column; i++)
        {
            result.Data[i] = Data[i] / Value;
        }
        return (result);
    }

    inline Class_Matrix_f32<row, column> &operator+=(const float &Value)
    {
        for (int i = 0; i < row * column; i++)
        {
            Data[i] += Value;
        }
        return (*this);
    }

    inline Class_Matrix_f32<row, column> &operator+=(const Class_Matrix_f32<row, column> &Matrix)
    {
        for (int i = 0; i < row * column; i++)
        {
            Data[i] += Matrix.Data[i];
        }
        return (*this);
    }

    inline Class_Matrix_f32<row, column> &operator-=(const float &Value)
    {
        for (int i = 0; i < row * column; i++)
        {
            Data[i] -= Value;
        }
        return (*this);
    }

    inline Class_Matrix_f32<row, column> &operator-=(const Class_Matrix_f32<row, column> &Matrix)
    {
        for (int i = 0; i < row * column; i++)
        {
            Data[i] -= Matrix.Data[i];
        }
        return (*this);
    }

    inline Class_Matrix_f32<row, column> &operator*=(const float &Value)
    {
        for (int i = 0; i < row * column; i++)
        {
            Data[i] *= Value;
        }
        return (*this);
    }

    inline Class_Matrix_f32<row, column> &operator/=(float &Value)
    {
        if (fabs(Value) <= Matrix_Compare_Epsilon)
        {
            return (*this);
        }
        for (int i = 0; i < row * column; i++)
        {
            Data[i] /= Value;
        }
        return (*this);
    }

    template<int tmp_row, int tmp_column>
    inline bool operator==(const Class_Matrix_f32<tmp_row, tmp_column> &Matrix) const
    {
        if (row == tmp_row && column == tmp_column)
        {
            for (int i = 0; i < row * column; i++)
            {
                if (fabs(Data[i] - Matrix.Matrix_Data[i]) > Matrix_Compare_Epsilon)
                {
                    return (false);
                }
            }
            return (true);
        }
        else
        {
            return (false);
        }
    }

    template<int tmp_row, int tmp_column>
    inline Class_Matrix_f32<row, column> operator!=(const Class_Matrix_f32<row, column> &Matrix) const
    {
        if (row != tmp_row || column != tmp_column)
        {
            return (true);
        }
        else
        {
            for (int i = 0; i < row * column; i++)
            {
                if (fabs(Data[i] - Matrix.Data[i]) > Matrix_Compare_Epsilon)
                {
                    return (true);
                }
            }
            return (false);
        }
    }

    inline Class_Matrix_f32<column, row> Get_Transpose() const;

    inline float Get_Trace() const;

    inline Class_Matrix_f32<row, 1> Get_Row(const int &Row_Index) const;

    inline Class_Matrix_f32<1, column> Get_Column(const int &Column_Index) const;

    // 方阵特有

    template<int tmp_row = row, int tmp_column = column>
    inline std::enable_if_t<tmp_row == tmp_column, Class_Matrix_f32<tmp_row, tmp_row>> &operator*=(const Class_Matrix_f32<tmp_row, tmp_row> &Matrix)
    {
        Class_Matrix_f32<tmp_row, tmp_row> result;
        for (int i = 0; i < tmp_row; i++)
        {
            for (int k = 0; k < tmp_row; k++)
            {
                float data_ik = Data[i * tmp_row + k];
                int result_index = i * tmp_row;
                int matrix_index = k * tmp_row;
                for (int j = 0; j < tmp_row; j++)
                {
                    result.Data[result_index + j] += data_ik * Matrix.Data[matrix_index + j];
                }
            }
        }
        *this = result;
        return (*this);
    }

    template<int tmp_row = row, int tmp_column = column>
    inline std::enable_if_t<tmp_row == tmp_column, Class_Matrix_f32<tmp_row, tmp_row>> Get_Inverse() const;

    // 列向量特有

    template<int tmp_row = row, int tmp_column = column>
    inline std::enable_if_t<tmp_column == 1, float> Get_Modulus() const;

    template<int tmp_row = row, int tmp_column = column>
    inline std::enable_if_t<tmp_column == 1, Class_Matrix_f32<tmp_row, 1>> Get_Normalization() const;

    // 三维列向量特有

    template<int tmp_row = row, int tmp_column = column>
    inline std::enable_if_t<tmp_row == 3 && tmp_column == 1, Class_Matrix_f32<3, 3>> Get_Cross_Matrix() const;

    // 单个元素特有

    template<int tmp_row = row, int tmp_column = column>
    inline std::enable_if_t<tmp_row == 1 && tmp_column == 1, float> Get_Single_Value() const;

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

namespace Namespace_ALG_Matrix
{
    Class_Matrix_f32<2, 1> Axis_X_2d();

    Class_Matrix_f32<2, 1> Axis_Y_2d();

    Class_Matrix_f32<3, 1> Axis_X_3d();

    Class_Matrix_f32<3, 1> Axis_Y_3d();

    Class_Matrix_f32<3, 1> Axis_Z_3d();

    Class_Matrix_f32<2, 2> From_Angle(const float &Angle);

    Class_Matrix_f32<3, 3> From_Axis_Angle(const float &Angle, const Class_Matrix_f32<3, 1> &Axis);

    Class_Matrix_f32<3, 3> From_Euler_Angle(const float &Yaw, const float &Pitch, const float &Roll);

    template<int row, int column>
    Class_Matrix_f32<row, column> Zero();

    template<int row, int column>
    Class_Matrix_f32<row, column> Identity();

    template<int row, int column>
    Class_Matrix_f32<row, column> Constant(const float &Value);

    template<int row>
    float Operator_Dot(const Class_Matrix_f32<row, 1> &Vector_1, const Class_Matrix_f32<row, 1> &Vector_2);

    Class_Matrix_f32<3, 1> Operator_Cross_3d(const Class_Matrix_f32<3, 1> &Vector_1, const Class_Matrix_f32<3, 1> &Vector_2);

    float Operator_Cross_2d(const Class_Matrix_f32<2, 1> &Vector_1, const Class_Matrix_f32<2, 1> &Vector_2);
}

/* Exported function declarations --------------------------------------------*/

/**
 * @brief 矩阵转置, 返回转置矩阵
 *
 * @tparam row 行数
 * @tparam column 列数
 * @return Class_Matrix_f32<column, row> 转置矩阵
 */
template<int row, int column>
inline Class_Matrix_f32<column, row> Class_Matrix_f32<row, column>::Get_Transpose() const
{
    Class_Matrix_f32<column, row> result;

    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < column; j++)
        {
            result[j][i] = Data[i * column + j];
        }
    }

    return (result);
}

/**
 * @brief 矩阵迹, 返回矩阵对角线元素之和
 *
 * @tparam row 行数
 * @tparam column 列数
 * @return float 矩阵迹
 */
template<int row, int column>
inline float Class_Matrix_f32<row, column>::Get_Trace() const
{
    float result = 0.0f;
    for (int i = 0; i < fmin(row, column); i++)
    {
        result += Data[i * column + i];
    }
    return (result);
}

/**
 * @brief 获取矩阵的某一行
 *
 * @tparam row 行数
 * @tparam column 列数
 * @param Row_Index 行索引
 * @return Class_Matrix_f32<row, 1> 行向量
 */
template<int row, int column>
inline Class_Matrix_f32<row, 1> Class_Matrix_f32<row, column>::Get_Row(const int &Row_Index) const
{
    Class_Matrix_f32<row, 1> result;

    memcpy(result.Data, &Data[Row_Index * column], sizeof(float) * column);

    return (result);
}

/**
 * @brief 获取矩阵的某一列
 *
 * @tparam row 行数
 * @tparam column 列数
 * @param Column_Index 列索引
 * @return Class_Matrix_f32<1, column> 列向量
 */
template<int row, int column>
inline Class_Matrix_f32<1, column> Class_Matrix_f32<row, column>::Get_Column(const int &Column_Index) const
{
    float result[column];

    for (int i = 0; i < row; i++)
    {
        result.Matrix_Data[i] = Data[i * column + Column_Index];
    }

    return (Class_Matrix_f32<1, column>(result));
}

/**
 * @brief 获取矩阵的逆矩阵
 *
 * @tparam row 行数
 * @tparam column 列数
 * @tparam tmp_row 行数
 * @tparam tmp_column 列数
 * @return Class_Matrix_f32<tmp_row, tmp_column> 逆矩阵
 */
template<int row, int column>
template<int tmp_row, int tmp_column>
inline std::enable_if_t<tmp_row == tmp_column, Class_Matrix_f32<tmp_row, tmp_row>> Class_Matrix_f32<row, column>::Get_Inverse() const
{
    // 扩展矩阵 [A|I]
    Class_Matrix_f32<tmp_row, 2 * tmp_row> extended_matrix = Namespace_ALG_Matrix::Zero<tmp_row, 2 * tmp_row>();

    // 扩展矩阵初始化
    for (int i = 0; i < tmp_row; i++)
    {
        // 矩阵A部分
        memcpy(&extended_matrix.Data[i * 2 * tmp_row], &Data[i * tmp_row], sizeof(float) * tmp_row);
        // 单位矩阵部分
        extended_matrix[i][i + tmp_row] = 1.0f;
    }

    // 高斯消元法
    for (int i = 0; i < tmp_row; i++)
    {
        float max_value = fabs(extended_matrix[i][i]);
        int max_index = i;
        for (int j = i + 1; j < tmp_row; j++)
        {
            if (fabs(extended_matrix[j][i]) > max_value)
            {
                max_value = fabs(extended_matrix[j][i]);
                max_index = j;
            }
        }
        // 最大元太小, 认为矩阵不可逆
        if (max_value <= Matrix_Compare_Epsilon)
        {
            return (Namespace_ALG_Matrix::Identity<tmp_row, tmp_row>());
        }
        // 交换行, 将第i行与主元所在行交换
        if (max_index != i)
        {
            float tmp_data[2 * tmp_row];
            memcpy(tmp_data, &extended_matrix.Data[i * 2 * tmp_row], sizeof(float) * 2 * tmp_row);
            memcpy(&extended_matrix.Data[i * 2 * tmp_row], &extended_matrix.Data[max_index * 2 * tmp_row], sizeof(float) * 2 * tmp_row);
            memcpy(&extended_matrix.Data[max_index * 2 * tmp_row], tmp_data, sizeof(float) * 2 * tmp_row);
        }

        // 行归一化
        float divisor = 1.0f / extended_matrix[i][i];
        for (int j = 0; j < 2 * tmp_row; j++)
        {
            extended_matrix.Data[i * 2 * tmp_row + j] *= divisor;
        }

        // 消元
        for (int j = 0; j < tmp_row; j++)
        {
            // 遍历每一行进行消元, 如果是主元行, 则跳过
            if (j != i)
            {
                float factor = extended_matrix[j][i];
                float tmp_data[2 * tmp_row];
                // 计算消元行
                for (int k = 0; k < 2 * tmp_row; k++)
                {
                    tmp_data[k] = -factor * extended_matrix.Data[i * 2 * tmp_row + k];
                }
                // 将消元行与当前行相加
                for (int k = 0; k < 2 * tmp_row; k++)
                {
                    extended_matrix.Data[j * 2 * tmp_row + k] += tmp_data[k];
                }
            }
        }
    }

    // 高斯消元法完成, 接下来是提取逆矩阵部分
    Class_Matrix_f32<tmp_row, tmp_row> result;

    for (int i = 0; i < tmp_row; i++)
    {
        memcpy(&result.Data[i * tmp_row], &extended_matrix.Data[i * 2 * tmp_row + tmp_row], sizeof(float) * tmp_row);
    }

    return (result);
}

/**
 * @brief 获取向量的模长
 *
 * @tparam row 行数
 * @tparam column 列数
 * @tparam tmp_row 行数
 * @tparam tmp_column 列数
 * @return Class_Matrix_f32<tmp_row, tmp_column> 模长
 */
template<int row, int column>
template<int tmp_row, int tmp_column>
inline std::enable_if_t<tmp_column == 1, float> Class_Matrix_f32<row, column>::Get_Modulus() const
{
    float norm = 0.0f;
    for (int i = 0; i < tmp_row; i++)
    {
        norm += Data[i] * Data[i];
    }
    norm = sqrtf(norm);
    return (norm);
}

/**
 * @brief 获取向量的归一化结果
 *
 * @tparam row 行数
 * @tparam column 列数
 * @tparam tmp_row 行数
 * @tparam tmp_column 列数
 * @return Class_Matrix_f32<tmp_row, tmp_column> 归一化结果
 */
template<int row, int column>
template<int tmp_row, int tmp_column>
inline std::enable_if_t<tmp_column == 1, Class_Matrix_f32<tmp_row, 1>> Class_Matrix_f32<row, column>::Get_Normalization() const
{
    Class_Matrix_f32<tmp_row, 1> result;

    float norm = 0.0f;
    for (int i = 0; i < tmp_row; i++)
    {
        norm += Data[i] * Data[i];
    }
    norm = 1.0f / sqrtf(norm);
    for (int i = 0; i < tmp_row; i++)
    {
        result.Data[i] = Data[i] * norm;
    }
    return (result);
}

/**
 * @brief 获取三维列向量的叉乘矩阵
 *
 * @tparam row 行数
 * @tparam column 列数
 * @tparam tmp_row 行数
 * @tparam tmp_column 列数
 * @return Class_Matrix_f32<tmp_row, tmp_column> 叉乘矩阵
 */
template<int row, int column>
template<int tmp_row, int tmp_column>
inline std::enable_if_t<tmp_row == 3 && tmp_column == 1, Class_Matrix_f32<3, 3>> Class_Matrix_f32<row, column>::Get_Cross_Matrix() const
{
    Class_Matrix_f32<3, 3> result;
    result.Data[0] = 0.0f;
    result.Data[1] = -Data[2];
    result.Data[2] = Data[1];
    result.Data[3] = Data[2];
    result.Data[4] = 0.0f;
    result.Data[5] = -Data[0];
    result.Data[6] = -Data[1];
    result.Data[7] = Data[0];
    result.Data[8] = 0.0f;
    return (result);
}

/**
 * @brief 获取单个元素的值
 *
 * @tparam row 行数
 * @tparam column 列数
 * @tparam tmp_row 行数
 * @tparam tmp_column 列数
 * @return float 元素值
 */
template<int row, int column>
template<int tmp_row, int tmp_column>
inline std::enable_if_t<tmp_row == 1 && tmp_column == 1, float> Class_Matrix_f32<row, column>::Get_Single_Value() const
{
    return (Data[0]);
}

/**
 * @brief 获取对应尺寸的零矩阵
 *
 * @tparam row 行数
 * @tparam column 列数
 * @return Class_Matrix_f32<row, column> 零矩阵
 */
template<int row, int column>
Class_Matrix_f32<row, column> Namespace_ALG_Matrix::Zero()
{
    Class_Matrix_f32<row, column> result;
    return (result);
}

/**
 * @brief 获取对应尺寸的单位矩阵
 *
 * @tparam row 行数
 * @tparam column 列数
 * @return Class_Matrix_f32<row, column> 单位矩阵
 */
template<int row, int column>
Class_Matrix_f32<row, column> Namespace_ALG_Matrix::Identity()
{
    Class_Matrix_f32<row, column> result;
    int min_dimension = (row < column) ? row : column;
    for (int i = 0; i < min_dimension; i++)
    {
        result[i][i] = 1.0f;
    }
    return (result);
}

/** * @brief 获取对应尺寸的常量矩阵
 *
 * @tparam row 行数
 * @tparam column 列数
 * @return Class_Matrix_f32<row, column> 常量矩阵
 */
template<int row, int column>
Class_Matrix_f32<row, column> Namespace_ALG_Matrix::Constant(const float &Value)
{
    Class_Matrix_f32<row, column> result;
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < column; j++)
        {
            result[i][j] = Value;
        }
    }
    return (result);
}

/**
 * @brief 计算两个列向量的点乘
 *
 * @tparam row 向量行数
 * @param Vector_1 向量1
 * @param Vector_2 向量2
 * @return Class_Matrix_f32<1, 1> 点乘结果
 */
template<int row>
float Namespace_ALG_Matrix::Operator_Dot(const Class_Matrix_f32<row, 1> &Vector_1, const Class_Matrix_f32<row, 1> &Vector_2)
{
    float result = 0.0f;
    for (int i = 0; i < row; i++)
    {
        result += Vector_1[i][0] * Vector_2[i][0];
    }
    return (result);
}

#endif

/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/
