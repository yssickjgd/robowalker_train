/**
 * @file alg_pid.cpp
 * @author yssickjgd 1345578933@qq.com
 * @brief PID算法
 * @version 0.1
 * @date 2022-05-08
 *
 * @copyright USTC-RoboWalker (c) 2022
 *
 */

/* Includes ------------------------------------------------------------------*/

#include "alg_pid.h"

/* Private macros ------------------------------------------------------------*/

/* Private types -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function declarations ---------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/

/**
 * @brief PID初始化
 *
 * @param __K_P P值
 * @param __K_I I值
 * @param __K_D D值
 * @param __K_F 前馈
 * @param __I_Out_Max 积分限幅
 * @param __Out_Max 输出限幅
 * @param __D_T 时间片长度
 */
void Class_PID::Init(float __K_P, float __K_I, float __K_D, float __K_F, float __I_Out_Max, float __Out_Max, float __D_T, float __Dead_Zone, float __I_Variable_Speed_A, float __I_Variable_Speed_B, float __I_Separate_Threshold, Enum_PID_D_First __D_First)
{
    K_P = __K_P;
    K_I = __K_I;
    K_D = __K_D;
    K_F = __K_F;
    I_Out_Max = __I_Out_Max;
    Out_Max = __Out_Max;
    D_T = __D_T;
    Dead_Zone = __Dead_Zone;
    I_Variable_Speed_A = __I_Variable_Speed_A;
    I_Variable_Speed_B = __I_Variable_Speed_B;
    I_Separate_Threshold = __I_Separate_Threshold;
    D_First = __D_First;
}

/**
 * @brief 获取输出值
 *
 * @return float 输出值
 */
float Class_PID::Get_Integral_Error()
{
    return (Integral_Error);
}

/**
 * @brief 获取输出值
 *
 * @return float 输出值
 */
float Class_PID::Get_Out()
{
    return (Out);
}

/**
 * @brief 设定PID的P
 *
 * @param __K_P PID的P
 */
void Class_PID::Set_K_P(float __K_P)
{
    K_P = __K_P;
}

/**
 * @brief 设定PID的I
 *
 * @param __K_I PID的I
 */
void Class_PID::Set_K_I(float __K_I)
{
    K_I = __K_I;
}

/**
 * @brief 设定PID的D
 *
 * @param __K_D PID的D
 */
void Class_PID::Set_K_D(float __K_D)
{
    K_D = __K_D;
}

/**
 * @brief 设定前馈
 *
 * @param __K_D 前馈
 */
void Class_PID::Set_K_F(float __K_F)
{
    K_F = __K_F;
}

/**
 * @brief 设定积分限幅, 0为不限制
 *
 * @param __I_Out_Max 积分限幅, 0为不限制
 */
void Class_PID::Set_I_Out_Max(float __I_Out_Max)
{
    I_Out_Max = __I_Out_Max;
}

/**
 * @brief 设定输出限幅, 0为不限制
 *
 * @param __Out_Max 输出限幅, 0为不限制
 */
void Class_PID::Set_Out_Max(float __Out_Max)
{
    Out_Max = __Out_Max;
}

/**
 * @brief 设定定速内段阈值, 0为不限制
 *
 * @param __I_Variable_Speed_A 定速内段阈值, 0为不限制
 */
void Class_PID::Set_I_Variable_Speed_A(float __I_Variable_Speed_A)
{
    I_Variable_Speed_A = __I_Variable_Speed_A;
}

/**
 * @brief 设定变速区间, 0为不限制
 *
 * @param __I_Variable_Speed_B 变速区间, 0为不限制
 */
void Class_PID::Set_I_Variable_Speed_B(float __I_Variable_Speed_B)
{
    I_Variable_Speed_B = __I_Variable_Speed_B;
}

/**
 * @brief 设定积分分离阈值，需为正数, 0为不限制
 *
 * @param __I_Separate_Threshold 积分分离阈值，需为正数, 0为不限制
 */
void Class_PID::Set_I_Separate_Threshold(float __I_Separate_Threshold)
{
    I_Separate_Threshold = __I_Separate_Threshold;
}

/**
 * @brief 设定目标值
 *
 * @param __Target 目标值
 */
void Class_PID::Set_Target(float __Target)
{
    Target = __Target;
}

/**
 * @brief 设定当前值
 *
 * @param __Now 当前值
 */
void Class_PID::Set_Now(float __Now)
{
    Now = __Now;
}

/**
 * @brief 设定积分, 一般用于积分清零
 *
 * @param __Set_Integral_Error 积分值
 */
void Class_PID::Set_Integral_Error(float __Integral_Error)
{
    Integral_Error = __Integral_Error;
}

/**
 * @brief PID调整值
 *
 * @return float 输出值
 */
void Class_PID::TIM_Adjust_PeriodElapsedCallback()
{
    // P输出
    float p_out = 0.0f;
    // I输出
    float i_out = 0.0f;
    // D输出
    float d_out = 0.0f;
    // F输出
    float f_out = 0.0f;
    //误差
    float error;
    //绝对值误差
    float abs_error;
    //线性变速积分
    float speed_ratio;

    error = Target - Now;
    abs_error = Math_Abs(error);

    //判断死区
    if (abs_error < Dead_Zone)
    {
        Target = Now;
        error = 0.0f;
        abs_error = 0.0f;
    }

    //计算p项

    p_out = K_P * error;

    //计算i项

    if (I_Variable_Speed_A == 0.0f && I_Variable_Speed_A == 0.0f)
    {
        //非变速积分
        speed_ratio = 1.0f;
    }
    else
    {
        //变速积分
        if (abs_error <= I_Variable_Speed_B)
        {
            speed_ratio = 1.0f;
        }
        else if (I_Variable_Speed_B < abs_error && abs_error < I_Variable_Speed_A + I_Variable_Speed_B)
        {
            speed_ratio = (I_Variable_Speed_A + I_Variable_Speed_B - abs_error) / I_Variable_Speed_A;
        }
        if (abs_error >= I_Variable_Speed_B)
        {
            speed_ratio = 0.0f;
        }
    }
    //积分限幅
    if (I_Out_Max != 0.0f)
    {
        Math_Constrain(&Integral_Error, -I_Out_Max / K_I, I_Out_Max / K_I);
    }
    if (I_Separate_Threshold == 0.0f)
    {
        //没有积分分离
        Integral_Error += speed_ratio * D_T * error;
        i_out = K_I * Integral_Error;
    }
    else
    {
        //积分分离使能
        if (abs_error < I_Separate_Threshold)
        {
            Integral_Error += speed_ratio * D_T * error;
            i_out = K_I * Integral_Error;
        }
        else
        {
            Integral_Error = 0.0f;
            i_out = 0.0f;
        }
    }

    //计算d项

    if (D_First == PID_D_First_DISABLE)
    {
        //没有微分先行
        d_out = K_D * (error - Pre_Error) / D_T;
    }
    else
    {
        //微分先行使能
        d_out = K_D * (Out - Pre_Out) / D_T;
    }

    //计算前馈

    f_out = (Target - Pre_Target) * K_F;

    //计算总共的输出

    Out = p_out + i_out + d_out + f_out;
    //输出限幅
    if (Out_Max != 0.0f)
    {
        Math_Constrain(&Out, -Out_Max, Out_Max);
    }

    //善后工作
    Pre_Now = Now;
    Pre_Target = Target;
    Pre_Out = Out;
    Pre_Error = error;
}

/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/
