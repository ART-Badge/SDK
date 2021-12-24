/****************************************************************************
 * IoTPay modules 1.0 2017/10/19
 * This software is provided "AS IS," without a warranty of any kind. ALL
 * EXPRESS OR IMPLIED CONDITIONS, REPRESENTATIONS AND WARRANTIES, INCLUDING
 * ANY IMPLIED WARRANTY OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 * OR NON-INFRINGEMENT, ARE HEREBY EXCLUDED. ALIPAY, INC.
 * AND ITS LICENSORS SHALL NOT BE LIABLE FOR ANY DAMAGES SUFFERED BY LICENSEE
 * AS A RESULT OF USING, MODIFYING OR DISTRIBUTING THIS SOFTWARE OR ITS
 * DERIVATIVES. IN NO EVENT WILL ALIPAY OR ITS LICENSORS BE LIABLE FOR ANY LOST
 * REVENUE, PROFIT OR DATA, OR FOR DIRECT, INDIRECT, SPECIAL, CONSEQUENTIAL,
 * INCIDENTAL OR PUNITIVE DAMAGES, HOWEVER CAUSED AND REGARDLESS OF THE THEORY
 * OF LIABILITY, ARISING OUT OF THE USE OF OR INABILITY TO USE THIS SOFTWARE,
 * EVEN IF ALIPAY HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 ****************************************************************************
 * You acknowledge that this software is not designed, licensed or intended
 * for use in the design, construction, operation or maintenance of any
 * nuclear facility.
*****************************************************************************/

#include "alipay_common.h"
#include "alipay_vendor.h"
#include "app_flash.h"
#include "ftl.h"
#include "trace.h"
#include "rtl876x_module_data.h"



/////////////////////////////////////////////
////安全存储:读取
////注:为防止存储破坏，需要加上校验值
/// 返回值：
/// RV_OK：读取成功
/// RV_READ_ERROR：读取失败
/// RV_NOT_FOUND：对应数据找不到
/////////////////////////////////////////////
EXTERNC retval_t ALIPAY_storage_read(ALIPAY_storage_item item,
                                     uint8_t *buf_content, uint32_t *len_content)
{
    retval_t ret = RV_OK;
    uint32_t ftl_offset = 0;
    switch (item)
    {
    case ALIPAY_ITEM_PRIVATE_KEY:
        {
            ftl_offset = FTL_ALIPAY_ITEM_PRIVATE_KEY_OFFSET;
            break;
        }
    case ALIPAY_ITEM_SHARE_KEY:
        {
            ftl_offset = FTL_ALIPAY_ITEM_SHARE_KEY_OFFSET;
            break;
        }
    case ALIPAY_ITEM_SEED:
        {
            ftl_offset = FTL_ALIPAY_ITEM_SEED_OFFSET;
            break;
        }
    case ALIPAY_ITEM_TIMEDIFF:
        {
            ftl_offset = FTL_ALIPAY_ITEM_TIMEDIFF_OFFSET;
            break;
        }
    case ALIPAY_ITEM_NICKNAME:
        {
            ftl_offset = FTL_ALIPAY_ITEM_NICKNAME_OFFSET;
            break;
        }
    case ALIPAY_ITEM_LOGONID:
        {
            ftl_offset = FTL_ALIPAY_ITEM_LOGONID_OFFSET;
            break;
        }
    case ALIPAY_ITEM_BINDFLAG:
        {
            ftl_offset = FTL_ALIPAY_ITEM_BINDFLAG_OFFSET;
            break;
        }
    default:
        {
            ret = RV_READ_ERROR;
            break;
        }
    }
    uint8_t item_status = 0;
    if (ftl_load(&item_status, ftl_offset, 4) != 0)
    {
        item_status = 0;
        ret = RV_READ_ERROR;
        return ret;
    }
    else
    {
        if ((item_status & item) == 0)
        {
            ret = RV_READ_ERROR;
            return ret;
        }

    }
    if (ftl_load(buf_content, FTL_ALIPAY_ITEM_PRIVATE_KEY_OFFSET, *len_content) != 0)
    {
        APP_PRINT_INFO0("ALIPAY_storage_read Fail");
        ret = RV_READ_ERROR;
    }

    return ret;
}

/////////////////////////////////////////////
////安全存储:写入
////注: 必须实现设备相关加解密，即写入到A设备
////的内容，即使拷贝到B设备，也无法正常使用
////此外为防止存储破坏，需要加上校验值
/// 返回值：
/// RV_OK：写入成功
/// RV_WRITE_ERROR：写入失败
/////////////////////////////////////////////
EXTERNC retval_t ALIPAY_storage_write(ALIPAY_storage_item item,
                                      const uint8_t *buf_content, uint32_t len_content)
{
    retval_t ret = RV_OK;
    uint32_t ftl_offset = 0;
    switch (item)
    {
    case ALIPAY_ITEM_PRIVATE_KEY:
        {
            ftl_offset = FTL_ALIPAY_ITEM_PRIVATE_KEY_OFFSET;
            break;
        }
    case ALIPAY_ITEM_SHARE_KEY:
        {
            ftl_offset = FTL_ALIPAY_ITEM_PRIVATE_KEY_OFFSET;
            break;
        }
    case ALIPAY_ITEM_SEED:
        {
            ftl_offset = FTL_ALIPAY_ITEM_PRIVATE_KEY_OFFSET;
            break;
        }
    case ALIPAY_ITEM_TIMEDIFF:
        {
            ftl_offset = FTL_ALIPAY_ITEM_PRIVATE_KEY_OFFSET;
            break;
        }
    case ALIPAY_ITEM_NICKNAME:
        {
            ftl_offset = FTL_ALIPAY_ITEM_PRIVATE_KEY_OFFSET;
            break;
        }
    case ALIPAY_ITEM_LOGONID:
        {
            ftl_offset = FTL_ALIPAY_ITEM_PRIVATE_KEY_OFFSET;
            break;
        }
    case ALIPAY_ITEM_BINDFLAG:
        {
            ftl_offset = FTL_ALIPAY_ITEM_PRIVATE_KEY_OFFSET;
            break;
        }
    default:
        {
            ret = RV_UNKNOWN;
            break;
        }
    }
    uint8_t item_status = 0;
    if (ftl_load(&item_status, FTL_ALIPAY_ITEM_STATUS_OFFSET, 4) != 0)
    {
        item_status = 0;
        item_status = item;
        ftl_save(&item_status, FTL_ALIPAY_ITEM_STATUS_OFFSET, 4);
    }
    else
    {
        item_status |= item;
        ftl_save(&item_status, FTL_ALIPAY_ITEM_STATUS_OFFSET, 4);
    }

    if (ftl_save((void *)buf_content, ftl_offset, len_content) != 0)
    {
        APP_PRINT_INFO0("ALIPAY_storage_write Fail");
        ret = RV_UNKNOWN;
    }
    return ret;
}

/////////////////////////////////////////////
////安全存储:删除(需要支持一次删除多个item,
////如:ALIPAY_ITEM_PRIVATE_KEY | ALIPAY_ITEM_SEED)
/// 返回值：
/// RV_OK：删除成功
/// RV_DEL_ERROR：删除失败
/////////////////////////////////////////////
EXTERNC retval_t ALIPAY_storage_delete(uint32_t items)
{
    retval_t ret = RV_OK;

    uint8_t item_status = 0;
    if (ftl_load(&item_status, FTL_ALIPAY_ITEM_STATUS_OFFSET, 4) != 0)
    {
        item_status = 0;
        ftl_save(&item_status, FTL_ALIPAY_ITEM_STATUS_OFFSET, 4);
    }
    else
    {
        item_status &= ((~items) & 0x7f);
        ftl_save(&item_status, FTL_ALIPAY_ITEM_STATUS_OFFSET, 4);
    }
    return ret;
}


/////////////////////////////////////////////
////安全存储:判断是否存在文件
/// 返回值：
/// RV_OK：存在
/// RV_NOT_FOUND：不存在
/////////////////////////////////////////////
EXTERNC retval_t ALIPAY_storage_exists(ALIPAY_storage_item item)
{
    retval_t ret = RV_OK;

    uint8_t item_status = 0;
    if (ftl_load(&item_status, FTL_ALIPAY_ITEM_STATUS_OFFSET, 4) != 0)
    {
        item_status = 0;
        ftl_save(&item_status, FTL_ALIPAY_ITEM_STATUS_OFFSET, 4);
        ret = RV_NOT_FOUND;
    }
    else
    {
        if ((item_status & item) == 0)
        {
            ret = RV_NOT_FOUND;
        }
    }
    return ret;
}


/////////////////////////////////////////////
////获取当前系统时间戳
/////////////////////////////////////////////
EXTERNC uint32_t ALIPAY_get_timestamp(void)
{
    return RtkWristbandSys.SecondCountRTC;
}

/*BLE写接口:
 * @param [in] data 待发送数据的指针
 * @param [in] len  待发送数据的大小
 * @return void
 */
EXTERNC void ALIPAY_ble_write(uint8_t *data, uint16_t len)
{

}

/////////////////////////////////////////////
////获取设备ID(以冒号分割的16进制mac地址)
////要求内容以‘\0’结尾且长度不包含'\0'
/////////////////////////////////////////////
EXTERNC retval_t ALIPAY_get_deviceid(uint8_t *buf_did, uint32_t *len_did)
{
    uint8_t addr[6];
    if (GAP_CAUSE_SUCCESS == gap_get_param(GAP_PARAM_BD_ADDR, addr))
    {
        for (int i = 0; i < 6; i++)
        {
            buf_did[2 * i] = addr[5 - i];
        }
        for (int i = 0; i < 6; i++)
        {
            buf_did[2 * i + 1] = ':';
        }
        buf_did[2 * 5 + 1] = '\0';
        *len_did = 6 + 6;
        return RV_OK;
    }
    else
    {
        return RV_UNKNOWN;
    }
}


/////////////////////////////////////////////////////////////////////////////////////
// 由于有些设备上不支持输出动态string, 所以没有定义成变长参数，
// 只会输出一些关键的trace log, 便于发现问题时快速定位
// 注: format参数中可能含有静态string, 实现时不可忽略
/////////////////////////////////////////////////////////////////////////////////////
EXTERNC void ALIPAY_log(ALIPAY_log_level level, const char *format, int32_t value)
{

}
//EXTERNC void ALIPAY_log_ext(const char *format, ...);







