/**
 * @file dvc_motor_dm.cpp
 * @author yssickjgd (1345578933@qq.com)
 * @brief 达妙电机配置与操作
 * @version 0.1
 * @date 2023-08-30 0.1 新增
 *
 * @copyright USTC-RoboWalker (c) 2023
 *
 */

/* Includes ------------------------------------------------------------------*/

#include "dvc_motor_dm.h"

/* Private macros ------------------------------------------------------------*/

/* Private types -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

// 清除电机错误信息, 传统模式有效
uint8_t DM_Motor_CAN_Message_Clear_Error[8] = {0xff,
                                               0xff,
                                               0xff,
                                               0xff,
                                               0xff,
                                               0xff,
                                               0xff,
                                               0xfb};
// 使能电机, 传统模式有效
uint8_t DM_Motor_CAN_Message_Enter[8] = {0xff,
                                         0xff,
                                         0xff,
                                         0xff,
                                         0xff,
                                         0xff,
                                         0xff,
                                         0xfc};
// 失能电机, 传统模式有效
uint8_t DM_Motor_CAN_Message_Exit[8] = {0xff,
                                        0xff,
                                        0xff,
                                        0xff,
                                        0xff,
                                        0xff,
                                        0xff,
                                        0xfd};
// 保存当前电机位置为零点, 传统模式有效
uint8_t DM_Motor_CAN_Message_Save_Zero[8] = {0xff,
                                             0xff,
                                             0xff,
                                             0xff,
                                             0xff,
                                             0xff,
                                             0xff,
                                             0xfe};

/* Private function declarations ---------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/

/**
 * @brief 分配CAN发送缓冲区, 一拖四模式有效
 *
 * @param hcan CAN编号
 * @param __CAN_ID CAN ID
 * @param __DJI_Motor_Driver_Version 大疆驱动版本, 当且仅当当前被分配电机为6020, 且是电流驱动新版本时选2023, 否则都是default
 * @return uint8_t* 缓冲区指针
 */
uint8_t *allocate_tx_data(CAN_HandleTypeDef *hcan, Enum_Motor_DM_Motor_ID_1_To_4 __CAN_Rx_ID_1_To_4)
{
    uint8_t *tmp_tx_data_ptr;
    if (hcan == &hcan1)
    {
        switch (__CAN_Rx_ID_1_To_4)
        {
        case (Motor_DM_ID_0x301):
        {
            tmp_tx_data_ptr = &(CAN1_0x3fe_Tx_Data[0]);

            break;
        }
        case (Motor_DM_ID_0x302):
        {
            tmp_tx_data_ptr = &(CAN1_0x3fe_Tx_Data[2]);

            break;
        }
        case (Motor_DM_ID_0x303):
        {
            tmp_tx_data_ptr = &(CAN1_0x3fe_Tx_Data[4]);

            break;
        }
        case (Motor_DM_ID_0x304):
        {
            tmp_tx_data_ptr = &(CAN1_0x3fe_Tx_Data[6]);

            break;
        }
        case (Motor_DM_ID_0x305):
        {
            tmp_tx_data_ptr = &(CAN1_0x4fe_Tx_Data[0]);

            break;
        }
        case (Motor_DM_ID_0x306):
        {
            tmp_tx_data_ptr = &(CAN1_0x4fe_Tx_Data[2]);

            break;
        }
        case (Motor_DM_ID_0x307):
        {
            tmp_tx_data_ptr = &(CAN1_0x4fe_Tx_Data[4]);

            break;
        }
        case (Motor_DM_ID_0x308):
        {
            tmp_tx_data_ptr = &(CAN1_0x4fe_Tx_Data[6]);

            break;
        }
        }
    }
    // else if (hcan == &hcan2)
    // {
    //     switch (__CAN_Rx_ID_1_To_4)
    //     {
    //     case (Motor_DM_ID_0x301):
    //     {
    //         tmp_tx_data_ptr = &(CAN2_0x3fe_Tx_Data[0]);

    //         break;
    //     }
    //     case (Motor_DM_ID_0x302):
    //     {
    //         tmp_tx_data_ptr = &(CAN2_0x3fe_Tx_Data[2]);

    //         break;
    //     }
    //     case (Motor_DM_ID_0x303):
    //     {
    //         tmp_tx_data_ptr = &(CAN2_0x3fe_Tx_Data[4]);

    //         break;
    //     }
    //     case (Motor_DM_ID_0x304):
    //     {
    //         tmp_tx_data_ptr = &(CAN2_0x3fe_Tx_Data[6]);

    //         break;
    //     }
    //     case (Motor_DM_ID_0x305):
    //     {
    //         tmp_tx_data_ptr = &(CAN2_0x4fe_Tx_Data[0]);

    //         break;
    //     }
    //     case (Motor_DM_ID_0x306):
    //     {
    //         tmp_tx_data_ptr = &(CAN2_0x4fe_Tx_Data[2]);

    //         break;
    //     }
    //     case (Motor_DM_ID_0x307):
    //     {
    //         tmp_tx_data_ptr = &(CAN2_0x4fe_Tx_Data[4]);

    //         break;
    //     }
    //     case (Motor_DM_ID_0x308):
    //     {
    //         tmp_tx_data_ptr = &(CAN2_0x4fe_Tx_Data[6]);

    //         break;
    //     }
    //     }
    // }
    return (tmp_tx_data_ptr);
}

/**
 * @brief 电机初始化
 *
 * @param hcan 绑定的CAN总线
 * @param __CAN_Rx_ID 收数据绑定的CAN ID, 与上位机驱动参数Master_ID保持一致, 传统模式有效
 * @param __CAN_Tx_ID 发数据绑定的CAN ID, 是上位机驱动参数CAN_ID加上控制模式的偏移量, 传统模式有效
 * @param __Motor_DM_Control_Method 电机控制方式
 * @param __Angle_Max 最大位置, 与上位机控制幅值PMAX保持一致, 传统模式有效
 * @param __Omega_Max 最大速度, 与上位机控制幅值VMAX保持一致, 传统模式有效
 * @param __Torque_Max 最大扭矩, 与上位机控制幅值TMAX保持一致, 传统模式有效
 */
void Class_Motor_DM_Normal::Init(CAN_HandleTypeDef *hcan, uint8_t __CAN_Rx_ID, uint8_t __CAN_Tx_ID, Enum_Motor_DM_Control_Method __Motor_DM_Control_Method, float __Angle_Max, float __Omega_Max, float __Torque_Max, float __Current_Max)
{
    if (hcan->Instance == CAN1)
    {
        CAN_Manage_Object = &CAN1_Manage_Object;
    }
    else if (hcan->Instance == CAN2)
    {
        CAN_Manage_Object = &CAN2_Manage_Object;
    }

    CAN_Rx_ID = __CAN_Rx_ID;
    switch (__Motor_DM_Control_Method)
    {
    case (Motor_DM_Control_Method_NORMAL_MIT):
    {
        CAN_Tx_ID = __CAN_Tx_ID;
        break;
    }
    case (Motor_DM_Control_Method_NORMAL_ANGLE_OMEGA):
    {
        CAN_Tx_ID = __CAN_Tx_ID + 0x100;
        break;
    }
    case (Motor_DM_Control_Method_NORMAL_OMEGA):
    {
        CAN_Tx_ID = __CAN_Tx_ID + 0x200;
        break;
    }
    case (Motor_DM_Control_Method_NORMAL_EMIT):
    {
        CAN_Tx_ID = __CAN_Tx_ID + 0x300;
        break;
    }
    }
    Motor_DM_Control_Method = __Motor_DM_Control_Method;
    Angle_Max = __Angle_Max;
    Omega_Max = __Omega_Max;
    Torque_Max = __Torque_Max;
    Current_Max = __Current_Max;
}

/**
 * @brief CAN通信接收回调函数
 *
 * @param Rx_Data 接收的数据
 */
void Class_Motor_DM_Normal::CAN_RxCpltCallback(uint8_t *Rx_Data)
{
    // 滑动窗口, 判断电机是否在线
    Flag += 1;

    Data_Process();
}

/**
 * @brief 发送清除错误信息
 *
 */
void Class_Motor_DM_Normal::CAN_Send_Clear_Error()
{
    CAN_Send_Data(CAN_Manage_Object->CAN_Handler, CAN_Tx_ID, DM_Motor_CAN_Message_Clear_Error, 8);
}

/**
 * @brief 发送使能电机
 *
 */
void Class_Motor_DM_Normal::CAN_Send_Enter()
{
    CAN_Send_Data(CAN_Manage_Object->CAN_Handler, CAN_Tx_ID, DM_Motor_CAN_Message_Enter, 8);
}

/**
 * @brief 发送失能电机
 *
 */
void Class_Motor_DM_Normal::CAN_Send_Exit()
{
    CAN_Send_Data(CAN_Manage_Object->CAN_Handler, CAN_Tx_ID, DM_Motor_CAN_Message_Exit, 8);
}

/**
 * @brief 发送保存当前位置为零点
 *
 */
void Class_Motor_DM_Normal::CAN_Send_Save_Zero()
{
    CAN_Send_Data(CAN_Manage_Object->CAN_Handler, CAN_Tx_ID, DM_Motor_CAN_Message_Save_Zero, 8);
}

/**
 * @brief TIM定时器中断定期检测电机是否存活, 检测周期取决于电机掉线时长
 *
 */
void Class_Motor_DM_Normal::TIM_Alive_PeriodElapsedCallback()
{
    // 判断该时间段内是否接收过电机数据
    if (Flag == Pre_Flag)
    {
        // 电机断开连接
        Motor_DM_Status = Motor_DM_Status_DISABLE;
    }
    else
    {
        // 电机保持连接
        Motor_DM_Status = Motor_DM_Status_ENABLE;
    }

    Pre_Flag = Flag;

    if(Motor_DM_Status == Motor_DM_Status_DISABLE)
    {
        CAN_Send_Enter();
    }
}

/**
 * @brief TIM定时器中断发送出去的回调函数, 计算周期取决于自主设置的控制周期
 *
 */
void Class_Motor_DM_Normal::TIM_Send_PeriodElapsedCallback()
{
    if (Rx_Data.Control_Status == Motor_DM_Status_ENABLE)
    {
        // 电机在线, 正常控制
        Math_Constrain(&Control_Angle, -Angle_Max, Angle_Max);
        Math_Constrain(&Control_Omega, -Omega_Max, Omega_Max);
        Math_Constrain(&Control_Torque, -Torque_Max, Torque_Max);
        Math_Constrain(&Control_Current, -Current_Max, Current_Max);
        Math_Constrain(&K_P, 0.0f, 500.0f);
        Math_Constrain(&K_D, 0.0f, 5.0f);

        Output();
    }
    else if (Rx_Data.Control_Status == Motor_DM_Status_DISABLE)
    {
        // 电机可能掉线, 使能电机
        CAN_Send_Enter();
    }
    else
    {
        // 电机错误, 发送清除错误帧
        CAN_Send_Clear_Error();
    }
}

/**
 * @brief 数据处理过程
 *
 */
void Class_Motor_DM_Normal::Data_Process()
{
    // 数据处理过程
    int32_t delta_encoder;
    uint16_t tmp_encoder, tmp_omega, tmp_torque;
    Struct_Motor_DM_CAN_Rx_Data_Normal *tmp_buffer = (Struct_Motor_DM_CAN_Rx_Data_Normal *)CAN_Manage_Object->Rx_Buffer.Data;

    // 电机ID不匹配, 则不进行处理
    if(tmp_buffer->CAN_ID != (CAN_Tx_ID & 0x0f))
    {
        return;
    }

    // 处理大小端
    Math_Endian_Reverse_16((void *)&tmp_buffer->Angle_Reverse, &tmp_encoder);
    tmp_omega = (tmp_buffer->Omega_11_4 << 4) | (tmp_buffer->Omega_3_0_Torque_11_8 >> 4);
    tmp_torque = ((tmp_buffer->Omega_3_0_Torque_11_8 & 0x0f) << 8) | (tmp_buffer->Torque_7_0);

    Rx_Data.Control_Status = static_cast<Enum_Motor_DM_Control_Status_Normal>(tmp_buffer->Control_Status_Enum);

    // 计算圈数与总角度值
    delta_encoder = tmp_encoder - Rx_Data.Pre_Encoder;
    if (delta_encoder < -(1 << 15))
    {
        // 正方向转过了一圈
        Rx_Data.Total_Round++;
    }
    else if (delta_encoder > (1 << 15))
    {
        // 反方向转过了一圈
        Rx_Data.Total_Round--;
    }
    Rx_Data.Total_Encoder = Rx_Data.Total_Round * (1 << 16) + tmp_encoder - ((1 << 15) - 1);

    // 计算电机本身信息
    Rx_Data.Now_Angle = (float)(Rx_Data.Total_Encoder) / (float)((1 << 16) - 1) * Angle_Max * 2.0f;
    Rx_Data.Now_Omega = Math_Int_To_Float(tmp_omega, 0x7ff, (1 << 12) - 1, 0, Omega_Max);
    Rx_Data.Now_Torque = Math_Int_To_Float(tmp_torque, 0x7ff, (1 << 12) - 1, 0, Torque_Max);
    Rx_Data.Now_MOS_Temperature = tmp_buffer->MOS_Temperature + CELSIUS_TO_KELVIN;
    Rx_Data.Now_Rotor_Temperature = tmp_buffer->Rotor_Temperature + CELSIUS_TO_KELVIN;

    // 存储预备信息
    Rx_Data.Pre_Encoder = tmp_encoder;
}

/**
 * @brief 电机数据输出到CAN总线
 *
 */
void Class_Motor_DM_Normal::Output()
{
    // 电机控制
    switch (Motor_DM_Control_Method)
    {
    case (Motor_DM_Control_Method_NORMAL_MIT):
    {
        Struct_Motor_DM_CAN_Tx_Data_Normal_MIT *tmp_buffer = (Struct_Motor_DM_CAN_Tx_Data_Normal_MIT *)Tx_Data;

        uint16_t tmp_angle, tmp_omega, tmp_torque, tmp_k_p, tmp_k_d;

        tmp_angle = Math_Float_To_Int(Control_Angle, 0, Angle_Max, 0x7fff, (1 << 16) - 1);
        tmp_omega = Math_Float_To_Int(Control_Omega, 0, Omega_Max, 0x7ff, (1 << 12) - 1);
        tmp_torque = Math_Float_To_Int(Control_Torque, 0, Torque_Max, 0x7ff, (1 << 12) - 1);
        tmp_k_p = Math_Float_To_Int(K_P, 0, 500.0f, 0, (1 << 12) - 1);
        tmp_k_d = Math_Float_To_Int(K_D, 0, 5.0f, 0, (1 << 12) - 1);

        tmp_buffer->Control_Angle_Reverse = Math_Endian_Reverse_16(&tmp_angle, nullptr);
        tmp_buffer->Control_Omega_11_4 = tmp_omega >> 4;
        tmp_buffer->Control_Omega_3_0_K_P_11_8 = ((tmp_omega & 0x0f) << 4) | (tmp_k_p >> 8);
        tmp_buffer->K_P_7_0 = tmp_k_p & 0xff;
        tmp_buffer->K_D_11_4 = tmp_k_d >> 4;
        tmp_buffer->K_D_3_0_Control_Torque_11_8 = ((tmp_k_d & 0x0f) << 4) | (tmp_torque >> 8);
        tmp_buffer->Control_Torque_7_0 = tmp_torque & 0xff;

        CAN_Send_Data(CAN_Manage_Object->CAN_Handler, CAN_Tx_ID, Tx_Data, 8);

        break;
    }
    case (Motor_DM_Control_Method_NORMAL_ANGLE_OMEGA):
    {
        Struct_Motor_DM_CAN_Tx_Data_Normal_Angle_Omega *tmp_buffer = (Struct_Motor_DM_CAN_Tx_Data_Normal_Angle_Omega *)Tx_Data;

        tmp_buffer->Control_Angle = Control_Angle;
        tmp_buffer->Control_Omega = Control_Omega;

        CAN_Send_Data(CAN_Manage_Object->CAN_Handler, CAN_Tx_ID, Tx_Data, 8);

        break;
    }
    case (Motor_DM_Control_Method_NORMAL_OMEGA):
    {
        Struct_Motor_DM_CAN_Tx_Data_Normal_Omega *tmp_buffer = (Struct_Motor_DM_CAN_Tx_Data_Normal_Omega *)Tx_Data;

        tmp_buffer->Control_Omega = Control_Omega;

        CAN_Send_Data(CAN_Manage_Object->CAN_Handler, CAN_Tx_ID, Tx_Data, 4);

        break;
    }
    case (Motor_DM_Control_Method_NORMAL_EMIT):
    {
        Struct_Motor_DM_CAN_Tx_Data_Normal_EMIT *tmp_buffer = (Struct_Motor_DM_CAN_Tx_Data_Normal_EMIT *)Tx_Data;

        tmp_buffer->Control_Angle = Control_Angle;
        tmp_buffer->Control_Omega = (uint16_t)(Control_Omega * 100.0f);
        tmp_buffer->Control_Current = (uint16_t)(Control_Current / Current_Max * 10000.0f);

        CAN_Send_Data(CAN_Manage_Object->CAN_Handler, CAN_Tx_ID, Tx_Data, 8);

        break;
    }
    }
}

/**
 * @brief 电机初始化
 * 
 * @param hcan 绑定的CAN总线
 * @param __CAN_Rx_ID 绑定的CAN ID
 * @param __Motor_DM_Control_Method 电机控制方式, 默认角度
 * @param __Encoder_Offset 编码器偏移, 默认0
 * @param __Current_Max 最大电流
 */
void Class_Motor_DM_1_To_4::Init(CAN_HandleTypeDef *hcan, Enum_Motor_DM_Motor_ID_1_To_4 __CAN_Rx_ID, Enum_Motor_DM_Control_Method __Motor_DM_Control_Method, int32_t __Encoder_Offset, float __Current_Max)
{
    if (hcan->Instance == CAN1)
    {
        CAN_Manage_Object = &CAN1_Manage_Object;
    }
    else if (hcan->Instance == CAN2)
    {
        CAN_Manage_Object = &CAN2_Manage_Object;
    }
    CAN_Rx_ID = __CAN_Rx_ID;
    Motor_DM_Control_Method = __Motor_DM_Control_Method;
    Encoder_Offset = __Encoder_Offset;
    Current_Max = __Current_Max;
    Tx_Data = allocate_tx_data(hcan, __CAN_Rx_ID);
}

/**
 * @brief CAN通信接收回调函数
 *
 * @param Rx_Data 接收的数据
 */
void Class_Motor_DM_1_To_4::CAN_RxCpltCallback(uint8_t *Rx_Data)
{
    // 滑动窗口, 判断电机是否在线
    Flag += 1;

    Data_Process();
}

/**
 * @brief TIM定时器中断定期检测电机是否存活
 *
 */
void Class_Motor_DM_1_To_4::TIM_100ms_Alive_PeriodElapsedCallback()
{
    // 判断该时间段内是否接收过电机数据
    if (Flag == Pre_Flag)
    {
        // 电机断开连接
        Motor_DM_Status = Motor_DM_Status_DISABLE;
        PID_Angle.Set_Integral_Error(0.0f);
        PID_Omega.Set_Integral_Error(0.0f);
    }
    else
    {
        // 电机保持连接
        Motor_DM_Status = Motor_DM_Status_ENABLE;
    }
    Pre_Flag = Flag;
}

/**
 * @brief TIM定时器中断计算回调函数, 计算周期取决于电机反馈周期
 *
 */
void Class_Motor_DM_1_To_4::TIM_1ms_Calculate_PeriodElapsedCallback()
{
    PID_Calculate();

    float tmp_value = Target_Current + Feedforward_Current;
    Math_Constrain(&tmp_value, -Current_Max, Current_Max);
    Out = tmp_value * Current_To_Out;

    Output();

    Feedforward_Current = 0.0f;
    Feedforward_Omega = 0.0f;
}

/**
 * @brief 数据处理过程
 *
 */
void Class_Motor_DM_1_To_4::Data_Process()
{
    // 数据处理过程
    int16_t delta_encoder;
    uint16_t tmp_encoder;
    int16_t tmp_omega, tmp_current;
    Struct_Motor_DM_CAN_Rx_Data_1_To_4 *tmp_buffer = (Struct_Motor_DM_CAN_Rx_Data_1_To_4 *) CAN_Manage_Object->Rx_Buffer.Data;

    // 处理大小端
    Math_Endian_Reverse_16((void *) &tmp_buffer->Encoder_Reverse, (void *) &tmp_encoder);
    Math_Endian_Reverse_16((void *) &tmp_buffer->Omega_Reverse, (void *) &tmp_omega);
    Math_Endian_Reverse_16((void *) &tmp_buffer->Current_Reverse, (void *) &tmp_current);

    // 计算圈数与总编码器值
    delta_encoder = tmp_encoder - Rx_Data.Pre_Encoder;
    if (delta_encoder < -Encoder_Num_Per_Round / 2)
    {
        // 正方向转过了一圈
        Rx_Data.Total_Round++;
    }
    else if (delta_encoder > Encoder_Num_Per_Round / 2)
    {
        // 反方向转过了一圈
        Rx_Data.Total_Round--;
    }
    Rx_Data.Total_Encoder = Rx_Data.Total_Round * Encoder_Num_Per_Round + tmp_encoder + Encoder_Offset;

    // 计算电机本身信息
    Rx_Data.Now_Angle = (float) Rx_Data.Total_Encoder / (float) Encoder_Num_Per_Round * 2.0f * PI;
    Rx_Data.Now_Omega = tmp_omega / 100.0f * RPM_TO_RADPS;
    Rx_Data.Now_Current = tmp_current / 1000.0f;
    Rx_Data.Now_MOS_Temperature = tmp_buffer->MOS_Temperature + CELSIUS_TO_KELVIN;
    Rx_Data.Now_Rotor_Temperature = tmp_buffer->Rotor_Temperature + CELSIUS_TO_KELVIN;

    // 存储预备信息
    Rx_Data.Pre_Encoder = tmp_encoder;
}

/**
 * @brief 计算PID
 *
 */
void Class_Motor_DM_1_To_4::PID_Calculate()
{
    switch (Motor_DM_Control_Method)
    {
    case (Motor_DM_Control_Method_1_TO_4_CURRENT):
    {
        break;
    }
    case (Motor_DM_Control_Method_1_TO_4_OMEGA):
    {
        PID_Omega.Set_Target(Target_Omega + Feedforward_Omega);
        PID_Omega.Set_Now(Rx_Data.Now_Omega);
        PID_Omega.TIM_Adjust_PeriodElapsedCallback();

        Target_Current = PID_Omega.Get_Out();

        break;
    }
    case (Motor_DM_Control_Method_1_TO_4_ANGLE):
    {
        PID_Angle.Set_Target(Target_Angle);
        PID_Angle.Set_Now(Rx_Data.Now_Angle);
        PID_Angle.TIM_Adjust_PeriodElapsedCallback();

        Target_Omega = PID_Angle.Get_Out();

        PID_Omega.Set_Target(Target_Omega + Feedforward_Omega);
        PID_Omega.Set_Now(Rx_Data.Now_Omega);
        PID_Omega.TIM_Adjust_PeriodElapsedCallback();

        Target_Current = PID_Omega.Get_Out();

        break;
    }
    default:
    {
        Target_Current = 0.0f;

        break;
    }
    }
}

/**
 * @brief 电机数据输出到CAN总线发送缓冲区
 *
 */
void Class_Motor_DM_1_To_4::Output()
{
    *(int16_t *) Tx_Data = (int16_t)(Out);
}

/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/
