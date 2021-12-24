#ifndef __SYNC_DATA_H__
#define __SYNC_DATA_H__

#ifdef __cplusplus
extern "C" {
#endif


#include "health_algo.h"
#include "communicate_protocol.h"
#include "communicate_sync_pedo.h"
#include "communicate_sync_sleep.h"
#include "communicate_sync_heart_rate.h"
#include "communicate_sync_gps.h"


#define MAX_SLEEP_ITEM_COUNT     8
#define SLEEP_ITEM_LENGTH        4
#define SLEEP_HEAD_LENGTH        4

#define MAX_PEDO_ITEM_COUNT      8
#define PEDO_ITEM_LENGTH         8
#define PEDO_HEAD_LENGTH         4

#define MAX_HEART_ITEM_COUNT     8
#define HEART_ITEM_LENGTH        4
#define HEART_HEAD_LENGTH        4



#ifdef __cplusplus
}
#endif

#endif
