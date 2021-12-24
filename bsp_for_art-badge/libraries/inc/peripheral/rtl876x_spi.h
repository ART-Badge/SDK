/**
*********************************************************************************************************
*               Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* \file     rtl876x_spi.h
* \brief    The header file of the peripheral SPI driver.
* \details  This file provides all SPI firmware functions.
* \author   elliot chen
* \date     2015-5-6
* \version  v1.0
* *********************************************************************************************************
*/

#ifndef _RTL876X_SPI_H_
#define _RTL876X_SPI_H_

#ifdef __cplusplus
extern "C" {
#endif


/**
 * \addtogroup  IO          Peripheral Drivers
 * \defgroup    SPI         SPI
 *
 * \brief       Manage the SPI peripheral functions.
 *
 * \ingroup     IO
 */

/*============================================================================*
 *                         Includes
 *============================================================================*/
#include "rtl876x.h"


/*============================================================================*
 *                         Types
 *============================================================================*/

/**
 * \defgroup    SPI_Exported_Types Init Params Struct
 *
 * \ingroup     SPI
 */

/**
 * \brief       SPI init structure definition.
 *
 * \ingroup     SPI_Exported_Types
 */

typedef struct
{
    uint16_t SPI_Direction;         /*!< Specifies the SPI unidirectional or bidirectional data mode.
                                            This parameter can be a value of \ref SPI_Data_Direction. */
    uint16_t SPI_Mode;              /*!< Specifies the SPI operating mode.
                                            This parameter can be a value of \ref SPI_Mode. */
    uint16_t SPI_DataSize;          /*!< Specifies the SPI data size.
                                            This parameter can be a value of \ref SPI_Data_Size. */
    uint16_t SPI_CPOL;              /*!< Specifies the serial clock steady state.
                                            This parameter can be a value of \ref SPI_Clock_Polarity */
    uint16_t SPI_CPHA;              /*!< Specifies the clock active edge for the bit capture.
                                            This parameter can be a value of \ref SPI_Clock_Phase */
    uint32_t SPI_SwapTxBitEn;       /*!< Specifies whether to swap SPI Tx data bit. */
    uint32_t SPI_SwapRxBitEn;       /*!< Specifies whether to swap SPI Rx data bit. */
    uint32_t SPI_SwapTxByteEn;      /*!< Specifies whether to swap SPI Tx data byte. */
    uint32_t SPI_SwapRxByteEn;      /*!< Specifies whether to swap SPI Rx data byte. */
    uint32_t SPI_ToggleEn;          /*!< Specifies whether to toggle when transfer done. */
    uint32_t SPI_BaudRatePrescaler; /*!< Specifies the speed of SCK clock. SPI Clock Speed = clk source/SPI_ClkDIV
                                            \note The communication clock is derived from the master clock. The slave clock does not need to be set. */
    uint16_t SPI_FrameFormat;       /*!< Specifies which serial protocol transfers the data.
                                            This parameter can be a value of \ref SPI_Frame_Format. */
    uint32_t SPI_TxThresholdLevel;  /*!< Specifies the transmit FIFO Threshold. */
    uint32_t SPI_RxThresholdLevel;  /*!< Specifies the receive FIFO Threshold. */
    uint32_t SPI_NDF;               /*!< Specifies the trigger condition in EEPROM mode.
                                            This parameter should be the value of the length of read data. */
    uint16_t SPI_TxDmaEn;           /*!< Specifies the Tx dma mode. */
    uint16_t SPI_RxDmaEn;           /*!< Specifies the Rx dma mode. */
    uint8_t SPI_TxWaterlevel;       /*!< Specifies the DMA tx water level. */
    uint8_t SPI_RxWaterlevel;       /*!< Specifies the DMA rx water level. */

} SPI_InitTypeDef;


/*============================================================================*
 *                         Constants
 *============================================================================*/

/**
 * \defgroup    SPI_Exported_Constants Macro Definitions
 *
 * \ingroup     SPI
 */

#define IS_SPI_ALL_PERIPH(PERIPH) (((PERIPH) == SPI0) || \
                                   ((PERIPH) == SPI1))

#define IS_SPI_CLOCK_SPEED(SPEED) (((SPEED) >= 0x01) && ((SPEED) <= 40000000))

/**
 * \defgroup    SPI_Data_Direction SPI Data Direction
 * \{
 * \ingroup     SPI_Exported_Constants
 */
#define SPI_Direction_FullDuplex        ((uint16_t)0x0000)
#define SPI_Direction_TxOnly            ((uint16_t)0x0001)
#define SPI_Direction_RxOnly            ((uint16_t)0x0002)
#define SPI_Direction_EEPROM            ((uint16_t)0x0003)
/** \} */

#define IS_SPI_DIRECTION_MODE(MODE) (((MODE) == SPI_Direction_FullDuplex) || \
                                     ((MODE) == SPI_Direction_RxOnly) || \
                                     ((MODE) == SPI_Direction_TxOnly) || \
                                     ((MODE) == SPI_Direction_EEPROM))

/**
 * \defgroup    SPI_Mode SPI Mode
 * \{
 * \ingroup     SPI_Exported_Constants
 */

#define SPI_Mode_Master                         ((uint16_t)0x0104)
#define SPI_Mode_Slave                          ((uint16_t)0x0000)
/** \} */
#define IS_SPI_MODE(MODE) (((MODE) == SPI_Mode_Master) || \
                           ((MODE) == SPI_Mode_Slave))

/**
 * \defgroup    SPI_Data_Size SPI Data Size
 * \{
 * \ingroup     SPI_Exported_Constants
 */

#define SPI_DataSize_4b                 ((uint16_t)0x0003)
#define SPI_DataSize_5b                 ((uint16_t)0x0004)
#define SPI_DataSize_6b                 ((uint16_t)0x0005)
#define SPI_DataSize_7b                 ((uint16_t)0x0006)
#define SPI_DataSize_8b                 ((uint16_t)0x0007)
#define SPI_DataSize_9b                 ((uint16_t)0x0008)
#define SPI_DataSize_10b                 ((uint16_t)0x0009)
#define SPI_DataSize_11b                 ((uint16_t)0x000a)
#define SPI_DataSize_12b                 ((uint16_t)0x000b)
#define SPI_DataSize_13b                 ((uint16_t)0x000c)
#define SPI_DataSize_14b                 ((uint16_t)0x000d)
#define SPI_DataSize_15b                 ((uint16_t)0x000e)
#define SPI_DataSize_16b                 ((uint16_t)0x000f)
#define SPI_DataSize_17b                 ((uint16_t)0x0010)
#define SPI_DataSize_18b                 ((uint16_t)0x0011)
#define SPI_DataSize_19b                 ((uint16_t)0x0012)
#define SPI_DataSize_20b                 ((uint16_t)0x0013)
#define SPI_DataSize_21b                 ((uint16_t)0x0014)
#define SPI_DataSize_22b                 ((uint16_t)0x0015)
#define SPI_DataSize_23b                 ((uint16_t)0x0016)
#define SPI_DataSize_24b                 ((uint16_t)0x0017)
#define SPI_DataSize_25b                 ((uint16_t)0x0018)
#define SPI_DataSize_26b                 ((uint16_t)0x0019)
#define SPI_DataSize_27b                 ((uint16_t)0x001A)
#define SPI_DataSize_28b                 ((uint16_t)0x001B)
#define SPI_DataSize_29b                 ((uint16_t)0x001C)
#define SPI_DataSize_30b                 ((uint16_t)0x001D)
#define SPI_DataSize_31b                 ((uint16_t)0x001E)
#define SPI_DataSize_32b                 ((uint16_t)0x001F)
/** \} */
#define IS_SPI_DATASIZE(DATASIZE) (((DATASIZE) == SPI_DataSize_4b)  || \
                                   ((DATASIZE) == SPI_DataSize_5b)  || \
                                   ((DATASIZE) == SPI_DataSize_6b)  || \
                                   ((DATASIZE) == SPI_DataSize_7b)  || \
                                   ((DATASIZE) == SPI_DataSize_8b)  || \
                                   ((DATASIZE) == SPI_DataSize_9b)  || \
                                   ((DATASIZE) == SPI_DataSize_10b) || \
                                   ((DATASIZE) == SPI_DataSize_11b) || \
                                   ((DATASIZE) == SPI_DataSize_12b) || \
                                   ((DATASIZE) == SPI_DataSize_13b) || \
                                   ((DATASIZE) == SPI_DataSize_14b) || \
                                   ((DATASIZE) == SPI_DataSize_15b) || \
                                   ((DATASIZE) == SPI_DataSize_16b) || \
                                   ((DATASIZE) == SPI_DataSize_17b) || \
                                   ((DATASIZE) == SPI_DataSize_18b) || \
                                   ((DATASIZE) == SPI_DataSize_19b) || \
                                   ((DATASIZE) == SPI_DataSize_20b) || \
                                   ((DATASIZE) == SPI_DataSize_21b) || \
                                   ((DATASIZE) == SPI_DataSize_22b) || \
                                   ((DATASIZE) == SPI_DataSize_23b) || \
                                   ((DATASIZE) == SPI_DataSize_24b) || \
                                   ((DATASIZE) == SPI_DataSize_25b) || \
                                   ((DATASIZE) == SPI_DataSize_26b) || \
                                   ((DATASIZE) == SPI_DataSize_27b) || \
                                   ((DATASIZE) == SPI_DataSize_28b) || \
                                   ((DATASIZE) == SPI_DataSize_29b) || \
                                   ((DATASIZE) == SPI_DataSize_30b) || \
                                   ((DATASIZE) == SPI_DataSize_31b) || \
                                   ((DATASIZE) == SPI_DataSize_32b))

/**
 * \defgroup    SPI_BaudRate_Prescaler SPI BaudRate Prescaler Value
 * \{
 * \ingroup     SPI_Exported_Constants
 */

#define SPI_BaudRatePrescaler_2         ((uint32_t)0x0002)
#define SPI_BaudRatePrescaler_4         ((uint32_t)0x0004)
#define SPI_BaudRatePrescaler_6         ((uint32_t)0x0006)
#define SPI_BaudRatePrescaler_8         ((uint32_t)0x0008)
#define SPI_BaudRatePrescaler_10        ((uint32_t)0x000A)
#define SPI_BaudRatePrescaler_12        ((uint32_t)0x000C)
#define SPI_BaudRatePrescaler_14        ((uint32_t)0x000E)
#define SPI_BaudRatePrescaler_16        ((uint32_t)0x0010)
#define SPI_BaudRatePrescaler_32        ((uint32_t)0x0020)
#define SPI_BaudRatePrescaler_64        ((uint32_t)0x0040)
#define SPI_BaudRatePrescaler_128       ((uint32_t)0x0080)
#define SPI_BaudRatePrescaler_256       ((uint32_t)0x0100)
/** \} */
#define IS_SPI_BAUDRATE_PRESCALER(PRESCALER) (((PRESCALER) == SPI_BaudRatePrescaler_2) || \
                                              ((PRESCALER) == SPI_BaudRatePrescaler_4) || \
                                              ((PRESCALER) == SPI_BaudRatePrescaler_8) || \
                                              ((PRESCALER) == SPI_BaudRatePrescaler_10) || \
                                              ((PRESCALER) == SPI_BaudRatePrescaler_12) || \
                                              ((PRESCALER) == SPI_BaudRatePrescaler_14) || \
                                              ((PRESCALER) == SPI_BaudRatePrescaler_16) || \
                                              ((PRESCALER) == SPI_BaudRatePrescaler_32) || \
                                              ((PRESCALER) == SPI_BaudRatePrescaler_64) || \
                                              ((PRESCALER) == SPI_BaudRatePrescaler_128) || \
                                              ((PRESCALER) == SPI_BaudRatePrescaler_256))

/**
 * \defgroup    SPI_Clock_Polarity SPI Clock Polarity
 * \{
 * \ingroup     SPI_Exported_Constants
 */

#define SPI_CPOL_Low                    ((uint16_t)0x0000)
#define SPI_CPOL_High                   ((uint16_t)0x0001)
/** \} */
#define IS_SPI_CPOL(CPOL) (((CPOL) == SPI_CPOL_Low) || \
                           ((CPOL) == SPI_CPOL_High))

/**
 * \defgroup    SPI_Clock_Phase SPI Clock Phase
 * \{
 * \ingroup     SPI_Exported_Constants
 */

#define SPI_CPHA_1Edge                  ((uint16_t)0x0000)
#define SPI_CPHA_2Edge                  ((uint16_t)0x0001)
/** \} */
#define IS_SPI_CPHA(CPHA) (((CPHA) == SPI_CPHA_1Edge) || \
                           ((CPHA) == SPI_CPHA_2Edge))

/**
 * \defgroup    SPI_Frame_Format SPI Frame Format
 * \{
 * \ingroup     SPI_Exported_Constants
 */

#define SPI_Frame_Motorola              ((uint16_t)0x0000)
#define SPI_Frame_TI_SSP                ((uint16_t)0x0001)
#define SPI_Frame_NS_MICROWIRE          ((uint16_t)0x0002)
#define SPI_Frame_Reserve               ((uint16_t)0x0003)
/** \} */
#define IS_SPI_FRAME_FORMAT(FRAME) (((FRAME) == SPI_Frame_Motorola) || \
                                    ((FRAME) == SPI_Frame_TI_SSP) || \
                                    ((FRAME) == SPI_Frame_NS_MICROWIRE) || \
                                    ((FRAME) == SPI_Frame_Reserve))

/**
 * \defgroup    SPI_Flags_Definition SPI Flags Definition
 * \{
 * \ingroup     SPI_Exported_Constants
 */

#define SPI_FLAG_BUSY                   ((uint16_t)0x0001)
#define SPI_FLAG_TFNF                   ((uint16_t)0x0002)
#define SPI_FLAG_TFE                    ((uint16_t)0x0004)
#define SPI_FLAG_RFNE                   ((uint16_t)0x0008)
#define SPI_FLAG_RFF                    ((uint16_t)0x0010)
#define SPI_FLAG_TXE                    ((uint16_t)0x0020)
#define SPI_FLAG_DCOL                   ((uint16_t)0x0040)
/** \} */
#define IS_SPI_GET_FLAG(FLAG)   (((FLAG) == SPI_FLAG_DCOL) || ((FLAG) == SPI_FLAG_TXE) || \
                                 ((FLAG) == SPI_FLAG_RFF) || ((FLAG) == SPI_FLAG_RFNE) || \
                                 ((FLAG) == SPI_FLAG_TFE) || ((FLAG) == SPI_FLAG_TFNF) || \
                                 ((FLAG) == SPI_FLAG_BUSY))

/**
 * \defgroup    SPI_Interrupt_Definition SPI Interrupt Definition
 * \{
 * \ingroup     SPI_Exported_Constants
 */

#define SPI_INT_TXE                  ((uint8_t)BIT(0))
#define SPI_INT_TXO                  ((uint8_t)BIT(1))
#define SPI_INT_RXU                  ((uint8_t)BIT(2))
#define SPI_INT_RXO                  ((uint8_t)BIT(3))
#define SPI_INT_RXF                  ((uint8_t)BIT(4))
#define SPI_INT_MST                  ((uint8_t)BIT(5))
#define SPI_INT_TUF                  ((uint8_t)BIT(6))
#define SPI_INT_RIG                  ((uint8_t)BIT(7))
/** \} */

#define IS_SPI_CONFIG_IT(IT) (((IT) == SPI_INT_TXE) || \
                              ((IT) == SPI_INT_TXO) || \
                              ((IT) == SPI_INT_RXU) || \
                              ((IT) == SPI_INT_RXO) || \
                              ((IT) == SPI_INT_RXF) || \
                              ((IT) == SPI_INT_MST) || \
                              ((IT) == SPI_INT_TUF) || \
                              ((IT) == SPI_INT_RIG) )
/**
 * \defgroup    SPI_GDMA_Transfer_Request SPI GDMA Transfer Request
 * \{
 * \ingroup     SPI_Exported_Constants
 */

#define SPI_GDMAReq_Tx               ((uint16_t)0x0002)
#define SPI_GDMAReq_Rx               ((uint16_t)0x0001)
/** \} */
#define IS_SPI_GDMAREQ(GDMAREQ) ((((GDMAREQ) & (uint16_t)0xFFFC) == 0x00) && ((GDMAREQ) != 0x00))

/**
 * \defgroup    SPI_Swap_Enable SPI Swap Enable
 * \{
 * \ingroup     SPI_Exported_Constants
 */

#define SPI_SWAP_ENABLE                ((uint32_t)0x0001)
#define SPI_SWAP_DISABLE               ((uint32_t)0x0000)
/** \} */
#define IS_SPI_SWAPMODE(mode) (((mode) == SPI_SWAP_ENABLE) || \
                               ((mode) == SPI_SWAP_DISABLE))


/*============================================================================*
 *                         Functions
 *============================================================================*/

/**
 * \defgroup    SPI_Exported_functions  Peripheral APIs
 * \{
 * \ingroup     SPI
 */

/**
 * \brief   Deinitializes the SPIx peripheral registers to their default reset values.
 * \param[in] SPIx: Where x can be 0 or 1 to select the SPI peripheral.
 * \return  None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void driver_spi_init(void)
 * {
 *     SPI_DeInit(SPI0);
 * }
 * \endcode
 */
void SPI_DeInit(SPI_TypeDef *SPIx);

/**
 * \brief   Initializes the SPIx peripheral according to the specified
 *          parameters in the SPI_InitStruct.
 * \param[in] SPIx: Where x can be 0 or 1 to select the SPI peripheral.
 * \param[in] SPI_InitStruct: Pointer to a SPI_InitTypeDef structure that
 *            contains the configuration information for the specified SPI peripheral.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void driver_spi_init(void)
 * {
 *     RCC_PeriphClockCmd(APBPeriph_SPI0, APBPeriph_SPI0_CLOCK, ENABLE);

 *     SPI_InitTypeDef  SPI_InitStruct;
 *     SPI_StructInit(&SPI_InitStruct);
 *
 *     SPI_InitStruct.SPI_Direction   = SPI_Direction_EEPROM;
 *     SPI_InitStruct.SPI_Mode        = SPI_Mode_Master;
 *     SPI_InitStruct.SPI_DataSize    = SPI_DataSize_8b;
 *     SPI_InitStruct.SPI_CPOL        = SPI_CPOL_High;
 *     SPI_InitStruct.SPI_CPHA        = SPI_CPHA_2Edge;
 *     SPI_InitStruct.SPI_BaudRatePrescaler  = 100;
 *     SPI_InitStruct.SPI_RxThresholdLevel  = 1 - 1;
 *     SPI_InitStruct.SPI_NDF               = 1 - 1;
 *     SPI_InitStruct.SPI_FrameFormat = SPI_Frame_Motorola;
 *
 *     SPI_Init(SPI0, &SPI_InitStruct);
 * }
 * \endcode
 */
void SPI_Init(SPI_TypeDef *SPIx, SPI_InitTypeDef *SPI_InitStruct);

/**
 * \brief  Fills each SPI_InitStruct member with its default value.
 * \param[in]  SPI_InitStruct: Pointer to a SPI_InitTypeDef structure which will be initialized.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void driver_spi_init(void)
 * {
 *
 *     RCC_PeriphClockCmd(APBPeriph_SPI0, APBPeriph_SPI0_CLOCK, ENABLE);

 *     SPI_InitTypeDef  SPI_InitStruct;
 *     SPI_StructInit(&SPI_InitStruct);
 *
 *     SPI_InitStruct.SPI_Direction   = SPI_Direction_EEPROM;
 *     SPI_InitStruct.SPI_Mode        = SPI_Mode_Master;
 *     SPI_InitStruct.SPI_DataSize    = SPI_DataSize_8b;
 *     SPI_InitStruct.SPI_CPOL        = SPI_CPOL_High;
 *     SPI_InitStruct.SPI_CPHA        = SPI_CPHA_2Edge;
 *     SPI_InitStruct.SPI_BaudRatePrescaler  = 100;
 *     SPI_InitStruct.SPI_RxThresholdLevel  = 1 - 1;
 *     SPI_InitStruct.SPI_NDF               = 1 - 1;
 *     SPI_InitStruct.SPI_FrameFormat = SPI_Frame_Motorola;
 *
 *     SPI_Init(SPI0, &SPI_InitStruct);
 * }
 * \endcode
 */
void SPI_StructInit(SPI_InitTypeDef *SPI_InitStruct);

/**
 * \brief  Enables or disables the selected SPI peripheral.
 * \param[in]  SPIx: Where x can be 0 or 1 to select the SPI peripheral.
 * \param[in]  NewState: New state of the SPIx peripheral.
 *      This parameter can be: ENABLE or DISABLE.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void driver_spi_init(void)
 * {
 *
 *     RCC_PeriphClockCmd(APBPeriph_SPI0, APBPeriph_SPI0_CLOCK, ENABLE);
 *
 *     SPI_InitTypeDef  SPI_InitStruct;
 *     SPI_StructInit(&SPI_InitStruct);
 *
 *     SPI_InitStruct.SPI_Direction   = SPI_Direction_EEPROM;
 *     SPI_InitStruct.SPI_Mode        = SPI_Mode_Master;
 *     SPI_InitStruct.SPI_DataSize    = SPI_DataSize_8b;
 *     SPI_InitStruct.SPI_CPOL        = SPI_CPOL_High;
 *     SPI_InitStruct.SPI_CPHA        = SPI_CPHA_2Edge;
 *     SPI_InitStruct.SPI_BaudRatePrescaler  = 100;
 *     SPI_InitStruct.SPI_RxThresholdLevel  = 1 - 1;
 *     SPI_InitStruct.SPI_NDF               = 1 - 1;
 *     SPI_InitStruct.SPI_FrameFormat = SPI_Frame_Motorola;
 *
 *     SPI_Init(SPI0, &SPI_InitStruct);
 *     SPI_Cmd(SPI0, ENABLE);
 * }
 * \endcode
 */
void SPI_Cmd(SPI_TypeDef *SPIx, FunctionalState NewState);

/**
 * \brief  Transmits a number of bytes through the SPIx peripheral.
 * \param[in] SPIx: Where x can be 0 or 1 to select the SPI peripheral.
 * \param[in] pBuf: Bytes to be transmitted.
 * \param[in] len: Byte length to be transmitted.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void spi_demo(void)
 * {
 *     uint8_t data_buf[] = {0x01,0x02,0x03};
 *     SPI_SendBuffer(SPI0, data_buf, sizeof(data_buf));
 * }
 * \endcode
 */
void SPI_SendBuffer(SPI_TypeDef *SPIx, uint8_t *pBuf, uint16_t len);

/**
  * \brief  Transmits a number of halfWords through the SPIx peripheral.
  * \param[in] SPIx: Where x can be 0 or 1 to select the SPI peripheral.
  * \param[in] pBuf: Halfwords to be transmitted.
  * \param[in] len: Halfwords length to be transmitted.
  * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void spi_demo(void)
 * {
 *     uint16_t data_buf[] = {0x0102,0x0203,0x0304};
 *     SPI_SendHalfWord(SPI0, data_buf, sizeof(data_buf));
 * }
 * \endcode
  */
void SPI_SendHalfWord(SPI_TypeDef *SPIx, uint16_t *pBuf, uint16_t len);

/**
 * \brief  Transmits a number of words through the SPIx peripheral.
 * \param[in] SPIx: Where x can be 0 or 1 to select the SPI peripheral.
 * \param[in] pBuf: Words to be transmitted.
 * \param[in] len: Word length to be transmitted.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void spi_demo(void)
 * {
 *     uint32_t data_buf[] = {0x01020304,0x02030405,0x03040506};
 *     SPI_SendWord(SPI0, data_buf, sizeof(data_buf));
 * }
 * \endcode
 */
void SPI_SendWord(SPI_TypeDef *SPIx, uint32_t *pBuf, uint16_t len);

/**
 * \brief  Enable or disable the specified SPI interrupt source.
 * \param[in] SPIx: Where x can be 0 or 1 to select the SPI peripheral.
 * \param[in] SPI_IT: Specifies the SPI interrupt source to be enabled or disabled.
 *      This parameter can be one of the following values:
 *      \arg SPI_INT_TXE: Transmit FIFO empty interrupt source.
 *      \arg SPI_INT_TXO: Transmit FIFO overflow interrupt source.
 *      \arg SPI_INT_RXU: Receive FIFO underflow interrupt source.
 *      \arg SPI_INT_RXO: Receive FIFO overflow interrupt source.
 *      \arg SPI_INT_RXF: Receive FIFO full interrupt source.
 *      \arg SPI_INT_MST: Multi-Master contention interrupt source.
 * \param[in] NewState: New state of the specified SPI interrupt source.
 *      This parameter can be: ENABLE or DISABLE.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void spi_demo(void)
 * {
 *     SPI_INTConfig(SPI0, SPI_INT_RXF, ENABLE);
 * }
 * \endcode
 */
void SPI_INTConfig(SPI_TypeDef *SPIx, uint8_t SPI_IT, FunctionalState NewState);

/**
 * \brief  Clear the specified SPI interrupt pending bit.
 * \param[in] SPIx: Where x can be 0 or 1 to select the SPI peripheral.
 * \param[in] SPI_IT: Specifies the SPI interrupt to clear.
 *      This parameter can be one of the following values:
 *      \arg SPI_INT_MST: Multi-Master Contention Interrupt.
 *      \arg SPI_INT_RXO: Receive FIFO Overflow Interrupt.
 *      \arg SPI_INT_RXU: Receive FIFO Underflow Interrupt.
 *      \arg SPI_INT_TXO: Transmit FIFO Overflow Interrupt.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void spi_demo(void)
 * {
 *     SPI_ClearINTPendingBit(SPI0, SPI_INT_RXF);
 * }
 * \endcode
 */
void SPI_ClearINTPendingBit(SPI_TypeDef *SPIx, uint16_t SPI_IT);

/**
 * \brief  Transmits a data through the SPIx peripheral.
 * \param[in] SPIx: Where x can be 0 or 1 to select the SPI peripheral.
 * \param[in] Data: Data to be transmitted.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void spi_demo(void)
 * {
 *     uint32_t data = 0x01020304;
 *     SPI_SendData(SPI0, data);
 * }
 * \endcode
 */
__STATIC_INLINE void SPI_SendData(SPI_TypeDef *SPIx, uint32_t Data)
{
    /* Check the parameters */
    assert_param(IS_SPI_ALL_PERIPH(SPIx));

    SPIx->DR[0] = Data;
    while (!(SPIx->SR & BIT(1)));
}

/**
 * \brief   Received data by the SPIx/I2Sx peripheral.
 * \param[in] SPIx: Where x can be 0 or 1.
 * \retval  The most recent received data.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void spi_demo(void)
 * {
 *     uint32_t data = SPI_ReceiveData(SPI0);
 * }
 * \endcode
 */
__STATIC_INLINE uint32_t SPI_ReceiveData(SPI_TypeDef *SPIx)
{
    /* Check the parameters */
    assert_param(IS_SPI_ALL_PERIPH(SPIx));

    return (uint32_t)SPIx->DR[0];
}

/**
 * \brief   Get data length in Tx FIFO through the SPIx peripheral.
 * \param[in] SPIx: Where x can be 0 or 1.
 * \return  Data length in Tx FIFO.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void spi_demo(void)
 * {
 *     uint8_t data_len = SPI_GetTxFIFOLen(SPI0);
 * }
 * \endcode
 */
__STATIC_INLINE uint8_t SPI_GetTxFIFOLen(SPI_TypeDef *SPIx)
{
    /* Check the parameters */
    assert_param(IS_SPI_ALL_PERIPH(SPIx));

    return (uint8_t)SPIx->TXFLR;
}

/**
 * \brief   Get data length in Rx FIFO through the SPIx peripheral.
 * \param[in] SPIx: Where x can be 0 or 1.
 * \return  Data length in Rx FIFO.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void spi_demo(void)
 * {
 *     uint8_t data_len = SPI_GetRxFIFOLen(SPI0);
 * }
 * \endcode
 */
__STATIC_INLINE uint8_t SPI_GetRxFIFOLen(SPI_TypeDef *SPIx)
{
    /* Check the parameters */
    assert_param(IS_SPI_ALL_PERIPH(SPIx));
    return (uint8_t)SPIx->RXFLR;
}

/**
 * \brief   Change SPI direction mode.
 * \param[in] SPIx: Where x can be 0 or 1.
 * \param[in] dir: Value of direction mode.
 * \return  None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void spi_demo(void)
 * {
 *     SPI_ChangeDirection(SPI0, SPI_Direction_EEPROM);
 * }
 * \endcode
 */
__STATIC_INLINE void SPI_ChangeDirection(SPI_TypeDef *SPIx, uint16_t dir)
{
    /* Check the parameters */
    assert_param(IS_SPI_ALL_PERIPH(SPIx));
    assert_param(IS_SPI_DIRECTION_MODE(dir));

    /* Disable the selected SPI peripheral */
    SPIx->SSIENR &= ~0x01;

    /* Change SPI direction mode */
    SPIx->CTRLR0 &= ~(0x03 << 8);
    SPIx->CTRLR0 |= dir << 8;

    /* Enable the selected SPI peripheral */
    SPIx->SSIENR |= 0x01;
}

/**
 * \brief   Set read Data length only in EEPROM mode through the SPIx peripheral,which
            enables you to receive up to 64 KB of data in a continuous transfer.
 * \param[in] SPIx: Where x can be 0 or 1
 * \param[in] len: Length of read data which can be 1 to 65536.
 * \return  None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void spi_demo(void)
 * {
 *     SPI_SetReadLen(SPI0, 100);
 * }
 * \endcode
 */
__STATIC_INLINE void SPI_SetReadLen(SPI_TypeDef *SPIx, uint16_t len)
{
    /* Check the parameters */
    assert_param(IS_SPI_ALL_PERIPH(SPIx));

    /* Disable the selected SPI peripheral */
    SPIx->SSIENR &= ~0x01;
    /* set read length in SPI EEPROM mode */
    SPIx->CTRLR1 = len - 1;
    /* Enable the selected SPI peripheral */
    SPIx->SSIENR |= 0x01;
}

/**
 * \brief   Set cs number through the SPIx peripheral.
 * \param[in] SPIx: Where x can be 0 or 1
 * \param[in] number: If SPIx is SPI0, number must be 0. If SPIx is SPI1, number can be 0 to 2.
 * \return  None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void spi_demo(void)
 * {
 *     SPI_SetCSNumber(SPI1, 1);
 * }
 * \endcode
 */
__STATIC_INLINE void SPI_SetCSNumber(SPI_TypeDef *SPIx, uint8_t number)
{
    /* Check the parameters */
    assert_param(IS_SPI_ALL_PERIPH(SPIx));

    /* set cs number */
    SPIx->SER = BIT(number);
}

/**
 * \brief  Check whether the specified SPI interrupt is set.
 * \param[in]  SPIx: Where x can be 0 or 1
 * \param[in]  SPI_IT: Specifies the SPI interrupt to check.
 *      This parameter can be one of the following values:
 *      \arg SPI_INT_MST: Multi-Master Contention Interrupt.
 *      \arg SPI_INT_RXF: Receive FIFO Full Interrupt.
 *      \arg SPI_INT_RXO: Receive FIFO Overflow Interrupt.
 *      \arg SPI_INT_RXU: Receive FIFO Underflow Interrupt.
 *      \arg SPI_INT_TXO: Transmit FIFO Overflow Interrupt .
 *      \arg SPI_INT_TXE: Transmit FIFO Empty Interrupt.
 * \return The new state of SPI_IT (SET or RESET).
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void spi_demo(void)
 * {
 *     ITStatus int_status = SPI_GetINTStatus(SPI0, SPI_INT_RXF);
 * }
 * \endcode
 */
__STATIC_INLINE ITStatus SPI_GetINTStatus(SPI_TypeDef *SPIx, uint32_t SPI_IT)
{
    ITStatus bit_status = RESET;

    /* Check the parameters */
    assert_param(IS_SPI_ALL_PERIPH(SPIx));
    assert_param(IS_SPI_CONFIG_IT(SPI_IT));

    if ((SPIx->ISR & SPI_IT) != (uint32_t)RESET)
    {
        bit_status = SET;
    }

    /* Return the SPI_IT status */
    return  bit_status;
}

/**
 * \brief  Check whether the specified SPI flag is set.
 * \param[in]  SPIx: Where x can be 0 or 1
 * \param[in]  SPI_FLAG: Specifies the SPI flag to check.
 *      This parameter can be one of the following values:
 *      \arg SPI_FLAG_DCOL: Data Collision Error flag.Set if it is actively transmitting in master mode when another master selects this device as a slave.
 *      \arg SPI_FLAG_TXE: Transmission error flag.Set if the transmit FIFO is empty when a transfer is started in slave mode.
 *      \arg SPI_FLAG_RFF: Receive FIFO full flag. Set if the receive FIFO is completely full.
 *      \arg SPI_FLAG_RFNE: Receive FIFO Not Empty flag.Set if receive FIFO is not empty.
 *      \arg SPI_FLAG_TFE: Transmit FIFO Empty flag.Set if transmit FIFO is empty.
 *      \arg SPI_FLAG_TFNF: Transmit FIFO Not Full flag.Set if transmit FIFO is not full.
 *      \arg SPI_FLAG_BUSY: SPI Busy flag.Set if it is actively transferring data.reset if it is idle or disabled.
 * \return The new state of SPI_FLAG (SET or RESET).
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void spi_demo(void)
 * {
 *     FlagStatus flag_status = SPI_GetFlagState(SPI0, SPI_FLAG_TXE);
 *
 * }
 * \endcode
 */
__STATIC_INLINE FlagStatus SPI_GetFlagState(SPI_TypeDef *SPIx, uint8_t SPI_FLAG)
{
    FlagStatus bitstatus = RESET;

    /* Check the parameters */
    assert_param(IS_SPI_ALL_PERIPH(SPIx));
    assert_param(IS_SPI_GET_FLAG(SPI_FLAG));

    /* Check the status of the specified SPI flag */
    if ((SPIx->SR & SPI_FLAG) != (uint8_t)RESET)
    {
        /* SPI_FLAG is set */
        bitstatus = SET;
    }

    /* Return the SPI_FLAG status */
    return  bitstatus;
}

/**
 * \brief   Enables or disables the SPIx GDMA interface.
 * \param[in] SPIx: Where x can be 0 or 1
 * \param[in] SPI_GDMAReq: Specifies the SPI GDMA transfer request to be enabled or disabled.
 *      This parameter can be one of the following values:
 *      \arg SPI_GDMAReq_Tx: Tx buffer DMA transfer request.
 *      \arg SPI_GDMAReq_Rx: Rx buffer DMA transfer request.
 * \param[in]  NewState: New state of the selected SPI GDMA transfer request.
 *      This parameter can be: ENABLE or DISABLE.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void spi_demo(void)
 * {
 *     SPI_GDMACmd(SPI0, SPI_GDMAReq_Tx, ENABLE);
 * }
 * \endcode
 */
__STATIC_INLINE void SPI_GDMACmd(SPI_TypeDef *SPIx, uint16_t SPI_GDMAReq, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_SPI_ALL_PERIPH(SPIx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    assert_param(IS_SPI_GDMAREQ(SPI_GDMAReq));

    if (NewState != DISABLE)
    {
        /* Enable the selected SPI GDMA request */
        SPIx->DMACR |= SPI_GDMAReq;
    }
    else
    {
        /* Disable the selected SPI GDMA request */
        SPIx->DMACR &= (uint16_t)~(SPI_GDMAReq);
    }
}
/**
 * \brief  Change SPI speed daynamically.
 * \param[in]  SPIx: Where x can be 0 or 1.
 * \param[in]  precalser: Value of prescaler.
 *      This parameter can be one of the following values:
 *      \arg  SPI_BaudRatePrescaler_2
 *      \arg  SPI_BaudRatePrescaler_4
 *      \arg  SPI_BaudRatePrescaler_6
 *      \arg  SPI_BaudRatePrescaler_8
 *      \arg  SPI_BaudRatePrescaler_10
 *      \arg  SPI_BaudRatePrescaler_12
 *      \arg  SPI_BaudRatePrescaler_14
 *      \arg  SPI_BaudRatePrescaler_16
 *      \arg  SPI_BaudRatePrescaler_32
 *      \arg  SPI_BaudRatePrescaler_64
 *      \arg  SPI_BaudRatePrescaler_128
 *      \arg  SPI_BaudRatePrescaler_256
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void spi_demo(void)
 * {
 *     SPI_Change_CLK(SPI0, SPI_BaudRatePrescaler_2);
 * }
 * \endcode
 */
__STATIC_INLINE void SPI_Change_CLK(SPI_TypeDef *SPIx, uint32_t prescaler)
{
    SPIx->BAUDR = prescaler % 0xFFFF;
}

/**
 * \brief   Set SPI Rx sample delay.
 * \param[in] SPIx: Where x can be 0 or 1.
 * \param[in] delay: This parameter can be 0 to 255.
 * \return  None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void spi_demo(void)
 * {
 *     SPI_SetRxSampleDly(SPI0, 1);
 * }
 * \endcode
 */
__STATIC_INLINE void SPI_SetRxSampleDly(SPI_TypeDef *SPIx, uint32_t delay)
{
    SPIx->RX_SAMPLE_DLY = delay & 0xFF;
}

/** \} */ /* End of group SPI_Exported_Functions */

#ifdef __cplusplus
}
#endif

#endif /* _RTL876X_SPI_H_ */


/******************* (C) COPYRIGHT 2015 Realtek Semiconductor Corporation *****END OF FILE****/

