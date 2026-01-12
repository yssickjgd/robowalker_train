/**
 * @file bsp_w25q64jv_register.h
 * @author yssickjgd (1345578933@qq.com)
 * @brief W25Q64JV寄存器
 * @version 0.1
 * @date 2025-10-13 0.1 新建文档
 *
 * @copyright USTC-RoboWalker (c) 2025
 *
 */

#ifndef BSP_W25Q64JV_REGISTER_H
#define BSP_W25Q64JV_REGISTER_H

/* Includes ------------------------------------------------------------------*/

#include "1_Middleware/Driver/OSPI/drv_ospi.h"

/* Exported macros -----------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

enum Enum_W25Q64JV_Command : uint8_t
{
    // 标准SPI命令

    // 读写状态寄存器
    W25Q64JV_Command_READ_STATUS_REGISTER_1 = 0x05,
    W25Q64JV_Command_READ_STATUS_REGISTER_2 = 0x35,
    W25Q64JV_Command_READ_STATUS_REGISTER_3 = 0x15,
    W25Q64JV_Command_WRITE_STATUS_REGISTER_1 = 0x01,
    W25Q64JV_Command_WRITE_STATUS_REGISTER_2 = 0x31,
    W25Q64JV_Command_WRITE_STATUS_REGISTER_3 = 0x11,

    // 读数据, 仅在标准SPI模式下使用, 默认50MHz
    W25Q64JV_Command_READ_DATA = 0x03,
    // 快速读数据, 比普通读数据多了个dummy字节, 给芯片反应时间, 可达104或133MHz
    W25Q64JV_Command_FAST_READ = 0x0b,

    // 擦除命令
    W25Q64JV_Command_SECTOR_ERASE = 0x20,
    W25Q64JV_Command_BLOCK_ERASE_32K = 0x52,
    W25Q64JV_Command_BLOCK_ERASE_64K = 0xd8,
    W25Q64JV_Command_CHIP_ERASE = 0xc7,

    // 擦除/写入暂停与恢复, 暂停后无需写使能即可发送恢复命令
    W25Q64JV_Command_ERASE_PROGRAM_SUSPEND = 0x75,
    W25Q64JV_Command_ERASE_PROGRAM_RESUME = 0x7a,

    // 写使能
    W25Q64JV_Command_WRITE_ENABLE = 0x06,
    // 写失能
    W25Q64JV_Command_WRITE_DISABLE = 0x04,
    // 易失性SR写入使能, 仅对下次写入有效, 后面写使能再传数据可跳过寄存器的写保护
    W25Q64JV_Command_VOLATILE_SR_WRITE_ENABLE = 0x50,

    // 页编程, 每次最多256B
    W25Q64JV_Command_PAGE_PROGRAM = 0x02,

    // 读取安全寄存器
    W25Q64JV_Command_READ_SECURITY_REGISTER = 0x48,
    // 擦除安全寄存器
    W25Q64JV_Command_ERASE_SECURITY_REGISTER = 0x44,
    // 编程安全寄存器, 每次最多256B
    W25Q64JV_Command_PROGRAM_SECURITY_REGISTER = 0x42,

    // 读取块锁定状态
    W25Q64JV_Command_READ_BLOCK_LOCK = 0x3d,
    // 单独块锁定与解锁
    W25Q64JV_Command_INDIVIDUAL_BLOCK_LOCK = 0x36,
    W25Q64JV_Command_INDIVIDUAL_BLOCK_UNLOCK = 0x39,
    // 全局块锁定与解锁
    W25Q64JV_Command_GLOBAL_BLOCK_LOCK = 0x7e,
    W25Q64JV_Command_GLOBAL_BLOCK_UNLOCK = 0x98,

    // 使能与复位命令, 要按照66, 66, 99的顺序发送才可以复位芯片
    W25Q64JV_Command_ENABLE_RESET = 0x66,
    W25Q64JV_Command_RESET_DEVICE = 0x99,

    // 进入掉电模式
    W25Q64JV_Command_POWER_DOWN = 0xb9,
    // 退出掉电模式, 亦可获取器件的电子标识
    W25Q64JV_Command_RELEASE_POWER_DOWN = 0xab,

    // 读取生产商和器件ID
    W25Q64JV_Command_MFTR_DEVICE_ID = 0x90,

    // 读取器件唯一ID
    W25Q64JV_Command_READ_UNIQUE_ID = 0x4b,

    // 读取旧版JEDEC ID
    W25Q64JV_Command_JEDEC_ID = 0x9f,

    // 读取SFDP寄存器, 用于读取器件参数
    W25Q64JV_Command_READ_SFDP_REGISTER = 0x5a,

    // 多线SPI命令

    // 1-1-2模式下快速读数据
    W25Q64JV_Command_FAST_READ_DUAL_OUTPUT = 0x3b,
    // 1-2-2模式下快速读数据
    W25Q64JV_Command_FAST_READ_DUAL_IO = 0xbb,
    // 1-1-4模式下快速读数据
    W25Q64JV_Command_FAST_READ_QUAD_OUTPUT = 0x6b,
    // 1-4-4模式下快速读数据
    W25Q64JV_Command_FAST_READ_QUAD_IO = 0xeb,

    // 1-1-4模式下快速页编程, 每次最多256B
    W25Q64JV_Command_QUAD_INPUT_PAGE_PROGRAM = 0x32,

    // 1-4-4模式下设置突发并环绕, 下次1-4-4模式下快速读数据会在对应地址的当前页面环绕读取
    W25Q64JV_Command_SET_BURST_WITH_WRAP = 0x77,

    // 1-2-2模式下读取生产商和器件ID
    W25Q64JV_Command_MFTR_DEVICE_ID_DUAL_IO = 0x92,
    // 1-4-4模式下读取生产商和器件ID
    W25Q64JV_Command_MFTR_DEVICE_ID_QUAD_IO = 0x94,
};

/* Exported variables ---------------------------------------------------------*/

/* Exported function prototypes -----------------------------------------------*/

#endif