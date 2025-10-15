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

/* UART ����ģʽ */
typedef enum {
    UART_MODE_BLOCKING = 0,                                      /*!< ����ģʽ */
    UART_MODE_IT,                                                /*!< �ж�ģʽ */
    UART_MODE_DMA                                                /*!< DMA ģʽ */
} UART_Mode_t;

typedef struct {
    UART_Num_t id;                                             /*!< UART ͨ���� */
    uint8_t *tx_buffer;                                      /*!< ���ͻ�����ָ�� */
    uint16_t tx_length;                                      /*!< ��ǰ�����ͳ��� */
    uint8_t *rx_buffer;                                      /*!< ���ջ�����ָ�� */
    uint16_t rx_length;                                        /*!< ��ǰ���ճ��� */
    uint32_t last_tick;                                        /*!< �ϴλʱ�� */
} UART_Handle_t;

/* ע�� UART */
BSP_UART_Status_t UART_Register(UART_Handle_t *handle, UART_Num_t id);

/* ����/�������ݣ�ָ��ģʽ */
BSP_UART_Status_t UART_Send(UART_Handle_t *handle, uint8_t *data, uint16_t len, UART_Mode_t mode);
BSP_UART_Status_t UART_Receive(UART_Handle_t *handle, uint8_t *buf, uint16_t len, UART_Mode_t mode);

/* ��ʱ������ */
void UART_Process(UART_Handle_t *handle, uint32_t current_tick);



#ifdef __cplusplus
}
#endif
#endif /*__ UART_APP_H__ */