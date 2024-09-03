/**
 * @file crt_booster.cpp
 * @author yssickjgd (1345578933@qq.com)
 * @brief 发射机构电控
 * @version 0.1
 * @date 2023-08-29 0.1 23赛季定稿
 *
 * @copyright USTC-RoboWalker (c) 2023
 *
 */

/**
 * @brief 摩擦轮编号
 * 1 2
 */

/* Includes ------------------------------------------------------------------*/

#include "crt_booster.h"

/* Private macros ------------------------------------------------------------*/

/* Private types -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function declarations ---------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/

/**
 * @brief 卡弹策略有限自动机
 *
 */
void Class_FSM_Anti_Jamming::TIM_1ms_Calculate_PeriodElapsedCallback()
{
    Status[Now_Status_Serial].Count_Time++;

    // 自己接着编写状态转移函数
    switch (Now_Status_Serial)
    {
    case (FSM_Anti_Jamming_Status_NORMAL):
    {
        // 正常状态

        if (Booster->Motor_Driver.Get_Now_Current() >= Driver_Torque_Threshold)
        {
            // 大扭矩->卡弹嫌疑状态
            Set_Status(FSM_Anti_Jamming_Status_JAMMING_SUSPECT);
        }

        break;
    }
    case (FSM_Anti_Jamming_Status_JAMMING_SUSPECT):
    {
        // 卡弹嫌疑状态

        if (Status[Now_Status_Serial].Count_Time >= Jamming_Suspect_Time_Threshold)
        {
            // 长时间大扭矩->卡弹反应状态
            Set_Status(FSM_Anti_Jamming_Status_JAMMING_CONFIRM);
        }
        else if (Booster->Motor_Driver.Get_Now_Current() < Driver_Torque_Threshold)
        {
            // 短时间大扭矩->正常状态
            Set_Status(FSM_Anti_Jamming_Status_NORMAL);
        }

        break;
    }
    case (FSM_Anti_Jamming_Status_JAMMING_CONFIRM):
    {
        // 卡弹反应状态->准备卡弹处理

        Booster->Motor_Driver.Set_Control_Method(Motor_DJI_Control_Method_ANGLE);
        Booster->Motor_Driver.Set_Target_Angle(Booster->Motor_Driver.Get_Now_Angle() - Driver_Back_Angle);

        Set_Status(FSM_Anti_Jamming_Status_PROCESSING);

        break;
    }
    case (FSM_Anti_Jamming_Status_PROCESSING):
    {
        // 卡弹处理状态

        if (Status[Now_Status_Serial].Count_Time >= Jamming_Solving_Time_Threshold)
        {
            // 长时间回拨->正常状态
            Set_Status(FSM_Anti_Jamming_Status_NORMAL);
        }

        break;
    }
    }
}

/**
 * @brief 发射机构初始化
 *
 */
void Class_Booster::Init()
{

    // 正常状态, 卡弹嫌疑状态, 卡弹反应状态, 卡弹处理状态
    FSM_Anti_Jamming.Booster = this;
    FSM_Anti_Jamming.Init(4, FSM_Anti_Jamming_Status_NORMAL);

    // 拨弹盘电机初始化

    // PID初始化
    Motor_Driver.PID_Angle.Init(50.0f, 0.0f, 0.0f, 0.0f, 2.0f * PI, 2.0f * PI);
    Motor_Driver.PID_Omega.Init(3.40f, 200.0f, 0.0f, 0.0f, 10.0f, 10.0f);
    // 电机初始化
    Motor_Driver.Init(&hcan1, Motor_DJI_ID_0x202, Motor_DJI_Control_Method_OMEGA);

    // 摩擦轮电机左

    // 滤波器初始化
    Motor_Friction_Left.Filter_Fourier_Omega.Init(0.0f, 0.0f, Filter_Fourier_Type_LOWPASS, 25.0f, 0.0f, 1000.0f);
    // PID初始化
    Motor_Friction_Left.PID_Omega.Init(0.15f, 0.3f, 0.002f, 0.0f, 20.0f, 20.0f);
    // 电机初始化
    Motor_Friction_Left.Init(&hcan1, Motor_DJI_ID_0x204, Motor_DJI_Control_Method_OMEGA, 1.0f);

    // 摩擦轮电机右

    // 滤波器初始化
    Motor_Friction_Right.Filter_Fourier_Omega.Init(0.0f, 0.0f, Filter_Fourier_Type_LOWPASS, 25.0f, 0.0f, 1000.0f);
    // PID初始化
    Motor_Friction_Right.PID_Omega.Init(0.15f, 0.3f, 0.002f, 0.0f, 20.0f, 20.0f);
    // 电机初始化
    Motor_Friction_Right.Init(&hcan1, Motor_DJI_ID_0x203, Motor_DJI_Control_Method_OMEGA, 1.0f);
}

/**
 * @brief TIM定时器中断定期检测电机是否存活
 *
 */
void Class_Booster::TIM_100ms_Alive_PeriodElapsedCallback()
{
    Motor_Driver.TIM_100ms_Alive_PeriodElapsedCallback();
    Motor_Friction_Left.TIM_100ms_Alive_PeriodElapsedCallback();
    Motor_Friction_Right.TIM_100ms_Alive_PeriodElapsedCallback();
}

/**
 * @brief 定时器计算函数
 *
 */
void Class_Booster::TIM_1ms_Calculate_PeriodElapsedCallback()
{
    FSM_Anti_Jamming.TIM_1ms_Calculate_PeriodElapsedCallback();

    // 如果有限状态机正常则执行拨弹逻辑, 否则由有限状态机进行卡弹处理
    if (FSM_Anti_Jamming.Get_Now_Status_Serial() == FSM_Anti_Jamming_Status_NORMAL || FSM_Anti_Jamming.Get_Now_Status_Serial() == FSM_Anti_Jamming_Status_JAMMING_SUSPECT)
    {
        Output();
    }

    Motor_Driver.TIM_Calculate_PeriodElapsedCallback();
    Motor_Friction_Left.TIM_Calculate_PeriodElapsedCallback();
    Motor_Friction_Right.TIM_Calculate_PeriodElapsedCallback();
}

/**
 * @brief 输出到电机
 *
 */
void Class_Booster::Output()
{
    switch (Booster_Control_Type)
    {
    case (Booster_Control_Type_DISABLE):
    {
        // 发射机构失能

        Motor_Driver.Set_Control_Method(Motor_DJI_Control_Method_CURRENT);
        Motor_Friction_Left.Set_Control_Method(Motor_DJI_Control_Method_OMEGA);
        Motor_Friction_Right.Set_Control_Method(Motor_DJI_Control_Method_OMEGA);

        Motor_Driver.PID_Angle.Set_Integral_Error(0.0f);
        Motor_Driver.PID_Omega.Set_Integral_Error(0.0f);
        Motor_Friction_Left.PID_Angle.Set_Integral_Error(0.0f);
        Motor_Friction_Right.PID_Angle.Set_Integral_Error(0.0f);

        Motor_Driver.Set_Target_Current(0.0f);
        Motor_Friction_Left.Set_Target_Omega(0.0f);
        Motor_Friction_Right.Set_Target_Omega(0.0f);

        break;
    }
    case (Booster_Control_Type_CEASEFIRE):
    {
        // 停火

        Now_Ammo_Shoot_Frequency = 0.0f;
        if (Motor_Driver.Get_Control_Method() == Motor_DJI_Control_Method_ANGLE)
        {
        }
        else if (Motor_Driver.Get_Control_Method() == Motor_DJI_Control_Method_OMEGA)
        {
            Motor_Driver.Set_Target_Omega(0.0f);
        }
        Motor_Friction_Left.Set_Target_Omega(Friction_Omega);
        Motor_Friction_Right.Set_Target_Omega(-Friction_Omega);

        break;
    }
    case (Booster_Control_Type_SPOT):
    {
        // 单发模式

        Motor_Driver.Set_Control_Method(Motor_DJI_Control_Method_ANGLE);
        Motor_Friction_Left.Set_Control_Method(Motor_DJI_Control_Method_OMEGA);
        Motor_Friction_Right.Set_Control_Method(Motor_DJI_Control_Method_OMEGA);

        Motor_Driver.Set_Target_Angle(Motor_Driver.Get_Now_Angle() + 2.0f * PI / 7.0f);
        Motor_Friction_Left.Set_Target_Omega(Friction_Omega);
        Motor_Friction_Right.Set_Target_Omega(-Friction_Omega);

        // 打出去立刻停火
        Booster_Control_Type = Booster_Control_Type_CEASEFIRE;

        break;
    }
    case (Booster_Control_Type_AUTO):
    {
        // 自动模式

        // 热量控制
        float tmp_delta = Heat_Limit_Max - Now_Heat;
        if (tmp_delta >= Heat_Limit_Slowdown_Threshold)
        {
            Now_Ammo_Shoot_Frequency = Target_Ammo_Shoot_Frequency;
        }
        else if (tmp_delta >= Heat_Limit_Ceasefire_Threshold && tmp_delta < Heat_Limit_Slowdown_Threshold)
        {
            Now_Ammo_Shoot_Frequency = (Target_Ammo_Shoot_Frequency * (Heat_Limit_Ceasefire_Threshold - tmp_delta) + Heat_CD / 10.0f * (tmp_delta - Heat_Limit_Slowdown_Threshold)) / (Heat_Limit_Ceasefire_Threshold - Heat_Limit_Slowdown_Threshold);
        }
        else if (tmp_delta < Heat_Limit_Ceasefire_Threshold)
        {
            Now_Ammo_Shoot_Frequency = 0.0f;
        }

        Motor_Driver.Set_Control_Method(Motor_DJI_Control_Method_OMEGA);
        Motor_Friction_Left.Set_Control_Method(Motor_DJI_Control_Method_OMEGA);
        Motor_Friction_Right.Set_Control_Method(Motor_DJI_Control_Method_OMEGA);

        Motor_Driver.Set_Target_Omega(Now_Ammo_Shoot_Frequency * 2.0f * PI / Ammo_Num_Per_Round);
        Motor_Friction_Left.Set_Target_Omega(Friction_Omega);
        Motor_Friction_Right.Set_Target_Omega(-Friction_Omega);

        break;
    }
    }
}

/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/
