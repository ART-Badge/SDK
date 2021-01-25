#include <string.h>
#include "trace.h"
#include "app_section.h"
#include "flash_device.h"
#include "patch_header_check.h"
#include "hw_aes.h"
#include "rtl876x_hw_aes.h"
#include "os_sync.h"
#include "otp.h"
#include "dfu_api.h"
#include "rtl876x_wdg.h"
#include "dfu_flash.h"
#include "sha256.h"
#include "crc16btx.h"
#include "board.h"
#include "otp_config.h"

#ifdef SUPPORT_SINGLE_BANK_OTA_USER_DATA
#if ((USER_DATA_START_ADDR & (FMC_BLK_SECTION_LEN - 1)) != 0)
#error "User data start addr is not 64KB Aligned!!"
#endif
#endif

/*============================================================================*
 *                              Macros
 *============================================================================*/
#define BTAON_FAST_REBOOT_SW_INFO0              0x2

#define DECODE_OFFSET                           (sizeof(T_COMPRESS_IMG_HEADER_FORMAT))
#define COMPRESS_HEADER_SHA256_OFFSET           36

#define CRC_BUFFER_SIZE                         128  //may modify to improve performance
#define SHA256_BUFFER_SIZE                      128

#define SHA256_LENGTH                           32

#define DFU_UPDATE_LEN                          256

/*============================================================================*
 *                               Types
 *============================================================================*/
typedef union _BTAON_FAST_REBOOT_SW_INFO0_TYPE
{
    uint8_t d8;
    struct
    {
        uint8_t rsvd0:                      5;
        uint8_t is_ota:                     1;  /* bit[5]: enter ota mode after reset           */
        uint8_t rsvd1:                      2;
    } flag;
} BTAON_FAST_REBOOT_SW_INFO0_TYPE;

/*============================================================================*
 *                               Variables
 *============================================================================*/
static uint32_t img_total_len = 0;
static uint32_t total_block_cnt = 0;
static uint32_t block_cnt = 0;
uint8_t g_flash_old_bp_lv = 0xff;
/*============================================================================*
 *                              Local Functions
 *============================================================================*/
#if (SUPPORT_PUBLIC_DECODE_OTA == 1)
static bool dfu_check_compressed_image_crc(T_IMG_CTRL_HEADER_FORMAT *p_header)
{
    /*
           Image layout is as below, and CRC will include part 2)
           1) Image Compress Header    96 bytes
           2) compressed app + header  bytes  - CRC
        */

    uint8_t buf[CRC_BUFFER_SIZE];
    uint16_t calced_crc16;

    uint32_t length = p_header->payload_len;
    uint8_t *pdata = (uint8_t *) p_header + sizeof(T_COMPRESS_IMG_HEADER_FORMAT);
    calced_crc16 = dfu_process_crc(BTXFCS_INIT, buf, CRC_BUFFER_SIZE, pdata, length);

    return (p_header->crc16 == calced_crc16);
}

static bool dfu_check_compressed_image_sha256(T_IMG_CTRL_HEADER_FORMAT *p_header)
{
    /*
           Image layout is as below, and CRC will include part 2)
           1) Image Compress Header    96 bytes
           2) compressed app + header  bytes  -sha256
        */

    SHA256_CTX ctx;
    uint8_t buf[SHA256_BUFFER_SIZE];
    uint8_t sha256sum[SHA256_LENGTH];
    uint8_t sha256img[SHA256_LENGTH];

    SHA256_Init(&ctx);

    uint8_t *pdata = (uint8_t *)p_header + sizeof(T_COMPRESS_IMG_HEADER_FORMAT);
    dfu_process_sha256(&ctx, buf, SHA256_BUFFER_SIZE, pdata, p_header->payload_len);

    SHA256_Final(&ctx, sha256sum);

    pdata = (uint8_t *)p_header + COMPRESS_HEADER_SHA256_OFFSET;
    for (int i = 0; i < SHA256_LENGTH; i++)
    {
        sha256img[i] = *(pdata++);
    }

    return (memcmp(sha256img, sha256sum, SHA256_LENGTH) == 0);
}

/**
 * @brief  calculated checksum(CRC16 or SHA256 determined by image) over the image, and compared
 *         with given checksum value.
 * @param  p_header image header info of the given image.
 * @return true if image integrity check pass via checksum compare, false otherwise.
*/
static bool check_compressed_image_chksum(T_IMG_CTRL_HEADER_FORMAT *p_header)
{
    bool ret;

    if (p_header->crc16)
    {
        ret = dfu_check_compressed_image_crc(p_header);
    }
    else
    {
        ret = dfu_check_compressed_image_sha256(p_header);
    }

    return ret;
}

#endif
/**
 * @brief erase a sector of the flash.
 * @param  addr   flash addr in sector to be erase.
 * @return  0 if erase successfully, fail otherwise
*/
static bool flash_erase_sector(uint32_t addr)
{
    bool result = false;
    result = flash_erase_locked(FLASH_ERASE_SECTOR, addr);
    DFU_PRINT_INFO2("<==dfu flash erase sector: addr=%x, result=%d(1:success)", addr, result);
    return result;
}

/**
 * @brief erase a block of the flash.
 * @param  addr   flash addr in sector to be erase.
 * @return  0 if erase successfully, fail otherwise
*/
static bool flash_erase_block(uint32_t addr)
{
    bool result = false;
    result = flash_erase_locked(FLASH_ERASE_BLOCK, addr);
    DFU_PRINT_INFO2("<==dfu flash erase block: addr=%x, result=%d(1:success)", addr, result);
    return result;
}

/**
* @brief check flash area is blank
* @param  image_id          image_id to identify FW.
* @param  offset             offset of the image.
* @param  size               size to check.
* @return  0 if blank check successfully, error line number otherwise
*/
static uint32_t dfu_flash_check_blank(uint16_t image_id, uint32_t offset, uint16_t size)
{
    uint32_t i = 0;
    uint32_t result = 0;
    uint32_t start_addr = 0;

    if (size & 0x3)
    {
        result = __LINE__;
        goto L_Return;
    }

    if ((image_id != IMAGE_USER_DATA) &&
        (image_id < OTA || image_id >= IMAGE_MAX))
    {
        result = __LINE__;
        goto L_Return;
    }

    if (image_id == IMAGE_USER_DATA)
    {
        start_addr = flash_get_bank_addr(FLASH_BKP_DATA1) + offset;
    }
    else
    {

#ifdef SUPPORT_ALONE_UPPERSTACK_IMG
        start_addr = get_temp_ota_bank_addr_by_img_id_app((T_IMG_ID)image_id) + offset;
#else
        start_addr = get_temp_ota_bank_addr_by_img_id((T_IMG_ID)image_id) + offset;
#endif
    }

    if (start_addr == 0)
    {
        result = __LINE__;
        goto L_Return;
    }

    for (i = 0; i < size / 4; i++)
    {
        uint32_t r_data;
        if (flash_auto_read_locked(start_addr, &r_data))
        {
            if (r_data != FMC_ERASE_PATTERN)
            {
                result = __LINE__;
                goto L_Return;
            }
            start_addr += 4;
        }
        else
        {
            result = __LINE__;
            goto L_Return;
        }
    }

L_Return:
    DFU_PRINT_INFO4("<==dfu_flash_check_blank: image_id=0x%x, size=%d, addr=%d, result=%d",
                    image_id, size, start_addr, result);
    return result;
}

/**
 * @brief  set specified image valid bit..
 * @param  p_header      specified image.
 * @return true if ready bit sets to 0, false otherwise
*/
void dfu_set_compressed_ready(T_IMG_CTRL_HEADER_FORMAT *p_header)
{
    /* clear not_ready and compressed_not_ready bit */
    uint32_t data;
    /*support for 4bit mode silent ota*/
    flash_auto_read_locked((uint32_t)p_header, &data);
    data &= ~BIT26;
    flash_auto_write_locked((uint32_t)p_header, data);
}

void dfu_set_ready(T_IMG_CTRL_HEADER_FORMAT *p_header)
{
    /* clear not_ready bit */
    uint32_t data;
    /*support for 4bit mode silent ota*/
    flash_auto_read_locked((uint32_t)p_header, &data);
    data &= ~BIT23;
    flash_auto_write_locked((uint32_t)p_header, data);
}

/**
* @brief check ota image size whether exceed flash layout address.
*/
static bool check_dfu_update_image_length(uint16_t image_id, uint32_t offset, uint32_t length,
                                          void *p_void, uint32_t *ret)
{
    uint32_t temp_bank_size = 0;
    *ret = 0;

    if (p_void == NULL)
    {
        *ret = __LINE__;
        return false;
    }

    if (IMAGE_USER_DATA == image_id)
    {
        temp_bank_size = flash_get_bank_size(FLASH_BKP_DATA1);
    }
    else
    {
#ifdef SUPPORT_ALONE_UPPERSTACK_IMG
        temp_bank_size = get_temp_ota_bank_size_by_img_id_app((T_IMG_ID)image_id);
#else
        temp_bank_size = get_temp_ota_bank_size_by_img_id((T_IMG_ID)image_id);
#endif
    }


    if (offset == 0)
    {
        T_IMG_CTRL_HEADER_FORMAT *p_header = (T_IMG_CTRL_HEADER_FORMAT *) p_void;
        uint32_t total_length = p_header->payload_len + IMG_HEADER_SIZE;

        if (total_length > temp_bank_size)
        {
            DBG_DIRECT("New Image too large! total_length=%d, temp_bank_size=%d", total_length,
                       temp_bank_size);
            *ret = __LINE__;
            return false;
        }
    }

    if (offset + length > temp_bank_size)
    {
        DFU_PRINT_ERROR3("New Image single packet too large! offset=%d, length=%d, temp_bank_size=%d",
                         offset, length, temp_bank_size);
        *ret = __LINE__;
        return false;
    }

    //check pass
    return true;
}

static uint32_t dfu_write(uint32_t start_addr, uint32_t length, uint8_t *p_void)
{
    uint32_t result = 0;
    uint16_t loop_cnt = 0;
    uint16_t remain_size = 0;
    uint32_t counter = 0;
    uint8_t s_val[DFU_UPDATE_LEN];

    loop_cnt = length / DFU_UPDATE_LEN;
    remain_size = length % DFU_UPDATE_LEN;
    DFU_PRINT_INFO2("dfu_write: start_addr=0x%x, length=%d",
                    start_addr, length);

#if 0  //auto mode access
    for (int i = 0; i < length; i = i + 4)
    {
        uint32_t s_val = 0;
        flash_auto_write_locked(start_addr + i, *(uint32_t *)p_void);
        flash_auto_read_locked(start_addr + i | FLASH_OFFSET_TO_NO_CACHE, &s_val);
        if (s_val != *(uint32_t *)p_void)
        {
            DFU_PRINT_ERROR3("<==dfu_update: ERROR! w_data=0x%x, r_data=0x%x, addr=0x%x",
                             *(uint32_t *)p_void, s_val, start_addr + i);
            result = __LINE__; //write fail
            goto L_Return;
        }
        else
        {
            p_void++;
        }
    }
#endif

    for (int i = 0; i < loop_cnt; i++)
    {
        if (flash_write_locked(start_addr + i * DFU_UPDATE_LEN, DFU_UPDATE_LEN,
                               p_void + i * DFU_UPDATE_LEN))
        {

            if (flash_split_read_locked(start_addr + i * DFU_UPDATE_LEN, DFU_UPDATE_LEN,
                                        s_val, &counter))
            {
                if (0 != memcmp(s_val, p_void + i * DFU_UPDATE_LEN, DFU_UPDATE_LEN))
                {
                    return __LINE__;
                }
            }
            else
            {
                return __LINE__;
            }
        }
        else
        {
            return __LINE__;
        }
    }

    if (remain_size)
    {
        if (flash_write_locked(start_addr + loop_cnt * DFU_UPDATE_LEN, remain_size,
                               p_void + loop_cnt * DFU_UPDATE_LEN))
        {
            if (flash_split_read_locked(start_addr + loop_cnt * DFU_UPDATE_LEN,
                                        remain_size, s_val, &counter))
            {
                //ensure p_void is ram addr
                if (0 != memcmp(s_val, p_void + loop_cnt * DFU_UPDATE_LEN, remain_size))
                {
                    return __LINE__;
                }
            }
            else
            {
                return __LINE__;
            }
        }
        else
        {
            return __LINE__;
        }
    }

    return result;
}

/*============================================================================*
 *                              External Functions
 *============================================================================*/
/**
 * @brief check whether in OTA mode.
*/
bool dfu_check_ota_mode_flag(void)
{
    BTAON_FAST_REBOOT_SW_INFO0_TYPE nFastBoot =
        (BTAON_FAST_REBOOT_SW_INFO0_TYPE)btaon_fast_read_safe(BTAON_FAST_REBOOT_SW_INFO0);
    DFU_PRINT_INFO1("dfu_check_ota_mode_flag: ota(%d)", nFastBoot.flag.is_ota);

    return nFastBoot.flag.is_ota ? true : false;
}

/**
 * @brief set aon reg value means whether in OTA mode or not.
*/
void dfu_set_ota_mode_flag(bool enable)
{
    BTAON_FAST_REBOOT_SW_INFO0_TYPE nFastBoot =
        (BTAON_FAST_REBOOT_SW_INFO0_TYPE)btaon_fast_read_safe(BTAON_FAST_REBOOT_SW_INFO0);
    if (enable)
    {
        nFastBoot.flag.is_ota = 1;
    }
    else
    {
        nFastBoot.flag.is_ota = 0;
    }
    btaon_fast_write_safe(BTAON_FAST_REBOOT_SW_INFO0, nFastBoot.d8);
    DFU_PRINT_INFO1("dfu_set_ota_mode_flag ota(%d)", nFastBoot.flag.is_ota);
}

/**
 * @brief switch to the OTA mode, if support normal ota app need call it.
*/
void dfu_switch_to_ota_mode(void)
{
    DFU_PRINT_INFO0("[==>dfu_switch_to_ota_mode");
    dfu_set_ota_mode_flag(true);
    WDG_SystemReset(RESET_ALL_EXCEPT_AON, DFU_SWITCH_TO_OTA_MODE);
}

/**
 * @brief OTA procedure do wdg system reset.
 * @param  is_active_fw true means dfu success! otherwise fail.
*/
void dfu_fw_reboot(bool is_active_fw)
{
    DFU_PRINT_TRACE1("==>dfu_fw_reboot: is_active_fw=%d", is_active_fw);
    if (is_active_fw)
    {
        WDG_SystemReset(RESET_ALL, DFU_ACTIVE_RESET);
    }
    else
    {
        WDG_SystemReset(RESET_ALL, DFU_FAIL_RESET);
    }
}

/**
 * @brief OTA image default ecb mode aes decrypt, last less then 16bytes no encrypted.
*/
void dfu_hw_aes_decrypt_image(uint8_t *input, uint8_t *output, uint32_t length)
{
    uint32_t offset = 0;
    hw_aes_init(OTP->aes_key, NULL, AES_MODE_ECB, OTP->ota_with_encryption_use_aes256);
    do
    {
        if ((length - offset) >= 16)
        {
            hw_aes_decrypt_16byte(input + offset, output + offset);
            offset += 16;
        }
        else
        {
            break;
        }
    }
    while (1);
}

/**
 * @brief  report specified target ic type.
 * @param  image_id     image_id to identify FW.
 * @param  p_ic_type     To store ic type.
 * @return 0 if report ic type info successfully, error line number otherwise
*/
uint32_t dfu_report_target_ic_type(uint16_t image_id, uint8_t *p_ic_type)
{
    uint32_t result = 0;
    T_IMG_CTRL_HEADER_FORMAT *p_header;

    if (!p_ic_type)
    {
        result = __LINE__;
        goto L_Return;
    }

    if (IMAGE_USER_DATA == image_id)
    {
        p_header = (T_IMG_CTRL_HEADER_FORMAT *)flash_get_bank_addr(FLASH_BKP_DATA1);
    }
    else
    {
        p_header = (T_IMG_CTRL_HEADER_FORMAT *)get_header_addr_by_img_id((T_IMG_ID)image_id);
    }

    if (!p_header)
    {
        result = __LINE__;
        goto L_Return;
    }

    *p_ic_type = p_header->ic_type;

L_Return:
    DFU_PRINT_INFO1("<==dfu_report_target_ic_type: result=%d", result);
    return result;
}

/**
 * @brief  report specified FW info and current OTA offset.
 * @param  image_id          image_id to identify FW.
 * @param  p_origin_fw_version     To store current FW version.
 * @param  p_offset            To store current file offset.
 * @return 0 if report FW info successfully, error line number otherwise
*/
uint32_t dfu_report_target_fw_info(uint16_t image_id, uint32_t *p_origin_fw_version,
                                   uint32_t *p_offset)
{
    uint32_t result = 0;
    T_IMG_HEADER_FORMAT *p_header;

    if (!p_offset)
    {
        result = __LINE__;
        goto L_Return;
    }

    if (!p_origin_fw_version)
    {
        result = __LINE__;
        goto L_Return;
    }

    if (IMAGE_USER_DATA == image_id)
    {
        p_header = (T_IMG_HEADER_FORMAT *)flash_get_bank_addr(FLASH_BKP_DATA1);
    }
    else
    {
        p_header = (T_IMG_HEADER_FORMAT *)get_header_addr_by_img_id((T_IMG_ID)image_id);
    }
    if (!p_header)
    {
        result = __LINE__;
        goto L_Return;
    }

    if (image_id == OTA)
    {
        T_OTA_HEADER_FORMAT *p_ota_header;
        p_ota_header = (T_OTA_HEADER_FORMAT *)p_header;
        *p_origin_fw_version = p_ota_header->ver_val;
    }
    else if ((IMAGE_USER_DATA == image_id) ||
             (image_id >= SecureBoot && image_id < IMAGE_MAX))
    {
        *p_origin_fw_version = p_header->git_ver.ver_info.version;
    }
    else//image_id don't support
    {
        result = __LINE__;
        goto L_Return;
    }

    /*bee2 default don't support re-ota*/
    *p_offset = 0;

L_Return:
    DFU_PRINT_INFO1("<==dfu_report_target_fw_info: result=%d", result);
    return result;
}



/**
* @brief calculate checksum of lenth of buffer in flash.
* @param  buf                data.
* @param  length             length of data, must align 2bytes.
* @param  crc_val            ret crc value point.
* @return  0 if buffer checksum calcs successfully, error line number otherwise
*/

uint32_t dfu_check_buf_crc(uint8_t *buf, uint32_t length, uint16_t crc_val)
{
    uint16_t checksum16 = 0;
    uint32_t result = 0;
    uint32_t i;
    uint16_t *p16;

    if (length & 0x1)
    {
        result =  __LINE__;
        goto L_Return;
    }

    p16 = (uint16_t *)buf;
    for (i = 0; i < length / 2; ++i)
    {
        checksum16 = checksum16 ^ (*p16);
        ++p16;
    }

    checksum16 = ((checksum16 & 0xff) << 8) | ((checksum16 & 0xff00) >> 8);

    if (checksum16 != crc_val)
    {
        result =  __LINE__;
        goto L_Return;
    }

L_Return:
    DFU_PRINT_INFO4("<==dfu_check_buf_crc: length=%d, checksum16=0x%x, crc_val=0x%x, result:%d",
                    length, checksum16, crc_val, result);
    return result;
}

/**
*  @brief: unlock flash is need when erase or write flash.
*/
bool unlock_flash_bp_all(void)
{
    DFU_PRINT_TRACE0("**********Flash unlock BP***********");
    if (flash_sw_protect_unlock_by_addr_locked(0x00800000, &g_flash_old_bp_lv))
    {
        DFU_PRINT_TRACE1("<==Unlock Total Flash Success! prev_bp_lv=%d", g_flash_old_bp_lv);
        return true;
    }
    return false;
}

/**
*  @brief: lock flash after erase or write flash.
*/
void lock_flash_bp(void)
{
    if (g_flash_old_bp_lv != 0xff)
    {
        flash_set_block_protect_locked(g_flash_old_bp_lv);
    }
}

/**
 * @brief erase a sector of the flash, will retry three times at most
 *
 * @param  image_id          image_id to identify FW.
 * @param  offset             offset of the image.
 * @return  0 if erase successfully, error line number otherwise,
*/
uint32_t dfu_flash_erase_sector_with_retry(uint16_t image_id, uint32_t offset)
{
    uint32_t result = 0;
    uint32_t cnt = 0;
    uint32_t dfu_base_addr = 0;

    if (image_id == IMAGE_USER_DATA)
    {
        dfu_base_addr = flash_get_bank_addr(FLASH_BKP_DATA1);;
    }
    else
    {
#ifdef SUPPORT_ALONE_UPPERSTACK_IMG
        dfu_base_addr = get_temp_ota_bank_addr_by_img_id_app((T_IMG_ID)image_id);
#else
        dfu_base_addr = get_temp_ota_bank_addr_by_img_id((T_IMG_ID)image_id);
#endif
    }
    if (dfu_base_addr == 0)
    {
        result = __LINE__;
        goto L_Return;
    }

    do
    {
        unlock_flash_bp_all();
        flash_erase_sector(dfu_base_addr + offset);
        lock_flash_bp();
        result = dfu_flash_check_blank(image_id, offset, FMC_SEC_SECTION_LEN);

        if (!result)
        {
            //erase ok
            break;
        }
        else
        {
            //retry
            cnt++;
        }
        if (cnt >= 3)
        {
            /*retry three times all fail*/
            goto L_Return;
        }
    }
    while (1);

L_Return:
    DFU_PRINT_INFO2("dfu_flash_erase_with_retry: cnt=%d, result=%d",
                    cnt, result);
    return result;
}


/**
 * @brief  write specified image data with specified length to flash
 * @param  image_id          image_id to identify FW.
 * @param  offset             offset of the image.
 * @param  length             length of data.
 * @param  p_void             pointer to data.
 * @return 0 if write FW image successfully, error line number otherwise
*/
uint32_t dfu_update(uint16_t image_id, uint32_t offset, uint32_t length,
                    uint32_t/*void*/ *p_void)
{
    uint32_t result = 0;
    uint32_t dfu_base_addr = 0;
    uint32_t start_addr = 0;
    bool erase_rst = true; //default value must be true

    DFU_PRINT_INFO2("==>dfu_update: offset=0x%x, length=%d", offset, length);

    if (length % 4)
    {
        result = __LINE__;
        goto L_Return;
    }

    if (p_void == 0)
    {
        result = __LINE__;
        goto L_Return;
    }

    if (IMAGE_USER_DATA != image_id)
    {
        /*get back up area address*/
#ifdef SUPPORT_ALONE_UPPERSTACK_IMG
        dfu_base_addr = get_temp_ota_bank_addr_by_img_id_app((T_IMG_ID)image_id);
#else
        dfu_base_addr = get_temp_ota_bank_addr_by_img_id((T_IMG_ID)image_id);
#endif
    }
    else
    {
        /*get back up area address*/
        dfu_base_addr = flash_get_bank_addr(FLASH_BKP_DATA1);
    }

    if (dfu_base_addr == 0)
    {
        result = __LINE__;
        goto L_Return;
    }

    /* before erase temp image or write image to flash temp, check access length depend flash layout */
    if (!check_dfu_update_image_length(image_id, offset, length, p_void, &result))
    {
        goto L_Return;
    }

    /*if it's start_packet*/
    if (offset == 0)
    {
        T_IMG_CTRL_HEADER_FORMAT *p_header = (T_IMG_CTRL_HEADER_FORMAT *) p_void;
        /*access ram addr, make sure image is not ready*/
        p_header->ctrl_flag.flag_value.not_ready = 0x1;
#if (SUPPORT_PUBLIC_DECODE_OTA == 1)
        if (IMAGE_COMPRESSED == p_header->ctrl_flag.flag_value.image_type)
        {
            /*access ram addr, make sure image is not ready*/
            p_header->ctrl_flag.flag_value.compressed_not_ready = 0x1;
        }

#endif
        if (IMAGE_USER_DATA == image_id)
        {
            img_total_len = p_header->payload_len + IMG_HEADER_SIZE;

            if (0 == (dfu_base_addr & (FMC_BLK_SECTION_LEN - 1)))  //start addr 64k align
            {
                total_block_cnt = img_total_len / FMC_BLK_SECTION_LEN;
                DFU_PRINT_TRACE2("<==dfu_update: img_total_len=0x%x, total_block_cnt=%d",
                                 img_total_len, total_block_cnt);
            }
            else
            {
                /*improve user data ota performance with the limitation of start addr aligned 64K*/
                DBG_DIRECT("Error User data start addr not aligned 64K!");

                result = __LINE__;
                goto L_Return;
            }

            if (0 != (img_total_len & (FMC_BLK_SECTION_LEN - 1)))  //img_total_len is not align 64KB
            {
                if (length > FMC_SEC_SECTION_LEN)
                {
                    /*DFU_TEMP_BUFFER_SIZE can't larger than 4KB if user data size not aligned 64KB,
                    otherwise DFU_TEMP_BUFFER_SIZE can't larger than 64KB when user data size aligned 64KB*/
                    DBG_DIRECT("Error DFU_TEMP_BUFFER_SIZE > 4KB");

                    result = __LINE__;
                    goto L_Return;

                }
            }
        }

        DFU_PRINT_TRACE3("<==dfu_update: New Image Header=0x%x, image_id=0x%x, dfu_base_addr=0x%x",
                         length, image_id, dfu_base_addr);
    }

    start_addr = dfu_base_addr + offset;

    unlock_flash_bp_all();
    if (IMAGE_USER_DATA == image_id)
    {
        if (block_cnt < total_block_cnt)
        {
            if (0 == (offset & (FMC_BLK_SECTION_LEN - 1)))   //new Block starts
            {
                erase_rst = flash_erase_block(start_addr);
                block_cnt ++;
            }
            else  // only cross two block
            {
                if (((offset % FMC_BLK_SECTION_LEN) + length > FMC_BLK_SECTION_LEN)
                    && (length <= FMC_BLK_SECTION_LEN))
                {
                    erase_rst = flash_erase_block((start_addr + length) & ~(FMC_BLK_SECTION_LEN - 1));
                    block_cnt ++;
                }
            }
        }
        else
        {
            /*img_total_len is not align 64KB, need erase sector*/
            if (0 != (img_total_len & (FMC_BLK_SECTION_LEN - 1)))
            {
                if (0 == (offset & (FMC_SEC_SECTION_LEN - 1)))   //new sector starts
                {
                    erase_rst = flash_erase_sector(start_addr);
                }
                else  // only cross two sector
                {
                    if (((offset % FMC_SEC_SECTION_LEN) + length > FMC_SEC_SECTION_LEN)
                        && (length <= FMC_SEC_SECTION_LEN))
                    {
                        erase_rst = flash_erase_sector((start_addr + length) & ~(FMC_SEC_SECTION_LEN - 1));
                    }
                }
            }
        }
    }
    else  //other images
    {
        if (0 == (offset & (FMC_SEC_SECTION_LEN - 1)))   //new sector starts
        {
            erase_rst = flash_erase_sector(start_addr);
        }
        else // only cross two sector
        {
            /*write space cross two sector. Note: dfu_base_addr have been 4K align*/
            /*if buffer size more than 4K, may cross more than two sectors*/
            if (((offset % FMC_SEC_SECTION_LEN) + length > FMC_SEC_SECTION_LEN)
                && (length <= FMC_SEC_SECTION_LEN))
            {
                erase_rst = flash_erase_sector((start_addr + length) & ~(FMC_SEC_SECTION_LEN - 1));
            }
        }
    }
    if (!erase_rst)
    {
        //erase fail
        result = __LINE__;
        goto L_Return;
    }

    //write data to flash
    result = dfu_write(start_addr, length, (uint8_t *)p_void);
    lock_flash_bp();

L_Return:
    DFU_PRINT_INFO1("<==dfu_update: result=%d", result);
    return result;
}


/**
 * @brief calculate checksum of the image and compare with given checksum value.
 *
 * @param   image_id     image id to identify image.
 * @return  true if the image integrity check passes, false otherwise
*/
bool dfu_check_checksum(uint16_t image_id)
{
    uint32_t error_code = 0;
    bool check_result = false;
    uint32_t base_addr = 0;
    T_IMG_CTRL_HEADER_FORMAT *p_header;

    if (image_id == IMAGE_USER_DATA)
    {
        /* because flash_get_bank_addr(FLASH_BKP_DATA1) is not located flash block protect range, needn't unlock bp */
        base_addr = flash_get_bank_addr(FLASH_BKP_DATA1) | FLASH_OFFSET_TO_NO_CACHE;
    }
    else
    {
        /* check OTA temp or running bank to see if received image is OK.*/
#ifdef SUPPORT_ALONE_UPPERSTACK_IMG
        base_addr = get_temp_ota_bank_addr_by_img_id_app((T_IMG_ID)image_id);
#else
        base_addr = get_temp_ota_bank_addr_by_img_id((T_IMG_ID)image_id);
#endif
    }
    if (base_addr == 0)
    {
        error_code = __LINE__;
        goto L_Return;
    }

    p_header = (T_IMG_CTRL_HEADER_FORMAT *)base_addr;
#if (SUPPORT_PUBLIC_DECODE_OTA == 1)
    if (IMAGE_NORMAL == p_header->ctrl_flag.flag_value.image_type)
    {
        check_result = check_image_chksum(p_header);

    }
    else if (IMAGE_COMPRESSED == p_header->ctrl_flag.flag_value.image_type)
    {
        check_result = check_compressed_image_chksum(p_header);
    }
    else
    {
        error_code = __LINE__;
        goto L_Return;
    }
#else
    check_result = check_image_chksum(p_header);
#endif

    if (!check_result)
    {
        error_code = __LINE__;
        goto L_Return;
    }

    unlock_flash_bp_all();
#if (SUPPORT_PUBLIC_DECODE_OTA == 1)
    /*only set compressed_not_reay to 0, and not_ready still is 1
    so that bootlader won't copy compressed image*/
    if (IMAGE_COMPRESSED == p_header->ctrl_flag.flag_value.image_type)
    {
        dfu_set_compressed_ready(p_header);
    }
    else if (IMAGE_NORMAL == p_header->ctrl_flag.flag_value.image_type)
    {
        dfu_set_ready(p_header);
    }
    else
    {
        error_code = __LINE__;
    }
#else
    dfu_set_ready(p_header);
#endif
    lock_flash_bp();

L_Return:
    DFU_PRINT_INFO3("<==dfu_check_checksum: check_result:%d, ota_tmp_addr:0x%x, error_code:%d",
                    check_result, base_addr, error_code);
    return check_result;
}


#if (ENABLE_BANK_SWITCH_COPY_APP_DATA == 1)
extern uint8_t *p_ota_temp_buffer_head;
/**
 * @brief   copy appdata from active bank to updating bank.
 *
 * @param   image_id    image_id to identify image.
 * @param   dlAddress   address the img copy to.
 * @param   dlSize      copy size.
 * @return  true if the image copied success, false otherwise
*/
bool dfu_copy_img(uint16_t image_id, uint32_t dlAddress, uint32_t dlSize)
{
    uint32_t error_code = 0;
    uint32_t source_base_addr;
    uint32_t dest_base_addr;
    int remain_size = dlSize;
    uint32_t s_val;
    uint32_t dlOffset, tmp_offset;

    if ((image_id < AppData1) || (image_id > AppData6))
    {
        error_code = __LINE__;
        goto L_Return;
    }
    if (dlAddress % 4096)
    {
        error_code = __LINE__;
        goto L_Return;
    }

    source_base_addr = get_active_ota_bank_addr() & 0xffffff;

    if (flash_get_bank_addr(FLASH_OTA_BANK_0) == get_active_ota_bank_addr())
    {
        dest_base_addr = flash_get_bank_addr(FLASH_OTA_BANK_1) & 0xffffff;
    }
    else
    {
        dest_base_addr = flash_get_bank_addr(FLASH_OTA_BANK_0) & 0xffffff;
    }
    if ((source_base_addr % 4096) || (dest_base_addr % 4096))
    {
        error_code = __LINE__;
        goto L_Return;
    }
    if (dest_base_addr >= dlAddress)
    {
        error_code = __LINE__;
        goto L_Return;
    }
    dlOffset = dlAddress - dest_base_addr;
    tmp_offset = dlOffset;
    if (dlOffset % 4096)
    {
        error_code = __LINE__;
        goto L_Return;
    }
    T_IMG_HEADER_FORMAT *p_data_header;
    p_data_header = (T_IMG_HEADER_FORMAT *)(source_base_addr + dlOffset);
    if (p_data_header->ctrl_header.image_id != image_id)
    {
        error_code = __LINE__;
        goto L_Return;
    }

    while (remain_size > 0)
    {
        if (!((dest_base_addr + tmp_offset) % 4096)) //must 4k align
        {
            flash_erase_sector(dest_base_addr + tmp_offset);
        }
        if (remain_size > 2048)
        {
            memcpy(p_ota_temp_buffer_head, (uint8_t *)(source_base_addr + tmp_offset), 2048);
            if (remain_size ==  dlSize)
            {
                T_IMG_CTRL_HEADER_FORMAT *p_header = (T_IMG_CTRL_HEADER_FORMAT *) p_ota_temp_buffer_head;
                p_header->ctrl_flag.flag_value.not_ready = 0x1; /*make sure image is not ready, will use it later*/
            }
            for (int i = 0; i < 2048; i = i + 4)
            {
                flash_auto_write_locked(dest_base_addr + tmp_offset + i, *(uint32_t *)p_ota_temp_buffer_head);

                flash_auto_read_locked(dest_base_addr + tmp_offset + i | FLASH_OFFSET_TO_NO_CACHE, &s_val);
                if (s_val != *(uint32_t *)p_ota_temp_buffer_head)
                {
                    DFU_PRINT_TRACE3("s_val:0x%08x, *p_void:0x%08x, i:0x%08x",
                                     s_val, *(uint32_t *)p_ota_temp_buffer_head, i);
                    error_code = __LINE__;
                    goto L_Return;
                }
                else
                {
                    p_ota_temp_buffer_head += 4;
                }
            }


            remain_size -= 2048;
        }
        else
        {
            memcpy(p_ota_temp_buffer_head, (uint8_t *)(source_base_addr + tmp_offset), remain_size);
            for (int i = 0; i < remain_size; i = i + 4)
            {
                flash_auto_write_locked(dest_base_addr + tmp_offset + i, *(uint32_t *)p_ota_temp_buffer_head);

                flash_auto_read_locked(dest_base_addr + tmp_offset + i | FLASH_OFFSET_TO_NO_CACHE, &s_val);
                if (s_val != *(uint32_t *)p_ota_temp_buffer_head)
                {
                    DFU_PRINT_TRACE3("s_val:0x%08x, *p_void:0x%08x, i:0x%08x",
                                     s_val, *(uint32_t *)p_ota_temp_buffer_head, i);
                    error_code = __LINE__;
                    goto L_Return;
                }
                else
                {
                    p_ota_temp_buffer_head += 4;
                }
            }
            remain_size = 0;
        }
        tmp_offset += 2048;
    }

L_Return:
    DFU_PRINT_INFO1("<====dfu_copy_img  error_code:%d", error_code);
    if (error_code)
    {
        return false;
    }
    return true;
}
#endif //end ENABLE_BANK_SWITCH_COPY_APP_DATA







