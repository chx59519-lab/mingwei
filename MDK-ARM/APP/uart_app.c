#include "uart_app.h"

/* ע�� UART */
BSP_UART_Status_t UART_Register(UART_Handle_t *handle, UART_Num_t id)
{
    if (!handle) return BSP_UART_ERR_INVALID_ID;

    BSP_UART_Status_t status = BSP_UART_Init(id);
    if (status == BSP_UART_OK) {
        handle->id = id;
        handle->tx_buffer = NULL;
        handle->rx_buffer = NULL;
        handle->tx_length = 0;
        handle->rx_length = 0;
        handle->last_tick = 0;
    }
    return status;
}

/* �������ݣ�ָ��ģʽ */
BSP_UART_Status_t UART_Send(UART_Handle_t *handle, uint8_t *data, uint16_t len, UART_Mode_t mode)
{
    if (!handle || !data || len == 0) return BSP_UART_ERR_INVALID_PARAM;

    handle->tx_buffer = data;
    handle->tx_length = len;

    BSP_UART_Status_t status;

    switch (mode) {
        case UART_MODE_BLOCKING:
            status = BSP_UART_Send(handle->id, data, len,10);  // ����ģʽ
            break;
        case UART_MODE_IT:
            status = BSP_UART_Send_IT(handle->id, data, len); // �ж�ģʽ
            break;
        case UART_MODE_DMA:
            status = BSP_UART_Send_DMA(handle->id, data, len); // DMA ģʽ
            break;
        default:
            status = BSP_UART_ERR_INVALID_PARAM;
            break;
    }

    return status;
}

/* �������ݣ�ָ��ģʽ */
BSP_UART_Status_t UART_Receive(UART_Handle_t *handle, uint8_t *buf, uint16_t len, UART_Mode_t mode)
{
    if (!handle || !buf || len == 0) return BSP_UART_ERR_INVALID_PARAM;

    handle->rx_buffer = buf;
    handle->rx_length = len;

    BSP_UART_Status_t status;

    switch (mode) {
        case UART_MODE_BLOCKING:
            status = BSP_UART_Receive(handle->id, buf, len,10);  // ����ģʽ
            break;
        case UART_MODE_IT:
            status = BSP_UART_Receive_IT(handle->id, buf, len); // �ж�ģʽ
            break;
        case UART_MODE_DMA:
            status = BSP_UART_Receive_DMA(handle->id, buf, len); // DMA ģʽ
            break;
        default:
            status = BSP_UART_ERR_INVALID_PARAM;
            break;
    }

    return status;
}

/* UART ��ʱ������ */
void UART_Process(UART_Handle_t *handle, uint32_t current_tick)
{
    if (!handle) return;

    if ((current_tick - handle->last_tick) > 1000) {
        handle->last_tick = current_tick;
        // ����չ�߼���������������ʱ����
    }
}