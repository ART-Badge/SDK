/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-05-28     Supperthomas the first version
 */


#include <string.h>
#include <stdio.h>

#include "bsal.h"
#include "bsal_osif.h"
#include "bsal_int.h"
#include "bsal_stack_api.h"


#include "bsal_rtk.h"

bsal_stack_obj_t rtk_obj;

static void *bsal_get_rtk_obj()
{
    return &rtk_obj;
}
//================================BSAL.C==========================
static void bsal_rtk_stack_init(uint8_t stack_type)
{
#define APP_MAX_LINKS 1
    le_gap_init(APP_MAX_LINKS);   //if changed must be removed from here
    /* Device name and device appearance */
//    uint16_t appearance = GAP_GATT_APPEARANCE_UNKNOWN;
#ifdef SOC_RTL_8762D
	   gap_lib_init();
#endif
    le_register_app_cb(bsal_app_gap_callback);
}

static void bsal_rtk_stack_startup(void)
{
    app_task_init();
}

static BSAL_STATUS bsal_rtk_le_adv_enable(bool enable)
{
    if (enable == true)
    {
        le_adv_start();
    }
    else
    {
        le_adv_stop();
    }
    return BSAL_RESULT_SUCCESS;
}
//for app
static BSAL_STATUS bsal_srv_profile_srv_begin(bool is_continue, uint8_t service_num, void *p_fnc_callback)
{
    server_init(service_num);
    //get local obj,
    //register obj
    bsal_stack_obj_t *p_bsal_stack =  bsal_get_rtk_obj();
    //add 0xffff
    p_bsal_stack->bsal_srv_objs =  bsal_osif_malloc((service_num + 1) * sizeof(bsal_srv_callback_t));
    p_bsal_stack->srv_num = service_num + 1;
    //server_builtin_service_reg(true);
    //save the profile num;
    server_register_app_cb(bsal_app_profile_callback);

    bsal_uuid_any_t srv_uuid;
    srv_uuid.u_type = BSAL_UUID_TYPE_16BIT; //16bit
    srv_uuid.u16.value = 0xffff;//p_srv[i].uuid->u16.value;
    // add the generate uuid for except common profile
    bsal_profile_insert(p_bsal_stack, 0xffff, p_fnc_callback, srv_uuid);

    return BSAL_RESULT_SUCCESS;
}
//common utils
static BSAL_STATUS bsal_srv_profile_reg_service(
    struct bsal_gatt_app_srv_def *p_srv,
    void *p_func  //callback
)
{
    uint16_t att_num = 0;
    uint8_t att_index = 0;
    uint16_t sum_of_descript = 0;
    uint8_t write_index = 0;
    uint8_t i = 0;
    uint8_t j = 0;
    uint8_t x = 0;
    uint8_t y = 0;
    bsal_stack_obj_t *p_bsal_stack =  bsal_get_rtk_obj();

    bsal_gatt_res_t res;
    bsal_util_sum_handle_num(p_srv, &res);
    att_num  = res.attrs;
    T_ATTRIB_APPL *rtk_table = bsal_osif_malloc(sizeof(T_ATTRIB_APPL) * att_num);
    //primary service
    struct bsal_gatt_app_srv_def *tmp_srv;
    bsal_gatt_chr_def_t      *tmp_chr;

    for (i = 0; p_srv[i].type != 0; i++)
    {
        //service
        tmp_srv = p_srv + i;
        if ((tmp_srv->type != BSAL_GATT_UUID_PRIMARY_SERVICE) &&
                (tmp_srv->type != BSAL_GATT_UUID_SECONDARY_SERVICE))
        {
            return BSAL_RESULT_FAIL;
        }
//          att_res->attrs++;
//                  att_res->svcs++;
        if (tmp_srv->uuid->u_type == BSAL_UUID_TYPE_16BIT)
        {
            rtk_table[write_index].type_value[0] = LO_WORD(tmp_srv->type);
            rtk_table[write_index].type_value[1] = HI_WORD(tmp_srv->type);
            rtk_table[write_index].type_value[2] = LO_WORD(tmp_srv->uuid->u16.value);
            rtk_table[write_index].type_value[3] = HI_WORD(tmp_srv->uuid->u16.value);
            rtk_table[write_index].value_len = UUID_16BIT_SIZE;
            rtk_table[write_index].p_value_context = NULL;
            rtk_table[write_index].permissions = GATT_PERM_READ;
            rtk_table[write_index].flags = (ATTRIB_FLAG_VALUE_INCL | ATTRIB_FLAG_LE);
        }
        else if (tmp_srv->uuid->u_type == BSAL_UUID_TYPE_128BIT)
        {
            rtk_table[write_index].type_value[0] = LO_WORD(BSAL_GATT_UUID_PRIMARY_SERVICE);
            rtk_table[write_index].type_value[1] = HI_WORD(BSAL_GATT_UUID_PRIMARY_SERVICE);
            rtk_table[write_index].p_value_context = tmp_srv->uuid->u128.value;
            rtk_table[write_index].value_len = UUID_128BIT_SIZE;
            rtk_table[write_index].permissions = GATT_PERM_READ;
            rtk_table[write_index].flags = (ATTRIB_FLAG_VALUE_INCL | ATTRIB_FLAG_LE);
        }
        write_index++;
        if (tmp_srv->includes != NULL)
        {
            for (j = 0; tmp_srv->includes[j] != NULL; j++)
            {
//                                att_res->attrs++;
//                                att_res->incs++;
                //add include /*TODO*/
            }
        }

        if (tmp_srv->characteristics != NULL)
        {
            for (x = 0; tmp_srv->characteristics[x].uuid != NULL; x++)
            {
                tmp_chr = tmp_srv->characteristics + x;
                if (tmp_chr->uuid->u_type == BSAL_UUID_TYPE_16BIT)
                {
                    /* <<Characteristic>> declear */
                    rtk_table[write_index].flags = (ATTRIB_FLAG_VALUE_INCL);
                    rtk_table[write_index].type_value[0] = LO_WORD(GATT_UUID_CHARACTERISTIC);
                    rtk_table[write_index].type_value[1] = HI_WORD(GATT_UUID_CHARACTERISTIC);
                    rtk_table[write_index].type_value[2] = LO_WORD(tmp_chr->properties & 0xff);
                    rtk_table[write_index].type_value[3] = HI_WORD(tmp_chr->properties >> 8);
                    rtk_table[write_index].permissions = GATT_PERM_READ;
                    rtk_table[write_index].value_len = tmp_chr->value_length;

                    write_index++;
                    /* <<Characteristic>> value */
                    rtk_table[write_index].flags = ATTRIB_FLAG_VALUE_APPL;
                    rtk_table[write_index].type_value[0] = LO_WORD(tmp_chr->uuid->u16.value);
                    rtk_table[write_index].type_value[1] = HI_WORD(tmp_chr->uuid->u16.value);
                    if (BSAL_GATT_PERM_GET_READ(tmp_srv->characteristics[x].permission) !=  0) ///ENCRYTYED
                    {
                        /* TODO*/
                        if (BSAL_GATT_GET_ENC_TYPE_AUTHEN_REQ(BSAL_GATT_PERM_GET_READ(tmp_srv->characteristics[x].permission)))
                        {
                            rtk_table[write_index].permissions |= GATT_PERM_READ_AUTHEN_REQ; //tmp_chr->permission;
                        }
                        else if (BSAL_GATT_GET_ENC_TYPE_AUTHOR_REQ(BSAL_GATT_PERM_GET_READ(tmp_srv->characteristics[x].permission)))
                        {
                            rtk_table[write_index].permissions |= GATT_PERM_READ_AUTHOR_REQ;
                        }
                        else
                        {
                            rtk_table[write_index].permissions |= GATT_PERM_READ;
                        }
                    }
                    else
                    {

                    }
                    //write
                    if (BSAL_GATT_PERM_GET_WRITE(tmp_srv->characteristics[x].permission) !=  0) ///ENCRYTYED
                    {
                        /* TODO*/
                        if (BSAL_GATT_GET_ENC_TYPE_AUTHEN_REQ(BSAL_GATT_PERM_GET_WRITE(tmp_srv->characteristics[x].permission)))
                        {
                            rtk_table[write_index].permissions |= GATT_PERM_WRITE_AUTHEN_REQ; //tmp_chr->permission;
                        }
                        else if (BSAL_GATT_GET_ENC_TYPE_AUTHOR_REQ(BSAL_GATT_PERM_GET_WRITE(tmp_srv->characteristics[x].permission)))
                        {
                            rtk_table[write_index].permissions |= GATT_PERM_WRITE_AUTHOR_REQ;
                        }
                        else
                        {
                            rtk_table[write_index].permissions |= GATT_PERM_WRITE;
                        }
                    }
                    //rtk_table[write_index].permissions = tmp_chr->permission;
                    rtk_table[write_index].value_len = 0;
                    write_index++;
//                            att_res->chrs++;
//                            att_res->attrs +=2;
                    if ((tmp_chr->properties & BSAL_ATT_P_NOTIFY) ||
                            (tmp_chr->properties & BSAL_ATT_P_INDICATE))
                    {
//                                    att_res->dscs++;
//                                    att_res->cccds++;
//                                    att_res->attrs++;
                        //ADD CCCD:
                        rtk_table[write_index].flags = ATTRIB_FLAG_VALUE_INCL | ATTRIB_FLAG_CCCD_APPL;
                        rtk_table[write_index].type_value[0] = LO_WORD(GATT_UUID_CHAR_CLIENT_CONFIG);
                        rtk_table[write_index].type_value[1] = HI_WORD(GATT_UUID_CHAR_CLIENT_CONFIG);
                        rtk_table[write_index].type_value[2] = LO_WORD(GATT_CLIENT_CHAR_CONFIG_DEFAULT);
                        rtk_table[write_index].type_value[3] = HI_WORD(GATT_CLIENT_CHAR_CONFIG_DEFAULT);
#if SIMP_SRV_AUTHEN_EN
                        rtk_table[write_index].permissions  = (GATT_PERM_READ_AUTHEN_REQ | GATT_PERM_WRITE_AUTHEN_REQ);   /* permissions */
#else
                        rtk_table[write_index].permissions = (GATT_PERM_READ | GATT_PERM_WRITE);          /* permissions */
#endif
                        rtk_table[write_index].value_len = 2;
                    }

                    if (tmp_chr->descriptors != NULL)
                    {
                        for (y = 0; tmp_chr->descriptors[y].uuid != NULL; y++)
                        {
//                                         att_res->dscs++;
//                                         att_res->attrs++;
                            /*TODO*/
                        }
                    }
                }
            }
        }
    }
    T_SERVER_ID service_id;

    if (false == server_add_service(&service_id,
                                    (uint8_t *)rtk_table,
                                    sizeof(T_ATTRIB_APPL) * att_num,
                                    bsal_cbs))
    {
        APP_PRINT_ERROR1("bas_add_service: service_id %d", service_id);
        service_id = 0xff;
    }
    APP_PRINT_ERROR1(" bas_add_service: service_id %d", service_id);
//    bsal_srv_objs[service_id].bsal_srv_fn_cb = (P_BSAL_PROFILE_CB)p_func;
//    bsal_srv_objs[service_id].start_handle = 0;// RTK NO NEED

    //insert the profile service_id to start_handle

    uint16_t start_handle = 0;
    for (i = 0; p_srv[i].type != 0; i++)
    {
        bsal_uuid_any_t srv_uuid;
        srv_uuid.u_type = BSAL_UUID_TYPE_16BIT; //16bit
        srv_uuid.u16.value = p_srv[i].uuid->u16.value;
        tmp_srv = p_srv + i;
        p_bsal_stack->g_att_index ++;
        //save the service id with the start_handle;
        //rtk_save_service_id_with_start_handle(service_id, p_bsal_stack->g_att_index);
        start_handle = p_bsal_stack->g_att_index;
        bsal_profile_insert(bsal_get_rtk_obj(), service_id, p_func, srv_uuid);
        if (tmp_srv->characteristics != NULL)
        {
            for (x = 0; tmp_srv->characteristics[x].uuid != NULL; x++)
            {
                tmp_chr = tmp_srv->characteristics + x;
                p_bsal_stack->g_att_index ++;//declear
                p_bsal_stack->g_att_index ++;//value
                bsal_srv_create_att(bsal_get_rtk_obj(), service_id, (p_bsal_stack->g_att_index - start_handle) , tmp_chr->value_length);

            }
        }
    }
    return BSAL_RESULT_SUCCESS;
}


//FOR APP
static BSAL_STATUS bsal_srv_profile_srv_end(void)
{
    //all register service is end
    return BSAL_RESULT_SUCCESS;
}

static BSAL_STATUS bsal_stack_srv_send_data(uint16_t conn_id, uint16_t service_id, uint16_t offset_handle, uint16_t value_len, const uint8_t *value, uint8_t send_type)
{

    if (server_send_data(conn_id, service_id, offset_handle, (uint8_t *)value, value_len,  send_type))
    {
        return BSAL_RESULT_SUCCESS;
    }
    return BSAL_RESULT_FAIL;
}

static uint32_t bsal_stack_rtk_get_unique_handle(uint16_t start_id, uint16_t offset_handle)
{
    return (start_id << 16) | offset_handle;
}

static void bsal_stack_rtk_set_bond_param(bsal_bond_param_t *bond_param)
{
    uint8_t  auth_pair_mode;
    uint16_t auth_flags;
    if (bond_param->bonding_flag == false)
    {
        //do nothing
        auth_pair_mode = GAP_PAIRING_MODE_NO_PAIRING;
        //no need check the other param
        gap_set_param(GAP_PARAM_BOND_PAIRING_MODE, sizeof(auth_pair_mode), &auth_pair_mode);
        return;
    }
    else
    {
        auth_pair_mode = GAP_PAIRING_MODE_PAIRABLE;
        gap_set_param(GAP_PARAM_BOND_PAIRING_MODE, sizeof(auth_pair_mode), &auth_pair_mode);
    }

    if (bond_param->force_auth_type != 0)
    {
        //force the
        auth_flags = bond_param->force_auth_type;
    }
    else
    {
        auth_flags = bond_param->auth_type;
    }
    gap_set_param(GAP_PARAM_BOND_AUTHEN_REQUIREMENTS_FLAGS, sizeof(auth_flags), &auth_flags);
    le_bond_set_param(GAP_PARAM_BOND_SEC_REQ_ENABLE, sizeof(bond_param->sm_req_enable), &bond_param->sm_req_enable);
    if (bond_param->sm_req_enable == true)
    {
        le_bond_set_param(GAP_PARAM_BOND_SEC_REQ_REQUIREMENT, sizeof(auth_flags),
                          &auth_flags);
    }

    gap_set_param(GAP_PARAM_BOND_IO_CAPABILITIES, sizeof(bond_param->io_capability), &bond_param->io_capability);
    if (bond_param->fixed_key_flag == true)
    {
        le_bond_set_param(GAP_PARAM_BOND_FIXED_PASSKEY, sizeof(bond_param->fixed_key), &bond_param->fixed_key);
        le_bond_set_param(GAP_PARAM_BOND_FIXED_PASSKEY_ENABLE, sizeof(bond_param->fixed_key_flag),
                          &bond_param->fixed_key_flag);
    }

    if (bond_param->oob_flag == true)
    {
#if F_BT_LE_SMP_OOB_SUPPORT
        gap_set_param(GAP_PARAM_BOND_OOB_ENABLED, sizeof(bond_param->oob_flag), &bond_param->oob_flag);
#endif
    }



}

static void bsal_rtk_le_set_adv_param(bsal_adv_param_t *adv_param)
{
    if (adv_param->set_param_flag & BSAL_SET_LE_ADV_DATA)
    {
        APP_PRINT_INFO2("bsal_rtk_le_set_adv_param :%d, %x", adv_param->adv_data[0], *((uint8_t *)((uint32_t)(adv_param->adv_data) + 1)));
        le_adv_set_param(GAP_PARAM_ADV_DATA, adv_param->adv_data[0], (void *)((uint32_t)(adv_param->adv_data) + 1));
    }
    else if (adv_param->set_param_flag & BSAL_SET_LE_SCAN_RSP_DATA)
    {
        le_adv_set_param(GAP_PARAM_SCAN_RSP_DATA, adv_param->scan_rsp_data[0], (void *)((uint32_t)(adv_param->scan_rsp_data) + 1));
    }
    else if (adv_param->set_param_flag & BSAL_SET_LE_ADV_PARAM)
    {
        le_adv_set_param(GAP_PARAM_ADV_EVENT_TYPE, sizeof(adv_param->adv_type), &adv_param->adv_type);
        le_adv_set_param(GAP_PARAM_ADV_DIRECT_ADDR_TYPE, sizeof(adv_param->direct_address_type), &adv_param->direct_address_type);
        le_adv_set_param(GAP_PARAM_ADV_DIRECT_ADDR, sizeof(adv_param->direct_address), adv_param->direct_address);
        le_adv_set_param(GAP_PARAM_ADV_CHANNEL_MAP, sizeof(adv_param->channel_map), &adv_param->channel_map);
        le_adv_set_param(GAP_PARAM_ADV_FILTER_POLICY, sizeof(adv_param->filter_policy), &adv_param->filter_policy);
        le_adv_set_param(GAP_PARAM_ADV_INTERVAL_MIN, sizeof(adv_param->adv_int_min), &adv_param->adv_int_min);
        le_adv_set_param(GAP_PARAM_ADV_INTERVAL_MAX, sizeof(adv_param->adv_int_max), &adv_param->adv_int_max);
    }
}
static void bsal_rtk_set_gas_param(bsal_gaps_param_t *adv_param)
{
    uint8_t  slave_init_mtu_req = false;

    /* Advertising parameters */
    /* Set device name and device appearance */
    char device_name[20];
    uint8_t bt_addr[6];
    gap_get_param(GAP_PARAM_BD_ADDR, bt_addr);
    sprintf(device_name,"%s_%02x%02x","ART-Badge",bt_addr[1],bt_addr[0]);
    le_set_gap_param(GAP_PARAM_DEVICE_NAME, GAP_DEVICE_NAME_LEN, device_name);
    le_set_gap_param(GAP_PARAM_APPEARANCE, sizeof(adv_param->apperance), &adv_param->apperance);
    le_set_gap_param(GAP_PARAM_SLAVE_INIT_GATT_MTU_REQ, sizeof(slave_init_mtu_req),
                     &slave_init_mtu_req);

}

bsal_stack_obj_t rtk_obj =
{
    .bsal_ops = {
        .init                                                = bsal_rtk_stack_init,
        .startup                                             = bsal_rtk_stack_startup,
        .le_adv_enable                                       = bsal_rtk_le_adv_enable,
        .le_srv_reg_begin                                    = bsal_srv_profile_srv_begin,
        .le_srv_reg_fun                                      = bsal_srv_profile_reg_service,
        .le_srv_reg_end                                      = bsal_srv_profile_srv_end,
        .le_srv_send_data                                    = bsal_stack_srv_send_data,
        .cov_handle_to_u_handle                              = bsal_stack_rtk_get_unique_handle,
        .set_bond_param                                      = bsal_stack_rtk_set_bond_param,
        .le_set_adv_param                                    = bsal_rtk_le_set_adv_param,
        .set_gaps_param                                      = bsal_rtk_set_gas_param,
        //callback
    },
};


/*TODO*/
//register rtk object

//=================================realtek source orignal codeAPP TASK========

//REALTEK ENTER
#ifndef SOC_RTL_8762D
int bt_app_main(void)
{
    bt_trace_set_switch(true);  //need called befor bt_trace_init() //default close
    bt_trace_init();
    bte_init();
    bsal_bt_app_main();
    return 0;
}
#endif



/** @defgroup  PERIPH_APP Peripheral Application
    * @brief This file handles BLE peripheral application routines.
    * @{
    */
/*============================================================================*
 *                              Variables
 *============================================================================*/
/** @} */ /* End of group PERIPH_SEVER_CALLBACK */
/** @defgroup  PERIPH_GAP_MSG GAP Message Handler
    * @brief Handle GAP Message
    * @{
    */
static T_GAP_DEV_STATE gap_dev_state = {0, 0, 0, 0};                 /**< GAP device state */
static T_GAP_CONN_STATE gap_conn_state = GAP_CONN_STATE_DISCONNECTED; /**< GAP connection state */

/*============================================================================*
 *                              Functions
 *============================================================================*/
void app_handle_gap_msg(T_IO_MSG  *p_gap_msg);


/**
 * @brief    Handle msg GAP_MSG_LE_DEV_STATE_CHANGE
 * @note     All the gap device state events are pre-handled in this function.
 *           Then the event handling function shall be called according to the new_state
 * @param[in] new_state  New gap device state
 * @param[in] cause GAP device state change cause
 * @return   void
 */
void app_handle_dev_state_evt(T_GAP_DEV_STATE new_state, uint16_t cause)
{
    APP_PRINT_INFO3("app_handle_dev_state_evt: init state %d, adv state %d, cause 0x%x",
                    new_state.gap_init_state, new_state.gap_adv_state, cause);
    if (gap_dev_state.gap_init_state != new_state.gap_init_state)
    {
        if (new_state.gap_init_state == GAP_INIT_STATE_STACK_READY)
        {
            APP_PRINT_INFO0("GAP stack ready");
            /*stack ready*/
#ifdef BSAL_DEBUG
            le_adv_start();
#else
            bsal_stack_ready(bsal_get_rtk_obj(), true);
#endif
        }
    }

    if (gap_dev_state.gap_adv_state != new_state.gap_adv_state)
    {
        if (new_state.gap_adv_state == GAP_ADV_STATE_IDLE)
        {
            if (new_state.gap_adv_sub_state == GAP_ADV_TO_IDLE_CAUSE_CONN)
            {
                APP_PRINT_INFO0("GAP adv stoped: because connection created");
            }
            else
            {
                APP_PRINT_INFO0("GAP adv stoped");
            }
        }
        else if (new_state.gap_adv_state == GAP_ADV_STATE_ADVERTISING)
        {
            APP_PRINT_INFO0("GAP adv start");
        }
    }

    gap_dev_state = new_state;
}

/**
 * @brief    Handle msg GAP_MSG_LE_CONN_STATE_CHANGE
 * @note     All the gap conn state events are pre-handled in this function.
 *           Then the event handling function shall be called according to the new_state
 * @param[in] conn_id Connection ID
 * @param[in] new_state  New gap connection state
 * @param[in] disc_cause Use this cause when new_state is GAP_CONN_STATE_DISCONNECTED
 * @return   void
 */
void app_handle_conn_state_evt(uint8_t conn_id, T_GAP_CONN_STATE new_state, uint16_t disc_cause)
{
    APP_PRINT_INFO4("app_handle_conn_state_evt: conn_id %d old_state %d new_state %d, disc_cause 0x%x",
                    conn_id, gap_conn_state, new_state, disc_cause);
    switch (new_state)
    {
    case GAP_CONN_STATE_DISCONNECTED:
    {
        if ((disc_cause != (HCI_ERR | HCI_ERR_REMOTE_USER_TERMINATE))
                && (disc_cause != (HCI_ERR | HCI_ERR_LOCAL_HOST_TERMINATE)))
        {
            APP_PRINT_ERROR1("app_handle_conn_state_evt: connection lost cause 0x%x", disc_cause);
        }
#ifdef BSAL_DEBUG
        le_adv_start();
#else

#endif

    }
    break;

    case GAP_CONN_STATE_CONNECTED:
    {
        uint16_t conn_interval;
        uint16_t conn_latency;
        uint16_t conn_supervision_timeout;
        uint8_t  remote_bd[6];
        T_GAP_REMOTE_ADDR_TYPE remote_bd_type;

        le_get_conn_param(GAP_PARAM_CONN_INTERVAL, &conn_interval, conn_id);
        le_get_conn_param(GAP_PARAM_CONN_LATENCY, &conn_latency, conn_id);
        le_get_conn_param(GAP_PARAM_CONN_TIMEOUT, &conn_supervision_timeout, conn_id);
        le_get_conn_addr(conn_id, remote_bd, (void *)&remote_bd_type);
        APP_PRINT_INFO5("GAP_CONN_STATE_CONNECTED:remote_bd %s, remote_addr_type %d, conn_interval 0x%x, conn_latency 0x%x, conn_supervision_timeout 0x%x",
                        TRACE_BDADDR(remote_bd), remote_bd_type,
                        conn_interval, conn_latency, conn_supervision_timeout);
    }
    break;
    default:
        break;
    }
//TODO need to changed
    bsal_gap_connect_status(bsal_get_rtk_obj(), conn_id, new_state, disc_cause);
    gap_conn_state = new_state;
}

/**
 * @brief    Handle msg GAP_MSG_LE_AUTHEN_STATE_CHANGE
 * @note     All the gap authentication state events are pre-handled in this function.
 *           Then the event handling function shall be called according to the new_state
 * @param[in] conn_id Connection ID
 * @param[in] new_state  New authentication state
 * @param[in] cause Use this cause when new_state is GAP_AUTHEN_STATE_COMPLETE
 * @return   void
 */
void app_handle_authen_state_evt(uint8_t conn_id, uint8_t new_state, uint16_t cause)
{
    APP_PRINT_INFO2("app_handle_authen_state_evt:conn_id %d, cause 0x%x", conn_id, cause);

    switch (new_state)
    {
    case GAP_AUTHEN_STATE_STARTED:
    {
        APP_PRINT_INFO0("app_handle_authen_state_evt: GAP_AUTHEN_STATE_STARTED");
    }
    break;

    case GAP_AUTHEN_STATE_COMPLETE:
    {
        if (cause == GAP_SUCCESS)
        {
            APP_PRINT_INFO0("app_handle_authen_state_evt: GAP_AUTHEN_STATE_COMPLETE pair success");

        }
        else
        {
            APP_PRINT_INFO0("app_handle_authen_state_evt: GAP_AUTHEN_STATE_COMPLETE pair failed");
        }
    }
    break;

    default:
    {
        APP_PRINT_ERROR1("app_handle_authen_state_evt: unknown newstate %d", new_state);
    }
    break;
    }
}

/**
 * @brief    Handle msg GAP_MSG_LE_CONN_MTU_INFO
 * @note     This msg is used to inform APP that exchange mtu procedure is completed.
 * @param[in] conn_id Connection ID
 * @param[in] mtu_size  New mtu size
 * @return   void
 */
void app_handle_conn_mtu_info_evt(uint8_t conn_id, uint16_t mtu_size)
{
    APP_PRINT_INFO2("app_handle_conn_mtu_info_evt: conn_id %d, mtu_size %d", conn_id, mtu_size);
}

/**
 * @brief    Handle msg GAP_MSG_LE_CONN_PARAM_UPDATE
 * @note     All the connection parameter update change  events are pre-handled in this function.
 * @param[in] conn_id Connection ID
 * @param[in] status  New update state
 * @param[in] cause Use this cause when status is GAP_CONN_PARAM_UPDATE_STATUS_FAIL
 * @return   void
 */
void app_handle_conn_param_update_evt(uint8_t conn_id, uint8_t status, uint16_t cause)
{
    switch (status)
    {
    case GAP_CONN_PARAM_UPDATE_STATUS_SUCCESS:
    {
        uint16_t conn_interval;
        uint16_t conn_slave_latency;
        uint16_t conn_supervision_timeout;

        le_get_conn_param(GAP_PARAM_CONN_INTERVAL, &conn_interval, conn_id);
        le_get_conn_param(GAP_PARAM_CONN_LATENCY, &conn_slave_latency, conn_id);
        le_get_conn_param(GAP_PARAM_CONN_TIMEOUT, &conn_supervision_timeout, conn_id);
        APP_PRINT_INFO3("app_handle_conn_param_update_evt update success:conn_interval 0x%x, conn_slave_latency 0x%x, conn_supervision_timeout 0x%x",
                        conn_interval, conn_slave_latency, conn_supervision_timeout);
    }
    break;

    case GAP_CONN_PARAM_UPDATE_STATUS_FAIL:
    {
        APP_PRINT_ERROR1("app_handle_conn_param_update_evt update failed: cause 0x%x", cause);
    }
    break;

    case GAP_CONN_PARAM_UPDATE_STATUS_PENDING:
    {
        APP_PRINT_INFO0("app_handle_conn_param_update_evt update pending.");
    }
    break;

    default:
        break;
    }
}
/**
 * @brief    All the BT GAP MSG are pre-handled in this function.
 * @note     Then the event handling function shall be called according to the
 *           subtype of T_IO_MSG
 * @param[in] p_gap_msg Pointer to GAP msg
 * @return   void
 */
void app_handle_gap_msg(T_IO_MSG *p_gap_msg)
{
    T_LE_GAP_MSG gap_msg;
    uint8_t conn_id;
    memcpy(&gap_msg, &p_gap_msg->u.param, sizeof(p_gap_msg->u.param));

    APP_PRINT_TRACE1("app_handle_gap_msg: subtype %d", p_gap_msg->subtype);
    switch (p_gap_msg->subtype)
    {
    case GAP_MSG_LE_DEV_STATE_CHANGE:
    {
        app_handle_dev_state_evt(gap_msg.msg_data.gap_dev_state_change.new_state,
                                 gap_msg.msg_data.gap_dev_state_change.cause);
    }
    break;

    case GAP_MSG_LE_CONN_STATE_CHANGE:
    {
        app_handle_conn_state_evt(gap_msg.msg_data.gap_conn_state_change.conn_id,
                                  (T_GAP_CONN_STATE)gap_msg.msg_data.gap_conn_state_change.new_state,
                                  gap_msg.msg_data.gap_conn_state_change.disc_cause);
    }
    break;

    case GAP_MSG_LE_CONN_MTU_INFO:
    {
        app_handle_conn_mtu_info_evt(gap_msg.msg_data.gap_conn_mtu_info.conn_id,
                                     gap_msg.msg_data.gap_conn_mtu_info.mtu_size);
    }
    break;

    case GAP_MSG_LE_CONN_PARAM_UPDATE:
    {
        app_handle_conn_param_update_evt(gap_msg.msg_data.gap_conn_param_update.conn_id,
                                         gap_msg.msg_data.gap_conn_param_update.status,
                                         gap_msg.msg_data.gap_conn_param_update.cause);
    }
    break;

    case GAP_MSG_LE_AUTHEN_STATE_CHANGE:
    {
        app_handle_authen_state_evt(gap_msg.msg_data.gap_authen_state.conn_id,
                                    gap_msg.msg_data.gap_authen_state.new_state,
                                    gap_msg.msg_data.gap_authen_state.status);
    }
    break;

    case GAP_MSG_LE_BOND_JUST_WORK:
    {
        conn_id = gap_msg.msg_data.gap_bond_just_work_conf.conn_id;
        le_bond_just_work_confirm(conn_id, GAP_CFM_CAUSE_ACCEPT);
        APP_PRINT_INFO0("GAP_MSG_LE_BOND_JUST_WORK");
    }
    break;

    case GAP_MSG_LE_BOND_PASSKEY_DISPLAY:
    {
        uint32_t display_value = 0;
        conn_id = gap_msg.msg_data.gap_bond_passkey_display.conn_id;
        //le_bond_get_display_key(conn_id, &display_value);
        APP_PRINT_INFO1("GAP_MSG_LE_BOND_PASSKEY_DISPLAY:passkey %d", display_value);
        le_bond_passkey_display_confirm(conn_id, GAP_CFM_CAUSE_ACCEPT);
    }
    break;

    case GAP_MSG_LE_BOND_USER_CONFIRMATION:
    {
        uint32_t display_value = 0;
        conn_id = gap_msg.msg_data.gap_bond_user_conf.conn_id;
        le_bond_get_display_key(conn_id, &display_value);
        APP_PRINT_INFO1("GAP_MSG_LE_BOND_USER_CONFIRMATION: passkey %d", display_value);
        le_bond_user_confirm(conn_id, GAP_CFM_CAUSE_ACCEPT);
    }
    break;

    case GAP_MSG_LE_BOND_PASSKEY_INPUT:
    {
        uint32_t passkey = 888888;
        conn_id = gap_msg.msg_data.gap_bond_passkey_input.conn_id;
        APP_PRINT_INFO1("GAP_MSG_LE_BOND_PASSKEY_INPUT: conn_id %d", conn_id);
        le_bond_passkey_input_confirm(conn_id, passkey, GAP_CFM_CAUSE_ACCEPT);
    }
    break;
#if F_BT_LE_SMP_OOB_SUPPORT
    case GAP_MSG_LE_BOND_OOB_INPUT:
    {
        uint8_t oob_data[GAP_OOB_LEN] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        conn_id = gap_msg.msg_data.gap_bond_oob_input.conn_id;
        APP_PRINT_INFO0("GAP_MSG_LE_BOND_OOB_INPUT");
        le_bond_set_param(GAP_PARAM_BOND_OOB_DATA, GAP_OOB_LEN, oob_data);
        le_bond_oob_input_confirm(conn_id, GAP_CFM_CAUSE_ACCEPT);
    }
    break;
#endif
    default:
        APP_PRINT_ERROR1("app_handle_gap_msg: unknown subtype %d", p_gap_msg->subtype);
        break;
    }
}
/**
 * @brief    All the application messages are pre-handled in this function
 * @note     All the IO MSGs are sent to this function, then the event handling
 *           function shall be called according to the MSG type.
 * @param[in] io_msg  IO message data
 * @return   void
 */
static void app_handle_io_msg(T_IO_MSG io_msg)
{
    uint16_t msg_type = io_msg.type;

    switch (msg_type)
    {
    case IO_MSG_TYPE_BT_STATUS:
    {
        app_handle_gap_msg(&io_msg);
    }
    break;
    default:
        break;
    }
}
/** @defgroup  PERIPH_APP_TASK Peripheral App Task
    * @brief This file handles the implementation of application task related functions.
    *
    * Create App task and handle events & messages
    * @{
    */
/*============================================================================*
 *                              Macros
 *============================================================================*/
#define APP_TASK_PRIORITY             12         //!< Task priorities
#define APP_TASK_STACK_SIZE           256 * 4   //!<  Task stack size
#define MAX_NUMBER_OF_GAP_MESSAGE     0x20      //!<  GAP message queue size
#define MAX_NUMBER_OF_IO_MESSAGE      0x20      //!<  IO message queue size
#define MAX_NUMBER_OF_EVENT_MESSAGE   (MAX_NUMBER_OF_GAP_MESSAGE + MAX_NUMBER_OF_IO_MESSAGE)    //!< Event message queue size

/*============================================================================*
 *                              Variables
 *============================================================================*/
static void *app_task_handle;   //!< APP Task handle
static void *evt_queue_handle;  //!< Event queue handle
static void *io_queue_handle;   //!< IO queue handle

/*============================================================================*
 *                              Functions
 *============================================================================*/
void app_main_task(void *p_param);
void bsal_ble_loop(void *p_param);

/**
 * @brief  Initialize App task
 * @return void
 */
void app_task_init()
{
    os_task_create(&app_task_handle, "app", app_main_task, 0, APP_TASK_STACK_SIZE,
                   APP_TASK_PRIORITY);
//    os_task_create(&app_task_handle, "bsal_loop", bsal_ble_loop, 0, APP_TASK_STACK_SIZE,
//                   3);
}

/**
 * @brief        App task to handle events & messages
 * @param[in]    p_param    Parameters sending to the task
 * @return       void
 */
static void app_main_task(void *p_param)
{
    uint8_t event;
    os_msg_queue_create(&io_queue_handle, MAX_NUMBER_OF_IO_MESSAGE, sizeof(T_IO_MSG));
    os_msg_queue_create(&evt_queue_handle, MAX_NUMBER_OF_EVENT_MESSAGE, sizeof(uint8_t));

    gap_start_bt_stack(evt_queue_handle, io_queue_handle, MAX_NUMBER_OF_GAP_MESSAGE);

    while (true)
    {
        if (os_msg_recv(evt_queue_handle, &event, 0xFFFFFFFF) == true)
        {
            if (event == EVENT_IO_TO_APP)
            {
                T_IO_MSG io_msg;
                if (os_msg_recv(io_queue_handle, &io_msg, 0) == true)
                {
                    app_handle_io_msg(io_msg);
                }
            }
            else
            {
                gap_handle_msg(event);
            }
        }
    }
}


T_APP_RESULT  bsal_attr_read_cb(uint8_t conn_id, T_SERVER_ID service_id,
                                uint16_t attrib_index, uint16_t offset, uint16_t *p_length, uint8_t **pp_value)
{
    T_APP_RESULT cause = APP_RESULT_SUCCESS;
    bsal_callbak_data_t p_param;
    p_param.stack_ptr = bsal_get_rtk_obj();
    p_param.start_handle = service_id;

    p_param.conn_id = conn_id;
    p_param.off_handle = attrib_index;
    p_param.msg_type = BSAL_CALLBACK_TYPE_READ_CHAR_VALUE;

    //load the new data
    //frome service_id find the service index;

    bsal_srv_callback_t *p_bsal_srv = bsal_profile_get(p_param.stack_ptr, service_id);
    memcpy(&p_param.srv_uuid, &p_bsal_srv->srv_uuid, sizeof(bsal_uuid_any_t));
    p_bsal_srv->bsal_srv_fn_cb((void *)&p_param);
    //load the new data
    //database get data
    //read from db
    //

    bsal_srv_read_data(bsal_get_rtk_obj(), service_id, attrib_index, p_length,  pp_value);
    return (cause);
}



T_APP_RESULT bsal_attr_write_cb(uint8_t conn_id, T_SERVER_ID service_id,
                                uint16_t attrib_index, T_WRITE_TYPE write_type, uint16_t length, uint8_t *p_value,
                                P_FUN_WRITE_IND_POST_PROC *p_write_ind_post_proc)
{
    T_APP_RESULT  cause = APP_RESULT_SUCCESS;
    bsal_callbak_data_t p_param;
    p_param.stack_ptr = bsal_get_rtk_obj();
    p_param.conn_id = conn_id;
    p_param.start_handle = service_id;
    p_param.off_handle = attrib_index;
    p_param.msg_type = BSAL_CALLBACK_TYPE_WRITE_CHAR_VALUE;
    p_param.length = length;
    p_param.data = (uint8_t *)p_value;// send the cccd;
    bsal_srv_callback_t *p_bsal_srv = bsal_profile_get(p_param.stack_ptr, service_id);
    memcpy(&p_param.srv_uuid, &p_bsal_srv->srv_uuid, sizeof(bsal_uuid_any_t));
    p_bsal_srv->bsal_srv_fn_cb((void *)&p_param);
    return (cause);
}

void bsal_cccd_update_cb(uint8_t conn_id, T_SERVER_ID service_id, uint16_t index,
                         uint16_t cccbits)
{
    bsal_callbak_data_t p_param;
    p_param.stack_ptr = bsal_get_rtk_obj();
    p_param.conn_id = conn_id;
    p_param.off_handle = index;
    p_param.start_handle = service_id;
    p_param.msg_type = BSAL_CALLBACK_TYPE_INDIFICATION_NOTIFICATION;
    p_param.length = 2;
    p_param.value = cccbits;// send the cccd;
    bsal_srv_callback_t *p_bsal_srv = bsal_profile_get(p_param.stack_ptr, service_id);
    memcpy(&p_param.srv_uuid, &p_bsal_srv->srv_uuid, sizeof(bsal_uuid_any_t));
    p_bsal_srv->bsal_srv_fn_cb((void *)&p_param);
}

static const T_FUN_GATT_SERVICE_CBS bsal_cbs =
{
    bsal_attr_read_cb,  // Read callback function pointer
    bsal_attr_write_cb, // Write callback function pointer
    bsal_cccd_update_cb  // CCCD update callback function pointer
};


static T_APP_RESULT bsal_app_gap_callback(uint8_t cb_type, void *p_cb_data)
{
    T_APP_RESULT result = APP_RESULT_SUCCESS;
    T_LE_CB_DATA *p_data = (T_LE_CB_DATA *)p_cb_data;

    switch (cb_type)
    {
    case GAP_MSG_LE_MODIFY_WHITE_LIST:
        APP_PRINT_INFO2("GAP_MSG_LE_MODIFY_WHITE_LIST: operation %d, cause 0x%x",
                        p_data->p_le_modify_white_list_rsp->operation,
                        p_data->p_le_modify_white_list_rsp->cause);
        break;

    default:
        APP_PRINT_ERROR1("app_gap_callback: unhandled cb_type 0x%x", cb_type);
        break;
    }
    return result;
}


static T_APP_RESULT bsal_app_profile_callback(T_SERVER_ID service_id, void *p_data)
{
    T_APP_RESULT app_result = APP_RESULT_SUCCESS;
    if (service_id == SERVICE_PROFILE_GENERAL_ID)
    {
        T_SERVER_APP_CB_DATA *p_param = (T_SERVER_APP_CB_DATA *)p_data;
        switch (p_param->eventId)
        {
        case PROFILE_EVT_SRV_REG_COMPLETE:// srv register result event.
            APP_PRINT_INFO1("PROFILE_EVT_SRV_REG_COMPLETE: result %d",
                            p_param->event_data.service_reg_result);
            break;
        case PROFILE_EVT_SEND_DATA_COMPLETE:
            APP_PRINT_INFO5("PROFILE_EVT_SEND_DATA_COMPLETE: conn_id %d, cause 0x%x, service_id %d, attrib_idx 0x%x, credits %d",
                            p_param->event_data.send_data_result.conn_id,
                            p_param->event_data.send_data_result.cause,
                            p_param->event_data.send_data_result.service_id,
                            p_param->event_data.send_data_result.attrib_idx,
                            p_param->event_data.send_data_result.credits);
            if (p_param->event_data.send_data_result.cause == GAP_SUCCESS)
            {
                APP_PRINT_INFO0("PROFILE_EVT_SEND_DATA_COMPLETE success");
            }
            else
            {
                APP_PRINT_ERROR0("PROFILE_EVT_SEND_DATA_COMPLETE failed");
            }
            break;

        default:
            break;
        }
    }
    return app_result;
}



