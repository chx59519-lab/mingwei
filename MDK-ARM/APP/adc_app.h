#ifndef __ADC_APP_H
#define __ADC_APP_H

#include "bsp_adc_driver.h"

/* ADC ����ģʽ */
typedef enum {
    ADC_MODE_BLOCKING = 0,   /*!< ����ģʽ����β��� */
    ADC_MODE_IT,             /*!< �ж�ģʽ */
    ADC_MODE_DMA             /*!< DMA ģʽ */
} ADC_Mode_t;

/* ADC Ӧ�ò��� */
typedef struct {
    ADC_Num_t id;            /*!< ADC ͨ����� */
    uint16_t *buffer;        /*!< ��������ַ������ DMA ģʽ�� */
    uint16_t length;         /*!< �������� */
    uint16_t last_value;     /*!< �������ֵ */
    uint32_t last_tick;      /*!< �ϴ�ʱ��� */
} ADC_Handle_t;

/* API ���� */
BSP_ADC_Status_t ADC_Register(ADC_Handle_t *handle, ADC_Num_t id);
BSP_ADC_Status_t ADC_ReadValue(ADC_Handle_t *handle, uint16_t *value);
BSP_ADC_Status_t ADC_StartRead(ADC_Handle_t *handle, uint16_t *buffer, uint16_t length, ADC_Mode_t mode);
BSP_ADC_Status_t ADC_Stop(ADC_Handle_t *handle, ADC_Mode_t mode);
BSP_ADC_Status_t ADC_GetValue(ADC_Handle_t *handle, uint16_t *out_value);
void ADC_Process(ADC_Handle_t *handle, uint32_t current_tick);

#endif /* __ADC_APP_H */
