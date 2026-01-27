/**
 * @file bsp_ws2812.h
 * @author yssickjgd (1345578933@qq.com)
 * @brief 板载LED灯珠
 * @version 0.1
 * @date 2025-08-14 0.1 新建文档
 *
 * @copyright USTC-RoboWalker (c) 2025
 *
 */

#ifndef BSP_WS2812_H
#define BSP_WS2812_H

/* Includes ------------------------------------------------------------------*/

#include "1_Middleware/Driver/SPI/drv_spi.h"

/* Exported macros -----------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/**
 * @brief WS2812单个灯珠颜色值
 *
 */
struct Struct_WS2812_Color
{
    uint8_t Red;
    uint8_t Green;
    uint8_t Blue;
};

/**
 * @brief Specialized, WS2812单个灯珠
 *
 */
class Class_WS2812
{
public:
    void Init(const uint8_t &__Red = 0, const uint8_t &__Green = 0, const uint8_t &__Blue = 0);

    inline void Set_Red(const uint8_t &__Red);

    inline void Set_Green(const uint8_t &__Green);

    inline void Set_Blue(const uint8_t &__Blue);

    inline void Set_RGB(const uint8_t &__Red, const uint8_t &__Green, const uint8_t &__Blue);

    inline void Set_Color(const Struct_WS2812_Color &__Color, const float &__Brightness = 1.0f);

    void TIM_10ms_Write_PeriodElapsedCallback() const;

protected:
    // 初始化相关常量

    // 绑定的SPI
    Struct_SPI_Manage_Object *SPI_Manage_Object;

    // 常量

    // 灯珠的逻辑码
    const uint8_t LEVEL_0 = 0x60;
    const uint8_t LEVEL_1 = 0x78;

    // 内部变量

    // 读变量

    // 写变量

    // RGB颜色值
    // WS2812的RGB顺序是GRB, 为方便使用, 仍采用RGB顺序
    Struct_WS2812_Color Color;

    // 读写变量

    // 内部函数
};

/* Exported variables --------------------------------------------------------*/

extern const Struct_WS2812_Color WS2812_COLOR_BLACK;
extern const Struct_WS2812_Color WS2812_COLOR_WHITE;
extern const Struct_WS2812_Color WS2812_COLOR_RED;
extern const Struct_WS2812_Color WS2812_COLOR_GREEN;
extern const Struct_WS2812_Color WS2812_COLOR_BLUE;
extern const Struct_WS2812_Color WS2812_COLOR_YELLOW;
extern const Struct_WS2812_Color WS2812_COLOR_CYAN;
extern const Struct_WS2812_Color WS2812_COLOR_MAGENTA;

extern Class_WS2812 BSP_WS2812;

/* Exported function declarations --------------------------------------------*/

/**
 * @brief 设置红色值
 *
 * @param Red 红色分量
 */
inline void Class_WS2812::Set_Red(const uint8_t &__Red)
{
    Color.Red = __Red;
    Color.Green = 0;
    Color.Blue = 0;
}

/**
 * @brief 设置绿色值
 *
 * @param Green 绿色分量
 */
inline void Class_WS2812::Set_Green(const uint8_t &__Green)
{
    Color.Red = 0;
    Color.Green = __Green;
    Color.Blue = 0;
}

/**
 * @brief 设置蓝色值
 *
 * @param Blue 蓝色分量
 */
inline void Class_WS2812::Set_Blue(const uint8_t &__Blue)
{
    Color.Red = 0;
    Color.Green = 0;
    Color.Blue = __Blue;
}

/**
 * @brief 设置RGB颜色值
 *
 * @param __Red 红色分量
 * @param __Green 绿色分量
 * @param __Blue 蓝色分量
 */
inline void Class_WS2812::Set_RGB(const uint8_t &__Red, const uint8_t &__Green, const uint8_t &__Blue)
{
    Color.Red = __Red;
    Color.Green = __Green;
    Color.Blue = __Blue;
}

/**
 * @brief 设置颜色值
 *
 * @param __Color 颜色值
 */
inline void Class_WS2812::Set_Color(const Struct_WS2812_Color &__Color, const float &__Brightness)
{
    Color.Red = (uint8_t)((float) (__Color.Red) * __Brightness);
    Color.Green = (uint8_t)((float) (__Color.Green) * __Brightness);
    Color.Blue = (uint8_t)((float) (__Color.Blue) * __Brightness);
}

#endif

/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/