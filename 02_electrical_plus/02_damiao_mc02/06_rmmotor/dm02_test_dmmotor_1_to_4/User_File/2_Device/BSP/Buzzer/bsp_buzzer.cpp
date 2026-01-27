/**
 * @file bsp_buzzer.cpp
 * @author yssickjgd (1345578933@qq.com)
 * @brief 板载蜂鸣器
 * @version 0.1
 * @date 2025-08-15 0.1 新建文档
 *
 * @copyright USTC-RoboWalker (c) 2025
 *
 */

/**
 * Cube中, 主频240M, psc240, arr250, 对应freq4000Hz
 * 原则上满足, arr * freq = 1MHz
 * 因此, freq = 1MHz / arr
 *
 * 此外, 该蜂鸣器有效音域为G3~ A7
 */

/* Includes ------------------------------------------------------------------*/

#include "bsp_buzzer.h"

/* Private macros ------------------------------------------------------------*/

/* Private types -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

// 常见音符频率
const float BUZZER_FREQUENCY_A0 = 27.501f;
const float BUZZER_FREQUENCY_A0_SHARP = 29.135f;
const float BUZZER_FREQUENCY_B0_FLAT = 29.135f;
const float BUZZER_FREQUENCY_B0 = 30.868f;
const float BUZZER_FREQUENCY_C1 = 32.703f;
const float BUZZER_FREQUENCY_C1_SHARP = 34.648f;
const float BUZZER_FREQUENCY_D1_FLAT = 34.648f;
const float BUZZER_FREQUENCY_D1 = 36.708f;
const float BUZZER_FREQUENCY_D1_SHARP = 38.891f;
const float BUZZER_FREQUENCY_E1_FLAT = 38.891f;
const float BUZZER_FREQUENCY_E1 = 41.203f;
const float BUZZER_FREQUENCY_F1 = 43.654f;
const float BUZZER_FREQUENCY_F1_SHARP = 46.249f;
const float BUZZER_FREQUENCY_G1_FLAT = 46.249f;
const float BUZZER_FREQUENCY_G1 = 49.000f;
const float BUZZER_FREQUENCY_G1_SHARP = 51.913f;
const float BUZZER_FREQUENCY_A1_FLAT = 51.913f;
const float BUZZER_FREQUENCY_A1 = 55.000f;
const float BUZZER_FREQUENCY_A1_SHARP = 58.270f;
const float BUZZER_FREQUENCY_B1_FLAT = 58.270f;
const float BUZZER_FREQUENCY_B1 = 61.735f;
const float BUZZER_FREQUENCY_C2 = 65.406f;
const float BUZZER_FREQUENCY_C2_SHARP = 69.296f;
const float BUZZER_FREQUENCY_D2_FLAT = 69.296f;
const float BUZZER_FREQUENCY_D2 = 73.416f;
const float BUZZER_FREQUENCY_D2_SHARP = 77.782f;
const float BUZZER_FREQUENCY_E2_FLAT = 77.782f;
const float BUZZER_FREQUENCY_E2 = 82.407f;
const float BUZZER_FREQUENCY_F2 = 87.307f;
const float BUZZER_FREQUENCY_F2_SHARP = 92.499f;
const float BUZZER_FREQUENCY_G2_FLAT = 92.499f;
const float BUZZER_FREQUENCY_G2 = 98.000f;
const float BUZZER_FREQUENCY_G2_SHARP = 103.826f;
const float BUZZER_FREQUENCY_A2_FLAT = 103.826f;
const float BUZZER_FREQUENCY_A2 = 110.000f;
const float BUZZER_FREQUENCY_A2_SHARP = 116.541f;
const float BUZZER_FREQUENCY_B2_FLAT = 116.541f;
const float BUZZER_FREQUENCY_B2 = 123.471f;
const float BUZZER_FREQUENCY_C3 = 130.813f;
const float BUZZER_FREQUENCY_C3_SHARP = 138.591f;
const float BUZZER_FREQUENCY_D3_FLAT = 138.591f;
const float BUZZER_FREQUENCY_D3 = 146.832f;
const float BUZZER_FREQUENCY_D3_SHARP = 155.563f;
const float BUZZER_FREQUENCY_E3_FLAT = 155.563f;
const float BUZZER_FREQUENCY_E3 = 164.814f;
const float BUZZER_FREQUENCY_F3 = 174.614f;
const float BUZZER_FREQUENCY_F3_SHARP = 184.997f;
const float BUZZER_FREQUENCY_G3_FLAT = 184.997f;
const float BUZZER_FREQUENCY_G3 = 196.000f;
const float BUZZER_FREQUENCY_G3_SHARP = 207.652f;
const float BUZZER_FREQUENCY_A3_FLAT = 207.652f;
const float BUZZER_FREQUENCY_A3 = 220.000f;
const float BUZZER_FREQUENCY_A3_SHARP = 233.082f;
const float BUZZER_FREQUENCY_B3_FLAT = 233.082f;
const float BUZZER_FREQUENCY_B3 = 246.942f;
const float BUZZER_FREQUENCY_C4 = 261.626f;
const float BUZZER_FREQUENCY_C4_SHARP = 277.183f;
const float BUZZER_FREQUENCY_D4_FLAT = 277.183f;
const float BUZZER_FREQUENCY_D4 = 293.665f;
const float BUZZER_FREQUENCY_D4_SHARP = 311.127f;
const float BUZZER_FREQUENCY_E4_FLAT = 311.127f;
const float BUZZER_FREQUENCY_E4 = 329.628f;
const float BUZZER_FREQUENCY_F4 = 349.228f;
const float BUZZER_FREQUENCY_F4_SHARP = 369.994f;
const float BUZZER_FREQUENCY_G4_FLAT = 369.994f;
const float BUZZER_FREQUENCY_G4 = 392.000f;
const float BUZZER_FREQUENCY_G4_SHARP = 415.305f;
const float BUZZER_FREQUENCY_A4_FLAT = 415.305f;
const float BUZZER_FREQUENCY_A4 = 440.000f;
const float BUZZER_FREQUENCY_A4_SHARP = 466.164f;
const float BUZZER_FREQUENCY_B4_FLAT = 466.164f;
const float BUZZER_FREQUENCY_B4 = 493.883f;
const float BUZZER_FREQUENCY_C5 = 523.251f;
const float BUZZER_FREQUENCY_C5_SHARP = 554.365f;
const float BUZZER_FREQUENCY_D5_FLAT = 554.365f;
const float BUZZER_FREQUENCY_D5 = 587.330f;
const float BUZZER_FREQUENCY_D5_SHARP = 622.254f;
const float BUZZER_FREQUENCY_E5_FLAT = 622.254f;
const float BUZZER_FREQUENCY_E5 = 659.255f;
const float BUZZER_FREQUENCY_F5 = 698.456f;
const float BUZZER_FREQUENCY_F5_SHARP = 739.989f;
const float BUZZER_FREQUENCY_G5_FLAT = 739.989f;
const float BUZZER_FREQUENCY_G5 = 783.991f;
const float BUZZER_FREQUENCY_G5_SHARP = 830.609f;
const float BUZZER_FREQUENCY_A5_FLAT = 830.609f;
const float BUZZER_FREQUENCY_A5 = 880.000f;
const float BUZZER_FREQUENCY_A5_SHARP = 932.328f;
const float BUZZER_FREQUENCY_B5_FLAT = 932.328f;
const float BUZZER_FREQUENCY_B5 = 987.767f;
const float BUZZER_FREQUENCY_C6 = 1046.502f;
const float BUZZER_FREQUENCY_C6_SHARP = 1108.731f;
const float BUZZER_FREQUENCY_D6_FLAT = 1108.731f;
const float BUZZER_FREQUENCY_D6 = 1174.659f;
const float BUZZER_FREQUENCY_D6_SHARP = 1244.508f;
const float BUZZER_FREQUENCY_E6_FLAT = 1244.508f;
const float BUZZER_FREQUENCY_E6 = 1318.510f;
const float BUZZER_FREQUENCY_F6 = 1396.913f;
const float BUZZER_FREQUENCY_F6_SHARP = 1479.978f;
const float BUZZER_FREQUENCY_G6_FLAT = 1479.978f;
const float BUZZER_FREQUENCY_G6 = 1567.982f;
const float BUZZER_FREQUENCY_G6_SHARP = 1661.219f;
const float BUZZER_FREQUENCY_A6_FLAT = 1661.219f;
const float BUZZER_FREQUENCY_A6 = 1760.000f;
const float BUZZER_FREQUENCY_A6_SHARP = 1864.655f;
const float BUZZER_FREQUENCY_B6_FLAT = 1864.655f;
const float BUZZER_FREQUENCY_B6 = 1975.533f;
const float BUZZER_FREQUENCY_C7 = 2093.005f;
const float BUZZER_FREQUENCY_C7_SHARP = 2217.461f;
const float BUZZER_FREQUENCY_D7_FLAT = 2217.461f;
const float BUZZER_FREQUENCY_D7 = 2349.318f;
const float BUZZER_FREQUENCY_D7_SHARP = 2489.016f;
const float BUZZER_FREQUENCY_E7_FLAT = 2489.016f;
const float BUZZER_FREQUENCY_E7 = 2637.020f;
const float BUZZER_FREQUENCY_F7 = 2793.826f;
const float BUZZER_FREQUENCY_F7_SHARP = 2959.955f;
const float BUZZER_FREQUENCY_G7_FLAT = 2959.955f;
const float BUZZER_FREQUENCY_G7 = 3135.963f;
const float BUZZER_FREQUENCY_G7_SHARP = 3322.438f;
const float BUZZER_FREQUENCY_A7_FLAT = 3322.438f;
const float BUZZER_FREQUENCY_A7 = 3520.000f;
const float BUZZER_FREQUENCY_A7_SHARP = 3729.310f;
const float BUZZER_FREQUENCY_B7_FLAT = 3729.310f;
const float BUZZER_FREQUENCY_B7 = 3951.066f;
const float BUZZER_FREQUENCY_C8 = 4186.009f;

Class_Buzzer BSP_Buzzer;

/* Private function declarations ---------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/

/**
 * @brief 初始化蜂鸣器
 *
 * @param __Frequency 蜂鸣器频率, 单位Hz, 默认4000Hz
 * @param __Loudness 蜂鸣器响度, 0-1, 默认0
 */
void Class_Buzzer::Init(const float &__Frequency, const float &__Loudness)
{
    htim = &htim12;
    TIM_Channel = TIM_CHANNEL_2;

    Frequency = Basic_Math_Constrain(__Frequency, 0.0f, 20000.0f);
    Loudness = Basic_Math_Constrain(__Loudness, 0.0f, 1.0f);

    float arr = 1000000.0f / Frequency;
    __HAL_TIM_SetAutoreload(htim, arr);

    float ccr = arr * Loudness * 0.5f;
    __HAL_TIM_SetCompare(htim, TIM_Channel, ccr);

    HAL_TIM_PWM_Start(htim, TIM_Channel);
}

/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/