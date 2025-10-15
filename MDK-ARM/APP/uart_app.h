/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    Uart_app.h
  * @brief   This file contUart_appains all the function prototypes for
  *          the Uart_app.c file
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




/* Define to prevent recursivARTe inclusion -------------------------------------*/
#ifndef __UART_APP_H
#define __UART_APP_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "bsp_uart_driver.h"



/* define---------------------------------------------------------------------*/

/* UART 操作模式 */
typedef enum {
    UART_MODE_BLOCKING = 0,                                      /*!< 阻塞模式 */
    UART_MODE_IT,                                                /*!< 中断模式 */
    UART_MODE_DMA                                                /*!< DMA 模式 */
} UART_Mode_t;

typedef struct {
    UART_Num_t id;                                             /*!< UART 通道号 */
    uint8_t *tx_buffer;                                      /*!< 发送缓冲区指针 */
    uint16_t tx_length;                                      /*!< 当前待发送长度 */
    uint8_t *rx_buffer;                                      /*!< 接收缓冲区指针 */
    uint16_t rx_length;                                        /*!< 当前接收长度 */
    uint32_t last_tick;                                        /*!< 上次活动时间 */
} UART_Handle_t;

/* 注册 UART */
BSP_UART_Status_t UART_Register(UART_Handle_t *handle, UART_Num_t id);

/* 发送/接收数据，指定模式 */
BSP_UART_Status_t UART_Send(UART_Handle_t *handle, uint8_t *data, uint16_t len, UART_Mode_t mode);
BSP_UART_Status_t UART_Receive(UART_Handle_t *handle, uint8_t *buf, uint16_t len, UART_Mode_t mode);

/* 定时处理函数 */
void UART_Process(UART_Handle_t *handle, uint32_t current_tick);



#ifdef __cplusplus
}
#endif
#endif /*__ UART_APP_H__ */