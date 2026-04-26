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
                Accel_Status.Transfering_Flag = false;
                Accel_Status.Update_Flag = true;
                Accel_Status.Update_Timestamp = SYS_Timestamp.Get_Now_Microsecond();
            }
            else if (SPI_Manage_Object->Rx_Buffer_Length == 2)
            {
                Temperature_Status.Transfering_Flag = false;
            }
        }
    }
    else if (SPI_Manage_Object->Activate_GPIOx == BMI088_GYRO__SPI_CS_GPIO_Port && SPI_Manage_Object->Activate_GPIO_Pin == BMI088_GYRO__SPI_CS_Pin)
    {
        BMI088_Gyro.SPI_RxCallback();

        if (Hardware_Init_Finished_Flag)
        {
            Gyro_Status.Transfering_Flag = false;
            Gyro_Status.Update_Flag = true;
            Gyro_Status.Update_Timestamp = SYS_Timestamp.Get_Now_Microsecond();
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
        Accel_Status.Ready_Flag = true;
        Accel_Status.Ready_Timestamp = SYS_Timestamp.Get_Now_Microsecond();
    }
    else if (GPIO_Pin == BMI088_GYRO__INTERRUPT_Pin)
    {
        Gyro_Status.Ready_Flag = true;
        Gyro_Status.Ready_Timestamp = SYS_Timestamp.Get_Now_Microsecond();
    }
}

/**
 * @brief 定时器周期中断回调函数
 *
 */
void Class_BMI088::TIM_128ms_Calculate_PeriodElapsedCallback()
{
    Temperature_Status.Ready_Flag = true;
    BMI088_Accel.TIM_128ms_Heater_PID_PeriodElapsedCallback();
}

/**
 * @brief 定时器周期中断回调函数
 *
 */
void Class_BMI088::TIM_125us_Calculate_PeriodElapsedCallback()
{
    // 当前时间戳
    uint64_t now_timestamp = SYS_Timestamp.Get_Now_Microsecond();

    Struct_BMI088_Status shadow_gyro_status = Gyro_Status;
    Struct_BMI088_Status shadow_accel_status = Accel_Status;

    // 角速度合法且在最大角加速度范围内则保存, 否则使用上次的数据
    Vector_Original_Gyro = BMI088_Gyro.Get_Raw_Gyro();
    if (BMI088_Gyro.Get_Valid_Flag())
    {
        if (BMI088_Accel.Get_Heater_Enable())
        {
            // 加热使能, 则使用校准后数据
            Vector_Original_Gyro += Class_Matrix_f32<3, 1>(GYRO_ZERO_OFFSET);
        }
    }
    else
    {
        Vector_Original_Gyro = Vector_Pre_Valid_Gyro;
    }

    // 加速度合法则保存, 不合法则跳过
    Vector_Original_Accel = BMI088_Accel.Get_Raw_Accel();
    if (BMI088_Accel.Get_Valid_Flag())
    {
        if (BMI088_Accel.Get_Heater_Enable())
        {
            // 加热使能, 则使用校准后数据
            Vector_Original_Accel = Class_Matrix_f32<3, 3>(ACCEL_AFFINE_DATA) * Vector_Original_Accel + Class_Matrix_f32<3, 1>(ACCEL_BIAS_DATA) * GRAVITY_ACCELERATION;
        }

        Vector_Normalized_Accel = Vector_Original_Accel.Get_Normalization();
    }
    else
    {
        shadow_accel_status.Update_Flag = false;
        Accel_Status.Update_Flag = false;
    }

    if (!EKF_Init_Finished_Flag && shadow_accel_status.Update_Flag && BMI088_Accel.Get_Valid_Flag())
    {
        // EKF未初始化, 且加速度数据更新且有效

        // EKF相关变量与函数
        Class_Matrix_f32<3, 3> matrix_q(EKF_Q);
        Class_Matrix_f32<3, 3> matrix_r(EKF_R);
        // 初始状态协方差矩阵
        Class_Matrix_f32<4, 4> matrix_p = Namespace_ALG_Matrix::Identity<4, 4>();
        // 初始状态向量
        Class_Matrix_f32<4, 1> vector_x;
        float init_euler[3] = {Vector_Euler_Angle[0][0], atan2f(Vector_Normalized_Accel[1][0], Vector_Normalized_Accel[2][0]), asinf(-Vector_Normalized_Accel[0][0])};
        vector_x = Namespace_ALG_Quaternion::From_Euler_Angle(Class_Matrix_f32<3, 1>(init_euler)).Get_Normalization();

        // 初始化EKF算法本体
        EKF_Quaternion.Init(matrix_q, matrix_r, matrix_p, vector_x);
        EKF_Quaternion.Config_Nonlinear_State_Model(EKF_Function_F, EKF_Function_Jacobian_F_X, EKF_Function_Jacobian_F_W);
        EKF_Quaternion.Config_Nonlinear_Measurement_Model(EKF_Function_H, EKF_Function_Jacobian_H_X, EKF_Function_Jacobian_H_V);

        // 设置标志位
        EKF_Init_Finished_Flag = true;
        Gyro_Status.Update_Flag = false;
        Accel_Status.Update_Flag = false;
        // 设置时间戳
        EKF_Pre_Valid_Timestamp = now_timestamp;
    }
    else if (EKF_Init_Finished_Flag)
    {
        // EKF已初始化, 执行下述逻辑

        if (!shadow_gyro_status.Update_Flag && !shadow_accel_status.Update_Flag)
        {
            // 没有数据更新, 则跳过内部状态更新逻辑
        }
        else if (shadow_gyro_status.Update_Flag && !shadow_accel_status.Update_Flag)
        {
            // 仅陀螺仪update存在, 则从pre_valid进行ekf预测到陀螺仪ready时间, pre_valid替换为陀螺仪ready时间, 陀螺仪update清除

            // 进行预测
            Valid_D_T = (shadow_gyro_status.Ready_Timestamp - EKF_Pre_Valid_Timestamp) / 1000000.0f;
            if (Valid_D_T > D_T_TIMEOUT_THRESHOLD)
            {
                EKF_Init_Finished_Flag = false;
                return;
            }
            EKF_Quaternion.Set_D_T(Valid_D_T);
            EKF_Quaternion.Vector_U = Vector_Original_Gyro;
            EKF_Quaternion.TIM_Predict_PeriodElapsedCallback();

            if (now_timestamp > shadow_gyro_status.Update_Timestamp)
            {
                Gyro_Status.Update_Flag = false;
            }
            EKF_Pre_Valid_Timestamp = shadow_gyro_status.Ready_Timestamp;
        }
        else if (!shadow_gyro_status.Update_Flag && shadow_accel_status.Update_Flag)
        {
            // 如若仅加速度计update存在, 则不进行操作
        }
        else if (shadow_gyro_status.Update_Flag && shadow_accel_status.Update_Flag)
        {
            // 根据时间戳分类讨论

            if (shadow_gyro_status.Ready_Timestamp == shadow_accel_status.Ready_Timestamp)
            {
                // 如若陀螺仪ready与加速度计的ready时间相等, 则直接一步完整ekf到位, 从pre_valid进行ekf预测到陀螺仪最新ready时间, pre_valid替换为陀螺仪ready时间, 陀螺仪和加速度计的update全部清除

                // 进行预测
                Valid_D_T = (shadow_gyro_status.Ready_Timestamp - EKF_Pre_Valid_Timestamp) / 1000000.0f;
                if (Valid_D_T > D_T_TIMEOUT_THRESHOLD)
                {
                    EKF_Init_Finished_Flag = false;
                    return;
                }
                EKF_Quaternion.Set_D_T(Valid_D_T);
                EKF_Quaternion.Vector_U = Vector_Original_Gyro;
                EKF_Quaternion.TIM_Predict_PeriodElapsedCallback();
                // 进行更新
                Accel_Chi_Square_Calculate();
                if (Accel_Chi_Square_Loss <= ACCEL_CHI_SQUARE_TEST_THRESHOLD)
                {
                    // 卡方检验通过, 更新
                    EKF_Quaternion.Vector_Z = Vector_Normalized_Accel;
                    EKF_Quaternion.TIM_Update_PeriodElapsedCallback();
                }

                if (now_timestamp > shadow_gyro_status.Update_Timestamp)
                {
                    Gyro_Status.Update_Flag = false;
                }
                if (now_timestamp > shadow_accel_status.Update_Timestamp)
                {
                    Accel_Status.Update_Flag = false;
                }
                EKF_Pre_Valid_Timestamp = shadow_gyro_status.Ready_Timestamp;
            }
            else if (shadow_gyro_status.Ready_Timestamp < shadow_accel_status.Ready_Timestamp)
            {
                // 如若陀螺仪ready早于加速度计的ready, 则从pre_valid进行ekf预测到陀螺仪最新ready时间, pre_valid替换为陀螺仪ready时间, 陀螺仪update清除

                // 进行预测
                Valid_D_T = (shadow_gyro_status.Ready_Timestamp - EKF_Pre_Valid_Timestamp) / 1000000.0f;
                if (Valid_D_T > D_T_TIMEOUT_THRESHOLD)
                {
                    EKF_Init_Finished_Flag = false;
                    return;
                }
                EKF_Quaternion.Set_D_T(Valid_D_T);
                EKF_Quaternion.Vector_U = Vector_Original_Gyro;
                EKF_Quaternion.TIM_Predict_PeriodElapsedCallback();

                if (now_timestamp > shadow_gyro_status.Update_Timestamp)
                {
                    Gyro_Status.Update_Flag = false;
                }
                EKF_Pre_Valid_Timestamp = shadow_gyro_status.Ready_Timestamp;
            }
            else if (shadow_gyro_status.Ready_Timestamp > shadow_accel_status.Ready_Timestamp)
            {
                // 如若陀螺仪ready晚于加速度计的ready, 则从pre_valid进行ekf预测到加速度计最新ready时间, 并进行ekf修正, 而后再从加速度计最新ready时间进行ekf预测到陀螺仪ready时间, pre_valid替换为陀螺仪ready时间, 陀螺仪update清除

                // 进行预测
                Valid_D_T = (shadow_accel_status.Ready_Timestamp - EKF_Pre_Valid_Timestamp) / 1000000.0f;
                if (Valid_D_T > D_T_TIMEOUT_THRESHOLD)
                {
                    EKF_Init_Finished_Flag = false;
                    return;
                }
                EKF_Quaternion.Set_D_T(Valid_D_T);
                EKF_Quaternion.Vector_U = Vector_Original_Gyro;
                EKF_Quaternion.TIM_Predict_PeriodElapsedCallback();
                // 进行更新
                Accel_Chi_Square_Calculate();
                if (Accel_Chi_Square_Loss <= ACCEL_CHI_SQUARE_TEST_THRESHOLD)
                {
                    // 卡方检验通过, 更新
                    EKF_Quaternion.Vector_Z = Vector_Normalized_Accel;
                    EKF_Quaternion.TIM_Update_PeriodElapsedCallback();
                }
                // 进行预测
                Valid_D_T = (shadow_gyro_status.Ready_Timestamp - shadow_accel_status.Ready_Timestamp) / 1000000.0f;
                if (Valid_D_T > D_T_TIMEOUT_THRESHOLD)
                {
                    EKF_Init_Finished_Flag = false;
                    return;
                }
                EKF_Quaternion.Set_D_T(Valid_D_T);
                EKF_Quaternion.Vector_U = Vector_Original_Gyro;
                EKF_Quaternion.TIM_Predict_PeriodElapsedCallback();

                if (now_timestamp > shadow_gyro_status.Update_Timestamp)
                {
                    Gyro_Status.Update_Flag = false;
                }
                if (now_timestamp > shadow_accel_status.Update_Timestamp)
                {
                    Accel_Status.Update_Flag = false;
                }
                EKF_Pre_Valid_Timestamp = Gyro_Status.Ready_Timestamp;
            }
        }

        // 数据输出

        // 输出四元数, 采用最新状态和陀螺仪预测来获取
        float dt = (now_timestamp - EKF_Pre_Valid_Timestamp) / 1000000.0f;
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

        if (BMI088_Gyro.Get_Valid_Flag())
        {
            Vector_Pre_Valid_Gyro = Vector_Original_Gyro;
        }
    }
}

/**
 * @brief 定时器周期中断回调函数
 *
 */
void Class_BMI088::TIM_10us_Calculate_PeriodElapsedCallback()
{
    if (Hardware_Init_Finished_Flag && Accel_Status.Ready_Flag && !Accel_Status.Transfering_Flag && !Gyro_Status.Transfering_Flag && !Temperature_Status.Transfering_Flag)
    {
        // 数据准备好, 读取加速度计
        Accel_Status.Transfering_Flag = true;
        Accel_Status.Transfering_Timestamp = SYS_Timestamp.Get_Now_Microsecond();
        BMI088_Accel.SPI_Request_Accel();
        Accel_Status.Ready_Flag = false;
        return;
    }

    if (Hardware_Init_Finished_Flag && Gyro_Status.Ready_Flag && !Accel_Status.Transfering_Flag && !Gyro_Status.Transfering_Flag && !Temperature_Status.Transfering_Flag)
    {
        // 数据准备好, 读取陀螺仪
        Gyro_Status.Transfering_Flag = true;
        Gyro_Status.Transfering_Timestamp = SYS_Timestamp.Get_Now_Microsecond();
        BMI088_Gyro.SPI_Request_Gyro();
        Gyro_Status.Ready_Flag = false;
        return;
    }

    if (Hardware_Init_Finished_Flag && Temperature_Status.Ready_Flag && !Accel_Status.Transfering_Flag && !Gyro_Status.Transfering_Flag && !Temperature_Status.Transfering_Flag)
    {
        // 温度准备好, 读取温度
        Temperature_Status.Transfering_Flag = true;
        Temperature_Status.Transfering_Timestamp = SYS_Timestamp.Get_Now_Microsecond();
        BMI088_Accel.SPI_Request_Temperature();
        Temperature_Status.Ready_Flag = false;
        return;
    }

    if (Hardware_Init_Finished_Flag && Accel_Status.Transfering_Flag && (SYS_Timestamp.Get_Now_Microsecond() - Accel_Status.Transfering_Timestamp) >= TRANSFERING_TIMEOUT)
    {
        // 加速度计传输超时
        Accel_Status.Transfering_Flag = true;
        Accel_Status.Transfering_Timestamp = SYS_Timestamp.Get_Now_Microsecond();
        BMI088_Accel.SPI_Request_Accel();
        Accel_Status.Ready_Flag = false;
        return;
    }

    if (Hardware_Init_Finished_Flag && Gyro_Status.Transfering_Flag && (SYS_Timestamp.Get_Now_Microsecond() - Gyro_Status.Transfering_Timestamp) >= TRANSFERING_TIMEOUT)
    {
        // 陀螺仪传输超时
        Gyro_Status.Transfering_Flag = true;
        Gyro_Status.Transfering_Timestamp = SYS_Timestamp.Get_Now_Microsecond();
        BMI088_Gyro.SPI_Request_Gyro();
        Gyro_Status.Ready_Flag = false;
        return;
    }

    if (Hardware_Init_Finished_Flag && Temperature_Status.Transfering_Flag && (SYS_Timestamp.Get_Now_Microsecond() - Temperature_Status.Transfering_Timestamp) >= TRANSFERING_TIMEOUT)
    {
        // 温度传输超时
        Temperature_Status.Transfering_Flag = true;
        Temperature_Status.Transfering_Timestamp = SYS_Timestamp.Get_Now_Microsecond();
        BMI088_Accel.SPI_Request_Temperature();
        Temperature_Status.Ready_Flag = false;
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
 * @brief 卡方检验
 *
 */
void Class_BMI088::Accel_Chi_Square_Calculate()
{
    Class_Matrix_f32<3, 1> vector_error;
    Class_Matrix_f32<3, 4> matrix_h_x;
    Class_Matrix_f32<3, 3> matrix_d;

    vector_error = Vector_Normalized_Accel - EKF_Quaternion.Function_H(EKF_Quaternion.Vector_X, Valid_D_T);

    matrix_h_x = EKF_Quaternion.Function_Jacobian_H_X(EKF_Quaternion.Vector_X, Valid_D_T);

    matrix_d = matrix_h_x * EKF_Quaternion.Matrix_P_Prior * matrix_h_x.Get_Transpose() + EKF_Quaternion.Matrix_R;

    Accel_Chi_Square_Loss = (vector_error.Get_Transpose() * matrix_d.Get_Inverse() * vector_error)[0][0];
}

/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/
