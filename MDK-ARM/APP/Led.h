/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    Led.h
  * @brief   This file contains all the function prototypes for
  *          the Led.c file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 .
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */




/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LED_H
#define __LED_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "BSP_LED_DRIVER.h"



/* define---------------------------------------------------------------------*/

/**
 * @brief LED 管理句柄
 */
typedef struct {
    LED_Num_t id;           /*!< LED 底层编号 */
    LED_Action_t state;     /*!< 当前状态缓存 */
    uint32_t blink_interval;/*!< 闪烁周期 (ms)，0 表示不闪烁 */
    uint32_t last_tick;     /*!< 上一次切换时间戳 */
} LED_Handle_t;



/* Private function prototypes -----------------------------------------------*/
/**
 * @brief 注册 LED（完成底层初始化）
 * @param handle LED 句柄
 * @param id     BSP 层的 LED 编号
 * @return BSP_LED_Status_t
 */
BSP_LED_Status_t LED_Register(LED_Handle_t *handle, LED_Num_t id);

/**
 * @brief 设置 LED 状态（开/关/翻转）
 */
BSP_LED_Status_t LED_Set(LED_Handle_t *handle, LED_Action_t action);

/**
 * @brief 启动 LED 闪烁模式
 * @param handle LED 句柄
 * @param interval_ms 闪烁间隔时间（毫秒）
 */
void LED_StartBlink(LED_Handle_t *handle, uint32_t interval_ms);

/**
 * @brief 停止 LED 闪烁
 */
void LED_StopBlink(LED_Handle_t *handle);

/**
 * @brief 在主循环中周期调用，驱动 LED 闪烁
 * @param handle LED 句柄
 * @param current_tick 当前系统时钟（例如 HAL_GetTick()）
 */
void LED_Process(LED_Handle_t *handle, uint32_t current_tick);




#ifdef __cplusplus
}
#endif
#endif /*__ LED_H__ */

