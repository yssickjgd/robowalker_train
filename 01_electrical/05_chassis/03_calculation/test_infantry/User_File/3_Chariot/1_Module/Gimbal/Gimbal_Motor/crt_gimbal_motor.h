/**
 * @file crt_gimbal_motor.h
 * @author yssickjgd (1345578933@qq.com)
 * @brief 云台电机
 * @version 0.1
 * @date 2024-01-30 新增
 *
 * @copyright USTC-RoboWalker (c) 2024
 *
 */

#ifndef CRT_GIMBAL_MOTOR_H
#define CRT_GIMBAL_MOTOR_H

/* Includes ------------------------------------------------------------------*/

#include "2_Device/AHRS/AHRS_WHEELTEC/dvc_ahrs_wheeltec.h"
#include "2_Device/AHRS/AHRS_WIT/dvc_ahrs_wit.h"
#include "2_Device/Motor/Motor_DJI/dvc_motor_dji.h"
#include "1_Middleware/2_Algorithm/Filter/alg_filter.h"

/* Exported macros -----------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

class Class_Posture;

class Class_Gimbal_Pitch_Motor_DJI_GM6020;

/**
 * @brief Specialized, yaw轴电机
 *
 */
class Class_Gimbal_Yaw_Motor_DJI_GM6020 : public Class_Motor_DJI_GM6020
{
public:

    // 云台陀螺仪
    Class_AHRS_WIT *AHRS_Gimbal;

    // 底盘陀螺仪
    Class_AHRS_WHEELTEC *AHRS_Chassis;

    Class_Gimbal_Pitch_Motor_DJI_GM6020 *Motor_Pitch;

    // 陀螺仪角度环PID
    Class_PID PID_AHRS_Angle;

    // 陀螺仪角速度环PID
    Class_PID PID_AHRS_Omega;

    inline float Get_Now_AHRS_Omega();

    void TIM_100ms_Alive_PeriodElapsedCallback();

    void TIM_Calculate_PeriodElapsedCallback();

protected:
    // 初始化相关常量

    // 常量

    // 内部变量

    // 读变量

    // 写变量

    // 读写变量

    // 内部函数

    void PID_Calculate();

};

/**
 * @brief Specialized, pitch轴电机
 *
 */
class Class_Gimbal_Pitch_Motor_DJI_GM6020 : public Class_Motor_DJI_GM6020
{
public:
    // 云台陀螺仪
    Class_AHRS_WIT *AHRS_Gimbal;

    // 陀螺仪角度环PID
    Class_PID PID_AHRS_Angle;

    // 陀螺仪角速度环PID
    Class_PID PID_AHRS_Omega;

    inline float Get_Now_AHRS_Omega();

    void TIM_100ms_Alive_PeriodElapsedCallback();

    void TIM_Calculate_PeriodElapsedCallback();

protected:
    // 初始化相关常量

    // 常量

    // 内部变量

    // 读变量

    // 写变量

    // 读写变量

    // 内部函数

    void PID_Calculate();

};

/* Exported variables --------------------------------------------------------*/

/* Exported function declarations --------------------------------------------*/

/**
 * @brief 获取当前的速度, 优先使用陀螺仪数据, rad/s
 *
 * @return float 当前的速度, 优先使用陀螺仪数据, rad/s
 */
inline float Class_Gimbal_Yaw_Motor_DJI_GM6020::Get_Now_AHRS_Omega()
{
    if (AHRS_Gimbal->Get_Status() == AHRS_WIT_Status_ENABLE && AHRS_Chassis->Get_Status() == AHRS_WHEELTEC_Status_ENABLE)
    {
        return (AHRS_Gimbal->Get_Omega_Z() * arm_cos_f32(-Motor_Pitch->Get_Now_Angle()) - AHRS_Gimbal->Get_Omega_X() * arm_sin_f32(-Motor_Pitch->Get_Now_Angle()) + AHRS_Chassis->Get_Omega_Z());
    }
    else
    {
        return (Rx_Data.Now_Omega);
    }
}

/**
 * @brief 获取当前的速度, 优先使用陀螺仪数据, rad/s
 *
 * @return float 当前的速度, 优先使用陀螺仪数据, rad/s
 */
inline float Class_Gimbal_Pitch_Motor_DJI_GM6020::Get_Now_AHRS_Omega()
{
    if (AHRS_Gimbal->Get_Status() == AHRS_WIT_Status_ENABLE)
    {
        return (-AHRS_Gimbal->Get_Omega_Y());
    }
    else
    {
        return (Rx_Data.Now_Omega);
    }
}

#endif

/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/
