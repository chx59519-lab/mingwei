#include "adc_app.h"

/* 注册 ADC 通道 */
BSP_ADC_Status_t ADC_Register(ADC_Handle_t *handle, ADC_Num_t id)
{
    if (!handle) return BSP_ADC_ERR_INVALID_ID;

    BSP_ADC_Status_t status = BSP_ADC_Init(id);
    if (status == BSP_ADC_OK) {
        handle->id = id;
        handle->buffer = NULL;
        handle->length = 0;
        handle->last_value = 0;
        handle->last_tick = 0;
    }
    return status;
}

/* 单次采样（阻塞方式） */
BSP_ADC_Status_t ADC_ReadValue(ADC_Handle_t *handle, uint16_t *value)
{
    if (!handle || !value) return BSP_ADC_ERR_INVALID_PARAM;

    BSP_ADC_Status_t status = BSP_ADC_Start(handle->id);
    if (status != BSP_ADC_OK) return status;

    status = BSP_ADC_GetValue(handle->id, value);
    if (status == BSP_ADC_OK)
        handle->last_value = *value;

    return status;
}

/* 启动连续采样（支持阻塞 / 中断 / DMA 模式） */
BSP_ADC_Status_t ADC_StartRead(ADC_Handle_t *handle, uint16_t *buffer, uint16_t length, ADC_Mode_t mode)
{
    if (!handle || !buffer || length == 0) return BSP_ADC_ERR_INVALID_PARAM;

    handle->buffer = buffer;
    handle->length = length;

    BSP_ADC_Status_t status;

    switch (mode) {
        case ADC_MODE_BLOCKING:
            // 阻塞模式：手动逐次采样
            for (uint16_t i = 0; i < length; i++) {
                status = BSP_ADC_Start(handle->id);
                if (status != BSP_ADC_OK) return status;

                status = BSP_ADC_GetValue(handle->id, &buffer[i]);
                if (status != BSP_ADC_OK) return status;
            }
            break;

        case ADC_MODE_IT:
            // 中断模式
            status = BSP_ADC_Start_IT(handle->id);
            break;

        case ADC_MODE_DMA:
            // DMA 模式
            status = BSP_ADC_Start_DMA(handle->id, buffer, length);
            break;

        default:
            status = BSP_ADC_ERR_INVALID_PARAM;
            break;
    }

    return status;
}

/* 停止采样 */
BSP_ADC_Status_t ADC_Stop(ADC_Handle_t *handle, ADC_Mode_t mode)
{
    if (!handle) return BSP_ADC_ERR_INVALID_PARAM;

    BSP_ADC_Status_t status;

    switch (mode) {
        case ADC_MODE_IT:
            status = BSP_ADC_Stop_IT(handle->id);
            break;

        case ADC_MODE_DMA:
            status = BSP_ADC_Stop_DMA(handle->id);
            break;

        case ADC_MODE_BLOCKING:
        default:
            status = BSP_ADC_Stop(handle->id);
            break;
    }

    return status;
}

/* 获取当前 ADC 缓存值 */
BSP_ADC_Status_t ADC_GetValue(ADC_Handle_t *handle, uint16_t *out_value)
{
    if (!handle || !out_value) return BSP_ADC_ERR_INVALID_PARAM;
    return BSP_ADC_GetValue(handle->id, out_value);
}

/* 定时处理函数，可用于周期检测或自动刷新 */
void ADC_Process(ADC_Handle_t *handle, uint32_t current_tick)
{
    if (!handle) return;

    if ((current_tick - handle->last_tick) > 1000) {
        handle->last_tick = current_tick;
        // 可扩展逻辑，例如采样完成回调检查、数据刷新等
    }
}
