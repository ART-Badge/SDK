/**
*********************************************************************************************************
*               Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     cp.h
* @brief    apple's authentication coprocessor driver header file.
* @details  none.
* @author   tifnan
* @date     2015-09-09
* @version  v1.0
*********************************************************************************************************
*/

#ifndef _CP_H_
#define _CP_H_

/*============================================================================*
 *                               Header Files
*============================================================================*/
#include <stdint.h>
#include <stdbool.h>

//cp device address, if reset pin pull low, 0x10; if reset pin pull high, 0x11
#define CP_DEVICE_ADDRESS_RESET_POLL_LOW   0x10
#define CP_DEVICE_ADDRESS_RESET_POLL_HIGH  0x11


//cp register address (block 1)
#define CP_REG_DEV_VER      0x00    //RO, device verision
#define CP_REG_FW_VER       0x01    //RO, firmware verision
#define CP_REG_MAJ_VER      0x02    //RO, authentication protocol major verision
#define CP_REG_MIN_VER      0x03    //RO, authentication protocol minor verision
#define CP_dev_id           0x04    //RO, device id, 4bytes(0x00000200)
#define CP_ERR_CODE         0x05    //RO, error code -- read clear

//cp register address (block 2)
#define CP_REG_CS           0x10    //RW, authentication control & status
#define CP_SIG_LEN          0x11    //RW, signature data length, 2bytes(128)
#define CP_SIG_DATA         0x12    //RW, signature data, 128 bytes
#define CP_REG_CHA_LEN      0x20    //RW, challenge data length, 2bytes(20)
#define CP_REG_CHA_DATA     0x21    //RW, challenge data, 128bytes

//cp register address (block 3)
#define CP_REG_ACD_LEN      0x30    //RO, accessory certificate data length, 2bytes(<=1280)
#define CP_REG_ACD1         0x31    //RO, accessory certificate data page1, 128bytes
#define CP_REG_ACD2         0x32    //RO, accessory certificate data page2, 128bytes
#define CP_REG_ACD3         0x33    //RO, accessory certificate data page3, 128bytes
#define CP_REG_ACD4         0x34    //RO, accessory certificate data page4, 128bytes
#define CP_REG_ACD5         0x35    //RO, accessory certificate data page5, 128bytes
#define CP_REG_ACD6         0x36    //RO, accessory certificate data page6, 128bytes
#define CP_REG_ACD7         0x37    //RO, accessory certificate data page7, 128bytes
#define CP_REG_ACD8         0x38    //RO, accessory certificate data page8, 128bytes
#define CP_REG_ACD9         0x39    //RO, accessory certificate data page9, 128bytes
#define CP_REG_ACD10        0x3A    //RO, accessory certificate data page10, 128bytes

//cp register address (block 4)
#define CP_REG_ST_CS        0x40    //RW, self-test control & status
#define CP_REG_SEC          0x4D    //RO, system event counter

//cp register address (block 5)
#define CP_REG_ADCD_LEN     0x50    //RW, apple device certificate data length, 2bytes(0x0000)
#define CP_REG_ADCD1        0x51    //RW, apple device certificate data page1, 128bytes
#define CP_REG_ADCD2        0x52    //RW, apple device certificate data page2, 128bytes
#define CP_REG_ADCD3        0x53    //RW, apple device certificate data page3, 128bytes
#define CP_REG_ADCD4        0x54    //RW, apple device certificate data page4, 128bytes
#define CP_REG_ADCD5        0x55    //RW, apple device certificate data page5, 128bytes
#define CP_REG_ADCD6        0x56    //RW, apple device certificate data page6, 128bytes
#define CP_REG_ADCD7        0x57    //RW, apple device certificate data page7, 128bytes
#define CP_REG_ADCD8        0x58    //RW, apple device certificate data page8, 128bytes



/** @defgroup ACP Authentication Coprocessor
  * @brief apple's authentication coprocessor module driver
  * @{
  */

/*============================================================================*
 *                         Types
 *============================================================================*/
/** @defgroup ACP_Exported_Types Authentication Coprocessor Exported Types
  * @{
  */

//cp process result
/**  @brief  ACP process repsonse value */
typedef enum
{
    CP_PRO_RES_NO_VALID     = 0x80,     //!<Most recent process did not produce valid results
    CP_PRO_RES_SIG_GEN_OK   = 0x10,     //!<Accessory signature successfully generated.
    CP_PRO_RES_CHA_GEN_OK   = 0x20,     //!<Challenge successfully generated.
    CP_PRO_RES_SIG_VEFY_OK  = 0x30,     //!<Apple device signature successfully verified
    CP_PRO_RES_CER_VAL_OK   = 0x40      //!<Apple device certificate successfully validated
} T_CP_PRO_RES;

/**  @brief  ACP command value */
typedef enum
{
    CP_CMD_START_SIG_GEN = 0x01,    //!<start signature generate process
    CP_CMD_START_CHA_GEN,           //!<start challenge generate process
    CP_CMD_START_SIG_VEFY,          //!<start signature-verification process
    CP_CMD_START_CER_VAL            //!<start certificate-validation process
} T_CP_CMD;

//cp error code enum
/**  @brief  ACP error code */
typedef enum
{
    CP_ERR_NO_ERR = 0x00,       //!<No error
    CP_ERR_INV_READ_REG,        //!<Invalid register for read
    CP_ERR_INV_WR_REG,          //!<Invalid register for write
    CP_ERR_INV_SIG_LEN,         //!<Invalid signature length
    CP_ERR_INV_CHA_LEN,         //!<Invalid challenge length
    CP_ERR_INV_CER_LEN,         //!<Invalid certificate length
    CP_ERR_INTR_SIG_GEN,        //!<Internal process error during signature generation
    CP_ERR_INTR_CHA_GEN,        //!<Internal process error during challenge generation
    CP_ERR_INTR_SIG_VEFY,       //!<Internal process error during signature verification
    CP_ERR_INTR_VER_VAL,        //!<Internal process error during certificate validation
    CP_ERR_INV_CTRL,            //!<Invalid process control
    CP_ERR_PRO_OUT_OF_SEQ,      //!<Process control out of sequence
    CP_ERR_RESVD
} T_CP_ERR_CODE;

/**  @brief  ACP device information */
typedef struct
{
    uint8_t dev_ver;
    uint8_t fw_ver;
    uint8_t aup_mar_ver;    //!<authentication protocol major verision
    uint8_t aup_mir_ver;    //!<authentication protocol minor verision
    uint8_t dev_id[4];
} T_CP_INFO;

/**  @brief  ACP function return status */
typedef enum
{
    CP_CAUSE_SUCCESS            = 0x00,
    CP_CAUSE_TIMEOUT            = 0x01,
    CP_CAUSE_INVALID_PARAM      = 0x02,
    CP_CAUSE_INVALID_STATE      = 0x03,
} T_CP_CAUSE;

/** End of ACP_Exported_Types
  * @}
  */

/*============================================================================*
 *                         Functions
 *============================================================================*/
/**
 * @defgroup ACP_EXPORT_Functions Authentication Coprocessor Exported Functions
 * @{
 */
/**
    * @brief  reset CP module
    * @param  void
    * @return none
    */
void cp_reset(void);

/**
    * @brief  set CP module I2C address
    * @param  poll_low  to specify the CP I2C address
    * @return none
    */
void cp_set_slave_address(bool poll_low);

/**
    * @brief  CP hardware init with specific I2C, reset pin number
    * @param  p_i2c     to specify the CP I2C pin
    * @param  reset_pin to specify the CP reset pin
    * @param  poll_low  to specify the CP I2C address
    * @return none
    */
void cp_hw_init(void *p_i2c, uint8_t reset_pin, bool poll_low);
/**
    * @brief  read CP information
    * @param  info     to store the cp information
    * @return function status @ref T_CP_CAUSE
    */
T_CP_CAUSE cp_read_cp_info(T_CP_INFO *info);
/**
    * @brief  read CP signature data
    * @param  sig_buf     to store the cp signature data
    * @param  length      the signature data length
    * @return function status @ref T_CP_CAUSE
    */
T_CP_CAUSE cp_read_signature_data(uint8_t *sig_buf, uint16_t length);

/**
    * @brief  read CP signature length
    * @param  p_signature_len      to store the signature length
    * @return function status @ref T_CP_CAUSE
    */
T_CP_CAUSE cp_read_signature_len(uint16_t *p_signature_len);

/**
    * @brief write challenge info.
    * @param[in] cha_buf challenge info buffer.
    * @param[in] buf_len challenge info buffer length.
    * @return function status @ref T_CP_CAUSE
    * @note the first 2 bytes in challenge info buffer are challenge data length.
*/
T_CP_CAUSE cp_write_cha_info(uint8_t *cha_buf, uint16_t buf_len);

/**
    * @brief read certificate data length.
    * @param p_crf_data_le    to store certificate data length
    * @return function status @ref T_CP_CAUSE
  */
T_CP_CAUSE cp_read_crf_data_len(uint16_t *p_crf_data_le);

/**
  * @brief read certificate data.
  * @param[in] cert_data_page
  * @param[in] data certificate data buffer.
  * @param[in] length certificate data length.
  * @return function status @ref T_CP_CAUSE
  */
T_CP_CAUSE cp_read_crf_data(uint8_t cert_data_page, uint8_t *data, uint16_t length);
/**
  * @brief start cp control process.
  * @param[in] cmd cp control comamnd.
  * @return function status @ref T_CP_CAUSE
  */
T_CP_CAUSE cp_ctrl(T_CP_CMD cmd);
/**
  * @brief write challenge data.
  * @param[in] cha_buf      challenge data buffer.
  * @param[in] buf_len      challenge data buffer length.
  * @return function status @ref T_CP_CAUSE
  */
T_CP_CAUSE cp_write_cha_data(uint8_t *cha_buf, uint16_t buf_len);

/**
  * @brief write challenge data length.
  * @param[in] len      challenge data length.
  * @return function status @ref T_CP_CAUSE
  */
T_CP_CAUSE cp_write_cha_len(uint16_t len);
/**
  * @brief read CP process result.
  * @param[out] p_cp_pro_res_m      resutl buffer
  * @return function status @ref T_CP_CAUSE
  */
T_CP_CAUSE cp_read_proc_result(T_CP_PRO_RES *p_cp_pro_res_m);
/**
  * @brief read CP error code.
  * @param[out] p_cp_err_code_m      error code buffer
  * @return function status @ref T_CP_CAUSE
  */
T_CP_CAUSE cp_read_err_code(T_CP_ERR_CODE *p_cp_err_code_m);

//void cp_test(void);
/** @} */ /* End of group ACP_EXPORT_Functions */
/** @} */ /* End of group ACP */

#endif  /*_CP_H_ */

