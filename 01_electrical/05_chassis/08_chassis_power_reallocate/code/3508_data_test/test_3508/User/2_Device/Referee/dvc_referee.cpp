/**
 * @file dvc_referee.cpp
 * @author yssickjgd (1345578933@qq.com)
 * @brief PM01裁判系统
 * @version 0.1
 * @date 2023-08-29 0.1 23赛季定稿
 * @date 2024-01-30 1.1 适配1.6.1通信协议
 *
 * @copyright USTC-RoboWalker (c) 2023-2024
 *
 */

/* Includes ------------------------------------------------------------------*/

#include "dvc_referee.h"

/* Private macros ------------------------------------------------------------*/

/* Private types -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

// CRC8校验码
static const uint8_t crc_8_table[256] = {0x00,
                                         0x5e,
                                         0xbc,
                                         0xe2,
                                         0x61,
                                         0x3f,
                                         0xdd,
                                         0x83,
                                         0xc2,
                                         0x9c,
                                         0x7e,
                                         0x20,
                                         0xa3,
                                         0xfd,
                                         0x1f,
                                         0x41,
                                         0x9d,
                                         0xc3,
                                         0x21,
                                         0x7f,
                                         0xfc,
                                         0xa2,
                                         0x40,
                                         0x1e,
                                         0x5f,
                                         0x01,
                                         0xe3,
                                         0xbd,
                                         0x3e,
                                         0x60,
                                         0x82,
                                         0xdc,
                                         0x23,
                                         0x7d,
                                         0x9f,
                                         0xc1,
                                         0x42,
                                         0x1c,
                                         0xfe,
                                         0xa0,
                                         0xe1,
                                         0xbf,
                                         0x5d,
                                         0x03,
                                         0x80,
                                         0xde,
                                         0x3c,
                                         0x62,
                                         0xbe,
                                         0xe0,
                                         0x02,
                                         0x5c,
                                         0xdf,
                                         0x81,
                                         0x63,
                                         0x3d,
                                         0x7c,
                                         0x22,
                                         0xc0,
                                         0x9e,
                                         0x1d,
                                         0x43,
                                         0xa1,
                                         0xff,
                                         0x46,
                                         0x18,
                                         0xfa,
                                         0xa4,
                                         0x27,
                                         0x79,
                                         0x9b,
                                         0xc5,
                                         0x84,
                                         0xda,
                                         0x38,
                                         0x66,
                                         0xe5,
                                         0xbb,
                                         0x59,
                                         0x07,
                                         0xdb,
                                         0x85,
                                         0x67,
                                         0x39,
                                         0xba,
                                         0xe4,
                                         0x06,
                                         0x58,
                                         0x19,
                                         0x47,
                                         0xa5,
                                         0xfb,
                                         0x78,
                                         0x26,
                                         0xc4,
                                         0x9a,
                                         0x65,
                                         0x3b,
                                         0xd9,
                                         0x87,
                                         0x04,
                                         0x5a,
                                         0xb8,
                                         0xe6,
                                         0xa7,
                                         0xf9,
                                         0x1b,
                                         0x45,
                                         0xc6,
                                         0x98,
                                         0x7a,
                                         0x24,
                                         0xf8,
                                         0xa6,
                                         0x44,
                                         0x1a,
                                         0x99,
                                         0xc7,
                                         0x25,
                                         0x7b,
                                         0x3a,
                                         0x64,
                                         0x86,
                                         0xd8,
                                         0x5b,
                                         0x05,
                                         0xe7,
                                         0xb9,
                                         0x8c,
                                         0xd2,
                                         0x30,
                                         0x6e,
                                         0xed,
                                         0xb3,
                                         0x51,
                                         0x0f,
                                         0x4e,
                                         0x10,
                                         0xf2,
                                         0xac,
                                         0x2f,
                                         0x71,
                                         0x93,
                                         0xcd,
                                         0x11,
                                         0x4f,
                                         0xad,
                                         0xf3,
                                         0x70,
                                         0x2e,
                                         0xcc,
                                         0x92,
                                         0xd3,
                                         0x8d,
                                         0x6f,
                                         0x31,
                                         0xb2,
                                         0xec,
                                         0x0e,
                                         0x50,
                                         0xaf,
                                         0xf1,
                                         0x13,
                                         0x4d,
                                         0xce,
                                         0x90,
                                         0x72,
                                         0x2c,
                                         0x6d,
                                         0x33,
                                         0xd1,
                                         0x8f,
                                         0x0c,
                                         0x52,
                                         0xb0,
                                         0xee,
                                         0x32,
                                         0x6c,
                                         0x8e,
                                         0xd0,
                                         0x53,
                                         0x0d,
                                         0xef,
                                         0xb1,
                                         0xf0,
                                         0xae,
                                         0x4c,
                                         0x12,
                                         0x91,
                                         0xcf,
                                         0x2d,
                                         0x73,
                                         0xca,
                                         0x94,
                                         0x76,
                                         0x28,
                                         0xab,
                                         0xf5,
                                         0x17,
                                         0x49,
                                         0x08,
                                         0x56,
                                         0xb4,
                                         0xea,
                                         0x69,
                                         0x37,
                                         0xd5,
                                         0x8b,
                                         0x57,
                                         0x09,
                                         0xeb,
                                         0xb5,
                                         0x36,
                                         0x68,
                                         0x8a,
                                         0xd4,
                                         0x95,
                                         0xcb,
                                         0x29,
                                         0x77,
                                         0xf4,
                                         0xaa,
                                         0x48,
                                         0x16,
                                         0xe9,
                                         0xb7,
                                         0x55,
                                         0x0b,
                                         0x88,
                                         0xd6,
                                         0x34,
                                         0x6a,
                                         0x2b,
                                         0x75,
                                         0x97,
                                         0xc9,
                                         0x4a,
                                         0x14,
                                         0xf6,
                                         0xa8,
                                         0x74,
                                         0x2a,
                                         0xc8,
                                         0x96,
                                         0x15,
                                         0x4b,
                                         0xa9,
                                         0xf7,
                                         0xb6,
                                         0xe8,
                                         0x0a,
                                         0x54,
                                         0xd7,
                                         0x89,
                                         0x6b,
                                         0x35,};

// CRC16校验码
static const uint16_t crc_16_table[256] = {0x0000,
                                           0x1189,
                                           0x2312,
                                           0x329b,
                                           0x4624,
                                           0x57ad,
                                           0x6536,
                                           0x74bf,
                                           0x8c48,
                                           0x9dc1,
                                           0xaf5a,
                                           0xbed3,
                                           0xca6c,
                                           0xdbe5,
                                           0xe97e,
                                           0xf8f7,
                                           0x1081,
                                           0x0108,
                                           0x3393,
                                           0x221a,
                                           0x56a5,
                                           0x472c,
                                           0x75b7,
                                           0x643e,
                                           0x9cc9,
                                           0x8d40,
                                           0xbfdb,
                                           0xae52,
                                           0xdaed,
                                           0xcb64,
                                           0xf9ff,
                                           0xe876,
                                           0x2102,
                                           0x308b,
                                           0x0210,
                                           0x1399,
                                           0x6726,
                                           0x76af,
                                           0x4434,
                                           0x55bd,
                                           0xad4a,
                                           0xbcc3,
                                           0x8e58,
                                           0x9fd1,
                                           0xeb6e,
                                           0xfae7,
                                           0xc87c,
                                           0xd9f5,
                                           0x3183,
                                           0x200a,
                                           0x1291,
                                           0x0318,
                                           0x77a7,
                                           0x662e,
                                           0x54b5,
                                           0x453c,
                                           0xbdcb,
                                           0xac42,
                                           0x9ed9,
                                           0x8f50,
                                           0xfbef,
                                           0xea66,
                                           0xd8fd,
                                           0xc974,
                                           0x4204,
                                           0x538d,
                                           0x6116,
                                           0x709f,
                                           0x0420,
                                           0x15a9,
                                           0x2732,
                                           0x36bb,
                                           0xce4c,
                                           0xdfc5,
                                           0xed5e,
                                           0xfcd7,
                                           0x8868,
                                           0x99e1,
                                           0xab7a,
                                           0xbaf3,
                                           0x5285,
                                           0x430c,
                                           0x7197,
                                           0x601e,
                                           0x14a1,
                                           0x0528,
                                           0x37b3,
                                           0x263a,
                                           0xdecd,
                                           0xcf44,
                                           0xfddf,
                                           0xec56,
                                           0x98e9,
                                           0x8960,
                                           0xbbfb,
                                           0xaa72,
                                           0x6306,
                                           0x728f,
                                           0x4014,
                                           0x519d,
                                           0x2522,
                                           0x34ab,
                                           0x0630,
                                           0x17b9,
                                           0xef4e,
                                           0xfec7,
                                           0xcc5c,
                                           0xddd5,
                                           0xa96a,
                                           0xb8e3,
                                           0x8a78,
                                           0x9bf1,
                                           0x7387,
                                           0x620e,
                                           0x5095,
                                           0x411c,
                                           0x35a3,
                                           0x242a,
                                           0x16b1,
                                           0x0738,
                                           0xffcf,
                                           0xee46,
                                           0xdcdd,
                                           0xcd54,
                                           0xb9eb,
                                           0xa862,
                                           0x9af9,
                                           0x8b70,
                                           0x8408,
                                           0x9581,
                                           0xa71a,
                                           0xb693,
                                           0xc22c,
                                           0xd3a5,
                                           0xe13e,
                                           0xf0b7,
                                           0x0840,
                                           0x19c9,
                                           0x2b52,
                                           0x3adb,
                                           0x4e64,
                                           0x5fed,
                                           0x6d76,
                                           0x7cff,
                                           0x9489,
                                           0x8500,
                                           0xb79b,
                                           0xa612,
                                           0xd2ad,
                                           0xc324,
                                           0xf1bf,
                                           0xe036,
                                           0x18c1,
                                           0x0948,
                                           0x3bd3,
                                           0x2a5a,
                                           0x5ee5,
                                           0x4f6c,
                                           0x7df7,
                                           0x6c7e,
                                           0xa50a,
                                           0xb483,
                                           0x8618,
                                           0x9791,
                                           0xe32e,
                                           0xf2a7,
                                           0xc03c,
                                           0xd1b5,
                                           0x2942,
                                           0x38cb,
                                           0x0a50,
                                           0x1bd9,
                                           0x6f66,
                                           0x7eef,
                                           0x4c74,
                                           0x5dfd,
                                           0xb58b,
                                           0xa402,
                                           0x9699,
                                           0x8710,
                                           0xf3af,
                                           0xe226,
                                           0xd0bd,
                                           0xc134,
                                           0x39c3,
                                           0x284a,
                                           0x1ad1,
                                           0x0b58,
                                           0x7fe7,
                                           0x6e6e,
                                           0x5cf5,
                                           0x4d7c,
                                           0xc60c,
                                           0xd785,
                                           0xe51e,
                                           0xf497,
                                           0x8028,
                                           0x91a1,
                                           0xa33a,
                                           0xb2b3,
                                           0x4a44,
                                           0x5bcd,
                                           0x6956,
                                           0x78df,
                                           0x0c60,
                                           0x1de9,
                                           0x2f72,
                                           0x3efb,
                                           0xd68d,
                                           0xc704,
                                           0xf59f,
                                           0xe416,
                                           0x90a9,
                                           0x8120,
                                           0xb3bb,
                                           0xa232,
                                           0x5ac5,
                                           0x4b4c,
                                           0x79d7,
                                           0x685e,
                                           0x1ce1,
                                           0x0d68,
                                           0x3ff3,
                                           0x2e7a,
                                           0xe70e,
                                           0xf687,
                                           0xc41c,
                                           0xd595,
                                           0xa12a,
                                           0xb0a3,
                                           0x8238,
                                           0x93b1,
                                           0x6b46,
                                           0x7acf,
                                           0x4854,
                                           0x59dd,
                                           0x2d62,
                                           0x3ceb,
                                           0x0e70,
                                           0x1ff9,
                                           0xf78f,
                                           0xe606,
                                           0xd49d,
                                           0xc514,
                                           0xb1ab,
                                           0xa022,
                                           0x92b9,
                                           0x8330,
                                           0x7bc7,
                                           0x6a4e,
                                           0x58d5,
                                           0x495c,
                                           0x3de3,
                                           0x2c6a,
                                           0x1ef1,
                                           0x0f78};

/* Private function declarations ---------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/

/**
 * @brief 裁判系统初始化
 *
 * @param __huart 指定的UART
 * @param __Frame_Header 数据包头标
 */
void Class_Referee::Init(UART_HandleTypeDef *huart, uint8_t __Frame_Header)
{
    if (huart->Instance == USART1)
    {
        UART_Manage_Object = &UART1_Manage_Object;
    }
    else if (huart->Instance == USART2)
    {
        UART_Manage_Object = &UART2_Manage_Object;
    }
    else if (huart->Instance == USART3)
    {
        UART_Manage_Object = &UART3_Manage_Object;
    }
    else if (huart->Instance == UART4)
    {
        UART_Manage_Object = &UART4_Manage_Object;
    }
    else if (huart->Instance == UART5)
    {
        UART_Manage_Object = &UART5_Manage_Object;
    }
    else if (huart->Instance == USART6)
    {
        UART_Manage_Object = &UART6_Manage_Object;
    }
    else if (huart->Instance == UART7)
    {
        UART_Manage_Object = &UART7_Manage_Object;
    }
    else if (huart->Instance == UART8)
    {
        UART_Manage_Object = &UART8_Manage_Object;
    }
    Frame_Header = __Frame_Header;
}

/**
 * @brief UART通信发送UI函数
 *
 * @param Graphic_1 图形地址
 */
void Class_Referee::UART_Send_Interaction_UI_Graphic_1(Struct_Referee_Data_Interaction_Graphic_Config *Graphic_1)
{
    Struct_Referee_UART_Data *tmp_buffer = (Struct_Referee_UART_Data *) UART_Manage_Object->Tx_Buffer;

    // 裁判系统帧头
    tmp_buffer->Frame_Header = 0xa5;
    tmp_buffer->Data_Length = sizeof(Struct_Referee_Tx_Data_Interaction_Graphic_1) - 2;
    tmp_buffer->Sequence = Sequence;
    tmp_buffer->CRC_8 = Verify_CRC_8((uint8_t *) tmp_buffer, 4);
    tmp_buffer->Referee_Command_ID = Referee_Command_ID_INTERACTION;

    // 交互帧头
    Struct_Referee_Tx_Data_Interaction_Graphic_1 *tmp_data = (Struct_Referee_Tx_Data_Interaction_Graphic_1 *) tmp_buffer->Data;
    tmp_data->Header = Referee_Interaction_Command_ID_UI_GRAPHIC_1;
    tmp_data->Sender = Robot_Status.Robot_ID;
    tmp_data->Receiver = static_cast<Enum_Referee_Data_Robots_Client_ID>((int) (Robot_Status.Robot_ID) + 0x100);

    // UI发一个图形帧内容
    tmp_data->Graphic[0] = *Graphic_1;

    tmp_data->CRC_16 = Verify_CRC_16((uint8_t *) tmp_buffer, 7 + tmp_buffer->Data_Length);

    HAL_UART_Transmit(UART_Manage_Object->UART_Handler, (uint8_t *) tmp_buffer, 7 + sizeof(Struct_Referee_Tx_Data_Interaction_Graphic_1), 100);

    Sequence++;
}

/**
 * @brief UART通信发送UI函数
 *
 * @param Graphic_1 图形地址
 * @param Graphic_2 图形地址
 */
void Class_Referee::UART_Send_Interaction_UI_Graphic_2(Struct_Referee_Data_Interaction_Graphic_Config *Graphic_1, Struct_Referee_Data_Interaction_Graphic_Config *Graphic_2)
{
    Struct_Referee_UART_Data *tmp_buffer = (Struct_Referee_UART_Data *) UART_Manage_Object->Tx_Buffer;

    // 裁判系统帧头
    tmp_buffer->Frame_Header = 0xa5;
    tmp_buffer->Data_Length = sizeof(Struct_Referee_Tx_Data_Interaction_Graphic_2) - 2;
    tmp_buffer->Sequence = Sequence;
    tmp_buffer->CRC_8 = Verify_CRC_8((uint8_t *) tmp_buffer, 4);
    tmp_buffer->Referee_Command_ID = Referee_Command_ID_INTERACTION;

    // 交互帧头
    Struct_Referee_Tx_Data_Interaction_Graphic_2 *tmp_data = (Struct_Referee_Tx_Data_Interaction_Graphic_2 *) tmp_buffer->Data;
    tmp_data->Header = Referee_Interaction_Command_ID_UI_GRAPHIC_2;
    tmp_data->Sender = Robot_Status.Robot_ID;
    tmp_data->Receiver = static_cast<Enum_Referee_Data_Robots_Client_ID>((int) (Robot_Status.Robot_ID) + 0x100);

    // UI发两个图形帧内容
    tmp_data->Graphic[0] = *Graphic_1;
    tmp_data->Graphic[1] = *Graphic_2;

    tmp_data->CRC_16 = Verify_CRC_16((uint8_t *) tmp_buffer, 7 + tmp_buffer->Data_Length);

    HAL_UART_Transmit(UART_Manage_Object->UART_Handler, (uint8_t *) tmp_buffer, 7 + sizeof(Struct_Referee_Tx_Data_Interaction_Graphic_2), 100);

    Sequence++;
}

/**
 * @brief UART通信发送UI函数
 *
 * @param Graphic_1 图形地址
 * @param Graphic_2 图形地址
 * @param Graphic_3 图形地址
 * @param Graphic_4 图形地址
 * @param Graphic_5 图形地址
 */
void Class_Referee::UART_Send_Interaction_UI_Graphic_5(Struct_Referee_Data_Interaction_Graphic_Config *Graphic_1, Struct_Referee_Data_Interaction_Graphic_Config *Graphic_2, Struct_Referee_Data_Interaction_Graphic_Config *Graphic_3, Struct_Referee_Data_Interaction_Graphic_Config *Graphic_4, Struct_Referee_Data_Interaction_Graphic_Config *Graphic_5)
{
    Struct_Referee_UART_Data *tmp_buffer = (Struct_Referee_UART_Data *) UART_Manage_Object->Tx_Buffer;

    // 裁判系统帧头
    tmp_buffer->Frame_Header = 0xa5;
    tmp_buffer->Data_Length = sizeof(Struct_Referee_Tx_Data_Interaction_Graphic_5) - 2;
    tmp_buffer->Sequence = Sequence;
    tmp_buffer->CRC_8 = Verify_CRC_8((uint8_t *) tmp_buffer, 4);
    tmp_buffer->Referee_Command_ID = Referee_Command_ID_INTERACTION;

    // 交互帧头
    Struct_Referee_Tx_Data_Interaction_Graphic_5 *tmp_data = (Struct_Referee_Tx_Data_Interaction_Graphic_5 *) tmp_buffer->Data;
    tmp_data->Header = Referee_Interaction_Command_ID_UI_GRAPHIC_5;
    tmp_data->Sender = Robot_Status.Robot_ID;
    tmp_data->Receiver = static_cast<Enum_Referee_Data_Robots_Client_ID>((int) (Robot_Status.Robot_ID) + 0x100);

    // UI发五个图形帧内容
    tmp_data->Graphic[0] = *Graphic_1;
    tmp_data->Graphic[1] = *Graphic_2;
    tmp_data->Graphic[2] = *Graphic_3;
    tmp_data->Graphic[3] = *Graphic_4;
    tmp_data->Graphic[4] = *Graphic_5;

    tmp_data->CRC_16 = Verify_CRC_16((uint8_t *) tmp_buffer, 7 + tmp_buffer->Data_Length);

    HAL_UART_Transmit(UART_Manage_Object->UART_Handler, (uint8_t *) tmp_buffer, 7 + sizeof(Struct_Referee_Tx_Data_Interaction_Graphic_5), 100);

    Sequence++;
}

/**
 * @brief UART通信发送UI函数
 *
 * @param Graphic_1 图形地址
 * @param Graphic_2 图形地址
 * @param Graphic_3 图形地址
 * @param Graphic_4 图形地址
 * @param Graphic_5 图形地址
 * @param Graphic_6 图形地址
 * @param Graphic_7 图形地址
 */
void Class_Referee::UART_Send_Interaction_UI_Graphic_7(Struct_Referee_Data_Interaction_Graphic_Config *Graphic_1, Struct_Referee_Data_Interaction_Graphic_Config *Graphic_2, Struct_Referee_Data_Interaction_Graphic_Config *Graphic_3, Struct_Referee_Data_Interaction_Graphic_Config *Graphic_4, Struct_Referee_Data_Interaction_Graphic_Config *Graphic_5, Struct_Referee_Data_Interaction_Graphic_Config *Graphic_6, Struct_Referee_Data_Interaction_Graphic_Config *Graphic_7)
{
    Struct_Referee_UART_Data *tmp_buffer = (Struct_Referee_UART_Data *) UART_Manage_Object->Tx_Buffer;

    // 裁判系统帧头
    tmp_buffer->Frame_Header = 0xa5;
    tmp_buffer->Data_Length = sizeof(Struct_Referee_Tx_Data_Interaction_Graphic_7) - 2;
    tmp_buffer->Sequence = Sequence;
    tmp_buffer->CRC_8 = Verify_CRC_8((uint8_t *) tmp_buffer, 4);
    tmp_buffer->Referee_Command_ID = Referee_Command_ID_INTERACTION;

    // 交互帧头
    Struct_Referee_Tx_Data_Interaction_Graphic_7 *tmp_data = (Struct_Referee_Tx_Data_Interaction_Graphic_7 *) tmp_buffer->Data;
    tmp_data->Header = Referee_Interaction_Command_ID_UI_GRAPHIC_7;
    tmp_data->Sender = Robot_Status.Robot_ID;
    tmp_data->Receiver = static_cast<Enum_Referee_Data_Robots_Client_ID>((int) (Robot_Status.Robot_ID) + 0x100);

    // UI发七个图形帧内容
    tmp_data->Graphic[0] = *Graphic_1;
    tmp_data->Graphic[1] = *Graphic_2;
    tmp_data->Graphic[2] = *Graphic_3;
    tmp_data->Graphic[3] = *Graphic_4;
    tmp_data->Graphic[4] = *Graphic_5;
    tmp_data->Graphic[5] = *Graphic_6;
    tmp_data->Graphic[6] = *Graphic_7;

    tmp_data->CRC_16 = Verify_CRC_16((uint8_t *) tmp_buffer, 7 + tmp_buffer->Data_Length);

    HAL_UART_Transmit(UART_Manage_Object->UART_Handler, (uint8_t *) tmp_buffer, 7 + sizeof(Struct_Referee_Tx_Data_Interaction_Graphic_7), 100);

    Sequence++;
}

/**
 * @brief UART通信发送UI函数
 *
 * @param Graphic_String 图形地址
 * @param String_Content 字符串地址
 */
void Class_Referee::UART_Send_Interaction_UI_Graphic_String(Struct_Referee_Data_Interaction_Graphic_Config *Graphic_String, const char *String_Content)
{
    Struct_Referee_UART_Data *tmp_buffer = (Struct_Referee_UART_Data *) UART_Manage_Object->Tx_Buffer;

    // 裁判系统帧头
    tmp_buffer->Frame_Header = 0xa5;
    tmp_buffer->Data_Length = sizeof(Struct_Referee_Tx_Data_Interaction_Graphic_String) - 2;
    tmp_buffer->Sequence = Sequence;
    tmp_buffer->CRC_8 = Verify_CRC_8((uint8_t *) tmp_buffer, 4);
    tmp_buffer->Referee_Command_ID = Referee_Command_ID_INTERACTION;

    // 交互帧头
    Struct_Referee_Tx_Data_Interaction_Graphic_String *tmp_data = (Struct_Referee_Tx_Data_Interaction_Graphic_String *) tmp_buffer->Data;
    tmp_data->Header = Referee_Interaction_Command_ID_UI_GRAPHIC_STRING;
    tmp_data->Sender = Robot_Status.Robot_ID;
    tmp_data->Receiver = static_cast<Enum_Referee_Data_Robots_Client_ID>((int) (Robot_Status.Robot_ID) + 0x100);

    // UI发字符串帧内容
    tmp_data->Graphic_String = *Graphic_String;
    bzero(tmp_data->String, 30);
    strcpy((char *) tmp_data->String, String_Content);

    tmp_data->CRC_16 = Verify_CRC_16((uint8_t *) tmp_buffer, 7 + tmp_buffer->Data_Length);

    HAL_UART_Transmit(UART_Manage_Object->UART_Handler, (uint8_t *) tmp_buffer, 7 + sizeof(Struct_Referee_Tx_Data_Interaction_Graphic_String), 80);

    Sequence++;
}

/**
 * @brief UART通信接收回调函数
 *
 * @param Rx_Data 接收的数据
 */
void Class_Referee::UART_RxCpltCallback(uint8_t *Rx_Data, uint16_t Length)
{
    // 滑动窗口, 判断裁判系统是否在线
    Flag += 1;

    Data_Process(Length);
}

/**
 * @brief TIM定时器中断定期检测裁判系统是否存活
 *
 */
void Class_Referee::TIM_1000ms_Alive_PeriodElapsedCallback()
{
    // 判断该时间段内是否接收过裁判系统数据
    if (Flag == Pre_Flag)
    {
        // 裁判系统断开连接
        Referee_Status = Referee_Status_DISABLE;

        UART_Reinit(UART_Manage_Object->UART_Handler);
    }
    else
    {
        // 裁判系统保持连接
        Referee_Status = Referee_Status_ENABLE;
    }
    Pre_Flag = Flag;
}

/**
 * @brief 裁判系统8bit循环冗余检验
 *
 * @param Message 消息
 * @param Length 字节数
 * @return uint8_t 校验码
 */
uint8_t Class_Referee::Verify_CRC_8(uint8_t *Message, uint32_t Length)
{
    uint8_t index;
    uint8_t check = 0xff;

    if (Message == nullptr)
    {
        return (check);
    }

    while (Length--)
    {
        index = *Message;
        Message++;
        check = crc_8_table[check ^ index];
    }
    return (check);
}

/**
 * @brief 裁判系统16bit循环冗余检验
 *
 * @param Message 消息
 * @param Length 字节数
 * @return uint8_t 校验码
 */
uint16_t Class_Referee::Verify_CRC_16(uint8_t *Message, uint32_t Length)
{
    uint8_t index;
    uint16_t check = 0xffff;

    if (Message == nullptr)
    {
        return (check);
    }

    while (Length--)
    {
        index = *Message;
        Message++;
        check = ((uint16_t) (check) >> 8) ^ crc_16_table[((uint16_t) (check) ^ (uint16_t) (index)) & 0xff];
    }
    return (check);
}

/**
 * @brief 数据处理过程, 为节约性能不作校验但提供了接口
 * 如遇到大规模丢包或错乱现象, 可重新启用校验过程
 *
 */
void Class_Referee::Data_Process(uint16_t Length)
{
    // 数据处理过程
    Struct_Referee_UART_Data *tmp_buffer;

    for (int i = 0; i < Length;)
    {
        tmp_buffer = (Struct_Referee_UART_Data *) &UART_Manage_Object->Rx_Buffer[i];

        // 未通过头校验
        if (tmp_buffer->Frame_Header != Frame_Header)
        {
            i++;
            continue;
        }
        // 未通过CRC8校验, 顺一位继续判断
        if (Verify_CRC_8((uint8_t *) tmp_buffer, 4) != tmp_buffer->CRC_8)
        {
            i++;
            continue;
        }
        // 未通过CRC16校验, 跨过当前包继续判断
        if (Verify_CRC_16((uint8_t *) tmp_buffer, 7 + tmp_buffer->Data_Length) != *(uint16_t *) ((uint32_t) tmp_buffer + 7 + tmp_buffer->Data_Length))
        {
            i += 9 + tmp_buffer->Data_Length;
            continue;
        }
        // 通过校验但帧不够长
        if (i + 7 + tmp_buffer->Data_Length + 2 > Length)
        {
            break;
        }

        switch (tmp_buffer->Referee_Command_ID)
        {
        case (Referee_Command_ID_GAME_STATUS):
        {
            memcpy(&Game_Status, tmp_buffer->Data, sizeof(Struct_Referee_Rx_Data_Game_Status));

            break;
        }
        case (Referee_Command_ID_GAME_RESULT):
        {
            memcpy(&Game_Result, tmp_buffer->Data, sizeof(Struct_Referee_Rx_Data_Game_Result));

            break;
        }
        case (Referee_Command_ID_GAME_ROBOT_HP):
        {
            memcpy(&Game_Robot_HP, tmp_buffer->Data, sizeof(Struct_Referee_Rx_Data_Game_Robot_HP));

            break;
        }
        case (Referee_Command_ID_EVENT_SELF_DATA):
        {
            memcpy(&Event_Self_Data, tmp_buffer->Data, sizeof(Struct_Referee_Rx_Data_Event_Self_Data));

            break;
        }
        case (Referee_Command_ID_EVENT_SELF_SUPPLY):
        {
            memcpy(&Event_Self_Supply, tmp_buffer->Data, sizeof(Struct_Referee_Rx_Data_Event_Self_Supply));

            break;
        }
        case (Referee_Command_ID_EVENT_SELF_REFEREE_WARNING):
        {
            memcpy(&Event_Referee_Warning, tmp_buffer->Data, sizeof(Struct_Referee_Rx_Data_Event_Referee_Warning));

            break;
        }
        case (Referee_Command_ID_EVENT_SELF_DART_STATUS):
        {
            memcpy(&Event_Dart_Status, tmp_buffer->Data, sizeof(Struct_Referee_Rx_Data_Event_Dart_Status));

            break;
        }
        case (Referee_Command_ID_ROBOT_STATUS):
        {
            memcpy(&Robot_Status, tmp_buffer->Data, sizeof(Struct_Referee_Rx_Data_Robot_Status));

            break;
        }
        case (Referee_Command_ID_ROBOT_POWER_HEAT):
        {
            memcpy(&Robot_Power_Heat, tmp_buffer->Data, sizeof(Struct_Referee_Rx_Data_Robot_Power_Heat));

            break;
        }
        case (Referee_Command_ID_ROBOT_POSITION):
        {
            memcpy(&Robot_Position, tmp_buffer->Data, sizeof(Struct_Referee_Rx_Data_Robot_Position));

            break;
        }
        case (Referee_Command_ID_ROBOT_BUFF):
        {
            memcpy(&Robot_Buff, tmp_buffer->Data, sizeof(Struct_Referee_Rx_Data_Robot_Buff));

            break;
        }
        case (Referee_Command_ID_ROBOT_AERIAL_STATUS):
        {
            memcpy(&Robot_Aerial_Status, tmp_buffer->Data, sizeof(Struct_Referee_Rx_Data_Robot_Aerial_Status));

            break;
        }
        case (Referee_Command_ID_ROBOT_DAMAGE):
        {
            memcpy(&Robot_Damage, tmp_buffer->Data, sizeof(Struct_Referee_Rx_Data_Robot_Damage));

            break;
        }
        case (Referee_Command_ID_ROBOT_BOOSTER):
        {
            memcpy(&Robot_Booster, tmp_buffer->Data, sizeof(Struct_Referee_Rx_Data_Robot_Booster));

            break;
        }
        case (Referee_Command_ID_ROBOT_REMAINING_AMMO):
        {
            memcpy(&Robot_Remaining_Ammo, tmp_buffer->Data, sizeof(Struct_Referee_Rx_Data_Robot_Remaining_Ammo));

            break;
        }
        case (Referee_Command_ID_ROBOT_RFID):
        {
            memcpy(&Robot_RFID, tmp_buffer->Data, sizeof(Struct_Referee_Rx_Data_Robot_RFID));

            break;
        }
        case (Referee_Command_ID_ROBOT_DART_COMMAND):
        {
            memcpy(&Robot_Dart_Command, tmp_buffer->Data, sizeof(Struct_Referee_Rx_Data_Robot_Dart_Command));

            break;
        }
        case (Referee_Command_ID_ROBOT_SENTRY_LOCATION):
        {
            memcpy(&Robot_Dart_Command, tmp_buffer->Data, sizeof(Struct_Referee_Rx_Data_Robot_Sentry_Location));

            break;
        }
        case (Referee_Command_ID_ROBOT_RADAR_MARK):
        {
            memcpy(&Robot_Dart_Command, tmp_buffer->Data, sizeof(Struct_Referee_Rx_Data_Robot_Radar_Mark));

            break;
        }
        case (Referee_Command_ID_ROBOT_SENTRY_DECISION):
        {
            memcpy(&Robot_Dart_Command, tmp_buffer->Data, sizeof(Struct_Referee_Rx_Data_Robot_Sentry_Decision));

            break;
        }
        case (Referee_Command_ID_ROBOT_RADAR_DECISION):
        {
            memcpy(&Robot_Dart_Command, tmp_buffer->Data, sizeof(Struct_Referee_Rx_Data_Robot_Radar_Decision));

            break;
        }
        }

        // 缓冲区直接推移
        i += 7 + tmp_buffer->Data_Length + 2;
    }
}

/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/
