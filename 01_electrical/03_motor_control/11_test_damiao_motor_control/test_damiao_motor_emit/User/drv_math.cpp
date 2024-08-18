/**
 * @file drv_math.cpp
 * @author yssickjgd 1345578933@qq.com
 * @brief 一些数学
 * @version 1.1
 * @date 2023-08-29 0.1 23赛季定稿
 * @date 2023-11-10 1.1 修改成cpp
 *
 * @copyright Copyright (c) 2023
 *
 */

/* Includes ------------------------------------------------------------------*/

#include "drv_math.h"

/* Private macros ------------------------------------------------------------*/

/* Private types -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function declarations ---------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/

/**
 * @brief 布尔值反转
 *
 * @param Value 布尔值地址
 */
void Math_Boolean_Logical_Not(bool *Value)
{
    if (*Value == false)
    {
        *Value = true;
    }
    else if (*Value == true)
    {
        *Value = false;
    }
}

/**
 * @brief 16位大小端转换
 *
 * @param Address 地址
 */
void Math_Endian_Reverse_16(void *Address)
{
    uint8_t *temp_address_8 = (uint8_t *) Address;
    uint16_t *temp_address_16 = (uint16_t *) Address;
    *temp_address_16 = temp_address_8[0] << 8 | temp_address_8[1];
}

/**
 * @brief 16位大小端转换
 *
 * @param Source 源数据地址
 * @param Destination 目标存储地址
 * @return uint16_t 结果
 */
uint16_t Math_Endian_Reverse_16(void *Source, void *Destination)
{
    uint8_t *temp_address_8 = (uint8_t *) Source;
    uint16_t temp_address_16;
    temp_address_16 = temp_address_8[0] << 8 | temp_address_8[1];

    if (Destination != nullptr)
    {
        uint8_t *temp_source, *temp_destination;
        temp_source = (uint8_t *) Source;
        temp_destination = (uint8_t *) Destination;

        temp_destination[0] = temp_source[1];
        temp_destination[1] = temp_source[0];
    }

    return temp_address_16;
}

/**
 * @brief 32位大小端转换
 *
 * @param Address 地址
 */
void Math_Endian_Reverse_32(void *Address)
{
    uint8_t *temp_address_8 = (uint8_t *) Address;
    uint32_t *temp_address_32 = (uint32_t *) Address;
    *temp_address_32 = temp_address_8[0] << 24 | temp_address_8[1] << 16 | temp_address_8[2] << 8 | temp_address_8[3];
}

/**
 * @brief 32位大小端转换
 *
 * @param Source 源数据地址
 * @param Destination 目标存储地址
 * @return uint32_t 结果
 */
uint32_t Math_Endian_Reverse_32(void *Source, void *Destination)
{
    uint8_t *temp_address_8 = (uint8_t *) Source;
    uint32_t temp_address_32;
    temp_address_32 = temp_address_8[0] << 24 | temp_address_8[1] << 16 | temp_address_8[2] << 8 | temp_address_8[3];

    if (Destination != nullptr)
    {
        uint8_t *temp_source, *temp_destination;
        temp_source = (uint8_t *) Source;
        temp_destination = (uint8_t *) Destination;

        temp_destination[0] = temp_source[3];
        temp_destination[1] = temp_source[2];
        temp_destination[2] = temp_source[1];
        temp_destination[3] = temp_source[0];
    }

    return temp_address_32;
}

/**
 * @brief 求和
 *
 * @param Address 起始地址
 * @param Length 被加的数据的数量, 注意不是字节数
 * @return uint8_t 结果
 */
uint8_t Math_Sum_8(uint8_t *Address, uint32_t Length)
{
    uint8_t sum = 0;
    for (int i = 0; i < Length; i++)
    {
        sum += Address[i];
    }
    return (sum);
}

/**
 * @brief 求和
 *
 * @param Address 起始地址
 * @param Length 被加的数据的数量, 注意不是字节数
 * @return uint16_t 结果
 */
uint16_t Math_Sum_16(uint16_t *Address, uint32_t Length)
{
    uint16_t sum = 0;
    for (int i = 0; i < Length; i++)
    {
        sum += Address[i];
    }
    return (sum);
}

/**
 * @brief 求和
 *
 * @param Address 起始地址
 * @param Length 被加的数据的数量, 注意不是字节数
 * @return uint32_t 结果
 */
uint32_t Math_Sum_32(uint32_t *Address, uint32_t Length)
{
    uint32_t sum = 0;
    for (int i = 0; i < Length; i++)
    {
        sum += Address[i];
    }
    return (sum);
}

/**
 * @brief sinc函数的实现
 *
 * @param x 输入
 * @return float 输出
 */
float Math_Sinc(float x)
{
    // 分母为0则按极限求法
    if (Math_Abs(x) <= 2.0f * FLT_EPSILON)
    {
        return (1.0f);
    }

    return (sin(x) / x);
}

/**
 * @brief 将浮点数映射到整型
 *
 * @param x 浮点数
 * @param Float_Min 浮点数最小值
 * @param Float_Max 浮点数最大值
 * @param Int_Min 整型最小值
 * @param Int_Max 整型最大值
 * @return int32_t 整型
 */
int32_t Math_Float_To_Int(float x, float Float_Min, float Float_Max, int32_t Int_Min, int32_t Int_Max)
{
    float tmp = (x - Float_Min) / (Float_Max - Float_Min);
    int32_t out = tmp * (float) (Int_Max - Int_Min) + Int_Min;
    return (out);
}

/**
 * @brief 将整型映射到浮点数
 *
 * @param x 整型
 * @param Int_Min 整型最小值
 * @param Int_Max 整型最大值
 * @param Float_Min 浮点数最小值
 * @param Float_Max 浮点数最大值
 * @return float 浮点数
 */
float Math_Int_To_Float(int32_t x, int32_t Int_Min, int32_t Int_Max, float Float_Min, float Float_Max)
{
    float tmp = (float) (x - Int_Min) / (float) (Int_Max - Int_Min);
    float out = tmp * (Float_Max - Float_Min) + Float_Min;
    return (out);
}

/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/
