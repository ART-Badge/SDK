
#ifndef __VIBRATE_MOTOR_H__
#define __VIBRATE_MOTOR_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "rtl876x.h"

typedef enum
{
    SOFT_INTERVAL = 20,
    HEAVY_INTERVAL = 100,   //in this design, used for incomming call notification
} VIBRATE_TYPE_t;

void motor_init(void);
void motor_start(uint16_t total_cycle, uint16_t pwm_out, uint16_t pwm_idle);
void motor_stop(void);
bool motor_getstatus(void);

#ifdef __cplusplus
}
#endif

#endif
