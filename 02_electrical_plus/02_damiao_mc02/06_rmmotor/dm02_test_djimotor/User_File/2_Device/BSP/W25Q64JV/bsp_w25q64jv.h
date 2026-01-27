/**
* @file bsp_w25q64jv.h
 * @author yssickjgd (1345578933@qq.com)
 * @brief W25Q64JV寄存器
 * @version 0.1
 * @date 2025-10-15 0.1 新建文档
 *
 * @copyright USTC-RoboWalker (c) 2025
 *
 */

#ifndef BSP_W25Q64JV_H
#define BSP_W25Q64JV_H

/* Includes ------------------------------------------------------------------*/

#include "bsp_w25q64jv_register.h"
#include "1_Middleware/Driver/OSPI/drv_ospi.h"
#include "stm32h7xx_hal.h"

/* Exported macros -----------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

enum Enum_W25Q64JV_Mode
{
    W25Q64JV_Mode_Normal = 0,
    W25Q64JV_Mode_MemoryMapped,
};

/**
 * @brief Specialized, flash芯片
 *
 */
class Class_W25Q64JV
{
public:
    void Init(const Enum_W25Q64JV_Mode &__Flash_Mode = W25Q64JV_Mode_Normal);

    inline void Get_Buffer(const uint32_t &Address, const uint8_t &Length);

    inline void Set_Write_Enable();

    inline void Set_Sector_Erased(const uint32_t &Address);

    inline void Set_Bolck_Erased_32K(const uint32_t &Address);

    inline void Set_Bolck_Erased_64K(const uint32_t &Address);

    inline void Set_Chip_Erased();

    inline void Set_Buffer(const uint8_t *Buffer, const uint32_t &Address, const uint8_t &Length);

    void OSPI_StatusMatchCallback();

    void OSPI_RxCallback();

    void TIM_1ms_AutoPollingTimeout_PeriodElapsedCallback();

protected:
    // 初始化相关常量

    // 绑定的OSPI
    Struct_OSPI_Manage_Object *OSPI_Manage_Object;
    // Flash的工作模式
    Enum_W25Q64JV_Mode Flash_Mode;

    // 常量

    // W25Q64JV默认命令内容
    const OSPI_RegularCmdTypeDef COMMAND_DEFAULT_CONFIG = {.OperationType = HAL_OSPI_OPTYPE_COMMON_CFG, .FlashId = HAL_OSPI_FLASH_ID_1, .Instruction = W25Q64JV_Command_JEDEC_ID, .InstructionMode = HAL_OSPI_INSTRUCTION_1_LINE, .InstructionSize = HAL_OSPI_INSTRUCTION_8_BITS, .InstructionDtrMode = HAL_OSPI_INSTRUCTION_DTR_DISABLE, .Address = 0, .AddressMode = HAL_OSPI_ADDRESS_NONE, .AddressSize = HAL_OSPI_ADDRESS_24_BITS, .AddressDtrMode = HAL_OSPI_ADDRESS_DTR_DISABLE, .AlternateBytes = 0, .AlternateBytesMode = HAL_OSPI_ALTERNATE_BYTES_NONE, .AlternateBytesSize = 0, .AlternateBytesDtrMode = HAL_OSPI_ALTERNATE_BYTES_DTR_DISABLE, .DataMode = HAL_OSPI_DATA_1_LINE, .NbData = 0, .DataDtrMode = HAL_OSPI_DATA_DTR_DISABLE, .DummyCycles = 0, .DQSMode = HAL_OSPI_DQS_DISABLE, .SIOOMode = HAL_OSPI_SIOO_INST_EVERY_CMD,};

    // W25Q64JV默认轮询内容
    const OSPI_AutoPollingTypeDef AUTO_POLLING_DEFAULT_CONFIG = {.Match = 0x00, .Mask = 0x01, .MatchMode = HAL_OSPI_MATCH_MODE_AND, .AutomaticStop = HAL_OSPI_AUTOMATIC_STOP_ENABLE, .Interval = 5,};

    // 默认轮询超时等待, 5000ms
    const uint64_t AUTOPOLLING_DEFAULT_TIMEOUT = 5000000;
    // 清理单个sector超时等待, 400ms
    const uint64_t AUTOPOLLING_SECTOR_ERASED_TIMEOUT = 400000;
    // 清理单个32K block超时等待, 1600ms
    const uint64_t AUTOPOLLING_BLOCK_ERASED_32K_TIMEOUT = 1600000;
    // 清理单个64K block超时等待, 2000ms
    const uint64_t AUTOPOLLING_BLOCK_ERASED_64K_TIMEOUT = 2000000;
    // 清理整片chip超时等待, 100000ms
    const uint64_t AUTOPOLLING_CHIP_ERASED_TIMEOUT = 100000000;

    // 内部变量

    // 标志位
    // 写使能已激活标志位
    bool Write_Enable_Activated_Flag = false;
    // 忙标志位, 只要涉及Command, Transmit, Receive就置1, 在StatusMatch中断中恢复
    bool Busy_Flag = false;
    // 置忙时间戳
    uint64_t Busy_Timestamp = 0;

    // 指令相关变量
    OSPI_RegularCmdTypeDef Command = COMMAND_DEFAULT_CONFIG;
    uint32_t Current_Instruction = 0;

    // 轮询相关变量
    // 当前指令对应的轮询超时等待
    uint64_t Current_Auto_Polling_Timeout = AUTOPOLLING_DEFAULT_TIMEOUT;
    // 轮询错误计数
    uint32_t Auto_Polling_Error_Count = 0;

    // 读变量

    // 写变量

    // 读写变量

    // 内部函数

    void Auto_Polling_With_Timeout();
};

/* Exported variables ---------------------------------------------------------*/

extern Class_W25Q64JV BSP_W25Q64JV;

/* Exported function prototypes -----------------------------------------------*/

/**
 * @brief 写使能
 *
 */
inline void Class_W25Q64JV::Set_Write_Enable()
{
    if (Busy_Flag)
    {
        return;
    }
    Busy_Flag = true;
    Busy_Timestamp = SYS_Timestamp.Get_Current_Timestamp();

    Command = COMMAND_DEFAULT_CONFIG;
    Command.Instruction = W25Q64JV_Command_WRITE_ENABLE;
    OSPI_Command(OSPI_Manage_Object->OSPI_Handler, &Command);
    Current_Instruction = W25Q64JV_Command_WRITE_ENABLE;
    Current_Auto_Polling_Timeout = AUTOPOLLING_DEFAULT_TIMEOUT;
    Auto_Polling_With_Timeout();
}

/**
 * @brief 读数据
 *
 */
inline void Class_W25Q64JV::Get_Buffer(const uint32_t &Address, const uint8_t &Length)
{
    if (Busy_Flag)
    {
        return;
    }

    Busy_Flag = true;

    // 读数据
    Command = COMMAND_DEFAULT_CONFIG;
    Command.Instruction = W25Q64JV_Command_FAST_READ_QUAD_IO;
    Command.Address = Address;
    Command.AddressMode = HAL_OSPI_ADDRESS_4_LINES;
    Command.DataMode = HAL_OSPI_DATA_4_LINES;
    Command.DummyCycles = 6;
    Command.NbData = Length;
    OSPI_Command_Receive_Data(OSPI_Manage_Object->OSPI_Handler, &Command);

    Current_Instruction = W25Q64JV_Command_FAST_READ_QUAD_IO;

    Auto_Polling_With_Timeout();
}

/**
 * @brief 擦除单个sector
 *
 */
inline void Class_W25Q64JV::Set_Sector_Erased(const uint32_t &Address)
{
    if (Busy_Flag)
    {
        return;
    }
    Busy_Flag = true;
    Busy_Timestamp = SYS_Timestamp.Get_Current_Timestamp();

    if (!Write_Enable_Activated_Flag)
    {
        return;
    }

    if (Address % 4096)
    {
        return;
    }

    // 擦除sector
    Command = COMMAND_DEFAULT_CONFIG;
    Command.Instruction = W25Q64JV_Command_SECTOR_ERASE;
    Command.Address = Address;
    Command.AddressMode = HAL_OSPI_ADDRESS_1_LINE;
    OSPI_Command(OSPI_Manage_Object->OSPI_Handler, &Command);
    Current_Instruction = W25Q64JV_Command_SECTOR_ERASE;
    Current_Auto_Polling_Timeout = AUTOPOLLING_SECTOR_ERASED_TIMEOUT;
    Auto_Polling_With_Timeout();
}

/**
 * @brief 擦除单个32K block
 *
 */
inline void Class_W25Q64JV::Set_Bolck_Erased_32K(const uint32_t &Address)
{
    if (Busy_Flag)
    {
        return;
    }
    Busy_Flag = true;
    Busy_Timestamp = SYS_Timestamp.Get_Current_Timestamp();

    if (!Write_Enable_Activated_Flag)
    {
        return;
    }

    if (Address % 32768)
    {
        return;
    }

    // 擦除32K block
    Command = COMMAND_DEFAULT_CONFIG;
    Command.Instruction = W25Q64JV_Command_BLOCK_ERASE_32K;
    Command.Address = Address;
    Command.AddressMode = HAL_OSPI_ADDRESS_1_LINE;
    OSPI_Command(OSPI_Manage_Object->OSPI_Handler, &Command);
    Current_Instruction = W25Q64JV_Command_BLOCK_ERASE_32K;
    Current_Auto_Polling_Timeout = AUTOPOLLING_BLOCK_ERASED_32K_TIMEOUT;
    Auto_Polling_With_Timeout();
}

/**
 * @brief 擦除单个64K block
 *
 */
inline void Class_W25Q64JV::Set_Bolck_Erased_64K(const uint32_t &Address)
{
    if (Busy_Flag)
    {
        return;
    }
    Busy_Flag = true;
    Busy_Timestamp = SYS_Timestamp.Get_Current_Timestamp();

    if (!Write_Enable_Activated_Flag)
    {
        return;
    }

    if (Address % 65536)
    {
        return;
    }

    // 擦除64K block
    Command = COMMAND_DEFAULT_CONFIG;
    Command.Instruction = W25Q64JV_Command_BLOCK_ERASE_64K;
    Command.Address = Address;
    Command.AddressMode = HAL_OSPI_ADDRESS_1_LINE;
    OSPI_Command(OSPI_Manage_Object->OSPI_Handler, &Command);
    Current_Instruction = W25Q64JV_Command_BLOCK_ERASE_64K;
    Current_Auto_Polling_Timeout = AUTOPOLLING_BLOCK_ERASED_64K_TIMEOUT;
    Auto_Polling_With_Timeout();
}

/**
 * @brief 擦除整片chip
 *
 */
inline void Class_W25Q64JV::Set_Chip_Erased()
{
    if (Busy_Flag)
    {
        return;
    }
    Busy_Flag = true;
    Busy_Timestamp = SYS_Timestamp.Get_Current_Timestamp();

    if (!Write_Enable_Activated_Flag)
    {
        return;
    }

    // 擦除chip
    Command = COMMAND_DEFAULT_CONFIG;
    Command.Instruction = W25Q64JV_Command_CHIP_ERASE;
    OSPI_Command(OSPI_Manage_Object->OSPI_Handler, &Command);
    Current_Instruction = W25Q64JV_Command_CHIP_ERASE;
    Current_Auto_Polling_Timeout = AUTOPOLLING_CHIP_ERASED_TIMEOUT;
    Auto_Polling_With_Timeout();
}

/**
 * @brief 写数据
 *
 */
inline void Class_W25Q64JV::Set_Buffer(const uint8_t *Buffer, const uint32_t &Address, const uint8_t &Length)
{
    if (Busy_Flag)
    {
        return;
    }
    Busy_Flag = true;
    Busy_Timestamp = SYS_Timestamp.Get_Current_Timestamp();

    if (!Write_Enable_Activated_Flag)
    {
        return;
    }

    if (Address % 256 + Length > 256)
    {
        return;
    }

    // 写数据
    Command = COMMAND_DEFAULT_CONFIG;
    Command.Instruction = W25Q64JV_Command_QUAD_INPUT_PAGE_PROGRAM;
    Command.Address = Address;
    Command.AddressMode = HAL_OSPI_ADDRESS_1_LINE;
    Command.DataMode = HAL_OSPI_DATA_4_LINES;
    Command.NbData = Length;
    OSPI_Command_Transmit_Data(OSPI_Manage_Object->OSPI_Handler, &Command);
    Current_Instruction = W25Q64JV_Command_QUAD_INPUT_PAGE_PROGRAM;
    Current_Auto_Polling_Timeout = AUTOPOLLING_DEFAULT_TIMEOUT;
    Auto_Polling_With_Timeout();
}

#endif