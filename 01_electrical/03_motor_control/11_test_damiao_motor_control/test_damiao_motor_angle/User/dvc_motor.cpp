/**
 * @file dvc_motor.cpp
 * @author yssickjgd (yssickjgd@mail.ustc.edu.cn)
 * @brief CAN电机配置与操作
 * @version 0.1
 * @date 2022-08-03
 *
 * @copyright USTC-RoboWalker (c) 2022
 *
 */

/* Includes ------------------------------------------------------------------*/

#include "dvc_motor.h"

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
 * @return uint8_t* 缓冲区指针
 */
uint8_t *allocate_tx_data(CAN_HandleTypeDef *hcan, Enum_CAN_Motor_ID __CAN_ID)
{
    uint8_t *tmp_tx_data_ptr;
    if (hcan == &hcan1)
    {
        switch (__CAN_ID)
        {
        case (CAN_Motor_ID_0x201):
        {
            tmp_tx_data_ptr = &(CAN1_0x200_Tx_Data[0]);
        }
        break;
        case (CAN_Motor_ID_0x202):
        {
            tmp_tx_data_ptr = &(CAN1_0x200_Tx_Data[2]);
        }
        break;
        case (CAN_Motor_ID_0x203):
        {
            tmp_tx_data_ptr = &(CAN1_0x200_Tx_Data[4]);
        }
        break;
        case (CAN_Motor_ID_0x204):
        {
            tmp_tx_data_ptr = &(CAN1_0x200_Tx_Data[6]);
        }
        break;
        case (CAN_Motor_ID_0x205):
        {
            tmp_tx_data_ptr = &(CAN1_0x1ff_Tx_Data[0]);
        }
        break;
        case (CAN_Motor_ID_0x206):
        {
            tmp_tx_data_ptr = &(CAN1_0x1ff_Tx_Data[2]);
        }
        break;
        case (CAN_Motor_ID_0x207):
        {
            tmp_tx_data_ptr = &(CAN1_0x1ff_Tx_Data[4]);
        }
        break;
        case (CAN_Motor_ID_0x208):
        {
            tmp_tx_data_ptr = &(CAN1_0x1ff_Tx_Data[6]);
        }
        break;
        case (CAN_Motor_ID_0x209):
        {
            tmp_tx_data_ptr = &(CAN1_0x2ff_Tx_Data[0]);
        }
        break;
        case (CAN_Motor_ID_0x20A):
        {
            tmp_tx_data_ptr = &(CAN1_0x2ff_Tx_Data[2]);
        }
        break;
        case (CAN_Motor_ID_0x20B):
        {
            tmp_tx_data_ptr = &(CAN1_0x2ff_Tx_Data[4]);
        }
        break;
        }
    }
    // else if (hcan == &hcan2)
    // {
    //     switch (__CAN_ID)
    //     {
    //     case (CAN_Motor_ID_0x201):
    //     {
    //         tmp_tx_data_ptr = &(CAN2_0x200_Tx_Data[0]);
    //     }
    //     break;
    //     case (CAN_Motor_ID_0x202):
    //     {
    //         tmp_tx_data_ptr = &(CAN2_0x200_Tx_Data[2]);
    //     }
    //     break;
    //     case (CAN_Motor_ID_0x203):
    //     {
    //         tmp_tx_data_ptr = &(CAN2_0x200_Tx_Data[4]);
    //     }
    //     break;
    //     case (CAN_Motor_ID_0x204):
    //     {
    //         tmp_tx_data_ptr = &(CAN2_0x200_Tx_Data[6]);
    //     }
    //     break;
    //     case (CAN_Motor_ID_0x205):
    //     {
    //         tmp_tx_data_ptr = &(CAN2_0x1ff_Tx_Data[0]);
    //     }
    //     break;
    //     case (CAN_Motor_ID_0x206):
    //     {
    //         tmp_tx_data_ptr = &(CAN2_0x1ff_Tx_Data[2]);
    //     }
    //     break;
    //     case (CAN_Motor_ID_0x207):
    //     {
    //         tmp_tx_data_ptr = &(CAN2_0x1ff_Tx_Data[4]);
    //     }
    //     break;
    //     case (CAN_Motor_ID_0x208):
    //     {
    //         tmp_tx_data_ptr = &(CAN2_0x1ff_Tx_Data[6]);
    //     }
    //     break;
    //     case (CAN_Motor_ID_0x209):
    //     {
    //         tmp_tx_data_ptr = &(CAN2_0x2ff_Tx_Data[0]);
    //     }
    //     break;
    //     case (CAN_Motor_ID_0x20A):
    //     {
    //         tmp_tx_data_ptr = &(CAN2_0x2ff_Tx_Data[2]);
    //     }
    //     break;
    //     case (CAN_Motor_ID_0x20B):
    //     {
    //         tmp_tx_data_ptr = &(CAN2_0x2ff_Tx_Data[4]);
    //     }
    //     break;
    //     }
    // }
    return (tmp_tx_data_ptr);
}

/**
 * @brief 电机初始化
 *
 * @param hcan 绑定的CAN总线
 * @param __CAN_ID 绑定的CAN ID
 * @param __Control_Method 电机控制方式, 默认角度
 * @param __Encoder_Offset 编码器偏移, 默认0
 * @param __Omega_Max 最大速度, 需根据不同负载测量后赋值, 也就开环输出用得到, 不过我感觉应该没有奇葩喜欢开环输出这玩意
 */
void Class_Motor_GM6020::Init(CAN_HandleTypeDef *hcan, Enum_CAN_Motor_ID __CAN_ID, Enum_Control_Method __Control_Method, int32_t __Encoder_Offset, float __Omega_Max)
{
    if (hcan->Instance == CAN1)
    {
        CAN_Manage_Object = &CAN1_Manage_Object;
    }
    else if (hcan->Instance == CAN2)
    {
        CAN_Manage_Object = &CAN2_Manage_Object;
    }
    CAN_ID = __CAN_ID;
    Control_Method = __Control_Method;
    Encoder_Offset = __Encoder_Offset;
    Omega_Max = __Omega_Max;
    CAN_Tx_Data = allocate_tx_data(hcan, __CAN_ID);
}

/**
 * @brief 电机数据输出到CAN总线发送缓冲区
 *
 */
void Class_Motor_GM6020::Output()
{
    CAN_Tx_Data[0] = (int16_t)Out >> 8;
    CAN_Tx_Data[1] = (int16_t)Out;
}

/**
 * @brief 获取最大输出电压
 *
 * @return uint16_t 最大输出电压
 */
uint16_t Class_Motor_GM6020::Get_Output_Max()
{
    return (Output_Max);
}

/**
 * @brief 获取电机状态
 *
 * @return Enum_CAN_Motor_Status 电机状态
 */
Enum_CAN_Motor_Status Class_Motor_GM6020::Get_CAN_Motor_Status()
{
    return (CAN_Motor_Status);
}

/**
 * @brief 获取当前的角度, rad
 *
 * @return float 当前的角度, rad
 */
float Class_Motor_GM6020::Get_Now_Angle()
{
    return (Now_Angle);
}

/**
 * @brief 获取当前的速度, rad/s
 *
 * @return float 当前的速度, rad/s
 */
float Class_Motor_GM6020::Get_Now_Omega()
{
    return (Now_Omega);
}

/**
 * @brief 获取当前的扭矩, 直接采用反馈值
 *
 * @return 当前的扭矩, 直接采用反馈值
 */
float Class_Motor_GM6020::Get_Now_Torque()
{
    return (Now_Torque);
}

/**
 * @brief 获取当前的温度, 摄氏度
 *
 * @return uint8_t 当前的温度, 摄氏度
 */
uint8_t Class_Motor_GM6020::Get_Now_Temperature()
{
    return (Now_Temperature);
}

/**
 * @brief 获取电机控制方式
 *
 * @return Enum_Control_Method 电机控制方式
 */
Enum_Control_Method Class_Motor_GM6020::Get_Control_Method()
{
    return (Control_Method);
}

/**
 * @brief 获取目标的角度, rad
 *
 * @return float 目标的角度, rad
 */
float Class_Motor_GM6020::Get_Target_Angle()
{
    return (Target_Angle);
}

/**
 * @brief 获取目标的速度, rad/s
 *
 * @return float 目标的速度, rad/s
 */
float Class_Motor_GM6020::Get_Target_Omega()
{
    return (Target_Omega);
}

/**
 * @brief 获取目标的扭矩, 直接采用反馈值
 *
 * @return float 目标的扭矩, 直接采用反馈值
 */
float Class_Motor_GM6020::Get_Target_Torque()
{
    return (Target_Torque);
}

/**
 * @brief 获取输出量
 *
 * @return float 输出量
 */
float Class_Motor_GM6020::Get_Out()
{
    return (Out);
}

/**
 * @brief 设定电机控制方式
 *
 * @param __Control_Method 电机控制方式
 */
void Class_Motor_GM6020::Set_Control_Method(Enum_Control_Method __Control_Method)
{
    Control_Method = __Control_Method;
}

/**
 * @brief 设定目标的角度, rad
 *
 * @param __Target_Angle 目标的角度, rad
 */
void Class_Motor_GM6020::Set_Target_Angle(float __Target_Angle)
{
    Target_Angle = __Target_Angle;
}

/**
 * @brief 设定目标的速度, rad/s
 *
 * @param __Target_Omega 目标的速度, rad/s
 */
void Class_Motor_GM6020::Set_Target_Omega(float __Target_Omega)
{
    Target_Omega = __Target_Omega;
}

/**
 * @brief 设定目标的扭矩, 直接采用反馈值
 *
 * @param __Target_Torque 目标的扭矩, 直接采用反馈值
 */
void Class_Motor_GM6020::Set_Target_Torque(float __Target_Torque)
{
    Target_Torque = __Target_Torque;
}

/**
 * @brief 设定输出量
 *
 * @param __Output_Voltage 输出量
 */
void Class_Motor_GM6020::Set_Out(float __Out)
{
    Out = __Out;
}

/**
 * @brief CAN通信接收回调函数
 *
 * @param Rx_Data 接收的数据
 */
void Class_Motor_GM6020::CAN_RxCpltCallback(uint8_t *Rx_Data)
{
    int16_t delta_encoder;

    Flag += 1;

    Pre_Encoder = Rx_Encoder;

    Rx_Encoder = (Rx_Data[0] << 8) | Rx_Data[1];
    Rx_Omega = (Rx_Data[2] << 8) | Rx_Data[3];
    Rx_Torque = (Rx_Data[4] << 8) | Rx_Data[5];
    Rx_Temperature = Rx_Data[6];

    delta_encoder = Rx_Encoder - Pre_Encoder;
    if (delta_encoder < -4096)
    {
        //正方向转过了一圈
        Total_Round++;
    }
    else if (delta_encoder > 4096)
    {
        //反方向转过了一圈
        Total_Round--;
    }
    Total_Encoder = Total_Round * Encoder_Num_Per_Round + Rx_Encoder + Encoder_Offset;

    Now_Angle = (float)Total_Encoder / (float)Encoder_Num_Per_Round * 2.0f * PI;
    Now_Omega = (float)Rx_Omega * RPM_TO_RADPS;
    Now_Torque = Rx_Torque;
    Now_Temperature = Rx_Temperature;
}

/**
 * @brief TIM定时器中断定期检测电机是否存活
 *
 */
void Class_Motor_GM6020::TIM_Alive_PeriodElapsedCallback()
{
    //判断该时间段内是否接收过电机数据
    if (Flag == Pre_Flag)
    {
        //电机断开连接
        CAN_Motor_Status = CAN_Motor_Status_DISABLE;
        PID_Angle.Set_Integral_Error(0.0f);
        PID_Omega.Set_Integral_Error(0.0f);
        PID_Torque.Set_Integral_Error(0.0f);
    }
    else
    {
        //电机保持连接
        CAN_Motor_Status = CAN_Motor_Status_ENABLE;
    }
    Pre_Flag = Flag;
}

/**
 * @brief TIM定时器中断计算回调函数
 *
 */
void Class_Motor_GM6020::TIM_PID_PeriodElapsedCallback()
{
    switch (Control_Method)
    {
    case (Control_Method_OPENLOOP):
    {
        //默认开环速度控制
        Set_Out(Target_Omega / Omega_Max * Output_Max);
    }
    break;
    case (Control_Method_TORQUE):
    {
        PID_Torque.Set_Target(Target_Torque);
        PID_Torque.Set_Now(Now_Torque);
        PID_Torque.TIM_Adjust_PeriodElapsedCallback();

        Set_Out(PID_Torque.Get_Out());
    }
    break;
    case (Control_Method_OMEGA):
    {
        PID_Omega.Set_Target(Target_Omega);
        PID_Omega.Set_Now(Now_Omega);
        PID_Omega.TIM_Adjust_PeriodElapsedCallback();

        Target_Torque = PID_Omega.Get_Out();

        PID_Torque.Set_Target(Target_Torque);
        PID_Torque.Set_Now(Now_Torque);
        PID_Torque.TIM_Adjust_PeriodElapsedCallback();

        Set_Out(PID_Torque.Get_Out());
    }
    break;
    case (Control_Method_ANGLE):
    {
        PID_Angle.Set_Target(Target_Angle);
        PID_Angle.Set_Now(Now_Angle);
        PID_Angle.TIM_Adjust_PeriodElapsedCallback();

        Target_Omega = PID_Angle.Get_Out();

        PID_Omega.Set_Target(Target_Omega);
        PID_Omega.Set_Now(Now_Omega);
        PID_Omega.TIM_Adjust_PeriodElapsedCallback();

        Target_Torque = PID_Omega.Get_Out();

        PID_Torque.Set_Target(Target_Torque);
        PID_Torque.Set_Now(Now_Torque);
        PID_Torque.TIM_Adjust_PeriodElapsedCallback();

        Set_Out(PID_Torque.Get_Out());
    }
    break;
    default:
    {
        Set_Out(0.0f);
    }
    break;
    }
    Output();
}

/**
 * @brief 电机初始化
 *
 * @param hcan CAN编号
 * @param __CAN_ID CAN ID
 * @param __Control_Method 电机控制方式, 默认角度
 * @param __Gearbox_Rate 减速箱减速比, 默认为原装减速箱, 如拆去减速箱则该值设为1
 * @param __Torque_Max 最大扭矩, 需根据不同负载测量后赋值, 也就开环和扭矩环输出用得到, 不过我感觉应该没有奇葩喜欢开环输出这玩意
 */
void Class_Motor_C610::Init(CAN_HandleTypeDef *hcan, Enum_CAN_Motor_ID __CAN_ID, Enum_Control_Method __Control_Method, float __Gearbox_Rate, float __Torque_Max)
{
    if (hcan->Instance == CAN1)
    {
        CAN_Manage_Object = &CAN1_Manage_Object;
    }
    else if (hcan->Instance == CAN2)
    {
        CAN_Manage_Object = &CAN2_Manage_Object;
    }
    CAN_ID = __CAN_ID;
    Control_Method = __Control_Method;
    Gearbox_Rate = __Gearbox_Rate;
    Torque_Max = __Torque_Max;
    CAN_Tx_Data = allocate_tx_data(hcan, __CAN_ID);
}

/**
 * @brief 电机数据输出到CAN总线发送缓冲区
 *
 */
void Class_Motor_C610::Output()
{
    CAN_Tx_Data[0] = (int16_t)Out >> 8;
    CAN_Tx_Data[1] = (int16_t)Out;
}

/**
 * @brief 获取最大输出电流
 *
 * @return uint16_t 最大输出电流
 */
uint16_t Class_Motor_C610::Get_Output_Max()
{
    return (Output_Max);
}

/**
 * @brief 获取电机状态
 *
 * @return Enum_CAN_Motor_Status 电机状态
 */
Enum_CAN_Motor_Status Class_Motor_C610::Get_CAN_Motor_Status()
{
    return (CAN_Motor_Status);
}

/**
 * @brief 获取当前的角度, rad
 *
 * @return float 当前的角度, rad
 */
float Class_Motor_C610::Get_Now_Angle()
{
    return (Now_Angle);
}

/**
 * @brief 获取当前的速度, rad/s
 *
 * @return float 当前的速度, rad/s
 */
float Class_Motor_C610::Get_Now_Omega()
{
    return (Now_Omega);
}

/**
 * @brief 获取当前的扭矩, 直接采用反馈值
 *
 * @return 当前的扭矩, 直接采用反馈值
 */
float Class_Motor_C610::Get_Now_Torque()
{
    return (Now_Torque);
}

/**
 * @brief 获取当前的温度, 摄氏度
 *
 * @return uint8_t 当前的温度, 摄氏度
 */
uint8_t Class_Motor_C610::Get_Now_Temperature()
{
    return (Now_Temperature);
}

/**
 * @brief 获取电机控制方式
 *
 * @return Enum_Control_Method 电机控制方式
 */
Enum_Control_Method Class_Motor_C610::Get_Control_Method()
{
    return (Control_Method);
}

/**
 * @brief 获取目标的角度, rad
 *
 * @return float 目标的角度, rad
 */
float Class_Motor_C610::Get_Target_Angle()
{
    return (Target_Angle);
}

/**
 * @brief 获取目标的速度, rad/s
 *
 * @return float 目标的速度, rad/s
 */
float Class_Motor_C610::Get_Target_Omega()
{
    return (Target_Omega);
}

/**
 * @brief 获取目标的扭矩, 直接采用反馈值
 *
 * @return float 目标的扭矩, 直接采用反馈值
 */
float Class_Motor_C610::Get_Target_Torque()
{
    return (Target_Torque);
}

/**
 * @brief 获取输出量
 *
 * @return float 输出量
 */
float Class_Motor_C610::Get_Out()
{
    return (Out);
}

/**
 * @brief 设定电机控制方式
 *
 * @param __Control_Method 电机控制方式
 */
void Class_Motor_C610::Set_Control_Method(Enum_Control_Method __Control_Method)
{
    Control_Method = __Control_Method;
}

/**
 * @brief 设定目标的角度, rad
 *
 * @param __Target_Angle 目标的角度, rad
 */
void Class_Motor_C610::Set_Target_Angle(float __Target_Angle)
{
    Target_Angle = __Target_Angle;
}

/**
 * @brief 设定目标的速度, rad/s
 *
 * @param __Target_Omega 目标的速度, rad/s
 */
void Class_Motor_C610::Set_Target_Omega(float __Target_Omega)
{
    Target_Omega = __Target_Omega;
}

/**
 * @brief 设定目标的扭矩, 直接采用反馈值
 *
 * @param __Target_Torque 目标的扭矩, 直接采用反馈值
 */
void Class_Motor_C610::Set_Target_Torque(float __Target_Torque)
{
    Target_Torque = __Target_Torque;
}

/**
 * @brief 设定输出量
 *
 * @param __Output_Voltage 输出量
 */
void Class_Motor_C610::Set_Out(float __Out)
{
    Out = __Out;
}

/**
 * @brief CAN通信接收回调函数
 *
 * @param Rx_Data 接收的数据
 */
void Class_Motor_C610::CAN_RxCpltCallback(uint8_t *Rx_Data)
{
    int16_t delta_encoder;

    Flag += 1;

    Pre_Encoder = Rx_Encoder;

    Rx_Encoder = (Rx_Data[0] << 8) | Rx_Data[1];
    Rx_Omega = (Rx_Data[2] << 8) | Rx_Data[3];
    Rx_Torque = (Rx_Data[4] << 8) | Rx_Data[5];
    Rx_Temperature = Rx_Data[6];

    delta_encoder = Rx_Encoder - Pre_Encoder;
    if (delta_encoder < -4096)
    {
        //正方向转过了一圈
        Total_Round++;
    }
    else if (delta_encoder > 4096)
    {
        //反方向转过了一圈
        Total_Round--;
    }
    Total_Encoder = Total_Round * Encoder_Num_Per_Round + Rx_Encoder;

    Now_Angle = (float)Total_Encoder / (float)Encoder_Num_Per_Round * 2.0f * PI / Gearbox_Rate;
    Now_Omega = (float)Rx_Omega * RPM_TO_RADPS / Gearbox_Rate;
    Now_Torque = Rx_Torque;
    Now_Temperature = Rx_Temperature;
}

/**
 * @brief TIM定时器中断定期检测电机是否存活
 *
 */
void Class_Motor_C610::TIM_Alive_PeriodElapsedCallback()
{
    //判断该时间段内是否接收过电机数据
    if (Flag == Pre_Flag)
    {
        //电机断开连接
        CAN_Motor_Status = CAN_Motor_Status_DISABLE;
        PID_Angle.Set_Integral_Error(0.0f);
        PID_Omega.Set_Integral_Error(0.0f);
    }
    else
    {
        //电机保持连接
        CAN_Motor_Status = CAN_Motor_Status_ENABLE;
    }
    Pre_Flag = Flag;
}

/**
 * @brief TIM定时器中断计算回调函数
 *
 */
void Class_Motor_C610::TIM_PID_PeriodElapsedCallback()
{
    switch (Control_Method)
    {
    case (Control_Method_OPENLOOP):
    {
        //默认开环扭矩控制
        Set_Out(Target_Torque / Torque_Max * Output_Max);
    }
    break;
    case (Control_Method_TORQUE):
    {
        //默认闭环扭矩控制
        Set_Out(Target_Torque / Torque_Max * Output_Max);
    }
    break;
    case (Control_Method_OMEGA):
    {
        PID_Omega.Set_Target(Target_Omega);
        PID_Omega.Set_Now(Now_Omega);
        PID_Omega.TIM_Adjust_PeriodElapsedCallback();

        Set_Out(PID_Omega.Get_Out());
    }
    break;
    case (Control_Method_ANGLE):
    {
        PID_Angle.Set_Target(Target_Angle);
        PID_Angle.Set_Now(Now_Angle);
        PID_Angle.TIM_Adjust_PeriodElapsedCallback();

        Target_Omega = PID_Angle.Get_Out();

        PID_Omega.Set_Target(Target_Omega);
        PID_Omega.Set_Now(Now_Omega);
        PID_Omega.TIM_Adjust_PeriodElapsedCallback();

        Set_Out(PID_Omega.Get_Out());
    }
    break;
    default:
    {
        Set_Out(0.0f);
    }
    break;
    }
    Output();
}

/**
 * @brief 电机初始化
 *
 * @param hcan CAN编号
 * @param __CAN_ID CAN ID
 * @param __Control_Method 电机控制方式, 默认速度
 * @param __Gearbox_Rate 减速箱减速比, 默认为原装减速箱, 如拆去减速箱则该值设为1
 * @param __Torque_Max 最大扭矩, 需根据不同负载测量后赋值, 也就开环和扭矩环输出用得到, 不过我感觉应该没有奇葩喜欢开环输出这玩意
 */
void Class_Motor_C620::Init(CAN_HandleTypeDef *hcan, Enum_CAN_Motor_ID __CAN_ID, Enum_Control_Method __Control_Method, float __Gearbox_Rate, float __Torque_Max)
{
    if (hcan->Instance == CAN1)
    {
        CAN_Manage_Object = &CAN1_Manage_Object;
    }
    else if (hcan->Instance == CAN2)
    {
        CAN_Manage_Object = &CAN2_Manage_Object;
    }
    CAN_ID = __CAN_ID;
    Control_Method = __Control_Method;
    Gearbox_Rate = __Gearbox_Rate;
    Torque_Max = __Torque_Max;
    CAN_Tx_Data = allocate_tx_data(hcan, __CAN_ID);
}

/**
 * @brief 电机数据输出到CAN总线发送缓冲区
 *
 */
void Class_Motor_C620::Output()
{
    CAN_Tx_Data[0] = (int16_t)Out >> 8;
    CAN_Tx_Data[1] = (int16_t)Out;
}

/**
 * @brief 获取最大输出电流
 *
 * @return uint16_t 最大输出电流
 */
uint16_t Class_Motor_C620::Get_Output_Max()
{
    return (Output_Max);
}

/**
 * @brief 获取电机状态
 *
 * @return Enum_CAN_Motor_Status 电机状态
 */
Enum_CAN_Motor_Status Class_Motor_C620::Get_CAN_Motor_Status()
{
    return (CAN_Motor_Status);
}

/**
 * @brief 获取当前的角度, rad
 *
 * @return float 当前的角度, rad
 */
float Class_Motor_C620::Get_Now_Angle()
{
    return (Now_Angle);
}

/**
 * @brief 获取当前的速度, rad/s
 *
 * @return float 当前的速度, rad/s
 */
float Class_Motor_C620::Get_Now_Omega()
{
    return (Now_Omega);
}

/**
 * @brief 获取当前的扭矩, 直接采用反馈值
 *
 * @return 当前的扭矩, 直接采用反馈值
 */
float Class_Motor_C620::Get_Now_Torque()
{
    return (Now_Torque);
}

/**
 * @brief 获取当前的温度, 摄氏度
 *
 * @return uint8_t 当前的温度, 摄氏度
 */
uint8_t Class_Motor_C620::Get_Now_Temperature()
{
    return (Now_Temperature);
}

/**
 * @brief 获取电机控制方式
 *
 * @return Enum_Control_Method 电机控制方式
 */
Enum_Control_Method Class_Motor_C620::Get_Control_Method()
{
    return (Control_Method);
}

/**
 * @brief 获取目标的角度, rad
 *
 * @return float 目标的角度, rad
 */
float Class_Motor_C620::Get_Target_Angle()
{
    return (Target_Angle);
}

/**
 * @brief 获取目标的速度, rad/s
 *
 * @return float 目标的速度, rad/s
 */
float Class_Motor_C620::Get_Target_Omega()
{
    return (Target_Omega);
}

/**
 * @brief 获取目标的扭矩, 直接采用反馈值
 *
 * @return float 目标的扭矩, 直接采用反馈值
 */
float Class_Motor_C620::Get_Target_Torque()
{
    return (Target_Torque);
}

/**
 * @brief 获取输出量
 *
 * @return float 输出量
 */
float Class_Motor_C620::Get_Out()
{
    return (Out);
}

/**
 * @brief 设定电机控制方式
 *
 * @param __Control_Method 电机控制方式
 */
void Class_Motor_C620::Set_Control_Method(Enum_Control_Method __Control_Method)
{
    Control_Method = __Control_Method;
}

/**
 * @brief 设定目标的角度, rad
 *
 * @param __Target_Angle 目标的角度, rad
 */
void Class_Motor_C620::Set_Target_Angle(float __Target_Angle)
{
    Target_Angle = __Target_Angle;
}

/**
 * @brief 设定目标的速度, rad/s
 *
 * @param __Target_Omega 目标的速度, rad/s
 */
void Class_Motor_C620::Set_Target_Omega(float __Target_Omega)
{
    Target_Omega = __Target_Omega;
}

/**
 * @brief 设定目标的扭矩, 直接采用反馈值
 *
 * @param __Target_Torque 目标的扭矩, 直接采用反馈值
 */
void Class_Motor_C620::Set_Target_Torque(float __Target_Torque)
{
    Target_Torque = __Target_Torque;
}

/**
 * @brief 设定输出量
 *
 * @param __Output_Voltage 输出量
 */
void Class_Motor_C620::Set_Out(float __Out)
{
    Out = __Out;
}

/**
 * @brief CAN通信接收回调函数
 *
 * @param Rx_Data 接收的数据
 */
void Class_Motor_C620::CAN_RxCpltCallback(uint8_t *Rx_Data)
{
    int16_t delta_encoder;

    Flag += 1;

    Pre_Encoder = Rx_Encoder;

    Rx_Encoder = (Rx_Data[0] << 8) | Rx_Data[1];
    Rx_Omega = (Rx_Data[2] << 8) | Rx_Data[3];
    Rx_Torque = (Rx_Data[4] << 8) | Rx_Data[5];
    Rx_Temperature = Rx_Data[6];

    delta_encoder = Rx_Encoder - Pre_Encoder;
    if (delta_encoder < -4096)
    {
        //正方向转过了一圈
        Total_Round++;
    }
    else if (delta_encoder > 4096)
    {
        //反方向转过了一圈
        Total_Round--;
    }
    Total_Encoder = Total_Round * Encoder_Num_Per_Round + Rx_Encoder;

    Now_Angle = (float)Total_Encoder / (float)Encoder_Num_Per_Round * 2.0f * PI / Gearbox_Rate;
    Now_Omega = (float)Rx_Omega * RPM_TO_RADPS / Gearbox_Rate;
    Now_Torque = Rx_Torque;
    Now_Temperature = Rx_Temperature;
}

/**
 * @brief TIM定时器中断定期检测电机是否存活
 *
 */
void Class_Motor_C620::TIM_Alive_PeriodElapsedCallback()
{
    //判断该时间段内是否接收过电机数据
    if (Flag == Pre_Flag)
    {
        //电机断开连接
        CAN_Motor_Status = CAN_Motor_Status_DISABLE;
        PID_Angle.Set_Integral_Error(0.0f);
        PID_Omega.Set_Integral_Error(0.0f);
    }
    else
    {
        //电机保持连接
        CAN_Motor_Status = CAN_Motor_Status_ENABLE;
    }
    Pre_Flag = Flag;
}

/**
 * @brief TIM定时器中断计算回调函数
 *
 */
void Class_Motor_C620::TIM_PID_PeriodElapsedCallback()
{
    switch (Control_Method)
    {
    case (Control_Method_OPENLOOP):
    {
        //默认开环扭矩控制
        Set_Out(Target_Torque / Torque_Max * Output_Max);
    }
    break;
    case (Control_Method_TORQUE):
    {
        //默认闭环扭矩控制
        Set_Out(Target_Torque / Torque_Max * Output_Max);
    }
    break;
    case (Control_Method_OMEGA):
    {
        PID_Omega.Set_Target(Target_Omega);
        PID_Omega.Set_Now(Now_Omega);
        PID_Omega.TIM_Adjust_PeriodElapsedCallback();

        Set_Out(PID_Omega.Get_Out());
    }
    break;
    case (Control_Method_ANGLE):
    {
        PID_Angle.Set_Target(Target_Angle);
        PID_Angle.Set_Now(Now_Angle);
        PID_Angle.TIM_Adjust_PeriodElapsedCallback();

        Target_Omega = PID_Angle.Get_Out();

        PID_Omega.Set_Target(Target_Omega);
        PID_Omega.Set_Now(Now_Omega);
        PID_Omega.TIM_Adjust_PeriodElapsedCallback();

        Set_Out(PID_Omega.Get_Out());
    }
    break;
    default:
    {
        Set_Out(0.0f);
    }
    break;
    }
    Output();
}

/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/
