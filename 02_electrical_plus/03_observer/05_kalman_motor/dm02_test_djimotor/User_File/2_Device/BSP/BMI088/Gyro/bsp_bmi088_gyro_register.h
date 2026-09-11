/**
 * @file bsp_bmi088_gyro_register.h
 * @author yssickjgd (1345578933@qq.com)
 * @brief BMI088组件之陀螺仪寄存器
 * @version 0.1
 * @date 2025-08-19 0.1 新建文档
 *
 * @copyright USTC-RoboWalker (c) 2025
 *
 */

#ifndef BSP_BMI088_GYRO_REGISTER_H
#define BSP_BMI088_GYRO_REGISTER_H

/* Includes ------------------------------------------------------------------*/

#include "1_Middleware/Driver/SPI/drv_spi.h"

/* Exported macros -----------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/**
 * @brief 陀螺仪寄存器映射
 *
 */
struct Struct_BMI088_Gyro_Register
{
    uint8_t GYRO_CHIP_ID_RO;
    uint8_t Reserved_0[1];

    // 陀螺仪数据
    int16_t RATE_X_RO;
    int16_t RATE_Y_RO;
    int16_t RATE_Z_RO;

    uint8_t Reserved_1[0x09 - 0x07];

    uint8_t GYRO_INT_STAT_1_RO;
    uint8_t Reserved_2[0x0d - 0x0a];

    uint8_t FIFO_STATUS_RO;

    uint8_t GYRO_RANGE_RW;
    uint8_t GYRO_BANDWODTH_RW;
    uint8_t GYRO_LPM1_RW;
    uint8_t Reserved_3[0x13 - 0x11];

    uint8_t GYRO_SOFTRESET_WO;

    uint8_t GYRO_INT_CTRL_RW;
    uint8_t INT3_INT4_IO_CONF_RW;
    uint8_t Reserved_4[0x17 - 0x16];

    uint8_t INT3_INT4_IO_MAP_RW;
    uint8_t Reserved_5[0x1d - 0x18];

    uint8_t FIFO_WM_EN_RW;
    uint8_t Reserved_6[0x33 - 0x1e];

    uint8_t FIFO_EXT_INT_S_RW;
    uint8_t Reserved_7[0x3b - 0x34];

    uint8_t GURO_SELF_TEST_RWX;

    uint8_t FIFO_CONFIG_0_RW;
    uint8_t FIFO_CONFIG_1_RW;

    uint8_t FIFO_DATA_RO;
} __attribute__((packed));

/* Exported variables --------------------------------------------------------*/

/* Exported function declarations --------------------------------------------*/

#endif

/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/