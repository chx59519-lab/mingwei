#ifndef __ADC_APP_H
#define __ADC_APP_H

#include "bsp_adc_driver.h"

/* ADC 工作模式 */
typedef enum {
    ADC_MODE_BLOCKING = 0,   /*!< 阻塞模式：逐次采样 */
    ADC_MODE_IT,             /*!< 中断模式 */
    ADC_MODE_DMA             /*!< DMA 模式 */
} ADC_Mode_t;

/* ADC 应用层句柄 */
typedef struct {
    ADC_Num_t id;            /*!< ADC 通道编号 */
    uint16_t *buffer;        /*!< 缓冲区地址（用于 DMA 模式） */
    uint16_t length;         /*!< 采样点数 */
    uint16_t last_value;     /*!< 最近采样值 */
    uint32_t last_tick;      /*!< 上次时间戳 */
} ADC_Handle_t;

/* API 函数 */
BSP_ADC_Status_t ADC_Register(ADC_Handle_t *handle, ADC_Num_t id);
BSP_ADC_Status_t ADC_ReadValue(ADC_Handle_t *handle, uint16_t *value);
BSP_ADC_Status_t ADC_StartRead(ADC_Handle_t *handle, uint16_t *buffer, uint16_t length, ADC_Mode_t mode);
BSP_ADC_Status_t ADC_Stop(ADC_Handle_t *handle, ADC_Mode_t mode);
BSP_ADC_Status_t ADC_GetValue(ADC_Handle_t *handle, uint16_t *out_value);
void ADC_Process(ADC_Handle_t *handle, uint32_t current_tick);

#endif /* __ADC_APP_H */
