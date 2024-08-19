/**
 * @file dvc_motor_dm.h
 * @author yssickjgd (1345578933@qq.com)
 * @brief 达妙电机配置与操作
 * @version 0.1
 * @date 2023-08-30 0.1 新增
 *
 * @copyright USTC-RoboWalker (c) 2023
 *
 */

#ifndef DVC_MOTOR_DM_H
#define DVC_MOTOR_DM_H

/* Includes ------------------------------------------------------------------*/

#include "alg_pid.h"
#include "drv_can.h"
#include "drv_math.h"

/* Exported macros -----------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/**
 * @brief 达妙电机状态
 *
 */
enum Enum_Motor_DM_Status
{
    Motor_DM_Status_DISABLE = 0,
    Motor_DM_Status_ENABLE,
};

/**
 * @brief 达妙电机的ID枚举类型, 一拖四模式用
 *
 */
enum Enum_Motor_DM_Motor_ID_1_To_4 : uint8_t
{
    Motor_DM_ID_0x301 = 1,
    Motor_DM_ID_0x302,
    Motor_DM_ID_0x303,
    Motor_DM_ID_0x304,
    Motor_DM_ID_0x305,
    Motor_DM_ID_0x306,
    Motor_DM_ID_0x307,
    Motor_DM_ID_0x308,
};

/**
 * @brief 达妙电机控制状态, 传统模式有效
 *
 */
enum Enum_Motor_DM_Control_Status_Normal
{
    Motor_DM_Control_Status_DISABLE = 0x0,
    Motor_DM_Control_Status_ENABLE,
    Motor_DM_Control_Status_OVERVOLTAGE = 0x8,
    Motor_DM_Control_Status_UNDERVOLTAGE,
    Motor_DM_Control_Status_OVERCURRENT,
    Motor_DM_Control_Status_MOS_OVERTEMPERATURE,
    Motor_DM_Control_Status_ROTOR_OVERTEMPERATURE,
    Motor_DM_Control_Status_LOSE_CONNECTION,
    Motor_DM_Control_Status_MOS_OVERLOAD,
};

/**
 * @brief 达妙电机控制方式
 *
 */
enum Enum_Motor_DM_Control_Method
{
    Motor_DM_Control_Method_NORMAL_MIT = 0,
    Motor_DM_Control_Method_NORMAL_ANGLE_OMEGA,
    Motor_DM_Control_Method_NORMAL_OMEGA,
    Motor_DM_Control_Method_NORMAL_EMIT,
    Motor_DM_Control_Method_1_TO_4_CURRENT,
    Motor_DM_Control_Method_1_TO_4_OMEGA,
    Motor_DM_Control_Method_1_TO_4_ANGLE,
};

/**
 * @brief 达妙电机传统模式源数据
 *
 */
struct Struct_Motor_DM_CAN_Rx_Data_Normal
{
    uint8_t CAN_ID : 4;
    uint8_t Control_Status_Enum : 4;
    uint16_t Angle_Reverse;
    uint8_t Omega_11_4;
    uint8_t Omega_3_0_Torque_11_8;
    uint8_t Torque_7_0;
    uint8_t MOS_Temperature;
    uint8_t Rotor_Temperature;
} __attribute__((packed));

/**
 * @brief 达妙电机一拖四模式源数据
 *
 */
struct Struct_Motor_DM_CAN_Rx_Data_1_To_4
{
    uint16_t Encoder_Reverse;
    // 角速度100倍
    int16_t Omega_Reverse;
    // 电流值, mA
    int16_t Current_Reverse;
    uint8_t Rotor_Temperature;
    uint8_t MOS_Temperature;
} __attribute__((packed));

/**
 * @brief 达妙电机常规源数据, MIT控制报文
 *
 */
struct Struct_Motor_DM_CAN_Tx_Data_Normal_MIT
{
    uint16_t Control_Angle_Reverse;
    uint8_t Control_Omega_11_4;
    uint8_t Control_Omega_3_0_K_P_11_8;
    uint8_t K_P_7_0;
    uint8_t K_D_11_4;
    uint8_t K_D_3_0_Control_Torque_11_8;
    uint8_t Control_Torque_7_0;
} __attribute__((packed));

/**
 * @brief 达妙电机常规源数据, 位置速度控制报文
 *
 */
struct Struct_Motor_DM_CAN_Tx_Data_Normal_Angle_Omega
{
    float Control_Angle;
    float Control_Omega;
} __attribute__((packed));

/**
 * @brief 达妙电机常规源数据, 速度控制报文
 *
 */
struct Struct_Motor_DM_CAN_Tx_Data_Normal_Omega
{
    float Control_Omega;
} __attribute__((packed));

/**
 * @brief 达妙电机常规源数据, EMIT控制报文
 *
 */
struct Struct_Motor_DM_CAN_Tx_Data_Normal_EMIT
{
    float Control_Angle;
    // 限定速度用, rad/s的100倍
    uint16_t Control_Omega;
    // 限定电流用, 电流最大值的10000倍
    uint16_t Control_Current;
} __attribute__((packed));

/**
 * @brief 达妙电机经过处理的数据, 传统模式有效
 *
 */
struct Struct_Motor_DM_Rx_Data_Normal
{
    Enum_Motor_DM_Control_Status_Normal Control_Status;
    float Now_Angle;
    float Now_Omega;
    float Now_Torque;
    float Now_MOS_Temperature;
    float Now_Rotor_Temperature;
    uint32_t Pre_Encoder;
    int32_t Total_Encoder;
    int32_t Total_Round;
};

/**
 * @brief 达妙电机经过处理的数据, 一拖四模式有效
 *
 */
struct Struct_Motor_DM_Rx_Data_1_To_4
{
    float Now_Angle;
    float Now_Omega;
    float Now_Current;
    float Now_MOS_Temperature;
    float Now_Rotor_Temperature;
    uint32_t Pre_Encoder;
    int32_t Total_Encoder;
    int32_t Total_Round;
};

/**
 * @brief Reusable, 达妙电机, 传统模式
 * 没有零点, 可在上位机调零点
 * 初始化的角度, 角速度, 扭矩, 电流等参数是J4310电机默认值
 *
 */
class Class_Motor_DM_Normal
{
public:

    void Init(CAN_HandleTypeDef *hcan, uint8_t __CAN_Rx_ID, uint8_t __CAN_Tx_ID, Enum_Motor_DM_Control_Method __Motor_DM_Control_Method = Motor_DM_Control_Method_NORMAL_MIT, float __Angle_Max = 12.5f, float __Omega_Max = 25.0f, float __Torque_Max = 10.0f, float __Current_Max = 10.261194f);

    inline float Get_Angle_Max();

    inline float Get_Omega_Max();

    inline float Get_Torque_Max();

    inline float Get_Current_Max();

    inline Enum_Motor_DM_Status Get_Status();

    inline Enum_Motor_DM_Control_Status_Normal Get_Control_Status();

    inline float Get_Now_Angle();

    inline float Get_Now_Omega();

    inline float Get_Now_Torque();

    inline float Get_Now_MOS_Temperature();

    inline float Get_Now_Rotor_Temperature();

    inline Enum_Motor_DM_Control_Method Get_Control_Method();

    inline float Get_Control_Angle();

    inline float Get_Control_Omega();

    inline float Get_Control_Torque();

    inline float Get_Control_Current();

    inline float Get_K_P();

    inline float Get_K_D();

    inline void Set_Control_Angle(float __Control_Angle);

    inline void Set_Control_Omega(float __Control_Omega);

    inline void Set_Control_Torque(float __Control_Torque);

    inline void Set_Control_Current(float __Control_Current);

    inline void Set_K_P(float __K_P);

    inline void Set_K_D(float __K_D);

    void CAN_RxCpltCallback(uint8_t *Rx_Data);

    void CAN_Send_Clear_Error();

    void CAN_Send_Enter();

    void CAN_Send_Exit();

    void CAN_Send_Save_Zero();

    void TIM_Alive_PeriodElapsedCallback();

    void TIM_Send_PeriodElapsedCallback();

protected:
    // 初始化相关变量

    // 绑定的CAN
    Struct_CAN_Manage_Object *CAN_Manage_Object;
    // 收数据绑定的CAN ID, 与上位机驱动参数Master_ID保持一致
    uint16_t CAN_Rx_ID;
    // 发数据绑定的CAN ID, 是上位机驱动参数CAN_ID加上控制模式的偏移量
    uint16_t CAN_Tx_ID;
    // 最大位置, 与上位机控制幅值PMAX保持一致
    float Angle_Max;
    // 最大速度, 与上位机控制幅值VMAX保持一致
    float Omega_Max;
    // 最大扭矩, 与上位机控制幅值TMAX保持一致
    float Torque_Max;
    // 最大电流, 与上位机串口中上电打印电流保持一致
    float Current_Max;

    // 常量

    // 内部变量

    // 当前时刻的电机接收flag
    uint32_t Flag = 0;
    // 前一时刻的电机接收flag
    uint32_t Pre_Flag = 0;

    // 发送缓冲区
    uint8_t Tx_Data[8];

    // 读变量

    // 电机状态
    Enum_Motor_DM_Status Motor_DM_Status = Motor_DM_Status_DISABLE;
    // 电机对外接口信息
    Struct_Motor_DM_Rx_Data_Normal Rx_Data;

    // 写变量

    // 读写变量

    // 电机控制方式
    Enum_Motor_DM_Control_Method Motor_DM_Control_Method = Motor_DM_Control_Method_NORMAL_MIT;

    // 角度, rad, 目标角度
    float Control_Angle = 0.0f;
    // 角速度, rad/s, MIT模式和速度模式是目标角速度, 其余模式是限幅
    float Control_Omega = 0.0f;
    // 扭矩, Nm, MIT模式是目标扭矩, EMIT模式无效, 其余模式是限幅
    float Control_Torque = 0.0f;
    // 电流, A, EMIT模式是限幅, 其余模式无效
    float Control_Current = 0.0f;
    // K_P, 0~500, MIT模式有效
    float K_P = 0.0f;
    // K_D, 0~5, MIT模式有效
    float K_D = 0.0f;

    // 内部函数

    void Data_Process();

    void Output();
};

/**
 * @brief Reusable, 达妙电机, 一拖四模式
 * 没有零点, 可在上位机调零点
 *
 */
class Class_Motor_DM_1_To_4
{
public:

    // PID角度环控制
    Class_PID PID_Angle;
    // PID角速度环控制
    Class_PID PID_Omega;

    void Init(CAN_HandleTypeDef *hcan, Enum_Motor_DM_Motor_ID_1_To_4 __CAN_Rx_ID, Enum_Motor_DM_Control_Method __Motor_DM_Control_Method = Motor_DM_Control_Method_1_TO_4_ANGLE, int32_t __Encoder_Offset = 0, float __Current_Max = 10.261194f);

    inline float Get_Current_Max();

    inline float Get_Theoretical_Output_Current_Max();

    inline Enum_Motor_DM_Status Get_Status();

    inline float Get_Now_Angle();

    inline float Get_Now_Omega();

    inline float Get_Now_Current();

    inline float Get_Now_MOS_Temperature();

    inline float Get_Now_Rotor_Temperature();

    inline Enum_Motor_DM_Control_Method Get_Control_Method();

    inline float Get_Target_Angle();

    inline float Get_Target_Omega();

    inline float Get_Target_Current();

    inline float Get_Feedforward_Omega();

    inline float Get_Feedforward_Current();

    inline void Set_Control_Method(Enum_Motor_DM_Control_Method __DM_Motor_Control_Method);

    inline void Set_Target_Angle(float __Target_Angle);

    inline void Set_Target_Omega(float __Target_Omega);

    inline void Set_Target_Current(float __Target_Current);

    inline void Set_Feedforward_Omega(float __Feedforward_Omega);

    inline void Set_Feedforward_Current(float __Feedforward_Current);

    void CAN_RxCpltCallback(uint8_t *Rx_Data);

    void TIM_100ms_Alive_PeriodElapsedCallback();

    void TIM_1ms_Calculate_PeriodElapsedCallback();

protected:
    // 初始化相关变量

    // 绑定的CAN
    Struct_CAN_Manage_Object *CAN_Manage_Object;
    // 收数据绑定的CAN ID, 达妙系列0x301~0x308
    Enum_Motor_DM_Motor_ID_1_To_4 CAN_Rx_ID;
    // 编码器偏移
    int32_t Encoder_Offset;
    // 发送缓存区
    uint8_t *Tx_Data;
    // 最大电流
    float Current_Max;

    // 常量

    // 一圈编码器刻度
    uint16_t Encoder_Num_Per_Round = 8192;

    // 电流到输出的转化系数
    float Current_To_Out = 16384.0f / 10.261194f;
    // 理论最大输出电流
    float Theoretical_Output_Current_Max = 10.261194f;

    // 内部变量

    // 当前时刻的电机接收flag
    uint32_t Flag = 0;
    // 前一时刻的电机接收flag
    uint32_t Pre_Flag = 0;
    // 输出量
    float Out = 0.0f;

    // 读变量

    // 电机状态
    Enum_Motor_DM_Status Motor_DM_Status = Motor_DM_Status_DISABLE;
    // 电机对外接口信息
    Struct_Motor_DM_Rx_Data_1_To_4 Rx_Data;

    // 写变量

    // 读写变量
    
    // 电机控制方式
    Enum_Motor_DM_Control_Method Motor_DM_Control_Method = Motor_DM_Control_Method_1_TO_4_ANGLE;
    // 目标的角度
    float Target_Angle = 0.0f;
    // 目标的速度, rad/s
    float Target_Omega = 0.0f;
    // 目标的电流, A
    float Target_Current = 0.0f;
    // 前馈的速度, rad/s
    float Feedforward_Omega = 0.0f;
    // 前馈的电流, A
    float Feedforward_Current = 0.0f;

    // 内部函数

    void Data_Process();

    void PID_Calculate();

    void Output();
};

/* Exported variables --------------------------------------------------------*/

/* Exported function declarations --------------------------------------------*/

/**
 * @brief 获取角度最大值
 *
 * @return float 角度最大值
 */
inline float Class_Motor_DM_Normal::Get_Angle_Max()
{
    return (Angle_Max);
}

/**
 * @brief 获取角速度最大值
 *
 * @return float 角速度最大值
 */
inline float Class_Motor_DM_Normal::Get_Omega_Max()
{
    return (Omega_Max);
}

/**
 * @brief 获取扭矩最大值
 *
 * @return float 扭矩最大值
 */
inline float Class_Motor_DM_Normal::Get_Torque_Max()
{
    return (Torque_Max);
}

/**
 * @brief 获取电流最大值
 *
 * @return float 电流最大值
 */
inline float Class_Motor_DM_Normal::Get_Current_Max()
{
    return (Current_Max);
}

/**
 * @brief 获取电机状态
 *
 * @return Enum_Motor_DM_Status 电机状态
 */
inline Enum_Motor_DM_Status Class_Motor_DM_Normal::Get_Status()
{
    return (Motor_DM_Status);
}

/**
 * @brief 获取电机控制状态
 *
 * @return Enum_Motor_DM_Control_Status_Normal 电机控制状态
 */
inline Enum_Motor_DM_Control_Status_Normal Class_Motor_DM_Normal::Get_Control_Status()
{
    return (Rx_Data.Control_Status);
}

/**
 * @brief 获取当前角度
 *
 * @return float 当前角度
 */
inline float Class_Motor_DM_Normal::Get_Now_Angle()
{
    return (Rx_Data.Now_Angle);
}

/**
 * @brief 获取当前角速度
 *
 * @return float 当前角速度
 */
inline float Class_Motor_DM_Normal::Get_Now_Omega()
{
    return (Rx_Data.Now_Omega);
}

/**
 * @brief 获取当前扭矩
 *
 * @return float 当前扭矩
 */
inline float Class_Motor_DM_Normal::Get_Now_Torque()
{
    return (Rx_Data.Now_Torque);
}

/**
 * @brief 获取当前MOS温度
 *
 * @return float 当前MOS温度
 */
inline float Class_Motor_DM_Normal::Get_Now_MOS_Temperature()
{
    return (Rx_Data.Now_MOS_Temperature);
}

/**
 * @brief 获取当前转子温度
 *
 * @return float 当前转子温度
 */
inline float Class_Motor_DM_Normal::Get_Now_Rotor_Temperature()
{
    return (Rx_Data.Now_Rotor_Temperature);
}

/**
 * @brief 获取电机控制方式
 *
 * @return Enum_Motor_DM_Control_Method 电机控制方式
 */
inline Enum_Motor_DM_Control_Method Class_Motor_DM_Normal::Get_Control_Method()
{
    return (Motor_DM_Control_Method);
}

/**
 * @brief 获取角度, rad, 目标角度
 *
 * @return float 角度, rad, 目标角度
 */
inline float Class_Motor_DM_Normal::Get_Control_Angle()
{
    return (Control_Angle);
}

/**
 * @brief 获取角速度, rad/s, MIT模式和速度模式是目标角速度, 其余模式是限幅
 *
 * @return float 角速度, rad/s, MIT模式和速度模式是目标角速度, 其余模式是限幅
 */
inline float Class_Motor_DM_Normal::Get_Control_Omega()
{
    return (Control_Omega);
}

/**
 * @brief 获取扭矩, Nm, MIT模式是目标扭矩, EMIT模式无效, 其余模式是限幅
 *
 * @return float 扭矩, Nm, MIT模式是目标扭矩, EMIT模式无效, 其余模式是限幅
 */
inline float Class_Motor_DM_Normal::Get_Control_Torque()
{
    return (Control_Torque);
}

/**
 * @brief 获取电流, A, EMIT模式是限幅, 其余模式无效
 *
 * @return float 电流, A, EMIT模式是限幅, 其余模式无效
 */
inline float Class_Motor_DM_Normal::Get_Control_Current()
{
    return (Control_Current);
}

/**
 * @brief 获取K_P, 0~500, MIT模式有效
 *
 * @return float K_P, 0~500, MIT模式有效
 */
inline float Class_Motor_DM_Normal::Get_K_P()
{
    return (K_P);
}

/**
 * @brief 获取K_D, 0~5, MIT模式有效
 *
 * @return float K_D, 0~5, MIT模式有效
 */
inline float Class_Motor_DM_Normal::Get_K_D()
{
    return (K_D);
}

/**
 * @brief 设定角度, rad, 目标角度
 *
 * @param __Control_Angle 角度, rad, 目标角度
 */
inline void Class_Motor_DM_Normal::Set_Control_Angle(float __Control_Angle)
{
    Control_Angle = __Control_Angle;
}

/**
 * @brief 设定角速度, rad/s, MIT模式和速度模式是目标角速度, 其余模式是限幅
 *
 * @param __Control_Omega 角速度, rad/s, MIT模式和速度模式是目标角速度, 其余模式是限幅
 */
inline void Class_Motor_DM_Normal::Set_Control_Omega(float __Control_Omega)
{
    Control_Omega = __Control_Omega;
}

/**
 * @brief 设定扭矩, Nm, MIT模式是目标扭矩, EMIT模式无效, 其余模式是限幅
 *
 * @param __Control_Torque 扭矩, Nm, MIT模式是目标扭矩, EMIT模式无效, 其余模式是限幅
 */
inline void Class_Motor_DM_Normal::Set_Control_Torque(float __Control_Torque)
{
    Control_Torque = __Control_Torque;
}

/**
 * @brief 设定电流, A, EMIT模式是限幅, 其余模式无效
 *
 * @param __Control_Current 电流, A, EMIT模式是限幅, 其余模式无效
 */
inline void Class_Motor_DM_Normal::Set_Control_Current(float __Control_Current)
{
    Control_Current = __Control_Current;
}

/**
 * @brief 设定K_P, 0~500, MIT模式有效
 *
 * @param __K_P K_P, 0~500, MIT模式有效
 */
inline void Class_Motor_DM_Normal::Set_K_P(float __K_P)
{
    K_P = __K_P;
}

/**
 * @brief 设定K_D, 0~5, MIT模式有效
 *
 * @param __K_D K_D, 0~5, MIT模式有效
 */
inline void Class_Motor_DM_Normal::Set_K_D(float __K_D)
{
    K_D = __K_D;
}

/**
 * @brief 获取电流最大值
 *
 * @return float 电流最大值
 */
inline float Class_Motor_DM_1_To_4::Get_Current_Max()
{
    return (Current_Max);
}

/**
 * @brief 获取理论最大输出电流
 *
 * @return float 理论最大输出电流
 */
inline float Class_Motor_DM_1_To_4::Get_Theoretical_Output_Current_Max()
{
    return (Theoretical_Output_Current_Max);
}

/**
 * @brief 获取电机状态
 *
 * @return Enum_Motor_DM_Status 电机状态
 */
inline Enum_Motor_DM_Status Class_Motor_DM_1_To_4::Get_Status()
{
    return (Motor_DM_Status);
}

/**
 * @brief 获取当前角度
 *
 * @return float 当前角度
 */
inline float Class_Motor_DM_1_To_4::Get_Now_Angle()
{
    return (Rx_Data.Now_Angle);
}

/**
 * @brief 获取当前角速度
 *
 * @return float 当前角速度
 */
inline float Class_Motor_DM_1_To_4::Get_Now_Omega()
{
    return (Rx_Data.Now_Omega);
}

/**
 * @brief 获取当前电流
 *
 * @return float 当前电流
 */
inline float Class_Motor_DM_1_To_4::Get_Now_Current()
{
    return (Rx_Data.Now_Current);
}

/**
 * @brief 获取当前MOS温度
 *
 * @return float 当前MOS温度
 */
inline float Class_Motor_DM_1_To_4::Get_Now_MOS_Temperature()
{
    return (Rx_Data.Now_MOS_Temperature);
}

/**
 * @brief 获取当前转子温度
 *
 * @return float 当前转子温度
 */
inline float Class_Motor_DM_1_To_4::Get_Now_Rotor_Temperature()
{
    return (Rx_Data.Now_Rotor_Temperature);
}

/**
 * @brief 获取电机控制方式
 *
 * @return Enum_Motor_DM_Control_Method 电机控制方式
 */
inline Enum_Motor_DM_Control_Method Class_Motor_DM_1_To_4::Get_Control_Method()
{
    return (Motor_DM_Control_Method);
}

/**
 * @brief 获取目标的角度
 *
 * @return float 目标的角度
 */
inline float Class_Motor_DM_1_To_4::Get_Target_Angle()
{
    return (Target_Angle);
}

/**
 * @brief 获取目标的速度, rad/s
 *
 * @return float 目标的速度, rad/s
 */
inline float Class_Motor_DM_1_To_4::Get_Target_Omega()
{
    return (Target_Omega);
}

/**
 * @brief 获取目标的电流, A
 *
 * @return float 目标的电流, A
 */
inline float Class_Motor_DM_1_To_4::Get_Target_Current()
{
    return (Target_Current);
}

/**
 * @brief 获取前馈的速度, rad/s
 *
 * @return float 前馈的速度, rad/s
 */
inline float Class_Motor_DM_1_To_4::Get_Feedforward_Omega()
{
    return (Feedforward_Omega);
}

/**
 * @brief 获取前馈的电流, A
 *
 * @return float 前馈的电流, A
 */
inline float Class_Motor_DM_1_To_4::Get_Feedforward_Current()
{
    return (Feedforward_Current);
}

/**
 * @brief 设定电机控制方式
 *
 * @param __DM_Motor_Control_Method 电机控制方式
 */
inline void Class_Motor_DM_1_To_4::Set_Control_Method(Enum_Motor_DM_Control_Method __DM_Motor_Control_Method)
{
    Motor_DM_Control_Method = __DM_Motor_Control_Method;
}

/**
 * @brief 设定目标的角度
 *
 * @param __Target_Angle 目标的角度
 */
inline void Class_Motor_DM_1_To_4::Set_Target_Angle(float __Target_Angle)
{
    Target_Angle = __Target_Angle;
}

/**
 * @brief 设定目标的速度, rad/s
 *
 * @param __Target_Omega 目标的速度, rad/s
 */
inline void Class_Motor_DM_1_To_4::Set_Target_Omega(float __Target_Omega)
{
    Target_Omega = __Target_Omega;
}

/**
 * @brief 设定目标的电流, A
 *
 * @param __Target_Current 目标的电流, A
 */
inline void Class_Motor_DM_1_To_4::Set_Target_Current(float __Target_Current)
{
    Target_Current = __Target_Current;
}

/**
 * @brief 设定前馈的速度, rad/s
 *
 * @param __Feedforward_Omega 前馈的速度, rad/s
 */
inline void Class_Motor_DM_1_To_4::Set_Feedforward_Omega(float __Feedforward_Omega)
{
    Feedforward_Omega = __Feedforward_Omega;
}

/**
 * @brief 设定前馈的电流, A
 *
 * @param __Feedforward_Current 前馈的电流, A
 */
inline void Class_Motor_DM_1_To_4::Set_Feedforward_Current(float __Feedforward_Current)
{
    Feedforward_Current = __Feedforward_Current;
}

#endif

/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/
