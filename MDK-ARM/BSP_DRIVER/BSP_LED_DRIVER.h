/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    BSP_LED_DRIVER.h
  * @brief   This file contains all the function prototypes for
  *          the BSP_LED_DRIVER.c file
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


#ifndef __BSP_LED_DRIVER_H
#define __BSP_LED_DRIVER_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"




/* define---------------------------------------------------------------------*/

/**
 * @brief LED 编号定义
 * @note  用于区分不同的 LED。LED_COUNT 用于边界检查。
 */
typedef enum {
    LED1 = 0,                                                  /*!< 板载 LED1 */
    LED2, 
    LED3,                                                     /*!< 板载 LED2 */
    LED_COUNT                           /*!< LED 总数（不可作为实际 LED 使用） */
} LED_Num_t;

/**
 * @brief LED 操作类型
 * @note  控制 LED 的三种动作
 */
typedef enum {
    LED_OFF = 0,                                                /*!< 关闭 LED */
    LED_ON,                                                     /*!< 打开 LED */
    LED_TOGGLE                                             /*!< 翻转 LED 状态 */
} LED_Action_t;

/**
 * @brief BSP_LED 模块的返回状态
 * @note  提供标准化的错误码，便于上层应用判断
 */
typedef enum {
    BSP_LED_OK = 0,                                              /*!< 操作成功 */
    BSP_LED_ERR_INVALID_ID,                                  /*!< LED 编号无效 */
    BSP_LED_ERR_INVALID_ACTION,                              /*!< LED 动作无效 */
    BSP_LED_ERR_NOT_INITIALIZED                              /*!< LED 未初始化 */
} BSP_LED_Status_t;


/* Private function prototypes -----------------------------------------------*/

BSP_LED_Status_t BSP_LED_Init(LED_Num_t led);
BSP_LED_Status_t BSP_LED_DeInit(LED_Num_t led);
BSP_LED_Status_t BSP_LED_Control(LED_Num_t led, LED_Action_t action);
BSP_LED_Status_t BSP_LED_GetState(LED_Num_t led,  LED_Action_t *out_action);



#ifdef __cplusplus
}
#endif
#endif/*__ BSP_LED_DRIVER__ */
