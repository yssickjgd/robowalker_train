/**
 * @file drv_ospi.cpp
 * @author yssickjgd (1345578933@qq.com)
 * @brief 仿照SCUT-Robotlab改写的OSPI通信初始化与配置流程
 * @version 0.1
 * @date 2023-08-29 0.1 新建文档
 *
 * @copyright USTC-RoboWalker (c) 2025
 *
 */

/* Includes ------------------------------------------------------------------*/

#include "drv_ospi.h"

/* Private macros ------------------------------------------------------------*/

/* Private types -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

Struct_OSPI_Manage_Object OSPI1_Manage_Object = {nullptr};
Struct_OSPI_Manage_Object OSPI2_Manage_Object = {nullptr};

/* Private function declarations ---------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/

/**
 * @brief 初始化OSPI
 *
 * @param hospi OSPI编号
 * @param Command_Callback_Function 指令完成回调函数
 * @param Auto_Polling_Callback_Function 自动轮询完成回调函数
 * @param Rx_Callback_Function 接收完成回调函数
 */
void OSPI_Init(OSPI_HandleTypeDef *hospi, OSPI_Status_Match_Callback Auto_Polling_Callback_Function, OSPI_Rx_Callback Rx_Callback_Function)
{
    if (hospi->Instance == OCTOSPI1)
    {
        OSPI1_Manage_Object.OSPI_Handler = hospi;
        OSPI1_Manage_Object.Status_Match_Callback_Function = Auto_Polling_Callback_Function;
        OSPI1_Manage_Object.Rx_Callback_Function = Rx_Callback_Function;
    }
    else if (hospi->Instance == OCTOSPI2)
    {
        OSPI2_Manage_Object.OSPI_Handler = hospi;
        OSPI2_Manage_Object.Status_Match_Callback_Function = Auto_Polling_Callback_Function;
        OSPI2_Manage_Object.Rx_Callback_Function = Rx_Callback_Function;
    }
}

/**
 * @brief 自动轮询
 *
 */
void OSPI_Auto_Polling(OSPI_HandleTypeDef *hospi, OSPI_AutoPollingTypeDef *Config)
{
    if (hospi->Instance == OCTOSPI1)
    {
        HAL_OSPI_AutoPolling_IT(hospi, Config);
    }
    else if (hospi->Instance == OCTOSPI2)
    {
        HAL_OSPI_AutoPolling_IT(hospi, Config);
    }
}

/**
 * @brief 发送指令
 *
 * @param hospi OSPI编号
 */
void OSPI_Command(OSPI_HandleTypeDef *hospi, OSPI_RegularCmdTypeDef *Command)
{
    if (hospi->Instance == OCTOSPI1)
    {
        HAL_OSPI_Command(hospi, Command, HAL_OSPI_TIMEOUT_DEFAULT_VALUE);
    }
    else if (hospi->Instance == OCTOSPI2)
    {
        HAL_OSPI_Command(hospi, Command, HAL_OSPI_TIMEOUT_DEFAULT_VALUE);
    }
}

/**
 * @brief 发送数据
 *
 * @param hospi OSPI编号
 */
void OSPI_Command_Transmit_Data(OSPI_HandleTypeDef *hospi, OSPI_RegularCmdTypeDef *Command)
{
    if (hospi->Instance == OCTOSPI1)
    {
        HAL_OSPI_Command(hospi, Command, HAL_OSPI_TIMEOUT_DEFAULT_VALUE);
        HAL_OSPI_Transmit_DMA(hospi, OSPI1_Manage_Object.Tx_Buffer);
    }
    else if (hospi->Instance == OCTOSPI2)
    {
        HAL_OSPI_Command(hospi, Command, HAL_OSPI_TIMEOUT_DEFAULT_VALUE);
        HAL_OSPI_Transmit_DMA(hospi, OSPI2_Manage_Object.Tx_Buffer);
    }
}

/**
 * @brief 接收数据
 *
 * @param hospi OSPI编号
 */
void OSPI_Command_Receive_Data(OSPI_HandleTypeDef *hospi, OSPI_RegularCmdTypeDef *Command)
{
    if (hospi->Instance == OCTOSPI1)
    {
        HAL_OSPI_Command(hospi, Command, HAL_OSPI_TIMEOUT_DEFAULT_VALUE);
        HAL_OSPI_Receive_DMA(hospi, OSPI1_Manage_Object.Rx_Buffer);
    }
    else if (hospi->Instance == OCTOSPI2)
    {
        HAL_OSPI_Command(hospi, Command, HAL_OSPI_TIMEOUT_DEFAULT_VALUE);
        HAL_OSPI_Receive_DMA(hospi, OSPI2_Manage_Object.Rx_Buffer);
    }
}

/**
 * @brief 交互数据
 *
 * @param hospi OSPI编号
 */
void OSPI_Command_Transmit_Receive_Data(OSPI_HandleTypeDef *hospi, OSPI_RegularCmdTypeDef *Command)
{
    if (hospi->Instance == OCTOSPI1)
    {
        HAL_OSPI_Command(hospi, Command, HAL_OSPI_TIMEOUT_DEFAULT_VALUE);
        HAL_OSPI_Transmit_DMA(hospi, OSPI1_Manage_Object.Tx_Buffer);
        HAL_OSPI_Receive_DMA(hospi, OSPI1_Manage_Object.Rx_Buffer);
    }
    else if (hospi->Instance == OCTOSPI2)
    {
        HAL_OSPI_Command(hospi, Command, HAL_OSPI_TIMEOUT_DEFAULT_VALUE);
        HAL_OSPI_Transmit_DMA(hospi, OSPI2_Manage_Object.Tx_Buffer);
        HAL_OSPI_Receive_DMA(hospi, OSPI2_Manage_Object.Rx_Buffer);
    }
}

/**
 * @brief HAL库OSPI自动轮询回调函数
 *
 */
void HAL_OSPI_StatusMatchCallback(OSPI_HandleTypeDef *hospi)
{
    if (hospi->Instance == OCTOSPI1)
    {
        OSPI1_Manage_Object.Auto_Polling_Timestamp = SYS_Timestamp.Get_Current_Timestamp();

        if (OSPI1_Manage_Object.Status_Match_Callback_Function != nullptr)
        {
            OSPI1_Manage_Object.Status_Match_Callback_Function();
        }
    }
    else if (hospi->Instance == OCTOSPI2)
    {
        OSPI2_Manage_Object.Auto_Polling_Timestamp = SYS_Timestamp.Get_Current_Timestamp();

        if (OSPI2_Manage_Object.Status_Match_Callback_Function != nullptr)
        {
            OSPI2_Manage_Object.Status_Match_Callback_Function();
        }
    }
}

/**
 * @brief HAL库OSPI接收回调函数
 *
 * @param hospi OSPI编号
 */
void HAL_OSPI_RxCpltCallback(OSPI_HandleTypeDef *hospi)
{
    if (hospi->Instance == OCTOSPI1)
    {
        OSPI1_Manage_Object.Rx_Timestamp = SYS_Timestamp.Get_Current_Timestamp();

        if (OSPI1_Manage_Object.Rx_Callback_Function != nullptr)
        {
            OSPI1_Manage_Object.Rx_Callback_Function(OSPI1_Manage_Object.Rx_Buffer, OSPI_BUFFER_SIZE);
        }
    }
    else if (hospi->Instance == OCTOSPI2)
    {
        OSPI2_Manage_Object.Rx_Timestamp = SYS_Timestamp.Get_Current_Timestamp();

        if (OSPI2_Manage_Object.Rx_Callback_Function != nullptr)
        {
            OSPI2_Manage_Object.Rx_Callback_Function(OSPI2_Manage_Object.Rx_Buffer, OSPI_BUFFER_SIZE);
        }
    }
}

/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/