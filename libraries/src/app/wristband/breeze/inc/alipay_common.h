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

#ifndef ALIPAY_COMMON_H
#define ALIPAY_COMMON_H

#include <string.h>
#include <stdint.h>

#define LIB_VERSION    "V18.03.14"

typedef enum
{
    RV_OK = 0,
    RV_WRITE_ERROR,
    RV_READ_ERROR,
    RV_DEL_ERROR,
    RV_NOT_FOUND,
    RV_IO_ERROR,
    RV_NOT_INITIALISED,
    RV_NETWORK_ERROR,
    RV_ECC_GENERATE_ERROR,
    RV_ECC_SHARE_ERROR,
    RV_BUF_TOO_SHORT,
    RV_ENCRYPTION_ERRO,
    RV_DECRYPTION_ERRO,
    RV_WRONG_TIMESTAMP,
    RV_WRONG_PARAM,
    RV_PRODECT_MODEL_ERROR,
    RV_BINDING_REPEAT,
    RV_SEMAPHORE_CREATE_ERROR,
    RV_DATA_WAITING_TIMEOUT,
    RV_BINDING_DATA_FORMAT_ERROR,
    RV_UNKNOWN
} retval_t;

typedef enum
{
    STATUS_BINDING_FAIL = 0x0,
    STATUS_UNBINDED,
    STATUS_START_BINDING,       //准备环境，进度0
    STATUS_GETTING_PROFILE,     //进度10%，进度>0时说明BLE连接已经OK，可以关闭绑定码，显示进度
    STATUS_SAVING_DATA,         //进度30%
    STATUS_SAVING_DATA_OK,      //进度70%
    STATUS_FINISH_BINDING,      //进度90%
    STATUS_FINISH_BINDING_OK,
    STATUS_BINDING_OK = 0xA5,
    STATUS_UNKNOWN = 0xFF
} binding_status_e;

#if 0
typedef unsigned short     uint16_t;
typedef unsigned char      uint8_t;
typedef long long          int64_t;
typedef unsigned long long uint64_t;
typedef int                int32_t;
typedef unsigned int       uint32_t;
#endif

#if !defined(__cplusplus)
#ifndef bool
typedef unsigned char bool;
#endif
#ifndef false
#define false ((bool)0)
#define true  ((bool)1)
#endif
#endif

#ifdef __cplusplus
#define EXTERNC extern "C"
#else
#define EXTERNC
#endif

#ifndef __DEBUG
#define STATIC static
#else
#define STATIC
#endif

#endif /* common_h */

