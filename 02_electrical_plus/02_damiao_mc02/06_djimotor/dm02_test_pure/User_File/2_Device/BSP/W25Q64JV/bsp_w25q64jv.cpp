/**
* @file bsp_w25q64jv.cpp
 * @author yssickjgd (1345578933@qq.com)
 * @brief W25Q64JV寄存器
 * @version 0.1
 * @date 2025-10-15 0.1 新建文档
 *
 * @copyright USTC-RoboWalker (c) 2025
 *
 */

/* Includes ------------------------------------------------------------------*/

#include "bsp_w25q64jv.h"

/* Private macros ------------------------------------------------------------*/

/* Private types -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

Class_W25Q64JV BSP_W25Q64JV;

/* Private function declarations ---------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/

/**
 * @brief 初始化W25Q64JV
 *
 * @param __Flash_Mode 工作模式
 */
void Class_W25Q64JV::Init(const Enum_W25Q64JV_Mode &__Flash_Mode)
{
    OSPI_Manage_Object = &OSPI2_Manage_Object;

    // 读取自身ID确认是否存活
    Command = COMMAND_DEFAULT_CONFIG;
    Command.NbData = 3;
    while (*reinterpret_cast<uint32_t *>(OSPI_Manage_Object->Rx_Buffer) != 0x001740ef)
    {
        OSPI_Command_Receive_Data(OSPI_Manage_Object->OSPI_Handler, &Command);
        Namespace_SYS_Timestamp::Delay_Millisecond(100);
    }

    if (__Flash_Mode == W25Q64JV_Mode_MemoryMapped)
    {
        // 内存映射模式
        Command = COMMAND_DEFAULT_CONFIG;
        Command.Instruction = W25Q64JV_Command_FAST_READ_QUAD_IO;
        Command.AddressMode = HAL_OSPI_ADDRESS_4_LINES;
        Command.DataMode = HAL_OSPI_DATA_4_LINES;
        Command.DummyCycles = 6;
        OSPI_Command(OSPI_Manage_Object->OSPI_Handler, &Command);
        Namespace_SYS_Timestamp::Delay_Millisecond(100);

        OSPI_MemoryMappedTypeDef tmp_config = {0};
        HAL_OSPI_MemoryMapped(OSPI_Manage_Object->OSPI_Handler, &tmp_config);
    }
}

/**
 * @brief OSPI轮询完成回调函数
 *
 */
void Class_W25Q64JV::OSPI_StatusMatchCallback()
{
    // 轮询结束
    Busy_Flag = false;

    if (Current_Instruction == W25Q64JV_Command_WRITE_ENABLE)
    {
        // 写使能成功
        Write_Enable_Activated_Flag = true;
    }
    else
    {
        // 后续可按需写特化的处理
    }
}

/**
 * @brief OSPI接收完成回调函数
 *
 */
void Class_W25Q64JV::OSPI_RxCallback()
{
    // 后续可按需写特化的处理
}

/**
 * @brief 定时器周期中断回调函数
 *
 */
void Class_W25Q64JV::TIM_1ms_AutoPollingTimeout_PeriodElapsedCallback()
{
    // 轮询超时
    if (Busy_Flag && (SYS_Timestamp.Get_Current_Timestamp() - OSPI_Manage_Object->Auto_Polling_Timestamp > Current_Auto_Polling_Timeout))
    {
        Busy_Flag = false;
        Auto_Polling_Error_Count++;
    }
}

/**
 * @brief 带超时的自动轮询
 */
void Class_W25Q64JV::Auto_Polling_With_Timeout()
{
    if (Busy_Flag)
    {
        return;
    }
    Busy_Flag = true;
    Busy_Timestamp = SYS_Timestamp.Get_Current_Timestamp();

    Command = COMMAND_DEFAULT_CONFIG;
    Command.Instruction = W25Q64JV_Command_READ_STATUS_REGISTER_1;
    Command.NbData = 1;
    OSPI_Command(OSPI_Manage_Object->OSPI_Handler, &Command);

    OSPI_AutoPollingTypeDef tmp_config = AUTO_POLLING_DEFAULT_CONFIG;
    OSPI_Auto_Polling(OSPI_Manage_Object->OSPI_Handler, &tmp_config);
}

/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/