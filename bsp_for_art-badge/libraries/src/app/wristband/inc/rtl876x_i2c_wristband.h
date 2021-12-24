/**
*********************************************************************************************************
*               Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* @file      rtl876x_i2c.h
* @brief
* @details
* @author    elliot chen
* @date      2015-4-20
* @version   v1.0
* *********************************************************************************************************
*/

#ifndef __RTL876X_I2C_H
#define __RTL876X_I2C_H

#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include "rtl876x.h"
#include "rtl876x_bitfields.h"


/** @addtogroup I2C I2C
  * @brief I2C driver module
  * @{
  */

/*============================================================================*
 *                         Types
 *============================================================================*/


/** @defgroup I2C_Exported_Types I2C Exported Types
  * @{
  */

/**
  * @brief  I2C Init structure definition
  */

typedef struct
{
    uint32_t I2C_Clock;           /*!< the clock frequency,default 40000000.
                                                This parameter must be set with I2C clock div */

    uint32_t I2C_ClockSpeed;      /*!< Specifies the clock frequency.
                                                This parameter must be set to a value lower than 400kHz */

    uint16_t I2C_DeviveMode;      /*!< Specifies the I2C devie mode.
                                                This parameter can be a value of @ref I2C_device_mode */

    uint16_t I2C_AddressMode;     /*!< Specifies the I2C address mode.
                                                This parameter can be a value of @ref I2C_address_mode */

    uint16_t I2C_SlaveAddress;    /*!< Specifies the first device own address.
                                                This parameter can be a 7-bit or 10-bit address. */

    uint16_t I2C_Ack;             /*!< Enables or disables the acknowledgement only in slave mode.
                                                This parameter can be a value of @ref I2C_acknowledgement */

    uint32_t I2C_TxThresholdLevel;  /* !< Specifies the transmit FIFO Threshold to trigger interrupt I2C_INT_TX_EMPTY.
                                                This parameter can be a value less than 24*/

    uint32_t I2C_RxThresholdLevel;  /* !<Specifies the receive FIFO Threshold to trigger interrupt I2C_INT_RX_FULL.
                                                This parameter can be a value less than 16*/

    uint16_t I2C_TxDmaEn;               /*!< Specifies the Tx dma mode.
                                                This parameter can be a value of ENABLE or DISABLE*/

    uint16_t I2C_RxDmaEn;               /*!< Specifies the Rx dma mode
                                                This parameter can be a value of ENABLE or DISABLE*/

    uint8_t  I2C_TxWaterlevel;          /*!< Specifies the DMA tx water level
                                                This parameter should be I2C Tx fifo depth minus GDMA MSize */

    uint8_t  I2C_RxWaterlevel;          /*!< Specifies the DMA rx water level
                                                This parameter should be I2C Rx GDMA MSize - 1 */
    uint8_t I2C_SDA_Pin;
    uint8_t I2C_SCL_Pin;

} I2C_InitTypeDef;

/** End of group I2C_Exported_Types
  * @}
  */

/*============================================================================*
 *                         Constants
 *============================================================================*/


/** @defgroup I2C_Exported_Constants I2C Exported Constants
  * @{
  */

#define IS_I2C_ALL_PERIPH(PERIPH) (((PERIPH) == I2C0) || \
                                   ((PERIPH) == I2C1))

/** @defgroup I2C_clock_speed I2C Clock Speed
  * @{
  */

#define IS_I2C_CLOCK_SPEED(SPEED) (((SPEED) >= 0x01) && ((SPEED) <= 400000))

/** End of group I2C_clock_speed
  * @}
  */

/** @defgroup I2C_device_mode I2C Device Mode
  * @{
  */

#define I2C_DeviveMode_Master                   ((uint16_t)0x0041)
#define I2C_DeviveMode_Slave                    ((uint16_t)0x0000)

/** End of group I2C_device_mode
  * @}
  */

/** @defgroup I2C_address_mode I2C Address Mode
  * @{
  */

#define I2C_AddressMode_7BIT                    ((uint16_t)0x0000)
#define I2C_AddressMode_10BIT                   ((uint16_t)0x0001)

/** End of group I2C_address_mode
  * @}
  */

/** @defgroup I2C_acknowledgement I2C Acknowledgement
  * @{
  */

#define I2C_Ack_Enable                          ((uint16_t)0x0001)
#define I2C_Ack_Disable                         ((uint16_t)0x0000)

/** End of group I2C_acknowledgement
  * @}
  */

/** @defgroup I2C_flags_definition I2C Flags Definition
  * @{
  */

#define I2C_FLAG_SLV_ACTIVITY                   ((uint32_t)0x00000040)
#define I2C_FLAG_MST_ACTIVITY                   ((uint32_t)0x00000020)
#define I2C_FLAG_RFF                            ((uint32_t)0x00000010)
#define I2C_FLAG_RFNE                           ((uint32_t)0x00000008)
#define I2C_FLAG_TFE                            ((uint32_t)0x00000004)
#define I2C_FLAG_TFNF                           ((uint32_t)0x00000002)
#define I2C_FLAG_ACTIVITY                       ((uint32_t)0x00000001)

#define IS_I2C_GET_FLAG(FLAG) (((FLAG) == I2C_FLAG_SLV_ACTIVITY) || ((FLAG) == I2C_FLAG_MST_ACTIVITY) || \
                               ((FLAG) == I2C_FLAG_RFF) || ((FLAG) == I2C_FLAG_RFNE) || \
                               ((FLAG) == I2C_FLAG_TFE) || ((FLAG) == I2C_FLAG_TFNF) || \
                               ((FLAG) == I2C_FLAG_ACTIVITY))
/** End of group I2C_flags_definition
  * @}
  */

/** @defgroup I2C_transmit_Abort_Source I2C Transmit Abort Source
  * @{
  */

#define ABRT_SLVRD_INTX                         ((uint32_t)BIT(15))
#define ABRT_SLV_ARBLOST                        ((uint32_t)BIT(14))
#define ABRT_SLVFLUSH_TXFIFO                    ((uint32_t)BIT(13))
#define ARB_LOST                                ((uint32_t)BIT(12))
#define ABRT_MASTER_DIS                         ((uint32_t)BIT(11))
#define ABRT_10B_RD_NORSTRT                     ((uint32_t)BIT(10))
#define ABRT_SBYTE_NORSTRT                      ((uint32_t)BIT(9))
#define ABRT_HS_NORSTRT                         ((uint32_t)BIT(8))
#define ABRT_SBYTE_ACKDET                       ((uint32_t)BIT(7))
#define ABRT_HS_ACKDET                          ((uint32_t)BIT(6))
#define ABRT_GCALL_READ                         ((uint32_t)BIT(5))
#define ABRT_GCALL_NOACK                        ((uint32_t)BIT(4))
#define ABRT_TXDATA_NOACK                       ((uint32_t)BIT(3))
#define ABRT_10ADDR2_NOACK                      ((uint32_t)BIT(2))
#define ABRT_10ADDR1_NOACK                      ((uint32_t)BIT(1))
#define ABRT_7B_ADDR_NOACK                      ((uint32_t)BIT(0))

#define MS_ALL_ABORT                            (ARB_LOST | ABRT_MASTER_DIS | ABRT_TXDATA_NOACK |\
                                                 ABRT_10ADDR2_NOACK | ABRT_10ADDR1_NOACK | ABRT_7B_ADDR_NOACK)

#define IS_I2C_EVENT(EVENT) (((EVENT) == ABRT_SLVRD_INTX) || \
                             ((EVENT) == ABRT_SLV_ARBLOST) || \
                             ((EVENT) == ABRT_SLVFLUSH_TXFIFO) || \
                             ((EVENT) == ARB_LOST) || \
                             ((EVENT) == ABRT_MASTER_DIS) || \
                             ((EVENT) == ABRT_10B_RD_NORSTRT) || \
                             ((EVENT) == ABRT_SBYTE_NORSTRT) || \
                             ((EVENT) == ABRT_HS_NORSTRT) || \
                             ((EVENT) == ABRT_SBYTE_ACKDET) || \
                             ((EVENT) == ABRT_HS_ACKDET) || \
                             ((EVENT) == ABRT_GCALL_READ) || \
                             ((EVENT) == ABRT_GCALL_NOACK) || \
                             ((EVENT) == ABRT_TXDATA_NOACK) || \
                             ((EVENT) == ABRT_10ADDR2_NOACK) || \
                             ((EVENT) == ABRT_10ADDR1_NOACK) || \
                             ((EVENT) == ABRT_7B_ADDR_NOACK))
/** End of group I2C_transmit_Abort_Source
  * @}
  */

/** @defgroup I2C_interrupts_definition I2C Interrupts Definition
  * @{
  */

#define I2C_INT_GEN_CALL                                ((uint32_t)BIT(11))
#define I2C_INT_START_DET                               ((uint32_t)BIT(10))
#define I2C_INT_STOP_DET                                ((uint32_t)BIT(9))
#define I2C_INT_ACTIVITY                                ((uint32_t)BIT(8))
#define I2C_INT_RX_DONE                                 ((uint32_t)BIT(7))
#define I2C_INT_TX_ABRT                                 ((uint32_t)BIT(6))
#define I2C_INT_RD_REQ                                  ((uint32_t)BIT(5))
#define I2C_INT_TX_EMPTY                                ((uint32_t)BIT(4))
#define I2C_INT_TX_OVER                                 ((uint32_t)BIT(3))
#define I2C_INT_RX_FULL                                 ((uint32_t)BIT(2))
#define I2C_INT_RX_OVER                                 ((uint32_t)BIT(1))
#define I2C_INT_RX_UNDER                                ((uint32_t)BIT(0))

#define I2C_GET_INT(INT)    (((INT) == I2C_INT_GEN_CALL) || ((INT) == I2C_INT_START_DET) || \
                             ((INT) == I2C_INT_STOP_DET) || ((INT) == I2C_INT_ACTIVITY) || \
                             ((INT) == I2C_INT_RX_DONE)  || ((INT) == I2C_INT_TX_ABRT) || \
                             ((INT) == I2C_INT_RD_REQ)   || ((INT) == I2C_INT_TX_EMPTY) || \
                             ((INT) == I2C_INT_TX_OVER)  || ((INT) == I2C_INT_RX_FULL) || \
                             ((INT) == I2C_INT_RX_OVER)  || ((INT) == I2C_INT_RX_UNDER))
/** End of group I2C_interrupts_definition
  * @}
  */

/** @defgroup I2C_GDMA_transfer_requests  I2C GDMA transfer requests
  * @{
  */

#define I2C_GDMAReq_Tx               ((uint16_t)0x0002)
#define I2C_GDMAReq_Rx               ((uint16_t)0x0001)
#define IS_I2C_GDMAREQ(GDMAREQ) ((((GDMAREQ) & (uint16_t)0xFFFC) == 0x00) && ((GDMAREQ) != 0x00))

/** End of group I2C_GDMA_transfer_requests
  * @}
  */

/** @defgroup I2C_send_command  I2C send command
  * @{
  */

#define I2C_WRITE_CMD       0
#define I2C_READ_CMD        BIT(8)
#define IS_I2C_CMD(CMD) (((CMD) == I2C_WRITE_CMD) || ((CMD) == I2C_READ_CMD))

/** End of group I2C_send_command
  * @}
  */

/** @defgroup I2C_send_stop  I2C send stop
  * @{
  */

#define I2C_STOP_ENABLE     BIT(9)
#define I2C_STOP_DISABLE    0
#define IS_I2C_STOP(CMD) (((CMD) == I2C_STOP_ENABLE) || ((CMD) == I2C_STOP_DISABLE))

/** End of group I2C_send_stop
  * @}
  */

/** @defgroup I2C_Status  I2C status
  * @{
  */

typedef enum
{
    I2C_Success,
    I2C_ARB_LOST,
    I2C_ABRT_MASTER_DIS,
    I2C_ABRT_TXDATA_NOACK,
    I2C_ABRT_10ADDR2_NOACK,
    I2C_ABRT_10ADDR1_NOACK,
    I2C_ABRT_7B_ADDR_NOACK,
    I2C_ERR_TIMEOUT
} I2C_Status;


/** End of group I2C_Status
  * @}
  */

/** End of group I2C_Exported_Constants
  * @}
  */

/*============================================================================*
 *                         Functions
 *============================================================================*/

/** @defgroup I2C_Exported_functions I2C Exported Functions
 * @{
 */

/**
  * @brief  Deinitializes the I2Cx peripheral registers to their default reset values.
  * @param  I2Cx: where x can be 0 or 1 to select the I2C peripheral.
  * @retval None
  */
void I2C_DeInit(I2C_TypeDef *I2Cx);

/**
  * @brief  Initializes the I2Cx peripheral according to the specified
  *   parameters in the I2C_InitStruct.
  * @param  I2Cx: where x can be 0 or 1 to select the I2C peripheral.
  * @param  I2C_InitStruct: pointer to a I2C_InitTypeDef structure that
  *   contains the configuration information for the specified I2C peripheral.
  * @retval None
  */
void I2C_Init(I2C_TypeDef *I2Cx, I2C_InitTypeDef *I2C_InitStruct);

/**
  * @brief  Enables or disables the specified I2C peripheral.
  * @param  I2Cx: where x can be 0 or 1 to select the I2C peripheral.
  * @param  NewState: new state of the I2Cx peripheral.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void I2C_Cmd(I2C_TypeDef *I2Cx, FunctionalState NewState);

/**
  * @brief  Fills each I2C_InitStruct member with its default value.
  * @param  I2C_InitStruct : pointer to a I2C_InitTypeDef structure which will be initialized.
  * @retval None
  */
void I2C_StructInit(I2C_InitTypeDef *I2C_InitStruct);

/**
  * @brief  Send data in master mode through the I2Cx peripheral.
  * @param  I2Cx: where x can be 0 or 1 to select the I2C peripheral.
  * @param  pBuf: Byte to be transmitted..
  * @param  len: data buffer length
  * @retval None
  */
I2C_Status I2C_MasterWrite(I2C_TypeDef *I2Cx, uint8_t *pBuf, uint16_t len);

/**
  * @brief  Read data in master mode through the I2Cx peripheral.
  * @param  I2Cx: where x can be 0 or 1 to select the I2C peripheral.
  * @param  pBuf: data buffer for receive
  * @param  len: data buffer length
  * @return I2C status @ref I2C_Status
  */
I2C_Status I2C_MasterRead(I2C_TypeDef *I2Cx, uint8_t *pBuf, uint16_t len);

/**
  * @brief  Sends data and read data in master mode through the I2Cx peripheral.Attention:Read data with time out mechanism.
  * @param  I2Cx: where x can be 0 or 1 to select the I2C peripheral.
  * @param  pWriteBuf: data buffer to send before read
  * @param  Writelen:  data length to send
  * @param  pReadBuf:  data buffer to receive
  * @param  Readlen:   data length to receive
  * @return I2C status @ref I2C_Status
  */
I2C_Status I2C_RepeatRead(I2C_TypeDef *I2Cx, uint8_t *pWriteBuf, uint16_t Writelen,
                          uint8_t *pReadBuf, uint16_t Readlen);

/**
  * @brief mask the specified I2C interrupt.
  * @param  I2Cx: where x can be 0 or 1
  * @param  I2C_IT
  * This parameter can be one of the following values:
  *     @arg I2C_INT_GEN_CALL: Set only when a General Call address is received and it is acknowledged.
  *     @arg I2C_INT_START_DET: Indicates whether a START or RESTART condition has occurred on the I2C
                              interface regardless of whether I2C is operating in slave or master mode.
  *     @arg I2C_INT_STOP_DET:  Indicates whether a STOP condition has occurred on the I2C interface regardless
                              of whether I2C is operating in slave or master mode
  *     @arg I2C_INT_ACTIVITY:  This bit captures I2C activity and stays set until it is cleared.
  *     @arg I2C_INT_RX_DONE:   When the I2C is acting as a slave-transmitter, this bit is set to 1 if the
                              master does not acknowledge a transmitted byte. This occurs on the last byte of
                              the transmission, indicating that the transmission is done.
  *     @arg I2C_INT_TX_ABRT:   This bit indicates if I2C as an I2C transmitter, is unable to complete the
                              intended actions on the contents of the transmit FIFO.
  *     @arg I2C_INT_RD_REQ:    This bit is set to 1 when acting as a slave and another I2C master
                              is attempting to read data.
  *     @arg I2C_INT_TX_EMPTY:  This bit is set to 1 when the transmit buffer is at or below the threshold value set
                              in the IC_TX_TL register.
  *     @arg I2C_INT_TX_OVER:   Set during transmit if the transmit buffer is filled to IC_TX_BUFFER_DEPTH and
                              the processor attempts to issue another I2C command.
  *     @arg I2C_INT_RX_FULL:   Set when the receive buffer reaches or goes above the RX_TL threshold in the
                              IC_RX_TL register
  *     @arg I2C_INT_RX_OVER:   Set if the receive buffer is completely filled to IC_RX_BUFFER_DEPTH and an
                               additional byte is received from an external I2C device.
  *     @arg I2C_INT_RX_UNDER:   Set if the processor attempts to read the receive buffer when it is empty by reading.
  * @param  NewState    disable or enable I2c interrupt
  * @retval None.
  */
void I2C_INTConfig(I2C_TypeDef *I2Cx, uint16_t I2C_IT, FunctionalState NewState);

/**
  * @brief clear the specified I2C interrupt.
  * @param  I2Cx: where x can be 0 or 1
  * @param  I2C_IT
  * This parameter can be one of the following values:
  *     @arg I2C_INT_GEN_CALL: Set only when a General Call address is received and it is acknowledged.
  *     @arg I2C_INT_START_DET: Indicates whether a START or RESTART condition has occurred on the I2C
                              interface regardless of whether I2C is operating in slave or master mode.
  *     @arg I2C_INT_STOP_DET:  Indicates whether a STOP condition has occurred on the I2C interface regardless
                              of whether I2C is operating in slave or master mode
  *     @arg I2C_INT_ACTIVITY:  This bit captures I2C activity and stays set until it is cleared.
  *     @arg I2C_INT_RX_DONE:   When the I2C is acting as a slave-transmitter, this bit is set to 1 if the
                              master does not acknowledge a transmitted byte. This occurs on the last byte of
                              the transmission, indicating that the transmission is done.
  *     @arg I2C_INT_TX_ABRT:   This bit indicates if I2C as an I2C transmitter, is unable to complete the
                              intended actions on the contents of the transmit FIFO.
  *     @arg I2C_INT_RD_REQ:    This bit is set to 1 when acting as a slave and another I2C master
                              is attempting to read data.
  *     @arg I2C_INT_TX_EMPTY:  This bit is set to 1 when the transmit buffer is at or below the threshold value set
                              in the IC_TX_TL register.
  *     @arg I2C_INT_TX_OVER:   Set during transmit if the transmit buffer is filled to IC_TX_BUFFER_DEPTH and
                              the processor attempts to issue another I2C command.
  *     @arg I2C_INT_RX_FULL:   Set when the receive buffer reaches or goes above the RX_TL threshold in the
                              IC_RX_TL register
  *     @arg I2C_INT_RX_OVER:   Set if the receive buffer is completely filled to IC_RX_BUFFER_DEPTH and an
                               additional byte is received from an external I2C device.
  *     @arg I2C_INT_RX_UNDER:   Set if the processor attempts to read the receive buffer when it is empty by reading.

  * @retval None.
  */
void I2C_ClearINTPendingBit(I2C_TypeDef *I2Cx, uint16_t I2C_IT);

/**
  * @brief  Configure slave device address.
  * @param  I2Cx: where x can be 0 or 1 to select the I2C peripheral.
  * @param  Address: specifies the slave address which will be transmitte.
  * @retval None.
  */
__STATIC_INLINE void I2C_SetSlaveAddress(I2C_TypeDef *I2Cx, uint16_t Address)
{
    /* Check the parameters */
    assert_param(IS_I2C_ALL_PERIPH(I2Cx));

    /* Mask target address */
    I2Cx->IC_TAR &= ~((uint16_t)0x3ff);
    /* Configure new target address */
    I2Cx->IC_TAR |= Address & 0x3ff;
}

/**
  * @brief  write command through the I2Cx peripheral.
  * @param  I2Cx: where x can be 0 or 1 to select the I2C peripheral.
  *@param   command: command of write or read.
                  I2C_READ_CMD: read command. data which want to transmit can be 0 in this situation.
                  I2C_WRITE_CMD: write command.
  * @param  data: data which to be transmitted.
  *@param   StopState: command of write or read.
             I2C_STOP_ENABLE: send stop signal.
                  I2C_STOP_DISABLE: do not send stop signal.
  * @retval None.
  */
__STATIC_INLINE void I2C_SendCmd(I2C_TypeDef *I2Cx, uint16_t command, uint8_t data,
                                 uint16_t StopState)
{
    /* Check the parameters */
    assert_param(IS_I2C_ALL_PERIPH(I2Cx));
    assert_param(IS_I2C_CMD(command));
    assert_param(IS_I2C_STOP(StopState));

    I2Cx->IC_DATA_CMD = data | command | StopState;
}

/**
  * @brief  Returns the most recent received data by the I2Cx peripheral.
  * @param  I2Cx: where x can be 1 or 2 to select the I2C peripheral.
  * @retval The value of the received data.
  */
__STATIC_INLINE uint8_t I2C_ReceiveData(I2C_TypeDef *I2Cx)
{
    /* Check the parameters */
    assert_param(IS_I2C_ALL_PERIPH(I2Cx));

    /* Return the data in the DR register */
    return (uint8_t)I2Cx->IC_DATA_CMD;
}

/**
  * @brief  read  data length in Rx FIFO through the I2Cx peripheral.
  * @param  I2Cx: where x can be 0 or 1
  * @retval None
  */
__STATIC_INLINE uint8_t I2C_GetRxFIFOLen(I2C_TypeDef *I2Cx)
{
    /* Check the parameters */
    assert_param(IS_I2C_ALL_PERIPH(I2Cx));

    return (uint8_t)I2Cx->IC_RXFLR;
}

/**
  * @brief  read data length in Tx FIFO through the I2Cx peripheral.
  * @param  I2Cx: where x can be 0 or 1
  * @retval None
  */
__STATIC_INLINE uint8_t I2C_GetTxFIFOLen(I2C_TypeDef *I2Cx)
{
    /* Check the parameters */
    assert_param(IS_I2C_ALL_PERIPH(I2Cx));

    return (uint8_t)I2Cx->IC_TXFLR;
}

/**
  * @brief clear all of I2C interrupt.
  * @param  I2Cx: where x can be 0 or 1
  */
__STATIC_INLINE void I2C_ClearAllINT(I2C_TypeDef *I2Cx)
{
    /* Check the parameters */
    assert_param(IS_I2C_ALL_PERIPH(I2Cx));

    I2Cx->IC_CLR_INTR;
}

/**
  * @brief  Checks whether the specified I2C flag is set or not.
  * @param  I2Cx: where x can be 0 or 1 to select the I2C peripheral.
  * @param  I2C_FLAG: specifies the flag to check.
  *   This parameter can be one of the following values:
  *     @arg I2C_FLAG_SLV_ACTIVITY:
  *     @arg I2C_FLAG_MST_ACTIVITY:
  *     @arg I2C_FLAG_RFF:
  *     @arg I2C_FLAG_RFNE:
  *     @arg I2C_FLAG_TFE:
  *     @arg I2C_FLAG_TFNF:
  *     @arg I2C_FLAG_ACTIVITY:
  * @retval The new state of I2C_FLAG (SET or RESET).
  */
__STATIC_INLINE FlagStatus I2C_GetFlagState(I2C_TypeDef *I2Cx, uint32_t I2C_FLAG)
{
    FlagStatus bit_status = RESET;

    /* Check the parameters */
    assert_param(IS_I2C_ALL_PERIPH(I2Cx));
    assert_param(IS_I2C_GET_FLAG(I2C_FLAG));

    if ((I2Cx->IC_STATUS & I2C_FLAG) != (uint32_t)RESET)
    {
        /* I2C_FLAG is set */
        bit_status = SET;
    }

    /* Return the I2C_FLAG status */
    return  bit_status;
}

/**
  * @brief  Checks whether the last I2Cx Event is equal to the one passed
  *   as parameter.
  * @param  I2Cx: where x can be 1 or 2 to select the I2C peripheral.
  * @param  I2C_EVENT: specifies the event to be checked about I2C Transmit Abort Status Register.
  *   This parameter can be one of the following values:
  *     @arg ABRT_SLVRD_INTX
  *     @arg ABRT_SLV_ARBLOST
  *     @arg ABRT_SLVFLUSH_TXFIFO
  *     @arg ARB_LOST                   Master has lost arbitration
  *     @arg ABRT_MASTER_DIS          User tries to initiate a Master operation with the Master mode disabled
  *     @arg ABRT_10B_RD_NORSTRT
  *     @arg ABRT_SBYTE_NORSTRT
  *     @arg ABRT_HS_NORSTRT
  *     @arg ABRT_SBYTE_ACKDET
  *     @arg ABRT_HS_ACKDET
  *     @arg ABRT_GCALL_READ
  *     @arg ABRT_GCALL_NOACK
  *     @arg ABRT_TXDATA_NOACK   Master did not receive an acknowledge from the remote slave.
  *     @arg ABRT_10ADDR2_NOACK  Master is in 10-bit address mode and the second address byte of the 10-bit address was not acknowledged by any slave.
  *     @arg ABRT_10ADDR1_NOACK  Master is in 10-bit address mode and the first 10-bit address byte was not acknowledged by any slave.
  *     @arg ABRT_7B_ADDR_NOACK   Master is in 7-bit addressing mode and th address sent was not acknowledged by any slave.
  * @retval An ErrorStatus enumeration value:
  * - SUCCESS: Last event is equal to the I2C_EVENT
  * - ERROR: Last event is different from the I2C_EVENT
  */
__STATIC_INLINE FlagStatus I2C_CheckEvent(I2C_TypeDef *I2Cx, uint32_t I2C_EVENT)
{
    FlagStatus bit_status = RESET;

    /* Check the parameters */
    assert_param(IS_I2C_ALL_PERIPH(I2Cx));
    assert_param(IS_I2C_EVENT(I2C_EVENT));

    if ((I2Cx->IC_TX_ABRT_SOURCE & I2C_EVENT) != (uint32_t)RESET)
    {

        bit_status = SET;
    }

    /* Return the I2C event status */
    return  bit_status;
}

/**
  * @brief Get the specified I2C interrupt status.
  * @param  I2Cx: where x can be 0 or 1.
  * @param  I2C_IT
  * This parameter can be one of the following values:
  *     @arg I2C_INT_GEN_CALL: Set only when a General Call address is received and it is acknowledged.
  *     @arg I2C_INT_START_DET: Indicates whether a START or RESTART condition has occurred on the I2C
                              interface regardless of whether DW_apb_i2c is operating in slave or master mode.
  *     @arg I2C_INT_STOP_DET:  Indicates whether a STOP condition has occurred on the I2C interface regardless
                              of whether DW_apb_i2c is operating in slave or master mode
  *     @arg I2C_INT_ACTIVITY:  This bit captures DW_apb_i2c activity and stays set until it is cleared.
  *     @arg I2C_INT_RX_DONE:   When the DW_apb_i2c is acting as a slave-transmitter, this bit is set to 1 if the
                              master does not acknowledge a transmitted byte. This occurs on the last byte of
                              the transmission, indicating that the transmission is done.
  *     @arg I2C_INT_TX_ABRT:   This bit indicates if DW_apb_i2c, as an I2C transmitter, is unable to complete the
                              intended actions on the contents of the transmit FIFO.
  *     @arg I2C_INT_RD_REQ:    This bit is set to 1 when acting as a slave and another I2C master
                              is attempting to read data.
  *     @arg I2C_INT_TX_EMPTY:  This bit is set to 1 when the transmit buffer is at or below the threshold value set
                              in the IC_TX_TL register.
  *     @arg I2C_INT_TX_OVER:   Set during transmit if the transmit buffer is filled to IC_TX_BUFFER_DEPTH and
                              the processor attempts to issue another I2C command.
  *     @arg I2C_INT_RX_FULL:   Set when the receive buffer reaches or goes above the RX_TL threshold in the
                              IC_RX_TL register
  *     @arg I2C_INT_RX_OVER:   Set if the receive buffer is completely filled to IC_RX_BUFFER_DEPTH and an
                               additional byte is received from an external I2C device.
  *     @arg I2C_INT_RX_UNDER:   Set if the processor attempts to read the receive buffer when it is empty by reading.
  * @retval The new state of I2C_IT (SET or RESET).
  */
__STATIC_INLINE ITStatus I2C_GetINTStatus(I2C_TypeDef *I2Cx, uint32_t I2C_IT)
{
    ITStatus bit_status = RESET;

    /* Check the parameters */
    assert_param(IS_I2C_ALL_PERIPH(I2Cx));
    assert_param(I2C_GET_INT(I2C_IT));

    if ((I2Cx->IC_INTR_STAT & I2C_IT) != (uint32_t)RESET)
    {

        bit_status = SET;
    }

    /* Return the I2C_IT status */
    return  bit_status;
}

/**
  * @brief  Enables or disables the I2Cx GDMA interface.
  * @param  I2Cx: where x can be 0 or 1
  * @param  I2C_GDMAReq: specifies the I2C GDMA transfer request to be enabled or disabled.
  *   This parameter can be one of the following values:
  *     @arg I2C_GDMAReq_Tx: Tx buffer GDMA transfer request
  *     @arg I2C_GDMAReq_Rx: Rx buffer GDMA transfer request
  * @param  NewState: new state of the selected I2C GDMA transfer request.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
__STATIC_INLINE void I2C_GDMACmd(I2C_TypeDef *I2Cx, uint16_t I2C_GDMAReq, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_I2C_ALL_PERIPH(I2Cx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    assert_param(IS_I2C_GDMAREQ(I2C_GDMAReq));

    if (NewState != DISABLE)
    {
        /* Enable the selected I2C GDMA request */
        I2Cx->IC_DMA_CR |= I2C_GDMAReq;
    }
    else
    {
        /* Disable the selected I2C GDMA request */
        I2Cx->IC_DMA_CR &= (uint16_t)~(I2C_GDMAReq);
    }
}

#ifdef __cplusplus
}
#endif

#endif /*__RTL8762X_I2C_H*/

/** @} */ /* End of group I2C_Exported_Functions */
/** @} */ /* End of group I2C */

/******************* (C) COPYRIGHT 2015 Realtek Semiconductor Corporation *****END OF FILE****/

