/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-02-01     zhangsz      Init first
 */

#ifndef __RIL_H__
#define __RIL_H__

#include <rtthread.h>

#if defined(PKG_USING_RIL)

#define SMS_FORMAT_PDU              0
#define SMS_FORMAT_TEXT             1

enum _RIL_MSG_ID
{
    /* ril service msg ids */
    RIL_MSG_NONE_ID = 0, //None
    RIL_MSG_GET_SIGNAL_LEVEL, //get signal length
    RIL_MSG_GET_SIM_STATE, //get sim card state
    RIL_MSG_GET_SERVICE_STATE, //get service state

    /* ril device msg ids */
    RIL_MSG_DEVICE_READY,
    RIL_MSG_OPEN_DIAL, //open dial
    RIL_MSG_INCOMING_RING, //phone incoming ring
    RIL_MSG_RING_OFF, //phone ring off
    RIL_MSG_CALL_ANSWER, //answer call
    RIL_MSG_CALL_STATE_CHANGE, //call status changed
    RIL_MSG_SMS_RECV, //sms comming
    RIL_MSG_SMS_READ, //get sms
    RIL_MSG_SMS_SEND, //send sms
    RIL_MSG_SMS_DELETE, //delete sms
    RIL_MSG_GSM_STATE_CHANGE, //gsm state comming
    RIL_MSG_SIM_STATE_CHANGE, //sim state comming
    RIL_MSG_MAX,
};

enum _RILS_LOOP_TIMER
{
    RILS_TMR_SIGNAL_LEVEL = 0,
    RILS_TMR_SIM_STATE,
    RILS_TMR_SERVICE_STATE,
    RILS_TMR_MAX,
};

enum _RIL_SIGNAL_STRENGTH_LEVEL
{
    SIGNAL_STRENGTH_NONE_OR_UNKNOWN = 0,
    SIGNAL_STRENGTH_POOR,
    SIGNAL_STRENGTH_MODERATE,
    SIGNAL_STRENGTH_GOOD,
    SIGNAL_STRENGTH_GREAT,
};

enum _RIL_SMS_STATE
{
    SMS_STATE_NONE = 0,
    SMS_STATE_NEW,
    SMS_STATE_UNREAD,
    SMS_STATE_READ,
    SMS_STATE_DELETE,
};

enum RIL_PHONE_ACTIVITY_STATUS
{
    ME_STATE_READY = 0,
    ME_STATE_DISABLE,
    ME_STATE_UNKNOWN,
    ME_STATE_RING,
    ME_STATE_INCALL,
    ME_STATE_ERROR,
};

struct rils_func_cb_ops
{
    void (*incoming_cb)(void *param);
    void (*answer_cb)(void *param);
    void (*end_call_cb)(void *param);
    void (*signal_strength_cb)(void *param);
    void (*gsm_service_cb)(void *param);
    void (*sim_state_cb)(void *param);
    void (*sms_receive_cb)(void *param);
    void (*sms_send_cb)(void *param);
};

struct netinfo_t
{
    rt_uint8_t type; /* TODO */
};

struct gsmstate_t
{
    rt_uint8_t type; /* TODO */
};

struct cell_t
{
    rt_uint8_t type; /* TODO */
};

struct setting_t
{
    rt_uint8_t type; /* TODO */
};

struct apn_t
{
    rt_uint8_t type; /* TODO */
};

typedef void (*rils_send_sms_cb_t)(rt_uint32_t status);
typedef void (*rils_delete_sms_cb_t)(rt_uint32_t status);
typedef void (*rils_open_dial_cb_t)(rt_uint32_t status);

struct ril_open_dial
{
    rt_uint32_t option; /* open dial option */
    rt_uint32_t status; /* dial status */
    rils_open_dial_cb_t func_cb; //callback
    char phone[RIL_CALL_NUM_MAX_LEN]; /* phone number */
};

struct _ril_send_sms
{
    rt_uint16_t sms_len; /* sms data length */
    rt_uint16_t option; /* sms format */
    rils_send_sms_cb_t func_cb; //callback
    char phone[RIL_CALL_NUM_MAX_LEN]; /* sms name : phone number */
    char sms_data[RIL_MAX_SMS_MSG_LEN]; /* sms pdu data */
};

struct ril_delete_sms
{
    rt_uint32_t sms_index; /* sms message index */
    rt_uint32_t sms_option; /* sms delete option */
    rils_delete_sms_cb_t func_cb; //callback
};

struct ril_recv_sms
{
    rt_uint32_t sms_index; /* sms index */
    rt_uint32_t sms_len; /* sms length */
    char recv_time[RIL_MAX_TIMESTAMP_LEN];
    char phone[RIL_CALL_NUM_MAX_LEN]; /* sms name : phone number */
    char sms_data[RIL_MAX_SMS_MSG_LEN]; /* sms pdu data */
};

struct rild_ops
{
    rt_err_t (*get_imei)(struct rt_device *device, char *pImeiStr); /* get IMEI string*/
    rt_err_t (*get_cgmi)(struct rt_device *device, char *pCgmiStr); /* get manufacturer identification */
    rt_err_t (*get_cgmm)(struct rt_device *device, char *pCgmmStr); /* get model identification */
    rt_err_t (*get_cgmr)(struct rt_device *device, char *pCgmrStr); /* get revision identification of software */
    rt_err_t (*get_smsc_addr)(struct rt_device *device, char *pSmscStr); /* get sms service center address */
    rt_err_t (*set_smsc_addr)(struct rt_device *device, char *pSmscStr, rt_uint32_t option); /* get sms service center address */
    rt_err_t (*get_creg)(struct rt_device *device, rt_uint8_t *pCreg); /* get Network Registration */
    rt_err_t (*get_apn)(struct rt_device *device, char *pApnStr); /* get apn string*/
    rt_err_t (*get_imsi)(struct rt_device *device, char *pImsiStr); /* get imsi string */
    rt_err_t (*get_rssi)(struct rt_device *device, rt_uint8_t *pRssi); /* get rssi */
    rt_err_t (*set_apn)(struct rt_device *device, char *pApnStr); /* get apn string */
    rt_err_t (*open_fly_mode)(struct rt_device *device); /* enter fly mode */
    rt_err_t (*close_fly_mode)(struct rt_device *device); /* close fly mode */
    rt_err_t (*get_fly_mode_status)(struct rt_device *device, rt_uint8_t *status); /* get fly mode status */
    rt_err_t (*open_data_service)(struct rt_device *device); /* open data service */
    rt_err_t (*close_data_service)(struct rt_device *device); /* close data service */
    rt_err_t (*get_data_service_status)(struct rt_device *device, rt_uint8_t *status); /* get data service status */
    rt_err_t (*get_signal_strength)(struct rt_device *device, rt_int32_t strength); /*  */
    rt_err_t (*set_signal_statecb)(struct rt_device *device, void (*singalcb)(void *args), void  *args, int onoff); /*  */
    rt_err_t (*get_network_info)(struct rt_device *device, struct netinfo_t *info); /*  */
    rt_err_t (*get_gsmservice_state)(struct rt_device *device, struct gsmstate_t *state); /*  */
    rt_err_t (*set_gsmservice_statecb)(struct rt_device *device, void (*gsmstatecb)(void *args), void  *args, int onoff); /*  */
    rt_err_t (*get_current_cellinfo)(struct rt_device *device, struct cell_t * cellinfo); /*  */
    rt_err_t (*get_near_cellinfo)(struct rt_device *device, struct cell_t *cellinfo); /*  */
    rt_err_t (*set_cellst_cb)(struct rt_device *device, void (*cellstatecb)(void *args), void  *args, int onoff); /*  */
    rt_err_t (*open_dial)(struct rt_device *device, char* phonenum, int option, rils_open_dial_cb_t func_cb); /*  */
    rt_err_t (*get_incoming_call)(struct rt_device *device, char* callnum); /*   */
    rt_err_t (*answer_call)(struct rt_device *device, int callid, int option); /*   */
    rt_err_t (*release_all_call)(struct rt_device *device, int option); /*   */
    rt_err_t (*end_call)(struct rt_device *device, int callid, int option); /*   */
    rt_err_t (*hold_call)(struct rt_device *device, int callid, int option); /*   */
    rt_err_t (*set_callstate_cb)(struct rt_device *device, void (*callstatecb)(void *args), void  *args, int onoff); /*   */
    rt_err_t (*set_call_volume)(struct rt_device *device, int vol); /*  */
    rt_err_t (*get_call_volume)(struct rt_device *device, int *p_volume); /*   */
    rt_err_t (*set_call_hitcmd)(struct rt_device *device, char *hit_cmd); /*   */
    rt_err_t (*get_call_state)(struct rt_device *device, rt_uint8_t *state); /* RIL_PHONE_ACTIVITY_STATUS */
    rt_err_t (*bt_call)(struct rt_device *device, char* phonenum, int option); /*   */
    rt_err_t (*bt_answer_call)(struct rt_device *device, int callid, int option); /*   */
    rt_err_t (*bt_end_call)(struct rt_device *device, int callid, int option); /*   */
    rt_err_t (*set_phone_vibrate)(struct rt_device *device, int onoff); /*  */
    rt_err_t (*switch_volte)(struct rt_device *device, int onoff); /*   */
    rt_err_t (*get_ims_state)(struct rt_device *device, char *state); /*   */
    rt_err_t (*set_ims_changecb)(struct rt_device *device, void (*imscb)(void *args), void  *args, int onoff); /*  */
    rt_err_t (*phone_setting)(struct rt_device *device, struct setting_t* set); /* */
    rt_err_t (*get_phone_setting)(struct rt_device *device, struct setting_t *set); /*  */
    rt_err_t (*sim_power_control)(struct rt_device *device, int onoff); /*   */
    rt_err_t (*get_iccid)(struct rt_device *device, char* iccid); /*  */
    rt_err_t (*get_meid)(struct rt_device *device, char* eid); /*   */
    rt_err_t (*get_mcc)(struct rt_device *device, char* mcc); /* get mcc */
    rt_err_t (*get_mnc)(struct rt_device *device, char* mnc); /* get mnc */
    rt_err_t (*get_spn)(struct rt_device *device, char* spn); /* get spn */
    rt_err_t (*get_gid)(struct rt_device *device, char* gid); /* get gid */
    rt_err_t (*send_sms_message)(struct rt_device *device, char *phone_num, char *smstext,
        rt_uint16_t smslen, rt_uint32_t option, rils_send_sms_cb_t func_cb); /* send sms message */
    rt_err_t (*delete_sms_message)(struct rt_device *device, rt_uint32_t sms_index, rt_uint32_t sms_option,
        rils_delete_sms_cb_t func_cb); /* delete sms message */
    rt_err_t (*read_sms_message)(struct rt_device *device, int index); /* read sms message */
    rt_err_t (*set_sms_msgcb)(struct rt_device *device, void (*smscb)(void *args), void  *args, int onoff); /*  */
    rt_err_t (*set_sms_service)(struct rt_device *device, char* address); /* sms service center address */
    rt_err_t (*open_modem)(struct rt_device *device); /* modem power on */
    rt_err_t (*close_modem)(struct rt_device *device); /* modem power off */
    rt_err_t (*get_modem_status)(struct rt_device *device, rt_uint8_t *status); /* get modem power status */
    rt_err_t (*get_operator_info)(struct rt_device *device, char *opera); /* get current network operator */
};

/* ril device and operations */
struct ril_device
{
    struct rt_device parent;
    const struct rild_ops *ops;
    const struct rils_func_cb_ops *func_cb_ops;
};

rt_err_t ril_device_register(const char *name, const struct rild_ops *ops, void *user_data);
rt_err_t rils_func_cb_register(const struct rils_func_cb_ops *ops);

enum ril_object_class_type
{
    RIL_Object_Class_Sim = 0x00, /**< The object is SIM. */
    RIL_Object_Class_Sms, /**< The object is SMS. */
    RIL_Object_Class_Call, /**< The object is Call. */
    RIL_Object_Class_Unknown, /**< The object is unknown. */
    RIL_Object_Class_Static = 0x80 /**< The object is a static object. */
};

/**
 * The container of the ril object
 */
struct ril_container
{
    enum ril_object_class_type type;    /**< object class type */
    rt_list_t   head;   /**< object list head */
    struct rt_mutex mutex;
};

/**
 * RIL Sim structure
 */
struct _ril_sim_info
{
    char name[RIL_IMSI_MAX_LEN]; /* sim name : sim card number */
    rt_uint8_t type; /* 0 : Normal; 1 : USIM; 2 : ESIM */
    rt_uint8_t state; /* 0 : Normal; 1 : error; */
};

/**
 * RIL Device structure
 */
struct _ril_dev_info
{
    rt_uint8_t signal_strength_level;
    rt_uint8_t modem_status;
    rt_uint8_t data_service_status;
    rt_uint8_t fly_mode;
    rt_uint8_t signal_strength;
    rt_uint8_t reg_status;
    char sn[RIL_SN_MAX_LEN];
    char imei[RIL_IMEI_MAX_LEN];
    char apn[RIL_APN_MAX_LEN];
    char imsi[RIL_IMSI_MAX_LEN];
    char iccid[RIL_ICCID_MAX_LEN];
    char meid[RIL_MEID_MAX_LEN];
    char mcc[RIL_MCC_MAX_LEN];
    char mnc[RIL_MNC_MAX_LEN];
    char spn[RIL_SPN_MAX_LEN];
    char gid[RIL_GID_MAX_LEN];
    char cgmi[RIL_CGMI_MAX_LEN];
    char cgmm[RIL_CGMM_MAX_LEN];
    char cgmr[RIL_CGMR_MAX_LEN];
    char smsc[RIL_CALL_NUM_MAX_LEN];
};

/**
 * RIL Call structure
 */
struct _ril_call_info
{
    char name[RIL_CALL_NUM_MAX_LEN]; /* call name : phone number */
    rt_uint8_t answer_call_id; /* call id */
    rt_uint8_t come_call_number1_state;
    time_t come_call_time1; /* call time */
};

/**
 * RIL SMS structure
 */
struct _ril_sms
{
    rt_uint8_t flag; /* old : 0 ; new : 1 ; other : 2 */
    rt_uint8_t type; /* receive: 0 ; send : 1 */
    rt_uint8_t status; /* normal: 0 ; error : 1 */
    char number[RIL_CALL_NUM_MAX_LEN]; /* sms name : phone number */
    char * sms_data; /* sms data */
    rt_uint16_t sms_len; /* sms data length */
    time_t recv_time; /* sms receive time */
    time_t send_time; /* sms receive time */
    rt_list_t list; /* sms list */

    void *user_data;
};
typedef struct _ril_sms *ril_sms_t;

struct ril_sms_setup
{
    rt_uint8_t flag; /* old : 0 ; new : 1 ; other : 2 */
    rt_uint8_t type; /* receive: 0 ; send : 1 */
    char number[RIL_CALL_NUM_MAX_LEN]; /* sms name : phone number */
    char * sms_data; /* sms data */
    rt_uint16_t sms_len; /* sms data length */
};

struct mq_msg
{
    rt_uint32_t cmd;
    rt_uint32_t dat;
    rt_uint32_t len;
    char *buf;
};

const char * rils_get_imei(void);
const char * rils_get_cgmi(void);
const char * rils_get_cgmm(void);
const char * rils_get_cgmr(void);
const char * rils_get_smsc_addr(rt_uint32_t option); /* sms service center addr */
rt_err_t rils_set_smsc_addr(const char *phoneNumber, rt_uint32_t option);
rt_uint8_t rils_get_creg(void);
const char * rils_get_apn(void);
const char * rils_get_imsi(void);
const char * rils_get_iccid(void);
const char * rils_get_meid(void);
const char * rils_get_mcc(void);
const char * rils_get_mnc(void);
const char * rils_get_spn(void);
const char * rils_get_gid(void);
const char * rils_get_operator_info(void);

rt_uint8_t rils_get_signal_strength(void);
rt_uint8_t rils_get_signal_strength_level(void);
char * rils_get_signal_strength_info(void);
char * rils_get_neighbor_cell_info(void);
char * rils_get_cell_info(void);

rt_err_t rils_open_modem(void);
rt_err_t rils_close_modem(void);
rt_bool_t rils_get_modem_status(void);
rt_uint8_t rils_get_call_state(void);

rt_err_t rils_open_data_service(void);
rt_err_t rils_close_data_service(void);
rt_bool_t rils_get_data_service_status(void);

rt_err_t rils_open_fly_mode(void);
rt_err_t rils_close_fly_mode(void);
rt_bool_t rils_get_fly_mode_status(void);

rt_err_t rils_dial(char* phonenum, int option, rils_open_dial_cb_t func_cb);
rt_err_t rils_answer_call(int callid, int option);
rt_err_t rils_end_call(int callid, int option);

rt_err_t rils_send_sms(char *phone_num, char *msg, rt_uint32_t option, rils_send_sms_cb_t func_cb);
rt_err_t rils_delete_sms(rt_uint32_t sms_index, rt_uint32_t sms_option, rils_delete_sms_cb_t func_cb);

void rild_ready_callback(const char *data, rt_size_t size);
void rild_answer_call_callback(const char *data, rt_size_t size);
void rild_sms_read_callback(rt_uint32_t index, const char *data, const char *phoneNumber,
    const char *timestamp);
void rild_sms_new_callback(rt_uint16_t index);
void rild_call_incoming_callback(const char *data, rt_size_t size);
void rild_call_close_callback(const char *data, rt_size_t size);

rt_err_t vendor_rild_init(const char *dev_name,  rt_size_t recv_bufsz);

#endif

#endif
