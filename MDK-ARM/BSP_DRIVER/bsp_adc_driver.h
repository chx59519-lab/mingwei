/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    BSP_DAC_DRIVER.h
  * @brief   This file contains all the function prototypes for
  *          the BSP_DAC_DRIVER.c file
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


#ifndef __BSP_ADC_DRIVER_H
#define __BSP_ADC_DRIVER_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"



/* define---------------------------------------------------------------------*/

/* ADC 通道编号 */
typedef enum {
    ADC_CH1 = 0,
#if defined(ADC2)
    ADC_CH2,
#endif
#if defined(ADC3)
    ADC_CH3,
#endif
    ADC_COUNT
} ADC_Num_t;


/* 状态枚举 */
typedef enum {
    BSP_ADC_OK = 0,
    BSP_ADC_ERR_INVALID_ID,
    BSP_ADC_ERR_NOT_INITIALIZED,
    BSP_ADC_ERR_INVALID_PARAM,
    BSP_ADC_ERR_HW
} BSP_ADC_Status_t;

/* Private function prototypes -----------------------------------------------*/
/* 函数声明 */
BSP_ADC_Status_t BSP_ADC_Init(ADC_Num_t adc);
BSP_ADC_Status_t BSP_ADC_DeInit(ADC_Num_t adc);
BSP_ADC_Status_t BSP_ADC_Start(ADC_Num_t adc);
BSP_ADC_Status_t BSP_ADC_Start_IT(ADC_Num_t adc);
BSP_ADC_Status_t BSP_ADC_Start_DMA(ADC_Num_t adc, uint16_t *buffer, uint16_t length);
BSP_ADC_Status_t BSP_ADC_Stop(ADC_Num_t adc);
BSP_ADC_Status_t BSP_ADC_Stop_IT(ADC_Num_t adc);
BSP_ADC_Status_t BSP_ADC_Stop_DMA(ADC_Num_t adc);
BSP_ADC_Status_t BSP_ADC_GetValue(ADC_Num_t adc, uint16_t *value);




#ifdef __cplusplus
}
#endif
#endif/*__ BSP_DAC_DRIVER__ */
