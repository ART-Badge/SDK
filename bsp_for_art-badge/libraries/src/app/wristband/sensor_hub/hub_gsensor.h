
#ifndef __WRISTBAND_GSENSOR_H__
#define __WRISTBAND_GSENSOR_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "stdint.h"
#include "board.h"
#include "app_msg.h"


typedef struct
{
    int16_t AXIS_X;
    int16_t AXIS_Y;
    int16_t AXIS_Z;
} AxesRaw_t;



void gsensor_add_hub_task(void);
void wristband_gsensor_driver_init(void);
void wristband_gsensor_device_init(void);
bool wristband_gsensor_get_fifo_data(uint8_t *len, AxesRaw_t *buff);
void gsensor_sample_timer_callback(void *pxTimer);
void wristband_gsensor_stop(void);
void wristband_gsensor_start(void);
void wristband_gsensor_enter_dlps(void);
void wristband_gsensor_exit_dlps(void);
bool wristband_gsensor_allowed_enter_dlps_check(void);
void wristband_gsensor_message_handle(void);
uint8_t wristband_gsensor_get_fifo_length(void);

void gsensor_event_handler(T_IO_MSG msg);

#ifdef __cplusplus
}
#endif

#endif //__WRISTBAND_GSENSOR_H__


