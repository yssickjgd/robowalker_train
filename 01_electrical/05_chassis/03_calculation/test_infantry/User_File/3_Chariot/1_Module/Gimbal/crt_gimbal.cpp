/**
 * @file crt_gimbal.cpp
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

/* Includes ------------------------------------------------------------------*/

#include "crt_gimbal.h"

/* Private macros ------------------------------------------------------------*/

/* Private types -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function declarations ---------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/

/**
 * @brief 云台初始化
 *
 */
void Class_Gimbal::Init()
{
    // yaw轴电机初始化

    // PID初始化
    Motor_Yaw.PID_Angle.Init(7.0f, 0.0f, 0.0f, 0.0f, 2.0f * PI, 2.0f * PI);
    Motor_Yaw.PID_Omega.Init(0.366f, 0.916f, 0.0f, 0.0f, 3.0f, 3.0f);
    Motor_Yaw.PID_AHRS_Angle.Init(6.0f, 0.0f, 0.00f, 0.0f, 2.0f * PI, 2.0f * PI);
    Motor_Yaw.PID_AHRS_Omega.Init(1.5f, 30.0f, 0.0f, 0.0f, 0.7f, 3.0f);
    // 绑定陀螺仪
    Motor_Yaw.AHRS_Gimbal = AHRS_Gimbal;
    Motor_Yaw.AHRS_Chassis = AHRS_Chassis;
    Motor_Yaw.Motor_Pitch = &Motor_Pitch;
    // 电机初始化
    Motor_Yaw.Init(&hcan1, Motor_DJI_ID_0x205, Motor_DJI_Control_Method_ANGLE, 3424, Motor_DJI_GM6020_Driver_Version_2023);

    // pitch轴电机初始化

    // PID初始化
    Motor_Pitch.PID_Angle.Init(7.0f, 0.0f, 0.0f, 0.0f, 6.0f * PI, 6.0f * PI);
    Motor_Pitch.PID_Omega.Init(0.366f, 0.916f, 0.0f, 0.0f, 3.0f, 3.0f);
    Motor_Pitch.PID_AHRS_Angle.Init(10.0f, 0.0f, 0.000f, 0.0f, 6.0f * PI, 6.0f * PI);
    Motor_Pitch.PID_AHRS_Omega.Init(1.6f, 40.0f, 0.00000f, 0.0f, 1.5f, 3.0f);
    // 绑定陀螺仪
    Motor_Pitch.AHRS_Gimbal = AHRS_Gimbal;
    // 电机初始化
    Motor_Pitch.Init(&hcan1, Motor_DJI_ID_0x206, Motor_DJI_Control_Method_ANGLE, -3111, Motor_DJI_GM6020_Driver_Version_2023);
}

/**
 * @brief TIM定时器中断定期检测电机是否存活
 *
 */
void Class_Gimbal::TIM_100ms_Alive_PeriodElapsedCallback()
{
    Motor_Yaw.TIM_100ms_Alive_PeriodElapsedCallback();
    Motor_Pitch.TIM_100ms_Alive_PeriodElapsedCallback();
}

/**
 * @brief TIM定时器中断解算回调函数
 *
 */
void Class_Gimbal::TIM_1ms_Resolution_PeriodElapsedCallback()
{
    Self_Resolution();
}

/**
 * @brief TIM定时器中断控制回调函数
 *
 */
void Class_Gimbal::TIM_1ms_Control_PeriodElapsedCallback()
{
    Output();

    Motor_Yaw.TIM_Calculate_PeriodElapsedCallback();
    Motor_Pitch.TIM_Calculate_PeriodElapsedCallback();
}

/**
 * @brief 自身解算
 *
 */
void Class_Gimbal::Self_Resolution()
{
    Now_Yaw_Angle = Motor_Yaw.Get_Now_Angle();

    // pitch轴角度归化到±PI / 2之间
    Now_Pitch_Angle = Math_Modulus_Normalization(-Motor_Pitch.Get_Now_Angle(), 2.0f * PI);

    Now_Yaw_Omega = Motor_Yaw.Get_Now_AHRS_Omega();

    Now_Pitch_Omega = -Motor_Pitch.Get_Now_AHRS_Omega();
}

/**
 * @brief 输出到电机
 *
 */
void Class_Gimbal::Output()
{
    if (Gimbal_Control_Type == Gimbal_Control_Type_DISABLE)
    {
        // 云台失能
        Motor_Yaw.Set_Control_Method(Motor_DJI_Control_Method_CURRENT);
        Motor_Pitch.Set_Control_Method(Motor_DJI_Control_Method_CURRENT);

        Motor_Yaw.PID_Angle.Set_Integral_Error(0.0f);
        Motor_Yaw.PID_Omega.Set_Integral_Error(0.0f);
        Motor_Yaw.PID_Current.Set_Integral_Error(0.0f);
        Motor_Yaw.PID_AHRS_Angle.Set_Integral_Error(0.0f);
        Motor_Yaw.PID_AHRS_Omega.Set_Integral_Error(0.0f);
        Motor_Pitch.PID_Angle.Set_Integral_Error(0.0f);
        Motor_Pitch.PID_Omega.Set_Integral_Error(0.0f);
        Motor_Pitch.PID_Current.Set_Integral_Error(0.0f);
        Motor_Pitch.PID_AHRS_Angle.Set_Integral_Error(0.0f);
        Motor_Pitch.PID_AHRS_Omega.Set_Integral_Error(0.0f);

        Motor_Yaw.Set_Target_Current(0.0f);
        Motor_Yaw.Set_Feedforward_Omega(0.0f);
        Motor_Pitch.Set_Target_Current(0.0f);
        Motor_Pitch.Set_Feedforward_Current(0.0f);
    }
    else if (Gimbal_Control_Type == Gimbal_Control_Type_ANGLE)
    {
        // 云台位控
        Motor_Yaw.Set_Control_Method(Motor_DJI_Control_Method_ANGLE);
        Motor_Pitch.Set_Control_Method(Motor_DJI_Control_Method_ANGLE);

        _Motor_Nearest_Transposition();

        Motor_Yaw.Set_Target_Angle(Target_Yaw_Angle);
        Motor_Pitch.Set_Target_Angle(-Target_Pitch_Angle);
    }
}

/**
 * @brief 电机就近转位
 *
 */
void Class_Gimbal::_Motor_Nearest_Transposition()
{
    // Yaw就近转位
    float tmp_delta_angle;
    tmp_delta_angle = fmod(Target_Yaw_Angle - Now_Yaw_Angle, 2.0f * PI);
    if (tmp_delta_angle > PI)
    {
        tmp_delta_angle -= 2.0f * PI;
    }
    else if (tmp_delta_angle < -PI)
    {
        tmp_delta_angle += 2.0f * PI;
    }
    Target_Yaw_Angle = Motor_Yaw.Get_Now_Angle() + tmp_delta_angle;

    // Pitch就近转位
    Math_Constrain(&Target_Pitch_Angle, Min_Pitch_Angle, Max_Pitch_Angle);
    tmp_delta_angle = Target_Pitch_Angle - Now_Pitch_Angle;
    Target_Pitch_Angle = -Motor_Pitch.Get_Now_Angle() + tmp_delta_angle;
}

/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/
