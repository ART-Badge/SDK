#ifndef _RTL876X_MODULE_TOUCH_GESTURE_BUTTON__
#define _RTL876X_MODULE_TOUCH_GESTURE_BUTTON__
#include "rtl876x_gpio.h"
#include "rtl876x_rcc.h"
#include "rtl876x_tim.h"
#include "rtl876x_nvic.h"
#include "rtl876x_pinmux.h"
#include "rtl876x.h"
#include "trace.h"


/*register address*/
#define FTS_REG_CHIP_ID           0xA3     //chip ID 
#define FTS_REG_LOW_POWER         0xA5     //low power
#define FTS_REG_FW_VER            0xA6     //FW  version 
#define FTS_REG_VENDOR_ID         0xA8     //TP vendor ID 
#define FTS_REG_POINT_RATE        0x88     //report rate

#define CHIP_59_ENTER_SLEEP       0x03
#define CHIP_OTHER_ENTER_SLEEP    0x03

typedef struct
{
    int16_t x;
    int16_t y;
    uint16_t t;
    bool  is_press;
} TOUCH_DATA;

typedef enum
{
    NO_ACTION,
    QUICK_SLIDE,
    SLOW_SLIDE,
    SHORT_TOUCH,
    LONG_TOUCH,
    TOUCH_RELEASE,
} GESTURE;


static void cst0x6_device_init(void);
void cst0x6_write(uint8_t reg, uint8_t data);
void cst0x6_read_key_value(void);
void touch_gesture_process_timeout(void);
void cst0x6_driver_init(void);
static void touch_gesture_release_timeout(void *pxTimer);
static void touch_gesture_longpress_timeout(void *pxTimer);
static void touch_gesture_pad_init(void);
void touch_gesture_enter_dlps(void);
void touch_gesture_exit_dlps(void);
void enable_touch_sensor(void);
void disable_touch_sensor(void);
void cst0x6_get_chip_id(uint8_t *p_chip_id);
void cst0x6_resume(void);
void cst0x6_suspend(void);

#endif //_RTL876X_MODULE_TOUCH_GESTURE_BUTTON__




