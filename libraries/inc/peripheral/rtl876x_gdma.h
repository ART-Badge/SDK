/**
*********************************************************************************************************
*               Copyright(c) 2019, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* \file     rtl876x_gdma.h
* \brief    The header file of the peripheral GDMA driver.
* \details  This file provides all GDMA firmware functions.
* \author   yuan
* \date     2019-11-14
* \version  v1.0
* *********************************************************************************************************
*/

#ifndef _RTL876X_GDMA_H_
#define _RTL876X_GDMA_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup  IO          Peripheral Drivers
 * \defgroup    GDMA        GDMA
 *
 * \brief       Manage the GDMA peripheral functions.
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
 * \defgroup    GDMA_Exported_Types Init Params Struct
 *
 * \ingroup     GDMA
 */

/**
 * \brief       GDMA init structure definition.
 *
 * \ingroup     GDMA_Exported_Types
 */
typedef struct
{
    uint8_t GDMA_ChannelNum;        /*!< Specifies channel number for GDMA. */
    uint8_t GDMA_DIR;               /*!< Specifies if the peripheral is the source or destination.
                                            This parameter can be a value of \ref GDMA_Data_Transfer_Direction. */
    uint32_t GDMA_BufferSize;       /*!< Specifies the buffer size(<=4095), in data unit, of the specified Channel.
                                            The data unit is equal to the configuration set in DMA_PeripheralDataSize
                                            or DMA_MemoryDataSize members depending in the transfer direction. */
    uint8_t GDMA_SourceInc;         /*!< Specifies whether the source address register is incremented or not.
                                            This parameter can be a value of \ref GDMA_Source_Incremented_Mode */

    uint8_t GDMA_DestinationInc;    /*!< Specifies whether the destination address register is incremented or not.
                                            This parameter can be a value of \ref GDMA_Destination_Incremented_Mode. */
    uint32_t GDMA_SourceDataSize;   /*!< Specifies the source data width.
                                            This parameter can be a value of \ref GDMA_Data_Size */
    uint32_t GDMA_DestinationDataSize; /*!< Specifies the Memory data width.
                                            This parameter can be a value of \ref GDMA_Data_Size */
    uint32_t GDMA_SourceMsize;      /*!< Specifies the number of data items to be transferred.
                                            This parameter can be a value of \ref GDMA_Msize */
    uint32_t GDMA_DestinationMsize; /*!< Specifies  the number of data items to be transferred.
                                            This parameter can be a value of \ref GDMA_Msize */
    uint32_t GDMA_SourceAddr;       /*!< Specifies the source base address for GDMA Channelx. */
    uint32_t GDMA_DestinationAddr;  /*!< Specifies the destination base address for GDMA Channelx. */
    uint32_t GDMA_ChannelPriority;  /*!< Specifies the software priority for the GDMA Channelx. */

    uint32_t GDMA_Multi_Block_Mode; /*!< Specifies the multi block transfer mode.
                                            This parameter can be a value of \ref GDMA_Multiblock_Mode. */
    uint32_t GDMA_Multi_Block_Struct; /*!< Pointer to the first struct of LLI. */
    uint8_t  GDMA_Multi_Block_En;   /*!< Enable or disable multi-block function. */
    uint8_t  GDMA_Gather_En;        /*!< Enable or disable Gather function. NOTE:4 bytes ALIGN. */
    uint8_t  GDMA_Scatter_En;       /*!< Enable or disable Scatter function. */
    uint32_t GDMA_GatherCount;      /*!< Specifies the GatherCount.NOTE:4 bytes ALIGN.*/
    uint32_t GDMA_GatherInterval;   /*!< Specifies the GatherInterval. */
    uint32_t GDMA_ScatterCount;     /*!< Specifies the ScatterCount. */
    uint32_t GDMA_ScatterInterval;  /*!< Specifies the ScatterInterval. */
    uint8_t  GDMA_SourceHandshake;  /*!< Specifies the handshake index in source.
                                            This parameter can be a value of \ref GDMA_Handshake_Type. */
    uint8_t  GDMA_DestHandshake;    /*!< Specifies the handshake index in Destination.
                                            This parameter can be a value of \ref GDMA_Handshake_Type. */

} GDMA_InitTypeDef;

/**
 * \brief       GDMA link list item structure definition.
 *
 * \ingroup     GDMA_Exported_Types
 */
typedef struct
{
    __IO uint32_t SAR;
    __IO uint32_t DAR;
    __IO uint32_t LLP;
    __IO uint32_t CTL_LOW;
    __IO uint32_t CTL_HIGH;
} GDMA_LLIDef;


/*============================================================================*
 *                         Constants
 *============================================================================*/

/**
 * \defgroup    GDMA_Exported_Constants Macro Definitions
 *
 * \ingroup     GDMA
 */

#define IS_GDMA_ALL_PERIPH(PERIPH) (((PERIPH) == GDMA_Channel0) || \
                                    ((PERIPH) == GDMA_Channel1) || \
                                    ((PERIPH) == GDMA_Channel2) || \
                                    ((PERIPH) == GDMA_Channel3) || \
                                    ((PERIPH) == GDMA_Channel4) || \
                                    ((PERIPH) == GDMA_Channel5))

#define IS_GDMA_ChannelNum(NUM) ((NUM) < 8)

/**
 * \defgroup    GDMA_Handshake_Type GDMA Handshake Type
 * \{
 * \ingroup     GDMA_Exported_Constants
 */
#define GDMA_Handshake_UART0_TX             (0)
#define GDMA_Handshake_UART0_RX             (1)
#define GDMA_Handshake_UART2_TX             (2)
#define GDMA_Handshake_UART2_RX             (3)
#define GDMA_Handshake_SPI0_TX              (4)
#define GDMA_Handshake_SPI0_RX              (5)
#define GDMA_Handshake_SPI1_TX              (6)
#define GDMA_Handshake_SPI1_RX              (7)
#define GDMA_Handshake_I2C0_TX              (8)
#define GDMA_Handshake_I2C0_RX              (9)
#define GDMA_Handshake_I2C1_TX              (10)
#define GDMA_Handshake_I2C1_RX              (11)
#define GDMA_Handshake_ADC                  (12)
#define GDMA_Handshake_AES_TX               (13)
#define GDMA_Handshake_AES_RX               (14)
#define GDMA_Handshake_UART1_TX             (15)
#define GDMA_Handshake_I2S0_TX              (16)
#define GDMA_Handshake_I2S0_RX              (17)
#define GDMA_Handshake_I2S1_TX              (18)
#define GDMA_Handshake_I2S1_RX              (19)
#define GDMA_Handshake_SPIC0_TX             (20)
#define GDMA_Handshake_SPIC0_RX             (21)
#define GDMA_Handshake_8080_TX              (22)
#define GDMA_Handshake_SPIC2_TX             (22)
#define GDMA_Handshake_UART1_RX             (23)
#define GDMA_Handshake_SPIC1_TX             (24)
#define GDMA_Handshake_SPIC1_RX             (25)
#define GDMA_Handshake_8080_RX              (26)
#define GDMA_Handshake_SPIC2_RX             (26)
#define GDMA_Handshake_IR_TX                (27)
#define GDMA_Handshake_IR_RX                (28)
#define GDMA_Handshake_TIM5                 (29)
#define GDMA_Handshake_TIM6                 (30)
#define GDMA_Handshake_TIM7                 (31)
/** \} */

#define IS_GDMA_TransferType(Type) (((Type) == GDMA_Handshake_UART0_TX) || \
                                    ((Type) == GDMA_Handshake_UART0_RX) || \
                                    ((Type) == GDMA_Handshake_UART2_TX) || \
                                    ((Type) == GDMA_Handshake_UART2_RX) || \
                                    ((Type) == GDMA_Handshake_SPI0_TX) || \
                                    ((Type) == GDMA_Handshake_SPI0_RX) || \
                                    ((Type) == GDMA_Handshake_SPI1_TX) || \
                                    ((Type) == GDMA_Handshake_SPI1_RX) || \
                                    ((Type) == GDMA_Handshake_I2C0_TX) || \
                                    ((Type) == GDMA_Handshake_I2C0_RX) || \
                                    ((Type) == GDMA_Handshake_I2C1_TX) || \
                                    ((Type) == GDMA_Handshake_I2C1_RX) || \
                                    ((Type) == GDMA_Handshake_ADC) || \
                                    ((Type) == GDMA_Handshake_AES_TX) || \
                                    ((Type) == GDMA_Handshake_AES_RX) || \
                                    ((Type) == GDMA_Handshake_UART1_TX) || \
                                    ((Type) == GDMA_Handshake_SPORT0_TX) || \
                                    ((Type) == GDMA_Handshake_SPORT0_RX) || \
                                    ((Type) == GDMA_Handshake_SPORT1_TX) || \
                                    ((Type) == GDMA_Handshake_SPORT1_RX) || \
                                    ((Type) == GDMA_Handshake_SPIC_TX) || \
                                    ((Type) == GDMA_Handshake_SPIC_RX) ||\
                                    ((Type) == GDMA_Handshake_LCD) ||\
                                    ((Type) == GDMA_Handshake_TIM0)||\
                                    ((Type) == GDMA_Handshake_TIM1)||\
                                    ((Type) == GDMA_Handshake_TIM2)||\
                                    ((Type) == GDMA_Handshake_TIM3)||\
                                    ((Type) == GDMA_Handshake_TIM4)||\
                                    ((Type) == GDMA_Handshake_TIM5)||\
                                    ((Type) == GDMA_Handshake_TIM6)||\
                                    ((Type) == GDMA_Handshake_TIM7))

/**
 * \defgroup    GDMA_Data_Size GDMA Data Size
 * \{
 * \ingroup     GDMA_Exported_Constants
 */
#define GDMA_DataSize_Byte                  ((uint32_t)0x00000000)
#define GDMA_DataSize_HalfWord              ((uint32_t)0x00000001)
#define GDMA_DataSize_Word                  ((uint32_t)0x00000002)
/** \} */

#define IS_GDMA_DATA_SIZE(SIZE) (((SIZE) == GDMA_DataSize_Byte) || \
                                 ((SIZE) == GDMA_DataSize_HalfWord) || \
                                 ((SIZE) == GDMA_DataSize_Word))

/**
 * \defgroup    GDMA_Msize GDMA Msize
 * \{
 * \ingroup     GDMA_Exported_Constants
 */
#define GDMA_Msize_1                        ((uint32_t)0x00000000)
#define GDMA_Msize_4                        ((uint32_t)0x00000001)
#define GDMA_Msize_8                        ((uint32_t)0x00000002)
#define GDMA_Msize_16                       ((uint32_t)0x00000003)
#define GDMA_Msize_32                       ((uint32_t)0x00000004)
#define GDMA_Msize_64                       ((uint32_t)0x00000005)
#define GDMA_Msize_128                      ((uint32_t)0x00000006)
#define GDMA_Msize_256                      ((uint32_t)0x00000007)
/** \} */
#define IS_GDMA_MSIZE(SIZE) (((SIZE) == GDMA_Msize_1) || \
                             ((SIZE) == GDMA_Msize_4) || \
                             ((SIZE) == GDMA_Msize_8) || \
                             ((SIZE) == GDMA_Msize_16) || \
                             ((SIZE) == GDMA_Msize_32) || \
                             ((SIZE) == GDMA_Msize_64) || \
                             ((SIZE) == GDMA_Msize_128) || \
                             ((SIZE) == GDMA_Msize_256))

/**
 * \defgroup    GDMA_Data_Transfer_Direction GDMA Data Transfer Direction
 * \{
 * \ingroup     GDMA_Exported_Constants
 */
#define GDMA_DIR_MemoryToMemory              ((uint32_t)0x00000000)
#define GDMA_DIR_MemoryToPeripheral          ((uint32_t)0x00000001)
#define GDMA_DIR_PeripheralToMemory          ((uint32_t)0x00000002)
#define GDMA_DIR_PeripheralToPeripheral      ((uint32_t)0x00000003)
/** \} */

#define IS_GDMA_DIR(DIR) (((DIR) == GDMA_DIR_MemoryToMemory) || \
                          ((DIR) == GDMA_DIR_MemoryToPeripheral) || \
                          ((DIR) == GDMA_DIR_PeripheralToMemory) ||\
                          ((DIR) == GDMA_DIR_PeripheralToPeripheral))

/**
 * \defgroup    GDMA_Source_Incremented_Mode GDMA Source Incremented Mode
 * \{
 * \ingroup     GDMA_Exported_Constants
 */
#define DMA_SourceInc_Inc                   ((uint32_t)0x00000000)
#define DMA_SourceInc_Dec                   ((uint32_t)0x00000001)
#define DMA_SourceInc_Fix                   ((uint32_t)0x00000002)
/** \} */

#define IS_GDMA_SourceInc(STATE) (((STATE) == DMA_SourceInc_Inc) || \
                                  ((STATE) == DMA_SourceInc_Dec) || \
                                  ((STATE) == DMA_SourceInc_Fix))

/**
 * \defgroup    GDMA_Destination_Incremented_Mode GDMA Destination Incremented Mode
 * \{
 * \ingroup     GDMA_Exported_Constants
 */
#define DMA_DestinationInc_Inc              ((uint32_t)0x00000000)
#define DMA_DestinationInc_Dec              ((uint32_t)0x00000001)
#define DMA_DestinationInc_Fix              ((uint32_t)0x00000002)
/** \} */

#define IS_GDMA_DestinationInc(STATE) (((STATE) == DMA_DestinationInc_Inc) || \
                                       ((STATE) == DMA_DestinationInc_Dec) || \
                                       ((STATE) == DMA_DestinationInc_Fix))

/**
 * \defgroup    DMA_Interrupts_Definition DMA Interrupts Definition
 * \{
 * \ingroup     GDMA_Exported_Constants
 */
#define GDMA_INT_Transfer                   ((uint32_t)0x00000001)
#define GDMA_INT_Block                      ((uint32_t)0x00000002)
#define GDMA_INT_SrcTransfer                ((uint32_t)0x00000004)
#define GDMA_INT_DstTransfer                ((uint32_t)0x00000008)
#define GDMA_INT_Error                      ((uint32_t)0x00000010)
/** \} */

#define IS_GDMA_CONFIG_IT(IT) ((((IT) & 0xFFFFFE00) == 0x00) && ((IT) != 0x00))

/**
 * \defgroup    GDMA_Multiblock_Mode GDMA Multi-block Mode
 * \{
 * \ingroup     GDMA_Exported_Constants
 */
#define AUTO_RELOAD_WITH_CONTIGUOUS_SAR     (BIT30)
#define AUTO_RELOAD_WITH_CONTIGUOUS_DAR     (BIT31)
#define AUTO_RELOAD_TRANSFER                (BIT30 | BIT31)
#define LLI_WITH_CONTIGUOUS_SAR             (BIT27)
#define LLI_WITH_AUTO_RELOAD_SAR            (BIT27 | BIT30)
#define LLI_WITH_CONTIGUOUS_DAR             (BIT28)
#define LLI_WITH_AUTO_RELOAD_DAR            (BIT28 | BIT31)
#define LLI_TRANSFER                        (BIT27 | BIT28)
/** \} */

#define IS_GDMA_MULTIBLOCKMODE(MODE) (((MODE) == AUTO_RELOAD_WITH_CONTIGUOUS_SAR) || ((MODE) == AUTO_RELOAD_WITH_CONTIGUOUS_DAR)\
                                      ||((MODE) == AUTO_RELOAD_TRANSFER) || ((MODE) == LLI_WITH_CONTIGUOUS_SAR)\
                                      ||((MODE) == LLI_WITH_AUTO_RELOAD_SAR) || ((MODE) == LLI_WITH_CONTIGUOUS_DAR)\
                                      ||((MODE) == LLI_WITH_AUTO_RELOAD_DAR) || ((MODE) == LLI_TRANSFER))

/**
 * \def GDMA_Multiblock_Select_Bit Multi-Block Select Bit
 *
 */
#define AUTO_RELOAD_SELECTED_BIT            (uint32_t)(0xC0000000)
#define LLP_SELECTED_BIT                    (uint32_t)(0x18000000)

/**
 * \def DMA_Suspend_Flag_Definition DMA Suspend Flag Definition
 */
#define GDMA_FIFO_STATUS                    (BIT(9))
#define GDMA_SUSPEND_TRANSMISSSION          (BIT(8))
#define GDMA_SUSPEND_CMD_STATUS             (BIT(2) | BIT(1))
#define GDMA_SUSPEND_CHANNEL_STATUS         (BIT(0))


/*============================================================================*
 *                         Functions
 *============================================================================*/

/**
 * \defgroup    GDMA_Exported_Functions Peripheral APIs
 * \{
 * \ingroup     GDMA
 */

/**
 * \brief  Deinitializes the GDMA registers to their default reset
 *         values.
 * \param  None.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void driver_gdma_init(void)
 * {
 *     GDMA_DeInit();
 * }
 * \endcode
 */
void GDMA_DeInit(void);

/**
 * \brief  Initializes the GDMA Channelx according to the specified
 *         parameters in the GDMA_InitStruct.
 * \param[in] GDMA_Channelx: Where x can be 0 to 7  to select the DMA Channel.
 * \param[in] GDMA_InitStruct: Pointer to a GDMA_InitTypeDef structure that
 *            contains the configuration information for the specified DMA Channel.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void driver_gdma_init(void)
 * {
 *
 *     for (uint32_t i = 0; i < UART_TX_GDMA_BUFFER_SIZE; i++)
 *     {
 *         GDMA_SendData_Buffer[i] = 0x10 + i;
 *     }

 *     GDMA_InitTypeDef GDMA_InitStruct;
 *     GDMA_StructInit(&GDMA_InitStruct);
 *     GDMA_InitStruct.GDMA_ChannelNum      = 1;
 *     GDMA_InitStruct.GDMA_DIR             = GDMA_DIR_MemoryToPeripheral;
 *     GDMA_InitStruct.GDMA_BufferSize      = UART_TX_GDMA_BUFFER_SIZE;//determine total transfer size
 *     GDMA_InitStruct.GDMA_SourceInc       = DMA_SourceInc_Inc;
 *     GDMA_InitStruct.GDMA_DestinationInc  = DMA_DestinationInc_Fix;
 *     GDMA_InitStruct.GDMA_SourceDataSize  = GDMA_DataSize_Byte;
 *     GDMA_InitStruct.GDMA_DestinationDataSize = GDMA_DataSize_Byte;
 *     GDMA_InitStruct.GDMA_SourceMsize      = GDMA_Msize_1;
 *     GDMA_InitStruct.GDMA_DestinationMsize = GDMA_Msize_1;
 *     GDMA_InitStruct.GDMA_SourceAddr      = (uint32_t)GDMA_SendData_Buffer;
 *     GDMA_InitStruct.GDMA_DestinationAddr = (uint32_t)(&(UART0->RB_THR));
 *     GDMA_InitStruct.GDMA_DestHandshake   = GDMA_Handshake_UART0_TX;
 *     GDMA_InitStruct.GDMA_ChannelPriority = 2;//channel prority between 0 to 5
 *     GDMA_Init(UART_TX_GDMA_CHANNEL, &GDMA_InitStruct);

 *     GDMA_INTConfig(UART_TX_GDMA_CHANNEL_NUM, GDMA_INT_Transfer, ENABLE);

 *     NVIC_InitTypeDef NVIC_InitStruct;
 *     NVIC_InitStruct.NVIC_IRQChannel = UART_TX_GDMA_CHANNEL_IRQN;
 *     NVIC_InitStruct.NVIC_IRQChannelPriority = 3;
 *     NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
 *     NVIC_Init(&NVIC_InitStruct);

 *     GDMA_Cmd(UART_TX_GDMA_CHANNEL_NUM, ENABLE);
 * }
 * \endcode
 */
void GDMA_Init(GDMA_ChannelTypeDef *GDMA_Channelx, GDMA_InitTypeDef *GDMA_InitStruct);

/**
 * \brief  Fills each GDMA_InitStruct member with its default value.
 * \param[in]  GDMA_InitStruct : pointer to a GDMA_InitTypeDef structure which will
 *         be initialized.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void driver_gdma_init(void)
 * {
 *
 *     for (uint32_t i = 0; i < UART_TX_GDMA_BUFFER_SIZE; i++)
 *     {
 *         GDMA_SendData_Buffer[i] = 0x10 + i;
 *     }

 *     GDMA_InitTypeDef GDMA_InitStruct;
 *     GDMA_StructInit(&GDMA_InitStruct);
 *     GDMA_InitStruct.GDMA_ChannelNum      = 1;
 *     GDMA_InitStruct.GDMA_DIR             = GDMA_DIR_MemoryToPeripheral;
 *     GDMA_InitStruct.GDMA_BufferSize      = UART_TX_GDMA_BUFFER_SIZE;//determine total transfer size
 *     GDMA_InitStruct.GDMA_SourceInc       = DMA_SourceInc_Inc;
 *     GDMA_InitStruct.GDMA_DestinationInc  = DMA_DestinationInc_Fix;
 *     GDMA_InitStruct.GDMA_SourceDataSize  = GDMA_DataSize_Byte;
 *     GDMA_InitStruct.GDMA_DestinationDataSize = GDMA_DataSize_Byte;
 *     GDMA_InitStruct.GDMA_SourceMsize      = GDMA_Msize_1;
 *     GDMA_InitStruct.GDMA_DestinationMsize = GDMA_Msize_1;
 *     GDMA_InitStruct.GDMA_SourceAddr      = (uint32_t)GDMA_SendData_Buffer;
 *     GDMA_InitStruct.GDMA_DestinationAddr = (uint32_t)(&(UART0->RB_THR));
 *     GDMA_InitStruct.GDMA_DestHandshake   = GDMA_Handshake_UART0_TX;
 *     GDMA_InitStruct.GDMA_ChannelPriority = 2;//channel prority between 0 to 5
 *     GDMA_Init(UART_TX_GDMA_CHANNEL, &GDMA_InitStruct);
 *
 * }
 * \endcode
 */
void GDMA_StructInit(GDMA_InitTypeDef *GDMA_InitStruct);

/**
 * \brief  Enables or disables the selected GDMA channel.
 * \param[in]  GDMA_Channel_Num: GDMA channel number, can be 0~5.
 * \param[in]  NewState: New state of the selected DMA channel.
 *      This parameter can be: ENABLE or DISABLE.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void driver_gdma_init(void)
 * {
 *
 *     for (uint32_t i = 0; i < UART_TX_GDMA_BUFFER_SIZE; i++)
 *     {
 *         GDMA_SendData_Buffer[i] = 0x10 + i;
 *     }

 *     GDMA_InitTypeDef GDMA_InitStruct;
 *     GDMA_StructInit(&GDMA_InitStruct);
 *     GDMA_InitStruct.GDMA_ChannelNum      = 1;
 *     GDMA_InitStruct.GDMA_DIR             = GDMA_DIR_MemoryToPeripheral;
 *     GDMA_InitStruct.GDMA_BufferSize      = UART_TX_GDMA_BUFFER_SIZE;//determine total transfer size
 *     GDMA_InitStruct.GDMA_SourceInc       = DMA_SourceInc_Inc;
 *     GDMA_InitStruct.GDMA_DestinationInc  = DMA_DestinationInc_Fix;
 *     GDMA_InitStruct.GDMA_SourceDataSize  = GDMA_DataSize_Byte;
 *     GDMA_InitStruct.GDMA_DestinationDataSize = GDMA_DataSize_Byte;
 *     GDMA_InitStruct.GDMA_SourceMsize      = GDMA_Msize_1;
 *     GDMA_InitStruct.GDMA_DestinationMsize = GDMA_Msize_1;
 *     GDMA_InitStruct.GDMA_SourceAddr      = (uint32_t)GDMA_SendData_Buffer;
 *     GDMA_InitStruct.GDMA_DestinationAddr = (uint32_t)(&(UART0->RB_THR));
 *     GDMA_InitStruct.GDMA_DestHandshake   = GDMA_Handshake_UART0_TX;
 *     GDMA_InitStruct.GDMA_ChannelPriority = 2;//channel prority between 0 to 5
 *     GDMA_Init(UART_TX_GDMA_CHANNEL, &GDMA_InitStruct);

 *     GDMA_INTConfig(UART_TX_GDMA_CHANNEL_NUM, GDMA_INT_Transfer, ENABLE);

 *     NVIC_InitTypeDef NVIC_InitStruct;
 *     NVIC_InitStruct.NVIC_IRQChannel = UART_TX_GDMA_CHANNEL_IRQN;
 *     NVIC_InitStruct.NVIC_IRQChannelPriority = 3;
 *     NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
 *     NVIC_Init(&NVIC_InitStruct);

 *     GDMA_Cmd(UART_TX_GDMA_CHANNEL_NUM, ENABLE);
 * }
 * \endcode
 */
void GDMA_Cmd(uint8_t GDMA_Channel_Num, FunctionalState NewState);

/**
 * \brief   Enables or disables the specified DMA channelx interrupt source.
 * \param[in] GDMA_Channel_Num: GDMA channel number, can be 0~5.
 * \param[in]  GDMA_IT: Specifies the GDMA interrupt source to be enabled or disabled.
 *      This parameter can be any combination of the following values:
 *      \arg GDMA_INT_Transfer: Transfer complete interrupt source.
 *      \arg GDMA_INT_Block: Block transfer interrupt source.
 *      \arg GDMA_INT_SrcTransfer: SourceTransfer interrupt source.
 *      \arg GDMA_INT_DstTransfer: Destination Transfer interruptsource.
 *      \arg GDMA_INT_Error: Transfer error interrupt source.
 * \param[in] NewState: New state of the specified DMA interrupt source.
 *      This parameter can be: ENABLE or DISABLE.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void driver_gdma_init(void)
 * {
 *
 *     for (uint32_t i = 0; i < UART_TX_GDMA_BUFFER_SIZE; i++)
 *     {
 *         GDMA_SendData_Buffer[i] = 0x10 + i;
 *     }

 *     GDMA_InitTypeDef GDMA_InitStruct;
 *     GDMA_StructInit(&GDMA_InitStruct);
 *     GDMA_InitStruct.GDMA_ChannelNum      = 1;
 *     GDMA_InitStruct.GDMA_DIR             = GDMA_DIR_MemoryToPeripheral;
 *     GDMA_InitStruct.GDMA_BufferSize      = UART_TX_GDMA_BUFFER_SIZE;//determine total transfer size
 *     GDMA_InitStruct.GDMA_SourceInc       = DMA_SourceInc_Inc;
 *     GDMA_InitStruct.GDMA_DestinationInc  = DMA_DestinationInc_Fix;
 *     GDMA_InitStruct.GDMA_SourceDataSize  = GDMA_DataSize_Byte;
 *     GDMA_InitStruct.GDMA_DestinationDataSize = GDMA_DataSize_Byte;
 *     GDMA_InitStruct.GDMA_SourceMsize      = GDMA_Msize_1;
 *     GDMA_InitStruct.GDMA_DestinationMsize = GDMA_Msize_1;
 *     GDMA_InitStruct.GDMA_SourceAddr      = (uint32_t)GDMA_SendData_Buffer;
 *     GDMA_InitStruct.GDMA_DestinationAddr = (uint32_t)(&(UART0->RB_THR));
 *     GDMA_InitStruct.GDMA_DestHandshake   = GDMA_Handshake_UART0_TX;
 *     GDMA_InitStruct.GDMA_ChannelPriority = 2;//channel prority between 0 to 5
 *     GDMA_Init(UART_TX_GDMA_CHANNEL, &GDMA_InitStruct);

 *     GDMA_INTConfig(UART_TX_GDMA_CHANNEL_NUM, GDMA_INT_Transfer, ENABLE);

 *     NVIC_InitTypeDef NVIC_InitStruct;
 *     NVIC_InitStruct.NVIC_IRQChannel = UART_TX_GDMA_CHANNEL_IRQN;
 *     NVIC_InitStruct.NVIC_IRQChannelPriority = 3;
 *     NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
 *     NVIC_Init(&NVIC_InitStruct);

 *     GDMA_Cmd(UART_TX_GDMA_CHANNEL_NUM, ENABLE);
 * }
 * \endcode
 */
void GDMA_INTConfig(uint8_t GDMA_Channel_Num, uint32_t GDMA_IT, FunctionalState NewState);

/**
 * \brief  Clear the specified DMA channelx interrupt pending bit.
 * \param[in] GDMA_Channel_Num: GDMA channel number, can be 0~5.
 * \param[in] GDMA_IT: Specifies the GDMA interrupts sources to be enabled or disabled.
 *      This parameter can be any combination of the following values:
 *      \arg GDMA_INT_Transfer: Transfer complete interrupt source.
 *      \arg GDMA_INT_Block: Block transfer interrupt source.
 *      \arg GDMA_INT_SrcTransfer: SourceTransfer interrupt source.
 *      \arg GDMA_INT_DstTransfer: Destination Transfer interruptsource.
 *      \arg GDMA_INT_Error: Transfer error interrupt source.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void gdma_demo(void)
 * {
 *     driver_gdma_init();
 * }
 *
 * void UART_TX_GDMA_Handler(void)
 * {
 *     GDMA_ClearINTPendingBit(1, GDMA_INT_Transfer);
 *     //Add user code here.
 * }
 * \endcode
 */
void GDMA_ClearINTPendingBit(uint8_t GDMA_Channel_Num, uint32_t GDMA_IT);

/**
 * \brief   Get selected GDMA channel status.
 * \param[in] GDMA_Channel_Num: GDMA channel number, can be 0~5.
 * \return  GDMA channel status.
 * \retval  SET: Channel is be used
 * \retval  RESET: Channel is free.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void gdma_demo(void)
 * {
 *     FlagStatus flag_status = GDMA_GetChannelStatus(0);
 * }
 * \endcode
 */
__STATIC_INLINE FlagStatus GDMA_GetChannelStatus(uint8_t GDMA_Channel_Num)
{
    FlagStatus bit_status = RESET;

    /* Check the parameters */
    assert_param(IS_GDMA_ChannelNum(GDMA_Channel_Num));

    if ((GDMA_BASE->ChEnReg & BIT(GDMA_Channel_Num)) != (uint32_t)RESET)
    {

        bit_status = SET;
    }

    /* Return the selected channel status */
    return  bit_status;
}

/**
 * \brief  Check whether GDMA Channel transfer interrupt is set.
 * \param[in] GDMA_Channel_Num: GDMA channel number, can be 0~5.
 * \return  Transfer interrupt status, SET or RESET.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void gdma_demo(void)
 * {
 *     ITStatus int_status = GDMA_GetTransferINTStatus(0);
 * }
 * \endcode
 */
__STATIC_INLINE ITStatus GDMA_GetTransferINTStatus(uint8_t GDMA_Channel_Num)
{
    ITStatus bit_status = RESET;

    /* Check the parameters */
    assert_param(IS_GDMA_ChannelNum(GDMA_Channel_Num));

    if ((GDMA_BASE->STATUS_TFR & BIT(GDMA_Channel_Num)) != (uint32_t)RESET)
    {

        bit_status = SET;
    }

    /* Return the transfer interrupt status */
    return  bit_status;
}

/**
 * \brief   Clear GDMA Channelx all interrupt.
 * \param[in] GDMA_Channel_Num: GDMA channel number, can be 0~5.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void gdma_demo(void)
 * {
 *     GDMA_ClearAllTypeINT(0);
 * }
 * \endcode
 */
__STATIC_INLINE void GDMA_ClearAllTypeINT(uint8_t GDMA_Channel_Num)
{
    /* Check the parameters */
    assert_param(IS_GDMA_ChannelNum(GDMA_Channel_Num));

    GDMA_BASE->CLEAR_TFR = BIT(GDMA_Channel_Num);
    GDMA_BASE->CLEAR_BLOCK = BIT(GDMA_Channel_Num);
    GDMA_BASE->CLEAR_DST_TRAN = BIT(GDMA_Channel_Num);
    GDMA_BASE->CLEAR_SRC_TRAN = BIT(GDMA_Channel_Num);
    GDMA_BASE->CLEAR_ERR = BIT(GDMA_Channel_Num);
}

/**
 * \brief  set GDMA transmission source address.
 * \param[in]  GDMA_Channelx: where x can be 0 to 5 to select the DMA Channel.
 * \param[in]  Address: Source address.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void gdma_demo(void)
 * {
 *     uint32_t data_buf[10] = {0};
 *     GDMA_SetSourceAddress(GDMA_Channel0,(uint32_t)data_buf);
 * }
 * \endcode
 */
__STATIC_INLINE void GDMA_SetSourceAddress(GDMA_ChannelTypeDef *GDMA_Channelx, uint32_t Address)
{
    /* Check the parameters */
    assert_param(IS_GDMA_ALL_PERIPH(GDMA_Channelx));

    GDMA_Channelx->SAR = Address;
}

/**
 * \brief  Set GDMA transmission destination address.
 * \param[in] GDMA_Channelx: Where x can be 0 to 5  to select the GDMA Channel.
 * \param[in] Address: Destination address.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void gdma_demo(void)
 * {
 *     uint32_t data_buf[10] = {0};
 *     GDMA_SetDestinationAddress(GDMA_Channel0,(uint32_t)data_buf);
 * }
 * \endcode
 */
__STATIC_INLINE void GDMA_SetDestinationAddress(GDMA_ChannelTypeDef *GDMA_Channelx,
                                                uint32_t Address)
{
    /* Check the parameters */
    assert_param(IS_GDMA_ALL_PERIPH(GDMA_Channelx));

    GDMA_Channelx->DAR = Address;
}

/**
 * \brief   Set GDMA buffer size.
 * \param[in] GDMA_Channelx: Where x can be 0 to 5 to select the GDMA Channel.
 * \param[in] buffer_size: Set GDMA_BufferSize.
 * \return  None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void gdma_demo(void)
 * {
 *     uint32_t data_buf_size = 4095;//max
 *     GDMA_SetBufferSize(GDMA_Channel0,data_buf_size);
 * }
 * \endcode
 */
__STATIC_INLINE void GDMA_SetBufferSize(GDMA_ChannelTypeDef *GDMA_Channelx, uint32_t buffer_size)
{
    /* Check the parameters */
    assert_param(IS_GDMA_ALL_PERIPH(GDMA_Channelx));

    /* configure high 32 bit of CTL register */
    GDMA_Channelx->CTL_HIGH = buffer_size;
}

/**
 * \brief  Suspend GDMA transmission from the source.
 * \param[in]  GDMA_Channelx: Where x can be 0 to 5 to select the GDMA Channel.
 * \param[in]  NewState: New state of the DMA Channelx.
 *      This parameter can be: ENABLE or DISABLE.
 * \return None.
 * \note    To prevent data loss, it is necessary to check whether FIFO data transmission is completed
 *          after suspend, and judge by checking whether GDMA FIFO is empty.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void gdma_demo(void)
 * {
 *     GDMA_SuspendCmd(GDMA_Channel0,ENABLE);
 * }
 * \endcode
 */
__STATIC_INLINE void GDMA_SuspendCmd(GDMA_ChannelTypeDef *GDMA_Channelx,
                                     FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_GDMA_ALL_PERIPH(GDMA_Channelx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState == DISABLE)
    {
        /* Not suspend transmission*/
        GDMA_Channelx->CFG_LOW &= ~(GDMA_SUSPEND_TRANSMISSSION);
    }
    else
    {
        /* Suspend transmission */
        GDMA_Channelx->CFG_LOW |= GDMA_SUSPEND_TRANSMISSSION;
    }
}

/**
 * \brief  Get GDMA transfer data length.
 * \param[in] GDMA_Channelx: Where x can be 0 to 5 to select the GDMA Channel.
 * \return GDMA transfer data length.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void gdma_demo(void)
 * {
 *     uint16_t data_len = GDMA_GetTransferLen(GDMA_Channel0);
 * }
 * \endcode
 */
__STATIC_INLINE uint16_t GDMA_GetTransferLen(GDMA_ChannelTypeDef *GDMA_Channelx)
{
    /* Check the parameters */
    assert_param(IS_GDMA_ALL_PERIPH(GDMA_Channelx));

    return (uint16_t)(GDMA_Channelx->CTL_HIGH & 0xfff);
}

/**
 * \brief  Set GDMA LLP stucture address.
 * \param[in] GDMA_Channelx: Only for GDMA_Channel0~4.
 * \param[in] Address: LLP stucture address.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void gdma_demo(void)
 * {
 *     GDMA_LLIDef GDMA_LLIStruct[4000];
 *     GDMA_SetLLPAddress(GDMA_Channel0,(uint32_t)GDMA_LLIStruct);
 * }
 * \endcode
 */
__STATIC_INLINE void GDMA_SetLLPAddress(GDMA_ChannelTypeDef *GDMA_Channelx, uint32_t Address)
{
    /* Check the parameters */
    assert_param(IS_GDMA_ALL_PERIPH(GDMA_Channelx));

    if ((GDMA_Channelx == GDMA_Channel0) | (GDMA_Channelx == GDMA_Channel1) | \
        (GDMA_Channelx == GDMA_Channel2) | (GDMA_Channelx == GDMA_Channel3))
    {
        GDMA_Channelx->LLP = Address;
    }
}

/**
 * \brief  Check GDMA suspend channel status.
 * \param[in] GDMA_Channelx: Where x can be 0 to 5 to select the GDMA Channel.
 * \return GDMA suspend channel status.
 * \retval SET: Inactive.
 * \retval RESET: Active.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void gdma_demo(void)
 * {
 *     FlagStatus flag_status = GDMA_GetSuspendChannelStatus(GDMA_Channel0);
 * }
 * \endcode
 */
__STATIC_INLINE FlagStatus GDMA_GetSuspendChannelStatus(GDMA_ChannelTypeDef *GDMA_Channelx)
{
    FlagStatus bit_status = RESET;

    /* Check the parameters */
    assert_param(IS_GDMA_ALL_PERIPH(GDMA_Channelx));

    if ((GDMA_Channelx->CFG_LOW & GDMA_SUSPEND_CHANNEL_STATUS) == GDMA_SUSPEND_CHANNEL_STATUS)
    {
        bit_status = SET;
    }

    /* Return the selected channel suspend status */
    return  bit_status;
}

/**
 * \brief  Check GDMA suspend command status.
 * \param  GDMA_Channelx: Where x can be 0 to 5 to select the GDMA Channel.
 * \return GDMA suspend command status.
 * \retval SET: Suspend.
 * \retval RESET: Not suspend.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void gdma_demo(void)
 * {
 *     FlagStatus flag_status = GDMA_GetSuspendCmdStatus(GDMA_Channel0);
 * }
 * \endcode
 */
__STATIC_INLINE FlagStatus GDMA_GetSuspendCmdStatus(GDMA_ChannelTypeDef *GDMA_Channelx)
{
    FlagStatus bit_status = RESET;

    /* Check the parameters */
    assert_param(IS_GDMA_ALL_PERIPH(GDMA_Channelx));

    if ((GDMA_Channelx->CFG_LOW & GDMA_SUSPEND_CMD_STATUS) == GDMA_SUSPEND_CMD_STATUS)
    {
        bit_status = SET;
    }

    /* Return the selected channel suspend status */
    return  bit_status;
}

/**
 * \brief  Check GDMA FIFO status.
 * \param[in] GDMA_Channelx: Where x can be 0 to 5 to select the GDMA Channel.
 * \return GDMA FIFO status.
 * \retval SET: FIFO empty.
 * \retval RESET: FIFO not empty.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void gdma_demo(void)
 * {
 *     FlagStatus flag_status = GDMA_GetFIFOStatus(GDMA_Channel0);
 * }
 * \endcode
 */
__STATIC_INLINE FlagStatus GDMA_GetFIFOStatus(GDMA_ChannelTypeDef *GDMA_Channelx)
{
    FlagStatus bit_status = RESET;

    /* Check the parameters */
    assert_param(IS_GDMA_ALL_PERIPH(GDMA_Channelx));

    if ((GDMA_Channelx->CFG_LOW & GDMA_FIFO_STATUS) != (uint32_t)RESET)
    {

        bit_status = SET;
    }

    /* Return the selected channel status */
    return  bit_status;
}

/**
 * \brief  Get GDMA source address.
 * \param[in] GDMA_Channelx: Where x can be 0 to 5 to select the DMA Channel.
 * \return Source address.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void gdma_demo(void)
 * {
 *     uint32_t address = GDMA_GetSrcTransferAddress(GDMA_Channel0);
 * }
 * \endcode
 */
__STATIC_INLINE uint32_t GDMA_GetSrcTransferAddress(GDMA_ChannelTypeDef *GDMA_Channelx)
{
    /* Check the parameters */
    assert_param(IS_GDMA_ALL_PERIPH(GDMA_Channelx));

    uint32_t address = 0;
    address = GDMA_Channelx->SAR;
    return address;
}

/**
 * \brief  Get GDMA destination address.
 * \param[in] GDMA_Channelx: Where x can be 0 to 5 to select the DMA Channel.
 * \return Destination address.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void gdma_demo(void)
 * {
 *     uint32_t address = GDMA_GetDstTransferAddress(GDMA_Channel0);
 * }
 * \endcode
 */
__STATIC_INLINE uint32_t GDMA_GetDstTransferAddress(GDMA_ChannelTypeDef *GDMA_Channelx)
{
    /* Check the parameters */
    assert_param(IS_GDMA_ALL_PERIPH(GDMA_Channelx));

    uint32_t address = 0;
    address = GDMA_Channelx->DAR;
    return address;
}

/** \} */ /* End of group GDMA_Exported_Functions */

#ifdef __cplusplus
}
#endif

#endif /*_RTL8762X_GDMA_H_*/


/******************* (C) COPYRIGHT 2019 Realtek Semiconductor Corporation *****END OF FILE****/

