/**
*********************************************************************************************************
*               Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* @file      board.h
* @brief     header file of Keypad demo.
* @details
* @author    tifnan_ge
* @date      2015-06-26
* @version   v0.1
* *********************************************************************************************************
*/


#ifndef _POWER_TEST_H_
#define _POWER_TEST_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "rtl876x.h"

typedef enum _POWER_SCENARIO
{
    POWER_ACTIVE = 0,
    POWER_SLEEP_OVERALL,
    POWER_SLEEP_VDIGI,
    POWER_DLPS,
    POWER_POWERDOWN,
    POWER_ADV_CONN,
    POWER_SCAN,
    POWER_CONTINUE_TX,
    POWER_CONTINUE_RX,
} T_POWER_SCENARIO;

typedef enum _POWER_SUPPLY_1P2
{
    POWER_SUPPLY_BUCK = 0,
    POWER_SUPPLY_LDO,
} T_POWER_SUPPLY_1P2V;

typedef enum _CPU_CLOCK
{
    CPU_CLOCK_40MHZ = 40000000,
    CPU_CLOCK_53MHZ = 53333333,
    CPU_CLOCK_80MHZ = 80000000,
    CPU_CLOCK_20MHZ = 20000000,
    CPU_CLOCK_10MHZ = 10000000,
    CPU_CLOCK_5MHZ = 5000000,
    CPU_CLOCK_2P5MHZ = 2500000,
    CPU_CLOCK_1P25MHZ = 1250000,
    CPU_CLOCK_625KHZ = 625000,
} T_CPU_CLOCK;

typedef enum _TX_POWER
{
    TX_MINUS_20_DBM,
    TX_0_DBM,
    TX_4_DBM,
    TX_8_DBM,
} T_TX_POWER;

typedef struct _POWER_SETTINGS
{
    uint8_t power_test_signature[8];
    T_POWER_SCENARIO power_scenario;
    T_POWER_SUPPLY_1P2V power_supply_1p2v;
    T_CPU_CLOCK active_clock;
    T_CPU_CLOCK slow_clock;
    uint16_t adv_interval;//interval = adv_interval * 0.625ms==>0x20:20ms  0x140 200ms 0x1e0: 300ms 0x640:1000ms
    uint16_t conn_interval;//interval = conn_interval * 1.25ms==>0x50:100ms
    T_TX_POWER tx_power;
    bool guard_time_tune_en;
} T_POWER_SETTINGS;

//extern const T_POWER_SETTINGS power_settings;
extern  T_POWER_SETTINGS power_settings;
#ifdef __cplusplus
}
#endif

#endif  /* _POWER_TEST_H_ */
