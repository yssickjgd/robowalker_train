/**
 * @file drv_spi.cpp
 * @author yssickjgd (1345578933@qq.com)
 * @brief 仿照SCUT-Robotlab改写的SPI通信初始化与配置流程
 * @version 3.1
 * @date 2023-08-29 0.1 23赛季定稿
 * @date 2023-09-13 1.1 加入SPI配置操作
 * @date 2023-11-19 1.2 修改成cpp, 24赛季定稿
 * @date 2024-08-22 2.1 新增回调函数空指针判定
 * @date 2025-08-13 3.1 适配达妙MC02板, 准备着手开发陀螺仪
 *
 * @copyright USTC-RoboWalker (c) 2023-2025
 *
 */

/* Includes ------------------------------------------------------------------*/

#include "drv_spi.h"

/* Private macros ------------------------------------------------------------*/

/* Private types -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

Struct_SPI_Manage_Object SPI1_Manage_Object = {nullptr};
Struct_SPI_Manage_Object SPI2_Manage_Object = {nullptr};
Struct_SPI_Manage_Object SPI3_Manage_Object = {nullptr};
Struct_SPI_Manage_Object SPI4_Manage_Object = {nullptr};
Struct_SPI_Manage_Object SPI5_Manage_Object = {nullptr};
Struct_SPI_Manage_Object SPI6_Manage_Object = {nullptr};

/* Private function declarations ---------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/

/**
 * @brief 初始化SPI
 *
 * @param hspi SPI编号
 * @param Callback_Function 处理回调函数
 */
void SPI_Init(SPI_HandleTypeDef *hspi, SPI_Callback Callback_Function)
{
    if (hspi->Instance == SPI1)
    {
        SPI1_Manage_Object.SPI_Handler = hspi;
        SPI1_Manage_Object.Callback_Function = Callback_Function;
    }
    else if (hspi->Instance == SPI2)
    {
        SPI2_Manage_Object.SPI_Handler = hspi;
        SPI2_Manage_Object.Callback_Function = Callback_Function;
    }
    else if (hspi->Instance == SPI3)
    {
        SPI3_Manage_Object.SPI_Handler = hspi;
        SPI3_Manage_Object.Callback_Function = Callback_Function;
    }
    else if (hspi->Instance == SPI4)
    {
        SPI4_Manage_Object.SPI_Handler = hspi;
        SPI4_Manage_Object.Callback_Function = Callback_Function;
    }
    else if (hspi->Instance == SPI5)
    {
        SPI5_Manage_Object.SPI_Handler = hspi;
        SPI5_Manage_Object.Callback_Function = Callback_Function;
    }
    else if (hspi->Instance == SPI6)
    {
        SPI6_Manage_Object.SPI_Handler = hspi;
        SPI6_Manage_Object.Callback_Function = Callback_Function;
    }
}

/**
 * @brief 发送数据
 *
 * @param hspi SPI编号
 * @param GPIOx 片选GPIO引脚编组
 * @param GPIO_Pin 片选GPIO引脚号
 * @param Activate_Level 片选GPIO引脚电平
 * @param Tx_Length 长度
 * @return uint8_t 执行状态
 */
uint8_t SPI_Transmit_Data(SPI_HandleTypeDef *hspi, GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, GPIO_PinState Activate_Level, uint16_t Tx_Length)
{
    if (hspi->Instance == SPI1)
    {
        SPI1_Manage_Object.Activate_GPIOx = GPIOx;
        SPI1_Manage_Object.Activate_GPIO_Pin = GPIO_Pin;
        SPI1_Manage_Object.Activate_Level = Activate_Level;
        SPI1_Manage_Object.Tx_Buffer_Length = Tx_Length;
        SPI1_Manage_Object.Rx_Buffer_Length = 0;

        if (SPI1_Manage_Object.Activate_GPIOx != nullptr)
        {
            HAL_GPIO_WritePin(SPI1_Manage_Object.Activate_GPIOx, SPI1_Manage_Object.Activate_GPIO_Pin, Activate_Level);
        }

        return (HAL_SPI_Transmit_DMA(hspi, SPI1_Manage_Object.Tx_Buffer, Tx_Length));
    }
    else if (hspi->Instance == SPI2)
    {
        SPI2_Manage_Object.Activate_GPIOx = GPIOx;
        SPI2_Manage_Object.Activate_GPIO_Pin = GPIO_Pin;
        SPI2_Manage_Object.Activate_Level = Activate_Level;
        SPI2_Manage_Object.Tx_Buffer_Length = Tx_Length;
        SPI2_Manage_Object.Rx_Buffer_Length = 0;

        if (SPI2_Manage_Object.Activate_GPIOx != nullptr)
        {
            HAL_GPIO_WritePin(SPI2_Manage_Object.Activate_GPIOx, SPI2_Manage_Object.Activate_GPIO_Pin, Activate_Level);
        }

        return (HAL_SPI_Transmit_DMA(hspi, SPI2_Manage_Object.Tx_Buffer, Tx_Length));
    }
    else if (hspi->Instance == SPI3)
    {
        SPI3_Manage_Object.Activate_GPIOx = GPIOx;
        SPI3_Manage_Object.Activate_GPIO_Pin = GPIO_Pin;
        SPI3_Manage_Object.Activate_Level = Activate_Level;
        SPI3_Manage_Object.Tx_Buffer_Length = Tx_Length;
        SPI3_Manage_Object.Rx_Buffer_Length = 0;

        if (SPI3_Manage_Object.Activate_GPIOx != nullptr)
        {
            HAL_GPIO_WritePin(SPI3_Manage_Object.Activate_GPIOx, SPI3_Manage_Object.Activate_GPIO_Pin, Activate_Level);
        }

        return (HAL_SPI_Transmit_DMA(hspi, SPI3_Manage_Object.Tx_Buffer, Tx_Length));
    }
    else if (hspi->Instance == SPI4)
    {
        SPI4_Manage_Object.Activate_GPIOx = GPIOx;
        SPI4_Manage_Object.Activate_GPIO_Pin = GPIO_Pin;
        SPI4_Manage_Object.Activate_Level = Activate_Level;
        SPI4_Manage_Object.Tx_Buffer_Length = Tx_Length;
        SPI4_Manage_Object.Rx_Buffer_Length = 0;

        if (SPI4_Manage_Object.Activate_GPIOx != nullptr)
        {
            HAL_GPIO_WritePin(SPI4_Manage_Object.Activate_GPIOx, SPI4_Manage_Object.Activate_GPIO_Pin, Activate_Level);
        }

        return (HAL_SPI_Transmit_DMA(hspi, SPI4_Manage_Object.Tx_Buffer, Tx_Length));
    }
    else if (hspi->Instance == SPI5)
    {
        SPI5_Manage_Object.Activate_GPIOx = GPIOx;
        SPI5_Manage_Object.Activate_GPIO_Pin = GPIO_Pin;
        SPI5_Manage_Object.Activate_Level = Activate_Level;
        SPI5_Manage_Object.Tx_Buffer_Length = Tx_Length;
        SPI5_Manage_Object.Rx_Buffer_Length = 0;

        if (SPI5_Manage_Object.Activate_GPIOx != nullptr)
        {
            HAL_GPIO_WritePin(SPI5_Manage_Object.Activate_GPIOx, SPI5_Manage_Object.Activate_GPIO_Pin, Activate_Level);
        }

        return (HAL_SPI_Transmit_DMA(hspi, SPI5_Manage_Object.Tx_Buffer, Tx_Length));
    }
    else if (hspi->Instance == SPI6)
    {
        SPI6_Manage_Object.Activate_GPIOx = GPIOx;
        SPI6_Manage_Object.Activate_GPIO_Pin = GPIO_Pin;
        SPI6_Manage_Object.Activate_Level = Activate_Level;
        SPI6_Manage_Object.Tx_Buffer_Length = Tx_Length;
        SPI6_Manage_Object.Rx_Buffer_Length = 0;

        if (SPI6_Manage_Object.Activate_GPIOx != nullptr)
        {
            HAL_GPIO_WritePin(SPI6_Manage_Object.Activate_GPIOx, SPI6_Manage_Object.Activate_GPIO_Pin, Activate_Level);
        }

        // 由于SPI6需要靠BDMA传输, 而BDMA只能访问SRAM4, 即外置SRAM, 因此只能使用阻塞传输
        return (HAL_SPI_Transmit(hspi, SPI6_Manage_Object.Tx_Buffer, Tx_Length, 1));
    }

    return (HAL_ERROR);
}

/**
 * @brief 交互数据帧
 * 
 * @param hspi SPI编号
 * @param GPIOx 片选GPIO引脚编组
 * @param GPIO_Pin 片选GPIO引脚号
 * @param Activate_Level 片选GPIO引脚电平
 * @param Tx_Length 发送数据长度
 * @param Rx_Length 接收数据长度
 * @return uint8_t 执行状态
 */
uint8_t SPI_Transmit_Receive_Data(SPI_HandleTypeDef *hspi, GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, GPIO_PinState Activate_Level, uint16_t Tx_Length, uint16_t Rx_Length)
{
    if (hspi->Instance == SPI1)
    {
        SPI1_Manage_Object.Activate_GPIOx = GPIOx;
        SPI1_Manage_Object.Activate_GPIO_Pin = GPIO_Pin;
        SPI1_Manage_Object.Activate_Level = Activate_Level;
        SPI1_Manage_Object.Tx_Buffer_Length = Tx_Length;
        SPI1_Manage_Object.Rx_Buffer_Length = Rx_Length;

        if (SPI1_Manage_Object.Activate_GPIOx != nullptr)
        {
            HAL_GPIO_WritePin(SPI1_Manage_Object.Activate_GPIOx, SPI1_Manage_Object.Activate_GPIO_Pin, Activate_Level);
        }

        return (HAL_SPI_TransmitReceive_DMA(hspi, SPI1_Manage_Object.Tx_Buffer, SPI1_Manage_Object.Rx_Buffer, Tx_Length + Rx_Length));
    }
    else if (hspi->Instance == SPI2)
    {
        SPI2_Manage_Object.Activate_GPIOx = GPIOx;
        SPI2_Manage_Object.Activate_GPIO_Pin = GPIO_Pin;
        SPI2_Manage_Object.Activate_Level = Activate_Level;
        SPI2_Manage_Object.Tx_Buffer_Length = Tx_Length;
        SPI2_Manage_Object.Rx_Buffer_Length = Rx_Length;

        if (SPI2_Manage_Object.Activate_GPIOx != nullptr)
        {
            HAL_GPIO_WritePin(SPI2_Manage_Object.Activate_GPIOx, SPI2_Manage_Object.Activate_GPIO_Pin, Activate_Level);
        }

        return (HAL_SPI_TransmitReceive_DMA(hspi, SPI2_Manage_Object.Tx_Buffer, SPI2_Manage_Object.Rx_Buffer, Tx_Length + Rx_Length));
    }
    else if (hspi->Instance == SPI3)
    {
        SPI3_Manage_Object.Activate_GPIOx = GPIOx;
        SPI3_Manage_Object.Activate_GPIO_Pin = GPIO_Pin;
        SPI3_Manage_Object.Activate_Level = Activate_Level;
        SPI3_Manage_Object.Tx_Buffer_Length = Tx_Length;
        SPI3_Manage_Object.Rx_Buffer_Length = Rx_Length;

        if (SPI3_Manage_Object.Activate_GPIOx != nullptr)
        {
            HAL_GPIO_WritePin(SPI3_Manage_Object.Activate_GPIOx, SPI3_Manage_Object.Activate_GPIO_Pin, Activate_Level);
        }

        return (HAL_SPI_TransmitReceive_DMA(hspi, SPI3_Manage_Object.Tx_Buffer, SPI3_Manage_Object.Rx_Buffer, Tx_Length + Rx_Length));
    }
    else if (hspi->Instance == SPI4)
    {
        SPI4_Manage_Object.Activate_GPIOx = GPIOx;
        SPI4_Manage_Object.Activate_GPIO_Pin = GPIO_Pin;
        SPI4_Manage_Object.Activate_Level = Activate_Level;
        SPI4_Manage_Object.Tx_Buffer_Length = Tx_Length;
        SPI4_Manage_Object.Rx_Buffer_Length = Rx_Length;

        if (SPI4_Manage_Object.Activate_GPIOx != nullptr)
        {
            HAL_GPIO_WritePin(SPI4_Manage_Object.Activate_GPIOx, SPI4_Manage_Object.Activate_GPIO_Pin, Activate_Level);
        }

        return (HAL_SPI_TransmitReceive_DMA(hspi, SPI4_Manage_Object.Tx_Buffer, SPI4_Manage_Object.Rx_Buffer, Tx_Length + Rx_Length));
    }
    else if (hspi->Instance == SPI5)
    {
        SPI5_Manage_Object.Activate_GPIOx = GPIOx;
        SPI5_Manage_Object.Activate_GPIO_Pin = GPIO_Pin;
        SPI5_Manage_Object.Activate_Level = Activate_Level;
        SPI5_Manage_Object.Tx_Buffer_Length = Tx_Length;
        SPI5_Manage_Object.Rx_Buffer_Length = Rx_Length;

        if (SPI5_Manage_Object.Activate_GPIOx != nullptr)
        {
            HAL_GPIO_WritePin(SPI5_Manage_Object.Activate_GPIOx, SPI5_Manage_Object.Activate_GPIO_Pin, Activate_Level);
        }

        return (HAL_SPI_TransmitReceive_DMA(hspi, SPI5_Manage_Object.Tx_Buffer, SPI5_Manage_Object.Rx_Buffer, Tx_Length + Rx_Length));
    }
    else if (hspi->Instance == SPI6)
    {
        SPI6_Manage_Object.Activate_GPIOx = GPIOx;
        SPI6_Manage_Object.Activate_GPIO_Pin = GPIO_Pin;
        SPI6_Manage_Object.Activate_Level = Activate_Level;
        SPI6_Manage_Object.Tx_Buffer_Length = Tx_Length;
        SPI6_Manage_Object.Rx_Buffer_Length = Rx_Length;

        if (SPI6_Manage_Object.Activate_GPIOx != nullptr)
        {
            HAL_GPIO_WritePin(SPI6_Manage_Object.Activate_GPIOx, SPI6_Manage_Object.Activate_GPIO_Pin, Activate_Level);
        }

        return (HAL_SPI_TransmitReceive(hspi, SPI6_Manage_Object.Tx_Buffer, SPI6_Manage_Object.Rx_Buffer, Tx_Length + Rx_Length, 1));
    }

    return (HAL_ERROR);
}

/**
 * @brief HAL库SPI仅发送回调函数
 *
 * @param hspi SPI编号
 */
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
    if (hspi->Instance == SPI1)
    {
        if (SPI1_Manage_Object.Activate_GPIOx != nullptr)
        {
            if (SPI1_Manage_Object.Activate_Level == GPIO_PIN_SET)
            {
                HAL_GPIO_WritePin(SPI1_Manage_Object.Activate_GPIOx, SPI1_Manage_Object.Activate_GPIO_Pin, GPIO_PIN_RESET);
            }
            else
            {
                HAL_GPIO_WritePin(SPI1_Manage_Object.Activate_GPIOx, SPI1_Manage_Object.Activate_GPIO_Pin, GPIO_PIN_SET);
            }
        }
    }
    else if (hspi->Instance == SPI2)
    {
        if (SPI2_Manage_Object.Activate_GPIOx != nullptr)
        {
            if (SPI2_Manage_Object.Activate_Level == GPIO_PIN_SET)
            {
                HAL_GPIO_WritePin(SPI2_Manage_Object.Activate_GPIOx, SPI2_Manage_Object.Activate_GPIO_Pin, GPIO_PIN_RESET);
            }
            else
            {
                HAL_GPIO_WritePin(SPI2_Manage_Object.Activate_GPIOx, SPI2_Manage_Object.Activate_GPIO_Pin, GPIO_PIN_SET);
            }
        }
    }
    else if (hspi->Instance == SPI3)
    {
        if (SPI3_Manage_Object.Activate_GPIOx != nullptr)
        {
            if (SPI3_Manage_Object.Activate_Level == GPIO_PIN_SET)
            {
                HAL_GPIO_WritePin(SPI3_Manage_Object.Activate_GPIOx, SPI3_Manage_Object.Activate_GPIO_Pin, GPIO_PIN_RESET);
            }
            else
            {
                HAL_GPIO_WritePin(SPI3_Manage_Object.Activate_GPIOx, SPI3_Manage_Object.Activate_GPIO_Pin, GPIO_PIN_SET);
            }
        }
    }
    else if (hspi->Instance == SPI4)
    {
        if (SPI4_Manage_Object.Activate_GPIOx != nullptr)
        {
            if (SPI4_Manage_Object.Activate_Level == GPIO_PIN_SET)
            {
                HAL_GPIO_WritePin(SPI4_Manage_Object.Activate_GPIOx, SPI4_Manage_Object.Activate_GPIO_Pin, GPIO_PIN_RESET);
            }
            else
            {
                HAL_GPIO_WritePin(SPI4_Manage_Object.Activate_GPIOx, SPI4_Manage_Object.Activate_GPIO_Pin, GPIO_PIN_SET);
            }
        }
    }
    else if (hspi->Instance == SPI5)
    {
        if (SPI5_Manage_Object.Activate_GPIOx != nullptr)
        {
            if (SPI5_Manage_Object.Activate_Level == GPIO_PIN_SET)
            {
                HAL_GPIO_WritePin(SPI5_Manage_Object.Activate_GPIOx, SPI5_Manage_Object.Activate_GPIO_Pin, GPIO_PIN_RESET);
            }
            else
            {
                HAL_GPIO_WritePin(SPI5_Manage_Object.Activate_GPIOx, SPI5_Manage_Object.Activate_GPIO_Pin, GPIO_PIN_SET);
            }
        }
    }
    else if (hspi->Instance == SPI6)
    {
        if (SPI6_Manage_Object.Activate_GPIOx != nullptr)
        {
            if (SPI6_Manage_Object.Activate_Level == GPIO_PIN_SET)
            {
                HAL_GPIO_WritePin(SPI6_Manage_Object.Activate_GPIOx, SPI6_Manage_Object.Activate_GPIO_Pin, GPIO_PIN_RESET);
            }
            else
            {
                HAL_GPIO_WritePin(SPI6_Manage_Object.Activate_GPIOx, SPI6_Manage_Object.Activate_GPIO_Pin, GPIO_PIN_SET);
            }
        }
    }
}

/**
 * @brief HAL库SPI全双工DMA中断
 * 
 * @param hspi SPI编号
 */
void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{
    // 选择回调函数
    if (hspi->Instance == SPI1)
    {
        if (SPI1_Manage_Object.Activate_GPIOx != nullptr)
        {
            if (SPI1_Manage_Object.Activate_Level == GPIO_PIN_SET)
            {
                HAL_GPIO_WritePin(SPI1_Manage_Object.Activate_GPIOx, SPI1_Manage_Object.Activate_GPIO_Pin, GPIO_PIN_RESET);
            }
            else
            {
                HAL_GPIO_WritePin(SPI1_Manage_Object.Activate_GPIOx, SPI1_Manage_Object.Activate_GPIO_Pin, GPIO_PIN_SET);
            }
        }

        SPI1_Manage_Object.Rx_Timestamp = SYS_Timestamp.Get_Current_Timestamp();

        if (SPI1_Manage_Object.Callback_Function != nullptr)
        {
            SPI1_Manage_Object.Callback_Function(SPI1_Manage_Object.Tx_Buffer, SPI1_Manage_Object.Rx_Buffer, SPI1_Manage_Object.Tx_Buffer_Length, SPI1_Manage_Object.Rx_Buffer_Length);
        }

        SPI1_Manage_Object.Activate_GPIOx = nullptr;
    }
    else if (hspi->Instance == SPI2)
    {
        if (SPI2_Manage_Object.Activate_GPIOx != nullptr)
        {
            if (SPI2_Manage_Object.Activate_Level == GPIO_PIN_SET)
            {
                HAL_GPIO_WritePin(SPI2_Manage_Object.Activate_GPIOx, SPI2_Manage_Object.Activate_GPIO_Pin, GPIO_PIN_RESET);
            }
            else
            {
                HAL_GPIO_WritePin(SPI2_Manage_Object.Activate_GPIOx, SPI2_Manage_Object.Activate_GPIO_Pin, GPIO_PIN_SET);
            }
        }

        SPI2_Manage_Object.Rx_Timestamp = SYS_Timestamp.Get_Current_Timestamp();

        if (SPI2_Manage_Object.Callback_Function != nullptr)
        {
            SPI2_Manage_Object.Callback_Function(SPI2_Manage_Object.Tx_Buffer, SPI2_Manage_Object.Rx_Buffer, SPI2_Manage_Object.Tx_Buffer_Length, SPI2_Manage_Object.Rx_Buffer_Length);
        }

        SPI2_Manage_Object.Activate_GPIOx = nullptr;
    }
    else if (hspi->Instance == SPI3)
    {
        if (SPI3_Manage_Object.Activate_GPIOx != nullptr)
        {
            if (SPI3_Manage_Object.Activate_Level == GPIO_PIN_SET)
            {
                HAL_GPIO_WritePin(SPI3_Manage_Object.Activate_GPIOx, SPI3_Manage_Object.Activate_GPIO_Pin, GPIO_PIN_RESET);
            }
            else
            {
                HAL_GPIO_WritePin(SPI3_Manage_Object.Activate_GPIOx, SPI3_Manage_Object.Activate_GPIO_Pin, GPIO_PIN_SET);
            }
        }

        SPI3_Manage_Object.Rx_Timestamp = SYS_Timestamp.Get_Current_Timestamp();

        if (SPI3_Manage_Object.Callback_Function != nullptr)
        {
            SPI3_Manage_Object.Callback_Function(SPI3_Manage_Object.Tx_Buffer, SPI3_Manage_Object.Rx_Buffer, SPI3_Manage_Object.Tx_Buffer_Length, SPI3_Manage_Object.Rx_Buffer_Length);
        }

        SPI3_Manage_Object.Activate_GPIOx = nullptr;
    }
    else if (hspi->Instance == SPI4)
    {
        if (SPI4_Manage_Object.Activate_GPIOx != nullptr)
        {
            if (SPI4_Manage_Object.Activate_Level == GPIO_PIN_SET)
            {
                HAL_GPIO_WritePin(SPI4_Manage_Object.Activate_GPIOx, SPI4_Manage_Object.Activate_GPIO_Pin, GPIO_PIN_RESET);
            }
            else
            {
                HAL_GPIO_WritePin(SPI4_Manage_Object.Activate_GPIOx, SPI4_Manage_Object.Activate_GPIO_Pin, GPIO_PIN_SET);
            }
        }

        SPI4_Manage_Object.Rx_Timestamp = SYS_Timestamp.Get_Current_Timestamp();

        if (SPI4_Manage_Object.Callback_Function != nullptr)
        {
            SPI4_Manage_Object.Callback_Function(SPI4_Manage_Object.Tx_Buffer, SPI4_Manage_Object.Rx_Buffer, SPI4_Manage_Object.Tx_Buffer_Length, SPI4_Manage_Object.Rx_Buffer_Length);
        }

        SPI4_Manage_Object.Activate_GPIOx = nullptr;
    }
    else if (hspi->Instance == SPI5)
    {
        if (SPI5_Manage_Object.Activate_GPIOx != nullptr)
        {
            if (SPI5_Manage_Object.Activate_Level == GPIO_PIN_SET)
            {
                HAL_GPIO_WritePin(SPI5_Manage_Object.Activate_GPIOx, SPI5_Manage_Object.Activate_GPIO_Pin, GPIO_PIN_RESET);
            }
            else
            {
                HAL_GPIO_WritePin(SPI5_Manage_Object.Activate_GPIOx, SPI5_Manage_Object.Activate_GPIO_Pin, GPIO_PIN_SET);
            }
        }

        SPI5_Manage_Object.Rx_Timestamp = SYS_Timestamp.Get_Current_Timestamp();

        if (SPI5_Manage_Object.Callback_Function != nullptr)
        {
            SPI5_Manage_Object.Callback_Function(SPI5_Manage_Object.Tx_Buffer, SPI5_Manage_Object.Rx_Buffer, SPI5_Manage_Object.Rx_Buffer_Length, SPI5_Manage_Object.Rx_Buffer_Length);
        }

        SPI5_Manage_Object.Activate_GPIOx = nullptr;
    }
    else if (hspi->Instance == SPI6)
    {
        if (SPI6_Manage_Object.Activate_GPIOx != nullptr)
        {
            if (SPI6_Manage_Object.Activate_Level == GPIO_PIN_SET)
            {
                HAL_GPIO_WritePin(SPI6_Manage_Object.Activate_GPIOx, SPI6_Manage_Object.Activate_GPIO_Pin, GPIO_PIN_RESET);
            }
            else
            {
                HAL_GPIO_WritePin(SPI6_Manage_Object.Activate_GPIOx, SPI6_Manage_Object.Activate_GPIO_Pin, GPIO_PIN_SET);
            }
        }

        SPI6_Manage_Object.Rx_Timestamp = SYS_Timestamp.Get_Current_Timestamp();

        if (SPI6_Manage_Object.Callback_Function != nullptr)
        {
            SPI6_Manage_Object.Callback_Function(SPI6_Manage_Object.Tx_Buffer, SPI6_Manage_Object.Rx_Buffer, SPI6_Manage_Object.Tx_Buffer_Length, SPI6_Manage_Object.Rx_Buffer_Length);
        }

        SPI6_Manage_Object.Activate_GPIOx = nullptr;
    }
}

/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/