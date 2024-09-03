/**
 * @file crt_gimbal.h
 * @author yssickjgd (1345578933@qq.com)
 * @brief 云台电控
 * @version 0.1
 * @date 2023-08-29 0.1 23赛季定稿
 *
 * @copyright USTC-RoboWalker (c) 2023
 *
 */

/**
 * @brief 云台陀螺仪对应关系
 * x pitch
 * z yaw
 */

#ifndef CRT_GIMBAL_H
#define CRT_GIMBAL_H

/* Includes ------------------------------------------------------------------*/

#include "3_Chariot/1_Module/Gimbal/Gimbal_Motor/crt_gimbal_motor.h"
#include "2_Device/AHRS/AHRS_WHEELTEC/dvc_ahrs_wheeltec.h"
#include "2_Device/AHRS/AHRS_WIT/dvc_ahrs_wit.h"

/* Exported macros -----------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/**
 * @brief 云台控制类型
 *
 */
enum Enum_Gimbal_Control_Type
{
    Gimbal_Control_Type_DISABLE = 0,
    Gimbal_Control_Type_ANGLE,
};

/**
 * @brief Specialized, 云台类
 *
 */
class Class_Gimbal
{
public:

    // 云台陀螺仪
    Class_AHRS_WIT *AHRS_Gimbal;

    // 底盘陀螺仪
    Class_AHRS_WHEELTEC *AHRS_Chassis;

    // yaw轴电机
    Class_Gimbal_Yaw_Motor_DJI_GM6020 Motor_Yaw;

    // pitch轴电机
    Class_Gimbal_Pitch_Motor_DJI_GM6020 Motor_Pitch;

    void Init();

    inline float Get_Now_Yaw_Angle();

    inline float Get_Now_Pitch_Angle();

    inline float Get_Now_Yaw_Omega();

    inline float Get_Now_Pitch_Omega();

    inline float Get_Target_Yaw_Angle();

    inline float Get_Target_Pitch_Angle();

    inline float Get_Target_Yaw_Omega();

    inline float Get_Target_Pitch_Omega();

    inline void Set_Gimbal_Control_Type(Enum_Gimbal_Control_Type __Gimbal_Control_Type);

    inline void Set_Target_Yaw_Angle(float __Target_Yaw_Angle);

    inline void Set_Target_Pitch_Angle(float __Target_Pitch_Angle);

    inline void Set_Target_Yaw_Omega(float __Target_Yaw_Omega);

    inline void Set_Target_Pitch_Omega(float __Target_Pitch_Omega);

    void TIM_100ms_Alive_PeriodElapsedCallback();

    void TIM_1ms_Resolution_PeriodElapsedCallback();

    void TIM_1ms_Control_PeriodElapsedCallback();

protected:
    // 初始化相关常量

    // 常量

    // pitch轴最小值
    float Min_Pitch_Angle = -0.60f;
    // pitch轴最大值
    float Max_Pitch_Angle = 0.33f;

    // 内部变量

    // 读变量

    // yaw轴当前角度
    float Now_Yaw_Angle = 0.0f;
    // pitch轴当前角度
    float Now_Pitch_Angle = 0.0f;

    // yaw轴当前角速度
    float Now_Yaw_Omega = 0.0f;
    // pitch轴当前角速度
    float Now_Pitch_Omega = 0.0f;

    // 写变量

    // 云台状态
    Enum_Gimbal_Control_Type Gimbal_Control_Type = Gimbal_Control_Type_DISABLE;

    // 读写变量

    // yaw轴目标角度
    float Target_Yaw_Angle = 0.0f;
    // pitch轴目标角度
    float Target_Pitch_Angle = 0.0f;

    // yaw轴目标角速度
    float Target_Yaw_Omega = 0.0f;
    // pitch轴目标角速度
    float Target_Pitch_Omega = 0.0f;

    // 内部函数

    void Self_Resolution();

    void Output();

    void _Motor_Nearest_Transposition();
};

/* Exported variables --------------------------------------------------------*/

/* Exported function declarations --------------------------------------------*/

/**
 * @brief 获取yaw轴当前角度
 *
 * @return float yaw轴当前角度
 */
inline float Class_Gimbal::Get_Now_Yaw_Angle()
{
    return (Now_Yaw_Angle);
}

/**
 * @brief 获取pitch轴当前角度
 *
 * @return float pitch轴当前角度
 */
inline float Class_Gimbal::Get_Now_Pitch_Angle()
{
    return (Now_Pitch_Angle);
}

/**
 * @brief 获取yaw轴当前角速度
 *
 * @return float yaw轴当前角速度
 */
inline float Class_Gimbal::Get_Now_Yaw_Omega()
{
    return (Now_Yaw_Omega);
}

/**
 * @brief 获取pitch轴当前角速度
 *
 * @return float pitch轴当前角速度
 */
inline float Class_Gimbal::Get_Now_Pitch_Omega()
{
    return (Now_Pitch_Omega);
}

/**
 * @brief 获取yaw轴目标角度
 *
 * @return float yaw轴目标角度
 */
inline float Class_Gimbal::Get_Target_Yaw_Angle()
{
    return (Target_Yaw_Angle);
}

/**
 * @brief 获取pitch轴目标角度
 *
 * @return float pitch轴目标角度
 */
inline float Class_Gimbal::Get_Target_Pitch_Angle()
{
    return (Target_Pitch_Angle);
}

/**
 * @brief 获取yaw轴目标角速度
 *
 * @return float yaw轴目标角速度
 */
inline float Class_Gimbal::Get_Target_Yaw_Omega()
{
    return (Target_Yaw_Omega);
}

/**
 * @brief 获取pitch轴目标角速度
 *
 * @return float pitch轴目标角速度
 */
inline float Class_Gimbal::Get_Target_Pitch_Omega()
{
    return (Target_Pitch_Omega);
}

/**
 * @brief 设定云台状态
 *
 * @param __Gimbal_Control_Type 云台状态
 */
inline void Class_Gimbal::Set_Gimbal_Control_Type(Enum_Gimbal_Control_Type __Gimbal_Control_Type)
{
    Gimbal_Control_Type = __Gimbal_Control_Type;
}

/**
 * @brief 设定yaw轴角度
 *
 * @param __Target_Yaw_Angle yaw轴角度
 */
inline void Class_Gimbal::Set_Target_Yaw_Angle(float __Target_Yaw_Angle)
{
    Target_Yaw_Angle = __Target_Yaw_Angle;
}

/**
 * @brief 设定pitch轴角度
 *
 * @param __Target_Pitch_Angle pitch轴角度
 */
inline void Class_Gimbal::Set_Target_Pitch_Angle(float __Target_Pitch_Angle)
{
    Target_Pitch_Angle = __Target_Pitch_Angle;
}

/**
 * @brief 设定yaw轴角速度
 *
 * @param __Target_Yaw_Omega yaw轴角速度
 */
inline void Class_Gimbal::Set_Target_Yaw_Omega(float __Target_Yaw_Omega)
{
    Target_Yaw_Angle = __Target_Yaw_Omega;
}

/**
 * @brief 设定pitch轴角速度
 *
 * @param __Target_Pitch_Omega pitch轴角速度
 */
inline void Class_Gimbal::Set_Target_Pitch_Omega(float __Target_Pitch_Omega)
{
    Target_Pitch_Angle = __Target_Pitch_Omega;
}

#endif

/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/
