/**
 * @file alg_filter.cpp
 * @author yssickjgd (1345578933@qq.com)
 * @brief 滤波器
 * @version 1.1
 * @date 2023-08-29 0.1 23赛季定稿
 * @date 2023-09-25 1.1 可自定义滤波器阶数
 *
 * @copyright Copyright (c) 2023
 *
 */

/* Includes ------------------------------------------------------------------*/

#include "alg_filter.h"

/* Private macros ------------------------------------------------------------*/

/* Private types -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function declarations ---------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/

/**
 * @brief 初始化Kalman滤波器
 * 
 * @param __Error_Measure 测量误差
 * @param __Value 当前值
 * @param __Error_Estimate 估计误差
 */
void Class_Filter_Kalman::Init(float __Error_Measure, float __Now, float __Error_Estimate)
{
    Error_Measure = __Error_Measure;

    Now = __Now;
    Error_Estimate = __Error_Estimate;
}

/**
 * @brief 滤波器调整值, 周期与采样周期相同
 *
 */
void Class_Filter_Kalman::TIM_Calculate_PeriodElapsedCallback()
{
    Kalman_Gain = Error_Estimate / (Error_Estimate + Error_Measure);

    Out = Out + Kalman_Gain * (Now - Out);

    Error_Estimate = (1.0f - Kalman_Gain) * Error_Estimate;
}

/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/
