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
/* ����ʵ�� CubeMX ���ɵ� ADC ����޸� */
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
 * @brief ��ʼ��ָ�� ADC ͨ��
 * @param adc: ADC ͨ�����
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
 * @brief ����ʼ��ָ�� ADC ͨ��
 * @param adc: ADC ͨ�����
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
 * @brief ���� ADC ��������������һ��ֵ
 * @param adc: ADC ͨ�����
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
 * @brief �����жϷ�ʽ����
 * @param adc: ADC ͨ�����
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
 * @brief ���� DMA ��ʽ����
 * @param adc: ADC ͨ�����
 * @param buffer: ���ݻ�����
 * @param length: ����������
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
 * @brief ֹͣ ADC ת��
 * @param adc: ADC ͨ�����
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
 * @brief ֹͣ DMA ת��
 * @param adc: ADC ͨ�����
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
 * @brief ֹͣ�жϲ���
 * @param adc: ADC ͨ�����
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
 * @brief ��ȡ ADC ��ǰ��ֵ
 * @param adc: ADC ͨ�����
 * @param value: �����ǰ����ֵ
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
