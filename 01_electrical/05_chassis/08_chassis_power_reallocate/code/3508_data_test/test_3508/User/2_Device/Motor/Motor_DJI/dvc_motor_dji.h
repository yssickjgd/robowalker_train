/**
 * @file dvc_motor_dji.h
 * @author yssickjgd (1345578933@qq.com)
 * @brief 大疆电机配置与操作
 * @version 1.2
 * @date 2023-08-29 0.1 23赛季定稿
 * @date 2024-01-01 1.1 官方6020驱动更新, 适配电压控制与电流控制
 * @date 2024-03-07 1.2 新增功率控制接口与相关函数
 *
 * @copyright USTC-RoboWalker (c) 2023-2024
 *
 */

#ifndef DVC_MOTOR_DJI_H
#define DVC_MOTOR_DJI_H

/* Includes ------------------------------------------------------------------*/

#include "1_Middleware/2_Algorithm/PID/alg_pid.h"
#include "1_Middleware/1_Driver/CAN/drv_can.h"

/* Exported macros -----------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/**
 * @brief 大疆状态
 *
 */
enum Enum_Motor_DJI_Status
{
    Motor_DJI_Status_DISABLE = 0,
    Motor_DJI_Status_ENABLE,
};

/**
 * @brief 大疆电机的ID枚举类型
 *
 */
enum Enum_Motor_DJI_ID
{
    Motor_DJI_ID_0x201 = 1,
    Motor_DJI_ID_0x202,
    Motor_DJI_ID_0x203,
    Motor_DJI_ID_0x204,
    Motor_DJI_ID_0x205,
    Motor_DJI_ID_0x206,
    Motor_DJI_ID_0x207,
    Motor_DJI_ID_0x208,
    Motor_DJI_ID_0x209,
    Motor_DJI_ID_0x20A,
    Motor_DJI_ID_0x20B,
};

/**
 * @brief 大疆电机控制方式
 *
 */
enum Enum_Motor_DJI_Control_Method
{
    Motor_DJI_Control_Method_VOLTAGE = 0,
    Motor_DJI_Control_Method_CURRENT,
    Motor_DJI_Control_Method_TORQUE,
    Motor_DJI_Control_Method_OMEGA,
    Motor_DJI_Control_Method_ANGLE,
};

/**
 * @brief 大疆电机驱动版本, 影响GM6020电机驱动方式
 *
 */
enum Enum_Motor_DJI_GM6020_Driver_Version
{
    Motor_DJI_GM6020_Driver_Version_DEFAULT = 0,
    Motor_DJI_GM6020_Driver_Version_2023,
};

/**
 * @brief 是否开启功率控制, 此时电机须电流作为输出模式, 不可电压控制
 *
 */
enum Enum_Motor_DJI_Power_Limit_Status
{
    Motor_DJI_Power_Limit_Status_DISABLE = 0,
    Motor_DJI_Power_Limit_Status_ENABLE,
};

/**
 * @brief 大疆电机源数据
 *
 */
struct Struct_Motor_DJI_CAN_Rx_Data
{
    uint16_t Encoder_Reverse;
    int16_t Omega_Reverse;
    int16_t Current_Reverse;
    uint8_t Temperature;
    uint8_t Reserved;
} __attribute__((packed));

/**
 * @brief 大疆电机经过处理的数据
 *
 */
struct Struct_Motor_DJI_Rx_Data
{
    float Now_Angle;
    float Now_Omega;
    float Now_Current;
    float Now_Temperature;
    float Now_Power;
    uint32_t Pre_Encoder;
    int32_t Total_Encoder;
    int32_t Total_Round;
};

/**
 * @brief Reusable, GM6020无刷电机, 单片机控制输出电压, 新驱动支持电流
 *
 */
class Class_Motor_DJI_GM6020
{
public:
    // PID角度环控制
    Class_PID PID_Angle;
    // PID角速度环控制
    Class_PID PID_Omega;
    // PID电流环控制
    Class_PID PID_Current;

    void Init(CAN_HandleTypeDef *hcan, Enum_Motor_DJI_ID __CAN_Rx_ID, Enum_Motor_DJI_Control_Method __Motor_DJI_Control_Method = Motor_DJI_Control_Method_ANGLE, int32_t __Encoder_Offset = 0, Enum_Motor_DJI_GM6020_Driver_Version __Drive_Version = Motor_DJI_GM6020_Driver_Version_DEFAULT, Enum_Motor_DJI_Power_Limit_Status __Power_Limit_Status = Motor_DJI_Power_Limit_Status_DISABLE, float __Voltage_Max = 24.0f, float __Current_Max = 3.0f);

    inline float Get_Voltage_Max();

    inline float Get_Current_Max();

    inline float Get_Theoretical_Output_Voltage_Max();

    inline float Get_Theoretical_Output_Current_Max();

    inline Enum_Motor_DJI_Status Get_Status();

    inline float Get_Now_Angle();

    inline float Get_Now_Omega();

    inline float Get_Now_Current();

    inline float Get_Now_Temperature();

    inline float Get_Now_Power();

    inline float Get_Power_Estimate();

    inline Enum_Motor_DJI_Control_Method Get_Control_Method();

    inline float Get_Target_Angle();

    inline float Get_Target_Omega();

    inline float Get_Target_Current();

    inline float Get_Target_Voltage();

    inline float Get_Feedforward_Omega();

    inline float Get_Feedforward_Current();

    inline float Get_Feedforward_Voltage();

    inline float Get_Power_Factor();

    inline void Set_Control_Method(Enum_Motor_DJI_Control_Method __Motor_DJI_Control_Method);

    inline void Set_Target_Angle(float __Target_Angle);

    inline void Set_Target_Omega(float __Target_Omega);

    inline void Set_Target_Current(float __Target_Current);

    inline void Set_Target_Voltage(float __Voltage);

    inline void Set_Feedforward_Omega(float __Feedforward_Omega);

    inline void Set_Feedforward_Current(float __Feedforward_Current);

    inline void Set_Feedforward_Voltage(float __Feedforward_Voltage);

    inline void Set_Power_Factor(float __Power_Factor);

    void CAN_RxCpltCallback(uint8_t *Rx_Data);

    void TIM_100ms_Alive_PeriodElapsedCallback();

    void TIM_Calculate_PeriodElapsedCallback();

    void TIM_Power_Limit_After_Calculate_PeriodElapsedCallback();

protected:
    // 初始化相关变量

    // 绑定的CAN
    Struct_CAN_Manage_Object *CAN_Manage_Object;
    // 收数据绑定的CAN ID, C6系列0x201~0x208, GM系列0x205~0x20b
    Enum_Motor_DJI_ID CAN_Rx_ID;
    // 发送缓存区
    uint8_t *Tx_Data;
    // 编码器偏移
    int32_t Encoder_Offset;
    // 电机驱动方式
    Enum_Motor_DJI_GM6020_Driver_Version Driver_Version;
    // 是否开启功率控制
    Enum_Motor_DJI_Power_Limit_Status Power_Limit_Status;
    // 最大电压
    float Voltage_Max;
    // 最大电流
    float Current_Max;

    // 常量

    // 功率计算系数
    float Power_K_0 = 0.8130f;
    float Power_K_1 = -0.0005f;
    float Power_K_2 = 6.0021f;
    float Power_A = 1.3715f;

    // 一圈编码器刻度
    uint16_t Encoder_Num_Per_Round = 8192;

    // 电压到输出的转化系数
    float Voltage_To_Out = 25000.0f / 24.0f;
    // 电流到输出的转化系数
    float Current_To_Out = 16384.0f / 3.0f;
    // 理论最大输出电压
    float Theoretical_Output_Voltage_Max = 24.0f;
    // 理论最大输出电流
    float Theoretical_Output_Current_Max = 3.0f;

    // 内部变量

    // 当前时刻的电机接收flag
    uint32_t Flag = 0;
    // 前一时刻的电机接收flag
    uint32_t Pre_Flag = 0;
    // 输出量
    float Out = 0.0f;

    // 读变量

    // 电机状态
    Enum_Motor_DJI_Status Motor_DJI_Status = Motor_DJI_Status_DISABLE;
    // 电机对外接口信息
    Struct_Motor_DJI_Rx_Data Rx_Data;
    // 下一时刻的功率估计值, W
    float Power_Estimate;

    // 写变量

    // 读写变量

    // 电机控制方式
    Enum_Motor_DJI_Control_Method Motor_DJI_Control_Method = Motor_DJI_Control_Method_ANGLE;
    // 目标的角度, rad
    float Target_Angle = 0.0f;
    // 目标的速度, rad/s
    float Target_Omega = 0.0f;
    // 目标的电流, A
    float Target_Current = 0.0f;
    // 目标的电压, V
    float Target_Voltage = 0.0f;
    // 前馈的速度, rad/s
    float Feedforward_Omega = 0.0f;
    // 前馈的电流, A
    float Feedforward_Current = 0.0f;
    // 前馈的电压, V
    float Feedforward_Voltage = 0.0f;

    // 功率衰减因数
    float Power_Factor = 1.0f;

    // 内部函数

    void Data_Process();

    void PID_Calculate();

    void Power_Limit_Control();

    void Output();
};

/**
 * @brief Reusable, C610无刷电调, 自带电流环, 单片机控制输出电流
 *
 */
class Class_Motor_DJI_C610
{
public:
    // PID角度环控制
    Class_PID PID_Angle;
    // PID角速度环控制
    Class_PID PID_Omega;

    void Init(CAN_HandleTypeDef *hcan, Enum_Motor_DJI_ID __CAN_Rx_ID, Enum_Motor_DJI_Control_Method __Motor_DJI_Control_Method = Motor_DJI_Control_Method_OMEGA, float __Gearbox_Rate = 36.0f, float __Current_Max = 10.0f);

    inline float Get_Current_Max();

    inline float Get_Theoretical_Output_Current_Max();

    inline Enum_Motor_DJI_Status Get_Status();

    inline float Get_Now_Angle();

    inline float Get_Now_Omega();

    inline float Get_Now_Current();

    inline uint8_t Get_Now_Temperature();

    inline Enum_Motor_DJI_Control_Method Get_Control_Method();

    inline float Get_Target_Angle();

    inline float Get_Target_Omega();

    inline float Get_Target_Current();

    inline float Get_Feedforward_Omega();

    inline float Get_Feedforward_Current();

    inline void Set_Control_Method(Enum_Motor_DJI_Control_Method __Motor_DJI_Control_Method);

    inline void Set_Target_Angle(float __Target_Angle);

    inline void Set_Target_Omega(float __Target_Omega);

    inline void Set_Target_Current(float __Target_Current);

    inline void Set_Feedforward_Omega(float __Feedforward_Omega);

    inline void Set_Feedforward_Current(float __Feedforward_Current);

    void CAN_RxCpltCallback(uint8_t *Rx_Data);

    void TIM_100ms_Alive_PeriodElapsedCallback();

    void TIM_Calculate_PeriodElapsedCallback();

protected:
    // 初始化相关常量

    // 绑定的CAN
    Struct_CAN_Manage_Object *CAN_Manage_Object;
    // 收数据绑定的CAN ID, C6系列0x201~0x208, GM系列0x205~0x20b
    Enum_Motor_DJI_ID CAN_Rx_ID;
    // 发送缓存区
    uint8_t *Tx_Data;
    // 减速比, 默认带减速箱
    float Gearbox_Rate;
    // 最大电流
    float Current_Max;

    // 常量

    // 一圈编码器刻度
    uint16_t Encoder_Num_Per_Round = 8192;

    // 电流到输出的转化系数
    float Current_To_Out = 10000.0f / 10.0f;
    // 理论最大输出电流
    float Theoretical_Output_Current_Max = 10.0f;

    // 内部变量

    // 当前时刻的电机接收flag
    uint32_t Flag = 0;
    // 前一时刻的电机接收flag
    uint32_t Pre_Flag = 0;
    // 输出量
    float Out = 0.0f;

    // 读变量

    // 电机状态
    Enum_Motor_DJI_Status Motor_DJI_Status = Motor_DJI_Status_DISABLE;
    // 电机对外接口信息
    Struct_Motor_DJI_Rx_Data Rx_Data;

    // 写变量

    // 读写变量

    // 电机控制方式
    Enum_Motor_DJI_Control_Method Motor_DJI_Control_Method = Motor_DJI_Control_Method_ANGLE;
    // 目标的角度, rad
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

/**
 * @brief Reusable, C620无刷电调, 自带电流环, 单片机控制输出电流
 *
 */
class Class_Motor_DJI_C620
{
public:
    // PID角度环控制
    Class_PID PID_Angle;
    // PID角速度环控制
    Class_PID PID_Omega;

    void Init(CAN_HandleTypeDef *hcan, Enum_Motor_DJI_ID __CAN_Rx_ID, Enum_Motor_DJI_Control_Method __Motor_DJI_Control_Method = Motor_DJI_Control_Method_OMEGA, float __Gearbox_Rate = 3591.0f / 187.0f, Enum_Motor_DJI_Power_Limit_Status __Power_Limit_Status = Motor_DJI_Power_Limit_Status_DISABLE, float __Current_Max = 20.0f);

    inline float Get_Current_Max();

    inline float Get_Theoretical_Output_Current_Max();

    inline Enum_Motor_DJI_Status Get_Status();

    inline float Get_Now_Angle();

    inline float Get_Now_Omega();

    inline float Get_Now_Current();

    inline float Get_Now_Temperature();

    inline float Get_Now_Power();

    inline float Get_Power_Estimate();

    inline Enum_Motor_DJI_Control_Method Get_Control_Method();

    inline float Get_Target_Angle();

    inline float Get_Target_Omega();

    inline float Get_Target_Current();

    inline float Get_Feedforward_Omega();

    inline float Get_Feedforward_Current();

    inline void Set_Control_Method(Enum_Motor_DJI_Control_Method __Motor_DJI_Control_Method);

    inline void Set_Target_Angle(float __Target_Angle);

    inline void Set_Target_Omega(float __Target_Omega);

    inline void Set_Target_Current(float __Target_Current);

    inline void Set_Feedforward_Omega(float __Feedforward_Omega);

    inline void Set_Feedforward_Current(float __Feedforward_Current);

    inline void Set_Power_Factor(float __Power_Factor);

    void CAN_RxCpltCallback(uint8_t *Rx_Data);

    void TIM_100ms_Alive_PeriodElapsedCallback();

    void TIM_Calculate_PeriodElapsedCallback();

    void TIM_Power_Limit_After_Calculate_PeriodElapsedCallback();

protected:
    // 初始化相关变量

    // 绑定的CAN
    Struct_CAN_Manage_Object *CAN_Manage_Object;
    // 收数据绑定的CAN ID, C6系列0x201~0x208, GM系列0x205~0x20b
    Enum_Motor_DJI_ID CAN_Rx_ID;
    // 发送缓存区
    uint8_t *Tx_Data;
    // 减速比, 默认带减速箱
    float Gearbox_Rate;
    // 是否开启功率控制
    Enum_Motor_DJI_Power_Limit_Status Power_Limit_Status;
    // 最大电流
    float Current_Max;

    // 常量

    // 功率计算系数
    float Power_K_0 = 0.2962f;
    float Power_K_1 = 0.0000f;
    float Power_K_2 = 0.1519f;
    float Power_A = 1.3544f;

    // 一圈编码器刻度
    uint16_t Encoder_Num_Per_Round = 8192;

    // 电流到输出的转化系数
    float Current_To_Out = 16384.0f / 20.0f;
    // 理论最大输出电流
    float Theoretical_Output_Current_Max = 20.0f;

    // 内部变量

    // 当前时刻的电机接收flag
    uint32_t Flag = 0;
    // 前一时刻的电机接收flag
    uint32_t Pre_Flag = 0;
    // 输出量
    float Out = 0.0f;

    // 读变量

    // 电机状态
    Enum_Motor_DJI_Status Motor_DJI_Status = Motor_DJI_Status_DISABLE;
    // 电机对外接口信息
    Struct_Motor_DJI_Rx_Data Rx_Data;
    // 下一时刻的功率估计值, W
    float Power_Estimate;

    // 写变量

    // 读写变量

    // 电机控制方式
    Enum_Motor_DJI_Control_Method Motor_DJI_Control_Method = Motor_DJI_Control_Method_ANGLE;
    // 目标的角度, rad
    float Target_Angle = 0.0f;
    // 目标的速度, rad/s
    float Target_Omega = 0.0f;
    // 目标的电流
    float Target_Current = 0.0f;
    // 前馈的速度, rad/s
    float Feedforward_Omega = 0.0f;
    // 前馈的电流, A
    float Feedforward_Current = 0.0f;
    // 功率衰减因数
    float Power_Factor = 1.0f;

    // 内部函数

    void Data_Process();

    void PID_Calculate();

    void Power_Limit_Control();

    void Output();
};

/* Exported variables --------------------------------------------------------*/

/* Exported function declarations --------------------------------------------*/

/**
 * @brief 获取最大电压
 *
 * @return float 最大电压
 */
inline float Class_Motor_DJI_GM6020::Get_Voltage_Max()
{
    return (Voltage_Max);
}

/**
 * @brief 获取最大电流
 *
 * @return float 最大电流
 */
inline float Class_Motor_DJI_GM6020::Get_Current_Max()
{
    return (Current_Max);
}

/**
 * @brief 获取理论最大输出电压
 *
 * @return float 理论最大输出电压
 */
inline float Class_Motor_DJI_GM6020::Get_Theoretical_Output_Voltage_Max()
{
    return (Theoretical_Output_Voltage_Max);
}

/**
 * @brief 获取理论最大输出电流
 *
 * @return float 理论最大输出电流
 */
inline float Class_Motor_DJI_GM6020::Get_Theoretical_Output_Current_Max()
{
    return (Theoretical_Output_Current_Max);
}

/**
 * @brief 获取电机状态
 *
 * @return Enum_Motor_DJI_Status 电机状态
 */
inline Enum_Motor_DJI_Status Class_Motor_DJI_GM6020::Get_Status()
{
    return (Motor_DJI_Status);
}

/**
 * @brief 获取当前的角度, rad
 *
 * @return float 当前的角度, rad
 */
inline float Class_Motor_DJI_GM6020::Get_Now_Angle()
{
    return (Rx_Data.Now_Angle);
}

/**
 * @brief 获取当前的速度, rad/s
 *
 * @return float 当前的速度, rad/s
 */
inline float Class_Motor_DJI_GM6020::Get_Now_Omega()
{
    return (Rx_Data.Now_Omega);
}

/**
 * @brief 获取当前的电流, A
 *
 * @return 当前的电流, A
 */
inline float Class_Motor_DJI_GM6020::Get_Now_Current()
{
    return (Rx_Data.Now_Current);
}

/**
 * @brief 获取当前的温度, K
 *
 * @return float 当前的温度, K
 */
inline float Class_Motor_DJI_GM6020::Get_Now_Temperature()
{
    return (Rx_Data.Now_Temperature);
}

/**
 * @brief 获取当前的功率, W
 *
 * @return float 当前的功率, W
 */
inline float Class_Motor_DJI_GM6020::Get_Now_Power()
{
    return (Rx_Data.Now_Power);
}

/**
 * @brief 获取下一时刻的功率估计值, W
 *
 * @return float 下一时刻的功率估计值, W
 */
inline float Class_Motor_DJI_GM6020::Get_Power_Estimate()
{
    return (Power_Estimate);
}

/**
 * @brief 获取电机控制方式
 *
 * @return Enum_Motor_DJI_Control_Method 电机控制方式
 */
inline Enum_Motor_DJI_Control_Method Class_Motor_DJI_GM6020::Get_Control_Method()
{
    return (Motor_DJI_Control_Method);
}

/**
 * @brief 获取目标的角度, rad
 *
 * @return float 目标的角度, rad
 */
inline float Class_Motor_DJI_GM6020::Get_Target_Angle()
{
    return (Target_Angle);
}

/**
 * @brief 获取目标的速度, rad/s
 *
 * @return float 目标的速度, rad/s
 */
inline float Class_Motor_DJI_GM6020::Get_Target_Omega()
{
    return (Target_Omega);
}

/**
 * @brief 获取目标的电流
 *
 * @return float 目标的电流
 */
inline float Class_Motor_DJI_GM6020::Get_Target_Current()
{
    return (Target_Current);
}

/**
 * @brief 获取输出量
 *
 * @return float 输出量
 */
inline float Class_Motor_DJI_GM6020::Get_Target_Voltage()
{
    return (Target_Voltage);
}

/**
 * @brief 获取前馈的速度, rad/s
 *
 * @return float 前馈的速度, rad/s
 */
inline float Class_Motor_DJI_GM6020::Get_Feedforward_Omega()
{
    return (Feedforward_Omega);
}

/**
 * @brief 获取前馈的电流, A
 *
 * @return float 前馈的电流, A
 */
inline float Class_Motor_DJI_GM6020::Get_Feedforward_Current()
{
    return (Feedforward_Current);
}

/**
 * @brief 获取前馈的电压, V
 *
 * @return float 前馈的电压, V
 */
inline float Class_Motor_DJI_GM6020::Get_Feedforward_Voltage()
{
    return (Feedforward_Voltage);
}

/**
 * @brief 获取功率衰减因数
 *
 * @return float 功率衰减因数
 */
inline float Class_Motor_DJI_GM6020::Get_Power_Factor()
{
    return (Power_Factor);
}

/**
 * @brief 设定电机控制方式
 *
 * @param __Motor_DJI_Control_Method 电机控制方式
 */
inline void Class_Motor_DJI_GM6020::Set_Control_Method(Enum_Motor_DJI_Control_Method __Motor_DJI_Control_Method)
{
    Motor_DJI_Control_Method = __Motor_DJI_Control_Method;
}

/**
 * @brief 设定目标的角度, rad
 *
 * @param __Target_Angle 目标的角度, rad
 */
inline void Class_Motor_DJI_GM6020::Set_Target_Angle(float __Target_Angle)
{
    Target_Angle = __Target_Angle;
}

/**
 * @brief 设定目标的速度, rad/s
 *
 * @param __Target_Omega 目标的速度, rad/s
 */
inline void Class_Motor_DJI_GM6020::Set_Target_Omega(float __Target_Omega)
{
    Target_Omega = __Target_Omega;
}

/**
 * @brief 设定目标的电流
 *
 * @param __Target_Current 目标的电流
 */
inline void Class_Motor_DJI_GM6020::Set_Target_Current(float __Target_Current)
{
    Target_Current = __Target_Current;
}

/**
 * @brief 设定输出量
 *
 * @param __Out 输出量
 */
inline void Class_Motor_DJI_GM6020::Set_Target_Voltage(float __Voltage)
{
    Target_Voltage = __Voltage;
}

/**
 * @brief 设定前馈的速度, rad/s
 *
 * @param __Feedforward_Omega 前馈的速度, rad/s
 */
inline void Class_Motor_DJI_GM6020::Set_Feedforward_Omega(float __Feedforward_Omega)
{
    Feedforward_Omega = __Feedforward_Omega;
}

/**
 * @brief 设定前馈的电流, A
 *
 * @param __Feedforward_Current 前馈的电流, A
 */
inline void Class_Motor_DJI_GM6020::Set_Feedforward_Current(float __Feedforward_Current)
{
    Feedforward_Current = __Feedforward_Current;
}

/**
 * @brief 设定前馈的电压, V
 *
 * @param __Feedforward_Voltage 前馈的电压, V
 */
inline void Class_Motor_DJI_GM6020::Set_Feedforward_Voltage(float __Feedforward_Voltage)
{
    Feedforward_Voltage = __Feedforward_Voltage;
}

/**
 * @brief 设定功率衰减因数
 *
 * @param __Power_Factor 功率衰减因数
 */
inline void Class_Motor_DJI_GM6020::Set_Power_Factor(float __Power_Factor)
{
    Power_Factor = __Power_Factor;
}

/**
 * @brief 获取最大电流
 *
 * @return float 最大电流
 */
inline float Class_Motor_DJI_C610::Get_Current_Max()
{
    return (Current_Max);
}

/**
 * @brief 获取理论最大输出电流
 *
 * @return float 理论最大输出电流
 */
inline float Class_Motor_DJI_C610::Get_Theoretical_Output_Current_Max()
{
    return (Theoretical_Output_Current_Max);
}

/**
 * @brief 获取电机状态
 *
 * @return Enum_Motor_DJI_Status 电机状态
 */
inline Enum_Motor_DJI_Status Class_Motor_DJI_C610::Get_Status()
{
    return (Motor_DJI_Status);
}

/**
 * @brief 获取当前的角度, rad
 *
 * @return float 当前的角度, rad
 */
inline float Class_Motor_DJI_C610::Get_Now_Angle()
{
    return (Rx_Data.Now_Angle);
}

/**
 * @brief 获取当前的速度, rad/s
 *
 * @return float 当前的速度, rad/s
 */
inline float Class_Motor_DJI_C610::Get_Now_Omega()
{
    return (Rx_Data.Now_Omega);
}

/**
 * @brief 获取当前的电流, A
 *
 * @return 当前的电流, A
 */
inline float Class_Motor_DJI_C610::Get_Now_Current()
{
    return (Rx_Data.Now_Current);
}

/**
 * @brief 获取当前的温度, K
 *
 * @return uint8_t 当前的温度, K
 */
inline uint8_t Class_Motor_DJI_C610::Get_Now_Temperature()
{
    return (Rx_Data.Now_Temperature);
}

/**
 * @brief 获取电机控制方式
 *
 * @return Enum_Motor_DJI_Control_Method 电机控制方式
 */
inline Enum_Motor_DJI_Control_Method Class_Motor_DJI_C610::Get_Control_Method()
{
    return (Motor_DJI_Control_Method);
}

/**
 * @brief 获取目标的角度, rad
 *
 * @return float 目标的角度, rad
 */
inline float Class_Motor_DJI_C610::Get_Target_Angle()
{
    return (Target_Angle);
}

/**
 * @brief 获取目标的速度, rad/s
 *
 * @return float 目标的速度, rad/s
 */
inline float Class_Motor_DJI_C610::Get_Target_Omega()
{
    return (Target_Omega);
}

/**
 * @brief 获取目标的电流
 *
 * @return float 目标的电流
 */
inline float Class_Motor_DJI_C610::Get_Target_Current()
{
    return (Target_Current);
}

/**
 * @brief 获取前馈的速度, rad/s
 *
 * @return float 前馈的速度, rad/s
 */
inline float Class_Motor_DJI_C610::Get_Feedforward_Omega()
{
    return (Feedforward_Omega);
}

/**
 * @brief 获取前馈的电流, A
 *
 * @return float 前馈的电流, A
 */
inline float Class_Motor_DJI_C610::Get_Feedforward_Current()
{
    return (Feedforward_Current);
}

/**
 * @brief 设定电机控制方式
 *
 * @param __Motor_DJI_Control_Method 电机控制方式
 */
inline void Class_Motor_DJI_C610::Set_Control_Method(Enum_Motor_DJI_Control_Method __Motor_DJI_Control_Method)
{
    Motor_DJI_Control_Method = __Motor_DJI_Control_Method;
}

/**
 * @brief 设定目标的角度, rad
 *
 * @param __Target_Angle 目标的角度, rad
 */
inline void Class_Motor_DJI_C610::Set_Target_Angle(float __Target_Angle)
{
    Target_Angle = __Target_Angle;
}

/**
 * @brief 设定目标的速度, rad/s
 *
 * @param __Target_Omega 目标的速度, rad/s
 */
inline void Class_Motor_DJI_C610::Set_Target_Omega(float __Target_Omega)
{
    Target_Omega = __Target_Omega;
}

/**
 * @brief 设定目标的电流, A
 *
 * @param __Target_Current 目标的电流, A
 */
inline void Class_Motor_DJI_C610::Set_Target_Current(float __Target_Current)
{
    Target_Current = __Target_Current;
}

/**
 * @brief 设定前馈的速度, rad/s
 *
 * @param __Feedforward_Omega 前馈的速度, rad/s
 */
inline void Class_Motor_DJI_C610::Set_Feedforward_Omega(float __Feedforward_Omega)
{
    Feedforward_Omega = __Feedforward_Omega;
}

/**
 * @brief 设定前馈的电流, A
 *
 * @param __Feedforward_Current 前馈的电流, A
 */
inline void Class_Motor_DJI_C610::Set_Feedforward_Current(float __Feedforward_Current)
{
    Feedforward_Current = __Feedforward_Current;
}

/**
 * @brief 获取最大电流
 *
 * @return float 最大电流
 */
inline float Class_Motor_DJI_C620::Get_Current_Max()
{
    return (Current_Max);
}

/**
 * @brief 获取理论最大输出电流
 *
 * @return float 理论最大输出电流
 */
inline float Class_Motor_DJI_C620::Get_Theoretical_Output_Current_Max()
{
    return (Theoretical_Output_Current_Max);
}

/**
 * @brief 获取电机状态
 *
 * @return Enum_Motor_DJI_Status 电机状态
 */
inline Enum_Motor_DJI_Status Class_Motor_DJI_C620::Get_Status()
{
    return (Motor_DJI_Status);
}

/**
 * @brief 获取当前的角度, rad
 *
 * @return float 当前的角度, rad
 */
inline float Class_Motor_DJI_C620::Get_Now_Angle()
{
    return (Rx_Data.Now_Angle);
}

/**
 * @brief 获取当前的速度, rad/s
 *
 * @return float 当前的速度, rad/s
 */
inline float Class_Motor_DJI_C620::Get_Now_Omega()
{
    return (Rx_Data.Now_Omega);
}

/**
 * @brief 获取当前的电流, A
 *
 * @return 当前的电流, A
 */
inline float Class_Motor_DJI_C620::Get_Now_Current()
{
    return (Rx_Data.Now_Current);
}

/**
 * @brief 获取当前的温度, K
 *
 * @return float 当前的温度, K
 */
inline float Class_Motor_DJI_C620::Get_Now_Temperature()
{
    return (Rx_Data.Now_Temperature);
}

/**
 * @brief 获取当前的功率, W
 *
 * @return float 当前的功率, W
 */
inline float Class_Motor_DJI_C620::Get_Now_Power()
{
    return (Rx_Data.Now_Power);
}

/**
 * @brief 获取下一时刻的功率估计值, W
 *
 * @return float 下一时刻的功率估计值, W
 */
inline float Class_Motor_DJI_C620::Get_Power_Estimate()
{
    return (Power_Estimate);
}

/**
 * @brief 获取电机控制方式
 *
 * @return Enum_Motor_DJI_Control_Method 电机控制方式
 */
inline Enum_Motor_DJI_Control_Method Class_Motor_DJI_C620::Get_Control_Method()
{
    return (Motor_DJI_Control_Method);
}

/**
 * @brief 获取目标的角度, rad
 *
 * @return float 目标的角度, rad
 */
inline float Class_Motor_DJI_C620::Get_Target_Angle()
{
    return (Target_Angle);
}

/**
 * @brief 获取目标的速度, rad/s
 *
 * @return float 目标的速度, rad/s
 */
inline float Class_Motor_DJI_C620::Get_Target_Omega()
{
    return (Target_Omega);
}

/**
 * @brief 获取目标的电流
 *
 * @return float 目标的电流
 */
inline float Class_Motor_DJI_C620::Get_Target_Current()
{
    return (Target_Current);
}

/**
 * @brief 获取前馈的速度, rad/s
 *
 * @return float 前馈的速度, rad/s
 */
inline float Class_Motor_DJI_C620::Get_Feedforward_Omega()
{
    return (Feedforward_Omega);
}

/**
 * @brief 获取前馈的电流, A
 *
 * @return float 前馈的电流, A
 */
inline float Class_Motor_DJI_C620::Get_Feedforward_Current()
{
    return (Feedforward_Current);
}

/**
 * @brief 设定电机控制方式
 *
 * @param __Motor_DJI_Control_Method 电机控制方式
 */
inline void Class_Motor_DJI_C620::Set_Control_Method(Enum_Motor_DJI_Control_Method __Motor_DJI_Control_Method)
{
    Motor_DJI_Control_Method = __Motor_DJI_Control_Method;
}

/**
 * @brief 设定目标的角度, rad
 *
 * @param __Target_Angle 目标的角度, rad
 */
inline void Class_Motor_DJI_C620::Set_Target_Angle(float __Target_Angle)
{
    Target_Angle = __Target_Angle;
}

/**
 * @brief 设定目标的速度, rad/s
 *
 * @param __Target_Omega 目标的速度, rad/s
 */
inline void Class_Motor_DJI_C620::Set_Target_Omega(float __Target_Omega)
{
    Target_Omega = __Target_Omega;
}

/**
 * @brief 设定目标的电流
 *
 * @param __Target_Current 目标的电流
 */
inline void Class_Motor_DJI_C620::Set_Target_Current(float __Target_Current)
{
    Target_Current = __Target_Current;
}

/**
 * @brief 设定前馈的速度, rad/s
 *
 * @param __Feedforward_Omega 前馈的速度, rad/s
 */
inline void Class_Motor_DJI_C620::Set_Feedforward_Omega(float __Feedforward_Omega)
{
    Feedforward_Omega = __Feedforward_Omega;
}

/**
 * @brief 设定前馈的电流, A
 *
 * @param __Feedforward_Current 前馈的电流, A
 */
inline void Class_Motor_DJI_C620::Set_Feedforward_Current(float __Feedforward_Current)
{
    Feedforward_Current = __Feedforward_Current;
}

/**
 * @brief 设定功率衰减因数
 *
 * @param __Power_Factor 功率衰减因数
 */
inline void Class_Motor_DJI_C620::Set_Power_Factor(float __Power_Factor)
{
    Power_Factor = __Power_Factor;
}

#endif

/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/
