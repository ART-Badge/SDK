/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-05-28     Supperthomas the first version
 */

#ifndef __BSAL_APP_API_H__
#define __BSAL_APP_API_H__
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include "bsal_common.h"

//0. CALLED BY STACK
/**
* BSAL connect status callback
*
* @param stack_ptr the point of the stack object
* @param conn_handle the connect handle of link
* @param conn_status the connection status
* @Note  callback the connect status called by stack
*/
int bsal_gap_connect_status(void *stack_ptr, uint16_t conn_handle, uint8_t connect_status, uint16_t disconnect_status);

//====================================ENTER MAIN==============================

//=================================APP API CALLED BY APP=======================

/**
* BSAL find the stack ptr
*
* @param stack_ptr the point of the stack object
* @param stack_name the name of the stack
* @Note  find the object ptr of the stack by the name
*        called by APP
*/
void *bsal_find_stack_ptr(char *stack_name);

/**
* BSAL init the stack
*
* @param stack_ptr the point of the stack object
* @param callback the function to deal with the event
* @Note  find the object ptr of the stack by the name
*        called by APP
*/
int bsal_stack_init(void *stack_ptr, void *callback);


/**
* BSAL start register the profile table
*
* @param stack_ptr the point of the stack object
* @param num the all number of the profile table
* @param p_fun_cb the function of the profile to callback
* @Note start to register the profile with the num
*/

void bsal_stack_le_srv_begin(void *stack_ptr, uint8_t num, void *p_fun_cb);

/**
* BSAL end register the profile table
*
* @param stack_ptr the point of the stack object
* @Note the end to register the profile with the num
*/
void bsal_stack_le_srv_end(void *stack_ptr);

/**
* BSAL start the stack
*
* @param stack_ptr the point of the stack object
* @Note to start the stack
*/
void bsal_stack_startup(void *stack_ptr);

//2. SERVICE CALLBACK API APP NEED KNOWN

typedef enum
{
    BSAL_CALLBACK_TYPE_READ_CHAR_VALUE = 1,              /**< client read event */
    BSAL_CALLBACK_TYPE_WRITE_CHAR_VALUE = 2,             /**< client write event */
    BSAL_CALLBACK_TYPE_INDIFICATION_NOTIFICATION = 3,    /**< CCCD update event */
    BSAL_CALLBACK_TYPE_HANDLE_TABLE = 4,                 /**< handle event */
} T_BSAL_SRV_CB_TYPE;

struct bsal_callbak_data
{
    void                    *stack_ptr;
    uint8_t                 conn_id;
    T_BSAL_SRV_CB_TYPE      msg_type;
    uint16_t                start_handle;
    uint32_t                off_handle;
    uint16_t                length;
    uint8_t                 *data;
    uint32_t                value;                   /**< CCCD save the temp value */
    bsal_uuid_any_t         srv_uuid;
};
typedef struct bsal_callbak_data bsal_callbak_data_t;


//3.ADV OPERATE
struct le_adv_ltv
{
    uint8_t length;
    uint8_t type;
    uint8_t *value;
};
typedef struct le_adv_ltv  le_adv_ltv_t;
/**
* BSAL ADD FLAG LTV TO adv_data.
*
* @param adv_data the array of adv_data
* @param flag the flag value of adv_type
* @Note the function is used for add flag
*/

void bsal_le_adv_data_add_flag(uint8_t *adv_data , uint8_t flag);
/**
* BSAL ADD name
*
* @param adv_data the array of adv_data
* @param name_length the length of name
* @note the function is used for add fla g
*/
void bsal_adv_data_add_name(uint8_t *adv_data, uint8_t name_length, char *name);

/**
* BSAL ADD ONE UUID
*
* @param adv_data the array of adv_data
* @param uuid the uuid valule of service
* @note the function is used for add UUID
*/
void bsal_adv_data_add_uuid16(uint8_t *adv_data, uint16_t uuid);
/**
* BSAL add LTV TO adv_data.
*
* @param adv_data the array of adv_data
* @param ltv the struct of lengthTypeValue
* @param timeout the maximum response time
* @Note the function is used in combine the adv_data of ltv
*/
void bsal_le_adv_data_add_ltv(uint8_t *adv_data, le_adv_ltv_t *ltv);
/**
* BSAL Set the adv data to stack.
*
* @param stack_ptr the object of the stack
* @param adv_data the array of adv_data
* @Note the function is used for add the data to stack
*/
void bsal_set_le_adv_data_user(void *stack_ptr, uint8_t *adv_data);
/**
* BSAL Set the scan rsp data to stack.
*
* @param stack_ptr the object of the stack
* @param scan_rsp_data the array of scan_rsp_data
* @Note the function is used for add the scan response data to stack
*/
void bsal_set_le_scan_rsp_data_user(void *stack_ptr, uint8_t *scan_rsp_data);

/** @defgroup BSAL_SET_LE_PARAM_TYPE
  * @{
  */
#define BSAL_SET_LE_ADV_PARAM                BIT(0)
#define BSAL_SET_LE_SCAN_RSP_DATA            BIT(1)
#define BSAL_SET_LE_ADV_DATA                 BIT(2)
/**
  * @}
  */
struct bsal_adv_param
{
    uint8_t  set_param_flag;                         //!< set the param bit see #BSAL_SET_LE_PARAM_TYPE
    uint16_t adv_int_min;                            //!< set adv interval min
    uint16_t adv_int_max;                            //!< set adv interval max
    uint8_t  adv_type;                                //!< set adv type
    uint8_t  own_address_type;                        //!< own address type
    uint8_t  direct_address_type;                     //!< set direct bt mac address type
    uint8_t  direct_address[6];                       //!< set the direct mac address
    uint8_t  channel_map;                             //!< set the channel map see  @ref BTCHANNEL_MAP
    uint8_t  filter_policy;                           //!< the filter policy see @ref ADV_PARAM_FILTER_POLICY

    uint8_t scan_rsp_data[BSAL_MAX_ADV_SCAN_DATA_LENGTH];                        //the scan rsp data

    uint8_t adv_data[BSAL_MAX_ADV_SCAN_DATA_LENGTH];                             //the adv data
};
typedef struct bsal_adv_param bsal_adv_param_t;
/**
* BSAL set le advertising param.
*
* @param stack_ptr the object of the stack
* @param adv_param the point of the adv_param
* @Note the function is used for set the adv param
*/
void bsal_set_le_adv_parm(void *stack_ptr, bsal_adv_param_t *adv_param);
/**
* BSAL start adv
*
* @param stack_ptr the object of the stack
* @Note the function is used for start tx the adv
*/
int bsal_stack_start_adv(void *stack_ptr);
/**
* BSAL stop adv
*
* @param stack_ptr the object of the stack
* @Note the function is used for stop tx the adv
*/
int bsal_stack_stop_adv(void *stack_ptr);
//BOND
//self define
typedef enum
{
    BSAL_NO_INPUT,
    BSAL_INPUT_YES_NO,
    BSAL_INPUT_KEYBOARD,
} T_BSAL_GAP_INPUT_CAP;

typedef enum
{
    BSAL_NO_OUTPUT,
    BSAL_YES_OUTPUT,
} T_BSAL_GAP_OUTPUT_CAP;
//bond
struct bsal_bond_param
{
    uint8_t  LE_BR_EDR_FLAG;                                       // LE is 1 BR/EDR is 0
    bool     bonding_flag;                                         //is bond
    uint8_t  oob_flag;                                             //not support;
    uint8_t  auth_type;                                            //AuthReq flag of device
    uint8_t  io_capability;                                        //io_capability

    //useless
    uint8_t  force_auth_type;
    bool     fixed_key_flag;
    uint32_t fixed_key;
    bool     sm_req_enable;
};
typedef struct bsal_bond_param bsal_bond_param_t;

/**
* BSAL set the bond type
*
* @param stack_ptr the object of the stack see
* @param is_bond the bool about is needed bond
* @param input the ability of input  see #T_BSAL_GAP_INPUT_CAP
* @param output the ability of output  see #T_BSAL_GAP_OUTPUT_CAP
* @param bond_type the bond type of the #BSAL_GAP_AUTH_TYPE
* @Note the function is used for stop tx the adv
*/
uint16_t bsal_set_device_le_bond_type(void *stack_ptr, bool is_bond, uint8_t input, uint8_t output, uint8_t bond_type, bool oob_enable);


/**
* BSAL set the bond param
*
* @param stack_ptr the object of the stack see
* @param bond_param the param of the bond
* @Note the function is used for le bond param directly
*/
uint16_t bsal_set_device_le_bond_param(void *stack_ptr, bsal_bond_param_t *bond_param);
struct bsal_gaps_param
{
    uint8_t name[BSAL_MAX_NAME_LENGTH];                          //!<  The GAP's SHOW NAME MAX LENGTH IS 40
    uint16_t apperance;                        //!<  The GAP's apperance
};
typedef struct bsal_gaps_param bsal_gaps_param_t;
/**
* BSAL set gap param set the param of gap
*
* @param stack_ptr the object of the stack
* @param gaps_param the struct of the gaps_param see #bsal_gaps_param_t
* @Note the function is set the gap param
*/
void bsal_set_gap_param(void *stack_ptr, bsal_gaps_param_t *gaps_param);
/**
* BSAL set set the device name
*
* @param stack_ptr the object of the stack
* @param length the length of the name
* @param name the buffer of the name
* @Note the function is set the device name
*/
int bsal_set_device_name(void *stack_ptr, uint8_t length, uint8_t *name);
/**
* BSAL set get the device name
*
* @param stack_ptr the object of the stack
* @Note the function is get the device name
*/
uint8_t *bsal_get_device_name(void *stack_ptr);

//========================================PROFILE API==CALLED BY SERVICE===========================
//2. SERVICE API
typedef BSAL_STATUS(*P_SRV_GENERAL_CB)(void *p_para);

/**
* BSAL register the profile table
*
* @param stack_ptr the object of the stack
* @param srv_def the table of the service
* @param callback the callback function of the profile
* @Note the function is used for register the servcie table
*       called by the profile
*/
int  bsal_stack_le_srv_reg_func(void *stack_ptr, void *srv_def, P_SRV_GENERAL_CB *callback);

//3. SERVICE SEND DATA

/**
* BSAL send the notify data
*
* @param stack_ptr the object of the stack
* @param conn_id the connect id of the link(like conn_handle)
* @param service_id the identify of the profile
* @param offset_handle the offset handle of the att in the profile
* @param value_length the data length of the data
* @param value the buff of the data
* @Note  called by the profile to send the notify
*/

int bsal_srv_send_notify_data(void *stack_ptr, uint16_t conn_id, uint16_t service_id, uint16_t offset_handle, uint16_t value_length, uint8_t *value);

/**
* BSAL send the indicate data
*
* @param stack_ptr the object of the stack
* @param conn_id the connect id of the link(like conn_handle)
* @param service_id the identify of the profile
* @param offset_handle the offset handle of the att in the profile
* @param value_length the data length of the data
* @param value the buff of the data
* @Note  called by the profile to send the indicate
*/
int bsal_srv_send_ind_data(void *stack_ptr, uint16_t conn_id, uint16_t service_id, uint16_t offset_handle, uint16_t value_length, uint8_t *value);

/**
* BSAL get the start handle
*
* @param stack_ptr the object of the stack
* @param uuid the profile main service uuid
* @Note  called by the profile to get the start handle
*/
uint16_t bsal_srv_get_start_handle(void *stack_ptr, bsal_uuid_any_t uuid);
/**
* BSAL get the start handle by value handle
*
* @param stack_ptr the object of the stack
* @param handle the u_handle of the device
* @Note  called by the profile to get the start handle
*/
uint16_t bsal_profile_get_start_handle(void *stack_ptr, uint16_t value_handle);




/**
* BSAL WRITE ATT
*
* @param p_db   the database of data
* @param u_handle the unique handle of mcu
* @note the function is used for read attribute
*/
int bsal_srv_write_data(void *stack_ptr, uint16_t start_handle, uint16_t offset_handle, uint16_t value_length, uint8_t *value);

/**
* BSAL READ ATT
*
* @param p_db   the database of data
* @param u_handle the unique handle of mcu
* @note the function is used for read attribute
*/
int bsal_srv_read_data(void *stack_ptr, uint16_t start_handle, uint16_t offset_handle, uint16_t *value_length, uint8_t **value);

/**
* BSAL CREATE ATT
*
* @param p_db   the database of data
* @param u_handle the unique handle of mcu
* @note the function is used for read attribute
*/
int bsal_srv_create_att(void *stack_ptr, uint16_t start_handle, uint16_t offset_handle, uint16_t value_length);
/**
* BSAL WRITE ATT
*
* @param p_db   the database of data
* @param u_handle the unique handle of mcu
* @note the function is used for read attribute
*/
int bsal_srv_write_data_by_handle(void *stack_ptr, uint16_t u_handle, uint16_t value_length, uint8_t *value);

/**
* BSAL READ ATT
*
* @param p_db   the database of data
* @param u_handle the unique handle of mcu
* @note the function is used for read attribute
*/
int bsal_srv_read_data_by_handle(void *stack_ptr, uint16_t u_handle, uint16_t *value_length, uint8_t **value);

/**
* BSAL CREATE ATT
*
* @param p_db   the database of data
* @param u_handle the unique handle of mcu
* @note the function is used for read attribute
*/
int bsal_srv_create_att_by_handle(void *stack_ptr, uint16_t u_handle, uint16_t value_length);

//=========================================STACK API ==CALLED BY STACK===========
/**
* BSAL READ ATT EVENT
*
* @param stack_ptr   point of the stack
* @param conn_handle conn id of the link
* @param start_handle the start handle of the profile
* @param offset_handle the att value of the handle
* @param length the length of the value
* @param[out] the buffer address of the value
* @note read the callback event, called by the stack
*/
void bsal_gatt_read_callback_event(void *stack_ptr, uint16_t conn_handle, uint16_t start_handle, uint16_t offset_handle, uint16_t *length , uint8_t **value);
/**
* @brief BSAL write ATT EVENT
*
* @param stack_ptr   point of the stack
* @param conn_handle conn id of the link
* @param start_handle the start handle of the profile
* @param offset_handle the att value of the handle
* @param length the length of the value
* @param value the data buffer address
* @note write the callback event, called by the stack
*/
void bsal_gatt_write_callback_event(void *stack_ptr, uint16_t conn_handle, uint16_t start_handle, uint16_t offset_handle, uint16_t length , uint8_t *value);
/**
* @brief BSAL CCCD ATT EVENT
*
* @param stack_ptr   point of the stack
* @param conn_handle conn id of the link
* @param start_handle the start handle of the profile
* @param offset_handle the att offset handle
* @param length the length of the value
* @param value the cccd_bit of the device
* @note cccd the callback event, called by the stack
*/
void bsal_gatt_cccd_callback_event(void *stack_ptr, uint16_t conn_handle, uint16_t start_handle, uint16_t offset_handle, uint16_t value);
/**
* BSAL READ ATT EVENT
*
* @param stack_ptr   point of the stack
* @param conn_handle conn id of the link
* @param u_handle the unique handle of the device
* @param length the length of the value
* @param[out] the buffer address of the value
* @note read the callback event, called by the stack
*/
void bsal_gatt_read_callback_event_by_handle(void *stack_ptr, uint16_t conn_handle, uint16_t u_handle, uint16_t *length , uint8_t **value);
/**
* @brief BSAL write ATT EVENT
*
* @param stack_ptr   point of the stack
* @param conn_handle conn id of the link
* @param u_handle the unique handle of the device
* @param length the length of the value
* @param value the data buffer address
* @note write the callback event, called by the stack
*/
void bsal_gatt_write_callback_event_by_handle(void *stack_ptr, uint16_t conn_handle, uint16_t u_handle, uint16_t length , uint8_t *value);
/**
* @brief BSAL CCCD ATT EVENT
*
* @param stack_ptr   point of the stack
* @param conn_handle conn id of the link
* @param u_handle the unique handle of the device
* @param value the CCCD value of the device
* @note cccd the callback event, called by the stack
*/
void bsal_gatt_cccd_callback_event_by_handle(void *stack_ptr, uint16_t conn_handle, uint16_t u_handle , uint16_t value);

#ifdef __cplusplus
}
#endif

#endif

