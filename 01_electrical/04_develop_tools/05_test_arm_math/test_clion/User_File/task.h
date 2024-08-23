/**
 * @file tsk_config_and_callback.h
 * @author yssickjgd (1345578933@qq.com)
 * @brief 临时任务调度测试用函数, 后续用来存放个人定义的回调函数以及若干任务
 * @version 0.1
 * @date 2023-08-29
 *
 * @copyright USTC-RoboWalker (c) 2023-2024
 *
 */

#ifndef TASK_H
#define TASK_H

/* Includes ------------------------------------------------------------------*/

/* Exported macros -----------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/* Exported variables --------------------------------------------------------*/

/* Exported function declarations --------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

void Task_Init();
void Task_Loop();

#ifdef __cplusplus
};
#endif

#endif

/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/
