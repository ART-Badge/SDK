/*
 * Copyright (C) 2015-2017 Alibaba Group Holding Limited
 */

#include <stdio.h>
#include <stdbool.h>
#include "string.h"
#include <breeze_export.h>
#include "trace.h"
#include "alipay.h"
#include "alipay_common.h"
#include "alipay_vendor.h"


#define SOFTWARE_VERSION "0.2.0"
#define SOFTWARE_VERSION_LEN 5

#ifndef CONFIG_MODEL_SECURITY
#define PRODUCT_ID     850958
#define DEVICE_SECRET  "jDp6NxUImRfZ231nt5Nt1AjEVYPJF4e3"
#define DEVICE_NAME    "new_bz_test123"
#define PRODUCT_KEY    "a1oZpFjRUf3"
#define PRODUCT_SECRET "eZ2JTVukkTz0juw0"
#else
#define PRODUCT_ID     577245
#define DEVICE_SECRET  "FFbidcX3hIuMMFeg70WzCdNEjZL6NZLE"
#define DEVICE_NAME    "12345678901234567890"
#define PRODUCT_KEY    "a1qWr3Wk3Oo"
#define PRODUCT_SECRET "oFTYrP7idjkYEBL9"
#endif
const uint8_t PRODUCT_MODEL[19] = "#ALIPAY_REAL";   //lenth must <= 19  recommend <=15



/* @brief Event handler for Ali-SDK. */
static void dev_status_changed_handler(breeze_event_t event)
{
    switch (event)
    {
    case CONNECTED:
        APP_PRINT_INFO0("dev_status_changed(): Connected.\n");
        break;

    case DISCONNECTED:
        APP_PRINT_INFO0("dev_status_changed(): Disconnected.\n");
        break;

    case AUTHENTICATED:
        APP_PRINT_INFO0("dev_status_changed(): Authenticated.\n");
        break;

    case TX_DONE:
        APP_PRINT_INFO0("dev_status_changed(): Tx-done.\n");
        break;

    default:
        break;
    }
}

/* @brief Data handler for control command 0x00. */
static void set_dev_status_handler(uint8_t *buffer, uint32_t length)
{
    APP_PRINT_INFO2("%s command (len: %d) received.\r\n", TRACE_STRING(__func__), length);
}

/* @brief Data handler for query command 0x02. */
static void get_dev_status_handler(uint8_t *buffer, uint32_t length)
{
    /* echo the receiving data */
    uint8_t cmd = 0x03;
    breeze_post_ext(cmd, buffer, length);
}

#ifdef CONTINUE_BEL_ADV
static void adv_work(void *arg)
{
    static uint8_t user_adv[] = {0x55, 0xaa};

    user_adv[0]++;
    user_adv[1]++;
    breeze_append_adv_data(user_adv, sizeof(user_adv) / sizeof(user_adv[0]));

    if (!ble_connected) { breeze_restart_advertising(); }
    aos_post_delayed_action(2000, adv_work, NULL);
}
#endif

static void apinfo_handler(breeze_apinfo_t *ap)
{
    APP_PRINT_INFO1("Hello %s\r\n", __func__);
}

static void ota_handler(breeze_otainfo_t *ota)
{
    /*need to move ota logic here*/
    if (ota != NULL)
    {
        if (ota->type == OTA_CMD)
        {
            APP_PRINT_INFO0("RECV OTA CMD\n");
        }
        else if (ota->type == OTA_EVT)
        {
            APP_PRINT_INFO1("RECV OTA EVT (%d)\n", ota->cmd_evt.m_evt.evt);
        }
        else
        {
            APP_PRINT_INFO0("unknown ota info\r\n");
        }

    }
}

void alink_work(void)
{
    bool                 ret;

    struct device_config init_bzlink;

    memset(&init_bzlink, 0, sizeof(struct device_config));

    init_bzlink.product_id        = PRODUCT_ID;
    init_bzlink.status_changed_cb = dev_status_changed_handler;
    init_bzlink.set_cb            = set_dev_status_handler;
    init_bzlink.get_cb            = get_dev_status_handler;
    init_bzlink.apinfo_cb         = apinfo_handler;

    init_bzlink.product_secret_len = strlen(PRODUCT_SECRET);
    memcpy(init_bzlink.product_secret, PRODUCT_SECRET,
           init_bzlink.product_secret_len);

    init_bzlink.product_key_len = strlen(PRODUCT_KEY);
    memcpy(init_bzlink.product_key, PRODUCT_KEY, init_bzlink.product_key_len);

    init_bzlink.device_key_len = strlen(DEVICE_NAME);
    memcpy(init_bzlink.device_key, DEVICE_NAME, init_bzlink.device_key_len);

#ifndef CONFIG_MODEL_SECURITY
    init_bzlink.secret_len = strlen(DEVICE_SECRET);
    memcpy(init_bzlink.secret, DEVICE_SECRET, init_bzlink.secret_len);
#else
    init_bzlink.secret_len = 0;
#endif



    init_bzlink.ota_cb = ota_handler;
    ret = breeze_start(&init_bzlink);
    if (ret != 0)
    {
        APP_PRINT_INFO0("breeze_start failed.\r\n");
    }
    else
    {
        APP_PRINT_INFO0("breeze_start succeed.\r\n");
    }
#if 1
    alipay_env_init();

    uint8_t testBuf[100];
    uint32_t testLen[1];

    if (alipay_get_binding_string(testBuf, testLen) == RV_OK)
    {
        APP_PRINT_INFO1("alipay_get_binding_string success! len = %d\n", testLen[0]);
    }
#endif
}

