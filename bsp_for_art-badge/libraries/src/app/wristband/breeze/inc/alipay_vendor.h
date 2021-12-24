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

#ifndef ALIPAY_VENDOR_H
#define ALIPAY_VENDOR_H

#include "alipay_common.h"

/*****************产品型号定义**************************
**
** 产品型号需要在.c文件中定义，定义格式如下例子
** 定义时务必保留开头的'#'字符，并且不允许加入新的'#'字符
** 长度必须<=19，建议<=15
**
**const uint8_t PRODUCT_MODEL[19] = "#ALIPAY_W100";   //lenth must <= 19  recommend <=15
*******************************************************/
extern const uint8_t PRODUCT_MODEL[19];

typedef enum
{
    // private key
    ALIPAY_ITEM_PRIVATE_KEY = 0x01 << 0,
    // shared key based on private key and server's public key
    ALIPAY_ITEM_SHARE_KEY   = 0x01 << 1,
    // encrypted seed from server side
    ALIPAY_ITEM_SEED        = 0x01 << 2,
    // time diff between iot client and server
    ALIPAY_ITEM_TIMEDIFF    = 0x01 << 3,
    // nick name for user
    ALIPAY_ITEM_NICKNAME    = 0x01 << 4,
    // logon ID for user
    ALIPAY_ITEM_LOGONID     = 0x01 << 5,
    // binding flag, see binding_status_e for value
    ALIPAY_ITEM_BINDFLAG    = 0x01 << 6,
} ALIPAY_storage_item;

/////////////////////////////////////////////
////安全存储:读取
////注:为防止存储破坏，需要加上校验值
/// 返回值：
/// RV_OK：读取成功
/// RV_READ_ERROR：读取失败
/// RV_NOT_FOUND：对应数据找不到
/////////////////////////////////////////////
EXTERNC retval_t ALIPAY_storage_read(ALIPAY_storage_item item,
                                     uint8_t *buf_content, uint32_t *len_content);

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
                                      const uint8_t *buf_content, uint32_t len_content);

/////////////////////////////////////////////
////安全存储:删除(需要支持一次删除多个item,
////如:ALIPAY_ITEM_PRIVATE_KEY | ALIPAY_ITEM_SEED)
/// 返回值：
/// RV_OK：删除成功
/// RV_DEL_ERROR：删除失败
/////////////////////////////////////////////
EXTERNC retval_t ALIPAY_storage_delete(uint32_t items);


/////////////////////////////////////////////
////安全存储:判断是否存在文件
/// 返回值：
/// RV_OK：存在
/// RV_NOT_FOUND：不存在
/////////////////////////////////////////////
EXTERNC retval_t ALIPAY_storage_exists(ALIPAY_storage_item item);


/////////////////////////////////////////////
////获取当前系统时间戳
/////////////////////////////////////////////
EXTERNC uint32_t ALIPAY_get_timestamp(void);

/*BLE写接口:
 * @param [in] data 待发送数据的指针
 * @param [in] len  待发送数据的大小
 * @return void
 */
EXTERNC void ALIPAY_ble_write(uint8_t *data, uint16_t len);

/////////////////////////////////////////////
////获取设备ID(以冒号分割的16进制mac地址)
////要求内容以‘\0’结尾且长度不包含'\0'
/////////////////////////////////////////////
EXTERNC retval_t ALIPAY_get_deviceid(uint8_t *buf_did, uint32_t *len_did);

/////////////////////////////////////////////
////日志信息输出接口
/////////////////////////////////////////////
typedef enum
{
    ALIPAY_LEVEL_FATAL = 0x01,
    ALIPAY_LEVEL_ERRO,
    ALIPAY_LEVEL_WARN,
    ALIPAY_LEVEL_INFO,
    ALIPAY_LEVEL_DBG,
} ALIPAY_log_level;

/////////////////////////////////////////////////////////////////////////////////////
// 由于有些设备上不支持输出动态string, 所以没有定义成变长参数，
// 只会输出一些关键的trace log, 便于发现问题时快速定位
// 注: format参数中可能含有静态string, 实现时不可忽略
/////////////////////////////////////////////////////////////////////////////////////
EXTERNC void ALIPAY_log(ALIPAY_log_level level, const char *format, int32_t value);
//EXTERNC void ALIPAY_log_ext(const char *format, ...);
#endif






