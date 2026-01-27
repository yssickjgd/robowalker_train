/**
 * @file alg_slope.cpp
 * @author yssickjgd (1345578933@qq.com)
 * @brief 斜坡函数, 用于速度规划等
 * @version 0.1
 * @date 2023-08-29 0.1 23赛季定稿
 * @date 2024-06-03 1.1 规划引入优先级方式
 *
 * @copyright USTC-RoboWalker (c) 2023-2024
 *
 */

/* Includes ------------------------------------------------------------------*/

#include "alg_slope.h"

/* Private macros ------------------------------------------------------------*/

/* Private types -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function declarations ---------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/

/**
 * @brief 初始化
 *
 * @param __Increase_Value 增长最大幅度
 * @param __Decrease_Value 降低最大幅度
 * @param __Slope_First 规划优先类型, 分为目标值优先和真实值优先
 */
void Class_Slope::Init(const float &__Increase_Value, const float &__Decrease_Value, const Enum_Slope_First &__Slope_First)
{
    Increase_Value = __Increase_Value;
    Decrease_Value = __Decrease_Value;
    Slope_First = __Slope_First;
}

/**
 * @brief 斜坡函数调整值, 计算周期取决于调用者
 *
 */
void Class_Slope::TIM_Calculate_PeriodElapsedCallback()
{
    // 规划为当前真实值优先的额外逻辑
    if (Slope_First == Slope_First_REAL)
    {
        if ((Target >= Now_Real && Now_Real >= Now_Planning) || (Target <= Now_Real && Now_Real <= Now_Planning))
        {
            Out = Now_Real;
        }
    }

    if (Now_Planning > 0.0f)
    {
        if (Target > Now_Planning)
        {
            // 正值加速
            if (Basic_Math_Abs(Now_Planning - Target) > Increase_Value)
            {
                Out += Increase_Value;
            }
            else
            {
                Out = Target;
            }
        }
        else if (Target < Now_Planning)
        {
            // 正值减速
            if (Basic_Math_Abs(Now_Planning - Target) > Decrease_Value)
            {
                Out -= Decrease_Value;
            }
            else
            {
                Out = Target;
            }
        }
    }
    else if (Now_Planning < 0.0f)
    {
        if (Target < Now_Planning)
        {
            // 负值加速
            if (Basic_Math_Abs(Now_Planning - Target) > Increase_Value)
            {
                Out -= Increase_Value;
            }
            else
            {
                Out = Target;
            }
        }
        else if (Target > Now_Planning)
        {
            // 负值减速
            if (Basic_Math_Abs(Now_Planning - Target) > Decrease_Value)
            {
                Out += Decrease_Value;
            }
            else
            {
                Out = Target;
            }
        }
    }
    else
    {
        if (Target > Now_Planning)
        {
            // 0值正加速
            if (Basic_Math_Abs(Now_Planning - Target) > Increase_Value)
            {
                Out += Increase_Value;
            }
            else
            {
                Out = Target;
            }
        }
        else if (Target < Now_Planning)
        {
            // 0值负加速
            if (Basic_Math_Abs(Now_Planning - Target) > Increase_Value)
            {
                Out -= Increase_Value;
            }
            else
            {
                Out = Target;
            }
        }
    }

    // 善后工作
    Now_Planning = Out;
}

/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/