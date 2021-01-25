/**
*****************************************************************************************
*     Copyright(c) 2018, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file     ams_client.h
  * @brief
  * @details
  * @author
  * @date
  * @version  v1.0
  * *************************************************************************************
*/

#ifndef _AMS_CLIENT_H_
#define _AMS_CLIENT_H_

#ifdef  __cplusplus
extern "C" {
#endif


#include <stdbool.h>
#include <stdint.h>
#include "profile_client.h"


/** @defgroup AMS_CLIENT AMS Client
* @brief AMS client
* @{
*/


/** @addtogroup AMS_CLIENT_Exported_Macros AMS Client Exported Macros
  * @brief
  * @{
  */

/** @brief  Define links number. range: 0-4 */
#define AMS_MAX_LINKS  4


/** End of AMS_CLIENT_Exported_Macros
* @}
*/



/** @defgroup AMS_CLIENT_Exported_Types AMS Client Exported Types
  * @brief
  * @{
  */


/** @brief AMS client remote command id*/
typedef enum
{
    REMOTE_CMD_ID_PLAY                  = 0,
    REMOTE_CMD_ID_PAUSE                 = 1,
    REMOTE_CMD_ID_TOGGLE_PLAY_PAUSE     = 2,
    REMOTE_CMD_ID_NEXT_TRACK            = 3,
    REMOTE_CMD_ID_PREVIOUS_TRACK        = 4,
    REMOTE_CMD_ID_VOLUME_UP             = 5,
    REMOTE_CMD_ID_VOLUME_DOWN           = 6,
    REMOTE_CMD_ID_ADVANCE_REPEAT_MODE   = 7,
    REMOTE_CMD_ID_ADVANCE_SHUFFLE_MODE  = 8,
    REMOTE_CMD_ID_SKIP_FORWARD          = 9,
    REMOTE_CMD_ID_SKIP_BACKWARD         = 10,
    REMOTE_CMD_ID_LIKE_TRACK            = 11,
    REMOTE_CMD_ID_DISLIKE_TRACK         = 12,
    REMOTE_CMD_ID_BOOK_MARK_TRACK       = 13,
    REMOTE_CMD_ID_RESERVED              = 255
} T_AMS_REMOTE_CMD_ID;

/** @brief AMS client entity id*/
typedef enum
{
    ENTITY_ID_PLAYER    = 0,
    ENTITY_ID_QUEUE     = 1,
    ENTITY_ID_TRACK     = 2,
    ENTITY_ID_RESERVED  = 255
} T_AMS_ENTITY_ID;

/** @brief AMS client entity player attribute id*/
typedef enum
{
    PLAYER_ATTR_IDPLAYER_ATTR_ID_VOLUME_NAME             = 0,
    PLAYER_ATTR_ID_PLAYBACK_INFO    = 1,
    PLAYER_ATTR_ID_VOLUME           = 2,
    PLAYER_ATTR_ID_RESERVED         = 255
} T_AMS_ENTITY_PLAYER_ATTR_ID;

/** @brief AMS client entity queue attribute id*/
typedef enum
{
    QUEUE_ATTR_ID_INDEX         = 0,
    QUEUE_ATTR_ID_COUNT         = 1,
    QUEUE_ATTR_ID_SHUFFLE_MODE  = 2,
    QUEUE_ATTR_ID_REPEAT_MODE   = 3,
    QUEUE_ATTR_ID_RESERVED      = 255
} T_AMS_ENTITY_QUEUE_ATTR_ID;

/** @brief AMS client entity track attribute id*/
typedef enum
{
    TRACK_ATTR_ID_ARTIST    = 0,
    TRACK_ATTR_ID_ALBUM     = 1,
    TRACK_ATTR_ID_TITLE     = 2,
    TRACK_ATTR_ID_DURATION  = 3,
    TRACK_ATTR_ID_RESERVED  = 255
} T_AMS_ENTITY_TRACK_ATTR_ID;

/** @brief AMS client entity attribute id*/
typedef union
{
    T_AMS_ENTITY_PLAYER_ATTR_ID     player_atrr_id;
    T_AMS_ENTITY_QUEUE_ATTR_ID      queue_attr_id;
    T_AMS_ENTITY_TRACK_ATTR_ID      track_attr_id;
} T_AMS_ENTITY_ATTR_ID;

/** @brief AMS client entity attribute*/
typedef struct
{
    T_AMS_ENTITY_ID         entity_id;
    T_AMS_ENTITY_ATTR_ID    attr_id;
} T_AMS_ENTITY_ATTR;


/** @brief AMS client handle type*/
typedef enum
{
    AMS_HDL_SRV_START,          //!< service start handle
    AMS_HDL_SRV_END,            //!< service end handle
    AMS_HDL_REMOTE_CMD_VALUE,   //!< remote command characteristic value handle
    AMS_HDL_REMOTE_CMD_CCCD,    //!< remote command characteristic CCCD handle
    AMS_HDL_ENTITY_UPD_VALUE,   //!< entity update characteristic value handle
    AMS_HDL_ENTITY_UPD_CCCD,    //!< entity update characteristic CCCD handle
    AMS_HDL_ENTITY_ATTR_VALUE,  //!< entity attribute characteristic value handle
    AMS_HDL_CACHE_LEN,          //!< handle cache length
} T_AMS_HANDLE_TYPE;


/** @brief AMS client discovery state*/
typedef enum
{
    AMS_DISC_IDLE,
    AMS_DISC_START,
    AMS_DISC_DONE,
    AMS_DISC_FAILED
} T_AMS_DISC_STATE;

/** @brief AMS client write type*/
typedef enum
{
    AMS_WRITE_REMOTE_CMD_VALUE,
    AMS_WRITE_REMOTE_CMD_NOTIFY_ENABLE,
    AMS_WRITE_REMOTE_CMD_NOTIFY_DISABLE,
    AMS_WRITE_ENTITY_UPD_VALUE,
    AMS_WRITE_ENTITY_UPD_NOTIFY_ENABLE,
    AMS_WRITE_ENTITY_UPD_NOTIFY_DISABLE,
    AMS_WRITE_ENTITY_ATTR_VALUE
} T_AMS_WRTIE_TYPE;

/** @brief AMS client write result*/
typedef struct
{
    T_AMS_WRTIE_TYPE type;
    uint16_t cause;
} T_AMS_WRITE_RESULT;

/** @brief AMS client data type*/
typedef enum
{
    AMS_NOTIFY_FROM_REMOTE_CMD,
    AMS_NOTIFY_FROM_ENTITY_UPD,
    AMS_NOTIFY_FROM_ENTITY_ATTR,
} T_AMS_NOTIFY_DATA_TYPE;

/** @brief AMS client data type*/
typedef enum
{
    AMS_READ_FROM_ENTITY_UPD,
} T_AMS_READ_DATA_TYPE;


/** @brief AMS client notification data*/
typedef struct
{
    T_AMS_READ_DATA_TYPE type;
    uint16_t cause;
    uint16_t value_size;
    uint8_t *p_value;
} T_AMS_READ_DATA;

/** @brief AMS client notification data*/
typedef struct
{
    T_AMS_NOTIFY_DATA_TYPE type;
    uint16_t value_size;
    uint8_t *p_value;
} T_AMS_NOTIFY_DATA;


/** @brief AMS client callback content*/
typedef union
{
    T_AMS_DISC_STATE disc_state;
    T_AMS_READ_DATA read_data;
    T_AMS_WRITE_RESULT write_result;
    T_AMS_NOTIFY_DATA notify_data;
} T_AMS_CB_CONTENT;

/** @brief AMS client callback type*/
typedef enum
{
    AMS_CLIENT_CB_TYPE_DISC_STATE,          //!< Discovery procedure state
    AMS_CLIENT_CB_TYPE_READ_RESULT,         //!< Read request result
    AMS_CLIENT_CB_TYPE_WRITE_RESULT,        //!< Write request result, success or fail
    AMS_CLIENT_CB_TYPE_NOTIF_IND_RESULT,    //!< Notification or indication data received from server
    AMS_CLIENT_CB_TYPE_DISCONNECT_INFO,     //!< LE link disconnect
    AMS_CLIENT_CB_TYPE_INVALID              //!< Invalid callback type, no practical usage
} T_AMS_CB_TYPE;

/** @brief AMS client callback data*/
typedef struct
{
    T_AMS_CB_TYPE       cb_type;
    T_AMS_CB_CONTENT    cb_content;
} T_AMS_CB_DATA;

/** @defgroup AMS_CLIENT_Exported_Functions AMS Client Exported Functions
  * @brief
  * @{
  */

/**
 * @brief       Add ams client.
 *
 * @param[in]   app_cb  Callbackto notify client read/write/notify/indicate events.
 * @param[in]   link_num Initialize link number
 * @return Client ID of the specific client module.
 * @retval 0xff failed.
 * @retval other success.
 *
 * <b>Example usage</b>
 * \code{.c}
    void ams_init(uint8_t link_num)
    {
        ams_client = ams_add_client(ams_client_cb, link_num);
    }
 * \endcode
 */
T_CLIENT_ID ams_add_client(P_FUN_GENERAL_APP_CB app_cb, uint8_t link_num);


/**
  * @brief  Used by application, to start the discovery procedure of AMS.
  * @param[in]  conn_id connection ID.
  * @retval true  send request to upper stack success.
  * @retval false  send request to upper stack failed.
  *
  * <b>Example usage</b>
  * \code{.c}
    static T_USER_CMD_PARSE_RESULT test(T_USER_CMD_PARSED_VALUE *p_parse_value)
    {
        uint8_t conn_id = p_parse_value->dw_param[0];
        bool ret = ams_start_discovery(conn_id);
    }
  * \endcode
  */
bool ams_start_discovery(uint8_t conn_id);

/**
  * @brief  Used by application, to set the notifcation flag of remote command.
  * @param[in]  conn_id connection ID.
  * @param[in]  subscribe  value to enable or disable notify.
  * @retval true send request to upper stack success.
  * @retval false send request to upper stack failed.
  */
bool ams_subscribe_remote_cmd(uint8_t conn_id, bool subscribe);

/**
  * @brief  Used by application, to set the notifcation flag of entity update.
  * @param[in]  conn_id connection ID.
  * @param[in]  subscribe  value to enable or disable notify.
  * @retval true send request to upper stack success.
  * @retval false send request to upper stack failed.
  */
bool ams_subscribe_entity_upd(uint8_t conn_id, bool subscribe);

/**
  * @brief  Used by application, to send remote command.
  * @param[in]  conn_id connection ID.
  * @param[in]  cmd_id  Remote Command ID.
  * @retval true send request to upper stack success.
  * @retval false send request to upper stack failed.
  *
  * <b>Example usage</b>
  * \code{.c}
    static T_USER_CMD_PARSE_RESULT cmd_amscmd(T_USER_CMD_PARSED_VALUE *p_parse_value)
    {
        uint8_t conn_id = p_parse_value->dw_param[0];
        uint8_t cmd_id = p_parse_value->dw_param[1];
        bool ret = ams_write_remote_cmd(conn_id, (T_AMS_REMOTE_CMD_ID)cmd_id);
    }
  * \endcode
  */
bool ams_write_remote_cmd(uint8_t conn_id, T_AMS_REMOTE_CMD_ID cmd_id);

/**
  * @brief  Used by application, to write the Entity Update characteristic.
  *
  * The Entity Update characteristic is the characteristic by which an MR can inform the MS
  * which entity/attribute pairs it is interested in, and be informed about changes on these whenever they occur.
  * For example, the MR can be informed about the title of the currently loaded track, or the name of the active media app.
  *
  * @param[in]  conn_id connection ID.
  * @param[in]  p_value  entity/attribute pairs.
  * @param[in]  value_len  value length of p_value.
  * @retval true send request to upper stack success.
  * @retval false send request to upper stack failed.
  *
  * <b>Example usage</b>
  * \code{.c}
    static T_USER_CMD_PARSE_RESULT cmd_amswrite(T_USER_CMD_PARSED_VALUE *p_parse_value)
    {
        uint8_t conn_id = p_parse_value->dw_param[0];
        uint8_t type = p_parse_value->dw_param[1];
        bool ret = false;

        if (type == 0)
        {
            uint8_t cmd[3];
            cmd[0] = ENTITY_ID_TRACK;
            cmd[1] = TRACK_ATTR_ID_TITLE;
            cmd[2] = TRACK_ATTR_ID_DURATION;
            ret = ams_write_entity_upd_cmd(conn_id, cmd, 3);
        }
    }
  * \endcode
  */
bool ams_write_entity_upd_cmd(uint8_t conn_id, uint8_t *p_value, uint8_t value_len);

/**
  * @brief  Used by application, to write the Entity Attribute characteristic.
  *
  * This characteristic should ideally only be used if the value of an entity/attribute pair was marked as truncated
  * in the corresponding Entity Update notification, and the MR wants to display more of the associated value.
  *
  * @param[in]  conn_id connection ID.
  * @param[in]  entity_attr  Entity Attribute command.
  * @retval true send request to upper stack success.
  * @retval false send request to upper stack failed.
  *
  * <b>Example usage</b>
  * \code{.c}
    static T_USER_CMD_PARSE_RESULT cmd_amswrite(T_USER_CMD_PARSED_VALUE *p_parse_value)
    {
        uint8_t conn_id = p_parse_value->dw_param[0];
        uint8_t type = p_parse_value->dw_param[1];
        bool ret = false;

        if (type == 1)
        {
            T_AMS_ENTITY_ATTR entity;
            entity.entity_id = ENTITY_ID_TRACK;
            entity.attr_id.track_attr_id = TRACK_ATTR_ID_TITLE;
            ret = ams_write_entity_attr(conn_id, entity);
        }
    }
  * \endcode
  */
bool ams_write_entity_attr(uint8_t conn_id, T_AMS_ENTITY_ATTR entity_attr);

/**
  * @brief  Used by application, to read the Entity Attribute characteristic.
  *
  * This characteristic should ideally only be used if the value of an entity/attribute pair was marked as truncated
  * in the corresponding Entity Update notification, and the MR wants to display more of the associated value.
  *
  * @param[in]  conn_id connection ID.
  * @param[in]  entity_attr  Entity Attribute command.
  * @retval true send request to upper stack success.
  * @retval false send request to upper stack failed.
  *
  * <b>Example usage</b>
  * \code{.c}
    static T_USER_CMD_PARSE_RESULT cmd_amsread(T_USER_CMD_PARSED_VALUE *p_parse_value)
    {
        uint8_t conn_id = p_parse_value->dw_param[0];
        T_AMS_ENTITY_ATTR entity;
        bool ret;
        entity.entity_id = ENTITY_ID_TRACK;
        entity.attr_id.track_attr_id = TRACK_ATTR_ID_TITLE;
        ret = ams_read_entity_attr(conn_id, entity);
    }
  * \endcode
  */
bool ams_read_entity_attr(uint8_t conn_id, T_AMS_ENTITY_ATTR entity_attr);


/**
  * @brief  Get the handle for gatt characteristic
  * @param[in]  conn_id          Connection ID.
  * @param[in]  handle_type      Pre-defined Handle Type.
  * @retval the handle value of a GATT characteristic.
  */
uint16_t ams_search_handle(uint8_t conn_id, T_AMS_HANDLE_TYPE handle_type);


/** @} End of AMS_CLIENT_Exported_Functions */

/** @} End of AMS_CLIENT */


#ifdef  __cplusplus
}
#endif

#endif
