/**
 * @file alg_filter_ekf.h
 * @author yssickjgd (1345578933@qq.com)
 * @brief 扩展Kalman滤波器
 * @version 0.1
 * @date 2025-09-15 0.1 新增
 *
 * @copyright USTC-RoboWalker (c) 2025
 *
 */

#ifndef ALG_FILTER_EKF_H
#define ALG_FILTER_EKF_H

/* Includes ------------------------------------------------------------------*/

#include "1_Middleware/Algorithm/Matrix/alg_matrix.h"

/* Exported macros -----------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/**
 * @brief Reusable, 扩展Kalman滤波器算法
 *
 * 系统模型
 * x_k = f(x_{k-1}, u_k, w_k)
 * z_k = h(x_k, v_k)
 * 其中, w_k ~ N(0, Q), v_k ~ N(0, R)
 *
 * 每轮调用预测和更新函数前, 需要先更新输入向量Vector_U和测量向量Vector_Z
 *
 * @tparam State_Dimension 状态维度
 * @tparam Input_Dimension 输入维度
 * @tparam Measurement_Dimension 测量维度
 */
template<uint32_t State_Dimension = 2, uint32_t Input_Dimension = 1, uint32_t Measurement_Dimension = 2>
class Class_Filter_EKF
{
public:
    // 随着系统确定的
    // 系统状态函数
    Class_Matrix_f32<State_Dimension, 1> (*Function_F)(const Class_Matrix_f32<State_Dimension, 1> &Vector_X, const Class_Matrix_f32<Input_Dimension, 1> &Vector_U, const float &D_T);

    // 系统状态函数对状态的雅可比矩阵
    Class_Matrix_f32<State_Dimension, State_Dimension> (*Function_Jacobian_F_X)(const Class_Matrix_f32<State_Dimension, 1> &Vector_X, const Class_Matrix_f32<Input_Dimension, 1> &Vector_U, const float &D_T);

    // 系统状态函数对过程噪声的雅可比矩阵
    Class_Matrix_f32<State_Dimension, Input_Dimension> (*Function_Jacobian_F_W)(const Class_Matrix_f32<State_Dimension, 1> &Vector_X, const Class_Matrix_f32<Input_Dimension, 1> &Vector_U, const float &D_T);

    // 系统测量函数
    Class_Matrix_f32<Measurement_Dimension, 1> (*Function_H)(const Class_Matrix_f32<State_Dimension, 1> &Vector_X, const float &D_T);

    // 系统测量函数对状态的雅可比矩阵
    Class_Matrix_f32<Measurement_Dimension, State_Dimension> (*Function_Jacobian_H_X)(const Class_Matrix_f32<State_Dimension, 1> &Vector_X, const float &D_T);

    // 系统测量函数对测量噪声的雅可比矩阵
    Class_Matrix_f32<Measurement_Dimension, Measurement_Dimension> (*Function_Jacobian_H_V)(const Class_Matrix_f32<State_Dimension, 1> &Vector_X, const float &D_T);

    // 需要自己调参的
    // 过程噪声协方差矩阵
    Class_Matrix_f32<Input_Dimension, Input_Dimension> Matrix_Q;
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

    void Init(const Class_Matrix_f32<Input_Dimension, Input_Dimension> &__Matrix_Q, const Class_Matrix_f32<Measurement_Dimension, Measurement_Dimension> &__Matrix_R, const Class_Matrix_f32<State_Dimension, State_Dimension> &__Matrix_P = Namespace_ALG_Matrix::Identity<State_Dimension, State_Dimension>(), const Class_Matrix_f32<State_Dimension, 1> &__Vector_X = Namespace_ALG_Matrix::Zero<State_Dimension, 1>(), const Class_Matrix_f32<Input_Dimension, 1> &__Matrix_U = Namespace_ALG_Matrix::Zero<Input_Dimension, 1>());

    inline void Config_Nonlinear_State_Model(Class_Matrix_f32<State_Dimension, 1> (*__Function_F)(const Class_Matrix_f32<State_Dimension, 1> &Vector_X, const Class_Matrix_f32<Input_Dimension, 1> &Vector_U, const float &D_T), Class_Matrix_f32<State_Dimension, State_Dimension> (*__Function_Jacobian_F_X)(const Class_Matrix_f32<State_Dimension, 1> &Vector_X, const Class_Matrix_f32<Input_Dimension, 1> &Vector_U, const float &D_T), Class_Matrix_f32<State_Dimension, Input_Dimension> (*__Function_Jacobian_F_W)(const Class_Matrix_f32<State_Dimension, 1> &Vector_X, const Class_Matrix_f32<Input_Dimension, 1> &Vector_U, const float &D_T));

    inline void Config_Nonlinear_Measurement_Model(Class_Matrix_f32<Measurement_Dimension, 1> (*__Function_H)(const Class_Matrix_f32<State_Dimension, 1> &Vector_X, const float &D_T), Class_Matrix_f32<Measurement_Dimension, State_Dimension> (*__Function_Jacobian_H_X)(const Class_Matrix_f32<State_Dimension, 1> &Vector_X, const float &D_T), Class_Matrix_f32<Measurement_Dimension, Measurement_Dimension> (*__Function_Jacobian_H_V)(const Class_Matrix_f32<State_Dimension, 1> &Vector_X, const float &D_T));

    void Set_D_T(const float &__D_T);

    void TIM_Predict_PeriodElapsedCallback();

    void TIM_Update_PeriodElapsedCallback();

protected:
    // 初始化相关常量

    // 常量

    // 单位矩阵
    const Class_Matrix_f32<State_Dimension, State_Dimension> MATRIX_I_STATE = Namespace_ALG_Matrix::Identity<State_Dimension, State_Dimension>();
    // 零矩阵
    const Class_Matrix_f32<State_Dimension, State_Dimension> MATRIX_ZERO_STATE = Namespace_ALG_Matrix::Zero<State_Dimension, State_Dimension>();

    // 内部变量

    // 读变量

    // 写变量

    // 时间差, 单位s
    float D_T = 0.001f;

    // 读写变量

    // 内部函数
};

/* Exported variables --------------------------------------------------------*/

/* Exported function declarations --------------------------------------------*/

/**
 * @brief 初始化扩展Kalman滤波器
 *
 * @tparam State_Dimension 状态维度
 * @tparam Input_Dimension 输入维度
 * @tparam Measurement_Dimension 测量维度
 * @param __Matrix_Q 过程噪声协方差矩阵
 * @param __Matrix_R 测量噪声协方差矩阵
 * @param __Vector_X 初始状态向量
 * @param __Matrix_U 初始输入向量
 */
template<uint32_t State_Dimension, uint32_t Input_Dimension, uint32_t Measurement_Dimension>
void Class_Filter_EKF<State_Dimension, Input_Dimension, Measurement_Dimension>::Init(const Class_Matrix_f32<Input_Dimension, Input_Dimension> &__Matrix_Q, const Class_Matrix_f32<Measurement_Dimension, Measurement_Dimension> &__Matrix_R, const Class_Matrix_f32<State_Dimension, State_Dimension> &__Matrix_P, const Class_Matrix_f32<State_Dimension, 1> &__Vector_X, const Class_Matrix_f32<Input_Dimension, 1> &__Matrix_U)
{
    Matrix_Q = __Matrix_Q;
    Matrix_R = __Matrix_R;

    Matrix_P = __Matrix_P;
    Vector_X = __Vector_X;
    Vector_U = __Matrix_U;
}

/**
 * @brief 设置非线性系统状态模型
 *
 * @tparam State_Dimension 状态维度
 * @tparam Input_Dimension 输入维度
 * @tparam Measurement_Dimension 测量维度
 * @param __Function_F 系统状态函数
 * @param __Function_Jacobian_F_X 系统状态函数对状态的雅可比矩阵
 * @param __Function_Jacobian_F_W 系统状态函数对过程噪声的雅可比矩阵
 */
template<uint32_t State_Dimension, uint32_t Input_Dimension, uint32_t Measurement_Dimension>
inline void Class_Filter_EKF<State_Dimension, Input_Dimension, Measurement_Dimension>::Config_Nonlinear_State_Model(Class_Matrix_f32 < State_Dimension, 1 > (*__Function_F)(const Class_Matrix_f32 < State_Dimension, 1 > &Vector_X, 
const Class_Matrix_f32<Input_Dimension, 1> &Vector_U, 
const float &D_T
)
,
Class_Matrix_f32<State_Dimension, State_Dimension> (*__Function_Jacobian_F_X)(const Class_Matrix_f32<State_Dimension, 1> &Vector_X, const Class_Matrix_f32<Input_Dimension, 1> &Vector_U, const float &D_T), Class_Matrix_f32<State_Dimension, Input_Dimension>(*__Function_Jacobian_F_W)(
const Class_Matrix_f32<State_Dimension, 1> &Vector_X, 
const Class_Matrix_f32<Input_Dimension, 1> &Vector_U, 
const float &D_T
)
)
{
    Function_F = __Function_F;
    Function_Jacobian_F_X = __Function_Jacobian_F_X;
    Function_Jacobian_F_W = __Function_Jacobian_F_W;
}

/**
 * @brief 设置非线性系统测量模型
 *
 * @tparam State_Dimension 状态维度
 * @tparam Input_Dimension 输入维度
 * @tparam Measurement_Dimension 测量维度
 * @param __Function_H 系统测量函数
 * @param __Function_Jacobian_H_X 系统测量函数对状态的雅可比矩阵
 * @param __Function_Jacobian_H_V 系统测量函数对测量噪声的雅可比矩阵
 */
template<uint32_t State_Dimension, uint32_t Input_Dimension, uint32_t Measurement_Dimension>
inline void Class_Filter_EKF<State_Dimension, Input_Dimension, Measurement_Dimension>::Config_Nonlinear_Measurement_Model(Class_Matrix_f32 < Measurement_Dimension, 1 > (*__Function_H)(const Class_Matrix_f32 < State_Dimension, 1 > &Vector_X, const float &D_T), Class_Matrix_f32 < Measurement_Dimension, State_Dimension > (*__Function_Jacobian_H_X)(const Class_Matrix_f32 < State_Dimension, 1 > &Vector_X, const float &D_T), Class_Matrix_f32 < Measurement_Dimension, Measurement_Dimension > (*__Function_Jacobian_H_V)(const Class_Matrix_f32 < State_Dimension, 1 > &Vector_X, const float &D_T))
{
    Function_H = __Function_H;
    Function_Jacobian_H_X = __Function_Jacobian_H_X;
    Function_Jacobian_H_V = __Function_Jacobian_H_V;
}

/**
 * @brief 设置计算周期
 *
 */
template<uint32_t State_Dimension, uint32_t Input_Dimension, uint32_t Measurement_Dimension>
void Class_Filter_EKF<State_Dimension, Input_Dimension, Measurement_Dimension>::Set_D_T(const float &__D_T)
{
    D_T = __D_T;
}

/**
 * @brief 扩展Kalman滤波器预测步骤, 周期与采样周期相同
 *
 * @tparam State_Dimension 状态维度
 * @tparam Input_Dimension 输入维度
 * @tparam Measurement_Dimension 测量维度
 */
template<uint32_t State_Dimension, uint32_t Input_Dimension, uint32_t Measurement_Dimension>
void Class_Filter_EKF<State_Dimension, Input_Dimension, Measurement_Dimension>::TIM_Predict_PeriodElapsedCallback()
{
    // 计算Jacobi矩阵
    Class_Matrix_f32 < State_Dimension, State_Dimension > matrix_f_x = Function_Jacobian_F_X(Vector_X, Vector_U, D_T);
    Class_Matrix_f32 < State_Dimension, Input_Dimension > matrix_f_w = Function_Jacobian_F_W(Vector_X, Vector_U, D_T);

    // 预测状态向量
    Vector_X_Prior = Function_F(Vector_X, Vector_U, D_T);

    // 预测误差协方差矩阵
    Matrix_P_Prior = matrix_f_x * Matrix_P * matrix_f_x.Get_Transpose() + matrix_f_w * Matrix_Q * matrix_f_w.Get_Transpose();

    // 在未更新观测时, 也能有后验
    Vector_X = Vector_X_Prior;
    Matrix_P = Matrix_P_Prior;
}

/**
 * @brief 扩展Kalman滤波器更新步骤, 周期与采样周期相同
 *
 * @tparam State_Dimension 状态维度
 * @tparam Input_Dimension 输入维度
 * @tparam Measurement_Dimension 测量维度
 */
template<uint32_t State_Dimension, uint32_t Input_Dimension, uint32_t Measurement_Dimension>
void Class_Filter_EKF<State_Dimension, Input_Dimension, Measurement_Dimension>::TIM_Update_PeriodElapsedCallback()
{
    // 计算Jacobi矩阵
    Class_Matrix_f32 < Measurement_Dimension, State_Dimension > matrix_h_x = Function_Jacobian_H_X(Vector_X_Prior, D_T);
    Class_Matrix_f32 < Measurement_Dimension, Measurement_Dimension > matrix_h_v = Function_Jacobian_H_V(Vector_X_Prior, D_T);

    // 计算Kalman增益矩阵
    Matrix_K = Matrix_P_Prior * matrix_h_x.Get_Transpose() * (matrix_h_x * Matrix_P_Prior * matrix_h_x.Get_Transpose() + matrix_h_v * Matrix_R * matrix_h_v.Get_Transpose()).Get_Inverse();

    // 更新状态向量
    Vector_X = Vector_X_Prior + Matrix_K * (Vector_Z - Function_H(Vector_X_Prior, D_T));

    // 更新误差协方差矩阵, 为获得更好的数值稳定性, 采用约瑟夫形式
    Class_Matrix_f32 < State_Dimension, State_Dimension > matrix_tmp = MATRIX_I_STATE - Matrix_K * matrix_h_x;
    Matrix_P = matrix_tmp * Matrix_P_Prior * matrix_tmp.Get_Transpose() + Matrix_K * matrix_h_v * Matrix_R * matrix_h_v.Get_Transpose() * Matrix_K.Get_Transpose();
}

#endif

/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/