#ifndef _WRISTBAND_INTERACTION_H_
#define _WRISTBAND_INTERACTION_H_

#include "rtl876x.h"
#include "trace.h"

typedef enum
{
    /***** Menu *******/
    INTERACT_BEGIN = 1,
    /**** All menu should place in this group ****/
    INTERACT_IDLE,
    INTERACT_HeartRate,
    INTERACT_BloodPressure,
    INTERACT_COUNTDOWN,
    INTERACT_HRS_DISPALY_VALUE,
    INTERACT_WEATHER,
    INTERACT_FIND_PHONE,
    INTERACT_START_FIND,
    INTERACT_POWEROFF,
    INTERACT_POWER_ON_AND_OFF,
    INTERACT_FUNCTION_MENU_MAIN,
    INTERACT_MUSIC_MENU,
    INTERACT_CAMERA_MENU,
    INTERACT_REBOOT_MENU,

    /***** Notify Message ******/
    INTERACT_CALLING_ACCEPT,        //14
    INTERACT_CALLING_REFUSE,
    INTERACT_CALLING,
    INTERACT_PHONEMESSAGE,
    INTERACT_NOTIFY_OTHERS,
    INTERACT_QQ,
    INTERACT_TIM,
    INTERACT_GMAIL,
    INTERACT_WECHAT,
    INTERACT_LINE,
    INTERACT_TWITTER,
    INTERACT_FACEBOOK,
    INTERACT_MESSENGER,
    INTERACT_WHATSAPP,
    INTERACT_LINKEDIN,
    INTERACT_INSTAGRAM,
    INTERACT_SKYPE,
    INTERACT_VIBER,
    INTERACT_KAKAOTALK,
    INTERACT_VKONTAKTE,
    INTERACT_DINGTALK,
    INTERACT_WORKWECHAT,
    INTERACT_APLUS,
    INTERACT_LINK,
    INTERACT_BEIKE,
    INTERACT_LIANJIA,
    INTERACT_MAIL,
    INTERACT_CALENDAR,
    INTERACT_APPLE_FACETIME,
    /***** Reminder Message ******/
    INTERACT_FIND_ME,
    INTERACT_LOGIN,
    INTERACT_CANCEL_BOND,
    INTERACT_BONDED,
    INTERACT_LOGO,
    INTERACT_CAMERA,
    INTERACT_PASSKEY,
    INTERACT_HEARTRATEHIGH,
    INTERACT_TARGET,
    INTERACT_NO_MOVEMENT,
    INTERACT_BAT_LOW_LEVEL,
    INTERACT_CHARGESTATUS,
    INTERACT_HEADSETSTATUS,
    INTERACT_ALARM,
    INTERACT_HRS_DETECT_FAIL,
    INTERACT_OTAUpdating,

    /***** Stop ******/
    INTERACT_STOP_MOTOR_ONLY,
    INTERACT_STOP_OLED_ONLY,
    INTERACT_STOP_MOTOR_AND_OLED,
    INTERACT_END,

} INTERACT_Type;//include interact priority

typedef enum
{
    Notify_calendar,
    Notify_facebook,
    Notify_facetime,
    Notify_instagram,
    Notify_kakaotalk,
    Notify_line,
    Notify_linkedin,
    Notify_mail,
    Notify_messenger,
    Notify_others,
    Notify_QQ,
    Notify_skype,
    Notify_SMS,
    Notify_snap,
    Notify_tim,
    Notify_twitter,
    Notify_viber,
    Notify_vk,
    Notify_wechat,
    Notify_whatsapp,
    Notify_gmail,
    Notify_dingtalk,
    Notify_workwechat,
    Notify_aplus,
    Notify_LINK,
    Notify_beike,
    Notify_lianjia,

    Notify_calling,
} Notifications_Type;// NOTIFICATIONS_TYPE;


typedef enum
{
    language_English    = 0,
    language_SChinese   = 1,
    language_TChinese   = 2,
    language_Polish     = 13,
    language_Denmark    = 17,
    language_German     = 18,
    language_Russisch   = 19,
    language_French     = 20,
    language_Finnish    = 22,
    language_Korean     = 29,
    language_Dutch      = 31,
    language_Norwegian  = 60,
    language_Portuguese = 62,
    language_Japanese   = 65,
    language_Sweden     = 66,
    language_Ukraine    = 85,
    language_Spanish    = 87,
    language_Greek      = 89,
    language_Italian    = 96,
    language_Indonesian = 99,
} language_t;

typedef struct
{
    INTERACT_Type type;
    uint8_t frame_num;
    void *pValue;
} T_IO_HUB_INTERACTION;
void wristband_system_interact(INTERACT_Type type, uint8_t frame_num, void *pValue);
void hub_system_interact(INTERACT_Type type, uint8_t frame_num, void *pValue);
#endif /* _WRISTBAND_INTERACTION_H_ */

