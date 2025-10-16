#ifndef __DAC_APP_H
#define __DAC_APP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "bsp_dac_driver.h"

/* DAC 模式定义 */
typedef enum {
    DAC_MODE_BLOCKING = 0,  /*!< 阻塞输出（单点） */
    DAC_MODE_DMA,           /*!< DMA 输出（连续波形） */
} DAC_Mode_t;

/* 上层句柄 */
typedef struct {
    DAC_Num_t id;           /*!< DAC 通道编号 */
    uint32_t *buffer;       /*!< 数据缓冲区指针（DMA 模式） */
    uint32_t length;        /*!< 缓冲区长度 */
    uint16_t last_value;    /*!< 上次输出值 */
    uint32_t last_tick;     /*!< 上次处理时间戳 */
} DAC_Handle_t;

/* 函数声明 */
BSP_DAC_Status_t DAC_Register(DAC_Handle_t *handle, DAC_Num_t id);
BSP_DAC_Status_t DAC_OutputValue(DAC_Handle_t *handle, uint16_t value);
BSP_DAC_Status_t DAC_Output(DAC_Handle_t *handle, uint32_t *data, uint32_t length, DAC_Mode_t mode);
BSP_DAC_Status_t DAC_Stop(DAC_Handle_t *handle);
BSP_DAC_Status_t DAC_GetValue(DAC_Handle_t *handle, uint16_t *out_value);
void DAC_Process(DAC_Handle_t *handle, uint32_t current_tick);

#ifdef __cplusplus
}
#endif

#endif /* __DAC_APP_H */
