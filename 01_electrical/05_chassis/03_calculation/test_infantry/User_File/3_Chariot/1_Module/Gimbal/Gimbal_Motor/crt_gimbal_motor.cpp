/**
 * @file crt_gimbal_motor.cpp
 * @author yssickjgd (1345578933@qq.com)
 * @brief 云台电机
 * @version 0.1
 * @date 2024-01-30 新增
 *
 * @copyright USTC-RoboWalker (c) 2024
 *
 */

/* Includes ------------------------------------------------------------------*/

#include "crt_gimbal_motor.h"

/* Private macros ------------------------------------------------------------*/

/* Private types -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function declarations ---------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/

/**
 * @brief TIM定时器中断定期检测电机是否存活
 *
 */
void Class_Gimbal_Yaw_Motor_DJI_GM6020::TIM_100ms_Alive_PeriodElapsedCallback()
{
    // 判断该时间段内是否接收过电机数据
    if (Flag == Pre_Flag)
    {
        // 电机断开连接
        Motor_DJI_Status = Motor_DJI_Status_DISABLE;
        PID_Angle.Set_Integral_Error(0.0f);
        PID_Omega.Set_Integral_Error(0.0f);
        PID_Current.Set_Integral_Error(0.0f);
        PID_AHRS_Angle.Set_Integral_Error(0.0f);
        PID_AHRS_Omega.Set_Integral_Error(0.0f);
    }
    else
    {
        // 电机保持连接
        Motor_DJI_Status = Motor_DJI_Status_ENABLE;
    }
    Pre_Flag = Flag;
}

/**
 * @brief TIM定时器中断计算回调函数, 计算周期取决于电机反馈周期
 *
 */
void Class_Gimbal_Yaw_Motor_DJI_GM6020::TIM_Calculate_PeriodElapsedCallback()
{
    PID_Calculate();

    if (Driver_Version == Motor_DJI_GM6020_Driver_Version_DEFAULT)
    {
        float tmp_value = Target_Voltage + Feedforward_Voltage;
        Math_Constrain(&tmp_value, -Voltage_Max, Voltage_Max);
        Out = tmp_value * Voltage_To_Out;
    }
    else if (Driver_Version == Motor_DJI_GM6020_Driver_Version_2023)
    {
        float tmp_value = Target_Current + Feedforward_Current;
        Math_Constrain(&tmp_value, -Current_Max, Current_Max);
        Out = tmp_value * Current_To_Out;
    }

    Output();

    if (Power_Limit_Status == Motor_DJI_Power_Limit_Status_DISABLE)
    {
        Feedforward_Voltage = 0.0f;
        Feedforward_Current = 0.0f;
        Feedforward_Omega = 0.0f;
    }
}

/**
 * @brief 计算PID
 *
 */
void Class_Gimbal_Yaw_Motor_DJI_GM6020::PID_Calculate()
{
    if (AHRS_Gimbal->Get_Status() == AHRS_WIT_Status_ENABLE && AHRS_Chassis->Get_Status() == AHRS_WHEELTEC_Status_ENABLE)
    {
        // 陀螺仪在线, 则使用陀螺仪数据进行反馈
        switch (Motor_DJI_Control_Method)
        {
        case (Motor_DJI_Control_Method_VOLTAGE):
        {
            if (Driver_Version == Motor_DJI_GM6020_Driver_Version_DEFAULT)
            {

            }
            else if (Driver_Version == Motor_DJI_GM6020_Driver_Version_2023)
            {
                Target_Voltage = 0.0f;
                Target_Current = 0.0f;
            }

            break;
        }
        case (Motor_DJI_Control_Method_CURRENT):
        {
            if (Driver_Version == Motor_DJI_GM6020_Driver_Version_DEFAULT)
            {

                PID_Current.Set_Target(Target_Current + Feedforward_Current);
                PID_Current.Set_Now(Rx_Data.Now_Current);
                PID_Current.TIM_Calculate_PeriodElapsedCallback();

                Target_Voltage = PID_Current.Get_Out();
            }
            else if (Driver_Version == Motor_DJI_GM6020_Driver_Version_2023)
            {
            }

            break;
        }
        case (Motor_DJI_Control_Method_OMEGA):
        {
            if (Driver_Version == Motor_DJI_GM6020_Driver_Version_DEFAULT)
            {
                PID_AHRS_Omega.Set_Target(Target_Omega + Feedforward_Omega);
                PID_AHRS_Omega.Set_Now(AHRS_Gimbal->Get_Omega_Z() * arm_cos_f32(-Motor_Pitch->Get_Now_Angle()) - AHRS_Gimbal->Get_Omega_X() * arm_sin_f32(-Motor_Pitch->Get_Now_Angle()) + AHRS_Chassis->Get_Omega_Z());
                PID_AHRS_Omega.TIM_Calculate_PeriodElapsedCallback();

                Target_Current = PID_AHRS_Omega.Get_Out();


                PID_Current.Set_Target(Target_Current + Feedforward_Current);
                PID_Current.Set_Now(Rx_Data.Now_Current);
                PID_Current.TIM_Calculate_PeriodElapsedCallback();

                Target_Voltage = PID_Current.Get_Out();
            }
            else if (Driver_Version == Motor_DJI_GM6020_Driver_Version_2023)
            {
                PID_AHRS_Omega.Set_Target(Target_Omega + Feedforward_Omega);
                PID_AHRS_Omega.Set_Now(AHRS_Gimbal->Get_Omega_Z() * arm_cos_f32(-Motor_Pitch->Get_Now_Angle()) - AHRS_Gimbal->Get_Omega_X() * arm_sin_f32(-Motor_Pitch->Get_Now_Angle()) + AHRS_Chassis->Get_Omega_Z());
                PID_AHRS_Omega.TIM_Calculate_PeriodElapsedCallback();

                Target_Current = PID_AHRS_Omega.Get_Out();
            }

            break;
        }
        case (Motor_DJI_Control_Method_ANGLE):
        {
            if (Driver_Version == Motor_DJI_GM6020_Driver_Version_DEFAULT)
            {
                PID_AHRS_Angle.Set_Target(Target_Angle);
                PID_AHRS_Angle.Set_Now(Rx_Data.Now_Angle);
                PID_AHRS_Angle.TIM_Calculate_PeriodElapsedCallback();

                Target_Omega = PID_AHRS_Angle.Get_Out();

                PID_AHRS_Omega.Set_Target(Target_Omega + Feedforward_Omega);
                PID_AHRS_Omega.Set_Now(AHRS_Gimbal->Get_Omega_Z() * arm_cos_f32(-Motor_Pitch->Get_Now_Angle()) - AHRS_Gimbal->Get_Omega_X() * arm_sin_f32(-Motor_Pitch->Get_Now_Angle()) + AHRS_Chassis->Get_Omega_Z());
                PID_AHRS_Omega.TIM_Calculate_PeriodElapsedCallback();

                Target_Current = PID_AHRS_Omega.Get_Out();

                PID_Current.Set_Target(Target_Current + Feedforward_Current);
                PID_Current.Set_Now(Rx_Data.Now_Current);
                PID_Current.TIM_Calculate_PeriodElapsedCallback();

                Target_Voltage = PID_Current.Get_Out();
            }
            else if (Driver_Version == Motor_DJI_GM6020_Driver_Version_2023)
            {
                PID_AHRS_Angle.Set_Target(Target_Angle);
                PID_AHRS_Angle.Set_Now(Rx_Data.Now_Angle);
                PID_AHRS_Angle.TIM_Calculate_PeriodElapsedCallback();

                Target_Omega = PID_AHRS_Angle.Get_Out();

                PID_AHRS_Omega.Set_Target(Target_Omega + Feedforward_Omega);
                PID_AHRS_Omega.Set_Now(AHRS_Gimbal->Get_Omega_Z() * arm_cos_f32(-Motor_Pitch->Get_Now_Angle()) - AHRS_Gimbal->Get_Omega_X() * arm_sin_f32(-Motor_Pitch->Get_Now_Angle()) + AHRS_Chassis->Get_Omega_Z());
                PID_AHRS_Omega.TIM_Calculate_PeriodElapsedCallback();

                Target_Current = PID_AHRS_Omega.Get_Out();

            }

            break;
        }
        default:
        {
            Out = 0.0f;

            break;
        }
        }
    }
    else
    {
        // 陀螺仪离线, 则使用电机原本的编码器数据进行反馈
        switch (Motor_DJI_Control_Method)
        {
        case (Motor_DJI_Control_Method_VOLTAGE):
        {
            if (Driver_Version == Motor_DJI_GM6020_Driver_Version_DEFAULT)
            {

            }
            else if (Driver_Version == Motor_DJI_GM6020_Driver_Version_2023)
            {
                Target_Voltage = 0.0f;
                Target_Current = 0.0f;
            }

            break;
        }
        case (Motor_DJI_Control_Method_CURRENT):
        {
            if (Driver_Version == Motor_DJI_GM6020_Driver_Version_DEFAULT)
            {

                PID_Current.Set_Target(Target_Current + Feedforward_Current);
                PID_Current.Set_Now(Rx_Data.Now_Current);
                PID_Current.TIM_Calculate_PeriodElapsedCallback();

                Target_Voltage = PID_Current.Get_Out();
            }
            else if (Driver_Version == Motor_DJI_GM6020_Driver_Version_2023)
            {
            }

            break;
        }
        case (Motor_DJI_Control_Method_OMEGA):
        {
            if (Driver_Version == Motor_DJI_GM6020_Driver_Version_DEFAULT)
            {
                PID_Omega.Set_Target(Target_Omega + Feedforward_Omega);
                PID_Omega.Set_Now(Rx_Data.Now_Omega);
                PID_Omega.TIM_Calculate_PeriodElapsedCallback();

                Target_Current = PID_Omega.Get_Out();


                PID_Current.Set_Target(Target_Current + Feedforward_Current);
                PID_Current.Set_Now(Rx_Data.Now_Current);
                PID_Current.TIM_Calculate_PeriodElapsedCallback();

                Target_Voltage = PID_Current.Get_Out();
            }
            else if (Driver_Version == Motor_DJI_GM6020_Driver_Version_2023)
            {
                PID_Omega.Set_Target(Target_Omega + Feedforward_Omega);
                PID_Omega.Set_Now(Rx_Data.Now_Omega);
                PID_Omega.TIM_Calculate_PeriodElapsedCallback();

                Target_Current = PID_Omega.Get_Out();

            }

            break;
        }
        case (Motor_DJI_Control_Method_ANGLE):
        {
            if (Driver_Version == Motor_DJI_GM6020_Driver_Version_DEFAULT)
            {
                PID_Angle.Set_Target(Target_Angle);
                PID_Angle.Set_Now(Rx_Data.Now_Angle);
                PID_Angle.TIM_Calculate_PeriodElapsedCallback();

                Target_Omega = PID_Angle.Get_Out();

                PID_Omega.Set_Target(Target_Omega + Feedforward_Omega);
                PID_Omega.Set_Now(Rx_Data.Now_Omega);
                PID_Omega.TIM_Calculate_PeriodElapsedCallback();

                Target_Current = PID_Omega.Get_Out();


                PID_Current.Set_Target(Target_Current + Feedforward_Current);
                PID_Current.Set_Now(Rx_Data.Now_Current);
                PID_Current.TIM_Calculate_PeriodElapsedCallback();

                Target_Voltage = PID_Current.Get_Out();
            }
            else if (Driver_Version == Motor_DJI_GM6020_Driver_Version_2023)
            {
                PID_Angle.Set_Target(Target_Angle);
                PID_Angle.Set_Now(Rx_Data.Now_Angle);
                PID_Angle.TIM_Calculate_PeriodElapsedCallback();

                Target_Omega = PID_Angle.Get_Out();

                PID_Omega.Set_Target(Target_Omega + Feedforward_Omega);
                PID_Omega.Set_Now(Rx_Data.Now_Omega);
                PID_Omega.TIM_Calculate_PeriodElapsedCallback();

                Target_Current = PID_Omega.Get_Out();
            }

            break;
        }
        default:
        {
            if (Driver_Version == Motor_DJI_GM6020_Driver_Version_DEFAULT)
            {
                Target_Voltage = 0.0f;
            }
            else if (Driver_Version == Motor_DJI_GM6020_Driver_Version_2023)
            {
                Target_Current = 0.0f;
            }

            break;
        }
        }
    }
}

/**
 * @brief TIM定时器中断定期检测电机是否存活
 *
 */
void Class_Gimbal_Pitch_Motor_DJI_GM6020::TIM_100ms_Alive_PeriodElapsedCallback()
{
    // 判断该时间段内是否接收过电机数据
    if (Flag == Pre_Flag)
    {
        // 电机断开连接
        Motor_DJI_Status = Motor_DJI_Status_DISABLE;
        PID_Angle.Set_Integral_Error(0.0f);
        PID_Omega.Set_Integral_Error(0.0f);
        PID_Current.Set_Integral_Error(0.0f);
        PID_AHRS_Angle.Set_Integral_Error(0.0f);
        PID_AHRS_Omega.Set_Integral_Error(0.0f);
    }
    else
    {
        // 电机保持连接
        Motor_DJI_Status = Motor_DJI_Status_ENABLE;
    }
    Pre_Flag = Flag;
}

/**
 * @brief TIM定时器中断计算回调函数, 计算周期取决于电机反馈周期
 *
 */
void Class_Gimbal_Pitch_Motor_DJI_GM6020::TIM_Calculate_PeriodElapsedCallback()
{
    PID_Calculate();

    if (Driver_Version == Motor_DJI_GM6020_Driver_Version_DEFAULT)
    {
        float tmp_value = Target_Voltage + Feedforward_Voltage;
        Math_Constrain(&tmp_value, -Voltage_Max, Voltage_Max);
        Out = tmp_value * Voltage_To_Out;
    }
    else if (Driver_Version == Motor_DJI_GM6020_Driver_Version_2023)
    {
        float tmp_value = Target_Current + Feedforward_Current;
        Math_Constrain(&tmp_value, -Current_Max, Current_Max);
        Out = tmp_value * Current_To_Out;
    }

    Output();

    if (Power_Limit_Status == Motor_DJI_Power_Limit_Status_DISABLE)
    {
        Feedforward_Voltage = 0.0f;
        Feedforward_Current = 0.0f;
        Feedforward_Omega = 0.0f;
    }
}

/**
 * @brief 计算PID
 *
 */
void Class_Gimbal_Pitch_Motor_DJI_GM6020::PID_Calculate()
{
    if (AHRS_Gimbal->Get_Status() == AHRS_WIT_Status_ENABLE)
    {
        // 陀螺仪在线, 则使用陀螺仪数据进行反馈
        switch (Motor_DJI_Control_Method)
        {
        case (Motor_DJI_Control_Method_VOLTAGE):
        {
            if (Driver_Version == Motor_DJI_GM6020_Driver_Version_DEFAULT)
            {

            }
            else if (Driver_Version == Motor_DJI_GM6020_Driver_Version_2023)
            {
                Target_Voltage = 0.0f;
                Target_Current = 0.0f;
            }

            break;
        }
        case (Motor_DJI_Control_Method_CURRENT):
        {
            if (Driver_Version == Motor_DJI_GM6020_Driver_Version_DEFAULT)
            {
                PID_Current.Set_Target(Target_Current + Feedforward_Current);
                PID_Current.Set_Now(Rx_Data.Now_Current);
                PID_Current.TIM_Calculate_PeriodElapsedCallback();

                Target_Voltage = PID_Current.Get_Out();
            }
            else if (Driver_Version == Motor_DJI_GM6020_Driver_Version_2023)
            {

            }

            break;
        }
        case (Motor_DJI_Control_Method_OMEGA):
        {
            if (Driver_Version == Motor_DJI_GM6020_Driver_Version_DEFAULT)
            {
                PID_AHRS_Omega.Set_Target(Target_Omega + Feedforward_Omega);
                PID_AHRS_Omega.Set_Now(-AHRS_Gimbal->Get_Omega_Y());
                PID_AHRS_Omega.TIM_Calculate_PeriodElapsedCallback();

                Target_Current = PID_AHRS_Omega.Get_Out();

                PID_Current.Set_Target(Target_Current + Feedforward_Current);
                PID_Current.Set_Now(Rx_Data.Now_Current);
                PID_Current.TIM_Calculate_PeriodElapsedCallback();

                Target_Voltage = PID_Current.Get_Out();
            }
            else if (Driver_Version == Motor_DJI_GM6020_Driver_Version_2023)
            {
                PID_AHRS_Omega.Set_Target(Target_Omega + Feedforward_Omega);
                PID_AHRS_Omega.Set_Now(AHRS_Gimbal->Get_Omega_Y());
                PID_AHRS_Omega.TIM_Calculate_PeriodElapsedCallback();

                Target_Current = PID_AHRS_Omega.Get_Out();
            }

            break;
        }
        case (Motor_DJI_Control_Method_ANGLE):
        {
            if (Driver_Version == Motor_DJI_GM6020_Driver_Version_DEFAULT)
            {
                PID_AHRS_Angle.Set_Target(Target_Angle);
                PID_AHRS_Angle.Set_Now(Rx_Data.Now_Angle);
                PID_AHRS_Angle.TIM_Calculate_PeriodElapsedCallback();

                Target_Omega = PID_AHRS_Angle.Get_Out();

                PID_AHRS_Omega.Set_Target(Target_Omega + Feedforward_Omega);
                PID_AHRS_Omega.Set_Now(-AHRS_Gimbal->Get_Omega_Y());
                PID_AHRS_Omega.TIM_Calculate_PeriodElapsedCallback();

                Target_Current = PID_AHRS_Omega.Get_Out();

                PID_Current.Set_Target(Target_Current + Feedforward_Current);
                PID_Current.Set_Now(Rx_Data.Now_Current);
                PID_Current.TIM_Calculate_PeriodElapsedCallback();

                Target_Voltage = PID_Current.Get_Out();
            }
            else if (Driver_Version == Motor_DJI_GM6020_Driver_Version_2023)
            {
                PID_AHRS_Angle.Set_Target(Target_Angle);
                PID_AHRS_Angle.Set_Now(Rx_Data.Now_Angle);
                PID_AHRS_Angle.TIM_Calculate_PeriodElapsedCallback();

                Target_Omega = PID_AHRS_Angle.Get_Out();

                PID_AHRS_Omega.Set_Target(Target_Omega + Feedforward_Omega);
                PID_AHRS_Omega.Set_Now(-AHRS_Gimbal->Get_Omega_Y());
                PID_AHRS_Omega.TIM_Calculate_PeriodElapsedCallback();

                Target_Current = PID_AHRS_Omega.Get_Out();
            }

            break;
        }
        default:
        {
            Out = 0.0f;

            break;
        }
        }
    }
    else
    {
        // 陀螺仪离线, 则使用电机原本的编码器数据进行反馈
        switch (Motor_DJI_Control_Method)
        {
        case (Motor_DJI_Control_Method_VOLTAGE):
        {
            if (Driver_Version == Motor_DJI_GM6020_Driver_Version_DEFAULT)
            {

            }
            else if (Driver_Version == Motor_DJI_GM6020_Driver_Version_2023)
            {
                Target_Voltage = 0.0f;
                Target_Current = 0.0f;
            }

            break;
        }
        case (Motor_DJI_Control_Method_CURRENT):
        {
            if (Driver_Version == Motor_DJI_GM6020_Driver_Version_DEFAULT)
            {
                PID_Current.Set_Target(Target_Current + Feedforward_Current);
                PID_Current.Set_Now(Rx_Data.Now_Current);
                PID_Current.TIM_Calculate_PeriodElapsedCallback();

                Out = PID_Current.Get_Out();
            }
            else if (Driver_Version == Motor_DJI_GM6020_Driver_Version_2023)
            {

            }

            break;
        }
        case (Motor_DJI_Control_Method_OMEGA):
        {
            if (Driver_Version == Motor_DJI_GM6020_Driver_Version_DEFAULT)
            {
                PID_Omega.Set_Target(Target_Omega + Feedforward_Omega);
                PID_Omega.Set_Now(Rx_Data.Now_Omega);
                PID_Omega.TIM_Calculate_PeriodElapsedCallback();

                Target_Current = PID_Omega.Get_Out();

                PID_Current.Set_Target(Target_Current + Feedforward_Current);
                PID_Current.Set_Now(Rx_Data.Now_Current);
                PID_Current.TIM_Calculate_PeriodElapsedCallback();

                Target_Voltage = PID_Current.Get_Out();
            }
            else if (Driver_Version == Motor_DJI_GM6020_Driver_Version_2023)
            {
                PID_Omega.Set_Target(Target_Omega + Feedforward_Omega);
                PID_Omega.Set_Now(Rx_Data.Now_Omega);
                PID_Omega.TIM_Calculate_PeriodElapsedCallback();

                Target_Current = PID_Omega.Get_Out();
            }

            break;
        }
        case (Motor_DJI_Control_Method_ANGLE):
        {
            if (Driver_Version == Motor_DJI_GM6020_Driver_Version_DEFAULT)
            {
                PID_Angle.Set_Target(Target_Angle);
                PID_Angle.Set_Now(Rx_Data.Now_Angle);
                PID_Angle.TIM_Calculate_PeriodElapsedCallback();

                Target_Omega = PID_Angle.Get_Out();

                PID_Omega.Set_Target(Target_Omega + Feedforward_Omega);
                PID_Omega.Set_Now(Rx_Data.Now_Omega);
                PID_Omega.TIM_Calculate_PeriodElapsedCallback();

                Target_Current = PID_Omega.Get_Out();

                PID_Current.Set_Target(Target_Current + Feedforward_Current);
                PID_Current.Set_Now(Rx_Data.Now_Current);
                PID_Current.TIM_Calculate_PeriodElapsedCallback();

                Target_Voltage = PID_Current.Get_Out();
            }
            else if (Driver_Version == Motor_DJI_GM6020_Driver_Version_2023)
            {
                PID_Angle.Set_Target(Target_Angle);
                PID_Angle.Set_Now(Rx_Data.Now_Angle);
                PID_Angle.TIM_Calculate_PeriodElapsedCallback();

                Target_Omega = PID_Angle.Get_Out();

                PID_Omega.Set_Target(Target_Omega + Feedforward_Omega);
                PID_Omega.Set_Now(Rx_Data.Now_Omega);
                PID_Omega.TIM_Calculate_PeriodElapsedCallback();

                Target_Current = PID_Omega.Get_Out();
            }

            break;
        }
        default:
        {
            if (Driver_Version == Motor_DJI_GM6020_Driver_Version_DEFAULT)
            {
                Target_Voltage = 0.0f;
            }
            else if (Driver_Version == Motor_DJI_GM6020_Driver_Version_2023)
            {
                Target_Current = 0.0f;
            }

            break;
        }
        }
    }
}

/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/
