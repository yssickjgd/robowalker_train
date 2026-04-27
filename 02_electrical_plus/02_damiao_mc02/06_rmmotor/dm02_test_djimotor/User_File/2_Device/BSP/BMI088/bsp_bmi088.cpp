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

// 类内静态变量实例化
Class_Matrix_f32<4, 1> Class_BMI088::Quaternion_Tmp;
float Class_BMI088::Modulus_Tmp;
Class_Matrix_f32<4, 4> Class_BMI088::Orthogonalization_Tmp;


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

    Hardware_Init_Finished_Flag = true;
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

        if (Hardware_Init_Finished_Flag)
        {
            if (SPI_Manage_Object->Rx_Buffer_Length == 6)
            {
                Accel_Transfering_Flag = false;

                if (BMI088_Accel.Get_Valid_Flag())
                {
                    Struct_BMI088_Buffer_Element tmp_buffer;
                    tmp_buffer.Ready_Timestamp = Accel_Ready_Timestamp;
                    tmp_buffer.Value = Class_Matrix_f32<3, 3>(ACCEL_AFFINE_DATA) * BMI088_Accel.Get_Raw_Accel() + Class_Matrix_f32<3, 1>(ACCEL_BIAS_DATA) * GRAVITY_ACCELERATION;

                    Vector_Original_Accel = tmp_buffer.Value;

                    Queue_Accel.Push(tmp_buffer);
                }
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

        if (Hardware_Init_Finished_Flag)
        {
            Gyro_Transfering_Flag = false;

            if (BMI088_Gyro.Get_Valid_Flag())
            {
                Struct_BMI088_Buffer_Element tmp_buffer;
                tmp_buffer.Ready_Timestamp = Gyro_Ready_Timestamp;
                tmp_buffer.Value = BMI088_Gyro.Get_Raw_Gyro() + Class_Matrix_f32<3, 1>(GYRO_ZERO_OFFSET);

                Vector_Original_Gyro = tmp_buffer.Value;

                Queue_Gyro.Push(tmp_buffer);
            }
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
 * @brief 定时器周期中断回调函数, 默认1000Hz
 *
 */
void Class_BMI088::TIM_Calculate_PeriodElapsedCallback()
{
    // 计算时间戳
    float now_timestamp = SYS_Timestamp.Get_Now_Microsecond();

    if (!EKF_Init_Finished_Flag && Queue_Accel.Get_Length() > 0)
    {
        // EKF未初始化, 且有加速度数据
        EKF_Init(Queue_Accel.Get_Front().Value.Get_Normalization<3, 1>());
    }
    else if (EKF_Init_Finished_Flag)
    {
        // EKF已初始化, 逐个拿取加速度计和陀螺仪的元素, 进行时间戳对齐, 直至陀螺仪队列空
        while (Queue_Gyro.Get_Length() > 0)
        {
            // 根据队列是否空分情况讨论
            if (Queue_Accel.Get_Length() == 0 || Queue_Accel.Get_Front().Ready_Timestamp > Queue_Gyro.Get_Front().Ready_Timestamp)
            {
                // 加速度计队列空, 或者加速度计时间戳晚于陀螺仪时间戳, 则直接仅进行预测即可, 而后推出陀螺仪数据

                Struct_BMI088_Buffer_Element tmp_gyro = Queue_Gyro.Get_Front();

                float dt = (tmp_gyro.Ready_Timestamp - EKF_Pre_Valid_Timestamp) / 1000000.0f;
                if (dt > D_T_TIMEOUT_THRESHOLD)
                {
                    EKF_Init_Finished_Flag = false;
                    Queue_Accel.Clear();
                    Queue_Gyro.Clear();
                    return;
                }

                EKF_Quaternion.Set_D_T(dt);
                EKF_Quaternion.Vector_U = tmp_gyro.Value;
                EKF_Quaternion.TIM_Predict_PeriodElapsedCallback();

                EKF_Pre_Valid_Timestamp = tmp_gyro.Ready_Timestamp;

                Queue_Gyro.Pop();
            }
            else
            {
                // 加速度计队列非空, 且加速度计时间戳不晚于陀螺仪时间戳, 则预测到加速度计时间戳再进行更新, 而后推出加速度计数据, 如若陀螺仪时间戳与加速度计时间戳相同, 则同时推出陀螺仪数据

                Struct_BMI088_Buffer_Element tmp_gyro = Queue_Gyro.Get_Front();
                Struct_BMI088_Buffer_Element tmp_accel = Queue_Accel.Get_Front();
                Class_Matrix_f32<3, 1> tmp_normalized_accel = tmp_accel.Value.Get_Normalization<3, 1>();

                float dt = (tmp_accel.Ready_Timestamp - EKF_Pre_Valid_Timestamp) / 1000000.0f;
                if (dt > D_T_TIMEOUT_THRESHOLD)
                {
                    EKF_Init_Finished_Flag = false;
                    Queue_Accel.Clear();
                    Queue_Gyro.Clear();
                    return;
                }

                EKF_Quaternion.Set_D_T(dt);
                EKF_Quaternion.Vector_U = tmp_gyro.Value;
                EKF_Quaternion.TIM_Predict_PeriodElapsedCallback();

                Accel_Chi_Square_Calculate(tmp_normalized_accel, dt);
                if (Accel_Chi_Square_Loss <= ACCEL_CHI_SQUARE_TEST_THRESHOLD)
                {
                    // 卡方检验通过, 更新
                    EKF_Quaternion.Vector_Z = tmp_normalized_accel;
                    EKF_Quaternion.TIM_Update_PeriodElapsedCallback();
                }

                EKF_Pre_Valid_Timestamp = tmp_accel.Ready_Timestamp;

                Queue_Accel.Pop();
                if (tmp_gyro.Ready_Timestamp == tmp_accel.Ready_Timestamp)
                {
                    Queue_Gyro.Pop();
                }
            }
        }

        // 数据输出

        float dt = (now_timestamp - EKF_Pre_Valid_Timestamp) / 1000000.0f;

        // 输出四元数, 采用最新状态和陀螺仪预测来获取
        Quarternion = EKF_Function_F(EKF_Quaternion.Vector_X, Vector_Original_Gyro, dt).Get_Normalization();
        // 输出姿态相关变量
        Vector_Euler_Angle = Quarternion.Get_Euler_Angle();
        Matrix_Rotation = Quarternion.Get_Rotation_Matrix();
        Vector_Axis_Angle = Quarternion.Get_Axis_Angle();
        // 机体坐标系下的重力加速度
        Class_Matrix_f32<3, 1> vector_gravity_body = Matrix_Rotation.Get_Transpose() * (-Namespace_ALG_Matrix::Axis_Z_3d() * GRAVITY_ACCELERATION);
        // 输出运动学相关变量
        Vector_Accel_Body = Vector_Original_Accel + vector_gravity_body;
        Vector_Accel_Odom = Matrix_Rotation * Vector_Accel_Body;
        Vector_Gyro_Body = Vector_Original_Gyro;
        Vector_Gyro_Odom = Matrix_Rotation * Vector_Gyro_Body;

        Calculating_Time = SYS_Timestamp.Get_Now_Microsecond() - now_timestamp;
    }
}

/**
 * @brief 定时器周期中断回调函数, 轮询队首元素, 力争每个队列每次新的中断到来时, 上一次请求的数据都已请求完毕
 *
 */
void Class_BMI088::TIM_10us_Transmission_Scheduling_PeriodElapsedCallback()
{
    // 数据准备好且没有传输中
    if (Hardware_Init_Finished_Flag)
    {
        // 数据准备好且未有传输
        if (Accel_Ready_Flag && !Accel_Transfering_Flag && !Gyro_Transfering_Flag && !Temperature_Transfering_Flag)
        {
            Accel_Transfering_Flag = true;
            Accel_Transfering_Timestamp = SYS_Timestamp.Get_Now_Microsecond();
            BMI088_Accel.SPI_Request_Accel();
            Accel_Ready_Flag = false;
        }
        else if (Gyro_Ready_Flag && !Accel_Transfering_Flag && !Gyro_Transfering_Flag && !Temperature_Transfering_Flag)
        {
            Gyro_Transfering_Flag = true;
            Gyro_Transfering_Timestamp = SYS_Timestamp.Get_Now_Microsecond();
            BMI088_Gyro.SPI_Request_Gyro();
            Gyro_Ready_Flag = false;
        }
        else if (Temperature_Ready_Flag && !Accel_Transfering_Flag && !Gyro_Transfering_Flag && !Temperature_Transfering_Flag)
        {
            Temperature_Transfering_Flag = true;
            Temperature_Transfering_Timestamp = SYS_Timestamp.Get_Now_Microsecond();
            BMI088_Accel.SPI_Request_Temperature();
            Temperature_Ready_Flag = false;
        }
        // 数据传输超时
        else if (Accel_Transfering_Flag && (SYS_Timestamp.Get_Now_Microsecond() - Accel_Transfering_Timestamp) >= TRANSFERING_TIMEOUT)
        {
            Accel_Transfering_Flag = true;
            Accel_Transfering_Timestamp = SYS_Timestamp.Get_Now_Microsecond();
            BMI088_Accel.SPI_Request_Accel();
            Accel_Ready_Flag = false;
        }
        else if (Gyro_Transfering_Flag && (SYS_Timestamp.Get_Now_Microsecond() - Gyro_Transfering_Timestamp >= TRANSFERING_TIMEOUT))
        {
            Gyro_Transfering_Flag = true;
            Gyro_Transfering_Timestamp = SYS_Timestamp.Get_Now_Microsecond();
            BMI088_Gyro.SPI_Request_Gyro();
            Gyro_Ready_Flag = false;
        }
        else if (Temperature_Transfering_Flag && (SYS_Timestamp.Get_Now_Microsecond() - Temperature_Transfering_Timestamp >= TRANSFERING_TIMEOUT))
        {
            Temperature_Transfering_Flag = true;
            Temperature_Transfering_Timestamp = SYS_Timestamp.Get_Now_Microsecond();
            BMI088_Accel.SPI_Request_Temperature();
            Temperature_Ready_Flag = false;
        }
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

    Quaternion_Tmp = Vector_X + 0.5f * D_T * matrix_omega * Vector_X;
    Modulus_Tmp = 1.0f / Quaternion_Tmp.Get_Modulus<4, 1>();
    Orthogonalization_Tmp = Modulus_Tmp * (Namespace_ALG_Matrix::Identity<4, 4>() - Modulus_Tmp * Modulus_Tmp * Quaternion_Tmp * Quaternion_Tmp.Get_Transpose());

    matrix_result = Quaternion_Tmp.Get_Normalization<4, 1>();

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

    matrix_result = Orthogonalization_Tmp * (Namespace_ALG_Matrix::Identity<4, 4>() + 0.5f * D_T * matrix_omega);

    return matrix_result;
}

/**
 * @brief 四元数状态转移函数对过程噪声的雅可比矩阵
 *
 * @param Vector_X 状态向量
 * @param Vector_U 输入向量
 * @param D_T 时间间隔
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

    matrix_result = Orthogonalization_Tmp * (0.5f * D_T * matrix_q);

    return matrix_result;
}

/**
 * @brief 四元数测量函数
 *
 * @param Vector_X 状态向量
 * @param D_T 时间间隔
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
 * @param D_T 时间间隔
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
 * @param D_T 时间间隔
 */
Class_Matrix_f32<3, 3> Class_BMI088::EKF_Function_Jacobian_H_V(const Class_Matrix_f32<4, 1> &Vector_X, const float &D_T)
{
    return Namespace_ALG_Matrix::Identity<3, 3>();
}

/**
 * @brief EKF相关初始化逻辑
 *
 * @param Vector_Normalized_Accel 加速度计归一化向量, 用于EKF初始状态的确定
 */
void Class_BMI088::EKF_Init(const Class_Matrix_f32<3, 1> &Vector_Normalized_Accel)
{
    // EKF相关变量与函数
    Class_Matrix_f32<3, 3> matrix_q(EKF_Q);
    Class_Matrix_f32<3, 3> matrix_r(EKF_R);
    // 初始状态协方差矩阵
    Class_Matrix_f32<4, 4> matrix_p = Namespace_ALG_Matrix::Identity<4, 4>();
    // 初始状态向量
    Class_Matrix_f32<4, 1> vector_x;
    float init_euler[3] = {Vector_Euler_Angle[0][0], asinf(-Vector_Normalized_Accel[0][0]), atan2f(Vector_Normalized_Accel[1][0], Vector_Normalized_Accel[2][0])};
    vector_x = Namespace_ALG_Quaternion::From_Euler_Angle(Class_Matrix_f32<3, 1>(init_euler)).Get_Normalization();

    // 初始化EKF算法本体
    EKF_Quaternion.Init(matrix_q, matrix_r, matrix_p, vector_x);
    EKF_Quaternion.Config_Nonlinear_State_Model(EKF_Function_F, EKF_Function_Jacobian_F_X, EKF_Function_Jacobian_F_W);
    EKF_Quaternion.Config_Nonlinear_Measurement_Model(EKF_Function_H, EKF_Function_Jacobian_H_X, EKF_Function_Jacobian_H_V);

    // 设置标志位
    EKF_Init_Finished_Flag = true;

    // 清理掉所有时间戳早于加速度该时间戳的角速度队列信息
    while (Queue_Gyro.Get_Length() > 0 && Queue_Gyro.Get_Front().Ready_Timestamp < Queue_Accel.Get_Front().Ready_Timestamp)
    {
        Queue_Gyro.Pop();
     }

    // 设置时间戳
    EKF_Pre_Valid_Timestamp = Queue_Accel.Get_Front().Ready_Timestamp;

    // 推出用过的加速度的数据
    Queue_Accel.Pop();
}


/**
 * @brief 卡方检验
 *
 */
void Class_BMI088::Accel_Chi_Square_Calculate(const Class_Matrix_f32<3, 1> &Vector_Normalized_Accel, const float &D_T)
{
    Class_Matrix_f32<3, 1> vector_error;
    Class_Matrix_f32<3, 4> matrix_h_x;
    Class_Matrix_f32<3, 3> matrix_d;

    vector_error = Vector_Normalized_Accel - EKF_Function_H(EKF_Quaternion.Vector_X, D_T);

    matrix_h_x = EKF_Function_Jacobian_H_X(EKF_Quaternion.Vector_X, D_T);

    matrix_d = matrix_h_x * EKF_Quaternion.Matrix_P_Prior * matrix_h_x.Get_Transpose() + EKF_Quaternion.Matrix_R;

    Accel_Chi_Square_Loss = (vector_error.Get_Transpose() * matrix_d.Get_Inverse() * vector_error)[0][0];
}

/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/
