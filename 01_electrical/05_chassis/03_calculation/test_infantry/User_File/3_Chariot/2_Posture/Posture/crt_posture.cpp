/**
 * @file crt_posture.cpp
 * @author yssickjgd (1345578933@qq.com)
 * @brief 战车姿态感知器
 * @version 0.1
 * @date 2024-06-13 0.1 新建文档
 *
 * @copyright USTC-RoboWalker (c) 2023-2024
 *
 */

/* Includes ------------------------------------------------------------------*/

#include "crt_posture.h"

/* Private macros ------------------------------------------------------------*/

/* Private types -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function declarations ---------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/

/**
 * @brief 姿态感知器初始化
 *
 */
void Class_Posture::Init()
{
    AHRS_Chassis.Init(&huart7);
    AHRS_Gimbal.Init(&huart8);
}

/**
 * @brief TIM定时器中断定期检测模块是否存活
 *
 */
void Class_Posture::TIM_100ms_Alive_PeriodElapsedCallback()
{
    AHRS_Chassis.TIM_100ms_Alive_PeriodElapsedCallback();
    AHRS_Gimbal.TIM_100ms_Alive_PeriodElapsedCallback();
}

/**
 * @brief TIM定时器中断定期检测电机是否存活
 *
 */
void Class_Posture::TIM_1ms_Calculate_PeriodElapsedCallback()
{
    Chassis_Velocity_X = Chassis->Get_Now_Velocity_X();
    Chassis_Velocity_Y = Chassis->Get_Now_Velocity_Y();
    Chassis_Omega = Chassis->Get_Now_AHRS_Omega();

    float gimbal_yaw, tmp_delta_gimbal_yaw;
    tmp_delta_gimbal_yaw = Math_Modulus_Normalization(AHRS_Gimbal.Get_Angle_Yaw() - Gimbal_Odom_Angle_Yaw, 2.0f * PI);
    gimbal_yaw = Gimbal_Odom_Angle_Yaw + tmp_delta_gimbal_yaw;

    Chassis_Odom_Angle_Yaw = gimbal_yaw - Gimbal->Get_Now_Yaw_Angle();
    Chassis_Odom_Angle_Pitch = Chassis->Get_Angle_Pitch();
    Chassis_Odom_Angle_Roll = Chassis->Get_Angle_Roll();

    float cos_gimbal_yaw, sin_gimbal_yaw;
    cos_gimbal_yaw = arm_cos_f32(-Gimbal->Get_Now_Yaw_Angle());
    sin_gimbal_yaw = arm_sin_f32(-Gimbal->Get_Now_Yaw_Angle());
    Gimbal_Velocity_X = Chassis_Velocity_X * cos_gimbal_yaw + Chassis_Velocity_Y * (-sin_gimbal_yaw);
    Gimbal_Velocity_Y = Chassis_Velocity_X * sin_gimbal_yaw + Chassis_Velocity_Y * cos_gimbal_yaw;

    Gimbal_Omega_Yaw = Gimbal->Get_Now_Yaw_Omega();
    Gimbal_Omega_Pitch = Gimbal->Get_Now_Pitch_Omega();

    Gimbal_Odom_Angle_Yaw = gimbal_yaw;
    Gimbal_Odom_Angle_Pitch = AHRS_Gimbal.Get_Angle_Pitch();
    Gimbal_Odom_Angle_Roll = AHRS_Gimbal.Get_Angle_Roll();

    // Matrix_Chassis_Odom_Rotation = Eigen::AngleAxisf(Chassis_Odom_Angle_Yaw, Eigen::Vector3f::UnitZ()) * Eigen::AngleAxisf(Chassis_Odom_Angle_Pitch, Eigen::Vector3f::UnitY()) * Eigen::AngleAxisf(Chassis_Odom_Angle_Roll, Eigen::Vector3f::UnitX());
    //
    // Matrix_Gimbal_Odom_Rotation = Eigen::AngleAxisf(Gimbal_Odom_Angle_Yaw, Eigen::Vector3f::UnitZ()) * Eigen::AngleAxisf(Gimbal_Odom_Angle_Pitch, Eigen::Vector3f::UnitY()) * Eigen::AngleAxisf(Gimbal_Odom_Angle_Roll, Eigen::Vector3f::UnitX());
    //
    // Matrix_Gimbal_Chassis_Rotation = Eigen::AngleAxisf(Gimbal->Get_Now_Yaw_Angle(), Eigen::Vector3f::UnitZ()) * Eigen::AngleAxisf(Gimbal->Get_Now_Pitch_Angle(), Eigen::Vector3f::UnitY());
}

/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/
