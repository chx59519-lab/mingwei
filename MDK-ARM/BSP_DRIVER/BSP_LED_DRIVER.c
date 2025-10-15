#include "BSP_LED_DRIVER.H"

/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    BSP_LED_DRIVER.c
  * @brief   This file provides the implementation of the LED BSP driver
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
#include "BSP_LED_DRIVER.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/**
 * @brief  硬件相关定义（移植时只改这里）
 * @note   使用结构体数组映射 LED -> GPIO
 */
typedef struct {
    GPIO_TypeDef *port;
    uint16_t pin;
    uint8_t initialized;                            /*!< 标记 LED 是否已初始化 */
    LED_Action_t state;                                 /*!< 当前 LED 状态缓存 */
} LED_HW_Map_t;

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* LED 硬件映射表 */
static LED_HW_Map_t led_hw_map[LED_COUNT] = {
    [LED1] = {GPIOA, GPIO_PIN_6, 0, LED_OFF},   /*!< 示例: LED1 -> PC6 */
    [LED2] = {GPIOD, GPIO_PIN_7, 0, LED_OFF},   /*!< 示例: LED2 -> PC7 */
    [LED3] = {GPIOB, GPIO_PIN_0, 0, LED_OFF},   /*!< 示例: LED3 -> PB0 */
};

/* Private function prototypes -----------------------------------------------*/
static int BSP_LED_IsValid(LED_Num_t led);

/* Exported functions --------------------------------------------------------*/

/**
 * @brief 初始化指定 LED
 * @param led: LED 编号
 * @retval BSP_LED_Status_t
 */
BSP_LED_Status_t BSP_LED_Init(LED_Num_t led)
{
    if (!BSP_LED_IsValid(led)) {
        return BSP_LED_ERR_INVALID_ID;
    }

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* 打开 GPIO 时钟（根据硬件修改，这里仅示例 PC/ PB） */
    if (led_hw_map[led].port == GPIOC) {
        __HAL_RCC_GPIOC_CLK_ENABLE();
    } else if (led_hw_map[led].port == GPIOB) {
        __HAL_RCC_GPIOB_CLK_ENABLE();
    }else if (led_hw_map[led].port == GPIOA) {
        __HAL_RCC_GPIOA_CLK_ENABLE();
    }else if (led_hw_map[led].port == GPIOD) {
        __HAL_RCC_GPIOD_CLK_ENABLE();
    }
    

    /* 配置为输出模式 */
    GPIO_InitStruct.Pin = led_hw_map[led].pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(led_hw_map[led].port, &GPIO_InitStruct);

    /* 默认关闭 LED */
    HAL_GPIO_WritePin(led_hw_map[led].port, led_hw_map[led].pin, GPIO_PIN_RESET);
    led_hw_map[led].initialized = 1;
    led_hw_map[led].state = LED_OFF;

    return BSP_LED_OK;
}

/**
 * @brief 反初始化指定 LED
 * @param led: LED 编号
 * @retval BSP_LED_Status_t
 */
BSP_LED_Status_t BSP_LED_DeInit(LED_Num_t led)
{
    if (!BSP_LED_IsValid(led)) {
        return BSP_LED_ERR_INVALID_ID;
    }

    HAL_GPIO_DeInit(led_hw_map[led].port, led_hw_map[led].pin);
    led_hw_map[led].initialized = 0;

    return BSP_LED_OK;
}

/**
 * @brief 控制 LED 状态
 * @param led: LED 编号
 * @param action: 动作（ON/OFF/TOGGLE）
 * @retval BSP_LED_Status_t
 */
BSP_LED_Status_t BSP_LED_Control(LED_Num_t led, LED_Action_t action)
{
    if (!BSP_LED_IsValid(led)) {
        return BSP_LED_ERR_INVALID_ID;
    }
    if (!led_hw_map[led].initialized) {
        return BSP_LED_ERR_NOT_INITIALIZED;
    }

    switch (action) {
        case LED_OFF:
            HAL_GPIO_WritePin(led_hw_map[led].port, led_hw_map[led].pin, GPIO_PIN_RESET);
            led_hw_map[led].state = LED_OFF;
            break;

        case LED_ON:
            HAL_GPIO_WritePin(led_hw_map[led].port, led_hw_map[led].pin, GPIO_PIN_SET);
            led_hw_map[led].state = LED_ON;
            break;

        case LED_TOGGLE:
            HAL_GPIO_TogglePin(led_hw_map[led].port, led_hw_map[led].pin);
            led_hw_map[led].state = (led_hw_map[led].state == LED_ON) ? LED_OFF : LED_ON;
            break;

        default:
            return BSP_LED_ERR_INVALID_ACTION;
    }

    return BSP_LED_OK;
}

/**
 * @brief 获取 LED 当前状态
 * @param led: LED 编号
 * @param out_action: 输出 LED 状态（ON / OFF）
 * @retval BSP_LED_Status_t
 */
BSP_LED_Status_t BSP_LED_GetState(LED_Num_t led, LED_Action_t *out_action)
{
    if (!BSP_LED_IsValid(led)) {
        return BSP_LED_ERR_INVALID_ID;
    }
    if (!led_hw_map[led].initialized) {
        return BSP_LED_ERR_NOT_INITIALIZED;
    }
    if (out_action == NULL) {
        return BSP_LED_ERR_INVALID_ACTION;
    }

    *out_action = led_hw_map[led].state;
    return BSP_LED_OK;
}

/* Private functions ---------------------------------------------------------*/

/**
 * @brief 判断 LED 编号是否合法
 * @param led: LED 编号
 * @retval 1 合法, 0 非法
 */
static int BSP_LED_IsValid(LED_Num_t led)
{
    return (led >= 0 && led < LED_COUNT);
}

