#ifndef _DEBUG_CMD_TP_H_
#define _DEBUG_CMD_TP_H_

#include "rtl876x.h"
#include "trace.h"
#include "app_msg.h"


void parse_raw_package(void);
void sim_tp_init(void);
void sim_tp_interrupt(void);
void sim_tp_release(void);
void sim_tp_handle_package(uint8_t *buf, uint32_t len);

#endif /* _DEBUG_CMD_TP_H_ */

