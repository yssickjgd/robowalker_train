/**
 * @file bsp_bmi088.cpp
 * @author yssickjgd (1345578933@qq.com)
 * @brief BMI088组件之加速度计, 内含加热电阻
 * @version 0.1
 * @date 2025-08-26 0.1 新建文档
 *
 * @copyright USTC-RoboWalker (c) 2025
 *
 */

/* Includes ------------------------------------------------------------------*/

#include "bsp_bmi088.h"

/* Private macros ------------------------------------------------------------*/

/* Private types -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

Class_BMI088 BSP_BMI088;

/* Private function declarations ---------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/

/**
 * @brief 初始化BMI088
 *
 */
void Class_BMI088::Init()
{
    SPI_Manage_Object = &SPI2_Manage_Object;

    BMI088_Accel.Init(true);
    BMI088_Gyro.Init();

    // 欧拉角需要辅助初始化EKF, 第一次初始化默认Yaw是0
    Vector_Euler_Angle[0][0] = 0.0f;

    Init_Finished_Flag = true;
}

/**
 * @brief SPI接收完成回调函数
 *
 */
void Class_BMI088::SPI_RxCpltCallback()
{
    if (SPI_Manage_Object->Activate_GPIOx == BMI088_ACCEL__SPI_CS_GPIO_Port && SPI_Manage_Object->Activate_GPIO_Pin == BMI088_ACCEL__SPI_CS_Pin)
    {
        BMI088_Accel.SPI_RxCpltCallback();

        if (Init_Finished_Flag)
        {
            if (SPI_Manage_Object->Rx_Buffer_Length == 6)
            {
                Accel_Transfering_Flag = false;
                Accel_Update_Flag = true;
                Accel_Update_Timestamp = SYS_Timestamp.Get_Now_Microsecond();
            }
            else if (SPI_Manage_Object->Rx_Buffer_Length == 2)
            {
                Temperature_Transfering_Flag = false;
            }
        }
    }
    else if (SPI_Manage_Object->Activate_GPIOx == BMI088_GYRO__SPI_CS_GPIO_Port && SPI_Manage_Object->Activate_GPIO_Pin == BMI088_GYRO__SPI_CS_Pin)
    {
        BMI088_Gyro.SPI_RxCallback();

        if (Init_Finished_Flag)
        {
            Gyro_Transfering_Flag = false;
            Gyro_Update_Flag = true;
            Gyro_Update_Timestamp = SYS_Timestamp.Get_Now_Microsecond();
        }
    }
}

/**
 * @brief EXTI中断回调函数
 *
 * @param GPIO_Pin 中断引脚
 */
void Class_BMI088::EXTI_Flag_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == BMI088_ACCEL__INTERRUPT_Pin)
    {
        Accel_Ready_Flag = true;
        Accel_Ready_Timestamp = SYS_Timestamp.Get_Now_Microsecond();
    }
    else if (GPIO_Pin == BMI088_GYRO__INTERRUPT_Pin)
    {
        Gyro_Ready_Flag = true;
        Gyro_Ready_Timestamp = SYS_Timestamp.Get_Now_Microsecond();
    }
}

/**
 * @brief 定时器周期中断回调函数
 *
 */
void Class_BMI088::TIM_128ms_Calculate_PeriodElapsedCallback()
{
    Temperature_Ready_Flag = true;
    BMI088_Accel.TIM_128ms_Heater_PID_PeriodElapsedCallback();
}

/**
 * @brief 定时器周期中断回调函数
 *
 */
void Class_BMI088::TIM_125us_Calculate_PeriodElapsedCallback()
{
    EKF_Now_Timestamp = SYS_Timestamp.Get_Now_Microsecond();

    Vector_Original_Accel = BMI088_Accel.Get_Raw_Accel();
    Vector_Original_Gyro = BMI088_Gyro.Get_Raw_Gyro();

    // 角速度合法则保存, 不合法则使用上次的数据
    if (!BMI088_Gyro.Get_Valid_Flag())
    {
        Vector_Original_Gyro = Vector_Pre_Original_Gyro;
    }

    // 如果加热电阻使能, 则进行零偏修正
    if (BMI088_Accel.Get_Heater_Enable())
    {
        Vector_Original_Accel = (Class_Matrix_f32<3, 3>(ACCEL_AFFINE_DATA) * Vector_Original_Accel / GRAVITY_ACCELERATION + Class_Matrix_f32<3, 1>(ACCEL_BIAS_DATA)) * GRAVITY_ACCELERATION;

        Vector_Original_Gyro = Vector_Original_Gyro + Class_Matrix_f32<3, 1>(GYRO_ZERO_OFFSET);
    }

    // 加速度计归一化数据
    Vector_Normalized_Accel = Vector_Original_Accel.Get_Normalization();


    // EKF初始化与计算
    if (!EKF_Init_Finished_Flag && Accel_Update_Flag && BMI088_Accel.Get_Valid_Flag())
    {
        // EKF相关变量与函数

        // 过程噪声协方差矩阵
        float array_q[9] = {0.865f, 0.0f, 0.0f, 0.0f, 0.975f, 0.0f, 0.0f, 0.0f, 1.077f};
        Class_Matrix_f32<3, 3> matrix_q(array_q);
        // 测量噪声协方差矩阵
        float array_r[9] = {0.0446f, 0.0f, 0.0f, 0.0f, 0.0476f, 0.0f, 0.0f, 0.0f, 0.0537f};
        Class_Matrix_f32<3, 3> matrix_r(array_r);
        // 初始状态协方差矩阵
        Class_Matrix_f32<4, 4> matrix_p = Namespace_ALG_Matrix::Identity<4, 4>();
        // 初始状态向量
        Class_Matrix_f32<4, 1> vector_x;
        float half_yaw = Vector_Euler_Angle[0][0] * 0.5f;
        float half_roll = atan2f(Vector_Normalized_Accel[1][0], Vector_Normalized_Accel[2][0]) * 0.5f;
        float half_pitch = asinf(-Vector_Normalized_Accel[0][0]) * 0.5f;
        float cy = cosf(half_yaw);
        float sy = sinf(half_yaw);
        float cp = cosf(half_pitch);
        float sp = sinf(half_pitch);
        float cr = cosf(half_roll);
        float sr = sinf(half_roll);
        vector_x[0][0] = cy * cp * cr + sy * sp * sr;
        vector_x[1][0] = cy * cp * sr - sy * sp * cr;
        vector_x[2][0] = sy * cp * sr + cy * sp * cr;
        vector_x[3][0] = sy * cp * cr - cy * sp * sr;
        vector_x = vector_x.Get_Normalization();

        EKF_Quaternion.Init(matrix_q, matrix_r, matrix_p, vector_x);

        EKF_Quaternion.Config_Nonlinear_State_Model(EKF_Function_F, EKF_Function_Jacobian_F_X, EKF_Function_Jacobian_F_W);
        EKF_Quaternion.Config_Nonlinear_Measurement_Model(EKF_Function_H, EKF_Function_Jacobian_H_X, EKF_Function_Jacobian_H_V);

        EKF_Init_Finished_Flag = true;
        Accel_Update_Flag = false;
        EKF_Pre_Timestamp = EKF_Now_Timestamp;

        return;
    }

    if (EKF_Init_Finished_Flag)
    {
        // 设置时间差
        D_T = (EKF_Now_Timestamp - EKF_Pre_Timestamp) / 1000000.0f;
        // 若时间差不合法, 则重新初始化EKF
        if (D_T <= 0.0f || D_T > D_T_TIMEOUT_THRESHOLD)
        {
            EKF_Init_Finished_Flag = false;
            return;
        }
        EKF_Quaternion.Set_D_T(D_T);

        // EKF预测
        EKF_Quaternion.Vector_U = Vector_Original_Gyro;
        EKF_Quaternion.TIM_Predict_PeriodElapsedCallback();

        // EKF更新
        if (Accel_Update_Flag && BMI088_Accel.Get_Valid_Flag())
        {
            Accel_Chi_Square_Calculate();
            if (Accel_Chi_Square_Loss <= ACCEL_CHI_SQUARE_TEST_THRESHOLD)
            {
                // 卡方检验通过, 更新
                EKF_Quaternion.Vector_Z = Vector_Normalized_Accel;
                EKF_Quaternion.TIM_Update_PeriodElapsedCallback();
            }
            // 加速度已利用过, 清除标志
            Accel_Update_Flag = false;
        }

        // x归一化, 按理来说这也算模型的一部分, 应当放到系统函数F中, 且需要更新Jacobi矩阵
        // 然而实测发现是否更新对性能影响不算太大, 更新反而占用了计算时间
        EKF_Quaternion.Vector_X = EKF_Quaternion.Vector_X.Get_Normalization();

        // 数据输出

        // 获取四元数
        Quarternion = EKF_Quaternion.Vector_X;

        // 输出姿态相关变量
        Vector_Euler_Angle = Quarternion.Get_Euler_Angle();
        Matrix_Rotation = Quarternion.Get_Rotation_Matrix();
        Vector_Axis_Angle = Quarternion.Get_Axis_Angle();

        // 机体坐标系下的重力加速度
        Class_Matrix_f32<3, 1> vector_gravity_body = Matrix_Rotation.Get_Transpose() * (-Namespace_ALG_Matrix::Axis_Z_3d() * GRAVITY_ACCELERATION);

        // 输出运动学相关变量
        Vector_Accel_Body = Vector_Original_Accel + vector_gravity_body;
        Vector_Accel = Matrix_Rotation * Vector_Accel_Body;
        Vector_Gyro_Body = Vector_Original_Gyro;
        Vector_Gyro = Matrix_Rotation * Vector_Gyro_Body;

        Calculating_Time = SYS_Timestamp.Get_Now_Microsecond() - EKF_Now_Timestamp;

        if (BMI088_Gyro.Get_Valid_Flag())
        {
            Vector_Pre_Original_Gyro = Vector_Original_Gyro;
        }
        EKF_Pre_Timestamp = EKF_Now_Timestamp;
    }
}

/**
 * @brief 定时器周期中断回调函数
 *
 */
void Class_BMI088::TIM_10us_Calculate_PeriodElapsedCallback()
{
    if (Init_Finished_Flag && Accel_Ready_Flag && !Accel_Transfering_Flag && !Gyro_Transfering_Flag && !Temperature_Transfering_Flag)
    {
        // 数据准备好, 读取加速度计
        Accel_Transfering_Flag = true;
        Accel_Transfering_Timestamp = SYS_Timestamp.Get_Now_Microsecond();
        BMI088_Accel.SPI_Request_Accel();
        Accel_Ready_Flag = false;
        return;
    }

    if (Init_Finished_Flag && Gyro_Ready_Flag && !Accel_Transfering_Flag && !Gyro_Transfering_Flag && !Temperature_Transfering_Flag)
    {
        // 数据准备好, 读取陀螺仪
        Gyro_Transfering_Flag = true;
        Gyro_Transfering_Timestamp = SYS_Timestamp.Get_Now_Microsecond();
        BMI088_Gyro.SPI_Request_Gyro();
        Gyro_Ready_Flag = false;
        return;
    }

    if (Init_Finished_Flag && Temperature_Ready_Flag && !Accel_Transfering_Flag && !Gyro_Transfering_Flag && !Temperature_Transfering_Flag)
    {
        // 温度准备好, 读取温度
        Temperature_Transfering_Flag = true;
        Temperature_Transfering_Timestamp = SYS_Timestamp.Get_Now_Microsecond();
        BMI088_Accel.SPI_Request_Temperature();
        Temperature_Ready_Flag = false;
        return;
    }

    if (Init_Finished_Flag && Accel_Transfering_Flag && (SYS_Timestamp.Get_Now_Microsecond() - Accel_Transfering_Timestamp) >= TRANSFERING_TIMEOUT)
    {
        // 加速度计传输超时
        Accel_Transfering_Flag = true;
        Accel_Transfering_Timestamp = SYS_Timestamp.Get_Now_Microsecond();
        BMI088_Accel.SPI_Request_Accel();
        Accel_Ready_Flag = false;
        return;
    }

    if (Init_Finished_Flag && Gyro_Transfering_Flag && (SYS_Timestamp.Get_Now_Microsecond() - Gyro_Transfering_Timestamp) >= TRANSFERING_TIMEOUT)
    {
        // 陀螺仪传输超时
        Gyro_Transfering_Flag = true;
        Gyro_Transfering_Timestamp = SYS_Timestamp.Get_Now_Microsecond();
        BMI088_Gyro.SPI_Request_Gyro();
        Gyro_Ready_Flag = false;
        return;
    }

    if (Init_Finished_Flag && Temperature_Transfering_Flag && (SYS_Timestamp.Get_Now_Microsecond() - Temperature_Transfering_Timestamp) >= TRANSFERING_TIMEOUT)
    {
        // 温度传输超时
        Temperature_Transfering_Flag = true;
        Temperature_Transfering_Timestamp = SYS_Timestamp.Get_Now_Microsecond();
        BMI088_Accel.SPI_Request_Temperature();
        Temperature_Ready_Flag = false;
        return;
    }
}

/**
 * @brief 四元数状态转移函数
 *
 * @param Vector_X 状态向量
 * @param Vector_U 输入向量
 */
Class_Matrix_f32<4, 1> Class_BMI088::EKF_Function_F(const Class_Matrix_f32<4, 1> &Vector_X, const Class_Matrix_f32<3, 1> &Vector_U, const float &D_T)
{
    Class_Matrix_f32<4, 1> matrix_result;

    // 角速度矩阵
    Class_Matrix_f32<4, 4> matrix_omega;
    matrix_omega[0][0] = 0.0f;
    matrix_omega[0][1] = -Vector_U[0][0];
    matrix_omega[0][2] = -Vector_U[1][0];
    matrix_omega[0][3] = -Vector_U[2][0];
    matrix_omega[1][0] = Vector_U[0][0];
    matrix_omega[1][1] = 0.0f;
    matrix_omega[1][2] = Vector_U[2][0];
    matrix_omega[1][3] = -Vector_U[1][0];
    matrix_omega[2][0] = Vector_U[1][0];
    matrix_omega[2][1] = -Vector_U[2][0];
    matrix_omega[2][2] = 0.0f;
    matrix_omega[2][3] = Vector_U[0][0];
    matrix_omega[3][0] = Vector_U[2][0];
    matrix_omega[3][1] = Vector_U[1][0];
    matrix_omega[3][2] = -Vector_U[0][0];
    matrix_omega[3][3] = 0.0f;

    matrix_result = Vector_X + 0.5f * D_T * matrix_omega * Vector_X;

    return matrix_result;
}

/**
 * @brief 四元数状态转移函数对状态的雅可比矩阵
 *
 * @param Vector_X 状态向量
 * @param Vector_U 输入向量
 */
Class_Matrix_f32<4, 4> Class_BMI088::EKF_Function_Jacobian_F_X(const Class_Matrix_f32<4, 1> &Vector_X, const Class_Matrix_f32<3, 1> &Vector_U, const float &D_T)
{
    Class_Matrix_f32<4, 4> matrix_result;

    // 角速度矩阵
    Class_Matrix_f32<4, 4> matrix_omega;
    matrix_omega[0][0] = 0.0f;
    matrix_omega[0][1] = -Vector_U[0][0];
    matrix_omega[0][2] = -Vector_U[1][0];
    matrix_omega[0][3] = -Vector_U[2][0];
    matrix_omega[1][0] = Vector_U[0][0];
    matrix_omega[1][1] = 0.0f;
    matrix_omega[1][2] = Vector_U[2][0];
    matrix_omega[1][3] = -Vector_U[1][0];
    matrix_omega[2][0] = Vector_U[1][0];
    matrix_omega[2][1] = -Vector_U[2][0];
    matrix_omega[2][2] = 0.0f;
    matrix_omega[2][3] = Vector_U[0][0];
    matrix_omega[3][0] = Vector_U[2][0];
    matrix_omega[3][1] = Vector_U[1][0];
    matrix_omega[3][2] = -Vector_U[0][0];

    matrix_result = Namespace_ALG_Matrix::Identity<4, 4>() + 0.5f * D_T * matrix_omega;

    return matrix_result;
}

/**
 * @brief 四元数状态转移函数对过程噪声的雅可比矩阵
 *
 * @param Vector_X 状态向量
 * @param Vector_U 输入向量
 */
Class_Matrix_f32<4, 3> Class_BMI088::EKF_Function_Jacobian_F_W(const Class_Matrix_f32<4, 1> &Vector_X, const Class_Matrix_f32<3, 1> &Vector_U, const float &D_T)
{
    Class_Matrix_f32<4, 3> matrix_result;

    // 四元数矩阵
    Class_Matrix_f32<4, 3> matrix_q;
    matrix_q[0][0] = -Vector_X[1][0];
    matrix_q[0][1] = -Vector_X[2][0];
    matrix_q[0][2] = -Vector_X[3][0];
    matrix_q[1][0] = Vector_X[0][0];
    matrix_q[1][1] = -Vector_X[3][0];
    matrix_q[1][2] = Vector_X[2][0];
    matrix_q[2][0] = Vector_X[3][0];
    matrix_q[2][1] = Vector_X[0][0];
    matrix_q[2][2] = -Vector_X[1][0];
    matrix_q[3][0] = -Vector_X[2][0];
    matrix_q[3][1] = Vector_X[1][0];
    matrix_q[3][2] = Vector_X[0][0];

    matrix_result = 0.5f * D_T * matrix_q;

    return matrix_result;
}

/**
 * @brief 四元数测量函数
 *
 * @param Vector_X 状态向量
 */
Class_Matrix_f32<3, 1> Class_BMI088::EKF_Function_H(const Class_Matrix_f32<4, 1> &Vector_X, const float &D_T)
{
    Class_Matrix_f32<3, 1> matrix_result;

    matrix_result[0][0] = 2.0f * (Vector_X[1][0] * Vector_X[3][0] - Vector_X[0][0] * Vector_X[2][0]);
    matrix_result[1][0] = 2.0f * (Vector_X[2][0] * Vector_X[3][0] + Vector_X[0][0] * Vector_X[1][0]);
    matrix_result[2][0] = Vector_X[0][0] * Vector_X[0][0] - Vector_X[1][0] * Vector_X[1][0] - Vector_X[2][0] * Vector_X[2][0] + Vector_X[3][0] * Vector_X[3][0];

    return matrix_result;
}

/**
 * @brief 四元数测量函数对状态的雅可比矩阵
 *
 * @param Vector_X 状态向量
 */
Class_Matrix_f32<3, 4> Class_BMI088::EKF_Function_Jacobian_H_X(const Class_Matrix_f32<4, 1> &Vector_X, const float &D_T)
{
    Class_Matrix_f32<3, 4> matrix_result;

    matrix_result[0][0] = -2.0f * Vector_X[2][0];
    matrix_result[0][1] = 2.0f * Vector_X[3][0];
    matrix_result[0][2] = -2.0f * Vector_X[0][0];
    matrix_result[0][3] = 2.0f * Vector_X[1][0];

    matrix_result[1][0] = 2.0f * Vector_X[1][0];
    matrix_result[1][1] = 2.0f * Vector_X[0][0];
    matrix_result[1][2] = 2.0f * Vector_X[3][0];
    matrix_result[1][3] = 2.0f * Vector_X[2][0];

    matrix_result[2][0] = 2.0f * Vector_X[0][0];
    matrix_result[2][1] = -2.0f * Vector_X[1][0];
    matrix_result[2][2] = -2.0f * Vector_X[2][0];
    matrix_result[2][3] = 2.0f * Vector_X[3][0];

    return matrix_result;
}

/**
 * @brief 四元数测量函数对测量噪声的雅可比矩阵
 *
 * @param Vector_X 状态向量
 */
Class_Matrix_f32<3, 3> Class_BMI088::EKF_Function_Jacobian_H_V(const Class_Matrix_f32<4, 1> &Vector_X, const float &D_T)
{
    return Namespace_ALG_Matrix::Identity<3, 3>();
}

/**
 * @brief 卡方检验
 *
 */
void Class_BMI088::Accel_Chi_Square_Calculate()
{
    Class_Matrix_f32<3, 1> vector_error;
    Class_Matrix_f32<3, 4> matrix_h_x;
    Class_Matrix_f32<3, 3> matrix_d;

    vector_error = Vector_Normalized_Accel - EKF_Quaternion.Function_H(EKF_Quaternion.Vector_X, D_T);

    matrix_h_x = EKF_Quaternion.Function_Jacobian_H_X(EKF_Quaternion.Vector_X, D_T);

    matrix_d = matrix_h_x * EKF_Quaternion.Matrix_P_Prior * matrix_h_x.Get_Transpose() + EKF_Quaternion.Matrix_R;

    Accel_Chi_Square_Loss = (vector_error.Get_Transpose() * matrix_d.Get_Inverse() * vector_error)[0][0];
}

/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/
