#ifndef _GUI_COMMON_MEEEEENU_H_
#define _GUI_COMMON_MEEEEENU_H_

#include "rtl876x.h"
#include "trace.h"
#include "gui_core.h"

typedef union
{
    uint8_t data;
    struct
    {
        uint8_t counter : 5;
        uint8_t reverse : 2;
        uint8_t start   : 1;
    } field;
} Navigation_param_t;

typedef union
{
    uint8_t data;
    struct
    {
        uint8_t counter : 4;
        uint8_t reverse : 3;
        uint8_t press   : 1;
    } field;
} ShowMessage_param_t;

typedef enum
{
    remainder_idle,
    remainder_alarm,
    remainder_findMe,
    remainder_login,
    remainder_cancelBond,
    remainder_bond,
    remainder_logo,
    remainder_target,
    remainder_noMovement,
    remainder_lowBattery,
    remainder_camera,
    remainder_heartRateHigh,
    remainder_chargeStatus,
    remainder_passkey,
    remainder_headsetStatus,
    remainder_callingRefuse,
    remainder_OTAUpdating,

} Reminder_Type;

typedef enum
{
    CLOCK_1ST_MENU,
    CLOCK_2ND_MENU,
    CLOCK_3RD_MENU,
    CLOCK_4TH_MENU,
    CLOCK_5TH_MENU,
    CLOCK_6TH_MENU,
    CLOCK_MAX_MENU,
    CLOCK_WATCHFACE_MENU,
    TEST_UI_MASK_MENU,
    TEST_UI_SETTING_TIME_MENU,
} T_CLOCK_MENU_TYPE;
/**************************************************************************
* stopwatch state defination
***************************************************************************/
typedef struct ExerciseWatchClass
{
    uint32_t  sec       : 6;
    uint32_t  min       : 6;
    uint32_t  hour      : 5;
    uint32_t  flag      : 1;
    uint32_t  running   : 1;
    uint32_t  pause     : 1;
    uint32_t  storeflag : 1;
    uint32_t  midnight  : 1;
    uint32_t  datepage  : 2;
    uint32_t  pause_counter   : 7;
    uint32_t  countdown : 1;
} ExerciseWatchType_t;

typedef struct _EXERCISE_DATA_t
{
    uint32_t start_steps;
    uint32_t start_distance;
    uint32_t start_calories;
    uint32_t end_steps;
    uint32_t end_distance;
    uint32_t end_calories;
    uint32_t midnight_steps;
    uint32_t midnight_distance;
    uint32_t midnight_calories;
    uint32_t pause_steps;
    uint32_t pause_distance;
    uint32_t pause_calories;
} EXERCISE_DATA_t;

typedef struct _EXERCISE_PAUSE_DATA_t
{
    uint32_t pause_start_CountRTC;
    uint16_t pause_minutes;
    uint8_t  pause_second;

} EXERCISE_PAUSE_DATA_t;


extern Navigation_param_t navigation_param;
extern ShowMessage_param_t showmsg_param;
extern Reminder_Type   remainderID;
extern ExerciseWatchType_t exerciseWatch;
extern EXERCISE_DATA_t  exerciseData;
extern EXERCISE_PAUSE_DATA_t exercisePauseData;
extern uint8_t messageCounter;
extern uint8_t unreadMessage;

extern UI_MenuTypeDef *pInitMenu;
extern UI_MenuTypeDef *pShowMenu;
extern UI_MenuTypeDef g_cur_menu;
extern UI_MenuTypeDef g_sub_menu;
extern UI_MenuTypeDef g_par_menu;

extern UI_MenuTypeDef ExerciseMenu;
extern UI_MenuTypeDef ClockMenu;
extern UI_MenuTypeDef ClockSpecialMenu;
extern UI_MenuTypeDef ClockFitMenu;
extern UI_MenuTypeDef NotificationMenu;
extern UI_MenuTypeDef SettingMenu;
extern UI_MenuTypeDef ExerciseRunMenu;
extern UI_MenuTypeDef WatchFaceAWMenu;
extern UI_MenuTypeDef MessageMenu;

extern UI_MenuTypeDef TestPSRAM0Menu;
extern UI_MenuTypeDef TestPSRAM1Menu;

extern UI_MenuTypeDef MPTestUIMenu;
extern UI_MenuTypeDef PlayerUIMenu;
extern UI_MenuTypeDef BTInquiryUIMenu;
extern UI_MenuTypeDef MusicUIMenu;
extern UI_MenuTypeDef HeadSetDevUIMenu;
extern UI_MenuTypeDef AmsPlayerUIMenu;


extern UI_MenuTypeDef Test454MenuMain;
extern UI_MenuTypeDef Test454MenuLeft1;
extern UI_MenuTypeDef Test454MenuLeft2;
extern UI_MenuTypeDef Test454MenuRight1;
extern UI_MenuTypeDef Test454MenuRight2;
extern UI_MenuTypeDef Test454MenuUp1;
extern UI_MenuTypeDef Test454MenuDown1;

extern UI_MenuTypeDef TempTest0;
extern UI_MenuTypeDef TempTest1;
extern UI_MenuTypeDef TempTest2;
extern UI_MenuTypeDef TempTest3;
extern UI_MenuTypeDef TempTest4;

extern void    *gui_dynamic_timer    ;
extern void    *gui_display_timer    ;
extern void    *gui_page_switch_timer;



#endif /* _GUI_COMMON_MEEEEENU_H_ */

