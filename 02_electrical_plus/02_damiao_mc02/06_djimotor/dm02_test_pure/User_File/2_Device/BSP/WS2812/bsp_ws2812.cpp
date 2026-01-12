/**
 * @file bsp_ws2812.cpp
 * @author yssickjgd (1345578933@qq.com)
 * @brief 板载LED灯珠
 * @version 0.1
 * @date 2025-08-14 0.1 新建文档
 *
 * @copyright USTC-RoboWalker (c) 2025
 *
 */

/* Includes ------------------------------------------------------------------*/

#include "bsp_ws2812.h"

/* Private macros ------------------------------------------------------------*/

/* Private types -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

// 纯黑
const Struct_WS2812_Color WS2812_COLOR_BLACK = {0, 0, 0};
// 纯白
const Struct_WS2812_Color WS2812_COLOR_WHITE = {255, 255, 255};

// 纯红
const Struct_WS2812_Color WS2812_COLOR_RED = {255, 0, 0};
// 纯绿
const Struct_WS2812_Color WS2812_COLOR_GREEN = {0, 255, 0};
// 纯蓝
const Struct_WS2812_Color WS2812_COLOR_BLUE = {0, 0, 255};

// 黄色
const Struct_WS2812_Color WS2812_COLOR_YELLOW = {255, 255, 0};
// 青色
const Struct_WS2812_Color WS2812_COLOR_CYAN = {0, 255, 255};
// 品红
const Struct_WS2812_Color WS2812_COLOR_MAGENTA = {255, 0, 255};

Class_WS2812 BSP_WS2812;

/* Private function declarations ---------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/

/**
 * @brief WS2812单个灯珠初始化, 默认灯灭
 *
 * @param __Red 红色分量
 * @param __Green 绿色分量
 * @param __Blue 蓝色分量
 */
void Class_WS2812::Init(const uint8_t &__Red, const uint8_t &__Green, const uint8_t &__Blue)
{
    SPI_Manage_Object = &SPI6_Manage_Object;

    Color.Red = __Red;
    Color.Green = __Green;
    Color.Blue = __Blue;
}

/**
 * @brief TIM定时器中断增加数据到发送缓冲区
 *
 */
void Class_WS2812::TIM_10ms_Write_PeriodElapsedCallback() const
{
    uint8_t *tmp_buffer = SPI_Manage_Object->Tx_Buffer;
    uint8_t res = 0;

    bzero(tmp_buffer, SPI_BUFFER_SIZE);
    for (uint8_t i = 0; i < 8; i++)
    {
        tmp_buffer[7 - i] = (Color.Green & (1 << i)) ? LEVEL_1 : LEVEL_0;
        tmp_buffer[15 - i] = (Color.Red & (1 << i)) ? LEVEL_1 : LEVEL_0;
        tmp_buffer[23 - i] = (Color.Blue & (1 << i)) ? LEVEL_1 : LEVEL_0;
    }

    SPI_Transmit_Data(SPI_Manage_Object->SPI_Handler, nullptr, 0, GPIO_PIN_SET, 25);
}

/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/