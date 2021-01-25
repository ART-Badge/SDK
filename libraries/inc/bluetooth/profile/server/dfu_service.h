/**
*********************************************************************************************************
*               Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     dfu_service.h
* @brief
* @details
* @author   ken_mei
* @date     02-09-2016
* @version  v1.0.0
******************************************************************************
* @attention
* <h2><center>&copy; COPYRIGHT 2015 Realtek Semiconductor Corporation</center></h2>
******************************************************************************
*/

#ifndef _DFU_SERVICE_H_
#define _DFU_SERVICE_H_

#ifdef __cplusplus
extern "C"  {
#endif      /* __cplusplus */

#include "profile_server.h"
#include "patch_header_check.h"
#include "dfu_api.h"

#define DFU_NOTIFY_ENABLE                       1
#define DFU_NOTIFY_DISABLE                      2

/*attribut index*/
#define INDEX_DFU_PACKET_VALUE                  0x02
#define INDEX_DFU_CONTROL_POINT_CHAR_VALUE      0x04
#define INDEX_DFU_CHAR_CCCD_INDEX               0x05



typedef struct _T_DFU_CTRL_POINT
{
    uint8_t opcode;
    T_IMG_CTRL_HEADER_FORMAT start_dfu; /* start dfu trans first 12 bytes ctrl header*/
} T_DFU_CTRL_POINT, * P_DFU_CTRL_POINT;

typedef struct
{
    T_IMG_CTRL_HEADER_FORMAT ctrl_header;
    uint32_t image_total_length;
    uint32_t origin_image_version;
    uint32_t cur_offset;
    uint8_t mtu_size;
    bool dfu_conn_para_update_in_progress;
    uint16_t dfu_conn_interval;
    uint16_t dfu_conn_lantency;
} T_DFU_PARA;

typedef enum
{
    DFU_FAIL_UPDATE_FLASH,
    DFU_FAIL_SYSTEM_RESET_CMD,
    DFU_FAIL_EXCEED_MAX_BUFFER_SIZE,
    DFU_FAIL_EXCEED_IMG_TOTAL_LEN,

} T_DFU_FAIL_REASON;

typedef enum
{
    DFU_WRITE_ATTR_ENTER,
    DFU_WRITE_ATTR_EXIT,
    DFU_WRITE_START,
    DFU_WRITE_DOING,
    DFU_WRITE_END,
    DFU_WRITE_FAIL,
} T_DFU_WRITE_OPCODE;


typedef struct
{
    uint8_t opcode;
    uint8_t write_attrib_index;
    uint16_t length;
    uint8_t *p_value;
} T_DFU_WRITE_MSG;

typedef union _TDFU_UPSTREAM_MSG_DATA
{
    uint8_t notification_indification_index;
    T_DFU_WRITE_MSG write;
} T_DFU_UPSTREAM_MSG_DATA;

/** Dfu service data to inform application */
typedef struct _TDFU_CALLBACK_DATA
{
    uint8_t                  conn_id;
    T_SERVICE_CALLBACK_TYPE  msg_type;
    T_DFU_UPSTREAM_MSG_DATA  msg_data;
} T_DFU_CALLBACK_DATA;

extern uint8_t *p_ota_temp_buffer_head;
extern uint16_t g_ota_tmp_buf_used_size;
extern T_DFU_PARA g_dfu_para;

extern void dfu_notify_conn_para_update_req(uint8_t conn_id, T_DFU_ARV_ERROR_CODE error_code);
extern void dfu_service_handle_valid_fw(uint8_t conn_id);
extern uint8_t dfu_add_service(void *pFunc);

#ifdef __cplusplus
}
#endif

#endif
