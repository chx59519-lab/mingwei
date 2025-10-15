/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    BSP_UART_DRIVER.h
  * @brief   This file contains all the function prototypes for
  *          the BSP_UART_DRIVER.c file
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


#ifndef __BSP_UART_DRIVER_H
#define __BSP_UART_DRIVER_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"




/* define---------------------------------------------------------------------*/

/**
 * @brief uart编号定义
 * @note  用于区分不同的 uart。Uart_COUNT 用于边界检查。
 */
typedef enum {
    UART1 = 0,                                                  
    UART2,                                                    
    UART_COUNT                           /*!< Uart总数（不可作为实际 Uart 使用） */
}UART_Num_t;


/**
 * @brief BSP_Uart模块的返回状态
 * @note  提供标准化的错误码，便于上层应用判断
 */
typedef enum {
    BSP_UART_OK = 0,
    BSP_UART_ERR_INVALID_ID,
    BSP_UART_ERR_NOT_INITIALIZED,
    BSP_UART_ERR_INIT_FAIL,
    BSP_UART_ERR_TX_FAIL,
    BSP_UART_ERR_RX_FAIL,
    BSP_UART_ERR_INVALID_ACTION,
    BSP_UART_ERR_INVALID_PARAM
} BSP_UART_Status_t;

/**
 * @brief BSP_Uart模块的回调类型
 */
typedef enum {
    BSP_UART_CALLBACK_TX,
    BSP_UART_CALLBACK_RX
} BSP_UART_CallbackType;
/* Private function prototypes -----------------------------------------------*/
typedef void (*BSP_UART_Callback_t)(void);

BSP_UART_Status_t BSP_UART_Init(UART_Num_t uart);
BSP_UART_Status_t BSP_UART_DeInit(UART_Num_t uart);
/* 阻塞模式 */
BSP_UART_Status_t BSP_UART_Send(UART_Num_t uart, uint8_t *pData, uint16_t size, uint32_t timeout);
BSP_UART_Status_t BSP_UART_Receive(UART_Num_t uart, uint8_t *pData, uint16_t size, uint32_t timeout);

/* 中断模式 */
BSP_UART_Status_t BSP_UART_Send_IT(UART_Num_t uart, uint8_t *pData, uint16_t size);
BSP_UART_Status_t BSP_UART_Receive_IT(UART_Num_t uart, uint8_t *pData, uint16_t size);

/* DMA 模式 */
BSP_UART_Status_t BSP_UART_Send_DMA(UART_Num_t uart, uint8_t *pData, uint16_t size);
BSP_UART_Status_t BSP_UART_Receive_DMA(UART_Num_t uart, uint8_t *pData, uint16_t size);

/* 回调注册 */
BSP_UART_Status_t BSP_UART_RegisterCallback(UART_Num_t uart,
                                            BSP_UART_CallbackType type,
                                            BSP_UART_Callback_t callback);



#ifdef __cplusplus
}
#endif
#endif/*__ BSP_UART_DRIVER__ */
