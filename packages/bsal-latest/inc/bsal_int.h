/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-05-28     Supperthomas the first version
 */


#ifndef __BSAL_INT_H__
#define __BSAL_INT_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include "bsal.h"

struct bsal_uuid
{
    uint8_t type;
    uint8_t data[16];
    uint16_t propertiy;
    uint16_t permision;
    uint32_t *handle;
};
typedef struct bsal_uuid bsal_uuid_t;

struct bsal_gatt_value_entry
{
    uint32_t u_handle;
    uint8_t *value;
    uint16_t value_length;
    uint32_t alloc_size;
    bool append;///for future
    struct bsal_gatt_value_entry *next_entry;
};
typedef struct bsal_gatt_value_entry bsal_gatt_value_entry_t;

struct bsal_stack_ops
{
    BSAL_STATUS(*le_set_stack_param)(uint32_t value_length, uint8_t *value);         //set param  param_type firstbit   1:br/edr param 0: ble
    BSAL_STATUS(*le_get_stack_param)(uint32_t value_length, uint8_t *value);        //get param  param_type firstbit   1:br/edr param 0: ble
    void (*get_mac_address)(uint8_t addr_type, uint8_t *bd_addr);                                      //get mac addr
    void (*init)(uint8_t stack_type);                                                                  //stack init     //set the default param stack_type: bit 0 :BR/EDR. bit 1: BLE, bit2:mesh
    void (*startup)(void);                                                                             //stack init
    //GAP OPERATE
    //bond
    void (*set_bond_param)(bsal_bond_param_t *value);              //set adv param
    void (*get_bond_param)(bsal_bond_param_t *value);
    //ADV PARAM
    void (*le_set_adv_param)(bsal_adv_param_t *adv_param);              //set adv param
    void (*le_get_adv_param)(bsal_adv_param_t *adv_param);
    BSAL_STATUS(*le_adv_enable)(bool enable);                                   // set adv param
    void (*set_gaps_param)(bsal_gaps_param_t *gaps_param);                                               //set device name

    //service profile
    uint32_t (*cov_handle_to_u_handle)(uint16_t service_id, uint16_t offset_handle);               // bsal_hanlde_to_stack_handle
    BSAL_STATUS(*le_srv_reg_begin)(bool is_continue, uint8_t service_num, void *p_func);
    BSAL_STATUS(*le_srv_reg_fun)(struct bsal_gatt_app_srv_def *p_srv, void *p_func);    //tobe define
    BSAL_STATUS(*le_srv_reg_end)(void);
    BSAL_STATUS(*le_srv_send_data)(uint16_t conn_handle, uint16_t service_id, uint16_t offse_handle, uint16_t value_len, const uint8_t *value, uint8_t send_type);
    BSAL_STATUS(*send_notify_complete)(uint32_t credits);   /*TODO*/
    //client profile
    // TO BE CALLED BY BSAL_RTK.C
};
typedef struct bsal_stack_ops  bsal_stack_ops_t;

//load the static info
struct bsal_stack_info
{
    uint8_t public_bd_addr[6];
    uint8_t device_name[BSAL_MAX_NAME_LENGTH];
    uint16_t le_mtu_size;
    uint16_t le_srv_tx_credits;
    bsal_adv_param_t adv_param;
    bsal_bond_param_t bond_param;
};
typedef struct bsal_stack_info bsal_stack_info_t;

typedef BSAL_STATUS(*P_BSAL_PROFILE_CB)(void *p_para);
struct bsal_srv_callback
{
    P_BSAL_PROFILE_CB  bsal_srv_fn_cb;
    uint16_t           start_handle;
    bsal_uuid_any_t    srv_uuid;
};
typedef struct bsal_srv_callback bsal_srv_callback_t;

struct bsal_stack_obj
{
    bool                       stack_ready_flag;
    bsal_stack_ops_t           bsal_ops;
    //att
    bsal_gatt_value_entry_t   *db;
    //service
    bsal_srv_callback_t       *bsal_srv_objs;
    //static info:
    bsal_stack_info_t          bsal_info;

    uint16_t                   g_att_index;
    uint8_t                    srv_num;
    //register
    void (*app_all_callbak)(void *stack_ptr, uint8_t cb_layer, uint16_t cb_sub_event, uint8_t value_length , void *value);
};
typedef struct bsal_stack_obj  bsal_stack_obj_t;

/**
* BSAL create the att first init
*
* @param stack_ptr the point of the stack object
* @param u_handle the unique handle of the att
* @param value_length the length of the value
* @param value the buffer of the value
* @Note create the attribute with the unique handle
*/

int bsal_att_create_entry(void *stack_ptr, uint32_t u_handle, uint16_t value_length);

/**
* BSAL read the attribute with u_handle
*
* @param stack_ptr the point of the stack object
* @param u_handle the unique handle of the att
* @param value_length the length of the value
* @param save the value the buffer of the value
* @Note create the attribute with the unique handle
*/
int bsal_att_read_entry(void *stack_ptr, uint32_t u_handle, uint16_t *value_length, uint8_t **value);

/**
* BSAL write the attribute with u_handle
*
* @param stack_ptr the point of the stack object
* @param u_handle the unique handle of the att
* @param value_length the length of the value
* @param value the buffer of the value
* @Note create the attribute with the unique handle
*/
int bsal_att_write_entry(void *stack_ptr, uint32_t u_handle, uint16_t value_length, uint8_t *value);

/**
* BSAL change the attribute size with u_handle
*
* @param stack_ptr the point of the stack object
* @param u_handle the unique handle of the att
* @param value_length the length of the value
* @param append the flag of append the value
* @Note change the size of value the attribute with the unique handle
*/
int bsal_att_resize_entry(void *stack_ptr, uint32_t u_handle, uint16_t value_length, bool append);

//send data
typedef enum
{
    BSAL_SEND_DATA_TYPE_ANY           = 0x00, /**<  Any PDU type. */
    BSAL_SEND_DATA_TYPE_NOTIFICATION  = 0x01, /**<  Notification PDU type. */
    BSAL_SEND_DATA_TYPE_INDICATION    = 0x02  /**<  Indication PDU type. */
} T_BSAL_SEND_DATA_TYPE;

/**
* BSAL find the start handle by the uuid
*
* @param stack_ptr the point of the stack object
* @param uuid the 16 bit of the uuid
* @Note get the profile start handle by the uuid
*/
uint16_t bsal_profile_get_start_handle_by_16_uuid(void *stack_ptr, uint16_t uuid);

/**
* BSAL get the profile entry by the start handle
*
* @param stack_ptr the point of the stack object
* @param the start handle
* @Note get the profile entry by the start_handle
*/
bsal_srv_callback_t *bsal_profile_get(void *stack_ptr, uint16_t start_handle);

#ifdef __cplusplus
}
#endif

#endif


