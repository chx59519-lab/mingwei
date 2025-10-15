#include "Led.h"

BSP_LED_Status_t LED_Register(LED_Handle_t *handle, LED_Num_t id)
{
    if (!handle) return BSP_LED_ERR_INVALID_ID;
    BSP_LED_Status_t status = BSP_LED_Init(id);
    if (status == BSP_LED_OK) {
        handle->id = id;
        handle->state = LED_OFF;
        handle->blink_interval = 0;
        handle->last_tick = 0;
    }
    return status;
}

BSP_LED_Status_t LED_Set(LED_Handle_t *handle, LED_Action_t action) 
{
    if (!handle) return BSP_LED_ERR_INVALID_ID;
    BSP_LED_Status_t status = BSP_LED_Control(handle->id, action);
    if (status == BSP_LED_OK) {
        handle->state = action;
    }
    return status;
}

void LED_StartBlink(LED_Handle_t *handle, uint32_t interval_ms)
 {
    if (!handle) return;
    handle->blink_interval = interval_ms;
    handle->last_tick = 0;
}

void LED_StopBlink(LED_Handle_t *handle) 
{
    if (!handle) return;
    handle->blink_interval = 0;
}

void LED_Process(LED_Handle_t *handle, uint32_t current_tick) 
{
    if (!handle || handle->blink_interval == 0) return;

    if ((current_tick - handle->last_tick) >= handle->blink_interval) {
        BSP_LED_Control(handle->id, LED_TOGGLE);
        handle->state = (handle->state == LED_ON) ? LED_OFF : LED_ON;
        handle->last_tick = current_tick;
    }
}