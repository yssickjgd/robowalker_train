/**
 * @file crt_chassis.h
 * @author yssickjgd (1345578933@qq.com)
 * @brief 底盘电控
 * @version 0.1
 * @date 2023-08-29 0.1 23赛季定稿
 * @date 2023-11-19 1.1 修改为四轮底盘
 * @date 2024-01-17 1.2 新增速度规划等算法, 将各个算法解耦合
 *
 * @copyright USTC-RoboWalker (c) 2023-2024
 *
 */

/**
 * @brief 轮组编号
 * 3[0] 2[3]
 * 4[1] 1[2]
 * 前x左y上z
 */

#ifndef CRT_CHASSIS_H
#define CRT_CHASSIS_H

/* Includes ------------------------------------------------------------------*/

#include "2_Device/AHRS/AHRS_WHEELTEC/dvc_ahrs_wheeltec.h"
#include "2_Device/Motor/Motor_DJI/dvc_motor_dji.h"
#include "2_Device/Referee/dvc_referee.h"
#include "1_Middleware/2_Algorithm/Slope/alg_slope.h"

/* Exported macros -----------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/**
 * @brief 底盘控制类型
 *
 */
enum Enum_Chassis_Control_Type
{
    Chassis_Control_Type_DISABLE = 0,
    Chassis_Control_Type_NORMAL,
};

/**
 * @brief Specialized, 舵轮底盘类
 *
 */
class Class_Chassis
{
public:

    // 底盘速度值PID
    Class_PID PID_Velocity_X;

    // 底盘速度方向PID
    Class_PID PID_Velocity_Y;

    // 底盘角速度PID
    Class_PID PID_Omega;

    // 底盘陀螺仪
    Class_AHRS_WHEELTEC *AHRS_Chassis;

    // 舵向电机
    Class_Motor_DJI_GM6020 Motor_Steer[4];

    // 轮向电机
    Class_Motor_DJI_C620 Motor_Wheel[4];

    void Init();

    inline float Get_Now_Motor_Power();

    inline float Get_Now_Steer_Motor_Power();

    inline float Get_Now_Wheel_Motor_Power();

    inline float Get_Steer_Factor();

    inline float Get_Wheel_Factor();

    inline float Get_Now_Velocity_X();

    inline float Get_Now_Velocity_Y();

    inline float Get_Now_Omega();

    inline float Get_Now_AHRS_Omega();

    inline float Get_Angle_Pitch();

    inline float Get_Angle_Roll();

    inline float Get_Slope_Direction_X();

    inline float Get_Slope_Direction_Y();

    inline float Get_Slope_Direction_Z();

    inline Enum_Chassis_Control_Type Get_Chassis_Control_Type();

    inline float Get_Target_Velocity_X();

    inline float Get_Target_Velocity_Y();

    inline float Get_Target_Omega();

    inline void Set_Power_Limit_Max(float __Power_Limit_Max);

    inline void Set_Chassis_Control_Type(Enum_Chassis_Control_Type __Chassis_Control_Type);

    inline void Set_Target_Velocity_X(float __Target_Velocity_X);

    inline void Set_Target_Velocity_Y(float __Target_Velocity_Y);

    inline void Set_Target_Omega(float __Target_Omega);

    void TIM_100ms_Alive_PeriodElapsedCallback();

    void TIM_2ms_Resolution_PeriodElapsedCallback();

    void TIM_2ms_Control_PeriodElapsedCallback();

protected:
    // 初始化相关常量

    // 常量

    // 轮组半径
    const float Wheel_Radius = 0.058f;
    // 轮距中心长度
    const float Wheel_To_Core_Distance[4] = {0.207f,
                                             0.207f,
                                             0.207f,
                                             0.207f,};
    // 轮组方位角
    const float Wheel_Azimuth[4] = {PI / 4.0f,
                                    3.0f * PI / 4.0f,
                                    5.0f * PI / 4.0f,
                                    7.0f * PI / 4.0f,};

    // 重心高度
    const float Center_Height = 0.15f;

    // 内部变量

    // 舵向电机角度目标值
    float Target_Steer_Angle[4];
    // 轮向电机角速度目标值
    float Target_Wheel_Omega[4];
    // 轮向电机电流目标值
    float Target_Wheel_Current[4];
    // 轮向电机静摩擦阻力电流值
    float Static_Resistance_Wheel_Current[4] = {0.0f,
                                                0.0f,
                                                0.0f,
                                                0.0f};
    // 轮向电机动摩擦阻力电流值
    float Dynamic_Resistance_Wheel_Current[4] = {0.0f,
                                                 0.0f,
                                                 0.0f,
                                                 0.0f};
    // 轮向电机摩擦阻力连续化的角速度阈值
    float Wheel_Resistance_Omega_Threshold = 1.0f;
    // 防单轮超速系数
    float Wheel_Speed_Limit_Factor = 0.5f;

    // 读变量

    // 功率
    float Now_Motor_Power = 0.0f;
    // 舵向电机功率
    float Now_Steer_Motor_Power = 0.0f;
    // 轮向电机功率
    float Now_Wheel_Motor_Power = 0.0f;

    // 舵向电机功率因数
    float Steer_Factor = 0.0f;
    // 轮向电机功率因数
    float Wheel_Factor = 0.0f;

    // 当前速度X
    float Now_Velocity_X = 0.0f;
    // 当前速度Y
    float Now_Velocity_Y = 0.0f;
    // 当前角速度
    float Now_Omega = 0.0f;

    // 底盘相对Odom角度
    float Angle_Pitch = 0.0f;
    float Angle_Roll = 0.0f;

    // 斜坡法向量在底盘方向向量
    float Slope_Direction_X = 0.0f;
    float Slope_Direction_Y = 0.0f;
    float Slope_Direction_Z = 1.0f;

    // 写变量

    // 功率限制上限
    float Power_Limit_Max = 45.0f;

    // 读写变量

    // 底盘控制方法
    Enum_Chassis_Control_Type Chassis_Control_Type = Chassis_Control_Type_DISABLE;

    // 目标速度X
    float Target_Velocity_X = 0.0f;
    // 目标速度Y
    float Target_Velocity_Y = 0.0f;
    // 目标角速度
    float Target_Omega = 0.0f;

    // 内部函数

    void Self_Resolution();

    void Kinematics_Inverse_Resolution();

    void _Steer_Motor_Kinematics_Nearest_Transposition();

    void Output_To_Dynamics();

    void Dynamics_Inverse_Resolution();

    void Output_To_Motor();

    void _Power_Limit_Control();

    float __Steer_Power_Limit_Control(float steer_available_power, float steer_consume_power);

    float __Wheel_Power_Limit_Control(float wheel_available_power, float wheel_consume_power);
};

/* Exported variables --------------------------------------------------------*/

/* Exported function declarations --------------------------------------------*/

/**
 * @brief 获取功率
 *
 * @return float 功率
 */
inline float Class_Chassis::Get_Now_Motor_Power()
{
    return (Now_Motor_Power);
}

/**
 * @brief 获取舵向电机功率
 *
 * @return float 舵向电机功率
 */
inline float Class_Chassis::Get_Now_Steer_Motor_Power()
{
    return (Now_Steer_Motor_Power);
}

/**
 * @brief 获取轮向电机功率
 *
 * @return float 轮向电机功率
 */
inline float Class_Chassis::Get_Now_Wheel_Motor_Power()
{
    return (Now_Wheel_Motor_Power);
}

/**
 * @brief 获取舵向电机功率因数
 *
 * @return float 舵向电机功率因数
 */
inline float Class_Chassis::Get_Steer_Factor()
{
    return (Steer_Factor);
}

/**
 * @brief 获取轮向电机功率因数
 *
 * @return float 轮向电机功率因数
 */
inline float Class_Chassis::Get_Wheel_Factor()
{
    return (Wheel_Factor);
}

/**
 * @brief 获取当前速度X
 *
 * @return float 当前速度X
 */
inline float Class_Chassis::Get_Now_Velocity_X()
{
    return (Now_Velocity_X);
}

/**
 * @brief 获取当前速度Y
 *
 * @return float 当前速度Y
 */
inline float Class_Chassis::Get_Now_Velocity_Y()
{
    return (Now_Velocity_Y);
}

/**
 * @brief 获取当前角速度
 *
 * @return float 当前角速度
 */
inline float Class_Chassis::Get_Now_Omega()
{
    return (Now_Omega);
}

/**
 * @brief 获取当前角速度, 优先使用陀螺仪数据, rad/s
 *
 * @return float 当前角速度, 优先使用陀螺仪数据, rad/s
 */
inline float Class_Chassis::Get_Now_AHRS_Omega()
{
    if (AHRS_Chassis->Get_Status() == AHRS_WHEELTEC_Status_ENABLE)
    {
        return (-AHRS_Chassis->Get_Omega_Z());
    }
    else
    {
        return (Now_Omega);
    }
}

/**
 * @brief 获取底盘相对Odom角度
 *
 * @return float 底盘相对Odom角度
 */
inline float Class_Chassis::Get_Angle_Pitch()
{
    return (Angle_Pitch);
}

/**
 * @brief 获取底盘相对Odom角度
 *
 * @return float 底盘相对Odom角度
 */
inline float Class_Chassis::Get_Angle_Roll()
{
    return (Angle_Roll);
}

/**
 * @brief 获取斜坡法向量在底盘方向向量
 *
 * @return float 斜坡法向量在底盘方向向量
 */
inline float Class_Chassis::Get_Slope_Direction_X()
{
    return (Slope_Direction_X);
}

/**
 * @brief 获取斜坡法向量在底盘方向向量
 *
 * @return float 斜坡法向量在底盘方向向量
 */
inline float Class_Chassis::Get_Slope_Direction_Y()
{
    return (Slope_Direction_Y);
}

/**
 * @brief 获取斜坡法向量在底盘方向向量
 *
 * @return float 斜坡法向量在底盘方向向量
 */
inline float Class_Chassis::Get_Slope_Direction_Z()
{
    return (Slope_Direction_Z);
}

/**
 * @brief 获取底盘控制方法
 *
 * @return Enum_Chassis_Control_Type 底盘控制方法
 */
inline Enum_Chassis_Control_Type Class_Chassis::Get_Chassis_Control_Type()
{
    return (Chassis_Control_Type);
}

/**
 * @brief 获取目标速度X
 *
 * @return float 目标速度X
 */
inline float Class_Chassis::Get_Target_Velocity_X()
{
    return (Target_Velocity_X);
}

/**
 * @brief 获取目标速度Y
 *
 * @return float 目标速度Y
 */
inline float Class_Chassis::Get_Target_Velocity_Y()
{
    return (Target_Velocity_Y);
}

/**
 * @brief 获取目标速度方向
 *
 * @return float 目标速度方向
 */
inline float Class_Chassis::Get_Target_Omega()
{
    return (Target_Omega);
}

/**
 * @brief 设定功率控制上限
 *
 * @return __Power_Limit_Max 功率控制上限
 */
inline void Class_Chassis::Set_Power_Limit_Max(float __Power_Limit_Max)
{
    Power_Limit_Max = __Power_Limit_Max;
}

/**
 * @brief 设定底盘控制方法
 *
 * @param __Chassis_Control_Type 底盘控制方法
 */
inline void Class_Chassis::Set_Chassis_Control_Type(Enum_Chassis_Control_Type __Chassis_Control_Type)
{
    Chassis_Control_Type = __Chassis_Control_Type;
}

/**
 * @brief 设定目标速度X
 *
 * @param __Target_Velocity_X 目标速度X
 */
inline void Class_Chassis::Set_Target_Velocity_X(float __Target_Velocity_X)
{
    Target_Velocity_X = __Target_Velocity_X;
}

/**
 * @brief 设定目标速度Y
 *
 * @param __Target_Velocity_Y 目标速度Y
 */
inline void Class_Chassis::Set_Target_Velocity_Y(float __Target_Velocity_Y)
{
    Target_Velocity_Y = __Target_Velocity_Y;
}

/**
 * @brief 设定目标角速度
 *
 * @param __Target_Omega 目标角速度
 */
inline void Class_Chassis::Set_Target_Omega(float __Target_Omega)
{
    Target_Omega = __Target_Omega;
}

#endif

/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/
