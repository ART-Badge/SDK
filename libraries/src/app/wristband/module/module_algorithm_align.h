
#ifndef __ALGORITHM_ALIGN_H__
#define __ALGORITHM_ALIGN_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "rtl876x.h"

void algorithm_align_timer_init(void);
void check_sensor_fifo_exit_dlps(void);
void algorithm_align_timer_start(void);
void algorithm_align_timer_stop(void);

#ifdef __cplusplus
}
#endif

#endif
