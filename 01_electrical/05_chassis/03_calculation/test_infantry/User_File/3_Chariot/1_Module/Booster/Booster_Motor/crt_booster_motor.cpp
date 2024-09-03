/**
 * @file crt_booster_motor.cpp
 * @author yssickjgd (1345578933@qq.com)
 * @brief 云台电机
 * @version 0.1
 * @date 2024-07-01 新增
 *
 * @copyright USTC-RoboWalker (c) 2024
 *
 */

/**
 * @brief 云台陀螺仪对应关系
 * x pitch
 * z yaw
 */

/* Includes ------------------------------------------------------------------*/

#include "crt_booster_motor.h"

/* Private macros ------------------------------------------------------------*/

/* Private types -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function declarations ---------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/

/**
 * @brief TIM定时器中断计算回调函数, 计算周期取决于电机反馈周期
 *
 */
void Class_Booster_Friction_Motor_DJI_C620::TIM_Calculate_PeriodElapsedCallback()
{
    Filter_Fourier_Omega.Set_Now(Rx_Data.Now_Omega);
    Filter_Fourier_Omega.TIM_Calculate_PeriodElapsedCallback();

    PID_Calculate();

    float tmp_value = Target_Current + Feedforward_Current;
    Math_Constrain(&tmp_value, -Current_Max, Current_Max);
    Out = tmp_value * Current_To_Out;

    Output();
}

/**
 * @brief 计算PID
 *
 */
void Class_Booster_Friction_Motor_DJI_C620::PID_Calculate()
{
    switch (Motor_DJI_Control_Method)
    {
    case (Motor_DJI_Control_Method_CURRENT):
    {
        break;
    }
    case (Motor_DJI_Control_Method_OMEGA):
    {
        PID_Omega.Set_Target(Target_Omega + Feedforward_Omega);
        PID_Omega.Set_Now(Filter_Fourier_Omega.Get_Out());
        PID_Omega.TIM_Calculate_PeriodElapsedCallback();

        Target_Current = PID_Omega.Get_Out();

        break;
    }
    case (Motor_DJI_Control_Method_ANGLE):
    {
        PID_Angle.Set_Target(Target_Angle);
        PID_Angle.Set_Now(Rx_Data.Now_Angle);
        PID_Angle.TIM_Calculate_PeriodElapsedCallback();

        Target_Omega = PID_Angle.Get_Out();

        PID_Omega.Set_Target(Target_Omega + Feedforward_Omega);
        PID_Omega.Set_Now(Filter_Fourier_Omega.Get_Out());
        PID_Omega.TIM_Calculate_PeriodElapsedCallback();

        Target_Current = PID_Omega.Get_Out();

        break;
    }
    default:
    {
        Target_Current = 0.0f;

        break;
    }
    }
    Feedforward_Current = 0.0f;
    Feedforward_Omega = 0.0f;
}

/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/
