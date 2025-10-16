#include "dac_app.h"

/* 注册 DAC 通道 */
BSP_DAC_Status_t DAC_Register(DAC_Handle_t *handle, DAC_Num_t id)
{
    if (!handle) return BSP_DAC_ERR_INVALID_ID;

    BSP_DAC_Status_t status = BSP_DAC_Init(id);
    if (status == BSP_DAC_OK) {
        handle->id = id;
        handle->buffer = NULL;
        handle->length = 0;
        handle->last_value = 0;
        handle->last_tick = 0;
    }
    return status;
}

/* 输出单值（阻塞方式） */
BSP_DAC_Status_t DAC_OutputValue(DAC_Handle_t *handle, uint16_t value)
{
    if (!handle) return BSP_DAC_ERR_INVALID_PARAM;
    handle->last_value = value;
    return BSP_DAC_SetValue(handle->id, value);
}

/* 启动 DAC 输出（支持阻塞 / DMA） */
BSP_DAC_Status_t DAC_Output(DAC_Handle_t *handle, uint32_t *data, uint32_t length, DAC_Mode_t mode)
{
    if (!handle || !data || length == 0) return BSP_DAC_ERR_INVALID_PARAM;

    handle->buffer = data;
    handle->length = length;

    BSP_DAC_Status_t status;

    switch (mode) {
        case DAC_MODE_BLOCKING:
            // 阻塞模式：逐个输出（适合调试或低速）
            for (uint32_t i = 0; i < length; i++) {
                status = BSP_DAC_SetValue(handle->id, (uint16_t)data[i]);
                if (status != BSP_DAC_OK) return status;
            }
            break;

        case DAC_MODE_DMA:
            // DMA 模式：自动输出波形
            status = BSP_DAC_Start_DMA(handle->id, data, length);
            break;

        default:
            status = BSP_DAC_ERR_INVALID_PARAM;
            break;
    }

    return status;
}

/* 停止 DMA 输出 */
BSP_DAC_Status_t DAC_Stop(DAC_Handle_t *handle)
{
    if (!handle) return BSP_DAC_ERR_INVALID_PARAM;
    return BSP_DAC_Stop_DMA(handle->id);
}

/* 获取当前 DAC 输出值 */
BSP_DAC_Status_t DAC_GetValue(DAC_Handle_t *handle, uint16_t *out_value)
{
    if (!handle || !out_value) return BSP_DAC_ERR_INVALID_PARAM;
    return BSP_DAC_GetValue(handle->id, out_value);
}

/* DAC 定时处理函数 */
void DAC_Process(DAC_Handle_t *handle, uint32_t current_tick)
{
    if (!handle) return;

    if ((current_tick - handle->last_tick) > 1000) {
        handle->last_tick = current_tick;
        // 可扩展逻辑，例如自动刷新、超时保护
    }
}