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


#ifndef __BSP_DAC_DRIVER_H
#define __BSP_DAC_DRIVER_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dac.h"



/* define---------------------------------------------------------------------*/

/* 枚举定义 */
typedef enum {
    DAC_CH1 = 0,
#if defined(DAC_CHANNEL_2)
    DAC_CH2,
#endif
    DAC_COUNT
} DAC_Num_t;

typedef enum {
    BSP_DAC_OK = 0,
    BSP_DAC_ERR_INVALID_ID,
    BSP_DAC_ERR_NOT_INITIALIZED,
    BSP_DAC_ERR_INVALID_PARAM,
    BSP_DAC_ERR_HW
} BSP_DAC_Status_t;

/* Private function prototypes -----------------------------------------------*/
/* 函数声明 */
BSP_DAC_Status_t BSP_DAC_Init(DAC_Num_t dac);
BSP_DAC_Status_t BSP_DAC_DeInit(DAC_Num_t dac);
BSP_DAC_Status_t BSP_DAC_SetValue(DAC_Num_t dac, uint16_t value);
BSP_DAC_Status_t BSP_DAC_GetValue(DAC_Num_t dac, uint16_t *value);
BSP_DAC_Status_t BSP_DAC_Start_DMA(DAC_Num_t dac, uint32_t *buffer, uint32_t length);
BSP_DAC_Status_t BSP_DAC_Stop_DMA(DAC_Num_t dac);




#ifdef __cplusplus
}
#endif
#endif/*__ BSP_DAC_DRIVER__ */
