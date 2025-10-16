#ifndef __DAC_APP_H
#define __DAC_APP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "bsp_dac_driver.h"

/* DAC ģʽ���� */
typedef enum {
    DAC_MODE_BLOCKING = 0,  /*!< ������������㣩 */
    DAC_MODE_DMA,           /*!< DMA ������������Σ� */
} DAC_Mode_t;

/* �ϲ��� */
typedef struct {
    DAC_Num_t id;           /*!< DAC ͨ����� */
    uint32_t *buffer;       /*!< ���ݻ�����ָ�루DMA ģʽ�� */
    uint32_t length;        /*!< ���������� */
    uint16_t last_value;    /*!< �ϴ����ֵ */
    uint32_t last_tick;     /*!< �ϴδ���ʱ��� */
} DAC_Handle_t;

/* �������� */
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
