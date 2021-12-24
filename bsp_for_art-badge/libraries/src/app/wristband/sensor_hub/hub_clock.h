#ifndef _WRISTBNAD_CLOCK_H_
#define _WRISTBNAD_CLOCK_H_


#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "communicate_protocol.h"
#include "board.h"

#define IsLeapYear(yr) (!((yr) % 400) || (((yr) % 100) && !((yr) % 4)))

#define YearLength(yr) (IsLeapYear(yr) ? 366 : 365)



#define BEGYEAR                     2000     // UTC started at 00:00:00 January 1, 2000
#define DAY                         86400UL  // 24 hours * 60 minutes * 60 seconds
#define SYSTEM_ORIGIN_DAY_OF_WEEK   (Sat)  //2000-01-01 is sat
#define LENGTH_OF_WEEK              (7)
#define RTC_CNT_MAX_VALUE           (1024*1024*16UL -1)         //RTC->CNT: [23:0]
#define RTC_PRESCALER_VALUE         0


extern uint32_t seconds_diff;
extern bool  seconds_cali_enable;

// To be used with
typedef struct
{
    volatile uint16_t year;    // 2000+
    volatile uint8_t month;    // 0-11
    volatile uint8_t day;      // 0-30
    volatile uint8_t seconds;  // 0-59
    volatile uint8_t minutes;  // 0-59
    volatile uint8_t hour;     // 0-23
} T_UTC_TIME;

typedef enum
{
    MOn  = 0,
    Tues  = 1,
    Wed  = 2,
    Thur = 3,
    Fri  = 4,
    Sat  = 5,
    Sun  = 6
} DAY_OF_WEEK;

#define ABS_PARAMS(a,b)             ((a>b) ? (a-b):(b-a))
typedef void (* pSystemClockCB)(void);
extern pSystemClockCB SystemClockCallBack;



void clock_add_hub_task(void);
static void wristband_clock_driver_init(void);
static void wristband_clock_enter_dlps(void);
static void wristband_clock_exit_dlps(void);
bool wristband_clock_allowed_enter_dlps_check(void);

void wall_clock_start(void);
void system_clock_init(uint32_t second);
void set_wristband_clock(time_union_t time);

DAY_OF_WEEK get_day_of_week(uint32_t secTime);//used to calculate day of week
static uint8_t month_length_calc(uint8_t lpyr, uint8_t mon);


void convert_to_utc_time(uint32_t sec_time);
uint32_t convert_time_to_second(time_union_t time);
void minute_system_clock_message_handle(void);
void update_system_clock(void);
void check_alarm(void);
void do_rtc_alarm(void);
void check_passed_onetime_alarm(void);
void RTCStartDlpsTimerAlign(uint32_t timeInTicks);
void RTCStopDlpsTimerAlign(void);
uint8_t get_system_clock_second(void);


#ifdef __cplusplus
}
#endif


#endif //_WRISTBNAD_CLOCK_H_

