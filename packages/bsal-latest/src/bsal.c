/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-05-28     Supperthomas the first version
 */


#include <stdio.h>
#include <string.h>
#include "bsal.h"
#include "bsal_osif.h"
#include "bsal_int.h"

//======================================BSAL UTILS======DEPEND NOTHING======================================================

void bsal_gatt_read_callback_event(void *stack_ptr, uint16_t conn_handle, uint16_t start_handle, uint16_t offset_handle, uint16_t *length , uint8_t **value)
{
    bsal_callbak_data_t p_param;
    p_param.stack_ptr = stack_ptr;
    p_param.conn_id = conn_handle;
    p_param.start_handle = start_handle;
    p_param.off_handle   = offset_handle;
    p_param.msg_type     = BSAL_CALLBACK_TYPE_READ_CHAR_VALUE;

    bsal_srv_callback_t *p_bsal_srv = bsal_profile_get(p_param.stack_ptr, p_param.start_handle);
    p_bsal_srv->bsal_srv_fn_cb((void *)&p_param);
    bsal_srv_read_data(p_param.stack_ptr, p_param.start_handle, p_param.off_handle, length, value);
}

void bsal_gatt_write_callback_event(void *stack_ptr, uint16_t conn_handle, uint16_t start_handle, uint16_t offset_handle, uint16_t length , uint8_t *value)
{
    bsal_callbak_data_t p_param;
    p_param.stack_ptr = stack_ptr;
    p_param.conn_id = conn_handle;
    p_param.start_handle = start_handle;
    p_param.off_handle   = offset_handle;
    p_param.msg_type     = BSAL_CALLBACK_TYPE_WRITE_CHAR_VALUE;
    p_param.data = value;
    p_param.length = length;
    bsal_srv_callback_t *p_bsal_srv = bsal_profile_get(p_param.stack_ptr, p_param.start_handle);
    memcpy(&p_param.srv_uuid, &p_bsal_srv->srv_uuid, sizeof(bsal_uuid_any_t));
    p_bsal_srv->bsal_srv_fn_cb((void *)&p_param);

}

void bsal_gatt_cccd_callback_event(void *stack_ptr, uint16_t conn_handle, uint16_t start_handle, uint16_t offset_handle, uint16_t value)
{
    bsal_callbak_data_t p_param;
    p_param.stack_ptr = stack_ptr;
    p_param.conn_id = conn_handle;
    p_param.start_handle = start_handle;
    p_param.off_handle   = offset_handle;
    p_param.msg_type     = BSAL_CALLBACK_TYPE_INDIFICATION_NOTIFICATION;
    p_param.length = 2;
    p_param.value = value;
    bsal_srv_callback_t *p_bsal_srv = bsal_profile_get(p_param.stack_ptr, p_param.start_handle);
    memcpy(&p_param.srv_uuid, &p_bsal_srv->srv_uuid, sizeof(bsal_uuid_any_t));
    p_bsal_srv->bsal_srv_fn_cb((void *)&p_param);

}


void bsal_gatt_read_callback_event_by_handle(void *stack_ptr, uint16_t conn_handle, uint16_t u_handle, uint16_t *length , uint8_t **value)
{
    uint16_t start_handle = bsal_profile_get_start_handle(stack_ptr, u_handle);
    uint16_t offset_handle = u_handle - start_handle;
    bsal_gatt_read_callback_event(stack_ptr, conn_handle, start_handle, offset_handle, length, value);
}

void bsal_gatt_write_callback_event_by_handle(void *stack_ptr, uint16_t conn_handle, uint16_t u_handle, uint16_t length , uint8_t *value)
{
    uint16_t start_handle = bsal_profile_get_start_handle(stack_ptr, u_handle);
    uint16_t offset_handle = u_handle - start_handle;
    bsal_gatt_write_callback_event(stack_ptr, conn_handle, start_handle, offset_handle, length, value);
}

void bsal_gatt_cccd_callback_event_by_handle(void *stack_ptr, uint16_t conn_handle, uint16_t u_handle , uint16_t value)
{
    uint16_t start_handle = bsal_profile_get_start_handle(stack_ptr, u_handle);
    uint16_t offset_handle = u_handle - start_handle;
    bsal_gatt_cccd_callback_event(stack_ptr, conn_handle, start_handle, offset_handle, value);
}



void bsal_le_adv_data_add_ltv(uint8_t *adv_data, le_adv_ltv_t *ltv)
{
    BSAL_ASSERT_PTR(data);
    BSAL_ASSERT_PTR(ltv);
    uint8_t index = 0;
    index = adv_data[0] + 1;
    adv_data[index++] = ltv->length;
    adv_data[index++] = ltv->type;
    memcpy(&adv_data[index], ltv->value, ltv->length - 1);
    index += ltv->length - 1;
    adv_data[0] = index - 1;
}

void bsal_le_adv_data_add_flag(uint8_t *adv_data , uint8_t flag)
{
    BSAL_ASSERT_PTR(ptr);
    le_adv_ltv_t ltv;
    ltv.length = 2;
    uint8_t value = flag;
    ltv.value = (uint8_t *)&value;
    ltv.type = BSAL_GAP_TYPE_FLAGS;
    bsal_le_adv_data_add_ltv(adv_data, &ltv);
}


void bsal_adv_data_add_name(uint8_t *adv_data, uint8_t name_length, char *name)
{
    BSAL_ASSERT_PTR(adv_data);
    le_adv_ltv_t ltv;
    ltv.length = name_length + 1;
    ltv.value = (uint8_t *)name;
    ltv.type = BSAL_GAP_TYPE_COMPLETE_LOCAL_NAME;
    bsal_le_adv_data_add_ltv(adv_data, &ltv);
}


void bsal_adv_data_add_uuid16(uint8_t *adv_data, uint16_t uuid)
{
    BSAL_ASSERT_PTR(adv_data);
    le_adv_ltv_t ltv;
    ltv.length = 3;
    ltv.type = BSAL_GAP_TYPE_COMPLETE_LIST_OF_16_BIT_SERVICE_CLASS_UUIDS;
    uint16_t tmp_uuid = uuid;
    ltv.value = (uint8_t *)&tmp_uuid;
    bsal_le_adv_data_add_ltv(adv_data, &ltv);
}

static bsal_gatt_value_entry_t *bsal_read_att(bsal_gatt_value_entry_t *p_db, uint32_t u_handle)
{
    BSAL_ASSERT_PTR(p_db);
    bsal_gatt_value_entry_t *tmp_db = p_db;
    while (tmp_db != NULL)
    {
        if (tmp_db->u_handle == u_handle)
        {
            return tmp_db;
        }
        else
        {
            tmp_db = tmp_db->next_entry;
        }
    }
    bsal_osif_printf_err("could not find the att:%d", u_handle);
    return NULL;
}

static int bsal_insert_att(void *stack_ptr, uint32_t u_handle, uint16_t value_length)
{
    BSAL_ASSERT_PTR(stack_ptr);
    struct bsal_stack_obj *p_ops = (struct bsal_stack_obj *)stack_ptr;
    bsal_gatt_value_entry_t *p_db = p_ops->db;
    if (p_db == NULL)
    {
        //first INIT
        bsal_gatt_value_entry_t *temp_db = bsal_osif_malloc(sizeof(bsal_gatt_value_entry_t));
        temp_db->next_entry = NULL;
        temp_db->u_handle = u_handle;
        temp_db->value = bsal_osif_malloc(value_length);
        temp_db->alloc_size = value_length;
        if (temp_db->value != NULL)
        {
            //first time
            p_ops->db = temp_db;
            return 0;
        }
        bsal_osif_printf_err("temp_db->value malloc failed1 value_length:%x", value_length);
        return 1;
    }
    bsal_gatt_value_entry_t *last_db = p_db;
    //find the end
    while (p_db != NULL)
    {
        last_db = p_db;
        p_db = p_db->next_entry;
    }

    if (p_db == NULL)
    {
        // the last one
        bsal_gatt_value_entry_t *temp_db = bsal_osif_malloc(sizeof(bsal_gatt_value_entry_t));
        last_db->next_entry = temp_db;
        temp_db->next_entry = NULL;
        temp_db->u_handle = u_handle;
        temp_db->value = bsal_osif_malloc(value_length);
        temp_db->alloc_size = value_length;
        if (temp_db->value != NULL)
        {
            return 0;
        }
        bsal_osif_printf_err("temp_db->value %x malloc value_length:%x", u_handle, value_length);
        return 0;
    }
    return 2;
}

int bsal_att_create_entry(void *stack_ptr, uint32_t u_handle, uint16_t value_length)
{
    //SAVE THE DATA
    BSAL_ASSERT_PTR(stack_ptr);
    BSAL_ASSERT_PTR(value_length);
    BSAL_ASSERT_PTR(u_handle);
    return bsal_insert_att(stack_ptr, u_handle, value_length);//just use the length malloc data
}

int bsal_att_read_entry(void *stack_ptr, uint32_t u_handle, uint16_t *value_length, uint8_t **value)
{
    //READ THE DATE
    BSAL_ASSERT_PTR(stack_ptr);
    BSAL_ASSERT_PTR(value_length);
    BSAL_ASSERT_PTR(value);
    struct bsal_stack_obj *p_ops = (struct bsal_stack_obj *)stack_ptr;
    bsal_gatt_value_entry_t *p_db = p_ops->db;
    bsal_gatt_value_entry_t *p_value = bsal_read_att(p_db, u_handle);//just use the length malloc data
    *value = p_value->value;
    *value_length = p_value->value_length;
    return 0;
}

int bsal_att_write_entry(void *stack_ptr, uint32_t u_handle, uint16_t value_length, uint8_t *value)
{
    //WRITE THE DATA
    BSAL_ASSERT_PTR(stack_ptr);
    BSAL_ASSERT_PTR(value_length);
    BSAL_ASSERT_PTR(value);
    struct bsal_stack_obj *p_ops = (struct bsal_stack_obj *)stack_ptr;
    bsal_gatt_value_entry_t *p_db = p_ops->db;
    bsal_gatt_value_entry_t *p_value = bsal_read_att(p_db, u_handle);//just use the length malloc data
    if (value_length <= p_value->alloc_size)
    {
        memcpy(p_value->value, value, value_length);
    }
    else
    {
        //size out of reset value ,fail
        //FAILED
        bsal_osif_printf_err("bsal_att_write_entry: FAIL WRITE THE DATA THE LENGTH IS %dLESS THAN ALLOCATED %d", value_length, p_value->alloc_size);
        return 1;
    }
    p_value->value_length = value_length;
    return 0;
}

int bsal_att_resize_entry(void *stack_ptr, uint32_t u_handle, uint16_t value_length, bool append)
{
    //RESIZE THE DATA
    BSAL_ASSERT_PTR(stack_ptr);
    BSAL_ASSERT_PTR(value_length);
    struct bsal_stack_obj *p_ops = (struct bsal_stack_obj *)stack_ptr;
    bsal_gatt_value_entry_t *p_db = p_ops->db;
    bsal_gatt_value_entry_t *p_value = bsal_read_att(p_db, u_handle);//just use the length malloc data
    bsal_osif_free(p_value->value);
    p_value->value = bsal_osif_malloc(value_length);
    p_value->append = append;
    return 0;
}

void bsal_util_sum_handle_num(bsal_gatt_app_srv_def_t *p_srv, bsal_gatt_res_t *att_res)
{
//      const struct bsal_gatt_app_srv_def *svc;
//      const struct bsal_gatt_app_chr_def *chr;
    BSAL_ASSERT_PTR(p_srv);
    BSAL_ASSERT_PTR(att_res);
    bsal_gatt_app_srv_def_t *tmp_srv;
    bsal_gatt_chr_def_t      *tmp_chr;
    uint8_t i = 0;
    uint8_t j = 0;
    uint8_t x = 0;
    uint8_t y = 0;
    memset(att_res, 0, sizeof(bsal_gatt_res_t));
    for (i = 0; p_srv[i].type != 0; i++)
    {
        //service
        tmp_srv = p_srv + i;
        if ((tmp_srv->type != BSAL_GATT_UUID_PRIMARY_SERVICE) &&
                (tmp_srv->type != BSAL_GATT_UUID_SECONDARY_SERVICE))
        {
            bsal_osif_printf_err("wrong tmp_srv->type:%x", tmp_srv->type);
            return;
        }
        att_res->attrs++;
        att_res->svcs++;
        if (tmp_srv->includes != NULL)
        {
            for (j = 0; tmp_srv->includes[j] != NULL; j++)
            {
                att_res->attrs++;
                att_res->incs++;
            }
        }

        if (tmp_srv->characteristics != NULL)
        {

            for (x = 0; tmp_srv->characteristics[x].uuid != NULL; x++)
            {
                tmp_chr = tmp_srv->characteristics + x;
                att_res->chrs++;
                att_res->attrs += 2;
                if ((tmp_chr->properties & BSAL_ATT_P_NOTIFY) ||
                        (tmp_chr->properties & BSAL_ATT_P_INDICATE))
                {
                    att_res->dscs++;
                    att_res->cccds++;
                    att_res->attrs++;
                }

                if (tmp_chr->descriptors != NULL)
                {
                    for (y = 0; tmp_chr->descriptors[y].uuid != NULL; y++)
                    {
                        att_res->dscs++;
                        att_res->attrs++;
                    }
                }
            }
        }
    }
}

uint16_t bsal_profile_get_start_handle(void *stack_ptr, uint16_t value_handle)
{
    BSAL_ASSERT_PTR(stack_ptr);
    BSAL_ASSERT_PTR(start_handle);
    uint8_t i = 0;
    static uint16_t last_handle;

    bsal_stack_obj_t *p_bsal_stack = stack_ptr;
    last_handle = p_bsal_stack->bsal_srv_objs[0].start_handle;

    for (i = 0; i < p_bsal_stack->srv_num; i++)
    {
        if (value_handle < p_bsal_stack->bsal_srv_objs[i].start_handle) //find the next value handle
        {
            if (value_handle > last_handle)
            {
                return last_handle;
            }
        }
        else
        {
            last_handle = p_bsal_stack->bsal_srv_objs[i].start_handle;
        }
    }
}



uint16_t bsal_srv_get_start_handle(void *stack_ptr, bsal_uuid_any_t uuid)
{
    BSAL_ASSERT_PTR(stack_ptr);
    if (uuid.u_type == 16)
    {
        return bsal_profile_get_start_handle_by_16_uuid(stack_ptr, uuid.u16.value);
    }
    else
    {
        /* TODO deal with the 128 bit*/
    }
    return 0;
}

BSAL_STATUS bsal_profile_insert(void *stack_ptr, uint16_t handle, void *callback, bsal_uuid_any_t uuid)
{
    BSAL_ASSERT_PTR(stack_ptr);
    uint8_t i = 0;
    bsal_stack_obj_t *p_bsal_stack = stack_ptr;

    if (handle == 0xffff)
    {
        //add the end
        i = p_bsal_stack->srv_num - 1;
        p_bsal_stack->bsal_srv_objs[i].bsal_srv_fn_cb = (P_BSAL_PROFILE_CB)callback;
        p_bsal_stack-> bsal_srv_objs[i].start_handle = handle;
        memcpy(&p_bsal_stack-> bsal_srv_objs[i].srv_uuid, &uuid, sizeof(bsal_uuid_any_t));
        return BSAL_RESULT_SUCCESS;
    }


    for (i = 0 ; i < p_bsal_stack->srv_num; i++)
    {
        if (p_bsal_stack->bsal_srv_objs[i].start_handle == 0)
        {
            p_bsal_stack->bsal_srv_objs[i].bsal_srv_fn_cb = (P_BSAL_PROFILE_CB)callback;
            p_bsal_stack-> bsal_srv_objs[i].start_handle = handle;
            memcpy(&p_bsal_stack-> bsal_srv_objs[i].srv_uuid, &uuid, sizeof(bsal_uuid_any_t));
            //find device
            return BSAL_RESULT_SUCCESS;
        }
    }
    return BSAL_RESULT_FAIL;
}


bsal_srv_callback_t *bsal_profile_get(void *stack_ptr, uint16_t start_handle)
{
    BSAL_ASSERT_PTR(stack_ptr);
    BSAL_ASSERT_PTR(start_handle);
    uint8_t i = 0;

    bsal_stack_obj_t *p_bsal_stack = stack_ptr;
    for (i = 0 ; i < p_bsal_stack->srv_num; i++)
    {
        if (p_bsal_stack->bsal_srv_objs[i].start_handle == start_handle)
        {
            return &(p_bsal_stack->bsal_srv_objs[i]);
        }
    }

    return NULL;
}


uint16_t bsal_profile_get_start_handle_by_16_uuid(void *stack_ptr, uint16_t uuid)
{
    BSAL_ASSERT_PTR(stack_ptr);
    uint8_t i = 0;
    bsal_stack_obj_t *p_bsal_stack =  stack_ptr;
    for (i = 0 ; i < p_bsal_stack->srv_num; i++)
    {
        if ((p_bsal_stack->bsal_srv_objs[i].srv_uuid.u_type == 16) && (p_bsal_stack->bsal_srv_objs[i].srv_uuid.u16.value == uuid))
        {
            return p_bsal_stack->bsal_srv_objs[i].start_handle;
        }
    }
    return NULL;
}
//bond about

uint8_t io_capablity[3][2] =
{
    {BSAL_GAP_IO_CAP_NO_INPUT_NO_OUTPUT, BSAL_GAP_IO_CAP_DISPLAY_ONLY},
    {BSAL_GAP_IO_CAP_NO_INPUT_NO_OUTPUT, BSAL_GAP_IO_CAP_DISPLAY_YES_NO},
    {BSAL_GAP_IO_CAP_KEYBOARD_ONLY, BSAL_GAP_IO_CAP_KEYBOARD_DISPLAY},
};

static uint8_t bsal_get_in_out_capability(uint8_t input, uint8_t output)
{
    return io_capablity[input][output];
}

//=======================ATT====OPERATE==========================

int bsal_srv_write_data_by_handle(void *stack_ptr, uint16_t u_handle, uint16_t value_length, uint8_t *value)
{
    BSAL_ASSERT_PTR(stack_ptr);
    bsal_stack_obj_t *p_ops = (bsal_stack_obj_t *)stack_ptr;
    BSAL_ASSERT_PTR(p_ops->bsal_ops.cov_handle_to_u_handle);
    return bsal_att_write_entry(stack_ptr, u_handle, value_length , value);
}

int bsal_srv_read_data_by_handle(void *stack_ptr, uint16_t u_handle, uint16_t *value_length, uint8_t **value)
{
    BSAL_ASSERT_PTR(stack_ptr);
    bsal_stack_obj_t *p_ops = (bsal_stack_obj_t *)stack_ptr;
    return bsal_att_read_entry(stack_ptr, u_handle, value_length , value);
}

int bsal_srv_create_att_by_handle(void *stack_ptr, uint16_t u_handle, uint16_t value_length)
{
    BSAL_ASSERT_PTR(stack_ptr);
    bsal_stack_obj_t *p_ops = (bsal_stack_obj_t *)stack_ptr;
    return bsal_att_create_entry(stack_ptr, u_handle, value_length);
}


int bsal_srv_write_data(void *stack_ptr, uint16_t start_handle, uint16_t offset_handle, uint16_t value_length, uint8_t *value)
{
    BSAL_ASSERT_PTR(stack_ptr);
    bsal_stack_obj_t *p_ops = (bsal_stack_obj_t *)stack_ptr;
    BSAL_ASSERT_PTR(p_ops->bsal_ops.cov_handle_to_u_handle);
    return bsal_att_write_entry(stack_ptr, p_ops->bsal_ops.cov_handle_to_u_handle(start_handle, offset_handle), value_length , value);
}

int bsal_srv_read_data(void *stack_ptr, uint16_t start_handle, uint16_t offset_handle, uint16_t *value_length, uint8_t **value)
{
    BSAL_ASSERT_PTR(stack_ptr);
    bsal_stack_obj_t *p_ops = (bsal_stack_obj_t *)stack_ptr;
    return bsal_att_read_entry(stack_ptr, p_ops->bsal_ops.cov_handle_to_u_handle(start_handle, offset_handle), value_length , value);
}

int bsal_srv_create_att(void *stack_ptr, uint16_t start_handle, uint16_t offset_handle, uint16_t value_length)
{
    BSAL_ASSERT_PTR(stack_ptr);
    bsal_stack_obj_t *p_ops = (bsal_stack_obj_t *)stack_ptr;
    return bsal_att_create_entry(stack_ptr, p_ops->bsal_ops.cov_handle_to_u_handle(start_handle, offset_handle), value_length);
}

int bsal_srv_send_notify_data(void *stack_ptr, uint16_t conn_id, uint16_t start_handle, uint16_t offset_handle, uint16_t value_length, uint8_t *value)
{
    BSAL_ASSERT_PTR(stack_ptr);
    bsal_stack_obj_t *p_ops = (bsal_stack_obj_t *)stack_ptr;
    //check the MTU SIZE
    BSAL_ASSERT_PTR(p_ops->bsal_ops.le_srv_send_data);
    return p_ops->bsal_ops.le_srv_send_data(conn_id, start_handle, offset_handle, value_length , value, BSAL_SEND_DATA_TYPE_ANY);
}

int bsal_srv_send_ind_data(void *stack_ptr, uint16_t conn_id, uint16_t service_id, uint16_t offset_handle, uint16_t value_length, uint8_t *value)
{
    BSAL_ASSERT_PTR(stack_ptr);
    bsal_stack_obj_t *p_ops = (bsal_stack_obj_t *)stack_ptr;
    //TODO check the MTU SIZE
    BSAL_ASSERT_PTR(p_ops->bsal_ops.le_srv_send_data);
    return p_ops->bsal_ops.le_srv_send_data(conn_id, service_id, offset_handle, value_length , value, BSAL_SEND_DATA_TYPE_ANY);
}
//===============================================================STACK OPERATE=============================================

void *bsal_find_stack_ptr(char *stack_name)
{

    bsal_osif_printf_dbg("%s line:%d: param %s", __FUNCTION__, __LINE__, stack_name);
    //find the device name
    //When init the rtk object has been insert the object
    /*TODO */
#ifdef PKG_BSAL_NIMBLE_STACK
    extern bsal_stack_obj_t nimble_obj;
    return &nimble_obj;
#else
    extern bsal_stack_obj_t rtk_obj;
    return &rtk_obj;
#endif


}


void bsal_stack_le_srv_begin(void *stack_ptr, uint8_t num, void *p_fun_cb)
{
    BSAL_ASSERT_PTR(stack_ptr);
    bsal_osif_printf_dbg("%s line:%d: param %d", __FUNCTION__, __LINE__, num);
    struct bsal_stack_obj *p_ops = (struct bsal_stack_obj *)stack_ptr;
    p_ops->bsal_ops.le_srv_reg_begin(false, num, p_fun_cb);

    p_ops->bsal_srv_objs =  bsal_osif_malloc((num + 1) * sizeof(bsal_srv_callback_t));
    p_ops->srv_num = num + 1;
    //add common service callback
    bsal_uuid_any_t srv_uuid;
    srv_uuid.u_type = BSAL_UUID_TYPE_16BIT;
    srv_uuid.u16.value = BSAL_SRV_UUID_GENERAL;
    // add the generate uuid for except common profile
    bsal_profile_insert(stack_ptr, 0xffff, p_fun_cb, srv_uuid);
}

int bsal_stack_le_srv_reg_func(void *stack_ptr, void *srv_def, P_SRV_GENERAL_CB *callback)
{
    BSAL_ASSERT_PTR(stack_ptr);
    bsal_osif_printf_dbg("%s line:%d: param srv_def :%p, callback: %p", __FUNCTION__, __LINE__, srv_def, callback);
    //reg the serverice
    struct bsal_stack_obj *p_ops = (struct bsal_stack_obj *)stack_ptr;
    //parse the struct
    BSAL_ASSERT_PTR(p_ops->bsal_ops.le_srv_reg_fun);
    return p_ops->bsal_ops.le_srv_reg_fun(srv_def, callback);
}
void bsal_stack_le_srv_end(void *stack_ptr)
{
    BSAL_ASSERT_PTR(stack_ptr);
    bsal_osif_printf_dbg("%s line:%d: param statck:%p", __FUNCTION__, __LINE__, stack_ptr);
    struct bsal_stack_obj *p_ops = (struct bsal_stack_obj *)stack_ptr;
    BSAL_ASSERT_PTR(p_ops->bsal_ops.le_srv_reg_end);
    p_ops->bsal_ops.le_srv_reg_end();
}



int bsal_stack_init(void *stack_ptr, void *callback)
{
    BSAL_ASSERT_PTR(stack_ptr);
    BSAL_ASSERT_PTR(callback);
    bsal_osif_printf_dbg("%s line:%d: param callback %p", __FUNCTION__, __LINE__, callback);

    struct bsal_stack_obj *p_ops = (struct bsal_stack_obj *)stack_ptr;
#define BSAL_BLE_STACK        0x01
#define BSAL_LEGACY_STACK     0x02
#define BSAL_MESH_STACK       0x04
    p_ops->bsal_ops.init(BSAL_BLE_STACK);


    //save the callback to the ops
    p_ops->app_all_callbak = callback;


    //set default param
    //1. set scan respnse data
    uint8_t tmp_data[32] = {0};
    le_adv_ltv_t ltv;
    ltv.length = 3;
    ltv.type = BSAL_GAP_TYPE_APPEARANCE;
    uint16_t tmp_uuid = BSAL_GAP_APPEARANCE_UNKNOWN;
    ltv.value = (uint8_t *)&tmp_uuid;
    bsal_le_adv_data_add_ltv(tmp_data, &ltv);
    bsal_set_le_scan_rsp_data_user(stack_ptr, tmp_data);

    //2. set adv param
    /** @brief  Default minimum advertising interval when device is discoverable (units of 625us, 160=100ms) */
#define DEFAULT_ADVERTISING_INTERVAL_MIN            320
    /** @brief  Default maximum advertising interval */
#define DEFAULT_ADVERTISING_INTERVAL_MAX            320
    bsal_adv_param_t temp_adv_param;
    memset(&temp_adv_param, 0, sizeof(bsal_adv_param_t));
    temp_adv_param.adv_int_max = DEFAULT_ADVERTISING_INTERVAL_MAX;
    temp_adv_param.adv_int_min = DEFAULT_ADVERTISING_INTERVAL_MIN;
    temp_adv_param.adv_type    = BSAL_GAP_ADTYPE_ADV_IND;
    temp_adv_param.own_address_type = BSAL_GAP_REMOTE_ADDR_LE_PUBLIC;
    temp_adv_param.direct_address_type = BSAL_GAP_REMOTE_ADDR_LE_PUBLIC;
    temp_adv_param.channel_map = BSAL_GAP_ADVCHAN_ALL;
    temp_adv_param.filter_policy = BSAL_GAP_ADV_FILTER_ANY;
    bsal_set_le_adv_parm(stack_ptr, &temp_adv_param);

//SET GAP NAME


//
    bsal_gaps_param_t gaps_param =
    {
        // .name = bsal_get_device_name(stack_ptr),
        .apperance =   BSAL_GAP_APPEARANCE_UNKNOWN,
    };
    memcpy(gaps_param.name, bsal_get_device_name(stack_ptr), BSAL_MAX_NAME_LENGTH);
    bsal_set_gap_param(stack_ptr, &gaps_param);
    //load default;
    bsal_set_device_le_bond_type(stack_ptr, false, BSAL_NO_INPUT, BSAL_NO_OUTPUT, BSAL_GAP_AUTHEN_BIT_NO_BONDING, false);

    p_ops->g_att_index = 0;
    return 0;
}

void bsal_stack_startup(void *stack_ptr)
{
    BSAL_ASSERT_PTR(stack_ptr);
    bsal_osif_printf_dbg("%s line:%d: param stack_ptr %p", __FUNCTION__, __LINE__, stack_ptr);
    struct bsal_stack_obj *p_ops = (struct bsal_stack_obj *)stack_ptr;
    p_ops->bsal_ops.startup();
}

///PARAM
//name
int bsal_stack_set_gap_name(void *stack_ptr, uint8_t length, uint8_t *name)
{

    BSAL_ASSERT_PTR(stack_ptr);
    bsal_osif_printf_dbg("%s line:%d: param stack_ptr %p: length:%d, name:%s", __FUNCTION__, __LINE__, stack_ptr, length, name);
    struct bsal_stack_obj *p_ops = (struct bsal_stack_obj *)stack_ptr;
    // p_ops->bsal_ops.set_gaps_param(length, name);

    return 0;
}

int bsal_stack_get_gap_name(void *stack_ptr, uint8_t *name, uint8_t *length)
{
    BSAL_ASSERT_PTR(stack_ptr);
    return 0;
}

//mac address only get
int bsal_stack_get_mac_addr(void *stack_ptr, uint8_t *bd_addr)
{
//    BSAL_ASSERT_PTR(stack_ptr);
//    BSAL_ASSERT_PTR(bd_addr);
//    bsal_stack_obj_t *p_ops = (bsal_stack_obj_t *)stack_ptr;
    return 0;
}

//adv
int bsal_le_set_adv_param(void *stack_ptr, uint16_t value_length, bsal_adv_param_t *adv_param)
{
    BSAL_ASSERT_PTR(stack_ptr);
    bsal_stack_obj_t *p_ops = (struct bsal_stack_obj *)stack_ptr;
    p_ops->bsal_ops.le_set_adv_param(adv_param);
    return 0;
}

int bsal_le_get_adv_param(void *stack_ptr, bsal_adv_param_t *adv_param)
{
    BSAL_ASSERT_PTR(stack_ptr);
    bsal_stack_obj_t *p_ops = (struct bsal_stack_obj *)stack_ptr;
    p_ops->bsal_ops.le_get_adv_param(adv_param);
    return 0;
}
int bsal_set_device_name(void *stack_ptr, uint8_t length, uint8_t *name)
{
    BSAL_ASSERT_PTR(stack_ptr);
    bsal_stack_obj_t *p_ops = (struct bsal_stack_obj *)stack_ptr;
    memcpy(p_ops->bsal_info.device_name, name, length);
    return 0;
}

uint8_t *bsal_get_device_name(void *stack_ptr)
{
    BSAL_ASSERT_PTR(stack_ptr);
    bsal_stack_obj_t *p_ops = (struct bsal_stack_obj *)stack_ptr;
    return p_ops->bsal_info.device_name;
}


int bsal_sm_set_bond_mode(void *stack_ptr, bsal_bond_param_t *bond_type)
{
    BSAL_ASSERT_PTR(stack_ptr);
    bsal_stack_obj_t *p_ops = (struct bsal_stack_obj *)stack_ptr;
    p_ops->bsal_ops.set_bond_param(bond_type);
    return 0;
}

int bsal_sm_get_bond_mode(void *stack_ptr, bsal_bond_param_t *bond_type)
{
    BSAL_ASSERT_PTR(stack_ptr);
    bsal_stack_obj_t *p_ops = (struct bsal_stack_obj *)stack_ptr;
    p_ops->bsal_ops.get_bond_param(bond_type);
    return 0;
}

int bsal_get_bd_address(void *stack_ptr, uint8_t *bd_addr)
{
    BSAL_ASSERT_PTR(stack_ptr);
    bsal_stack_obj_t *p_ops = (struct bsal_stack_obj *)stack_ptr;
    p_ops->bsal_ops.get_mac_address(BSAL_GAP_REMOTE_ADDR_LE_PUBLIC, bd_addr);
    return 0;
}

int bsal_stack_start_adv(void *stack_ptr)
{
    BSAL_ASSERT_PTR(stack_ptr);
    bsal_stack_obj_t *p_ops = (struct bsal_stack_obj *)stack_ptr;
    p_ops->bsal_ops.le_adv_enable(true);
    return 0;
}
int bsal_stack_stop_adv(void *stack_ptr)
{
    BSAL_ASSERT_PTR(stack_ptr);
    bsal_stack_obj_t *p_ops = (struct bsal_stack_obj *)stack_ptr;
    p_ops->bsal_ops.le_adv_enable(false);
    return 0;
}


uint16_t bsal_set_device_le_bond_type(void *stack_ptr, bool is_bond, uint8_t input, uint8_t output, uint8_t bond_type, bool oob_enable)
{
    BSAL_ASSERT_PTR(stack_ptr);
    bsal_bond_param_t temp_bond_type;
    temp_bond_type.bonding_flag = is_bond;
    temp_bond_type.auth_type = bond_type;
    temp_bond_type.oob_flag = oob_enable;
    temp_bond_type.io_capability = bsal_get_in_out_capability(input, output);
    temp_bond_type.sm_req_enable = false;
    temp_bond_type.force_auth_type = 0;
    bsal_stack_obj_t *p_ops = (bsal_stack_obj_t *)stack_ptr;
    p_ops->bsal_ops.set_bond_param(&temp_bond_type);
    return 0;
}

uint16_t bsal_set_device_le_bond_param(void *stack_ptr, bsal_bond_param_t *bond_param)
{
    BSAL_ASSERT_PTR(stack_ptr);
    bsal_stack_obj_t *p_ops = (bsal_stack_obj_t *)stack_ptr;
    p_ops->bsal_ops.set_bond_param(bond_param);
    return 0;
}


void bsal_set_le_adv_data(void *stack_ptr, uint8_t length, uint8_t *data)
{
    BSAL_ASSERT_PTR(stack_ptr);
    bsal_stack_obj_t *p_ops = (bsal_stack_obj_t *)stack_ptr;
    bsal_adv_param_t temp_adv_param;
    temp_adv_param.set_param_flag = BSAL_SET_LE_ADV_DATA;
    memcpy(temp_adv_param.adv_data, data, length);
    if (length < 31)
    {
        p_ops->bsal_ops.le_set_adv_param(&temp_adv_param);
    }
    else
    {
        //ERROR
    }

}
void bsal_set_le_adv_data_user(void *stack_ptr, uint8_t *adv_data)
{
    BSAL_ASSERT_PTR(stack_ptr);
    bsal_set_le_adv_data(stack_ptr, adv_data[0] + 1 , adv_data);
}



void bsal_set_le_srp_data(void *stack_ptr, uint8_t length, uint8_t *data)
{
    BSAL_ASSERT_PTR(stack_ptr);
    bsal_stack_obj_t *p_ops = (bsal_stack_obj_t *)stack_ptr;
    bsal_adv_param_t temp_adv_param;
    temp_adv_param.set_param_flag = BSAL_SET_LE_SCAN_RSP_DATA;
    memcpy(temp_adv_param.scan_rsp_data, data, length);
    if (length < 31)
    {
        p_ops->bsal_ops.le_set_adv_param(&temp_adv_param);
    }
    else
    {
        //ERROR
    }
}

void bsal_set_le_scan_rsp_data_user(void *stack_ptr, uint8_t *scan_rsp_data)
{
    BSAL_ASSERT_PTR(stack_ptr);
    bsal_set_le_srp_data(stack_ptr, scan_rsp_data[0] + 1 , scan_rsp_data);
}

void bsal_set_le_adv_parm(void *stack_ptr, bsal_adv_param_t *adv_param)
{
    BSAL_ASSERT_PTR(stack_ptr);
    bsal_stack_obj_t *p_ops = (bsal_stack_obj_t *)stack_ptr;
    adv_param->set_param_flag = BSAL_SET_LE_ADV_PARAM;
    p_ops->bsal_ops.le_set_adv_param(adv_param);
}


void bsal_set_gap_param(void *stack_ptr, bsal_gaps_param_t *gaps_param)
{
    BSAL_ASSERT_PTR(stack_ptr);
    bsal_stack_obj_t *p_ops = (bsal_stack_obj_t *)stack_ptr;
    p_ops->bsal_ops.set_gaps_param(gaps_param);
}

//API FOR CALLBACK  CALLED BY STACK
int bsal_stack_ready(void *stack_ptr, bool status)
{
    BSAL_ASSERT_PTR(stack_ptr);
    //true is ready
    //false is fail
    struct bsal_stack_obj *p_ops = (struct bsal_stack_obj *)stack_ptr;
    if (status == true)
    {
        p_ops->app_all_callbak(p_ops, BSAL_CB_LAYER_GAP, BSAL_CB_STACK_READY, 0 , NULL);
    }
    else
    {
        p_ops->app_all_callbak(p_ops, BSAL_CB_LAYER_GAP, BSAL_CB_STACK_ERROR, 0 , NULL);
    }
    return 0;
}

int bsal_gap_connect_status(void *stack_ptr, uint16_t conn_handle, uint8_t connect_status, uint16_t disconnect_status)
{
    BSAL_ASSERT_PTR(stack_ptr);
    T_BSAL_GAP_MSG_DATA  bsal_gap_msg_data;
    bsal_gap_msg_data.gap_conn_state_change.new_state = connect_status;
    bsal_gap_msg_data.gap_conn_state_change.conn_id = conn_handle;
    bsal_gap_msg_data.gap_conn_state_change.disc_cause = disconnect_status;
    struct bsal_stack_obj *p_ops = (struct bsal_stack_obj *)stack_ptr;
    p_ops->app_all_callbak(p_ops, BSAL_CB_LAYER_GAP, BSAL_CB_CONNECT_STATUS, sizeof(bsal_gap_msg_data) , &bsal_gap_msg_data);
    return 0;
}

