/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    BSP_ADC_DRIVER.c
  * @brief   This file provides the implementation of the ADC BSP driver
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 chx.
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
#include "bsp_adc_driver.h"

/* Private typedef -----------------------------------------------------------*/
typedef struct {
    ADC_HandleTypeDef *hadc;
    uint32_t channel;
    uint8_t initialized;
    uint16_t last_value;
} ADC_HW_Map_t;

/* Private define ------------------------------------------------------------*/
/* 根据实际 CubeMX 生成的 ADC 句柄修改 */
extern ADC_HandleTypeDef hadc1;
#ifdef ADC2
extern ADC_HandleTypeDef hadc2;
#endif
#ifdef ADC3
extern ADC_HandleTypeDef hadc3;
#endif

/* Private variables ---------------------------------------------------------*/
static ADC_HW_Map_t adc_hw_map[ADC_COUNT] = {
    [ADC_CH1] = {&hadc1, ADC_CHANNEL_0, 0, 0},
#if ADC_COUNT > 1
    [ADC_CH2] = {&hadc1, ADC_CHANNEL_2, 0, 0},
#endif
#if ADC_COUNT > 2
    [ADC_CH3] = {&hadc1, ADC_CHANNEL_3, 0, 0},
#endif
};

/* Private function prototypes -----------------------------------------------*/
static int BSP_ADC_IsValid(ADC_Num_t adc);

/* Exported functions --------------------------------------------------------*/

/**
 * @brief 初始化指定 ADC 通道
 * @param adc: ADC 通道编号
 * @retval BSP_ADC_Status_t
 */
BSP_ADC_Status_t BSP_ADC_Init(ADC_Num_t adc)
{
    if (!BSP_ADC_IsValid(adc)) {
        return BSP_ADC_ERR_INVALID_ID;
    }

    ADC_HW_Map_t *dev = &adc_hw_map[adc];


    dev->initialized = 1;
    dev->last_value = 0;

    return BSP_ADC_OK;
}

/**
 * @brief 反初始化指定 ADC 通道
 * @param adc: ADC 通道编号
 * @retval BSP_ADC_Status_t
 */
BSP_ADC_Status_t BSP_ADC_DeInit(ADC_Num_t adc)
{
    if (!BSP_ADC_IsValid(adc)) {
        return BSP_ADC_ERR_INVALID_ID;
    }

    ADC_HW_Map_t *dev = &adc_hw_map[adc];

    HAL_ADC_Stop(dev->hadc);
    dev->initialized = 0;

    return BSP_ADC_OK;
}

/**
 * @brief 启动 ADC 阻塞采样并返回一个值
 * @param adc: ADC 通道编号
 * @retval BSP_ADC_Status_t
 */
BSP_ADC_Status_t BSP_ADC_Start(ADC_Num_t adc)
{
    if (!BSP_ADC_IsValid(adc)) return BSP_ADC_ERR_INVALID_ID;

    ADC_HW_Map_t *dev = &adc_hw_map[adc];
    if (!dev->initialized) return BSP_ADC_ERR_NOT_INITIALIZED;

    if (HAL_ADC_Start(dev->hadc) != HAL_OK)
        return BSP_ADC_ERR_HW;

    if (HAL_ADC_PollForConversion(dev->hadc, 10) != HAL_OK)
        return BSP_ADC_ERR_HW;

    dev->last_value = HAL_ADC_GetValue(dev->hadc);
    return BSP_ADC_OK;
}

/**
 * @brief 启动中断方式采样
 * @param adc: ADC 通道编号
 * @retval BSP_ADC_Status_t
 */
BSP_ADC_Status_t BSP_ADC_Start_IT(ADC_Num_t adc)
{
    if (!BSP_ADC_IsValid(adc)) return BSP_ADC_ERR_INVALID_ID;

    ADC_HW_Map_t *dev = &adc_hw_map[adc];
    if (!dev->initialized) return BSP_ADC_ERR_NOT_INITIALIZED;

    if (HAL_ADC_Start_IT(dev->hadc) != HAL_OK)
        return BSP_ADC_ERR_HW;

    return BSP_ADC_OK;
}

/**
 * @brief 启动 DMA 方式采样
 * @param adc: ADC 通道编号
 * @param buffer: 数据缓存区
 * @param length: 缓冲区长度
 * @retval BSP_ADC_Status_t
 */
BSP_ADC_Status_t BSP_ADC_Start_DMA(ADC_Num_t adc, uint16_t *buffer, uint16_t length)
{
    if (!BSP_ADC_IsValid(adc) || !buffer || length == 0)
        return BSP_ADC_ERR_INVALID_PARAM;

    ADC_HW_Map_t *dev = &adc_hw_map[adc];
    if (!dev->initialized) return BSP_ADC_ERR_NOT_INITIALIZED;

    if (HAL_ADC_Start_DMA(dev->hadc, (uint32_t *)buffer, length) != HAL_OK)
        return BSP_ADC_ERR_HW;

    return BSP_ADC_OK;
}

/**
 * @brief 停止 ADC 转换
 * @param adc: ADC 通道编号
 * @retval BSP_ADC_Status_t
 */
BSP_ADC_Status_t BSP_ADC_Stop(ADC_Num_t adc)
{
    if (!BSP_ADC_IsValid(adc)) return BSP_ADC_ERR_INVALID_ID;

    ADC_HW_Map_t *dev = &adc_hw_map[adc];
    if (!dev->initialized) return BSP_ADC_ERR_NOT_INITIALIZED;

    HAL_ADC_Stop(dev->hadc);
    return BSP_ADC_OK;
}

/**
 * @brief 停止 DMA 转换
 * @param adc: ADC 通道编号
 * @retval BSP_ADC_Status_t
 */
BSP_ADC_Status_t BSP_ADC_Stop_DMA(ADC_Num_t adc)
{
    if (!BSP_ADC_IsValid(adc)) return BSP_ADC_ERR_INVALID_ID;

    ADC_HW_Map_t *dev = &adc_hw_map[adc];
    if (!dev->initialized) return BSP_ADC_ERR_NOT_INITIALIZED;

    HAL_ADC_Stop_DMA(dev->hadc);
    return BSP_ADC_OK;
}

/**
 * @brief 停止中断采样
 * @param adc: ADC 通道编号
 * @retval BSP_ADC_Status_t
 */
BSP_ADC_Status_t BSP_ADC_Stop_IT(ADC_Num_t adc)
{
    if (!BSP_ADC_IsValid(adc)) return BSP_ADC_ERR_INVALID_ID;

    ADC_HW_Map_t *dev = &adc_hw_map[adc];
    if (!dev->initialized) return BSP_ADC_ERR_NOT_INITIALIZED;

    HAL_ADC_Stop_IT(dev->hadc);
    return BSP_ADC_OK;
}

/**
 * @brief 获取 ADC 当前数值
 * @param adc: ADC 通道编号
 * @param value: 输出当前缓存值
 * @retval BSP_ADC_Status_t
 */
BSP_ADC_Status_t BSP_ADC_GetValue(ADC_Num_t adc, uint16_t *value)
{
    if (!BSP_ADC_IsValid(adc) || !value)
        return BSP_ADC_ERR_INVALID_PARAM;

    ADC_HW_Map_t *dev = &adc_hw_map[adc];
    if (!dev->initialized) return BSP_ADC_ERR_NOT_INITIALIZED;

    *value = dev->last_value;
    return BSP_ADC_OK;
}

/* Private functions ---------------------------------------------------------*/
static int BSP_ADC_IsValid(ADC_Num_t adc)
{
    return (adc >= 0 && adc < ADC_COUNT);
}
