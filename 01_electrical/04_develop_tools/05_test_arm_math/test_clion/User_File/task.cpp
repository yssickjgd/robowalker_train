/**
 * @file tsk_config_and_callback.cpp
 * @author yssickjgd (1345578933@qq.com)
 * @brief 临时任务调度测试用函数, 后续用来存放个人定义的回调函数以及若干任务
 * @version 0.1
 * @date 2023-08-29
 *
 * @copyright USTC-RoboWalker (c) 2023-2024
 *
 */

/* Includes ------------------------------------------------------------------*/

#include "task.h"

#include <math.h>
#include "arm_math.h"
#include "dvc_serialplot.h"

/* Private macros ------------------------------------------------------------*/

/* Private types -------------------------------------------------------------*/

float flag = 0, i = 0, j = 0;
Class_Serialplot serialplot;
extern TIM_HandleTypeDef htim4;

/* Private variables ---------------------------------------------------------*/

/* Private function declarations ---------------------------------------------*/

void uart_null_callback(uint8_t *Buffer, uint16_t Length)
{

}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim->Instance == TIM4)
    {
        serialplot.TIM_Write_PeriodElapsedCallback();
        TIM_UART_PeriodElapsedCallback();
    }
}

/* Function prototypes -------------------------------------------------------*/

void Task_Init()
{
    UART_Init(&huart2, uart_null_callback, UART_BUFFER_SIZE);
    serialplot.Init(&huart2);
    serialplot.Set_Data(2, &flag, &i);
    HAL_TIM_Base_Start_IT(&htim4);
}

void Task_Loop()
{
    for(i = 0; i < 1000; i++)
    {
        for(j = 0; j < 1000; j++)
        {
            sin((float)(i + j));
        }
    }
    flag = 1;
    for(i = 0; i < 1000; i++)
    {
        for(j = 0; j < 1000; j++)
        {
            arm_sin_f32((float)(i + j));
        }
    }
    flag = 0;
}

/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/
