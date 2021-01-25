/**
 *Name        : ctp_hynitron_ext.h
 *Author      : gary
 *Version     : V1.0
 *Create      : 2017-11-23
 *Copyright   : zxzz
 */


#ifndef CTP_HYNITRON_EXT_H__
#define CTP_HYNITRON_EXT_H__

#include <stdbool.h>

#define __TOUCH_PANEL_CAPACITY__
#define CTP_HYNITRON_EXT 1
#define CTP_HYNITRON_EXT_CST816S_UPDATE   1

extern bool ctp_hynitron_update(void);
void hctp_update_test(void);
void touch_update_test_callback(void *pxTimer);
#endif /*CTP_HYNITRON_EXT_H__*/


