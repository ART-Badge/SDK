/**
*********************************************************************************************************
*               Copyright(c) 2014, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* @file      single_tone.h
* @brief     single tone test defines.
* @details   none.
* @author    Chenjie Jin
* @date      2018-05-30
* @version   v2.0
* *********************************************************************************************************
*/

#ifndef _BEE_SINGLETONE_H_
#define _BEE_SINGLETONE_H_

#include "stdbool.h"
#include "stdint.h"

typedef enum
{
    SINGLE_TONE_EVT_OPENED,     /* single tone open completed */
    SINGLE_TONE_EVT_CLOSED,     /* single tone close completed */
    SINGLE_TONE_EVT_DATA_IND,   /* single tone rx data indicated */
    SINGLE_TONE_EVT_DATA_XMIT,  /* single tone tx data transmitted */
    SINGLE_TONE_EVT_ERROR,      /* single tone error occurred */
} T_SINGLE_TONE_EVT;

#pragma pack(1)
typedef struct
{
    uint8_t pkt_type;
    uint16_t opcode;
    uint8_t length;
    uint8_t start;
    uint8_t channle;
    uint8_t tx_power;
    uint8_t is_le;
} T_SINGLE_TONE_VEND_CMD_PARAMS;
#pragma pack()

typedef bool (*P_SINGLE_TONE_CALLBACK)(T_SINGLE_TONE_EVT evt, bool status, uint8_t *p_buf,
                                       uint32_t len);

extern bool hci_if_open(P_SINGLE_TONE_CALLBACK p_callback);

extern bool hci_if_close(void);

extern bool hci_if_write(uint8_t *p_buf, uint32_t len);

extern bool hci_if_confirm(uint8_t *p_buf);

/* export functions */
void single_tone_init(void);

#endif  /* _BEE_SINGLETONE_H_ */

