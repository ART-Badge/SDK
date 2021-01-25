/*
 * File      : js_message.c
 * COPYRIGHT (C) 2012-2020, Shanghai Real-Thread Technology Co., Ltd
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-10-26     realthread   the first version
 */
#ifndef NOTIFICATION_MC_H__
#define NOTIFICATION_MC_H__

#include <rtdef.h>
#include <js_api_wrapper.h>

#define TIME_SORT_MODE              "TimeMode";     // 0
#define PRIORITY_SORT_MODE          "PriorityMode"; // 1

#define NOTIFI_SORT_MODE            "notifi_sort_mode";

#define NOTIFI_ID                     "id"
#define NOTIFI_TITLE                  "title"
#define NOTIFI_ICON_PATH              "icon_path"
#define NOTIFI_SENDER                 "sender"
#define NOTIFI_TEXT_CONTEXT           "text_content"
#define NOTIFI_IMAGE_CONTEXT_PATH     "image_context_path"
#define NOTIFI_MAG_TYPE               "msg_type"
#define NOTIFI_NOTIFICATION_WAY       "notification_way"
#define NOTIFI_PRIORITY               "priority"
#define NOTIFI_ARRIVE_TIME            "arrived_time"


#if defined(JSFW_USING_NOTIFICATION)
typedef enum
{
    NORMALTEXTMSGTYPE = 0,
    NORMALPICTUREMSGTYPE,
    SCHEDULEMSGTYPE

}MessageType;

typedef enum
{
    SHOCK = 0,
    VOICE,
    SHOCKANDVOICE,
    NOPROMPT

}NotificationWay;

typedef enum
{
    NotReadPool = 0,
    HaveReadPool,
    AllPool

}PoolType;

struct Notification_Msg
{
    char* title;                      // msg title
    char* icon_path;                  // msg icon path
    char* sender;                     // msg sender app name
    char* text_content;               // msg text content
    char* image_context_path;         // msg image content path
    char* msg_type;                   // msg msg typpe(text or picrue)
    char* notification_way;           // msg notification way
    rt_uint32_t priority;                   // msg priority
    time_t arrived_time;                    // msg arrived time
};

struct notifi_blob
{
    js_value_t buf;                            /* blob data buffer */
    size_t len;                                /* blob data length */
};
typedef struct notifi_blob notifi_blob_t;

struct notifi_cb
{
    js_value_t this_value;
    js_value_t callback;                       /* value change callback function */
    js_value_t user_data;                      /* callback user-data */
    struct notifi_cb* next;                    /* callback list */
};
typedef struct notifi_cb notifi_cb_t;

struct notifi_node
{
    char name[20];                       /* cache node name */
    struct notifi_blob blob;
    struct notifi_node* next;
};
typedef struct notifi_node notifi_node_t;

struct notifi_pool
{
    char sort_mode[15];
    const char* name;                         /* cache pool name */
    struct notifi_cb *cb_list;                /* cache node callback list */
    struct notifi_node* clist;                /* cache node list array */
};
typedef struct notifi_pool notifi_pool_t;


js_value_t struct_to_js_obj(const char* title,
                            const char* icon_path,
                            const char* sender,
                            const char* text_content,
                            const char* image_context_path,
                            const char* msg_type,
                            const char* notification_way,
                            rt_uint32_t priority,
                            time_t arrived_time);

int notifi_pool_init(void);
int notifi_delete(const char* key);
int notifi_pool_delete_all(void);
int notifi_node_put(js_value_t msg);
int notifi_pool_get_item_count(PoolType pool_type);
int notifi_pool_set_sort_mode(const char* mode);
int notifi_pool_update_read_status(const char* key);
int notifi_pool_bind_callback(struct notifi_cb *cb_info);
int notifi_pool_unbind_callback(js_value_t this_value, js_value_t call_back);
const struct notifi_node* notifi_node_get(int index, PoolType pool_type);


 /* C code use only these */
rt_bool_t pushNotifiMsg(const char* title,
                        const char* icon_path,
                        const char* sender,
                        const char* text_content,
                        const char* image_context_path,
                        const char* msg_type,
                        const char* notification_way,
                        rt_uint32_t priority,
                        time_t arrived_time);

NotificationWay string_to_notificationway(const char* str);
MessageType string_to_messagetype(const char* str);
char* notificationway_to_string(NotificationWay way);
char* messagetype_to_string(MessageType msg_type);


#endif

#endif // NOTIFICATION_MC_H__
