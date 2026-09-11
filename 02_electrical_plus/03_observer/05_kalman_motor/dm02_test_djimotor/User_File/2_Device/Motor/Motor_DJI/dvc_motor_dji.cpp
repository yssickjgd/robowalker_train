/**
 * @file dvc_motor_dji.cpp
 * @author yssickjgd (1345578933@qq.com)
 * @brief 大疆电机配置与操作
 * @version 1.2
 * @date 2023-08-29 0.1 23赛季定稿
 * @date 2024-01-01 1.1 官方6020驱动更新, 适配电压控制与电流控制
 * @date 2024-03-07 1.2 新增功率控制接口与相关函数, 24赛季定稿
 * @date 2025-12-03 2.1 彻底抛弃6020电压控制版本, 仅保留电流控制版本
 *
 * @copyright USTC-RoboWalker (c) 2023-2025
 *
 */

/* Includes ------------------------------------------------------------------*/

#include "dvc_motor_dji.h"

/* Private macros ------------------------------------------------------------*/

/* Private types -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function declarations ---------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/

/**
 * @brief 分配CAN发送缓冲区
 *
 * @param hcan CAN编号
 * @param __CAN_ID CAN ID
 * @param __DJI_Motor_Driver_Version 大疆驱动版本, 当且仅当当前被分配电机为6020, 且是电流驱动新版本时选2023, 否则都是default
 * @return uint8_t* 缓冲区指针
 */
uint8_t *allocate_tx_data(const FDCAN_HandleTypeDef *hcan, const Enum_Motor_DJI_ID &__CAN_ID, const Enum_Motor_DJI_GM6020_Driver_Version &__DJI_Motor_Driver_Version = Motor_DJI_GM6020_Driver_Version_DEFAULT)
{
    uint8_t *tmp_tx_data_ptr = nullptr;
    if (hcan == &hfdcan1)
    {
        switch (__CAN_ID)
        {
        case (Motor_DJI_ID_0x201):
        {
            tmp_tx_data_ptr = &(CAN1_0x200_Tx_Data[0]);

            break;
        }
        case (Motor_DJI_ID_0x202):
        {
            tmp_tx_data_ptr = &(CAN1_0x200_Tx_Data[2]);

            break;
        }
        case (Motor_DJI_ID_0x203):
        {
            tmp_tx_data_ptr = &(CAN1_0x200_Tx_Data[4]);

            break;
        }
        case (Motor_DJI_ID_0x204):
        {
            tmp_tx_data_ptr = &(CAN1_0x200_Tx_Data[6]);

            break;
        }
        case (Motor_DJI_ID_0x205):
        {
            if (__DJI_Motor_Driver_Version == Motor_DJI_GM6020_Driver_Version_DEFAULT)
            {
                tmp_tx_data_ptr = &(CAN1_0x1ff_Tx_Data[0]);
            }
            else
            {
                tmp_tx_data_ptr = &(CAN1_0x1fe_Tx_Data[0]);
            }

            break;
        }
        case (Motor_DJI_ID_0x206):
        {
            if (__DJI_Motor_Driver_Version == Motor_DJI_GM6020_Driver_Version_DEFAULT)
            {
                tmp_tx_data_ptr = &(CAN1_0x1ff_Tx_Data[2]);
            }
            else
            {
                tmp_tx_data_ptr = &(CAN1_0x1fe_Tx_Data[2]);
            }

            break;
        }
        case (Motor_DJI_ID_0x207):
        {
            if (__DJI_Motor_Driver_Version == Motor_DJI_GM6020_Driver_Version_DEFAULT)
            {
                tmp_tx_data_ptr = &(CAN1_0x1ff_Tx_Data[4]);
            }
            else
            {
                tmp_tx_data_ptr = &(CAN1_0x1fe_Tx_Data[4]);
            }

            break;
        }
        case (Motor_DJI_ID_0x208):
        {
            if (__DJI_Motor_Driver_Version == Motor_DJI_GM6020_Driver_Version_DEFAULT)
            {
                tmp_tx_data_ptr = &(CAN1_0x1ff_Tx_Data[6]);
            }
            else
            {
                tmp_tx_data_ptr = &(CAN1_0x1fe_Tx_Data[6]);
            }

            break;
        }
        case (Motor_DJI_ID_0x209):
        {
            if (__DJI_Motor_Driver_Version == Motor_DJI_GM6020_Driver_Version_DEFAULT)
            {
                tmp_tx_data_ptr = &(CAN1_0x2ff_Tx_Data[0]);
            }
            else
            {
                tmp_tx_data_ptr = &(CAN1_0x2fe_Tx_Data[0]);
            }

            break;
        }
        case (Motor_DJI_ID_0x20A):
        {
            if (__DJI_Motor_Driver_Version == Motor_DJI_GM6020_Driver_Version_DEFAULT)
            {
                tmp_tx_data_ptr = &(CAN1_0x2ff_Tx_Data[2]);
            }
            else
            {
                tmp_tx_data_ptr = &(CAN1_0x2fe_Tx_Data[2]);
            }

            break;
        }
        case (Motor_DJI_ID_0x20B):
        {
            if (__DJI_Motor_Driver_Version == Motor_DJI_GM6020_Driver_Version_DEFAULT)
            {
                tmp_tx_data_ptr = &(CAN1_0x2ff_Tx_Data[4]);
            }
            else
            {
                tmp_tx_data_ptr = &(CAN1_0x2fe_Tx_Data[4]);
            }

            break;
        }
        }
    }
    else if (hcan == &hfdcan2)
    {
        switch (__CAN_ID)
        {
        case (Motor_DJI_ID_0x201):
        {
            tmp_tx_data_ptr = &(CAN2_0x200_Tx_Data[0]);

            break;
        }
        case (Motor_DJI_ID_0x202):
        {
            tmp_tx_data_ptr = &(CAN2_0x200_Tx_Data[2]);

            break;
        }
        case (Motor_DJI_ID_0x203):
        {
            tmp_tx_data_ptr = &(CAN2_0x200_Tx_Data[4]);

            break;
        }
        case (Motor_DJI_ID_0x204):
        {
            tmp_tx_data_ptr = &(CAN2_0x200_Tx_Data[6]);

            break;
        }
        case (Motor_DJI_ID_0x205):
        {
            if (__DJI_Motor_Driver_Version == Motor_DJI_GM6020_Driver_Version_DEFAULT)
            {
                tmp_tx_data_ptr = &(CAN2_0x1ff_Tx_Data[0]);
            }
            else
            {
                tmp_tx_data_ptr = &(CAN2_0x1fe_Tx_Data[0]);
            }

            break;
        }
        case (Motor_DJI_ID_0x206):
        {
            if (__DJI_Motor_Driver_Version == Motor_DJI_GM6020_Driver_Version_DEFAULT)
            {
                tmp_tx_data_ptr = &(CAN2_0x1ff_Tx_Data[2]);
            }
            else
            {
                tmp_tx_data_ptr = &(CAN2_0x1fe_Tx_Data[2]);
            }

            break;
        }
        case (Motor_DJI_ID_0x207):
        {
            if (__DJI_Motor_Driver_Version == Motor_DJI_GM6020_Driver_Version_DEFAULT)
            {
                tmp_tx_data_ptr = &(CAN2_0x1ff_Tx_Data[4]);
            }
            else
            {
                tmp_tx_data_ptr = &(CAN2_0x1fe_Tx_Data[4]);
            }

            break;
        }
        case (Motor_DJI_ID_0x208):
        {
            if (__DJI_Motor_Driver_Version == Motor_DJI_GM6020_Driver_Version_DEFAULT)
            {
                tmp_tx_data_ptr = &(CAN2_0x1ff_Tx_Data[6]);
            }
            else
            {
                tmp_tx_data_ptr = &(CAN2_0x1fe_Tx_Data[6]);
            }

            break;
        }
        case (Motor_DJI_ID_0x209):
        {
            if (__DJI_Motor_Driver_Version == Motor_DJI_GM6020_Driver_Version_DEFAULT)
            {
                tmp_tx_data_ptr = &(CAN2_0x2ff_Tx_Data[0]);
            }
            else
            {
                tmp_tx_data_ptr = &(CAN2_0x2fe_Tx_Data[0]);
            }

            break;
        }
        case (Motor_DJI_ID_0x20A):
        {
            if (__DJI_Motor_Driver_Version == Motor_DJI_GM6020_Driver_Version_DEFAULT)
            {
                tmp_tx_data_ptr = &(CAN2_0x2ff_Tx_Data[2]);
            }
            else
            {
                tmp_tx_data_ptr = &(CAN2_0x2fe_Tx_Data[2]);
            }

            break;
        }
        case (Motor_DJI_ID_0x20B):
        {
            if (__DJI_Motor_Driver_Version == Motor_DJI_GM6020_Driver_Version_DEFAULT)
            {
                tmp_tx_data_ptr = &(CAN2_0x2ff_Tx_Data[4]);
            }
            else
            {
                tmp_tx_data_ptr = &(CAN2_0x2fe_Tx_Data[4]);
            }

            break;
        }
        }
    }
    else if (hcan == &hfdcan3)
    {
        switch (__CAN_ID)
        {
        case (Motor_DJI_ID_0x201):
        {
            tmp_tx_data_ptr = &(CAN3_0x200_Tx_Data[0]);

            break;
        }
        case (Motor_DJI_ID_0x202):
        {
            tmp_tx_data_ptr = &(CAN3_0x200_Tx_Data[2]);

            break;
        }
        case (Motor_DJI_ID_0x203):
        {
            tmp_tx_data_ptr = &(CAN3_0x200_Tx_Data[4]);

            break;
        }
        case (Motor_DJI_ID_0x204):
        {
            tmp_tx_data_ptr = &(CAN3_0x200_Tx_Data[6]);

            break;
        }
        case (Motor_DJI_ID_0x205):
        {
            if (__DJI_Motor_Driver_Version == Motor_DJI_GM6020_Driver_Version_DEFAULT)
            {
                tmp_tx_data_ptr = &(CAN3_0x1ff_Tx_Data[0]);
            }
            else
            {
                tmp_tx_data_ptr = &(CAN3_0x1fe_Tx_Data[0]);
            }

            break;
        }
        case (Motor_DJI_ID_0x206):
        {
            if (__DJI_Motor_Driver_Version == Motor_DJI_GM6020_Driver_Version_DEFAULT)
            {
                tmp_tx_data_ptr = &(CAN3_0x1ff_Tx_Data[2]);
            }
            else
            {
                tmp_tx_data_ptr = &(CAN3_0x1fe_Tx_Data[2]);
            }

            break;
        }
        case (Motor_DJI_ID_0x207):
        {
            if (__DJI_Motor_Driver_Version == Motor_DJI_GM6020_Driver_Version_DEFAULT)
            {
                tmp_tx_data_ptr = &(CAN3_0x1ff_Tx_Data[4]);
            }
            else
            {
                tmp_tx_data_ptr = &(CAN3_0x1fe_Tx_Data[4]);
            }
            break;
        }
        case (Motor_DJI_ID_0x208):
        {
            if (__DJI_Motor_Driver_Version == Motor_DJI_GM6020_Driver_Version_DEFAULT)
            {
                tmp_tx_data_ptr = &(CAN3_0x1ff_Tx_Data[6]);
            }
            else
            {
                tmp_tx_data_ptr = &(CAN3_0x1fe_Tx_Data[6]);
            }
            break;
        }
        case (Motor_DJI_ID_0x209):
        {
            if (__DJI_Motor_Driver_Version == Motor_DJI_GM6020_Driver_Version_DEFAULT)
            {
                tmp_tx_data_ptr = &(CAN3_0x2ff_Tx_Data[0]);
            }
            else
            {
                tmp_tx_data_ptr = &(CAN3_0x2fe_Tx_Data[0]);
            }
            break;
        }
        case (Motor_DJI_ID_0x20A):
        {
            if (__DJI_Motor_Driver_Version == Motor_DJI_GM6020_Driver_Version_DEFAULT)
            {
                tmp_tx_data_ptr = &(CAN3_0x2ff_Tx_Data[2]);
            }
            else
            {
                tmp_tx_data_ptr = &(CAN3_0x2fe_Tx_Data[2]);
            }
            break;
        }
        case (Motor_DJI_ID_0x20B):
        {
            if (__DJI_Motor_Driver_Version == Motor_DJI_GM6020_Driver_Version_DEFAULT)
            {
                tmp_tx_data_ptr = &(CAN3_0x2ff_Tx_Data[4]);
            }
            else
            {
                tmp_tx_data_ptr = &(CAN3_0x2fe_Tx_Data[4]);
            }
            break;
        }
        }
    }
    return (tmp_tx_data_ptr);
}

/**
 * @brief 估计功率值
 *
 * @param K_0 电机建模系数
 * @param K_1 电机建模系数
 * @param K_2 电机建模系数
 * @param A 电机建模系数
 * @param Torque 减速箱前扭矩, Nm
 * @param Omega 减速箱前角速度, rad/s
 * @return
 */
float power_calculate(float K_0, float K_1, float K_2, float A, float Torque, float Omega)
{
    return (K_0 * Torque * Omega + K_1 * Omega * Omega + K_2 * Torque * Torque + A);
}

/**
 * @brief 电机初始化
 *
 * @param hcan 绑定的CAN总线
 * @param __CAN_Rx_ID 绑定的CAN ID
 * @param __Motor_DJI_Control_Method 电机控制方式, 默认角度
 * @param __Encoder_Offset 编码器偏移, 默认0
 * @param __Nearest_Angle 就近转位的单次最大旋转角度, 其数值一般为圆周的整数倍或纯小数倍, 且纯小数倍时可均分圆周, 0表示不启用就近转位
 */
void Class_Motor_DJI_GM6020::Init(const FDCAN_HandleTypeDef *hcan, const Enum_Motor_DJI_ID &__CAN_Rx_ID, const Enum_Motor_DJI_Control_Method &__Motor_DJI_Control_Method, const int32_t &__Encoder_Offset, const float &__Nearest_Angle)
{
    if (hcan->Instance == FDCAN1)
    {
        CAN_Manage_Object = &CAN1_Manage_Object;
    }
    else if (hcan->Instance == FDCAN2)
    {
        CAN_Manage_Object = &CAN2_Manage_Object;
    }
    else if (hcan->Instance == FDCAN3)
    {
        CAN_Manage_Object = &CAN3_Manage_Object;
    }
    CAN_Rx_ID = __CAN_Rx_ID;
    Motor_DJI_Control_Method = __Motor_DJI_Control_Method;
    Encoder_Offset = __Encoder_Offset;
    Nearest_Angle = __Nearest_Angle;
    Tx_Data = allocate_tx_data(hcan, __CAN_Rx_ID, Motor_DJI_GM6020_Driver_Version_6020);
}

/**
 * @brief CAN通信接收回调函数
 *
 */
void Class_Motor_DJI_GM6020::CAN_RxCpltCallback()
{
    // 滑动窗口, 判断电机是否在线
    Flag += 1;

    Data_Process();
}

/**
 * @brief TIM定时器中断定期检测电机是否存活
 *
 */
void Class_Motor_DJI_GM6020::TIM_100ms_Alive_PeriodElapsedCallback()
{
    // 判断该时间段内是否接收过电机数据
    if (Flag == Pre_Flag)
    {
        // 电机断开连接
        Motor_DJI_Status = Motor_DJI_Status_DISABLE;
        PID_Angle.Set_Integral_Error(0.0f);
        PID_Omega.Set_Integral_Error(0.0f);
    }
    else
    {
        // 电机保持连接
        Motor_DJI_Status = Motor_DJI_Status_ENABLE;
    }
    Pre_Flag = Flag;
}

/**
 * @brief TIM定时器中断计算回调函数, 计算周期取决于电机反馈周期
 *
 */
void Class_Motor_DJI_GM6020::TIM_Calculate_PeriodElapsedCallback()
{
    PID_Calculate();

    Out = (Target_Torque + Feedforward_Torque) / CURRENT_TO_TORQUE * CURRENT_TO_OUT;
    Basic_Math_Constrain(&Out, -OUT_MAX, OUT_MAX);

    Output();

    Feedforward_Omega = 0.0f;
    Feedforward_Torque = 0.0f;
}

/**
 * @brief 数据处理过程
 *
 */
void Class_Motor_DJI_GM6020::Data_Process()
{
    // 数据处理过程
    int16_t delta_encoder;
    uint16_t tmp_encoder;
    int16_t tmp_omega, tmp_current;
    Struct_Motor_DJI_CAN_Rx_Data *tmp_buffer = (Struct_Motor_DJI_CAN_Rx_Data *) CAN_Manage_Object->Rx_Buffer;

    // 处理大小端
    Basic_Math_Endian_Reverse_16((void *) &tmp_buffer->Encoder_Reverse, (void *) &tmp_encoder);
    Basic_Math_Endian_Reverse_16((void *) &tmp_buffer->Omega_Reverse, (void *) &tmp_omega);
    Basic_Math_Endian_Reverse_16((void *) &tmp_buffer->Current_Reverse, (void *) &tmp_current);

    // 计算圈数与总编码器值
    delta_encoder = tmp_encoder - Rx_Data.Pre_Encoder;
    if (delta_encoder < -ENCODER_NUM_PER_ROUND / 2)
    {
        // 正方向转过了一圈
        Rx_Data.Total_Round++;
    }
    else if (delta_encoder > ENCODER_NUM_PER_ROUND / 2)
    {
        // 反方向转过了一圈
        Rx_Data.Total_Round--;
    }
    Rx_Data.Total_Encoder = Rx_Data.Total_Round * ENCODER_NUM_PER_ROUND + tmp_encoder + Encoder_Offset;

    // 计算电机本身信息
    Rx_Data.Now_Angle = (float) Rx_Data.Total_Encoder / (float) ENCODER_NUM_PER_ROUND * 2.0f * PI;
    Rx_Data.Now_Omega = (float) tmp_omega * BASIC_MATH_RPM_TO_RADPS;
    Rx_Data.Now_Torque = tmp_current / CURRENT_TO_OUT * CURRENT_TO_TORQUE;
    Rx_Data.Now_Temperature = tmp_buffer->Temperature + BASIC_MATH_CELSIUS_TO_KELVIN;
    Rx_Data.Now_Power = power_calculate(POWER_K_0, POWER_K_1, POWER_K_2, POWER_A, Rx_Data.Now_Torque, Rx_Data.Now_Omega);

    // 存储预备信息
    Rx_Data.Pre_Encoder = tmp_encoder;
}

/**
 * @brief 计算PID
 *
 */
void Class_Motor_DJI_GM6020::PID_Calculate()
{
    switch (Motor_DJI_Control_Method)
    {
    case (Motor_DJI_Control_Method_TORQUE):
    {
        break;
    }
    case (Motor_DJI_Control_Method_OMEGA):
    {

        PID_Omega.Set_Target(Target_Omega + Feedforward_Omega);
        PID_Omega.Set_Now(Rx_Data.Now_Omega);
        PID_Omega.TIM_Calculate_PeriodElapsedCallback();

        Target_Torque = PID_Omega.Get_Out();

        break;
    }
    case (Motor_DJI_Control_Method_ANGLE):
    {
        PID_Angle.Set_Target(Target_Angle);
        PID_Angle.Set_Now(Rx_Data.Now_Angle);
        PID_Angle.TIM_Calculate_PeriodElapsedCallback();

        Target_Omega = PID_Angle.Get_Out();

        PID_Omega.Set_Target(Target_Omega + Feedforward_Omega);
        PID_Omega.Set_Now(Rx_Data.Now_Omega);
        PID_Omega.TIM_Calculate_PeriodElapsedCallback();

        Target_Torque = PID_Omega.Get_Out();

        break;
    }
    }
}

/**
 * @brief 电机数据输出到CAN总线发送缓冲区
 *
 */
void Class_Motor_DJI_GM6020::Output()
{
    Tx_Data[0] = (int16_t) Out >> 8;
    Tx_Data[1] = (int16_t) Out;
}

/**
 * @brief 电机初始化
 *
 * @param hcan CAN编号
 * @param __CAN_Rx_ID CAN ID
 * @param __Motor_DJI_Control_Method 电机控制方式, 默认角度
 * @param __Gearbox_Rate 减速箱减速比, 默认为原装减速箱, 如拆去减速箱则该值设为1
 * @param __Current_Max 最大电流
 */
void Class_Motor_DJI_C610::Init(const FDCAN_HandleTypeDef *hcan, const Enum_Motor_DJI_ID &__CAN_Rx_ID, const Enum_Motor_DJI_Control_Method &__Motor_DJI_Control_Method, const float &__Gearbox_Rate)
{
    if (hcan->Instance == FDCAN1)
    {
        CAN_Manage_Object = &CAN1_Manage_Object;
    }
    else if (hcan->Instance == FDCAN2)
    {
        CAN_Manage_Object = &CAN2_Manage_Object;
    }
    else if (hcan->Instance == FDCAN3)
    {
        CAN_Manage_Object = &CAN3_Manage_Object;
    }
    CAN_Rx_ID = __CAN_Rx_ID;
    Motor_DJI_Control_Method = __Motor_DJI_Control_Method;
    Gearbox_Rate = __Gearbox_Rate;
    Tx_Data = allocate_tx_data(hcan, __CAN_Rx_ID);
}

/**
 * @brief CAN通信接收回调函数
 *
 */
void Class_Motor_DJI_C610::CAN_RxCpltCallback()
{
    // 滑动窗口, 判断电机是否在线
    Flag += 1;

    Data_Process();
}

/**
 * @brief TIM定时器中断定期检测电机是否存活
 *
 */
void Class_Motor_DJI_C610::TIM_100ms_Alive_PeriodElapsedCallback()
{
    // 判断该时间段内是否接收过电机数据
    if (Flag == Pre_Flag)
    {
        // 电机断开连接
        Motor_DJI_Status = Motor_DJI_Status_DISABLE;
        PID_Angle.Set_Integral_Error(0.0f);
        PID_Omega.Set_Integral_Error(0.0f);
    }
    else
    {
        // 电机保持连接
        Motor_DJI_Status = Motor_DJI_Status_ENABLE;
    }
    Pre_Flag = Flag;
}

/**
 * @brief TIM定时器中断计算回调函数, 计算周期取决于电机反馈周期
 *
 */
void Class_Motor_DJI_C610::TIM_Calculate_PeriodElapsedCallback()
{
    PID_Calculate();

    Out = (Target_Torque + Feedforward_Torque) / CURRENT_TO_TORQUE / Gearbox_Rate * CURRENT_TO_OUT;
    Basic_Math_Constrain(&Out, -OUT_MAX, OUT_MAX);

    Output();

    Feedforward_Torque = 0.0f;
    Feedforward_Omega = 0.0f;
}

/**
 * @brief 数据处理过程
 *
 */
void Class_Motor_DJI_C610::Data_Process()
{
    // 数据处理过程
    int16_t delta_encoder;
    uint16_t tmp_encoder;
    int16_t tmp_omega, tmp_current;
    Struct_Motor_DJI_CAN_Rx_Data *tmp_buffer = (Struct_Motor_DJI_CAN_Rx_Data *) CAN_Manage_Object->Rx_Buffer;

    // 处理大小端
    Basic_Math_Endian_Reverse_16((void *) &tmp_buffer->Encoder_Reverse, (void *) &tmp_encoder);
    Basic_Math_Endian_Reverse_16((void *) &tmp_buffer->Omega_Reverse, (void *) &tmp_omega);
    Basic_Math_Endian_Reverse_16((void *) &tmp_buffer->Current_Reverse, (void *) &tmp_current);

    // 计算圈数与总编码器值
    delta_encoder = tmp_encoder - Rx_Data.Pre_Encoder;
    if (delta_encoder < -ENCODER_NUM_PER_ROUND / 2)
    {
        // 正方向转过了一圈
        Rx_Data.Total_Round++;
    }
    else if (delta_encoder > ENCODER_NUM_PER_ROUND / 2)
    {
        // 反方向转过了一圈
        Rx_Data.Total_Round--;
    }
    Rx_Data.Total_Encoder = Rx_Data.Total_Round * ENCODER_NUM_PER_ROUND + tmp_encoder;

    // 计算电机本身信息
    Rx_Data.Now_Angle = (float) Rx_Data.Total_Encoder / (float) ENCODER_NUM_PER_ROUND * 2.0f * PI / Gearbox_Rate;
    Rx_Data.Now_Omega = (float) tmp_omega * BASIC_MATH_RPM_TO_RADPS / Gearbox_Rate;
    Rx_Data.Now_Torque = tmp_current / CURRENT_TO_OUT * CURRENT_TO_TORQUE * Gearbox_Rate;
    Rx_Data.Now_Temperature = tmp_buffer->Temperature + BASIC_MATH_CELSIUS_TO_KELVIN;

    // 存储预备信息
    Rx_Data.Pre_Encoder = tmp_encoder;
}

/**
 * @brief 计算PID
 *
 */
void Class_Motor_DJI_C610::PID_Calculate()
{
    switch (Motor_DJI_Control_Method)
    {
    case (Motor_DJI_Control_Method_TORQUE):
    {
        break;
    }
    case (Motor_DJI_Control_Method_OMEGA):
    {
        PID_Omega.Set_Target(Target_Omega + Feedforward_Omega);
        PID_Omega.Set_Now(Rx_Data.Now_Omega);
        PID_Omega.TIM_Calculate_PeriodElapsedCallback();

        Target_Torque = PID_Omega.Get_Out();

        break;
    }
    case (Motor_DJI_Control_Method_ANGLE):
    {
        PID_Angle.Set_Target(Target_Angle);
        PID_Angle.Set_Now(Rx_Data.Now_Angle);
        PID_Angle.TIM_Calculate_PeriodElapsedCallback();

        Target_Omega = PID_Angle.Get_Out();

        PID_Omega.Set_Target(Target_Omega + Feedforward_Omega);
        PID_Omega.Set_Now(Rx_Data.Now_Omega);
        PID_Omega.TIM_Calculate_PeriodElapsedCallback();

        Target_Torque = PID_Omega.Get_Out();

        break;
    }
    }
}

/**
 * @brief 电机数据输出到CAN总线发送缓冲区
 *
 */
void Class_Motor_DJI_C610::Output()
{
    Tx_Data[0] = (int16_t) Out >> 8;
    Tx_Data[1] = (int16_t) Out;
}

/**
 * @brief 电机初始化
 *
 * @param hcan CAN编号
 * @param __CAN_Rx_ID CAN ID
 * @param __Motor_DJI_Control_Method 电机控制方式, 默认速度
 * @param __Gearbox_Rate 减速箱减速比, 默认为原装减速箱, 如拆去减速箱则该值设为1
 */
void Class_Motor_DJI_C620::Init(const FDCAN_HandleTypeDef *hcan, const Enum_Motor_DJI_ID &__CAN_Rx_ID, const Enum_Motor_DJI_Control_Method &__Motor_DJI_Control_Method, const float &__Gearbox_Rate)
{
    if (hcan->Instance == FDCAN1)
    {
        CAN_Manage_Object = &CAN1_Manage_Object;
    }
    else if (hcan->Instance == FDCAN2)
    {
        CAN_Manage_Object = &CAN2_Manage_Object;
    }
    else if (hcan->Instance == FDCAN3)
    {
        CAN_Manage_Object = &CAN3_Manage_Object;
    }
    CAN_Rx_ID = __CAN_Rx_ID;
    Motor_DJI_Control_Method = __Motor_DJI_Control_Method;
    Gearbox_Rate = __Gearbox_Rate;
    Tx_Data = allocate_tx_data(hcan, __CAN_Rx_ID);
}

/**
 * @brief CAN通信接收回调函数
 *
 */
void Class_Motor_DJI_C620::CAN_RxCpltCallback()
{
    // 滑动窗口, 判断电机是否在线
    Flag += 1;

    Data_Process();
}

/**
 * @brief TIM定时器中断定期检测电机是否存活
 *
 */
void Class_Motor_DJI_C620::TIM_100ms_Alive_PeriodElapsedCallback()
{
    // 判断该时间段内是否接收过电机数据
    if (Flag == Pre_Flag)
    {
        // 电机断开连接
        Motor_DJI_Status = Motor_DJI_Status_DISABLE;
        PID_Angle.Set_Integral_Error(0.0f);
        PID_Omega.Set_Integral_Error(0.0f);
    }
    else
    {
        // 电机保持连接
        Motor_DJI_Status = Motor_DJI_Status_ENABLE;
    }
    Pre_Flag = Flag;
}

/**
 * @brief TIM定时器中断计算回调函数, 计算周期取决于电机反馈周期
 *
 */
void Class_Motor_DJI_C620::TIM_Calculate_PeriodElapsedCallback()
{
    PID_Calculate();

    Out = (Target_Torque + Feedforward_Torque) / CURRENT_TO_TORQUE / Gearbox_Rate * CURRENT_TO_OUT;
    Basic_Math_Constrain(&Out, -OUT_MAX, OUT_MAX);

    Output();

    Feedforward_Torque = 0.0f;
    Feedforward_Omega = 0.0f;
}

/**
 * @brief 数据处理过程
 *
 */
void Class_Motor_DJI_C620::Data_Process()
{
    // 数据处理过程
    int16_t delta_encoder;
    uint16_t tmp_encoder;
    int16_t tmp_omega, tmp_current;
    Struct_Motor_DJI_CAN_Rx_Data *tmp_buffer = (Struct_Motor_DJI_CAN_Rx_Data *) CAN_Manage_Object->Rx_Buffer;

    // 处理大小端
    Basic_Math_Endian_Reverse_16((void *) &tmp_buffer->Encoder_Reverse, (void *) &tmp_encoder);
    Basic_Math_Endian_Reverse_16((void *) &tmp_buffer->Omega_Reverse, (void *) &tmp_omega);
    Basic_Math_Endian_Reverse_16((void *) &tmp_buffer->Current_Reverse, (void *) &tmp_current);

    // 计算圈数与总编码器值
    delta_encoder = tmp_encoder - Rx_Data.Pre_Encoder;
    if (delta_encoder < -ENCODER_NUM_PER_ROUND / 2)
    {
        // 正方向转过了一圈
        Rx_Data.Total_Round++;
    }
    else if (delta_encoder > ENCODER_NUM_PER_ROUND / 2)
    {
        // 反方向转过了一圈
        Rx_Data.Total_Round--;
    }
    Rx_Data.Total_Encoder = Rx_Data.Total_Round * ENCODER_NUM_PER_ROUND + tmp_encoder;

    // 计算电机本身信息
    Rx_Data.Now_Angle = (float) Rx_Data.Total_Encoder / (float) ENCODER_NUM_PER_ROUND * 2.0f * PI / Gearbox_Rate;
    Rx_Data.Now_Omega = (float) tmp_omega * BASIC_MATH_RPM_TO_RADPS / Gearbox_Rate;
    Rx_Data.Now_Torque = tmp_current / CURRENT_TO_OUT * CURRENT_TO_TORQUE * Gearbox_Rate;
    Rx_Data.Now_Temperature = tmp_buffer->Temperature + BASIC_MATH_CELSIUS_TO_KELVIN;
    Rx_Data.Now_Power = power_calculate(POWER_K_0, POWER_K_1, POWER_K_2, POWER_A, Rx_Data.Now_Torque / Gearbox_Rate, Rx_Data.Now_Omega * Gearbox_Rate);

    // 存储预备信息
    Rx_Data.Pre_Encoder = tmp_encoder;
}

/**
 * @brief 计算PID
 *
 */
void Class_Motor_DJI_C620::PID_Calculate()
{
    switch (Motor_DJI_Control_Method)
    {
    case (Motor_DJI_Control_Method_TORQUE):
    {
        break;
    }
    case (Motor_DJI_Control_Method_OMEGA):
    {
        PID_Omega.Set_Target(Target_Omega + Feedforward_Omega);
        PID_Omega.Set_Now(Rx_Data.Now_Omega);
        PID_Omega.TIM_Calculate_PeriodElapsedCallback();

        Target_Torque = PID_Omega.Get_Out();

        break;
    }
    case (Motor_DJI_Control_Method_ANGLE):
    {
        PID_Angle.Set_Target(Target_Angle);
        PID_Angle.Set_Now(Rx_Data.Now_Angle);
        PID_Angle.TIM_Calculate_PeriodElapsedCallback();

        Target_Omega = PID_Angle.Get_Out();

        PID_Omega.Set_Target(Target_Omega + Feedforward_Omega);
        PID_Omega.Set_Now(Rx_Data.Now_Omega);
        PID_Omega.TIM_Calculate_PeriodElapsedCallback();

        Target_Torque = PID_Omega.Get_Out();

        break;
    }
    default:
    {
        Target_Torque = 0.0f;

        break;
    }
    }
}

/**
 * @brief 电机数据输出到CAN总线发送缓冲区
 *
 */
void Class_Motor_DJI_C620::Output()
{
    Tx_Data[0] = (int16_t) Out >> 8;
    Tx_Data[1] = (int16_t) Out;
}

/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/