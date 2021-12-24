#ifndef __APP_FLASH_CONFIG_H__
#define __APP_FLASH_CONFIG_H__

#ifdef __cplusplus
extern "C" {
#endif


#include "board.h"
#include "mem_config.h"
#include "flash_device.h"
#include "health_algo.h"
#include "rtl876x_wdg.h"
#include "dfu_api.h"

#define FLASH_SECTOR_SIZE             (4  * 1024)

#define WRISTBAND_CONFIG_START_ADDR   (BANK0_APP_DATA1_ADDR)     //APP DATA 0
#define WRISTBAND_CONFIG_SIZE         (FLASH_SECTOR_SIZE)
#define SLEEP_DATA_START_ADDRESS      (BANK0_APP_DATA1_ADDR + WRISTBAND_CONFIG_SIZE)
#define SLEEP_DATA_SIZE               (2 * FLASH_SECTOR_SIZE)
#define PEDO_DATA_START_ADDRESS       (SLEEP_DATA_START_ADDRESS + SLEEP_DATA_SIZE)
#define PEDO_DATA_SIZE                (2 * FLASH_SECTOR_SIZE)
#define HEART_DATA_START_ADDRESS      (PEDO_DATA_START_ADDRESS + PEDO_DATA_SIZE)
#define HEART_DATA_SIZE               (2 * FLASH_SECTOR_SIZE)
#define GPS_DATA_START_ADDRESS        (HEART_DATA_START_ADDRESS + HEART_DATA_SIZE)
#define GPS_DATA_SIZE                 (2 * FLASH_SECTOR_SIZE)
#define MUSIC_INFO_START_ADDRESS      (GPS_DATA_START_ADDRESS + GPS_DATA_SIZE)
#define MUSIC_INFO_SIZE               (2 * FLASH_SECTOR_SIZE)
#define HEADSET_INFO_START_ADDRESS    (MUSIC_INFO_START_ADDRESS + MUSIC_INFO_SIZE)
#define HEADSET_INFO_SIZE             (2 * FLASH_SECTOR_SIZE)
#define AMS_ENTITY_UPDATE_ADDRESS     (HEADSET_INFO_START_ADDRESS + HEADSET_INFO_SIZE)
#define AMS_ENTITY_UPDATE_SIZE        (2 * FLASH_SECTOR_SIZE)
#define MSG_DATA_START_ADDRESS        (AMS_ENTITY_UPDATE_ADDRESS + AMS_ENTITY_UPDATE_SIZE)
#define MSG_DATA_SIZE                 (2 * FLASH_SECTOR_SIZE)

/****************************Normal data offset**********************************/
#define DEVICE_NAME_OFFSET                  0
#define DEVICE_NAME_SIZE                    (20+4)
#define ALARM_OFFSET                        (DEVICE_NAME_OFFSET + DEVICE_NAME_SIZE)
#define ALARM_SIZE                          24
#define ALARM_NUM_OFFSET                    (ALARM_OFFSET + ALARM_SIZE)
#define ALARM_NUM_SIZE                      4
#define SIT_ALERT_OFFSET                    (ALARM_NUM_OFFSET + ALARM_NUM_SIZE)
#define SIT_ALERT_SIZE                      8
#define USER_PROFILE_OFFSET                 (SIT_ALERT_OFFSET + SIT_ALERT_SIZE)
#define USER_PROFILE_SIZE                   4
#define MSG_SWITCH_OFFSET                   (USER_PROFILE_OFFSET + USER_PROFILE_SIZE)
#define MSG_SWITCH_SIZE                     4
#define LIFT_SWITCH_OFFSET                  (MSG_SWITCH_OFFSET + MSG_SWITCH_SIZE)
#define LIFT_SWITCH_SIZE                    4
#define TWIST_SWITCH_OFFSET                 (LIFT_SWITCH_OFFSET + LIFT_SWITCH_SIZE)
#define TWIST_SWITCH_SIZE                   4
#define HRS_PERIOD_OFFSET                   (TWIST_SWITCH_OFFSET + TWIST_SWITCH_SIZE)
#define HRS_PERIOD_SIZE                     4
#define REBOOT_COUNT_OFFSET                 (HRS_PERIOD_OFFSET + HRS_PERIOD_SIZE)
#define REBOOT_COUNT_SIZE                   4
#define REBOOT_REASON_OFFSET                (REBOOT_COUNT_OFFSET + REBOOT_COUNT_SIZE)
#define REBOOT_REASON_SIZE                  4

/***************for alipay iot**************/
#define FTL_ALIPAY_ITEM_STATUS_OFFSET               (REBOOT_REASON_OFFSET + REBOOT_REASON_SIZE)
#define FTL_ALIPAY_ITEM_STATUS_SIZE                 4
#define FTL_ALIPAY_ITEM_PRIVATE_KEY_OFFSET          (FTL_ALIPAY_ITEM_STATUS_OFFSET + FTL_ALIPAY_ITEM_STATUS_SIZE)
#define FTL_ALIPAY_ITEM_PRIVATE_KEY_SIZE            128
#define FTL_ALIPAY_ITEM_SHARE_KEY_OFFSET            (FTL_ALIPAY_ITEM_PRIVATE_KEY_OFFSET + FTL_ALIPAY_ITEM_PRIVATE_KEY_SIZE)
#define FTL_ALIPAY_ITEM_SHARE_KEY_SIZE              128
#define FTL_ALIPAY_ITEM_SEED_OFFSET                 (FTL_ALIPAY_ITEM_SHARE_KEY_OFFSET + FTL_ALIPAY_ITEM_SHARE_KEY_SIZE)
#define FTL_ALIPAY_ITEM_SEED_SIZE                   128
#define FTL_ALIPAY_ITEM_TIMEDIFF_OFFSET             (FTL_ALIPAY_ITEM_SEED_OFFSET + FTL_ALIPAY_ITEM_SEED_SIZE)
#define FTL_ALIPAY_ITEM_TIMEDIFF_SIZE               128
#define FTL_ALIPAY_ITEM_NICKNAME_OFFSET             (FTL_ALIPAY_ITEM_TIMEDIFF_OFFSET + FTL_ALIPAY_ITEM_TIMEDIFF_SIZE)
#define FTL_ALIPAY_ITEM_NICKNAME_SIZE               128
#define FTL_ALIPAY_ITEM_LOGONID_OFFSET              (FTL_ALIPAY_ITEM_NICKNAME_OFFSET + FTL_ALIPAY_ITEM_NICKNAME_SIZE)
#define FTL_ALIPAY_ITEM_LOGONID_SIZE                128
#define FTL_ALIPAY_ITEM_BINDFLAG_OFFSET             (FTL_ALIPAY_ITEM_LOGONID_OFFSET + FTL_ALIPAY_ITEM_LOGONID_SIZE)
#define FTL_ALIPAY_ITEM_BINDFLAG_SIZE               128
/***************for alipay iot**************/

#ifdef __cplusplus
}
#endif



#endif

