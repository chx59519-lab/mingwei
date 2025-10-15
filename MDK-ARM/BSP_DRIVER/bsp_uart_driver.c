/* Includes ------------------------------------------------------------------*/
#include "bsp_uart_driver.h"

/* Private typedef -----------------------------------------------------------*/
typedef struct {
    UART_HandleTypeDef *huart;                           /*!< HAL UART handle */
    uint8_t initialized;                                       /*!< Init flag */
    BSP_UART_Callback_t rx_callback;                         /*!< ������ɻص� */
    BSP_UART_Callback_t tx_callback;                         /*!< ������ɻص� */
} UART_HW_Map_t;

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern UART_HandleTypeDef huart1;
/* UART Ӳ��ӳ���CubeMX �ѳ�ʼ���� */
static UART_HW_Map_t uart_hw_map[UART_COUNT] = {
    [UART1] = {&huart1, 0},
};

/* Private function prototypes -----------------------------------------------*/
static int BSP_UART_IsValid(UART_Num_t uart);

/* Exported functions --------------------------------------------------------*/

/**
 * @brief ��ʼ��ָ�� UART���� CubeMX ����ײ㣩
 * @param uart: UART ���
 * @retval BSP_UART_Status_t
 */
BSP_UART_Status_t BSP_UART_Init(UART_Num_t uart)
{
    if (!BSP_UART_IsValid(uart)) {
        return BSP_UART_ERR_INVALID_ID;
    }

    UART_HandleTypeDef *huart = uart_hw_map[uart].huart;

    /* ���� CubeMX ���ɵĳ�ʼ������ */
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
 * @brief ����ʼ�� UART
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
/* ����ģʽ                                                                   */
/* -------------------------------------------------------------------------- */

/**
 * @brief �������ݣ�����ģʽ��
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
 * @brief �������ݣ�����ģʽ��
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
 * @brief ��ȡ UART ��ʼ��״̬
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
/* �ж�ģʽ                                                                   */
/* -------------------------------------------------------------------------- */

/**
 * @brief �жϷ���
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
 * @brief �жϽ���
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
/* DMA ģʽ                                                                   */
/* -------------------------------------------------------------------------- */

/**
 * @brief DMA ����
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
 * @brief DMA ����
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
/* �ص�ע�����                                                               */
/* -------------------------------------------------------------------------- */

/**
 * @brief ע�ᷢ�����/������ɻص�
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
/* HAL �ص��Žӣ������жϻص��ļ��л��Զ�������                              */
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
