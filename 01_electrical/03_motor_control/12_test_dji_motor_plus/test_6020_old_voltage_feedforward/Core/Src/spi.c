/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    spi.c
  * @brief   This file provides code for the configuration
  *          of the SPI instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "spi.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

SPI_HandleTypeDef hspi5;
DMA_HandleTypeDef hdma_spi5_rx;
DMA_HandleTypeDef hdma_spi5_tx;

/* SPI5 init function */
void MX_SPI5_Init(void)
{

  /* USER CODE BEGIN SPI5_Init 0 */

  /* USER CODE END SPI5_Init 0 */

  /* USER CODE BEGIN SPI5_Init 1 */

  /* USER CODE END SPI5_Init 1 */
  hspi5.Instance = SPI5;
  hspi5.Init.Mode = SPI_MODE_MASTER;
  hspi5.Init.Direction = SPI_DIRECTION_2LINES;
  hspi5.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi5.Init.CLKPolarity = SPI_POLARITY_HIGH;
  hspi5.Init.CLKPhase = SPI_PHASE_2EDGE;
  hspi5.Init.NSS = SPI_NSS_SOFT;
  hspi5.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
  hspi5.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi5.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi5.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi5.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi5) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI5_Init 2 */

  /* USER CODE END SPI5_Init 2 */

}

void HAL_SPI_MspInit(SPI_HandleTypeDef* spiHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(spiHandle->Instance==SPI5)
  {
  /* USER CODE BEGIN SPI5_MspInit 0 */

  /* USER CODE END SPI5_MspInit 0 */
    /* SPI5 clock enable */
    __HAL_RCC_SPI5_CLK_ENABLE();

    __HAL_RCC_GPIOF_CLK_ENABLE();
    /**SPI5 GPIO Configuration
    PF7     ------> SPI5_SCK
    PF9     ------> SPI5_MOSI
    PF8     ------> SPI5_MISO
    */
    GPIO_InitStruct.Pin = BoardA_SPI5_Clk_Pin|BoardA_SPI5_MOSI_Pin|BoardA_SPI5_MISO_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI5;
    HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

    /* SPI5 DMA Init */
    /* SPI5_RX Init */
    hdma_spi5_rx.Instance = DMA2_Stream3;
    hdma_spi5_rx.Init.Channel = DMA_CHANNEL_2;
    hdma_spi5_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_spi5_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_spi5_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_spi5_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_spi5_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_spi5_rx.Init.Mode = DMA_NORMAL;
    hdma_spi5_rx.Init.Priority = DMA_PRIORITY_LOW;
    hdma_spi5_rx.Init.FIFOMode = DMA_FIFOMODE_ENABLE;
    hdma_spi5_rx.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
    hdma_spi5_rx.Init.MemBurst = DMA_MBURST_SINGLE;
    hdma_spi5_rx.Init.PeriphBurst = DMA_PBURST_SINGLE;
    if (HAL_DMA_Init(&hdma_spi5_rx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(spiHandle,hdmarx,hdma_spi5_rx);

    /* SPI5_TX Init */
    hdma_spi5_tx.Instance = DMA2_Stream4;
    hdma_spi5_tx.Init.Channel = DMA_CHANNEL_2;
    hdma_spi5_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_spi5_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_spi5_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_spi5_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_spi5_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_spi5_tx.Init.Mode = DMA_NORMAL;
    hdma_spi5_tx.Init.Priority = DMA_PRIORITY_LOW;
    hdma_spi5_tx.Init.FIFOMode = DMA_FIFOMODE_ENABLE;
    hdma_spi5_tx.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
    hdma_spi5_tx.Init.MemBurst = DMA_MBURST_SINGLE;
    hdma_spi5_tx.Init.PeriphBurst = DMA_PBURST_SINGLE;
    if (HAL_DMA_Init(&hdma_spi5_tx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(spiHandle,hdmatx,hdma_spi5_tx);

    /* SPI5 interrupt Init */
    HAL_NVIC_SetPriority(SPI5_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(SPI5_IRQn);
  /* USER CODE BEGIN SPI5_MspInit 1 */

  /* USER CODE END SPI5_MspInit 1 */
  }
}

void HAL_SPI_MspDeInit(SPI_HandleTypeDef* spiHandle)
{

  if(spiHandle->Instance==SPI5)
  {
  /* USER CODE BEGIN SPI5_MspDeInit 0 */

  /* USER CODE END SPI5_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_SPI5_CLK_DISABLE();

    /**SPI5 GPIO Configuration
    PF7     ------> SPI5_SCK
    PF9     ------> SPI5_MOSI
    PF8     ------> SPI5_MISO
    */
    HAL_GPIO_DeInit(GPIOF, BoardA_SPI5_Clk_Pin|BoardA_SPI5_MOSI_Pin|BoardA_SPI5_MISO_Pin);

    /* SPI5 DMA DeInit */
    HAL_DMA_DeInit(spiHandle->hdmarx);
    HAL_DMA_DeInit(spiHandle->hdmatx);

    /* SPI5 interrupt Deinit */
    HAL_NVIC_DisableIRQ(SPI5_IRQn);
  /* USER CODE BEGIN SPI5_MspDeInit 1 */

  /* USER CODE END SPI5_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
