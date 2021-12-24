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


//#include "bsal_stack.h"

bsal_stack_obj_t rtk_obj;

static void *bsal_get_rtk_obj()
{
    return &rtk_obj;
}
//================================BSAL.C==========================
static void bsal_rtk_stack_init(uint8_t stack_type)
{
	    /* TODO*/
}

static void bsal_rtk_stack_startup(void)
{
      /* TODO*/
}

static BSAL_STATUS bsal_rtk_le_adv_enable(bool enable)
{
    if (enable == true)
    {
      /* TODO*/
    }
    else
    {
			/* TODO*/
    }
    return BSAL_RESULT_SUCCESS;
}
//for app
static BSAL_STATUS bsal_srv_profile_srv_begin(bool is_continue, uint8_t service_num, void *p_fnc_callback)
{
	  /* TODO */
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


    bsal_gatt_res_t res;
    bsal_util_sum_handle_num(p_srv, &res);
    att_num  = res.attrs;
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
	  /* TODO */
    return BSAL_RESULT_FAIL;
}

static uint32_t bsal_stack_rtk_get_unique_handle(uint16_t start_id, uint16_t offset_handle)
{
	  /* TODO */
    return 0;
}

static void bsal_stack_rtk_set_bond_param(bsal_bond_param_t *bond_param)
{
    uint8_t  auth_pair_mode;
    uint16_t auth_flags;
     /* TODO */
}

static void bsal_rtk_le_set_adv_param(bsal_adv_param_t *adv_param)
{
/* TODO*/
}
static void bsal_rtk_set_gas_param(bsal_gaps_param_t *adv_param)
{
    uint8_t  slave_init_mtu_req = false;
/* TODO*/
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



