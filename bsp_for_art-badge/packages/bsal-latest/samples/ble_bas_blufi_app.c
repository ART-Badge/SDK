/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-05-28     Supperthomas the first version
 * 2021-10-23     rb 		   the second version
 */


#include "bsal.h"
#include <stdio.h>
#include <string.h>
#include "bsal_osif.h"
#include "bsal_srv_blufi.h"
#include "bsal_srv_bas.h"
#include "gap.h"
#include <cJSON.h>

#define DBG_TAG    "ble_bas_blufi_app"
#define DBG_LVL    DBG_INFO
#include <rtdbg.h>

#define BSAL_STACK_NAME PKG_BSAL_STACK_NAME

typedef enum
{
    STEP01 = 0X00,
    STEP02 = 0X01,
    STEP03 = 0X02
} REC_JSON_STEP;

typedef enum
{
    MEET_PAGE00 = 0X00,
    MEET_PAGE01 = 0X01,
    MEET_PAGE02 = 0X02,
    MEET_PAGE03 = 0X03
} MEETING_PAGE;

static void *bsal_stack_ptr = NULL;
uint16_t bsal_app_conn_handle;
static uint8_t battery_flag = 0;
static uint8_t  gap_conn_state = BSAL_GAP_CONN_STATE_DISCONNECTED; /**< GAP connection state */

extern void ui_metting1_update(const char *data);
extern void ui_metting2_update(const char *data);
extern void ui_metting3_update(const char *data);
extern void ui_metting4_update(const char *data);
extern void ui_metting5_update(const char *data);
extern void ui_metting6_update(const char *data);
extern void ui_metting7_update(const char *data);
extern void ui_metting8_update(const char *data);
extern void ui_metting9_update(const char *data);
extern void metting_save_update(const char *data);
static void ble_debug_loginfo(void);

RT_WEAK void ble_info_get_callback(char *dev_name)
{
    rt_kprintf("ble_name:%s\n", dev_name);
    extern void ui_ble_mac_update(const char *ble);
    ui_ble_mac_update(dev_name);
}

void bsa_app_set_adv_data(void *stack_ptr)
{
    uint8_t tmp_data[32] = {0} ; //must be zero
    char tmp_name[20] = {0};
    bsal_le_adv_data_add_flag(tmp_data, BSAL_GAP_ADTYPE_FLAGS_LIMITED | BSAL_GAP_ADTYPE_FLAGS_BREDR_NOT_SUPPORTED);

    char *adv_name = (char *)bsal_get_device_name(stack_ptr);

    //bsal_adv_data_add_uuid16(tmp_data, BSAL_GATT_SERVICE_BATTERY_SERVICE);
    uint8_t bt_addr[6];
    gap_get_param(GAP_PARAM_BD_ADDR, bt_addr);
    sprintf(tmp_name, "%s_%02x%02x", "ART-Badge", bt_addr[1], bt_addr[0]);
    ble_info_get_callback(tmp_name);
    bsal_adv_data_add_name(tmp_data, strlen(tmp_name), tmp_name);
    bsal_set_le_adv_data_user(stack_ptr, tmp_data);
}

void bsal_app_all_callback(void *stack_ptr, uint8_t cb_layer, uint16_t cb_sub_event, uint8_t value_length, void *value)
{
    T_BSAL_GAP_MSG_DATA  *bsal_gap_msg_data = (T_BSAL_GAP_MSG_DATA *)value;
    uint8_t bd_addr[6];

    switch (cb_layer)
    {
        case BSAL_CB_LAYER_GAP:
            switch (cb_sub_event)
            {
                case BSAL_CB_STACK_READY:
                    //get mac address
                    bsal_osif_printf_info("============stack ready===========\r\n");
                    bsa_app_set_adv_data(stack_ptr);
                    bsal_stack_start_adv(stack_ptr);
                    break;

                case BSAL_CB_CONNECT_STATUS:
                    bsal_osif_printf_info("============stack connect id %d===========\r\n", bsal_gap_msg_data->gap_conn_state_change.conn_id);

                    if (bsal_gap_msg_data->gap_conn_state_change.new_state == BSAL_GAP_CONN_STATE_CONNECTED)
                    {
                        bsal_app_conn_handle = bsal_gap_msg_data->gap_conn_state_change.conn_id;
                    }
                    else if (bsal_gap_msg_data->gap_conn_state_change.new_state == BSAL_GAP_CONN_STATE_DISCONNECTED)
                    {
                        bsal_stack_start_adv(stack_ptr);
                        battery_flag = 0;
                    }

                    bsal_osif_printf_info("BSAL: conn_id %d old_state %d new_state %d, disc_cause 0x%x",
                                          bsal_gap_msg_data->gap_conn_state_change.conn_id, gap_conn_state, bsal_gap_msg_data->gap_conn_state_change.new_state, bsal_gap_msg_data->gap_conn_state_change.disc_cause);

                    break;

                default:
                    break;
            }

            if (cb_sub_event == BSAL_CB_STACK_READY)
            {
                //stack ready
                //   APP_PRINT_INFO0("GAP stack ready");

            }

            break;

        case BSAL_CB_LAYER_GATT_PROFILE:
            switch (cb_sub_event)
            {
                    //save the service start_handle
                    //case uuid profile save start_handle
                    //case SRV_CALLBACK66
                    //save the identity
            }

            break;

        case BSAL_CB_LAYER_SM:
            break;

        case BSAL_CB_LAYER_COMMON:
            //connected save the connect id

            break;

        case BSAL_CB_LAYER_UNKNOWN:
            break;

        default:
            break;
    }

}

rt_err_t rtc_set_time(time_t timestamp)
{
    /* converts the local time in time to calendar time. */
    rt_device_t rtc_device;
    rt_err_t ret = -RT_ERROR;

    rtc_device = rt_device_find("rtc");

    if (rtc_device == RT_NULL)
    {
        return -RT_ERROR;
    }

    /* update to RTC device. */
    ret = rt_device_control(rtc_device, RT_DEVICE_CTRL_RTC_SET_TIME, &timestamp);

    return ret;
}

// 蓝牙传输数据回调 -- 解析之后的结果
RT_WEAK void card_info_get_callback(char *name, double timestamp)
{
    rt_kprintf("\n========================= CALLBCK START ===========================\n");
    rt_kprintf("\n name:%s\n", name);
    extern void ui_name_update(const char *name);
    ui_name_update(name);
    rtc_set_time((time_t)(timestamp / 1000) + 8 * 3600);
    rt_kprintf("========================== CALLBCK END ============================\n\n");
}

RT_WEAK void weather_info_get_callback(char *city, char* day_weather, char* min_degree, char* max_degree)
{
    extern void ui_day_weather_update(const char *day_weather);
    extern void ui_degree_update(const char *min_degree);

    rt_kprintf("\n========================= CALLBCK START ===========================\n");
    rt_kprintf("\n city:%s  day_weather:%s  min_degree:%s  max_degree:%s\n", city, day_weather, min_degree, max_degree);
    char city_weather_str[20] = {0};
    char degree_str[10] = {0};
    sprintf(city_weather_str, "%s:%s", city, day_weather);
    sprintf(degree_str, "%s~%s", min_degree, max_degree);
    ui_day_weather_update(city_weather_str);
    ui_degree_update(degree_str);
    rt_kprintf("========================== CALLBCK END ============================\n\n");
}

#include <dcm.h>
#include <dcmtype.h>

RT_WEAK void metting_info_get_callback(uint8_t rec_step, char* time, char* content)
{
    rt_kprintf("\n========================= CALLBCK START ===========================\n");
    char meet_info_str[50];
    static uint8_t metting_page = MEET_PAGE00;

    switch(metting_page)
    {
        case MEET_PAGE00:
            switch(rec_step)
            {
                case STEP01:
                    bsal_osif_printf_info("<======================== ONE RECEIVE START ========================>\n\n");
                    bsal_osif_printf_info("STEP1 data: %s %s\n", time, content);
                    sprintf(meet_info_str, "%s     %s", time, content);
                    ui_metting1_update(meet_info_str);
                    break;

                case STEP02:
                    bsal_osif_printf_info("STEP2 data: %s %s\n", time, content);
                    sprintf(meet_info_str, "%s     %s", time, content);
                    ui_metting2_update(meet_info_str);
                    break;

                case STEP03:
                    // 下一页
                    metting_page = MEET_PAGE01;
                    bsal_osif_printf_info("STEP3 data: %s %s\n", time, content);
                    sprintf(meet_info_str, "%s    %s", time, content);
                    ui_metting3_update(meet_info_str);
                    bsal_osif_printf_info("<======================== ONE RECEIVE END ========================>\n\n");
                    break;

                default:
                    break;
            }

            break;

        case MEET_PAGE01:
            switch(rec_step)
            {
                case STEP01:
                    bsal_osif_printf_info("<======================== SECOND RECEIVE START ========================>\n\n");
                    bsal_osif_printf_info("STEP1 data: %s %s\n", time, content);
                    sprintf(meet_info_str, " %s     %s", time, content);
                    ui_metting4_update(meet_info_str);
                    break;

                case STEP02:
                    bsal_osif_printf_info("STEP2 data: %s %s\n", time, content);
                    sprintf(meet_info_str, " %s     %s", time, content);
                    ui_metting5_update(meet_info_str);
                    // 通知界面跳转
//					extern void ui_page_update(const char *name);
//					char time_str[10] = {0};
//					log_e("tick:%d\n", rt_tick_get());
//					sprintf(time_str, "%d", rt_tick_get());
//					ui_page_update(time_str);
                    break;

                case STEP03:
                    // 下一页
                    metting_page = MEET_PAGE02;
                    bsal_osif_printf_info("STEP3 data: %s %s\n", time, content);
                    sprintf(meet_info_str, " %s     %s", time, content);
                    ui_metting6_update(meet_info_str);
                    bsal_osif_printf_info("<======================== SECOND RECEIVE END ========================>\n\n");
                    break;

                default:
                    break;
            }

            break;

        case MEET_PAGE02:
            switch(rec_step)
            {
                case STEP01:
                    bsal_osif_printf_info("<======================== THIRED RECEIVE START ========================>\n\n");
                    bsal_osif_printf_info("STEP1 data: %s %s\n", time, content);
                    sprintf(meet_info_str, " %s     %s", time, content);
                    ui_metting7_update(meet_info_str);
                    break;

                case STEP02:
                    bsal_osif_printf_info("STEP2 data: %s %s\n", time, content);
                    sprintf(meet_info_str, " %s     %s", time, content);
                    ui_metting8_update(meet_info_str);
                    break;

                case STEP03:
                    // 下一页
                    metting_page = MEET_PAGE03;
                    bsal_osif_printf_info("STEP3 data: %s %s\n", time, content);
                    sprintf(meet_info_str, " %s     %s", time, content);
                    ui_metting9_update(meet_info_str);
                    bsal_osif_printf_info("\n<======================== THIRED RECEIVE END ========================>\n\n");
                    //ble_debug_loginfo(); // log
                    // 标记存储结束避免重复存储
                    metting_save_update("save_ok");
                    break;

                default:
                    break;
            }

            break;
    }

    rt_kputs(meet_info_str);
    rt_kputs("\n");
    rt_kprintf("========================== CALLBCK END ============================\n\n");
}

static void ble_debug_loginfo(void)
{
    struct dcm_pool *pool1;
    const dt_value_t *p_value;

    pool1 = dcm_pool_find("system");

    p_value = dcm_value_get(pool1, "m.p.id.name");
    LOG_I("@ Get m.p.id.name: %s\n", dt_unpack_string(p_value));

    p_value = dcm_value_get(pool1, "m.p.id.metting1");
    LOG_I("@ Get m.p.id.metting1: %s\n", dt_unpack_string(p_value));

    p_value = dcm_value_get(pool1, "m.p.id.metting2");
    LOG_I("@ Get m.p.id.metting2: %s\n", dt_unpack_string(p_value));

    p_value = dcm_value_get(pool1, "m.p.id.metting3");
    LOG_I("@ Get m.p.id.metting3: %s\n", dt_unpack_string(p_value));

    p_value = dcm_value_get(pool1, "m.p.id.metting4");
    LOG_I("@ Get m.p.id.metting4: %s\n", dt_unpack_string(p_value));

    p_value = dcm_value_get(pool1, "m.p.id.metting5");
    LOG_I("@ Get m.p.id.metting5: %s\n", dt_unpack_string(p_value));

    p_value = dcm_value_get(pool1, "m.p.id.metting6");
    LOG_I("@ Get m.p.id.metting6: %s\n", dt_unpack_string(p_value));

    p_value = dcm_value_get(pool1, "m.p.id.metting7");
    LOG_I("@ Get m.p.id.metting7: %s\n", dt_unpack_string(p_value));

    p_value = dcm_value_get(pool1, "m.p.id.metting8");
    LOG_I("@ Get m.p.id.metting8: %s\n", dt_unpack_string(p_value));

    p_value = dcm_value_get(pool1, "m.p.id.metting9");
    LOG_I("@ Get m.p.id.metting9: %s\n", dt_unpack_string(p_value));
}

void bsal_app_profile_callback(void *p)
{
    bsal_callbak_data_t *bsal_param = (bsal_callbak_data_t *)p;

    if (bsal_param->msg_type == BSAL_CALLBACK_TYPE_READ_CHAR_VALUE)
    {
        bsal_osif_printf_info("========callback read from %x====%x=======\r\n", bsal_param->off_handle, bsal_param->srv_uuid.u16.value);
    }
    else if (bsal_param->msg_type == BSAL_CALLBACK_TYPE_INDIFICATION_NOTIFICATION)
    {
        uint16_t  cccbits = bsal_param->value;
        bsal_osif_printf_info("======callback notify from %x===data cccd %x====%x=====\r\n", bsal_param->off_handle, cccbits, bsal_param->srv_uuid.u16.value);

        if (bsal_param->srv_uuid.u16.value == BSAL_GATT_SERVICE_BATTERY_SERVICE)
        {
            if (cccbits & BSAL_GATT_CCC_NOTIFY)
            {
                bsal_osif_printf_info("=========NOTIFY ENABLE from %x===data cccd %x====%x=====\r\n", bsal_param->off_handle, cccbits, bsal_param->srv_uuid.u16.value);
                battery_flag = 1;
            }
            else
            {
                bsal_osif_printf_info("========NOTIFY DISABLE from %x===data cccd %x====%x=====\r\n", bsal_param->off_handle, cccbits, bsal_param->srv_uuid.u16.value);
                battery_flag = 0;
            }
        }
        else if (bsal_param->srv_uuid.u16.value == GATT_UUID_BLUFI_PROFILE)
        {
            if (cccbits & BSAL_GATT_CCC_NOTIFY)
            {
                bsal_osif_printf_info("=========NOTIFY ENABLE from %x===data cccd %x====%x=====\r\n", bsal_param->off_handle, cccbits, bsal_param->srv_uuid.u16.value);
            }
            else
            {
                bsal_osif_printf_info("========NOTIFY DISABLE from %x===data cccd %x====%x=====\r\n", bsal_param->off_handle, cccbits, bsal_param->srv_uuid.u16.value);
            }
        }
    }
    else if (bsal_param->msg_type == BSAL_CALLBACK_TYPE_WRITE_CHAR_VALUE)
    {
        cJSON *root = NULL;
        cJSON *user_item = NULL, *weather_item = NULL;
        cJSON *meet_info_Obj = NULL, *meet_time_item = NULL, *meet_data_item = NULL, *arrayItem = NULL;
        static rt_bool_t metting_save_already = RT_FALSE;

        //bsal_osif_printf_info("\r\n BSAL: THE DATA IS :%s\r\n", bsal_param->data);
        root = cJSON_Parse((const char*)bsal_param->data);

        if (!root)
        {
            rt_kprintf("Error before: [%s]\r\n", cJSON_GetErrorPtr());
        }
        else
        {
            if(strstr((const char*)bsal_param->data, "name"))
            {
                user_item = cJSON_GetObjectItem(root, "name");
                char *name_item = user_item->valuestring;
                user_item = cJSON_GetObjectItem(root, "time");
                double time_item = user_item->valuedouble;
                card_info_get_callback(name_item, time_item);
            }
            else if(strstr((const char*)bsal_param->data, "city"))
            {
                weather_item = cJSON_GetObjectItem(root, "city");
                char *city = weather_item->valuestring;
                weather_item = cJSON_GetObjectItem(root, "day_weather");
                char *day_weather = weather_item->valuestring;
                weather_item = cJSON_GetObjectItem(root, "min_degree");
                char *min_degree = weather_item->valuestring;
                weather_item = cJSON_GetObjectItem(root, "max_degree");
                char *max_degree = weather_item->valuestring;

                weather_info_get_callback(city, day_weather, min_degree, max_degree);
            }
            else
            {
//                if(metting_save_already == RT_TRUE)
//                {
//                    goto __exit;
//                }

//                struct dcm_pool *pool1;

//                const dt_value_t *p_value;

//                pool1 = dcm_pool_find("system");

//                p_value = dcm_value_get(pool1, "m.p.id.save");

//                rt_kprintf("p_value=%s \r\n", dt_unpack_string(p_value));

//                if(strcmp(dt_unpack_string(p_value), "save_ok") == RT_EOK)
//                {
//                    log_i("metting save is ok!");
//                    metting_save_already = RT_TRUE;
//					goto __exit;
//                }

                int size = cJSON_GetArraySize(root);
                rt_kprintf("\ncJSON_GetArraySize: size=%d\r\n", size);

                if((arrayItem = cJSON_GetObjectItem(root, "m_info")) != NULL)
                {
                    if((meet_info_Obj = cJSON_GetArrayItem(arrayItem, 0)) != NULL)
                    {
                        static uint8_t rec_step = 0;

                        switch(rec_step)
                        {
                            case STEP01:
                                rt_kprintf("RECEIVE[1] STEP01 ==>\n");

                                if((meet_time_item = cJSON_GetObjectItem(meet_info_Obj, "m_time")) != NULL)
                                {
                                    rt_kprintf("meet_time1: %s\n", meet_time_item->valuestring);
                                }

                                if((meet_data_item = cJSON_GetObjectItem(meet_info_Obj, "m_data")) != NULL)
                                {
                                    rt_kprintf("meet_data1: %s\n", meet_data_item->valuestring);
                                }

                                rec_step = STEP02;

                                break;

                            case STEP02:
                                rt_kprintf("RECEIVE[1] STEP02 ==>\n");

                                if((meet_time_item = cJSON_GetObjectItem(meet_info_Obj, "m_time")) != NULL)
                                {
                                    rt_kprintf("meet_time1: %s\n", meet_time_item->valuestring);
                                }

                                if((meet_data_item = cJSON_GetObjectItem(meet_info_Obj, "m_data")) != NULL)
                                {
                                    rt_kprintf("meet_data1: %s\n", meet_data_item->valuestring);
                                }

                                rec_step = STEP03;

                                break;

                            case STEP03:
                                rt_kprintf("RECEIVE[1] STEP03 ==>\n");

                                if((meet_time_item = cJSON_GetObjectItem(meet_info_Obj, "m_time")) != NULL)
                                {
                                    rt_kprintf("meet_time1: %s\n", meet_time_item->valuestring);
                                }

                                if((meet_data_item = cJSON_GetObjectItem(meet_info_Obj, "m_data")) != NULL)
                                {
                                    rt_kprintf("meet_data1: %s\n", meet_data_item->valuestring);
                                }

                                rec_step = STEP01;
                                break;

                            default:
                                break;
                        }

                        metting_info_get_callback(STEP01, (char*)meet_time_item->valuestring, (char*)meet_data_item->valuestring);
                    }

                    if((meet_info_Obj = cJSON_GetArrayItem(arrayItem, 1)) != NULL)
                    {
                        static uint8_t rec_step = 0;

                        switch(rec_step)
                        {
                            case STEP01:
                                rt_kprintf("RECEIVE[2] STEP01 ==>\n");

                                if((meet_time_item = cJSON_GetObjectItem(meet_info_Obj, "m_time")) != NULL)
                                {
                                    rt_kprintf("meet_time2: %s\n", meet_time_item->valuestring);
                                }

                                if((meet_data_item = cJSON_GetObjectItem(meet_info_Obj, "m_data")) != NULL)
                                {
                                    rt_kprintf("meet_data2: %s\n", meet_data_item->valuestring);
                                }

                                rec_step = STEP02;
                                break;

                            case STEP02:
                                rt_kprintf("RECEIVE[2] STEP02 ==>\n");

                                if((meet_time_item = cJSON_GetObjectItem(meet_info_Obj, "m_time")) != NULL)
                                {
                                    rt_kprintf("meet_time2: %s\n", meet_time_item->valuestring);
                                }

                                if((meet_data_item = cJSON_GetObjectItem(meet_info_Obj, "m_data")) != NULL)
                                {
                                    rt_kprintf("meet_data2: %s\n", meet_data_item->valuestring);
                                }

                                rec_step = STEP03;
                                break;

                            case STEP03:
                                rt_kprintf("RECEIVE[2] STEP03 ==>\n");

                                if((meet_time_item = cJSON_GetObjectItem(meet_info_Obj, "m_time")) != NULL)
                                {
                                    rt_kprintf("meet_time2: %s\n", meet_time_item->valuestring);
                                }

                                if((meet_data_item = cJSON_GetObjectItem(meet_info_Obj, "m_data")) != NULL)
                                {
                                    rt_kprintf("meet_data2: %s\n", meet_data_item->valuestring);
                                }

                                rec_step = STEP01;
                                break;

                            default:
                                break;
                        }

                        metting_info_get_callback(STEP02, (char*)meet_time_item->valuestring, (char*)meet_data_item->valuestring);
                    }

                    if((meet_info_Obj = cJSON_GetArrayItem(arrayItem, 2)) != NULL)
                    {
                        static uint8_t rec_step = 0;

                        switch(rec_step)
                        {
                            case STEP01:
                                rt_kprintf("RECEIVE[3] STEP01 ==>\n");

                                if((meet_time_item = cJSON_GetObjectItem(meet_info_Obj, "m_time")) != NULL)
                                {
                                    rt_kprintf("meet_time3: %s\n", meet_time_item->valuestring);
                                }

                                if((meet_data_item = cJSON_GetObjectItem(meet_info_Obj, "m_data")) != NULL)
                                {
                                    rt_kprintf("meet_data3: %s\n", meet_data_item->valuestring);
                                }

                                rec_step = STEP02;
                                break;

                            case STEP02:
                                rt_kprintf("RECEIVE[3] STEP02 ==>\n");

                                if((meet_time_item = cJSON_GetObjectItem(meet_info_Obj, "m_time")) != NULL)
                                {
                                    rt_kprintf("meet_time3: %s\n", meet_time_item->valuestring);
                                }

                                if((meet_data_item = cJSON_GetObjectItem(meet_info_Obj, "m_data")) != NULL)
                                {
                                    rt_kprintf("meet_data3: %s\n", meet_data_item->valuestring);
                                }

                                rec_step = STEP03;
                                break;

                            case STEP03:
                                rt_kprintf("RECEIVE[3] STEP03 ==>\n");

                                if((meet_time_item = cJSON_GetObjectItem(meet_info_Obj, "m_time")) != NULL)
                                {
                                    rt_kprintf("meet_time3: %s\n", meet_time_item->valuestring);
                                }

                                if((meet_data_item = cJSON_GetObjectItem(meet_info_Obj, "m_data")) != NULL)
                                {
                                    rt_kprintf("meet_data3: %s\n", meet_data_item->valuestring);
                                }

                                rec_step = STEP01;
                                break;

                            default:
                                break;
                        }

                        metting_info_get_callback(STEP03, (char*)meet_time_item->valuestring, (char*)meet_data_item->valuestring);
                    }
                }
            }
        }

__exit:
        cJSON_Delete(root);
    }
}

//add the db for
void bsal_ble_loop(void *p_param)
{
    uint8_t count = 0;
    battery_flag = 1;

    while (1)
    {
        bsal_osif_delay(20000);
        bsal_osif_printf_info("====hello world===%d=\r\n", battery_flag);

        if (battery_flag == 1)
        {
            uint8_t battery_level = 90 - count * 10;
            count++;
            bsal_bas_send_notify_level(bsal_stack_ptr, bsal_app_conn_handle, battery_level);
        }

        bsal_osif_delay(2000);
    }
}


int bsal_bas_blufi_app(void)
{
    void *stack_ptr = bsal_find_stack_ptr(BSAL_STACK_NAME);

    if (stack_ptr == NULL)
    {
        //print error;
        return 1;
    }

    //set iocapability
    bsal_stack_ptr  = stack_ptr;
    //1. init stack
    bsal_stack_init(stack_ptr, bsal_app_all_callback);  // init param not start stack
    // set device name
    char device_name[20];
    uint8_t bt_addr[6];
    gap_get_param(GAP_PARAM_BD_ADDR, bt_addr);
    sprintf(device_name, "%s_%02x%02x", "ART-Badge", bt_addr[1], bt_addr[0]);
    bsal_set_device_name(stack_ptr, strlen(device_name), (uint8_t *)device_name);
    //2. bond type
    bsal_set_device_le_bond_type(stack_ptr, false, BSAL_NO_INPUT, BSAL_NO_OUTPUT, BSAL_GAP_AUTHEN_BIT_NO_BONDING, false);
    //set the bond flag:

    //3. service begin
    bsal_stack_le_srv_begin(stack_ptr, 2, bsal_app_profile_callback);  //will add 1 service
    //4. bas_init
    bsal_le_bas_svr_init(stack_ptr, bsal_app_profile_callback); //add battery servcie

    //5. blufi_init
    bsal_le_blufi_svr_init(stack_ptr, bsal_app_profile_callback);

    //6. srv_end
    bsal_stack_le_srv_end(stack_ptr);    //end srv add

    //start stack
    bsal_stack_startup(stack_ptr);    //start she

    return 0;
}
MSH_CMD_EXPORT_ALIAS(bsal_bas_blufi_app, bsal_bas_blufi_app, "bluetoooth blufi and bas sample");

//CMD


