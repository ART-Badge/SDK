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
#ifdef PKG_BSAL_NIMBLE_STACK
#include "bsal_nimble.h"
bsal_stack_obj_t nimble_obj;
#endif

static void *bsal_get_local_stack_obj()
{
    return &nimble_obj;
}
#ifdef PKG_BSAL_NIMBLE_STACK
static int
bsal_nimble_gap_event(struct ble_gap_event *event, void *arg)
{
    struct ble_gap_conn_desc desc;
    int rc;
    uint16_t cccbits = 0;
    switch (event->type)
    {
    case BLE_GAP_EVENT_CONNECT:
        if (event->connect.status == 0)
        {
            rc = ble_gap_conn_find(event->connect.conn_handle, &desc);
            //save the connect_handle;
            //BSAL CALLBACK
            bsal_gap_connect_status(arg, event->connect.conn_handle, BSAL_GAP_CONN_STATE_CONNECTED, 0);
        }
        break;
    case BLE_GAP_EVENT_DISCONNECT:
        //BSAL CALLBACK
        bsal_gap_connect_status(arg, event->connect.conn_handle, BSAL_GAP_CONN_STATE_DISCONNECTED, 0);
        break;
    case BLE_GAP_EVENT_SUBSCRIBE:
        /* TODO NOTIFY ENABLE DISABLE*/
        MODLOG_DFLT(INFO, "subscribe event; conn_handle=%d attr_handle=%d "
                    "reason=%d prevn=%d curn=%d previ=%d curi=%d\n",
                    event->subscribe.conn_handle,
                    event->subscribe.attr_handle,
                    event->subscribe.reason,
                    event->subscribe.prev_notify,
                    event->subscribe.cur_notify,
                    event->subscribe.prev_indicate,
                    event->subscribe.cur_indicate);

        if (event->subscribe.cur_notify == 1)
        {
            cccbits |= BSAL_GATT_CCC_NOTIFY;
        }

        if (event->subscribe.cur_indicate == 1)
        {
            cccbits |= BSAL_GATT_CCC_INDICATE;
        }
        //BSAL CALLBACK
        bsal_gatt_cccd_callback_event_by_handle(bsal_get_local_stack_obj(), event->subscribe.conn_handle, event->subscribe.attr_handle + 1,  cccbits);
        break;
    default:
        break;
    }
    return 0;
}
//===============================================STACK ABOUT CODE=================================


static void
basl_nimble_on_reset(int reason)
{
    MODLOG_DFLT(ERROR, "Resetting state; reason=%d\n", reason);
    //BSAL CALLBACK
    bsal_stack_ready(bsal_get_local_stack_obj(), false);
    while (1);
}

static void
bsal_nimble_on_sync(void)
{
    int rc;
    //BSAL CALLBACK
    bsal_stack_ready(bsal_get_local_stack_obj(), true);
}

static void
bsal_gatt_svr_register_cb(struct ble_gatt_register_ctxt *ctxt, void *arg)
{
    char buf[BLE_UUID_STR_LEN];

    switch (ctxt->op)
    {
    case BLE_GATT_REGISTER_OP_SVC:
        MODLOG_DFLT(INFO, "registered service %s with handle=%d\n",
                    ble_uuid_to_str(ctxt->svc.svc_def->uuid, buf),
                    ctxt->svc.handle);
        break;
    case BLE_GATT_REGISTER_OP_CHR:
        MODLOG_DFLT(INFO, "registering characteristic %s with "
                    "def_handle=%d val_handle=%d\n",
                    ble_uuid_to_str(ctxt->chr.chr_def->uuid, buf),
                    ctxt->chr.def_handle,
                    ctxt->chr.val_handle);
        //load the last srv handle
        bsal_srv_create_att_by_handle(bsal_get_local_stack_obj(), ctxt->chr.val_handle, ctxt->chr.chr_def->min_key_size);
        break;
    case BLE_GATT_REGISTER_OP_DSC:

        /* TODO no need templete*/
        MODLOG_DFLT(INFO, "registering descriptor %s with handle=%d\n",
                    ble_uuid_to_str(ctxt->dsc.dsc_def->uuid, buf),
                    ctxt->dsc.handle);
        break;
    default:
        assert(0);
        break;
    }
}



int
bsal_ble_svc_bas_access(uint16_t conn_handle, uint16_t attr_handle,
                        struct ble_gatt_access_ctxt *ctxt,
                        void *arg)
{
    int rc;

    uint16_t p_length;
    uint8_t *pp_value;

//     *     o  BLE_GATT_ACCESS_OP_READ_CHR
//     *     o  BLE_GATT_ACCESS_OP_WRITE_CHR
//     *     o  BLE_GATT_ACCESS_OP_READ_DSC
//     *     o  BLE_GATT_ACCESS_OP_WRITE_DSC

    switch (ctxt->op)
    {
    case BLE_GATT_ACCESS_OP_READ_CHR:
    case BLE_GATT_ACCESS_OP_READ_DSC:
        bsal_gatt_read_callback_event_by_handle(bsal_get_local_stack_obj(), conn_handle, *ctxt->chr->val_handle, &p_length,  &pp_value);
        rc = os_mbuf_append(ctxt->om, (const void *) pp_value, p_length);
        return rc == 0 ? 0 : BLE_ATT_ERR_INSUFFICIENT_RES;
        break;
    case BLE_GATT_ACCESS_OP_WRITE_CHR:
    case BLE_GATT_ACCESS_OP_WRITE_DSC:
        bsal_gatt_write_callback_event_by_handle(bsal_get_local_stack_obj(), conn_handle, *ctxt->chr->val_handle, ctxt->om->om_len,  ctxt->om->om_data);
        break;
    default:
        break;
    }
}
#endif
//=================================================END============================================

//================================BSAL.C==========================
static void bsal_stack_int_init(uint8_t stack_type)
{
#ifdef PKG_BSAL_NIMBLE_STACK
    ble_hs_cfg.reset_cb = basl_nimble_on_reset; //fail callback
    ble_hs_cfg.sync_cb = bsal_nimble_on_sync;
    ble_hs_cfg.gatts_register_cb = bsal_gatt_svr_register_cb;
    ble_hs_cfg.gatts_register_arg = bsal_get_local_stack_obj();
    ble_hs_cfg.store_status_cb = ble_store_util_status_rr;
#endif
}

static void bsal_stack_int_startup(void)
{
#ifdef PKG_BSAL_NIMBLE_STACK
    ble_hs_thread_startup();
#endif
}


static BSAL_STATUS bsal_stack_int_le_adv_enable(bool enable)
{
    bsal_stack_obj_t *p_bsal_stack =  bsal_get_local_stack_obj();

    if (enable == true)
    {
        /* ENABLE ADV */
#ifdef PKG_BSAL_NIMBLE_STACK
        struct ble_gap_adv_params adv_param;
        memset(&adv_param, 0, sizeof(struct ble_gap_adv_params));

        if (p_bsal_stack->bsal_info.adv_param.adv_type == BSAL_GAP_ADTYPE_ADV_IND)
        {
            adv_param.conn_mode = BLE_GAP_CONN_MODE_UND;
            adv_param.disc_mode = BLE_GAP_DISC_MODE_GEN;
        }

        adv_param.channel_map = p_bsal_stack->bsal_info.adv_param.channel_map;
        adv_param.filter_policy = p_bsal_stack->bsal_info.adv_param.filter_policy;
        adv_param.itvl_max = p_bsal_stack->bsal_info.adv_param.adv_int_max;
        adv_param.itvl_min = p_bsal_stack->bsal_info.adv_param.adv_int_min;
        ble_addr_t direct_address;
        direct_address.type = p_bsal_stack->bsal_info.adv_param.direct_address_type;
        memcpy(direct_address.val, p_bsal_stack->bsal_info.adv_param.direct_address, 6);
        ble_gap_adv_start(p_bsal_stack->bsal_info.adv_param.own_address_type, &direct_address , BLE_HS_FOREVER,
                          &adv_param, bsal_nimble_gap_event, p_bsal_stack);
#endif
    }
    else
    {
        /* DISABLE ADV */
#ifdef PKG_BSAL_NIMBLE_STACK
        ble_gap_adv_stop();
#endif
    }
    return BSAL_RESULT_SUCCESS;
}

//for app
static BSAL_STATUS bsal_int_srv_profile_srv_begin(bool is_continue, uint8_t service_num, void *p_fnc_callback)
{
    return BSAL_RESULT_SUCCESS;
}

//common utils
static BSAL_STATUS bsal_int_srv_profile_reg_service(
    struct bsal_gatt_app_srv_def *p_srv,
    void *p_func  //callback
)
{
#ifdef PKG_BSAL_NIMBLE_STACK
    uint16_t att_num = 0;
    uint8_t att_index = 0;
    uint16_t sum_of_descript = 0;
    uint8_t write_index = 0;
    uint8_t i = 0;
    uint8_t j = 0;
    uint8_t x = 0;
    uint8_t y = 0;
    bsal_gatt_res_t res;
    bsal_util_sum_handle_num(p_srv, &res);
    att_num  = res.attrs;
    bsal_stack_obj_t *p_bsal_stack =  bsal_get_local_stack_obj();
    //malloc memory:
    struct ble_gatt_svc_def *nimble_srvs = bsal_osif_malloc(sizeof(struct ble_gatt_svc_def) * (res.svcs + 1));
    struct bsal_gatt_app_srv_def *tmp_srv;
    bsal_gatt_chr_def_t      *tmp_chr;
    uint8_t srv_idx = 0;
    uint8_t chr_idx = 0;
    uint8_t dsc_idx = 0;

    //combine the srv
    for (i = 0; p_srv[i].type != 0; i++)
    {
        tmp_srv = p_srv + i;

        //add service============================================
        if ((tmp_srv->type != BSAL_GATT_UUID_PRIMARY_SERVICE) &&
                (tmp_srv->type != BSAL_GATT_UUID_SECONDARY_SERVICE))
        {
            return BSAL_RESULT_FAIL;
        }
        nimble_srvs[srv_idx].type = BLE_GATT_SVC_TYPE_PRIMARY;
        write_index++;
        if (tmp_srv->uuid->u_type  == BSAL_UUID_TYPE_16BIT)
        {
            ble_uuid16_t *tmp_uuid = bsal_osif_malloc(sizeof(ble_uuid16_t));
            tmp_uuid->u.type = BLE_UUID_TYPE_16;
            tmp_uuid->value = tmp_srv->uuid->u16.value;
            nimble_srvs->uuid = &tmp_uuid->u;
        }
        else if (tmp_srv->characteristics[x].uuid->u_type == BSAL_UUID_TYPE_128BIT)
        {

        }
        write_index++;
        //add include=============================================================
        if (tmp_srv->includes != NULL)
        {
            for (j = 0; tmp_srv->includes[j] != NULL; j++)
            {
//               att_res->attrs++;
//               att_res->incs++;
                //add include /*TODO*/
                write_index++;
            }
        }
        //add char=========================================================
        if (tmp_srv->characteristics != NULL)
        {
            for (x = 0; tmp_srv->characteristics[x].uuid != NULL; x++)
            {
                //figure the chr_num
            }
            write_index++;
            //write_index++;
            struct ble_gatt_chr_def *nimble_chrs = bsal_osif_malloc(sizeof(struct ble_gatt_chr_def) * (x + 1));
            nimble_srvs[srv_idx].characteristics = nimble_chrs;
            for (x = 0; tmp_srv->characteristics[x].uuid != NULL; x++)
            {
                tmp_chr = tmp_srv->characteristics + x;

                if (tmp_chr->uuid->u_type == BSAL_UUID_TYPE_16BIT)
                {
                    ble_uuid16_t *tmp_uuid = bsal_osif_malloc(sizeof(ble_uuid16_t));
                    tmp_uuid->u.type = BLE_UUID_TYPE_16;
                    tmp_uuid->value = tmp_srv->characteristics[x].uuid->u16.value;

                    nimble_chrs[x].uuid = &tmp_uuid->u;
                }
                else if (tmp_srv->characteristics[x].uuid->u_type == BSAL_UUID_TYPE_128BIT)
                {

                }
                nimble_chrs[x].access_cb = bsal_ble_svc_bas_access;
                nimble_chrs[x].arg = bsal_get_local_stack_obj();
                nimble_chrs[x].flags = tmp_srv->characteristics[x].properties;
                nimble_chrs[x].min_key_size = tmp_srv->characteristics[x].value_length;
                nimble_chrs[x].val_handle = bsal_osif_malloc(tmp_srv->characteristics[x].value_length);
                //read

                if (BSAL_GATT_PERM_GET_READ(tmp_srv->characteristics[x].permission) !=  0) ///ENCRYTYED
                {
                    /* TODO*/
                    if (BSAL_GATT_GET_ENC_TYPE_AUTHEN_REQ(BSAL_GATT_PERM_GET_READ(tmp_srv->characteristics[x].permission)))
                    {
                        nimble_chrs[x].flags |= BLE_GATT_CHR_F_READ_AUTHEN;
                    }
                    else if (BSAL_GATT_GET_ENC_TYPE_AUTHOR_REQ(BSAL_GATT_PERM_GET_READ(tmp_srv->characteristics[x].permission)))
                    {
                        nimble_chrs[x].flags |= BLE_GATT_CHR_F_READ_AUTHOR;
                    }
                    else
                    {
                        nimble_chrs[x].flags |= BLE_GATT_CHR_F_READ;
                    }
                }
                //write
                if (BSAL_GATT_PERM_GET_WRITE(tmp_srv->characteristics[x].permission) !=  0) ///ENCRYTYED
                {
                    /* TODO*/
                    if (BSAL_GATT_GET_ENC_TYPE_AUTHEN_REQ(BSAL_GATT_PERM_GET_WRITE(tmp_srv->characteristics[x].permission)))
                    {
                        nimble_chrs[x].flags |= BLE_GATT_CHR_F_WRITE_AUTHEN;
                    }
                    else if (BSAL_GATT_GET_ENC_TYPE_AUTHOR_REQ(BSAL_GATT_PERM_GET_WRITE(tmp_srv->characteristics[x].permission)))
                    {
                        nimble_chrs[x].flags |= BLE_GATT_CHR_F_WRITE_AUTHOR;
                    }
                    else
                    {
                        nimble_chrs[x].flags |= BLE_GATT_CHR_F_WRITE;
                    }
                }
                //no need add NOTIFY STACK WILL ADD
                write_index++;
            }
        }
    }

    for (i = 0; p_srv[i].type != 0; i++)
    {
        bsal_uuid_any_t srv_uuid;
        srv_uuid.u_type = BSAL_UUID_TYPE_16BIT; //16bit
        srv_uuid.u16.value = p_srv[i].uuid->u16.value;
        tmp_srv = p_srv + i;
        p_bsal_stack->g_att_index++;
        bsal_profile_insert(bsal_get_local_stack_obj(), p_bsal_stack->g_att_index, p_func, srv_uuid);
        if (tmp_srv->characteristics != NULL)
        {
            for (x = 0; tmp_srv->characteristics[x].uuid != NULL; x++)
            {
                //ONLY READ NEED ADD THIS ATT
                tmp_chr = tmp_srv->characteristics + x;
                p_bsal_stack->g_att_index++;
                p_bsal_stack->g_att_index++;
                bsal_att_create_entry(bsal_get_local_stack_obj(), p_bsal_stack->g_att_index, tmp_chr->value_length);

                if ((tmp_srv->characteristics[x].properties & BSAL_ATT_P_NOTIFY) || (tmp_srv->characteristics[x].properties & BSAL_ATT_P_INDICATE))
                {
                    p_bsal_stack->g_att_index ++;
                }
            }
        }
    }

    int rc;
    /* Ensure this function only gets called by sysinit. */
    SYSINIT_ASSERT_ACTIVE();

    rc = ble_gatts_count_cfg(nimble_srvs);
    SYSINIT_PANIC_ASSERT(rc == 0);

    rc = ble_gatts_add_svcs(nimble_srvs);
    SYSINIT_PANIC_ASSERT(rc == 0);
#endif
    return BSAL_RESULT_SUCCESS;
}

//FOR APP
static BSAL_STATUS bsal_int_srv_profile_srv_end(void)
{
    //all register service is end
    //NIMBLE NO NEED
    return BSAL_RESULT_SUCCESS;
}
static uint32_t bsal_stack_get_unique_handle(uint16_t start_handle, uint16_t offset_handle)
{
    return (start_handle + offset_handle);
}
static BSAL_STATUS bsal_int_stack_srv_send_data(uint16_t conn_id, uint16_t service_id, uint16_t offset_handle, uint16_t value_len, const uint8_t *value, uint8_t send_type)
{
#ifdef PKG_BSAL_NIMBLE_STACK
    int rc;
    struct os_mbuf *om;
    /*TODO*/
    om = ble_hs_mbuf_from_flat(value, value_len);
    if (send_type != BSAL_SEND_DATA_TYPE_INDICATION)
    {
        rc = ble_gattc_notify_custom(conn_id, bsal_stack_get_unique_handle(service_id, offset_handle), om);
    }
    else
    {
        rc = ble_gattc_indicate_custom(conn_id, bsal_stack_get_unique_handle(service_id, offset_handle), om);
    }
#endif

    return BSAL_RESULT_SUCCESS;
}

static void bsal_stack_set_bond_param(bsal_bond_param_t *bond_param)
{
#ifdef PKG_BSAL_NIMBLE_STACK
    /* TODO*/

    if (bond_param->bonding_flag == false)
    {
        ble_hs_cfg.sm_bonding = 0;
        return;
    }
    else
    {
        ble_hs_cfg.sm_bonding = 1;
    }
    //set iocapability
    ble_hs_cfg.sm_io_cap = bond_param->io_capability;
    //set mitm
    if (bond_param->io_capability != BSAL_GAP_IO_CAP_NO_INPUT_NO_OUTPUT)
    {
        //maybe have mitm
        if (bond_param->auth_type & BSAL_GAP_AUTHEN_BIT_MITM_FLAG)
        {
            ble_hs_cfg.sm_mitm = 1;
        }
        if (bond_param->auth_type & BSAL_GAP_AUTHEN_BIT_SC_FLAG)
        {
            ble_hs_cfg.sm_sc = 1;
        }
        if (bond_param->auth_type & BSAL_GAP_AUTHEN_BIT_KEYPRESS_FLAG)
        {
            ble_hs_cfg.sm_keypress = 1;
        }
    }

    if (bond_param->oob_flag == true)
    {
        ble_hs_cfg.sm_oob_data_flag = 1;
    }
#endif
}

static void bsal_stack_le_set_adv_param(bsal_adv_param_t *adv_param)
{
    if (adv_param->set_param_flag & BSAL_SET_LE_ADV_PARAM)
    {
        //save to  local adv_param
        bsal_stack_obj_t *p_bsal_stack =  bsal_get_local_stack_obj();
        //save the adv param
        memcpy(&p_bsal_stack->bsal_info.adv_param, adv_param, sizeof(bsal_adv_param_t));
    }
    else if (adv_param->set_param_flag & BSAL_SET_LE_ADV_DATA)
    {
#ifdef PKG_BSAL_NIMBLE_STACK
        ble_gap_adv_set_data((void *)((uint32_t)(adv_param->adv_data) + 1), adv_param->adv_data[0]);
#endif
    }
    else if (adv_param->set_param_flag & BSAL_SET_LE_SCAN_RSP_DATA)
    {
#ifdef PKG_BSAL_NIMBLE_STACK
        ble_gap_adv_rsp_set_data((void *)((uint32_t)(adv_param->scan_rsp_data) + 1), adv_param->scan_rsp_data[0]);
#endif
    }
}
static void bsal_stack_set_gas_param(bsal_gaps_param_t *adv_param)
{
    /* TODO*/
    uint8_t  slave_init_mtu_req = false;
}

bsal_stack_obj_t nimble_obj =
{
    .bsal_ops = {
        .init                     = bsal_stack_int_init ,
        .startup                  = bsal_stack_int_startup,
        .le_adv_enable            = bsal_stack_int_le_adv_enable,
        .le_srv_reg_begin         = bsal_int_srv_profile_srv_begin,
        .le_srv_reg_fun           = bsal_int_srv_profile_reg_service,
        .le_srv_reg_end           = bsal_int_srv_profile_srv_end,
        .le_srv_send_data         = bsal_int_stack_srv_send_data,
        .cov_handle_to_u_handle   = bsal_stack_get_unique_handle,
        .set_bond_param           = bsal_stack_set_bond_param,
        .le_set_adv_param         = bsal_stack_le_set_adv_param,
        .set_gaps_param           = bsal_stack_set_gas_param,
        //callback
    },
};



