/**
 * @file alg_filter_kalman.h
 * @author yssickjgd (1345578933@qq.com)
 * @brief Kalman滤波器
 * @version 0.1
 * @date 2025-09-07 0.1 新增
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef ALG_FILTER_KALMAN_H
#define ALG_FILTER_KALMAN_H

/* Includes ------------------------------------------------------------------*/

#include "1_Middleware/Algorithm/Matrix/alg_matrix.h"

/* Exported macros -----------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/**
 * @brief Reusable, Kalman滤波器算法
 *
 * 系统模型
 * x_k = A * x_{k-1} + B * u_k + w_k
 * z_k = H * x_k + v_k
 * 其中, w_k ~ N(0, Q), v_k ~ N(0, R)
 *
 * 每轮调用预测和更新函数前, 需要先更新输入向量Vector_U和测量向量Vector_Z
 *
 * @tparam State_Dimension 状态维度
 * @tparam Input_Dimension 输入维度
 * @tparam Measurement_Dimension 测量维度
 */
template<uint32_t State_Dimension = 2, uint32_t Input_Dimension = 1, uint32_t Measurement_Dimension = 2>
class Class_Filter_Kalman
{
public:
    // 随着系统确定的
    // 系统状态矩阵
    Class_Matrix_f32<State_Dimension, State_Dimension> Matrix_A;
    Class_Matrix_f32<State_Dimension, Input_Dimension> Matrix_B;
    // 测量矩阵
    Class_Matrix_f32<Measurement_Dimension, State_Dimension> Matrix_H;

    // 需要自己调参的
    // 过程噪声协方差矩阵
    Class_Matrix_f32<State_Dimension, State_Dimension> Matrix_Q;
    // 测量噪声协方差矩阵
    Class_Matrix_f32<Measurement_Dimension, Measurement_Dimension> Matrix_R;

    // 内部运算需要的
    // 误差协方差矩阵
    Class_Matrix_f32<State_Dimension, State_Dimension> Matrix_P;
    // 状态向量
    Class_Matrix_f32<State_Dimension, 1> Vector_X;
    // 输入向量
    Class_Matrix_f32<Input_Dimension, 1> Vector_U;
    // 先验估计状态向量
    Class_Matrix_f32<State_Dimension, 1> Vector_X_Prior;
    // 先验误差协方差矩阵
    Class_Matrix_f32<State_Dimension, State_Dimension> Matrix_P_Prior;
    // Kalman增益矩阵
    Class_Matrix_f32<State_Dimension, Measurement_Dimension> Matrix_K;

    // 传感器拿到的
    // 测量向量
    Class_Matrix_f32<Measurement_Dimension, 1> Vector_Z;

    void Init(const Class_Matrix_f32<State_Dimension, State_Dimension> &__Matrix_A, const Class_Matrix_f32<State_Dimension, Input_Dimension> &__Matrix_B, const Class_Matrix_f32<Measurement_Dimension, State_Dimension> &__Matrix_H, const Class_Matrix_f32<State_Dimension, State_Dimension> &__Matrix_Q, const Class_Matrix_f32<Measurement_Dimension, Measurement_Dimension> &__Matrix_R, const Class_Matrix_f32<State_Dimension, State_Dimension> &__Matrix_P = Namespace_ALG_Matrix::Identity<State_Dimension, State_Dimension>(), const Class_Matrix_f32<State_Dimension, 1> &__Vector_X = Namespace_ALG_Matrix::Zero<State_Dimension, 1>(), const Class_Matrix_f32<Input_Dimension, 1> &__Matrix_U = Namespace_ALG_Matrix::Zero<Input_Dimension, 1>());

    void TIM_Predict_PeriodElapsedCallback();

    void TIM_Update_PeriodElapsedCallback();

protected:
    // 初始化相关常量

    // 常量

    // 单位矩阵
    const Class_Matrix_f32<State_Dimension, State_Dimension> MATRIX_I_STATE = Namespace_ALG_Matrix::Identity<State_Dimension, State_Dimension>();

    // 内部变量

    // 读变量

    // 写变量

    // 读写变量

    // 内部函数
};

/* Exported variables --------------------------------------------------------*/

/* Exported function declarations --------------------------------------------*/

/**
 * @brief 初始化Kalman滤波器
 *
 * @tparam State_Dimension 状态维度
 * @tparam Input_Dimension 输入维度
 * @tparam Measurement_Dimension 测量维度
 * @param __Matrix_A 系统状态矩阵
 * @param __Matrix_B 控制矩阵
 * @param __Matrix_H 测量矩阵
 * @param __Matrix_Q 过程噪声协方差矩阵
 * @param __Matrix_R 测量噪声协方差矩阵
 * @param __Vector_X 初始状态向量
 * @param __Matrix_U 初始输入向量
 */
template<uint32_t State_Dimension, uint32_t Input_Dimension, uint32_t Measurement_Dimension>
void Class_Filter_Kalman<State_Dimension, Input_Dimension, Measurement_Dimension>::Init(const Class_Matrix_f32<State_Dimension, State_Dimension> &__Matrix_A, const Class_Matrix_f32<State_Dimension, Input_Dimension> &__Matrix_B, const Class_Matrix_f32<Measurement_Dimension, State_Dimension> &__Matrix_H, const Class_Matrix_f32<State_Dimension, State_Dimension> &__Matrix_Q, const Class_Matrix_f32<Measurement_Dimension, Measurement_Dimension> &__Matrix_R, const Class_Matrix_f32<State_Dimension, State_Dimension> &__Matrix_P, const Class_Matrix_f32<State_Dimension, 1> &__Vector_X, const Class_Matrix_f32<Input_Dimension, 1> &__Matrix_U)
{
    Matrix_A = __Matrix_A;
    Matrix_B = __Matrix_B;
    Matrix_H = __Matrix_H;

    Matrix_Q = __Matrix_Q;
    Matrix_R = __Matrix_R;

    Matrix_P = __Matrix_P;
    Vector_X = __Vector_X;
    Vector_U = __Matrix_U;
}

/**
 * @brief Kalman滤波器预测步骤, 周期与采样周期相同
 *
 * @tparam State_Dimension 状态维度
 * @tparam Input_Dimension 输入维度
 * @tparam Measurement_Dimension 测量维度
 */
template<uint32_t State_Dimension, uint32_t Input_Dimension, uint32_t Measurement_Dimension>
void Class_Filter_Kalman<State_Dimension, Input_Dimension, Measurement_Dimension>::TIM_Predict_PeriodElapsedCallback()
{
    // 预测状态向量
    Vector_X_Prior = Matrix_A * Vector_X + Matrix_B * Vector_U;

    // 预测误差协方差矩阵
    Matrix_P_Prior = Matrix_A * Matrix_P * Matrix_A.Get_Transpose() + Matrix_Q;
}

/**
 * @brief Kalman滤波器更新步骤, 周期与采样周期相同
 *
 * @tparam State_Dimension 状态维度
 * @tparam Input_Dimension 输入维度
 * @tparam Measurement_Dimension 测量维度
 */
template<uint32_t State_Dimension, uint32_t Input_Dimension, uint32_t Measurement_Dimension>
void Class_Filter_Kalman<State_Dimension, Input_Dimension, Measurement_Dimension>::TIM_Update_PeriodElapsedCallback()
{
    // 计算Kalman增益矩阵
    Matrix_K = Matrix_P_Prior * Matrix_H.Get_Transpose() * (Matrix_H * Matrix_P_Prior * Matrix_H.Get_Transpose() + Matrix_R).Get_Inverse();

    // 更新状态向量
    Vector_X = Vector_X_Prior + Matrix_K * (Vector_Z - Matrix_H * Vector_X_Prior);

    // 更新误差协方差矩阵, 为获得更好的数值稳定性, 采用约瑟夫形式
    Class_Matrix_f32 < State_Dimension, State_Dimension > matrix_tmp = MATRIX_I_STATE - Matrix_K * Matrix_H;
    // Matrix_P = matrix_tmp * Matrix_P_Prior;
    Matrix_P = matrix_tmp * Matrix_P_Prior * matrix_tmp.Get_Transpose() + Matrix_K * Matrix_R * Matrix_K.Get_Transpose();
}

#endif

/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/