/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    BSP_DAC_DRIVER.c
  * @brief   This file provides the implementation of the DAC BSP driver
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
#include "bsp_dac_driver.h"

/* Private typedef -----------------------------------------------------------*/
typedef struct {
    DAC_HandleTypeDef *hdac;
    uint32_t channel;
    uint8_t initialized;
    uint16_t last_value;
} DAC_HW_Map_t;

/* Private define ------------------------------------------------------------*/
/* �������ʵ��Ӳ�������޸� */
extern DAC_HandleTypeDef hdac;
#ifdef DAC2
extern DAC_HandleTypeDef hdac;
#endif

/* Private variables ---------------------------------------------------------*/
static DAC_HW_Map_t dac_hw_map[DAC_COUNT] = {
    [DAC_CH1] = {&hdac, DAC_CHANNEL_1, 0, 0},
#if DAC_COUNT > 1
    [DAC_CH2] = {&hdac, DAC_CHANNEL_2, 0, 0},
#endif
};

/* Private function prototypes -----------------------------------------------*/
static int BSP_DAC_IsValid(DAC_Num_t dac);

/* Exported functions --------------------------------------------------------*/

/**
 * @brief ��ʼ��ָ�� DAC ͨ��
 * @param dac: DAC ͨ�����
 * @retval BSP_DAC_Status_t
 */
BSP_DAC_Status_t BSP_DAC_Init(DAC_Num_t dac)
{
    if (!BSP_DAC_IsValid(dac)) {
        return BSP_DAC_ERR_INVALID_ID;
    }

    DAC_HW_Map_t *dev = &dac_hw_map[dac];

    if (HAL_DAC_Start(dev->hdac, dev->channel) != HAL_OK) {
        return BSP_DAC_ERR_HW;
    }

    dev->initialized = 1;
    dev->last_value = 0;

    return BSP_DAC_OK;
}

/**
 * @brief ����ʼ�� DAC ͨ��
 * @param dac: DAC ͨ�����
 * @retval BSP_DAC_Status_t
 */
BSP_DAC_Status_t BSP_DAC_DeInit(DAC_Num_t dac)
{
    if (!BSP_DAC_IsValid(dac)) {
        return BSP_DAC_ERR_INVALID_ID;
    }

    DAC_HW_Map_t *dev = &dac_hw_map[dac];

    HAL_DAC_Stop(dev->hdac, dev->channel);
    dev->initialized = 0;
    return BSP_DAC_OK;
}

/**
 * @brief ͨ����ͨ��ʽ���ָ��ֵ
 * @param dac: DAC ͨ�����
 * @param value: ���ֵ��12λ���ȣ�
 * @retval BSP_DAC_Status_t
 */
BSP_DAC_Status_t BSP_DAC_SetValue(DAC_Num_t dac, uint16_t value)
{
    if (!BSP_DAC_IsValid(dac)) return BSP_DAC_ERR_INVALID_ID;

    DAC_HW_Map_t *dev = &dac_hw_map[dac];
    if (!dev->initialized) return BSP_DAC_ERR_NOT_INITIALIZED;

    if (HAL_DAC_SetValue(dev->hdac, dev->channel, DAC_ALIGN_12B_R, value) != HAL_OK)
        return BSP_DAC_ERR_HW;

    dev->last_value = value;
    return BSP_DAC_OK;
}

/**
 * @brief ͨ�� DMA ���������
 * @param dac: DAC ͨ�����
 * @param buffer: ���ݻ�����
 * @param length: ���ݳ���
 * @retval BSP_DAC_Status_t
 */
BSP_DAC_Status_t BSP_DAC_Start_DMA(DAC_Num_t dac, uint32_t *buffer, uint32_t length)
{
    if (!BSP_DAC_IsValid(dac)) return BSP_DAC_ERR_INVALID_ID;

    DAC_HW_Map_t *dev = &dac_hw_map[dac];
    if (!dev->initialized) return BSP_DAC_ERR_NOT_INITIALIZED;

    if (HAL_DAC_Start_DMA(dev->hdac, dev->channel, buffer, length, DAC_ALIGN_12B_R) != HAL_OK)
        return BSP_DAC_ERR_HW;

    return BSP_DAC_OK;
}

/**
 * @brief ֹͣ DMA ���
 * @param dac: DAC ͨ�����
 * @retval BSP_DAC_Status_t
 */
BSP_DAC_Status_t BSP_DAC_Stop_DMA(DAC_Num_t dac)
{
    if (!BSP_DAC_IsValid(dac)) return BSP_DAC_ERR_INVALID_ID;

    DAC_HW_Map_t *dev = &dac_hw_map[dac];
    if (!dev->initialized) return BSP_DAC_ERR_NOT_INITIALIZED;

    HAL_DAC_Stop_DMA(dev->hdac, dev->channel);
    return BSP_DAC_OK;
}

/**
 * @brief ��ȡ DAC ��ǰ���ֵ
 * @param dac: DAC ͨ�����
 * @param value: �����ǰ����ֵ
 * @retval BSP_DAC_Status_t
 */
BSP_DAC_Status_t BSP_DAC_GetValue(DAC_Num_t dac, uint16_t *value)
{
    if (!BSP_DAC_IsValid(dac) || !value) return BSP_DAC_ERR_INVALID_ID;

    DAC_HW_Map_t *dev = &dac_hw_map[dac];
    if (!dev->initialized) return BSP_DAC_ERR_NOT_INITIALIZED;

    *value = dev->last_value;
    return BSP_DAC_OK;
}

/* Private functions ---------------------------------------------------------*/
static int BSP_DAC_IsValid(DAC_Num_t dac)
{
    return (dac >= 0 && dac < DAC_COUNT);
}