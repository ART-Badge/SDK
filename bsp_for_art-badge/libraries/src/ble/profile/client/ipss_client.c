/**
*****************************************************************************************
*     Copyright(c) 2017, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file     ipss_client.c
  * @brief    Source file for client of internet protocol support service.
  * @details  Global data and function implement.
  * @author   Jeff_Zheng
  * @date     2017-12-05
  * @version  v1.0
  * *************************************************************************************
  */
#include "trace.h"
#include "os_mem.h"
#include "profile_client.h"
#include "ipss.h"


static T_CLIENT_ID ipss_cl_id = CLIENT_PROFILE_GENERAL_ID;
//static P_FUN_GENERAL_APP_CB ipss_cl_gen_cb = NULL;

/**
  * @brief discover IPS service by service uuid .
  *
  * @param[in] conn_id  connection id.
  * @return Operation result.
  * @retval true Operation success.
  * @retval false Operation failure.
  */
bool ipss_find_srv_dcl(uint8_t conn_id)
{
    if (client_by_uuid_srv_discovery(conn_id, ipss_cl_id, GATT_UUID_IPSS) == GAP_CAUSE_SUCCESS)
    {
        return true;
    }
    return false;
}

/**
  * @brief IPS discover result callback.
  *
  * @param[in] conn_id  connection id.
  * @param[in] result_type result type @ref T_DISCOVERY_RESULT_TYPE
  * @param[in] result_data result data @ref T_DISCOVERY_RESULT_DATA
  * @return void
  */
static void ipss_discover_result_cb(uint8_t conn_id, T_DISCOVERY_RESULT_TYPE result_type,
                                    T_DISCOVERY_RESULT_DATA result_data)
{
    switch (result_type)
    {
    case DISC_RESULT_SRV_DATA:
        {
            uint16_t start_handle = result_data.p_srv_disc_data->att_handle;
            uint16_t end_handle = result_data.p_srv_disc_data->end_group_handle;
            APP_PRINT_INFO2("ipss_discover_result_cb: start handle %x, end handle %d", start_handle,
                            end_handle);
            break;
        }

    default:
        break;

    }
}

/**
  * @brief IPS service client callback function pointer.
  */
const T_FUN_CLIENT_CBS ipss_cl_cbs =
{
    NULL,                       //!< Discovery State callback function pointer
    ipss_discover_result_cb,    //!< Discovery result callback function pointer
    NULL,                       //!< Read response callback function pointer
    NULL,                       //!< Write result callback function pointer
    NULL,                       //!< Notify Indicate callback function pointer
    NULL                        //!< Link disconnection callback function pointer
};

/**
  * @brief add IPS service client.
  *
  * @param[in] p_func  pointer of app callback function called by client.
  * @return client id.
  */
uint8_t ipss_add_client(void *p_func)
{
    if (false == client_register_spec_client_cb(&ipss_cl_id, &ipss_cl_cbs))
    {
        ipss_cl_id = CLIENT_PROFILE_GENERAL_ID;
        PROFILE_PRINT_ERROR0("ipss_add_client: register fail");
    }
    else
    {
        //ipss_cl_gen_cb = (P_FUN_GENERAL_APP_CB)p_func;
    }

    PROFILE_PRINT_INFO1("ipss_add_client: client id %d", ipss_cl_id);

    return ipss_cl_id;
}

