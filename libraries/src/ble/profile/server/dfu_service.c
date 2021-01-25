#include <string.h>
#include "gatt.h"
#include "gap_conn_le.h"
#include "patch_header_check.h"
#include "rtl876x_wdg.h"
#include "flash_device.h"
#include "dfu_flash.h"
#include "dfu_api.h"
#include "dfu_service.h"
#include "otp.h"
#include "trace.h"
#include "board.h"

/*============================================================================*
 *                              Macros
 *============================================================================*/

//#define CAL_OFFSET(type, member) ((size_t)(&((type *)0)->member))

/*============================================================================*
 *                              External Variables
 *============================================================================*/
T_DFU_PARA g_dfu_para;
uint8_t *p_ota_temp_buffer_head;
uint16_t g_ota_tmp_buf_used_size;

/*============================================================================*
 *                              Local Variables
 *============================================================================*/
uint8_t ota_temp_buffer_head[DFU_TEMP_BUFFER_SIZE];
static bool buffer_check_en = false;
static uint32_t dfu_resend_offset = 0;
static T_SERVER_ID dfu_service_id;

P_FUN_SERVER_GENERAL_CB pfn_dfu_service_cb = NULL;

const uint8_t   SILENCE_GATT_UUID128_DFU_SERVICE[16] = {GATT_UUID128_DFU_SERVICE};

T_ATTRIB_APPL gatt_dfu_service_table[] =
{

    /*-------------------------- DFU Service ---------------------------*/
    /* <<Primary Service>>, .. */
    {
        (ATTRIB_FLAG_VOID | ATTRIB_FLAG_LE),           /* flags     */
        {
            LO_WORD(GATT_UUID_PRIMARY_SERVICE),
            HI_WORD(GATT_UUID_PRIMARY_SERVICE),              /* type_value */
        },
        UUID_128BIT_SIZE,                                    /* bValueLen     */
        (void *)SILENCE_GATT_UUID128_DFU_SERVICE,           /* p_value_context */
        GATT_PERM_READ                                      /* permissions  */
    },



    /* <<Characteristic>>, .. */
    {
        ATTRIB_FLAG_VALUE_INCL,                     /* flags */
        {                                           /* type_value */
            LO_WORD(GATT_UUID_CHARACTERISTIC),
            HI_WORD(GATT_UUID_CHARACTERISTIC),
            GATT_CHAR_PROP_WRITE_NO_RSP/* characteristic properties */
            /* characteristic UUID not needed here, is UUID of next attrib. */
        },
        1,                                          /* bValueLen */
        NULL,
        GATT_PERM_READ                              /* permissions */
    },
    /*--- DFU packet characteristic value ---*/
    {
        ATTRIB_FLAG_VALUE_APPL | ATTRIB_FLAG_UUID_128BIT,                              /* flags */
        {                                                           /* type_value */
            GATT_UUID128_DFU_DATA
        },
        0,                                                 /* bValueLen */
        NULL,
#if DFU_SERVER_REQUIRE_AUTH
        GATT_PERM_WRITE_AUTHEN_MITM_REQ                             /* permissions */
#else
        GATT_PERM_WRITE           /* permissions */
#endif
    },
    /* <<Characteristic>>, .. */
    {
        ATTRIB_FLAG_VALUE_INCL,                     /* flags */
        {                                           /* type_value */
            LO_WORD(GATT_UUID_CHARACTERISTIC),
            HI_WORD(GATT_UUID_CHARACTERISTIC),
            (GATT_CHAR_PROP_WRITE |                   /* characteristic properties */
             GATT_CHAR_PROP_NOTIFY)
            /* characteristic UUID not needed here, is UUID of next attrib. */
        },
        1,                                          /* bValueLen */
        NULL,
        GATT_PERM_READ                              /* permissions */
    },
    /*--- DFU Control Point value ---*/
    {
        ATTRIB_FLAG_VALUE_APPL | ATTRIB_FLAG_UUID_128BIT,                              /* flags */
        {                                                           /* type_value */
            GATT_UUID128_DFU_CONTROL_POINT
        },
        0,                                                 /* bValueLen */
        NULL,
#if DFU_SERVER_REQUIRE_AUTH
        GATT_PERM_WRITE_AUTHEN_MITM_REQ                             /* permissions */
#else
        GATT_PERM_WRITE           /* permissions */
#endif
    },
    /* client characteristic configuration */
    {
        (ATTRIB_FLAG_VALUE_INCL |                   /* flags */
         ATTRIB_FLAG_CCCD_APPL),
        {                                           /* type_value */
            LO_WORD(GATT_UUID_CHAR_CLIENT_CONFIG),
            HI_WORD(GATT_UUID_CHAR_CLIENT_CONFIG),
            /* NOTE: this value has an instantiation for each client, a write to */
            /* this attribute does not modify this default value:                */
            LO_WORD(GATT_CLIENT_CHAR_CONFIG_DEFAULT), /* client char. config. bit field */
            HI_WORD(GATT_CLIENT_CHAR_CONFIG_DEFAULT)
        },
        2,                                          /* bValueLen */
        NULL,
#if DFU_SERVER_REQUIRE_AUTH
        (GATT_PERM_READ | GATT_PERM_WRITE_AUTHEN_MITM_REQ)          /* permissions */
#else
        (GATT_PERM_READ | GATT_PERM_WRITE)          /* permissions */
#endif
    }

};


/*============================================================================*
 *                              Local Functions
 *============================================================================*/
/**
 * @brief dfu_buffer_check_process
 *
 * @param buffer_check_size     size for buffer check.
 * @param buffer_crc            calced buffer crc value.
 * @return None
*/
void dfu_buffer_check_process(uint8_t conn_id, uint16_t buffer_check_size, uint16_t buffer_crc)
{
    uint8_t notif_data[DFU_NOTIFY_LENGTH_REPORT_BUFFER_CRC] = {0};
    notif_data[0] = DFU_OPCODE_NOTIFICATION;
    notif_data[1] = DFU_OPCODE_REPORT_BUFFER_CRC;

    T_DFU_CALLBACK_DATA callback_data;
    callback_data.conn_id = conn_id;
    callback_data.msg_type = SERVICE_CALLBACK_TYPE_WRITE_CHAR_VALUE;
    callback_data.msg_data.write.write_attrib_index = INDEX_DFU_CONTROL_POINT_CHAR_VALUE;

    if (buffer_check_size > DFU_TEMP_BUFFER_SIZE)
    {
        //invalid para
        DFU_PRINT_ERROR3("<==dfu_buffer_check_process: invalid buffer_check_size=%d(>%d), cur_offset=%d",
                         buffer_check_size, DFU_TEMP_BUFFER_SIZE, g_dfu_para.cur_offset);
        g_ota_tmp_buf_used_size = 0;
        notif_data[2] = DFU_ARV_FAIL_INVALID_PARAMETER;
        LE_UINT32_TO_ARRAY(&notif_data[3], g_dfu_para.cur_offset);
        server_send_data(conn_id, dfu_service_id, INDEX_DFU_CONTROL_POINT_CHAR_VALUE, \
                         notif_data, DFU_NOTIFY_LENGTH_REPORT_BUFFER_CRC, GATT_PDU_TYPE_NOTIFICATION);
        return;
    }

    if (g_ota_tmp_buf_used_size == buffer_check_size ||
        g_dfu_para.cur_offset + g_ota_tmp_buf_used_size == g_dfu_para.image_total_length)
    {
        if (dfu_check_buf_crc(p_ota_temp_buffer_head, g_ota_tmp_buf_used_size, buffer_crc))
        {
            //crc error
            DFU_PRINT_ERROR1("<==dfu_buffer_check_process: Buf CRC Error! cur_offset=%d",
                             g_dfu_para.cur_offset);
            g_ota_tmp_buf_used_size = 0;
            notif_data[2] = DFU_ARV_FAIL_CRC_ERROR;
            LE_UINT32_TO_ARRAY(&notif_data[3], g_dfu_para.cur_offset);
            server_send_data(conn_id, dfu_service_id, INDEX_DFU_CONTROL_POINT_CHAR_VALUE, \
                             notif_data, DFU_NOTIFY_LENGTH_REPORT_BUFFER_CRC, GATT_PDU_TYPE_NOTIFICATION);
            return;
        }

        else //crc ok
        {
            //1. devrypt data
            if (OTP->ota_with_encryption_data)
            {
                dfu_hw_aes_decrypt_image(p_ota_temp_buffer_head, p_ota_temp_buffer_head, g_ota_tmp_buf_used_size);
            }
            //2. write flash
            uint32_t result = dfu_update(g_dfu_para.ctrl_header.image_id, g_dfu_para.cur_offset,
                                         g_ota_tmp_buf_used_size, (uint32_t *)p_ota_temp_buffer_head);

            if (result == 0)
            {
                uint32_t updated_success_len = g_ota_tmp_buf_used_size;
                callback_data.msg_data.write.opcode = DFU_WRITE_DOING;
                callback_data.msg_data.write.length = 4;
                callback_data.msg_data.write.p_value = (uint8_t *)&updated_success_len;
                if (pfn_dfu_service_cb)
                {
                    T_APP_RESULT w_cause = pfn_dfu_service_cb(dfu_service_id, (void *)&callback_data);
                    if (w_cause != APP_RESULT_SUCCESS)
                    {
                        return;
                    }
                }

                g_dfu_para.cur_offset += g_ota_tmp_buf_used_size;

                if ((g_dfu_para.cur_offset - dfu_resend_offset) >= FMC_SEC_SECTION_LEN)
                {
                    dfu_resend_offset += FMC_SEC_SECTION_LEN;
                }
                g_ota_tmp_buf_used_size = 0;
                DFU_PRINT_INFO2("<==dfu_buffer_check_process: dfu_update Success! cur_offset=%d, dfu_resend_offset=%d",
                                g_dfu_para.cur_offset, dfu_resend_offset);
                notif_data[2] = DFU_ARV_SUCCESS; //valid
                LE_UINT32_TO_ARRAY(&notif_data[3], g_dfu_para.cur_offset);
                server_send_data(conn_id, dfu_service_id, INDEX_DFU_CONTROL_POINT_CHAR_VALUE, \
                                 notif_data, DFU_NOTIFY_LENGTH_REPORT_BUFFER_CRC, GATT_PDU_TYPE_NOTIFICATION);
                return;
            }
            else
            {
                DFU_PRINT_ERROR1("<==dfu_buffer_check_process: dfu_update Fail result=%d", result);

                result = dfu_flash_erase_sector_with_retry(g_dfu_para.ctrl_header.image_id, dfu_resend_offset);
                if (result)
                {
                    //erase fail
                    g_ota_tmp_buf_used_size = 0;
                    g_dfu_para.cur_offset = dfu_resend_offset;
                    DFU_PRINT_ERROR1("<==dfu_buffer_check_process: erase flash fail 3 times! cur_offset=%d",
                                     g_dfu_para.cur_offset);

                    notif_data[2] = DFU_ARV_FAIL_ERASE_ERROR;
                    LE_UINT32_TO_ARRAY(&notif_data[3], g_dfu_para.cur_offset);
                    server_send_data(conn_id, dfu_service_id, INDEX_DFU_CONTROL_POINT_CHAR_VALUE, \
                                     notif_data, DFU_NOTIFY_LENGTH_REPORT_BUFFER_CRC, GATT_PDU_TYPE_NOTIFICATION);
                    return;
                }

                if ((g_dfu_para.cur_offset - dfu_resend_offset) > FMC_SEC_SECTION_LEN) //need erase two sector
                {
                    DFU_PRINT_INFO0("<==dfu_buffer_check_process:Need erase two sectors");
                    result = dfu_flash_erase_sector_with_retry(g_dfu_para.ctrl_header.image_id,
                                                               dfu_resend_offset + FMC_SEC_SECTION_LEN);
                    if (result)
                    {
                        //erase fail
                        g_ota_tmp_buf_used_size = 0;
                        g_dfu_para.cur_offset =  dfu_resend_offset;
                        DFU_PRINT_ERROR1("<==dfu_buffer_check_process: erase flash fail 3 times! cur_offset=%d",
                                         g_dfu_para.cur_offset);

                        notif_data[2] = DFU_ARV_FAIL_ERASE_ERROR;
                        LE_UINT32_TO_ARRAY(&notif_data[3], g_dfu_para.cur_offset);
                        server_send_data(conn_id, dfu_service_id, INDEX_DFU_CONTROL_POINT_CHAR_VALUE, \
                                         notif_data, DFU_NOTIFY_LENGTH_REPORT_BUFFER_CRC, GATT_PDU_TYPE_NOTIFICATION);
                        return;
                    }

                }
                //erase ok
                g_ota_tmp_buf_used_size = 0;
                g_dfu_para.cur_offset =  dfu_resend_offset;
                DFU_PRINT_INFO1("<==dfu_buffer_check_process: erase ok! cur_offset=%d", g_dfu_para.cur_offset);
                notif_data[2] = DFU_ARV_FAIL_PROG_ERROR;
                LE_UINT32_TO_ARRAY(&notif_data[3], g_dfu_para.cur_offset);
                server_send_data(conn_id, dfu_service_id, INDEX_DFU_CONTROL_POINT_CHAR_VALUE, \
                                 notif_data, DFU_NOTIFY_LENGTH_REPORT_BUFFER_CRC, GATT_PDU_TYPE_NOTIFICATION);
                return;
            }
        }
    }
    else
    {
        DFU_PRINT_ERROR4("<==dfu_buffer_check_process: Error buffer_check_size=%d,buf_used_size=%d,cur_offset=%d,image_total_length=%d",
                         buffer_check_size, g_ota_tmp_buf_used_size,
                         g_dfu_para.cur_offset, g_dfu_para.image_total_length);
        //flush buffer.
        g_ota_tmp_buf_used_size = 0;
        notif_data[2] = DFU_ARV_FAIL_LENGTH_ERROR;
        LE_UINT32_TO_ARRAY(&notif_data[3], g_dfu_para.cur_offset);
        server_send_data(conn_id, dfu_service_id, INDEX_DFU_CONTROL_POINT_CHAR_VALUE, \
                         notif_data, DFU_NOTIFY_LENGTH_REPORT_BUFFER_CRC, GATT_PDU_TYPE_NOTIFICATION);
        return;
    }
}

void dfu_notify_conn_para_update_req(uint8_t conn_id, T_DFU_ARV_ERROR_CODE error_code)
{
    if (g_dfu_para.dfu_conn_para_update_in_progress == true)
    {
        g_dfu_para.dfu_conn_para_update_in_progress = false;
        uint8_t notif_data[3] = {0};
        notif_data[0] = DFU_OPCODE_NOTIFICATION;
        notif_data[1] = DFU_OPCODE_CONN_PARA_TO_UPDATE_REQ;
        notif_data[2] = error_code;

        DFU_PRINT_INFO1("<==dfu_notify_conn_para_update_req error_code=0x%x", error_code);
        /* Connection Param Update rejected, we should notify the fail result to remote device. */
        server_send_data(conn_id, dfu_service_id, INDEX_DFU_CONTROL_POINT_CHAR_VALUE, \
                         notif_data, DFU_NOTIFY_LENGTH_CONN_PARA_UPDATE_REQ, GATT_PDU_TYPE_NOTIFICATION);
    }
}

void dfu_service_handle_valid_fw(uint8_t conn_id)
{
    uint8_t notif_data[DFU_NOTIFY_LENGTH_VALID_FW] = {0};

    bool check_result = dfu_check_checksum(g_dfu_para.ctrl_header.image_id);
    DFU_PRINT_INFO1("DFU_OPCODE_VALID_FW: check_result=%d (1: Success, 0: Fail)", check_result);

    if (check_result)
    {
        notif_data[2] = DFU_ARV_SUCCESS;
    }
    else
    {
        notif_data[2] = DFU_ARV_FAIL_CRC_ERROR;
    }
    notif_data[0] = DFU_OPCODE_NOTIFICATION;
    notif_data[1] = DFU_OPCODE_VALID_FW;
    server_send_data(conn_id, dfu_service_id, INDEX_DFU_CONTROL_POINT_CHAR_VALUE, \
                     notif_data, DFU_NOTIFY_LENGTH_VALID_FW, GATT_PDU_TYPE_NOTIFICATION);
}

/**
 * @brief dfu_service_handle_control_point_req
 *
 * @param length     control point cmd length.
 * @param p_value    control point cmd address..
 * @return None
*/
void dfu_service_handle_control_point_req(uint8_t conn_id, uint16_t length,
                                          uint8_t *p_value)
{
    T_APP_RESULT w_cause = APP_RESULT_SUCCESS;
    T_DFU_CTRL_POINT dfu_control_point;
    uint8_t *p = p_value + 1;
    uint8_t notif_data[DFU_NOTIFY_LENGTH_MAX] = {0};

    T_DFU_CALLBACK_DATA callback_data;
    callback_data.conn_id = conn_id;
    callback_data.msg_type = SERVICE_CALLBACK_TYPE_WRITE_CHAR_VALUE;
    callback_data.msg_data.write.write_attrib_index = INDEX_DFU_CONTROL_POINT_CHAR_VALUE;

    dfu_control_point.opcode = *p_value;
    DFU_PRINT_TRACE2("==>dfu_service_handle_control_point_req: opcode=0x%x, length=%d",
                     dfu_control_point.opcode, length);

    if (dfu_control_point.opcode >= DFU_OPCODE_MAX || dfu_control_point.opcode <= DFU_OPCODE_MIN)
    {
        notif_data[0] = DFU_OPCODE_NOTIFICATION;
        notif_data[1] = dfu_control_point.opcode;
        notif_data[2] = 0xff;
        server_send_data(conn_id, dfu_service_id, INDEX_DFU_CONTROL_POINT_CHAR_VALUE, \
                         notif_data, DFU_NOTIFY_LENGTH_START_DFU, GATT_PDU_TYPE_NOTIFICATION);
        return;
    }

    switch (dfu_control_point.opcode)
    {
    case DFU_OPCODE_START_DFU: //0x01
        if (length == DFU_LENGTH_START_DFU)/*4 bytes is padding for encrypt*/
        {
            if (OTP->ota_with_encryption_data)
            {
                DFU_PRINT_INFO1("ctrl header before decryped=%b", TRACE_BINARY(16, p));
                dfu_hw_aes_decrypt_image(p, p, 16);
                DFU_PRINT_INFO1("ctrl header after decryped=%b", TRACE_BINARY(16, p));
            }

            dfu_control_point.start_dfu.ic_type = (*p);
            p += 1;
            dfu_control_point.start_dfu.secure_version = (*p);
            p += 1;
            LE_ARRAY_TO_UINT16(dfu_control_point.start_dfu.ctrl_flag.value, p);
            p += 2;
            LE_ARRAY_TO_UINT16(dfu_control_point.start_dfu.image_id, p);
            p += 2;
            LE_ARRAY_TO_UINT16(dfu_control_point.start_dfu.crc16, p);
            p += 2;
            LE_ARRAY_TO_UINT32(dfu_control_point.start_dfu.payload_len, p);

            DFU_PRINT_INFO6("DFU_OPCODE_START_DFU: ic_type=0x%x, secure_version=0x%x, ctrl_flag.value=0x%x, image_id=0x%x,crc16=0x%x, payload_len=%d",
                            dfu_control_point.start_dfu.ic_type,
                            dfu_control_point.start_dfu.secure_version,
                            dfu_control_point.start_dfu.ctrl_flag.value,
                            dfu_control_point.start_dfu.image_id,
                            dfu_control_point.start_dfu.crc16,
                            dfu_control_point.start_dfu.payload_len
                           );
            g_dfu_para.ctrl_header.ic_type = dfu_control_point.start_dfu.ic_type;
            g_dfu_para.ctrl_header.ctrl_flag.value = dfu_control_point.start_dfu.ctrl_flag.value;
            g_dfu_para.ctrl_header.image_id = dfu_control_point.start_dfu.image_id;
            g_dfu_para.ctrl_header.crc16 = dfu_control_point.start_dfu.crc16;
            g_dfu_para.ctrl_header.payload_len = dfu_control_point.start_dfu.payload_len;
            g_dfu_para.image_total_length = g_dfu_para.ctrl_header.payload_len + IMG_HEADER_SIZE;

            /*check if start dfu fileds are vaild*/
            if (g_dfu_para.ctrl_header.ic_type == DEFINED_IC_TYPE)
            {
                if (((g_dfu_para.ctrl_header.image_id >= OTA) && (g_dfu_para.ctrl_header.image_id < IMAGE_MAX))
                    || (g_dfu_para.ctrl_header.image_id == IMAGE_USER_DATA))
                {
                    callback_data.msg_data.write.opcode = DFU_WRITE_START;
                    callback_data.msg_data.write.length = DFU_HEADER_SIZE;
                    callback_data.msg_data.write.p_value = (uint8_t *)&g_dfu_para.ctrl_header;
                    if (pfn_dfu_service_cb)
                    {
                        w_cause = pfn_dfu_service_cb(dfu_service_id, (void *)&callback_data);
                        if (w_cause != APP_RESULT_SUCCESS)
                        {
                            return;
                        }
                    }

                    uint32_t result = dfu_update(g_dfu_para.ctrl_header.image_id, 0, DFU_HEADER_SIZE,
                                                 (uint32_t *)&g_dfu_para.ctrl_header);
                    if (result)
                    {
                        DBG_DIRECT("DFU_OPCODE_START_DFU: dfu_update fail=%d", result);
                        T_DFU_FAIL_REASON dfu_fail_reason = DFU_FAIL_UPDATE_FLASH;
                        callback_data.msg_data.write.opcode = DFU_WRITE_FAIL;
                        callback_data.msg_data.write.length = sizeof(T_DFU_FAIL_REASON);
                        callback_data.msg_data.write.p_value = (uint8_t *)&dfu_fail_reason;
                        if (pfn_dfu_service_cb)
                        {
                            w_cause = pfn_dfu_service_cb(dfu_service_id, (void *)&callback_data);
                            if (w_cause != APP_RESULT_SUCCESS)
                            {
                                return;
                            }
                        }

                        dfu_fw_reboot(false);
                    }
                    else
                    {
                        g_dfu_para.cur_offset += DFU_HEADER_SIZE;
                        DFU_PRINT_INFO0("DFU_OPCODE_START_DFU: start success!");

                        uint32_t updated_success_len = DFU_HEADER_SIZE;
                        callback_data.msg_data.write.opcode = DFU_WRITE_DOING;
                        callback_data.msg_data.write.length = 4;
                        callback_data.msg_data.write.p_value = (uint8_t *)&updated_success_len;
                        if (pfn_dfu_service_cb)
                        {
                            w_cause = pfn_dfu_service_cb(dfu_service_id, (void *)&callback_data);
                            if (w_cause != APP_RESULT_SUCCESS)
                            {
                                return;
                            }
                        }

                        notif_data[0] = DFU_OPCODE_NOTIFICATION;
                        notif_data[1] = DFU_OPCODE_START_DFU;
                        notif_data[2] = DFU_ARV_SUCCESS;
                        server_send_data(conn_id, dfu_service_id, INDEX_DFU_CONTROL_POINT_CHAR_VALUE, \
                                         notif_data, DFU_NOTIFY_LENGTH_START_DFU, GATT_PDU_TYPE_NOTIFICATION);
                    }


                }
                else
                {
                    DFU_PRINT_ERROR1("DFU_OPCODE_START_DFU: image id=0x%x Error!", g_dfu_para.ctrl_header.image_id);
                    notif_data[0] = DFU_OPCODE_NOTIFICATION;
                    notif_data[1] = DFU_OPCODE_START_DFU;
                    notif_data[2] = DFU_ARV_FAIL_INVALID_PARAMETER;
                    server_send_data(conn_id, dfu_service_id, INDEX_DFU_CONTROL_POINT_CHAR_VALUE, \
                                     notif_data, DFU_NOTIFY_LENGTH_START_DFU, GATT_PDU_TYPE_NOTIFICATION);
                    return;
                }
            }
            else
            {
                DFU_PRINT_ERROR1("DFU_OPCODE_START_DFU: ic type=0x%x Error!", g_dfu_para.ctrl_header.ic_type);
                notif_data[0] = DFU_OPCODE_NOTIFICATION;
                notif_data[1] = DFU_OPCODE_START_DFU;
                notif_data[2] = DFU_ARV_FAIL_INVALID_PARAMETER;
                server_send_data(conn_id, dfu_service_id, INDEX_DFU_CONTROL_POINT_CHAR_VALUE, \
                                 notif_data, DFU_NOTIFY_LENGTH_START_DFU, GATT_PDU_TYPE_NOTIFICATION);
                return;
            }
        }
        break;

    case DFU_OPCODE_RECEIVE_FW_IMAGE_INFO://0x02
        if (length == DFU_LENGTH_RECEIVE_FW_IMAGE_INFO)
        {
            LE_ARRAY_TO_UINT16(g_dfu_para.ctrl_header.image_id, p);
            p += 2;
            LE_ARRAY_TO_UINT32(g_dfu_para.cur_offset, p);
            if ((g_dfu_para.cur_offset == 0) || (g_dfu_para.cur_offset == DFU_HEADER_SIZE))
            {
                g_ota_tmp_buf_used_size = 0;
                dfu_resend_offset = 0;
            }
            DFU_PRINT_INFO3("DFU_OPCODE_RECEIVE_FW_IMAGE_INFO: image_id=0x%x, cur_offset=%d, g_ota_tmp_buf_used_size=%d",
                            g_dfu_para.ctrl_header.image_id, g_dfu_para.cur_offset, g_ota_tmp_buf_used_size);
        }
        else
        {
            DFU_PRINT_ERROR1("DFU_OPCODE_RECEIVE_FW_IMAGE_INFO: length=%d Error!", length);
        }
        break;

    case DFU_OPCODE_VALID_FW://0x03

        if (length == DFU_LENGTH_VALID_FW)
        {
            LE_ARRAY_TO_UINT16(g_dfu_para.ctrl_header.image_id, p);
            DFU_PRINT_TRACE1("DFU_OPCODE_VALID_FW: image_id=0x%x", g_dfu_para.ctrl_header.image_id);

            /*if ota large img, need modify wdg timeout period*/
            if (OTP->wdgEnableInRom && g_dfu_para.image_total_length > 0x100000)
            {
                /*1M and less---4s, 2M and less---8s,..., 8M and less---32s*/
                uint32_t img_align_len = ((g_dfu_para.image_total_length + (0x100000 - 1)) & (~(0x100000 - 1)));
                uint8_t wdg_period_4s = (img_align_len / 0x100000);
                uint16_t div_factor = (32 * wdg_period_4s - 1);
                DFU_PRINT_TRACE2("DFU_OPCODE_VALID_FW: Change WDG Period to %ds, div_factor=%d", wdg_period_4s << 2,
                                 div_factor);
                WDG_Config(div_factor, 15, RESET_ALL); //31 - 4s, 63 - 8s
            }
            /*It is not recommended to do things that take a long time in upperstack task cb.
                So dfu service cb need send msg to app task to handle checksum image if support silent ota.
                While app task must handle IO_MSG_TYPE_DFU_VALID_FW by calling dfu_service_handle_valid_fw.
            */
        }
        else
        {
            DFU_PRINT_ERROR1("DFU_OPCODE_VALID_FW: length=%d Error!", length);
        }
        break;

    case DFU_OPCODE_ACTIVE_IMAGE_RESET://0x04
        {
            /*notify bootloader to reset and use new image*/
            DFU_PRINT_INFO0("DFU_OPCODE_ACTIVE_IMAGE_RESET");
            callback_data.msg_data.write.opcode = DFU_WRITE_END;
            callback_data.msg_data.write.length = DFU_HEADER_SIZE;
            callback_data.msg_data.write.p_value = (uint8_t *)&g_dfu_para.ctrl_header;
            if (pfn_dfu_service_cb)
            {
                w_cause = pfn_dfu_service_cb(dfu_service_id, (void *)&callback_data);
                if (w_cause != APP_RESULT_SUCCESS)
                {
                    return;
                }
            }

            if (!is_ota_support_bank_switch())
            {
                /*note: must unlock flash bp for ota copy before reset when not support bank switch*/
                unlock_flash_bp_all();
            }
        }
        break;

    case DFU_OPCODE_SYSTEM_RESET://0x05
        {
            /*whatever cause ota fail, clinet will send this cmd to reset device*/
            DBG_DIRECT("DFU_OPCODE_SYSTEM_RESET");

            /*if select not active image by Phone even if image transport successful. Not for single bank user data*/
            if (g_dfu_para.ctrl_header.image_id >= OTA && g_dfu_para.ctrl_header.image_id < IMAGE_MAX)
            {
#ifdef SUPPORT_ALONE_UPPERSTACK_IMG
                uint32_t temp_addr = get_temp_ota_bank_addr_by_img_id_app((T_IMG_ID)
                                                                          g_dfu_para.ctrl_header.image_id);
#else
                uint32_t temp_addr = get_temp_ota_bank_addr_by_img_id((T_IMG_ID)g_dfu_para.ctrl_header.image_id);
#endif
                T_IMG_CTRL_HEADER_FORMAT *p_temp_header = (T_IMG_CTRL_HEADER_FORMAT *)temp_addr;
                if (p_temp_header && !p_temp_header->ctrl_flag.flag_value.not_ready)
                {
                    flash_erase_locked(FLASH_ERASE_SECTOR, temp_addr);
                }
            }

            T_DFU_FAIL_REASON dfu_fail_reason = DFU_FAIL_SYSTEM_RESET_CMD;
            callback_data.msg_data.write.length = sizeof(T_DFU_FAIL_REASON);
            callback_data.msg_data.write.p_value = (uint8_t *)&dfu_fail_reason;
            callback_data.msg_data.write.opcode = DFU_WRITE_FAIL;
            if (pfn_dfu_service_cb)
            {
                w_cause = pfn_dfu_service_cb(dfu_service_id, (void *)&callback_data);
                if (w_cause != APP_RESULT_SUCCESS)
                {
                    return;
                }
            }

            dfu_fw_reboot(false);
        }
        break;

    case DFU_OPCODE_REPORT_TARGET_INFO://0x06
        if (length == DFU_LENGTH_REPORT_TARGET_INFO)
        {
            LE_ARRAY_TO_UINT16(g_dfu_para.ctrl_header.image_id, p);
            dfu_report_target_fw_info(g_dfu_para.ctrl_header.image_id, &g_dfu_para.origin_image_version,
                                      (uint32_t *)&g_dfu_para.cur_offset);
            g_dfu_para.cur_offset = 0;
            DFU_PRINT_INFO3("DFU_OPCODE_REPORT_TARGET_INFO: image_id=0x%x,origin_image_ver=0x%x, cur_offset=%d",
                            g_dfu_para.ctrl_header.image_id, g_dfu_para.origin_image_version, g_dfu_para.cur_offset);

            notif_data[0] = DFU_OPCODE_NOTIFICATION;
            notif_data[1] = DFU_OPCODE_REPORT_TARGET_INFO;
            notif_data[2] = DFU_ARV_SUCCESS;

            LE_UINT32_TO_ARRAY(&notif_data[3], g_dfu_para.origin_image_version);
            LE_UINT32_TO_ARRAY(&notif_data[7], g_dfu_para.cur_offset);
            server_send_data(conn_id, dfu_service_id, INDEX_DFU_CONTROL_POINT_CHAR_VALUE, \
                             notif_data, DFU_NOTIFY_LENGTH_REPORT_TARGET_INFO, GATT_PDU_TYPE_NOTIFICATION);
        }
        else
        {
            DFU_PRINT_ERROR1("DFU_OPCODE_REPORT_TARGET_INFO: length=%d Error!", length);
        }
        break;

    case DFU_OPCODE_CONN_PARA_TO_UPDATE_REQ://0x07
        {
            notif_data[0] = DFU_OPCODE_NOTIFICATION;
            notif_data[1] = DFU_OPCODE_CONN_PARA_TO_UPDATE_REQ;

            if (length  == DFU_LENGTH_CONN_PARA_TO_UPDATE_REQ)
            {
                if (g_dfu_para.dfu_conn_para_update_in_progress)
                {
                    DFU_PRINT_ERROR0("DFU_OPCODE_CONN_PARA_TO_UPDATE_REQ: OTA ConnParaUpdInProgress!");
                    notif_data[2] = DFU_ARV_FAIL_OPERATION;
                    server_send_data(conn_id, dfu_service_id, INDEX_DFU_CONTROL_POINT_CHAR_VALUE, \
                                     notif_data, DFU_NOTIFY_LENGTH_ARV, GATT_PDU_TYPE_NOTIFICATION);
                }
                else
                {
                    uint16_t conn_interval_min;
                    uint16_t conn_interval_max;
                    uint16_t conn_latency;
                    uint16_t superv_tout;

                    LE_ARRAY_TO_UINT16(conn_interval_min, p_value + 1);
                    LE_ARRAY_TO_UINT16(conn_interval_max, p_value + 3);
                    LE_ARRAY_TO_UINT16(conn_latency, p_value + 5);
                    LE_ARRAY_TO_UINT16(superv_tout, p_value + 7);

                    if (le_update_conn_param(0, conn_interval_min, conn_interval_max, conn_latency,
                                             superv_tout, conn_interval_min * 2 - 2, conn_interval_max * 2 - 2) == GAP_CAUSE_SUCCESS)
                    {
                        /* Connection Parameter Update Request sent successfully, means this procedure is in progress. */
                        g_dfu_para.dfu_conn_para_update_in_progress = true;
                        DFU_PRINT_INFO4("DFU_OPCODE_CONN_PARA_TO_UPDATE_REQ: conn_min=0x%x, conn_max=0x%x, latcy=0x%x, timeout=0x%x",
                                        conn_interval_min, conn_interval_max, conn_latency, superv_tout);
                    }
                    else
                    {
                        notif_data[2] = DFU_ARV_FAIL_OPERATION;
                        server_send_data(conn_id, dfu_service_id, INDEX_DFU_CONTROL_POINT_CHAR_VALUE, \
                                         notif_data, DFU_NOTIFY_LENGTH_ARV, GATT_PDU_TYPE_NOTIFICATION);
                    }
                }
            }
            else
            {
                /*TODO: to be masked.*/
                DFU_PRINT_ERROR1("DFU_OPCODE_CONN_PARA_TO_UPDATE_REQ: length=%d Error!", length);
                notif_data[2] = DFU_ARV_FAIL_INVALID_PARAMETER;
                server_send_data(conn_id, dfu_service_id, INDEX_DFU_CONTROL_POINT_CHAR_VALUE, \
                                 notif_data, DFU_NOTIFY_LENGTH_ARV, GATT_PDU_TYPE_NOTIFICATION);
            }
        }
        break;

    case DFU_OPCODE_BUFFER_CHECK_EN: //0x09
        {
            le_get_conn_param(GAP_PARAM_CONN_MTU_SIZE, &g_dfu_para.mtu_size, conn_id);
            DFU_PRINT_TRACE2("DFU_OPCODE_BUFFER_CHECK_EN: mtu_size=%d, max_bufffer_size=%d",
                             g_dfu_para.mtu_size,
                             DFU_TEMP_BUFFER_SIZE);

            notif_data[0] = DFU_OPCODE_NOTIFICATION;
            notif_data[1] = DFU_OPCODE_BUFFER_CHECK_EN;
#if (DFU_BUFFER_CHECK_ENABLE == 1)
            buffer_check_en = true;
            notif_data[2] = DFU_ARV_SUCCESS;
#else
            buffer_check_en = false;
            notif_data[2] = DFU_ARV_FAIL_OPERATION;
#endif
            LE_UINT16_TO_ARRAY(&notif_data[3], DFU_TEMP_BUFFER_SIZE);
            LE_UINT16_TO_ARRAY(&notif_data[5], g_dfu_para.mtu_size);
            server_send_data(conn_id, dfu_service_id, INDEX_DFU_CONTROL_POINT_CHAR_VALUE, \
                             notif_data, DFU_NOTIFY_LENGTH_BUFFER_CHECK_EN, GATT_PDU_TYPE_NOTIFICATION);
        }
        break;

    case DFU_OPCODE_REPORT_BUFFER_CRC:        //0x0a
        {
            uint16_t mBufSize;
            uint16_t mCrcVal;
            LE_ARRAY_TO_UINT16(mBufSize, p);
            p += 2;
            LE_ARRAY_TO_UINT16(mCrcVal, p);
            DFU_PRINT_INFO2("DFU_OPCODE_REPORT_BUFFER_CRC: mBufSize=0x%x, mCrcVal=0x%x", mBufSize, mCrcVal);
            dfu_buffer_check_process(conn_id, mBufSize, mCrcVal);
        }
        break;

    case DFU_OPCODE_RECEIVE_IC_TYPE://0x0b
        {
            uint8_t ic_type = 0; //0 means invalid ic type
            notif_data[0] = DFU_OPCODE_NOTIFICATION;
            notif_data[1] = DFU_OPCODE_RECEIVE_IC_TYPE;
            if (dfu_report_target_ic_type(OTA, &ic_type))
            {
                notif_data[2] = DFU_ARV_FAIL_INVALID_PARAMETER;
                notif_data[3] =  ic_type;
            }
            else
            {
                notif_data[2] = DFU_ARV_SUCCESS;
                notif_data[3] =  ic_type;
            }
            DFU_PRINT_INFO1("DFU_OPCODE_RECEIVE_IC_TYPE: ic_type=0x%x", ic_type);
            server_send_data(conn_id, dfu_service_id, INDEX_DFU_CONTROL_POINT_CHAR_VALUE, \
                             notif_data, DFU_NOTIFY_LENGTH_RECEIVE_IC_TYPE, GATT_PDU_TYPE_NOTIFICATION);
        }
        break;

#if (ENABLE_BANK_SWITCH_COPY_APP_DATA == 1)
    case DFU_OPCODE_COPY_IMG://0x0c
        {
            uint32_t dlAddress, dlSize;
            LE_ARRAY_TO_UINT16(g_dfu_para.ctrl_header.image_id, p);
            p += 2;
            LE_ARRAY_TO_UINT32(dlAddress, p);
            p += 4;
            LE_ARRAY_TO_UINT32(dlSize, p);
            DFU_PRINT_TRACE2("DFU_OPCODE_COPY_IMG: dlAddress=0x%x,dlSize=0x%x", dlAddress,
                             dlSize);

            notif_data[0] = DFU_OPCODE_NOTIFICATION;
            notif_data[1] = DFU_OPCODE_COPY_IMG;

            if (dfu_copy_img(g_dfu_para.ctrl_header.image_id, dlAddress, dlSize))
            {
                notif_data[2] = DFU_ARV_SUCCESS;
            }
            else
            {
                notif_data[2] = DFU_ARV_FAIL_INVALID_PARAMETER;
            }
            server_send_data(conn_id, dfu_service_id, INDEX_DFU_CONTROL_POINT_CHAR_VALUE, \
                             notif_data, DFU_NOTIFY_LENGTH_ARV, GATT_PDU_TYPE_NOTIFICATION);
        }
        break;
#endif  //end ENABLE_BANK_SWITCH_COPY_APP_DATA

    default:
        {
            DFU_PRINT_TRACE1("dfu_service_handle_control_point_req: Unknown Opcode=0x%x",
                             dfu_control_point.opcode);
        }
        break;
    }
}


/**
 * @brief dfu_service_handle_packet_req
 *
 * @param length     data reviewed length.
 * @param p_value    data receive point address.
 * @return None
*/
void dfu_service_handle_packet_req(uint8_t conn_id, uint16_t length, uint8_t *p_value)
{
    T_APP_RESULT w_cause = APP_RESULT_SUCCESS;
    T_DFU_CALLBACK_DATA callback_data;
    callback_data.conn_id = conn_id;
    callback_data.msg_type = SERVICE_CALLBACK_TYPE_WRITE_CHAR_VALUE;
    callback_data.msg_data.write.write_attrib_index = INDEX_DFU_PACKET_VALUE;
    callback_data.msg_data.write.length = length;
    callback_data.msg_data.write.p_value = p_value;

    DFU_PRINT_INFO4("dfu_service_handle_packet_req: length=%d, cur_offset=%d, g_ota_tmp_buf_used_size=%d, image_total_length=%d",
                    length, g_dfu_para.cur_offset,
                    g_ota_tmp_buf_used_size, g_dfu_para.image_total_length);

    if (buffer_check_en == true)
    {
        /*to avoid memory overflow*/
        if (g_ota_tmp_buf_used_size + length > DFU_TEMP_BUFFER_SIZE)
        {
            DFU_PRINT_ERROR3("<==dfu_service_handle_packet_req: Buf overflow! ota_tmp_buf_used_size=%d,length=%d, max_buffer_size=%d",
                             g_ota_tmp_buf_used_size, length, DFU_TEMP_BUFFER_SIZE);

            T_DFU_FAIL_REASON dfu_fail_reason = DFU_FAIL_EXCEED_MAX_BUFFER_SIZE;
            callback_data.msg_data.write.length = sizeof(T_DFU_FAIL_REASON);
            callback_data.msg_data.write.p_value = (uint8_t *)&dfu_fail_reason;
            callback_data.msg_data.write.opcode = DFU_WRITE_FAIL;
            if (pfn_dfu_service_cb)
            {
                w_cause = pfn_dfu_service_cb(dfu_service_id, (void *)&callback_data);
                if (w_cause != APP_RESULT_SUCCESS)
                {
                    return;
                }
            }

            dfu_fw_reboot(false);
        }
        else
        {
            memcpy(p_ota_temp_buffer_head + g_ota_tmp_buf_used_size, p_value, length);
            g_ota_tmp_buf_used_size += length;
        }
    }
    else
    {
        /*when disable buffer check, Default client send 20bytes per packet*/
        uint32_t max_buffer_size = DFU_TEMP_BUFFER_SIZE - (DFU_TEMP_BUFFER_SIZE % 20);
        /*0.check memcpy buffer boundary*/
        if (g_ota_tmp_buf_used_size + length > max_buffer_size)
        {
            DBG_DIRECT("<==dfu_service_handle_packet_req: Buf overflow! ota_tmp_buf_used_size=%d,length=%d, max_buffer_size=%d",
                       g_ota_tmp_buf_used_size, length, max_buffer_size);

            T_DFU_FAIL_REASON dfu_fail_reason = DFU_FAIL_EXCEED_MAX_BUFFER_SIZE;
            callback_data.msg_data.write.opcode = DFU_WRITE_FAIL;
            callback_data.msg_data.write.length = sizeof(T_DFU_FAIL_REASON);
            callback_data.msg_data.write.p_value = (uint8_t *)&dfu_fail_reason;
            if (pfn_dfu_service_cb)
            {
                w_cause = pfn_dfu_service_cb(dfu_service_id, (void *)&callback_data);
                if (w_cause != APP_RESULT_SUCCESS)
                {
                    return;
                }
            }

            dfu_fw_reboot(false);
        }

        /*0.check total length*/
        if (g_dfu_para.cur_offset + g_ota_tmp_buf_used_size + length > g_dfu_para.image_total_length)
        {
            DFU_PRINT_ERROR1("<==dfu_service_handle_packet_req: received data total length beyond image_total_length(%d bytes)",
                             g_dfu_para.image_total_length);

            T_DFU_FAIL_REASON dfu_fail_reason = DFU_FAIL_EXCEED_IMG_TOTAL_LEN;
            callback_data.msg_data.write.opcode = DFU_WRITE_FAIL;
            callback_data.msg_data.write.length = sizeof(T_DFU_FAIL_REASON);
            callback_data.msg_data.write.p_value = (uint8_t *)&dfu_fail_reason;
            if (pfn_dfu_service_cb)
            {
                w_cause = pfn_dfu_service_cb(dfu_service_id, (void *)&callback_data);
                if (w_cause != APP_RESULT_SUCCESS)
                {
                    return;
                }
            }

            dfu_fw_reboot(false);
        }

        /*start handle received data*/
        //1. deceypt data
        if (OTP->ota_with_encryption_data)
        {
            dfu_hw_aes_decrypt_image(p_value, p_value, length);
        }

        //2. copy received data to buffer
        memcpy(p_ota_temp_buffer_head + g_ota_tmp_buf_used_size, p_value, length);
        g_ota_tmp_buf_used_size += length;

        //3. write to flash
        if (g_ota_tmp_buf_used_size == max_buffer_size ||
            g_dfu_para.cur_offset + g_ota_tmp_buf_used_size == g_dfu_para.image_total_length)
        {
            uint32_t result = dfu_update(g_dfu_para.ctrl_header.image_id, g_dfu_para.cur_offset,
                                         g_ota_tmp_buf_used_size,
                                         (uint32_t *)p_ota_temp_buffer_head);
            if (result)
            {
                DBG_DIRECT("Buffer check disable: dfu_update fail=%d", result);

                /*eflash write fail, we should restart ota procedure.*/
                T_DFU_FAIL_REASON dfu_fail_reason = DFU_FAIL_UPDATE_FLASH;
                callback_data.msg_data.write.opcode = DFU_WRITE_FAIL;
                callback_data.msg_data.write.length = sizeof(T_DFU_FAIL_REASON);
                callback_data.msg_data.write.p_value = (uint8_t *)&dfu_fail_reason;
                if (pfn_dfu_service_cb)
                {
                    w_cause = pfn_dfu_service_cb(dfu_service_id, (void *)&callback_data);
                    if (w_cause != APP_RESULT_SUCCESS)
                    {
                        return;
                    }
                }

                dfu_fw_reboot(false);
            }
            else
            {
                uint32_t updated_success_len = g_ota_tmp_buf_used_size;
                callback_data.msg_data.write.opcode = DFU_WRITE_DOING;
                callback_data.msg_data.write.length = 4;
                callback_data.msg_data.write.p_value = (uint8_t *)&updated_success_len;
                if (pfn_dfu_service_cb)
                {
                    w_cause = pfn_dfu_service_cb(dfu_service_id, (void *)&callback_data);
                    if (w_cause != APP_RESULT_SUCCESS)
                    {
                        return;
                    }
                }

                /*update varibals value*/
                g_dfu_para.cur_offset += g_ota_tmp_buf_used_size;
                g_ota_tmp_buf_used_size = 0;
            }
        }
    }

}

/**
 * @brief write characteristic data from service.
 *
 * @param ServiceID          ServiceID to be written.
 * @param iAttribIndex       Attribute index of characteristic.
 * @param wLength            length of value to be written.
 * @param pValue             value to be written.
 * @return Profile procedure result
*/
T_APP_RESULT dfu_attr_write_cb(uint8_t conn_id, uint8_t service_id, uint16_t attrib_index,
                               T_WRITE_TYPE write_type,
                               uint16_t length, uint8_t *p_value, P_FUN_WRITE_IND_POST_PROC *p_write_ind_post_proc)
{
    T_APP_RESULT w_cause = APP_RESULT_SUCCESS;
    T_DFU_CALLBACK_DATA callback_data;
    callback_data.conn_id = conn_id;
    callback_data.msg_type = SERVICE_CALLBACK_TYPE_WRITE_CHAR_VALUE;
    callback_data.msg_data.write.write_attrib_index = attrib_index;
    callback_data.msg_data.write.length = length;
    callback_data.msg_data.write.p_value = p_value;

    /* Notify Application. */
    callback_data.msg_data.write.opcode = DFU_WRITE_ATTR_ENTER;
    if (pfn_dfu_service_cb)
    {
        w_cause = pfn_dfu_service_cb(service_id, (void *)&callback_data);
        if (w_cause != APP_RESULT_SUCCESS)
        {
            return w_cause;
        }
    }

    if (attrib_index == INDEX_DFU_CONTROL_POINT_CHAR_VALUE)
    {
        dfu_service_handle_control_point_req(conn_id, length, p_value);
    }
    else if (attrib_index == INDEX_DFU_PACKET_VALUE)
    {
        dfu_service_handle_packet_req(conn_id, length, p_value);
    }
    else
    {
        DFU_PRINT_INFO1("dfu_attr_write_cb fail! attrib_index=%d", attrib_index);
        w_cause = APP_RESULT_ATTR_NOT_FOUND;
    }

    /* Notify Application. */
    callback_data.msg_data.write.opcode = DFU_WRITE_ATTR_EXIT;
    if (pfn_dfu_service_cb)
    {
        w_cause = pfn_dfu_service_cb(service_id, (void *)&callback_data);
        if (w_cause != APP_RESULT_SUCCESS)
        {
            return w_cause;
        }
    }

    return w_cause;
}

/**
 * @brief update CCCD bits from stack.
 *
 * @param ServiceId          Service ID.
 * @param Index          Attribute index of characteristic data.
 * @param wCCCBits         CCCD bits from stack.
 * @return None
*/
void dfu_cccd_update_cb(uint8_t conn_id, T_SERVER_ID service_id, uint16_t index, uint16_t ccc_bits)
{
    T_DFU_CALLBACK_DATA callback_data;
    callback_data.msg_type = SERVICE_CALLBACK_TYPE_INDIFICATION_NOTIFICATION;
    callback_data.conn_id = conn_id;
    bool b_handle = true;
    DFU_PRINT_INFO2("dfu_cccd_update_cb: index=%d, ccc_bits=0x%x", index, ccc_bits);
    switch (index)
    {
    case INDEX_DFU_CHAR_CCCD_INDEX:
        {
            if (ccc_bits & GATT_CLIENT_CHAR_CONFIG_NOTIFY)
            {
                // Enable Notification
                callback_data.msg_type = SERVICE_CALLBACK_TYPE_INDIFICATION_NOTIFICATION;
                callback_data.msg_data.notification_indification_index = DFU_NOTIFY_ENABLE;
            }
            else
            {
                callback_data.msg_type = SERVICE_CALLBACK_TYPE_INDIFICATION_NOTIFICATION;
                callback_data.msg_data.notification_indification_index = DFU_NOTIFY_DISABLE;
            }
            break;
        }
    default:
        {
            b_handle = false;
            break;
        }

    }
    /* Notify Application. */
    if (pfn_dfu_service_cb && (b_handle == true))
    {
        T_APP_RESULT update_cause = pfn_dfu_service_cb(service_id, (void *)&callback_data);
        if (update_cause != APP_RESULT_SUCCESS)
        {
            return;
        }
    }

    return;
}

/**
 * @brief OTA ble Service Callbacks.
*/
const T_FUN_GATT_SERVICE_CBS DfuServiceCBs =
{
    NULL,                // Read callback function pointer
    dfu_attr_write_cb,   // Write callback function pointer
    dfu_cccd_update_cb   // CCCD update callback function pointer
};

/**
 * @brief  add OTA ble service to application.
 *
 * @param  pFunc          pointer of app callback function called by profile.
 * @return service ID auto generated by profile layer.
 * @retval ServiceId
*/
uint8_t dfu_add_service(void *pFunc)
{
    if (false == server_add_service(&dfu_service_id,
                                    (uint8_t *)gatt_dfu_service_table,
                                    sizeof(gatt_dfu_service_table),
                                    DfuServiceCBs))
    {
        DFU_PRINT_ERROR1("dfu_add_service: service_id=%d", dfu_service_id);
        dfu_service_id = 0xff;
        return dfu_service_id;
    }
    pfn_dfu_service_cb = (P_FUN_SERVER_GENERAL_CB)pFunc;
    p_ota_temp_buffer_head = ota_temp_buffer_head;

    return dfu_service_id;
}


