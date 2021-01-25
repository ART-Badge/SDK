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

#ifndef ALIPAY_H
#define ALIPAY_H
#include "alipay_common.h"

/**
 * 初始化绑定状态为STATUS_START_BINDING
 * 开始时调用
 */
EXTERNC void alipay_env_init(void);

/**
 * 清除绑定状态数据
 * 退出绑定界面时调用
 */
EXTERNC void alipay_env_deinit(void);

/**
 * 获取绑定码
 * @param [out] binding_str     存放绑定码的指针
 * @param [out] len_binding_str 绑定码数据大小
 * @return  RV_OK：    获取成功
 *          RV_BINDING_STR_TOO_LONG：数据size超过限制，请减少PRODUCT_MODEL的长度
 */
EXTERNC retval_t alipay_get_binding_string(uint8_t *binding_str, uint32_t *len_binding_str);

/**
 * 获取支付码，支付码为18或19位纯数字字符串，条形码编码和显示时应注意
 * @param [out] paycode     存放支付码的指针
 * @param [out] len_paycode 支付码数据大小
 * @return  RV_OK：    获取成功
 *          其他value：获取失败
 */
EXTERNC retval_t alipay_get_paycode(uint8_t *paycode, uint32_t *len_paycode);

/**
 * 支付功能解绑，清除绑定数据
 */
EXTERNC retval_t alipay_unbinding(void);

/**
 * 获取绑定状态
 * @param [in] void
 * @return 返回绑定状态
 */
EXTERNC binding_status_e alipay_get_binding_status(void);

/**
 * 从Flash读取存储的logon_ID
 * @param [out] id 存放数据的指针
 * @param [out] len_id数据大小，最大读取不超过传入这个值的数据大小
 * @return  RV_OK：读取成功
 *          RV_NOT_FOUND：未发现该存储项
 */
EXTERNC retval_t alipay_get_logon_ID(uint8_t *id, uint32_t *len_id);

/**
 * 从Flash读取存储的nick_name
 * @param [out] nick_name 存放数据的指针
 * @param [out] len_nick_name数据大小，最大读取不超过传入这个值的数据大小
 * @return  RV_OK：读取成功
 *          RV_NOT_FOUND：未发现该存储项
 */
EXTERNC retval_t alipay_get_nick_name(uint8_t *nick_name, uint32_t *len_nick_name);

/**
 * BLE数据接收的CallBack函数
 * @param [out] data 存放数据的指针
 * @param [out] len 收到的数据大小
 * @return void
 */
EXTERNC void alipay_recv_data_handle(const uint8_t *data, uint16_t len);

/**
 * Vendor软件如果有对时逻辑，设备与服务端对时成功之后应该调用本接口通知SDK
 * （此处对时特指将本地时间和网络端时间对齐。对齐之后通知SDK即可。）
 */
EXTERNC void alipay_vendor_sync_time_done(void);
#endif
