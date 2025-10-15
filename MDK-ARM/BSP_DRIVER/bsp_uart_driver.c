/* Includes ------------------------------------------------------------------*/
#include "bsp_uart_driver.h"

/* Private typedef -----------------------------------------------------------*/
typedef struct {
    UART_HandleTypeDef *huart;                           /*!< HAL UART handle */
    uint8_t initialized;                                       /*!< Init flag */
    BSP_UART_Callback_t rx_callback;                         /*!< 接收完成回调 */
    BSP_UART_Callback_t tx_callback;                         /*!< 发送完成回调 */
} UART_HW_Map_t;

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern UART_HandleTypeDef huart1;
/* UART 硬件映射表（CubeMX 已初始化） */
static UART_HW_Map_t uart_hw_map[UART_COUNT] = {
    [UART1] = {&huart1, 0},
};

/* Private function prototypes -----------------------------------------------*/
static int BSP_UART_IsValid(UART_Num_t uart);

/* Exported functions --------------------------------------------------------*/

/**
 * @brief 初始化指定 UART（由 CubeMX 管理底层）
 * @param uart: UART 编号
 * @retval BSP_UART_Status_t
 */
BSP_UART_Status_t BSP_UART_Init(UART_Num_t uart)
{
    if (!BSP_UART_IsValid(uart)) {
        return BSP_UART_ERR_INVALID_ID;
    }

    UART_HandleTypeDef *huart = uart_hw_map[uart].huart;

    /* 调用 CubeMX 生成的初始化函数 */
    switch (uart) {
        case UART1:
            break;
        case UART2:
            break;
        default:
            return BSP_UART_ERR_INVALID_ID;
    }

    uart_hw_map[uart].initialized = 1;
    return BSP_UART_OK;
}

/**
 * @brief 反初始化 UART
 */
BSP_UART_Status_t BSP_UART_DeInit(UART_Num_t uart)
{
    if (!BSP_UART_IsValid(uart)) {
        return BSP_UART_ERR_INVALID_ID;
    }

    HAL_UART_DeInit(uart_hw_map[uart].huart);
    uart_hw_map[uart].initialized = 0;
    return BSP_UART_OK;
}

/* -------------------------------------------------------------------------- */
/* 阻塞模式                                                                   */
/* -------------------------------------------------------------------------- */

/**
 * @brief 发送数据（阻塞模式）
 */
BSP_UART_Status_t BSP_UART_Send(UART_Num_t uart, uint8_t *pData, uint16_t size, uint32_t timeout)
{
    if (!BSP_UART_IsValid(uart) || !uart_hw_map[uart].initialized) {
        return BSP_UART_ERR_NOT_INITIALIZED;
    }

    if (HAL_UART_Transmit(uart_hw_map[uart].huart, pData, size, timeout) != HAL_OK) {
        return BSP_UART_ERR_TX_FAIL;
    }

    return BSP_UART_OK;
}

/**
 * @brief 接收数据（阻塞模式）
 */
BSP_UART_Status_t BSP_UART_Receive(UART_Num_t uart, uint8_t *pData, uint16_t size, uint32_t timeout)
{
    if (!BSP_UART_IsValid(uart) || !uart_hw_map[uart].initialized) {
        return BSP_UART_ERR_NOT_INITIALIZED;
    }

    if (HAL_UART_Receive(uart_hw_map[uart].huart, pData, size, timeout) != HAL_OK) {
        return BSP_UART_ERR_RX_FAIL;
    }

    return BSP_UART_OK;
}

/**
 * @brief 获取 UART 初始化状态
 */
BSP_UART_Status_t BSP_UART_GetState(UART_Num_t uart, uint8_t *out_init)
{
    if (!BSP_UART_IsValid(uart) || out_init == NULL) {
        return BSP_UART_ERR_INVALID_ID;
    }

    *out_init = uart_hw_map[uart].initialized;
    return BSP_UART_OK;
}

/* -------------------------------------------------------------------------- */
/* 中断模式                                                                   */
/* -------------------------------------------------------------------------- */

/**
 * @brief 中断发送
 */
BSP_UART_Status_t BSP_UART_Send_IT(UART_Num_t uart, uint8_t *pData, uint16_t size)
{
    if (!BSP_UART_IsValid(uart) || !uart_hw_map[uart].initialized)
        return BSP_UART_ERR_NOT_INITIALIZED;

    if (HAL_UART_Transmit_IT(uart_hw_map[uart].huart, pData, size) != HAL_OK)
        return BSP_UART_ERR_TX_FAIL;

    return BSP_UART_OK;
}

/**
 * @brief 中断接收
 */
BSP_UART_Status_t BSP_UART_Receive_IT(UART_Num_t uart, uint8_t *pData, uint16_t size)
{
    if (!BSP_UART_IsValid(uart) || !uart_hw_map[uart].initialized)
        return BSP_UART_ERR_NOT_INITIALIZED;

    if (HAL_UART_Receive_IT(uart_hw_map[uart].huart, pData, size) != HAL_OK)
        return BSP_UART_ERR_RX_FAIL;

    return BSP_UART_OK;
}

/* -------------------------------------------------------------------------- */
/* DMA 模式                                                                   */
/* -------------------------------------------------------------------------- */

/**
 * @brief DMA 发送
 */
BSP_UART_Status_t BSP_UART_Send_DMA(UART_Num_t uart, uint8_t *pData, uint16_t size)
{
    if (!BSP_UART_IsValid(uart) || !uart_hw_map[uart].initialized)
        return BSP_UART_ERR_NOT_INITIALIZED;

    if (HAL_UART_Transmit_DMA(uart_hw_map[uart].huart, pData, size) != HAL_OK)
        return BSP_UART_ERR_TX_FAIL;

    return BSP_UART_OK;
}

/**
 * @brief DMA 接收
 */
BSP_UART_Status_t BSP_UART_Receive_DMA(UART_Num_t uart, uint8_t *pData, uint16_t size)
{
    if (!BSP_UART_IsValid(uart) || !uart_hw_map[uart].initialized)
        return BSP_UART_ERR_NOT_INITIALIZED;

    if (HAL_UART_Receive_DMA(uart_hw_map[uart].huart, pData, size) != HAL_OK)
        return BSP_UART_ERR_RX_FAIL;

    return BSP_UART_OK;
}

/* -------------------------------------------------------------------------- */
/* 回调注册机制                                                               */
/* -------------------------------------------------------------------------- */

/**
 * @brief 注册发送完成/接收完成回调
 */
BSP_UART_Status_t BSP_UART_RegisterCallback(UART_Num_t uart,
                                            BSP_UART_CallbackType type,
                                            BSP_UART_Callback_t callback)
{
    if (!BSP_UART_IsValid(uart))
        return BSP_UART_ERR_INVALID_ID;

    if (type == BSP_UART_CALLBACK_RX)
        uart_hw_map[uart].rx_callback = callback;
    else if (type == BSP_UART_CALLBACK_TX)
        uart_hw_map[uart].tx_callback = callback;
    else
        return BSP_UART_ERR_INVALID_ACTION;

    return BSP_UART_OK;
}

/* -------------------------------------------------------------------------- */
/* HAL 回调桥接（放在中断回调文件中会自动触发）                              */
/* -------------------------------------------------------------------------- */

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    for (int i = 0; i < UART_COUNT; i++) {
        if (uart_hw_map[i].huart == huart && uart_hw_map[i].tx_callback) {
            uart_hw_map[i].tx_callback();
        }
    }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    for (int i = 0; i < UART_COUNT; i++) {
        if (uart_hw_map[i].huart == huart && uart_hw_map[i].rx_callback) {
            uart_hw_map[i].rx_callback();
        }
    }
}


/* Private function ----------------------------------------------------------*/
static int BSP_UART_IsValid(UART_Num_t uart)
{
    return (uart >= 0 && uart < UART_COUNT);
}
