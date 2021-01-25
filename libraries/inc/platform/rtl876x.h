
/**
*****************************************************************************************
*     Copyright(c) 2017, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file    rtl876x.h
  * @brief   CMSIS Cortex-M4 Peripheral Access Layer Header File for
  *          RTL876X from Realtek Semiconductor.
  * @date    2016.3.3
  * @version v1.0

 * @date     3. March 2015
 *
 * @note     Generated with SVDConv Vx.xxp
 *           from CMSIS SVD File 'RTL876X.xml' Version x.xC,
 *
 * @par      Copyright (c) 2015 Realtek Semiconductor. All Rights Reserved.
 *
 *           The information contained herein is property of Realtek Semiconductor.
 *           Terms and conditions of usage are described in detail in Realtek
 *           SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 *           Licensees are granted free, non-transferable use of the information. NO
 *           WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 *           the file.
 *
 *

  * *************************************************************************************
   * @attention
   * <h2><center>&copy; COPYRIGHT 2017 Realtek Semiconductor Corporation</center></h2>
   * *************************************************************************************
*/

#ifndef RTL876X_H
#define RTL876X_H

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup RTL876X Rtl876x
  * @brief   CMSIS Cortex-M4 peripheral access layer header file for
  *          RTL876X from Realtek Semiconductor
  * @{
  */
/*============================================================================*
 *                              Types
*============================================================================*/
/** @defgroup RTL876x_Exported_types RTL876X Exported types
  * @{
  */

/** brief Interrupt Number Definition */
typedef enum IRQn
{
    /* -------------------  Cortex-M4 Processor Exceptions Numbers  ------------------- */
    NonMaskableInt_IRQn           = -14,      /**<  2 Non Maskable Interrupt */
    HardFault_IRQn                = -13,      /**<  3 HardFault Interrupt */
    MemoryManagement_IRQn         = -12,      /**<  4 Memory Management Interrupt */
    BusFault_IRQn                 = -11,      /**<  5 Bus Fault Interrupt */
    UsageFault_IRQn               = -10,      /**<  6 Usage Fault Interrupt */
    SVCall_IRQn                   =  -5,      /**< 11 SV Call Interrupt */
    DebugMonitor_IRQn             =  -4,      /**< 12 Debug Monitor Interrupt */
    PendSV_IRQn                   =  -2,      /**< 14 Pend SV Interrupt */
    SysTick_IRQn                  =  -1,      /**< 15 System Tick Interrupt */

    System_IRQn = 0,             /**<  [0] System Interrupt */
    WDG_IRQn,                    /**<  [1] Watch Dog Interrupt */
    BTMAC_IRQn,                  /**<  [2] BTMAC Interrupt ( an Extension of interrupt ) */
    TIMER3_IRQn,                 /**<  [3] Timer3 global interrupt */
    TIMER2_IRQn,                 /**<  [4] Timer2 global interrupt */
    Platform_IRQn,               /**<  [5] Platform error interrupt */
    I2S0_TX_IRQn,                /**<  [6] I2S0 TX interrupt */
    I2S0_RX_IRQn,                /**<  [7] I2S0 RX interrupt */
    TIMER4_7_IRQn,               /**<  [8] Timer 4 to 7 interrupt  ( an Extension of interrupt ) */
    GPIO4_IRQn,                  /**<  [9] GPIO 4 interrupt */
    GPIO5_IRQn,                  /**<  [10] GPIO 5 interrupt */
    UART1_IRQn,                  /**<  [11] UART1 interrupt */
    UART0_IRQn,                  /**<  [12] UART0 interrupt */
    RTC_IRQn,                    /**<  [13] Realtime counter interrupt */
    SPI0_IRQn,                   /**<  [14] SPI0 interrupt */
    SPI1_IRQn,                   /**<  [15] SPI1 interrupt */
    I2C0_IRQn,                   /**<  [16] I2C0 interrupt */
    I2C1_IRQn,                   /**<  [17] I2C1 interrupt */
    ADC_IRQn,                    /**<  [18] ADC global interrupt */
    Peripheral_IRQn,             /**<  [19] Peripheral Interrupt  ( an Extension of interrupt ) */
    GDMA0_Channel0_IRQn,         /**<  [20] RTK-DMA0 channel 0 global interrupt */
    GDMA0_Channel1_IRQn,         /**<  [21] RTK-DMA0 channel 1 global interrupt */
    GDMA0_Channel2_IRQn,         /**<  [22] RTK-DMA0 channel 2 global interrupt */
    GDMA0_Channel3_IRQn,         /**<  [23] RTK-DMA0 channel 3 global interrupt */
    GDMA0_Channel4_IRQn,         /**<  [24] RTK-DMA0 channel 4 global interrupt */
    GDMA0_Channel5_IRQn,         /**<  [25] RTK-DMA0 channel 5 global interrupt */
    GPIO_Group3_IRQn,            /**<  [26] GPIO Group3 Interrupt ( an Extension of interrupt ) */
    GPIO_Group2_IRQn,            /**<  [27] GPIO Group2 Interrupt ( an Extension of interrupt )  */
    IR_IRQn,                     /**<  [28] IR Interrupt */
    GPIO_Group1_IRQn,            /**<  [29] GPIO Group1 Interrupt ( an Extension of interrupt )  */
    GPIO_Group0_IRQn,            /**<  [30] GPIO Group0 Interrupt ( an Extension of interrupt )  */
    UART2_IRQn,                  /**<  [31] UART2 interrupt */
    /******  Bee2 Extension Interrupt Numbers ************/
    TIMER4_IRQn                     = 8,
    TIMER5_IRQn                     = 8,
    TIMER6_IRQn                     = 8,
    TIMER7_IRQn                     = 8,
    GPIO3_IRQn                      = 26,
    GPIO7_IRQn                      = 26,
    GPIO11_IRQn                     = 26,
    GPIO15_IRQn                     = 26,
    GPIO19_IRQn                     = 26,
    GPIO23_IRQn                     = 26,
    GPIO27_IRQn                     = 26,
    GPIO31_IRQn                     = 26,
    GPIO2_IRQn                      = 27,
    GPIO6_IRQn                      = 27,
    GPIO10_IRQn                     = 27,
    GPIO14_IRQn                     = 27,
    GPIO18_IRQn                     = 27,
    GPIO22_IRQn                     = 27,
    GPIO26_IRQn                     = 27,
    GPIO30_IRQn                     = 27,
    GPIO1_IRQn                      = 29,
    GPIO9_IRQn                      = 29,
    GPIO13_IRQn                     = 29,
    GPIO17_IRQn                     = 29,
    GPIO21_IRQn                     = 29,
    GPIO25_IRQn                     = 29,
    GPIO29_IRQn                     = 29,
    GPIO0_IRQn                      = 30,
    GPIO8_IRQn                      = 30,
    GPIO12_IRQn                     = 30,
    GPIO16_IRQn                     = 30,
    GPIO20_IRQn                     = 30,
    GPIO24_IRQn                     = 30,
    GPIO28_IRQn                     = 30,
    Peripheral_First_IRQn           = 52,
    SPIC0_IRQn                      = 52,
    qdecode_IRQn                    = 53,
    KeyScan_IRQn                    = 54,
    SPI2W_IRQn                      = 55,
    LPCOMP_IRQn                     = 56,
    PTA_Mailbox_IRQn                = 57,
    I2S1_TX_IRQn                    = 58,
    I2S1_RX_IRQn                    = 59,
    LCD_IRQn                        = 60,
    TRNG_IRQn                       = 61,
    SPIC1_IRQn                      = 62,
    SPIC2_IRQn                      = 63,
} IRQn_Type, *PIRQn_Type;
/** @} */ /* End of group RTL876x_Exported_types */

/** @defgroup Configuration_of_CMSIS Configuration of CMSIS
  * @brief   Configuration of the cm4 Processor and Core Peripherals
  * @{
  */
/* ----------------Configuration of the cm4 Processor and Core Peripherals---------------- */
#define __CM4_REV                      0x0001U      /* Core revision r0p1                                                      */
#define __MPU_PRESENT                  1            /*!< MPU present or not                                                    */
#define __FPU_PRESENT                  1            /* FPU present                                                             */
#define __NVIC_PRIO_BITS               3            /*!< Number of Bits used for Priority Levels                               */
#define __Vendor_SysTickConfig         0           /*!< Set to 1 if different SysTick Config is used                          */
/** @} */ /* End of group Configuration_of_CMSIS */

/*============================================================================*
 *                               Header Files
*============================================================================*/
#include "core_cm4.h"                       /* Processor and core peripherals */
#include "system_rtl876x.h"


/*============================================================================*
 *                              Types
*============================================================================*/
/** @addtogroup RTL876x_Exported_types RTL876X Exported types
  * @{
  */

typedef enum
{
    RESET = 0,
    SET = !RESET
} FlagStatus, ITStatus;

typedef enum
{
    DISABLE = 0,
    ENABLE = !DISABLE
} FunctionalState;

#define IS_FUNCTIONAL_STATE(STATE) (((STATE) == DISABLE) || ((STATE) == ENABLE))
//typedef enum {ERROR = 0, SUCCESS = !ERROR} ErrorStatus;

/** @} */ /* End of group RTL876x_Exported_types */



/*============================================================================*
 *                              RTL876X Pin Number
*============================================================================*/
/** @defgroup RTL876X_Pin_Number RTL876X Pin Number
  * @{
  */
#define P0_0    0   /**<GPIO0   */
#define P0_1    1   /**<GPIO1   */
#define P0_2    2   /**<GPIO2   */
#define P0_3    3   /**<GPIO3   */
#define P0_4    4   /**<GPIO4   */
#define P0_5    5   /**<GPIO5   */
#define P0_6    6   /**<GPIO6   */
#define P0_7    7   /**<GPIO7   */
#define P1_0    8   /**<GPIO8   */
#define P1_1    9   /**<GPIO9   */
#define P1_2    10   /**<GPIO10   */
#define P1_3    11   /**<GPIO11   */
#define P1_4    12   /**<GPIO12   */
#define P1_5    13   /**<GPIO13   */
#define P1_6    14   /**<GPIO14   */
#define P1_7    15   /**<GPIO15   */
#define P2_0    16   /**<GPIO16   */
#define P2_1    17   /**<GPIO17   */
#define P2_2    18   /**<GPIO18   */
#define P2_3    19   /**<GPIO19   */
#define P2_4    20   /**<GPIO20   */
#define P2_5    21   /**<GPIO21   */
#define P2_6    22   /**<GPIO22   */
#define P2_7    23   /**<GPIO23   */
#define P3_0    24   /**<GPIO24   */
#define P3_1    25   /**<GPIO25   */
#define P3_2    26   /**<GPIO26   */
#define P3_3    27   /**<GPIO27   */
#define P3_4    28   /**<GPIO28   */
#define P3_5    29   /**<GPIO29   */
#define P3_6    30   /**<GPIO30   */
#define P4_0    32   /**<GPIO28   */
#define P4_1    33   /**<GPIO29   */
#define P4_2    34   /**<GPIO30   */
#define P4_3    35   /**<GPIO31   */
#define H_0     36   /**<GPIO25   MICBIAS   */
#define H_1     37   /**<GPIO26   32K_XI   */
#define H_2     38   /**<GPIO27   32K_XO   */
/** @} */ /* End of group RTL876X_Pin_Number */


/* ================================================================================ */
/* ================    Peripheral Registers Structures Section     ================ */
/* ================================================================================ */

/** @defgroup RTL876X_Peripheral_Registers_Structures RTL876X Register Structure
  * @{
  */

/* ================================================================================ */
/* ================                      UART                      ================ */
/* ================================================================================ */

/**
  * @brief Universal Asynchronous Receiver/Transmitter, version 1.0. (UART)
  */

typedef struct                                      /*!< UART Structure */
{
    __IO  uint32_t DLL;
    __IO  uint32_t DLH_INTCR;
    __IO  uint32_t INTID_FCR;
    __IO  uint32_t LCR;
    __IO  uint32_t MCR;             /*!<0x10 */
    __I   uint32_t LSR;
    __I   uint32_t MSR;
    __IO  uint32_t SPR;
    __IO  uint32_t STSR;            /*!<0x20 */
    __IO  uint32_t RB_THR;
    __IO  uint32_t MISCR;           /*!<may not be seen for client */
    __IO  uint32_t TXPLSR;
    __IO  uint32_t RSVD0;           /*!<0x30 */
    __IO  uint32_t RSVD1;           /*!<0x34 */
    __IO  uint32_t RSVD2;           /*!<0x38 */
    __IO  uint32_t DBG_UART;        /*!<0x3C */
    __IO  uint32_t RX_IDLE_TOCR;    /*!<0x40 */
    __IO  uint32_t RX_IDLE_INTSR;
    __IO  uint32_t RX_IDLE_INTCR;
    __I   uint32_t FIFO_LEVEL;
    __IO  uint32_t INTMASK;         /*!<0x50 */
    __I   uint32_t TX_DONE_INTSR;
    __I   uint32_t TX_THR_INTSR;
} UART_TypeDef;

typedef enum
{
    LOG_CHANNEL_UART0,
    LOG_CHANNEL_UART1,
    LOG_CHANNEL_UART2
} LogChannel_TypeDef;

/* ================================================================================ */
/* ================                 2WIRE_SPI                      ================ */
/* ================================================================================ */

/**
  * @brief 2wire spi, mostly used with mouse sensor. (2WIRE_SPI)
  */

typedef struct                                      /*!< 3WIRE_SPI Structure */
{
    __IO  uint32_t RSVD0[12];       /*!< 0x00 -- 0x2C for Q-decode */
    __IO  uint32_t CFGR;            /*!< 0x30 */
    __IO  uint32_t CR;
    __IO  uint32_t INTCR;
    __I   uint32_t SR;
    __IO  uint32_t RD0;             /*!< 0x40 */
    __IO  uint32_t RD1;
    __IO  uint32_t RD2;
    __IO  uint32_t RD3;
} SPI3WIRE_TypeDef;

/* ================================================================================ */
/* ================                 IR                             ================ */
/* ================================================================================ */

/**
  * @brief IR.
  */

typedef struct
{
    __IO  uint32_t CLK_DIV;         /*!< 0x00 */
    __IO  uint32_t TX_CONFIG;       /*!< 0x04 */
    __IO  uint32_t TX_SR;           /*!< 0x08 */
    __IO  uint32_t RSVD0;           /*!< 0x0c */
    __IO  uint32_t TX_INT_CLR;      /*!< 0x10 */
    __IO  uint32_t TX_FIFO;         /*!< 0x14 */
    __IO  uint32_t RX_CONFIG;       /*!< 0x18 */
    __IO  uint32_t RX_SR;           /*!< 0x1c */
    __IO  uint32_t RX_INT_CLR;      /*!< 0x20 */
    __IO  uint32_t RX_CNT_INT_SEL;  /*!< 0x24 */
    __IO  uint32_t RX_FIFO;         /*!< 0x28 */
    __IO  uint32_t IR_VERSION;      /*!< 0x2c */
    __IO  uint32_t RSVD1[8];
    __IO  uint32_t DMA_CONFIG;      /*!< 0x50 */
} IR_TypeDef;

/* ================================================================================ */
/* ================                       SPI                      ================ */
/* ================================================================================ */

/**
  * @brief SPI master 0. (SPI)
  */
typedef struct                                          /*!< SPI Structure  */
{
    __IO  uint32_t    CTRLR0;                              /*!< 0x00 */
    __IO  uint32_t    CTRLR1;                              /*!< 0x04 */
    __IO  uint32_t    SSIENR;                              /*!< 0x08 */
    __IO  uint32_t    RSVD_0C;                             /*!< 0x0C */
    __IO  uint32_t    SER;                                 /*!< 0x10 */
    __IO  uint32_t    BAUDR;                               /*!< 0x14 */
    __IO  uint32_t    TXFTLR;                              /*!< 0x18 */
    __IO  uint32_t    RXFTLR;                              /*!< 0x1C */
    __I  uint32_t    TXFLR;                                /*!< 0x20 */
    __I  uint32_t    RXFLR;                                /*!< 0x24 */
    __I  uint32_t    SR;                                   /*!< 0x28 */
    __IO  uint32_t    IMR;                                 /*!< 0x2C */
    __I  uint32_t    ISR;                                  /*!< 0x30 */
    __I  uint32_t    RISR;                                 /*!< 0x34 */
    __I  uint32_t    TXOICR;                               /*!< 0x38 */
    __I  uint32_t    RXOICR;                               /*!< 0x3C */
    __I  uint32_t    RXUICR;                               /*!< 0x40 */
    __I  uint32_t    FAEICR;/*RSVD_44;*/                   /*!< 0x44 */
    __I  uint32_t    ICR;                                  /*!< 0x48 */
    __IO  uint32_t    DMACR;                               /*!< 0x4C */
    __IO  uint32_t    DMATDLR;                             /*!< 0x50 */
    __IO  uint32_t    DMARDLR;                             /*!< 0x54 */
    __I  uint32_t    TXUICR;/*IDR;*/                       /*!< 0x58 */
    __I  uint32_t    SSRICR;                               /*!< 0x5C */
    __IO  uint32_t    DR[36];                              /*!< 0x60 - 0xEC */
    __IO  uint32_t    RX_SAMPLE_DLY;                       /*!< 0xF0 */
} SPI_TypeDef;

/* ================================================================================ */
/* ================                      I2C                      ================= */
/* ================================================================================ */

/**
  * @brief I2C
  */
typedef struct
{
    __IO uint32_t IC_CON;
    __IO uint32_t IC_TAR;
    __IO uint32_t IC_SAR;
    __IO uint32_t IC_HS_MADDR;
    __IO uint32_t IC_DATA_CMD;
    __IO uint32_t IC_SS_SCL_HCNT;
    __IO uint32_t IC_SS_SCL_LCNT;
    __IO uint32_t IC_FS_SCL_HCNT;
    __IO uint32_t IC_FS_SCL_LCNT;
    __IO uint32_t IC_HS_SCL_HCNT;
    __IO uint32_t IC_HS_SCL_LCNT;
    __I uint32_t IC_INTR_STAT;
    __IO uint32_t IC_INTR_MASK;
    __I uint32_t IC_RAW_INTR_STAT;
    __IO uint32_t IC_RX_TL;
    __IO uint32_t IC_TX_TL;
    __I uint32_t IC_CLR_INTR;
    __I uint32_t IC_CLR_RX_UNDER;
    __I uint32_t IC_CLR_RX_OVER;
    __I uint32_t IC_CLR_TX_OVER;
    __I uint32_t IC_CLR_RD_REQ;
    __I uint32_t IC_CLR_TX_ABRT;
    __I uint32_t IC_CLR_RX_DONE;
    __I uint32_t IC_CLR_ACTIVITY;
    __I uint32_t IC_CLR_STOP_DET;
    __I uint32_t IC_CLR_START_DET;
    __I uint32_t IC_CLR_GEN_CALL;
    __IO uint32_t IC_ENABLE;
    __I uint32_t IC_STATUS;
    __I uint32_t IC_TXFLR;
    __I uint32_t IC_RXFLR;
    __IO uint32_t IC_SDA_HOLD;
    __I uint32_t IC_TX_ABRT_SOURCE;
    __IO uint32_t IC_SLV_DATA_NACK_ONLY;
    __IO uint32_t IC_DMA_CR;
    __IO uint32_t IC_DMA_TDLR;
    __IO uint32_t IC_DMA_RDLR;
    __IO uint32_t IC_SDA_SETUP;
    __IO uint32_t IC_ACK_GENERAL_CALL;
    __IO uint32_t IC_ENABLE_STATUS;
} I2C_TypeDef;

/* ================================================================================ */
/* ================                       ADC                      ================ */
/* ================================================================================ */

/**
  * @brief Analog to digital converter. (ADC)
  */

typedef struct                                      /*!< ADC Structure */
{
    __O  uint32_t FIFO;        //!<0x00
    __IO uint32_t CR;          //!<0x04
    __IO uint32_t SCHCR;       //!<0x08
    __IO uint32_t INTCR;       //!<0x0C
    __IO uint32_t SCHTAB0;     //!<0x10
    __IO uint32_t SCHTAB1;     //!<0x14
    __IO uint32_t SCHTAB2;     //!<0x18
    __IO uint32_t SCHTAB3;     //!<0x1C
    __IO uint32_t SCHTAB4;     //!<0x20
    __IO uint32_t SCHTAB5;     //!<0x24
    __IO uint32_t SCHTAB6;     //!<0x28
    __IO uint32_t SCHTAB7;     //!<0x2C
    __IO uint32_t SCHD0;       //!<0x30
    __IO uint32_t SCHD1;       //!<0x34
    __IO uint32_t SCHD2;       //!<0x38
    __IO uint32_t SCHD3;       //!<0x3C
    __IO uint32_t SCHD4;       //!<0x40
    __IO uint32_t SCHD5;       //!<0x44
    __IO uint32_t SCHD6;       //!<0x48
    __IO uint32_t SCHD7;       //!<0x4C
    __IO uint32_t PWRDLY;      //!<0x50
    __IO uint32_t DATCLK;      //!<0x54
    __IO uint32_t ANACTL;      //!<0x58

} ADC_TypeDef;

/* =============================================================================== */
/* ================                      TIM                      ================ */
/* =============================================================================== */

/**
  * @brief TIM
  */
typedef struct
{
    __IO uint32_t LoadCount;        /*!< 0x00*/
    __I  uint32_t CurrentValue;     /*!< 0x04*/
    __IO uint32_t ControlReg;       /*!< 0x08*/
    __I  uint32_t EOI;              /*!< 0x0C*/
    __I  uint32_t IntStatus;        /*!< 0x10*/
} TIM_TypeDef;

/* ============================================================================== */
/* ================                      PWM                     ================ */
/* ============================================================================== */

/**
  * @brief PWM
  */
typedef struct
{
    __IO uint32_t CR;               /*!< 0x00 */
} PWM_TypeDef;

/* ================================================================================ */
/* ================                      GDMA                      ================ */
/* ================================================================================ */
/**
  * @brief GDMA
  */
typedef struct
{
    __I uint32_t RAW_TFR;
    uint32_t RSVD0;
    __I uint32_t RAW_BLOCK;
    uint32_t RSVD1;
    __I uint32_t RAW_SRC_TRAN;
    uint32_t RSVD2;
    __I uint32_t RAW_DST_TRAN;
    uint32_t RSVD3;
    __I uint32_t RAW_ERR;
    uint32_t RSVD4;

    __I uint32_t STATUS_TFR;
    uint32_t RSVD5;
    __I uint32_t STATUS_BLOCK;
    uint32_t RSVD6;
    __I uint32_t STATUS_SRC_TRAN;
    uint32_t RSVD7;
    __I uint32_t STATUS_DST_TRAN;
    uint32_t RSVD8;
    __I uint32_t STATUS_ERR;
    uint32_t RSVD9;

    __IO uint32_t MASK_TFR;
    uint32_t RSVD10;
    __IO uint32_t MASK_BLOCK;
    uint32_t RSVD11;
    __IO uint32_t MASK_SRC_TRAN;
    uint32_t RSVD12;
    __IO uint32_t MASK_DST_TRAN;
    uint32_t RSVD13;
    __IO uint32_t MASK_ERR;
    uint32_t RSVD14;

    __O uint32_t CLEAR_TFR;
    uint32_t RSVD15;
    __O uint32_t CLEAR_BLOCK;
    uint32_t RSVD16;
    __O uint32_t CLEAR_SRC_TRAN;
    uint32_t RSVD17;
    __O uint32_t CLEAR_DST_TRAN;
    uint32_t RSVD18;
    __O uint32_t CLEAR_ERR;
    uint32_t RSVD19;
    __O uint32_t StatusInt;
    uint32_t RSVD191;

    __IO uint32_t ReqSrcReg;
    uint32_t RSVD20;
    __IO uint32_t ReqDstReg;
    uint32_t RSVD21;
    __IO uint32_t SglReqSrcReg;
    uint32_t RSVD22;
    __IO uint32_t SglReqDstReg;
    uint32_t RSVD23;
    __IO uint32_t LstSrcReg;
    uint32_t RSVD24;
    __IO uint32_t LstDstReg;
    uint32_t RSVD25;

    __IO uint32_t DmaCfgReg;
    uint32_t RSVD26;
    __IO uint32_t ChEnReg;
    uint32_t RSVD27;
    __I uint32_t DmaIdReg;
    uint32_t RSVD28;
    __IO uint32_t DmaTestReg;
    uint32_t RSVD29;
} GDMA_TypeDef;

typedef struct
{
    __IO uint32_t SAR;
    uint32_t RSVD0;
    __IO uint32_t DAR;
    uint32_t RSVD1;
    __IO uint32_t LLP;
    uint32_t RSVD2;
    __IO uint32_t CTL_LOW;
    __IO uint32_t CTL_HIGH;
    __IO uint32_t SSTAT;
    uint32_t RSVD4;
    __IO uint32_t DSTAT;
    uint32_t RSVD5;
    __IO uint32_t SSTATAR;
    uint32_t RSVD6;
    __IO uint32_t DSTATAR;
    uint32_t RSVD7;
    __IO uint32_t CFG_LOW;
    __IO uint32_t CFG_HIGH;
    __IO uint32_t SGR;
    uint32_t RSVD9;
    __IO uint32_t DSR;
    uint32_t RSVD10;
} GDMA_ChannelTypeDef;

/* ================================================================================ */
/* ================                       RTC                      ================ */
/* ================================================================================ */

/**
  * @brief Real time counter 0. (RTC)
  */

typedef struct                                      /*!< RTC Structure                                                         */
{
    __IO uint32_t CR0;              /*!< 0x00 */
    __IO uint32_t INT_CLR;          /*!< 0x04 */
    __IO uint32_t INT_SR;           /*!< 0x08 */
    __IO uint32_t PRESCALER;        /*!< 0x0C */
    __IO uint32_t COMP0;            /*!< 0x10 */
    __IO uint32_t COMP1;            /*!< 0x14 */
    __IO uint32_t COMP2;            /*!< 0x18 */
    __IO uint32_t COMP3;            /*!< 0x1C */
    __I  uint32_t CNT;              /*!< 0x20 */
    __IO uint32_t PRE_CNT;          /*!< 0x24 */
    __IO uint32_t PRE_COMP;         /*!< 0x28 */
} RTC_TypeDef;
/**
  * @brief Real time for lp (RTC)
  */
typedef struct
{
    __IO uint32_t LPC_CR0;
    __IO uint32_t LPC_SR;
    __IO uint32_t LPC_CMP;
    __IO uint32_t LPC_CNT;
} LPC_TypeDef;

///**
//  * @brief SLEEP MODE LED (RTC)
//  */

//typedef struct
//{
//    __IO uint32_t S_LED_CR;         //!<0x00
//    __IO uint32_t S_LED_CH0_CR0;    //!<0x04
//    __IO uint32_t S_LED_CH0_CR1;    //!<0x08
//    __IO uint32_t S_LED_CH0_P1_CR;  //!<0x0C
//    __IO uint32_t S_LED_CH0_P2_CR;  //!<0x10
//    __IO uint32_t S_LED_CH0_P3_CR;  //!<0x14
//    __IO uint32_t S_LED_CH0_P4_CR;  //!<0x18
//    __IO uint32_t S_LED_CH0_P5_CR;  //!<0x1C
//    __IO uint32_t S_LED_CH0_P6_CR;  //!<0x20
//    __IO uint32_t S_LED_CH0_P7_CR;  //!<0x24
//    __IO uint32_t S_LED_CH0_P8_CR;  //!<0x28

//    __IO uint32_t RSV0;             //!<0x2C

//    __IO uint32_t S_LED_CH1_CR0;    //!<0x30
//    __IO uint32_t S_LED_CH1_CR1;    //!<0x34
//    __IO uint32_t S_LED_CH1_P1_CR;  //!<0x38
//    __IO uint32_t S_LED_CH1_P2_CR;  //!<0x3C
//    __IO uint32_t S_LED_CH1_P3_CR;  //!<0x40
//    __IO uint32_t S_LED_CH1_P4_CR;  //!<0x44
//    __IO uint32_t S_LED_CH1_P5_CR;  //!<0x48
//    __IO uint32_t S_LED_CH1_P6_CR;  //!<0x4C
//    __IO uint32_t S_LED_CH1_P7_CR;  //!<0x50
//    __IO uint32_t S_LED_CH1_P8_CR;  //!<0x54

//    __IO uint32_t RSV1;             //!<0x58
//    __IO uint32_t RSV2;             //!<0x5C

//    __IO uint32_t S_LED_CH2_CR0;    //!<0x60
//    __IO uint32_t S_LED_CH2_CR1;    //!<0x64
//    __IO uint32_t S_LED_CH2_P1_CR;  //!<0x68
//    __IO uint32_t S_LED_CH2_P2_CR;  //!<0x6C
//    __IO uint32_t S_LED_CH2_P3_CR;  //!<0x70
//    __IO uint32_t S_LED_CH2_P4_CR;  //!<0x74
//    __IO uint32_t S_LED_CH2_P5_CR;  //!<0x78
//    __IO uint32_t S_LED_CH2_P6_CR;  //!<0x7C
//    __IO uint32_t S_LED_CH2_P7_CR;  //!<0x80
//    __IO uint32_t S_LED_CH2_P8_CR;  //!<0x84
//} RTC_LED_TypeDef;

/* ================================================================================ */
/* ================                      QDEC                      ================ */
/* ================================================================================ */

/**
  * @brief Rotary decoder. (QDEC)
  */

typedef struct                                      /*!< QDEC Structure           */
{
    __IO uint32_t   REG_DIV;        /*!< 0x00 */
    __IO uint32_t   REG_CR_X;       /*!< 0x04 */
    __IO uint32_t   REG_SR_X;       /*!< 0x08 */
    __IO uint32_t   REG_CR_Y;       /*!< 0x0C */
    __IO uint32_t   REG_SR_Y;       /*!< 0x10 */
    __IO uint32_t   REG_CR_Z;       /*!< 0x14 */
    __IO uint32_t   REG_SR_Z;       /*!< 0x18 */
    __IO uint32_t   INT_MASK;       /*!< 0x1C */
    __IO uint32_t   INT_SR;         /*!< 0x20 */
    __IO uint32_t   INT_CLR;        /*!< 0x24 */
    __IO uint32_t   REG_DBG;        /*!< 0x28 */
    __IO uint32_t   REG_VERSION;    /*!< 0x2C  */
} QDEC_TypeDef;

/* ================================================================================ */
/* ================                     Watch Dog                  ================ */
/* ================================================================================ */

/**
  * @brief Watch Dog. (WDG)
  */

typedef struct                                   /*!< WDG Structure           */
{
    __IO uint32_t WDG_CTL;          /*!< 0x00 */
} WDG_TypeDef;

/**
  * @brief AON Watchdog
  */
typedef struct
{
    union
    {
        __IO uint32_t CRT; /*!< 0x00 (R/W) Control Register                                       */
        struct
        {
            uint32_t EN: 2;     /*!< R/W WDT2 Enable, 2'b10: OFF, the other value: ON             */
            uint32_t CNT_CTL: 1; /*!< R/W Counter Control,
                                     1: WDG continue count in DLPS/Power Down/Hibernate,
                                     0: WDG stop count in DLPS/Power Down/Hibernate               */
            uint32_t RST_LVL: 1; /*!< R/W Reset Level,
                                   1: Level 2 reset (reset whole chip),
                                   0: Level 1 reset (reset whole chip except partial AON and RTC) */
            uint32_t CNT_RELOAD: 1; /*!< R/W Reload Counter,
                                        1: Reload counter when exit DLPS/Power Down/Hibernate,
                                        0: Not reload counter when exit DLPS/Power Down/Hibernate */
            uint32_t RSVD0: 3;
            uint32_t COMP: 18;  /*!< R/W Set the period of AON WDG : 1/450Hz ~ 1/2.3KHz           */
            uint32_t RSVD1: 6;
        } CRT_BITS;
    } u;
    __IO uint32_t CNT_CLR; /*!< 0x04 (R/W) Clear Register, write 1 to reload AON WDG counter      */
    __IO uint32_t WP;      /*!< 0x08 (R/W) Write Protect Register, write 1 to enable write CRT    */
} AON_WDG_TypeDef;

/* ================================================================================ */
/* ================                     random generator           ================ */
/* ================================================================================ */

/**
  * @brief random generator. (RAN_GEN)
  */

typedef struct                                      /*!< RAN_GEN Structure */
{
    union
    {
        __IO uint32_t CTL;                                        /*!< 0x00              */
        struct
        {
            __IO uint32_t rand_gen_en: 1;
            __IO uint32_t seed_upd: 1;
            __IO uint32_t random_req: 1;
            __IO uint32_t opt_rand_upd: 1;
            __IO uint32_t soft_rst: 1;
            __IO uint32_t rsvd: 27;
        } CTL_BITS;
    } u;
    __IO uint32_t POLYNOMIAL;                                     /*!< 0x04              */
    __IO uint32_t SEED;                                           /*!< 0x08              */
    __IO uint32_t RAN_NUM;                                        /*!< 0x0C              */
} RAN_GEN_TypeDef;

/* ================================================================================ */
/* ================            System Block Control            ================ */
/* ================================================================================ */

/**
  * @brief System Block Control. (SYS_BLKCTRL)
  */

typedef struct                                          /*!< SYS_BLKCTRL Structure      */
{
    union
    {
        __IO uint32_t        SYS_CLK_SEL;                      /*!< 0x200  */
        struct
        {
            __IO uint32_t r_cpu_slow_en: 1;
            __IO uint32_t r_cpu_slow_opt_wfi: 1;
            __IO uint32_t r_cpu_slow_opt_at_tx: 1;
            __IO uint32_t r_cpu_slow_opt_at_rx: 1;
            __IO uint32_t rsvd0: 2;
            __IO uint32_t r_zone1_wrp_en: 1;
            __IO uint32_t r_zone2_wrp_en: 1;
            __IO uint32_t r_wrp_en_lock: 1;
            __IO uint32_t rsvd1: 10;
            __IO uint32_t r_bt_ahb_wait_cnt: 6;
            __IO uint32_t rsvd2: 3;
            __IO uint32_t r_aon_rd_opt: 1;
            __IO uint32_t r_btaon_acc_no_block: 1;
            __IO uint32_t bzdma_autoslow_eco_disable: 1;
            __IO uint32_t rsvd3: 1;
        } BITS_200;
    } u_200;
    __IO uint32_t    RSVD_204;                             /*!< 0x204     */
    union
    {
        __IO uint32_t        CTRL_208;                         /*!< 0x208  */
        struct
        {
            __IO uint32_t r_cpu_div_sel: 4;
            __IO uint32_t r_cpu_div_sel_slow: 4;
            __IO uint32_t r_cpu_div_en: 1;
            __IO uint32_t r_CPU_CLK_SRC_EN: 1;
            __IO uint32_t r_dsp_auto_slow_filter_en: 1;
            __IO uint32_t r_cpu_clk_mux_0_cg_en: 1;
            __IO uint32_t r_cpu_pll_clk_cg_en: 1; //12
            __IO uint32_t r_cpu_xtal_clk_cg_en: 1;
            __IO uint32_t r_cpu_osc40_clk_cg_en: 1;
            __IO uint32_t r_cpu_div_en_slow: 1;
            __IO uint32_t r_disable_pll_clk: 1; //16 W1O
            __IO uint32_t r_lock_cpu_divider: 1;//17 W1O
            __IO uint32_t r_dw_spi0_clock: 1;   //18 W1O
            __IO uint32_t r_dw_spi1_clock: 1;   //19 W1O
            __IO uint32_t rsvd1: 1; //20
            __IO uint32_t r_disable_dataram0: 1;//21 W1O
            __IO uint32_t r_disable_dataram1: 1;//22 W1O
            __IO uint32_t rsvd2: 2; //23 - 24
            __IO uint32_t r_disable_spic1: 1; //25
            __IO uint32_t r_disable_spic2: 1; //26
            __IO uint32_t rsvd3: 5; //27 - 31
        } BITS_208;
    } u_208;
    union
    {
        __IO uint32_t        RSVD_20C;                         /*!< 0x20C */
        struct
        {
            __IO uint32_t r_flash_div_sel: 4;
            __IO uint32_t r_flash_div_en: 1;
            __IO uint32_t r_FLASH_CLK_SRC_EN: 1;
            __IO uint32_t r_flash_clk_src_sel_1: 1;
            __IO uint32_t r_flash_clk_src_sel_0: 1;
            __IO uint32_t r_flash_mux_1_clk_cg_en: 1;
            __IO uint32_t r_rng_sfosc_sel: 1;
            __IO uint32_t rsvd1: 12;
            __IO uint32_t r_CLK_40M_SRC_EN: 1;
            __IO uint32_t rsvd2: 1;
            __IO uint32_t r_40m_clk_src_sel_0: 1;
            __IO uint32_t rsvd3: 1;
            __IO uint32_t r_CLK_40M_SRC_DIV_EN: 1;
            __IO uint32_t r_CLK_20M_SRC_EN: 1;
            __IO uint32_t r_CLK_10M_SRC_EN: 1;
            __IO uint32_t r_CLK_5M_SRC_EN: 1;
            __IO uint32_t rsvd4: 2;
        } BITS_20C;
    } u_20C;
    union
    {
        __IO uint32_t        SOC_FUNC_EN;                      /*!< 0x210 */
        struct
        {
            __IO uint32_t rsvd0: 2;
            __IO uint32_t BIT_SOC_BTBUS_EN: 1;
            __IO uint32_t BIT_SOC_FLASH_EN1: 1;
            __IO uint32_t BIT_SOC_FLASH_EN: 1;
            __IO uint32_t BIT_SOC_FLASH_EN2: 1;
            __IO uint32_t rsvd2: 6;
            __IO uint32_t BIT_SOC_LOG_UART_EN: 1;
            __IO uint32_t BIT_SOC_GDMA0_EN: 1;
            __IO uint32_t rsvd3: 2;
            __IO uint32_t BIT_SOC_GTIMER_EN: 1;
            __IO uint32_t rsvd4: 15;
        } BITS_210;
    } u_210;
    __IO uint32_t    RSVD1;                                /*!< 0x214     */
    union
    {
        __IO uint32_t        PERI_FUNC0_EN;                    /*!< 0x218    */
        struct
        {
            __IO uint32_t BIT_PERI_UART0_EN: 1;
            __IO uint32_t BIT_PERI_LOGUART1_EN: 1;
            __IO uint32_t BIT_PERI_AES_EN: 1;
            __IO uint32_t BIT_PERI_RNG_EN: 1;
            __IO uint32_t rsvd0: 4;
            __IO uint32_t BIT_PERI_SPI0_EN: 1;
            __IO uint32_t BIT_PERI_SPI1_EN: 1;
            __IO uint32_t BIT_PERI_IR_EN: 1;
            __IO uint32_t rsvd1: 5;
            __IO uint32_t BIT_PERI_I2C0_EN: 1;
            __IO uint32_t BIT_PERI_I2C1_EN: 1;
            __IO uint32_t BIT_PERI_QDECODE_EN: 1;
            __IO uint32_t BIT_PERI_KEYSCAN_EN: 1;
            __IO uint32_t rsvd2: 4;
            __IO uint32_t BIT_PERI_SPI2W_EN: 1;
            __IO uint32_t BIT_LCD_EN: 1;
            __IO uint32_t rsvd3: 4;
            __IO uint32_t BIT_EFUSE_EN: 1;
            __IO uint32_t rsvd4: 1;
        } BITS_218;
    } u_218;
    union
    {
        __IO uint32_t        PERI_FUNC1_EN;                    /*!< 0x21C      */
        struct
        {
            __IO uint32_t BIT_PERI_ADC_EN: 1;
            __IO uint32_t rsvd0: 7;
            __IO uint32_t BIT_PERI_GPIO_EN: 1;
            __IO uint32_t rsvd1: 23;
        } BITS_21C;
    } u_21C;
    __IO uint32_t    u_220;                                 /*!< 0x220  */
    __IO uint32_t    RSVD2[3];                             /*!< 0x224~0x22C   */
    union
    {
        __IO uint32_t        CLK_CTRL;                         /*!< 0x230  */
        struct
        {
            __IO uint32_t BIT_CKE_HWSPI: 1;
            __IO uint32_t BIT_CKE_CORDIC: 1;
            __IO uint32_t BIT_SOC_CKE_PLFM: 1;
            __IO uint32_t BIT_CKE_EFC: 1;
            __IO uint32_t BIT_CKE_BUS_RAM_SLP: 1;
            __IO uint32_t BIT_CKE_BT_VEN: 1;
            __IO uint32_t BIT_SOC_ACTCK_VENDOR_REG_EN: 1;
            __IO uint32_t BIT_SOC_SLPCK_VENDOR_REG_EN: 1;
            __IO uint32_t BIT_SOC_ACTCK_FLASH_EN: 1;
            __IO uint32_t BIT_SOC_SLPCK_FLASH_EN: 1;
            __IO uint32_t BIT_SOC_ACTCK_LOG_UART1_EN: 1;
            __IO uint32_t BIT_SOC_SLPCK_LOG_UART1_EN: 1;
            __IO uint32_t BIT_SOC_ACTCK_LOG_UART_EN: 1;
            __IO uint32_t BIT_SOC_SLPCK_LOG_UART_EN: 1;
            __IO uint32_t BIT_SOC_ACTCK_TIMER_EN: 1;
            __IO uint32_t BIT_SOC_SLPCK_TIMER_EN: 1;
            __IO uint32_t BIT_SOC_ACTCK_GDMA0_EN: 1;
            __IO uint32_t BIT_SOC_SLPCK_GDMA0_EN: 1;
            __IO uint32_t rsvd0: 6;
            __IO uint32_t BIT_SOC_ACTCK_GPIO_EN: 1;
            __IO uint32_t BIT_SOC_SLPCK_GPIO_EN: 1;
            __IO uint32_t rsvd1: 6;
        } BITS_230;
    } u_230;
    union
    {
        __IO uint32_t        PERI_CLK_CTRL0;                   /*!< 0x234     */
        struct
        {
            __IO uint32_t BIT_SOC_ACTCK_UART0_DATA_EN: 1;
            __IO uint32_t BIT_SOC_SLPCK_UART0_DATA_EN: 1;
            __IO uint32_t BIT_SOC_ACTCK_UART1_HCI_EN: 1;
            __IO uint32_t BIT_SOC_SLPCK_UART1_HCI_EN: 1;
            __IO uint32_t BIT_CKE_MODEM: 1;
            __IO uint32_t BIT_CKE_CAL32K: 1;
            __IO uint32_t BIT_CKE_RNG: 1;
            __IO uint32_t rsvd0: 9;
            __IO uint32_t BIT_SOC_ACTCK_SPI0_EN: 1;
            __IO uint32_t BIT_SOC_SLPCK_SPI0_EN: 1;
            __IO uint32_t BIT_SOC_ACTCK_SPI1_EN: 1;
            __IO uint32_t BIT_SOC_SLPCK_SPI1_EN: 1;
            __IO uint32_t BIT_SOC_ACTCK_IR_EN: 1;
            __IO uint32_t BIT_SOC_SLPCK_IR_EN: 1;
            __IO uint32_t rsvd1: 2;
            __IO uint32_t BIT_SOC_ACTCK_FLASH1_EN: 1;
            __IO uint32_t BIT_SOC_SLPCK_FLASH1_EN: 1;
            __IO uint32_t BIT_SOC_ACTCK_FLASH2_EN: 1;
            __IO uint32_t BIT_SOC_SLPCK_FLASH2_EN: 1;
            __IO uint32_t rsvd2: 4;
        } BITS_234;
    } u_234;
    union
    {
        __IO uint32_t        PERI_CLK_CTRL1;                   /*!< 0x238    */
        struct
        {
            __IO uint32_t BIT_SOC_ACTCK_I2C0_EN: 1;
            __IO uint32_t BIT_SOC_SLPCK_I2C0_EN: 1;
            __IO uint32_t BIT_SOC_ACTCK_I2C1_EN: 1;
            __IO uint32_t BIT_SOC_SLPCK_I2C1_EN: 1;
            __IO uint32_t BIT_SOC_ACTCK_QDECODE_EN: 1;
            __IO uint32_t BIT_SOC_SLPCK_QDECODE_EN: 1;
            __IO uint32_t BIT_SOC_ACTCK_KEYSCAN_EN: 1;
            __IO uint32_t BIT_SOC_SLPCK_KEYSCAN_EN: 1;
            __IO uint32_t BIT_SOC_ACTCK_AES_EN: 1;
            __IO uint32_t BIT_SOC_SLPCK_AES_EN: 1;
            __IO uint32_t rsvd0: 6;
            __IO uint32_t BIT_SOC_ACTCK_SPI_3WIRE_EN: 1;
            __IO uint32_t BIT_SOC_SLPCK_SPI_3WIRE_EN: 1;
            __IO uint32_t rsvd1: 6;
            __IO uint32_t BIT_SOC_ACTCK_ADC_EN: 1;
            __IO uint32_t BIT_SOC_SLPCK_ADC_EN: 1;
            __IO uint32_t rsvd2: 2;
            __IO uint32_t BIT_SOC_ACTCKE_LCD: 1;
            __IO uint32_t BIT_SOC_SLPCKE_LCD: 1;
            __IO uint32_t rsvd3: 1;
            __IO uint32_t BIT_SOC_CLK_EFUSE: 1;
        } BITS_238;
    } u_238;
    __IO uint32_t    RSVD3[2];                             /*!< 0x23C~0x240  */
    union
    {
        __IO uint32_t        COM_CLK_CTRL1;                    /*!< 0x244    */
        struct
        {
            __IO uint32_t BIT_SOC_ACTCK_BTBUS_EN: 1;
            __IO uint32_t BIT_SOC_SLPCK_BTBUS_EN: 1;
            __IO uint32_t rsvd0: 20;
            __IO uint32_t r_LS_mcu_rom: 1;
            __IO uint32_t rsvd1: 9;
        } BITS_244;
    } u_244;
    __IO uint32_t    RSVD4[24];                            /*!< 0x248~0x2A4     */
    union
    {
        __IO uint32_t REG_2A8;
        __IO uint32_t        REG_TEST_MODE;                    /*!< 0x2A8  */
        struct
        {
            __IO uint32_t PMUX_TEST_MODE: 4;
            __IO uint32_t rsvd: 3;
            __IO uint32_t PMUX_TEST_MODE_EN: 1;
            __IO uint32_t SPIC1_MASTER_EN: 1;
            __IO uint32_t SPIC2_MASTER_EN: 1;
            __IO uint32_t rsvd0: 6;
            __IO uint32_t PMUX_DBG_MODE_SEL: 4;
            __IO uint32_t rsvd1: 1;
            __IO uint32_t PMUX_LCD_RD_DIS: 1;
            __IO uint32_t PMUX_LCD_VSYNC_DIS: 1;
            __IO uint32_t PMUX_DIG_SMUX_EN: 1;
            __IO uint32_t SPIC_MASTER_EN: 1;
            __IO uint32_t PMUX_SPIM_EN_OPT: 1;
            __IO uint32_t PMUX_FLASH_EXTC_EN: 1;
            __IO uint32_t PMUX_FLASH_EN: 1;
            __IO uint32_t PMUX_HCI_UART_DISABLE: 1;
            __IO uint32_t rsvd2: 3;
        } BITS_2A8;
    } u_2A8;
    __IO uint32_t    RSVD5[29];                            /*!< 0x2AC~0x31C   */
    union
    {
        __IO uint32_t BT_ANAPAR_PLL;                           /*!< 0x320  */
        struct
        {
            //PLL0
            __IO uint32_t reg_fref_edge: 1; //0
            __IO uint32_t reg_dogenb:   1; //1
            __IO uint32_t reg_cp_bia:   3; //2:4
            __IO uint32_t reg_vc_THL:   2; //5:6
            __IO uint32_t reg_rs_set:   3; //7:9
            __IO uint32_t reg_cs_set:   2; //10:11
            __IO uint32_t reg_cp_set:   2; //12:13
            __IO uint32_t pow_od:       1; //14
            __IO uint32_t reg_pll:      1; //15
            //PLL1
            __IO uint32_t reg_r3_set:   3; //16:18
            __IO uint32_t reg_c3_set:   2; //19:20
            __IO uint32_t reg_div:      6; //21:26
            __IO uint32_t reg_CKO_en:   1; //27
            __IO uint32_t dummy:        4; //28:31
        } BITS_320;
    } u_320;
    __IO uint32_t    RSVD6[2];                             /*!< 0x324~0x328   */
    union
    {
        __IO uint32_t        RG0X_40M;                         /*!< 0x32C  */
        struct
        {
            __IO uint32_t FSET_OSC40: 4;
            __IO uint32_t rsvd0: 28;
        } BITS_32C;
    } u_32C;
    union
    {
        __IO uint32_t        PERI_ON_330;                      /*!< 0x330 */
        struct
        {
            __IO uint32_t rst_n_aac: 1;
            __IO uint32_t Offset_plus: 1;
            __IO uint32_t XAAC_GM_offset: 6;
            __IO uint32_t GM_STEP: 1;
            __IO uint32_t GM_INIT: 6;
            __IO uint32_t XTAL_CLK_SET: 3;
            __IO uint32_t GM_STUP: 6;
            __IO uint32_t GM_MANUAL: 6;
            __IO uint32_t r_EN_XTAL_AAC_DIGI: 1;
            __IO uint32_t r_EN_XTAL_AAC_TRIG: 1;
        } BITS_330;
    } u_330;
    union
    {
        __IO uint32_t        PERI_ON_334;                      /*!< 0x334  */
        struct
        {
            __IO uint32_t XAAC_BUSY: 1;
            __IO uint32_t XAAC_READY: 1;
            __IO uint32_t XTAL_GM_OUT: 6;
            __IO uint32_t xaac_curr_state: 4;
            __IO uint32_t EN_XTAL_AAC_GM: 1;
            __IO uint32_t EN_XTAL_AAC_PKDET: 1;
            __IO uint32_t rsvd0: 18;
        } BITS_334;
    } u_334;
    union
    {
        __IO uint32_t        PERI_ON_338;                      /*!< 0x338  */
        struct
        {
            __IO uint32_t rsvd0: 24;
            __IO uint32_t BT_PLL_READY: 1;
            __IO uint32_t XTAL_OK: 1;
            __IO uint32_t rsvd1: 6;
        } BITS_338;
    } u_338;
} SYS_BLKCTRL_TypeDef;

/* ================================================================================ */
/* ================                     Pinmux                     ================ */
/* ================================================================================ */

/**
  * @brief Pinmux. (Pinmux)
  */

typedef struct                                      /*!< Pinmux Structure */
{
    __IO uint32_t
    CFG[10];                              /*!<                                            */
} PINMUX_TypeDef;

/* ================================================================================ */
/* ================                   Peripheral                   ================ */
/* ================================================================================ */

/**
  * @brief Peripheral. (Peripheral)
  */

typedef struct                                      /*!< Peripheral Structure */
{
    uint32_t RSVD0[134];                                /*!< Reserved */
    __IO uint32_t    PERI_FUNC0_EN;                     /*!< 0x218 */
    __IO uint32_t    PERI_FUNC1_EN;                     /*!< 0x21C */
    __IO uint32_t    PERI_BD_FUNC0_EN;                  /*!< 0x220 */
    uint32_t RSVD1[3];                                  /*!< Reserved */
    __IO uint32_t    PERI_CLK_CTRL;                     /*!< 0x230 */
    __IO uint32_t    PERI_CLK_CTRL0;                    /*!< 0x234 */
    __IO uint32_t    PERI_CLK_CTRL1;                    /*!< 0x238 */
} PHERIPHERIAL_TypeDef;

/* ================================================================================ */
/* ================              Peripheral Interrupt              ================ */
/* ================================================================================ */

/**
  * @brief Peripheral Interrupt. (Peripheral Interrupt)
  */

typedef struct                 /*!< Peripheral Interrupt Structure */
{
    __IO uint32_t RSVD0;       /*!< 0x00 */
    __IO uint32_t STATUS;      /*!< 0x04 */
    __IO uint32_t MODE;        /*!< 0x08, 0: high level active, 1: edge trigger */
    __IO uint32_t EN;          /*!< 0x0C */
    __IO uint32_t RSVD1;       /*!< 0x10 */
    __IO uint32_t RSVD2;       /*!< 0x14 */
    __IO uint32_t EDGE_MODE;   /*!< 0x18, 0: rising edge (HW default), 1: both edge */
} PERI_INT_TypeDef;

/* ================================================================================ */
/* ================                    Security                    ================ */
/* ================================================================================ */

/**
  * @brief Security. (Security)
  */

typedef union                                               /*!< Security Structure */
{
    __IO uint32_t d32;
    struct
    {
        __IO uint32_t reg_pon_debug_port_wp: 1;
        __IO uint32_t reg_pon_debug_port: 1;
        __IO uint32_t reg_pon_hwspi_en_wp: 1;
        __IO uint32_t reg_pon_hwspi_en: 1;
        __IO uint32_t is_efuse_invalid: 1;
        __IO uint32_t reg_efuse_system_general_ctrl_lock: 1;
        __IO uint32_t rsvd0: 2;
        __IO uint32_t is_enable_efuse_read_protect: 1;
        __IO uint32_t is_enable_efuse_write_protect: 1;
        __IO uint32_t is_disable_hci_ram_patch: 1;
        __IO uint32_t is_disable_hci_flash_access: 1;
        __IO uint32_t is_disable_hci_system_access: 1;
        __IO uint32_t is_disable_hci_efuse_access: 1;
        __IO uint32_t is_disable_hci_bt_test: 1;
        __IO uint32_t is_debug_password_invalid: 1;
        __IO uint32_t rsvd1: 4;
        __IO uint32_t reg_pon_clk_flash_dis: 1;
        __IO uint32_t rsvd2: 11;
    };
} SECURITY_TypeDef;

/* ================================================================================ */
/* ================                    Key Scan                    ================ */
/* ================================================================================ */

/**
  * @brief Key Scan. (KeyScan)
  */

typedef struct                                      /*!< KeyScan Structure */
{

    __IO uint32_t CLKDIV;               /*!< 0x00 */
    __IO uint32_t TIMERCR;              /*!< 0x04 */
    __IO uint32_t CR;                   /*!< 0x08 */
    __IO uint32_t COLCR;                /*!< 0x0C */
    __IO uint32_t ROWCR;                /*!< 0x10 */
    __I  uint32_t FIFODATA;             /*!< 0x14 */
    __IO uint32_t INTMASK;              /*!< 0x18 */
    __IO uint32_t INTCLR;               /*!< 0x1C */
    __I  uint32_t STATUS;               /*!< 0x20 */

} KEYSCAN_TypeDef;

/* ======================================================== */
/* ================                      GPIO                       ================ */
/* ======================================================== */

/**
  * @brief General purpose input and output. (GPIO)
  */

typedef struct
{
    __IO uint32_t DATAOUT;                              /*!< Data register: data output */
    __IO uint32_t DATADIR;                              /*!< Data direction register */
    __IO uint32_t DATASRC;                              /*!< Data source register  */
    uint32_t RSVD[9];
    __IO uint32_t INTEN;                                /*!< Interrupt enable register */
    __IO uint32_t INTMASK;                              /*!< Interrupt mask register */
    __IO uint32_t INTTYPE;                              /*!< Interrupt level register */
    __IO uint32_t INTPOLARITY;                          /*!< Interrupt polarity register */
    __IO uint32_t INTSTATUS;                            /*!< Interrupt status of Port A  */
    __IO uint32_t RAWINTSTATUS;           /*!< Raw interrupt status of Port A (premasking) */
    __IO uint32_t DEBOUNCE;                             /*!< Debounce enable register */
    __O  uint32_t INTCLR;                               /*!< clear interrupt register */
    __I  uint32_t DATAIN;                               /*!< external port register */
    uint32_t RSVD1[3];
    __IO uint32_t LSSYNC;                 /*!< Level-sensitive synchronization enable register*/
    __I  uint32_t IDCODE;                               /*!< ID code register */
    __IO uint32_t INTBOTHEDGE;                          /*!< Both edge to trigger interrupt*/
} GPIO_TypeDef;

/* ============================================================================== */
/* ================                 Peri clock reg               ================ */
/* ============================================================================== */

/**
  * @brief PWM
  */
typedef struct
{
    __IO uint32_t CLKSELE;              /*!< 0x348*/
    uint32_t RSVD[4];
    __IO uint32_t CLK_SRCL;
    __IO uint32_t CLK_SRCH;             /*!< 0x360*/
    __IO uint32_t PWM0_CTRL_L;
    __IO uint32_t PWM0_CTRL_H;
    __IO uint32_t PWM1_CTRL_L;
    __IO uint32_t PWM1_CTRL_H;          /*!< 0x370*/
    __IO uint32_t PWM2_CTRL_L;
    __IO uint32_t PWM2_CTRL_H;
    __IO uint32_t PWM3_CTRL_L;
    __IO uint32_t PWM3_CTRL_H;          /*!< 0x380*/
    __IO uint32_t TIM_EVT_CTRL;

} Peri_ClockGate_TypeDef;


/* ======================================================= */
/* ================                      CACHE                     ================ */
/* ======================================================= */

/**
  * @brief cache for flash
  */
typedef struct
{
    __IO uint32_t CACHE_ENABLE;      /*!< SPIC cache Enable Register, Address Offset: 0x00*/
    __IO uint32_t FLUSH;             /*!< Cache Flush register, Address Offset: 0x04*/
    __IO uint32_t INTR;              /*!< Cache Interrupt register, Address Offset: 0x08*/
    __O  uint32_t RST_CNT;           /*!< Cache Reset Counter register, Address Offset: 0x0C*/
    __I  uint32_t RD_EVT_CNT;        /*!< Cache Read Event Counter register, Address Offset: 0x10*/
    __I  uint32_t HIT_EVT_CNT;       /*!< Cache HIT Event Counter register, Address Offset: 0x14*/
    __I  uint32_t HIT_LSTW_EVT_CNT;  /*!< Cache Hit lastway event counter register, Offset: 0x18*/
    __I  uint32_t RD_PND_CNT;        /*!< Cache Read pending counter register, Offset: 0x1c*/
    __I  uint32_t RSVD0[4];          /*!< Reserved, Offset: 0x20 ~ 0x2C */
    __IO uint32_t HITWAYBUF_CTRL;    /*!< Cache Hit way buffer control register, Offset: 0x30 */
    __IO uint32_t CACHE_RAM_CTRL;    /*!< Cache Two way function control register, Offset: 0x34 */
    __IO uint32_t COUNTER_ENABLE;    /*!< Cache counter enable register, Offset: 0x38 */
} CACHE_TypeDef;

/* ================================================================================ */
/* ================                      SPIC                      ================ */
/* ================================================================================ */
typedef struct
{
    __IO uint32_t ctrlr0;           /*!< SPIC control register0,        Address offset: 0x000 */
    __IO uint32_t ctrlr1;           /*!< SPIC control register1,        Address offset: 0x004 */
    __IO uint32_t ssienr;           /*!< SPIC enable register,      Address offset: 0x008 */
    __IO uint32_t mwcr;         /*!< N/A,                   Address offset: 0x00C */
    __IO uint32_t ser;              /*!< SPIC slave enable register,    Address offset: 0x010 */
    __IO uint32_t baudr;            /*!< SPIC baudrate select register, Address offset: 0x014 */
    __IO uint32_t txftlr;       /*!< SPIC transmit FIFO threshold level,    Address offset: 0x018 */
    __IO uint32_t rxftlr;       /*!< SPIC receive FIFO threshold level, Address offset: 0x01C */
    __IO uint32_t txflr;            /*!< SPIC transmit FIFO level register, Address offset: 0x020 */
    __IO uint32_t rxflr;            /*!< SPIC receive FIFO level register,  Address offset: 0x024 */
    __IO uint32_t sr;               /*!< SPIC status register,              Address offset: 0x028 */
    __IO uint32_t imr;              /*!< SPIC interrupt mask register,      Address offset: 0x02C */
    __IO uint32_t isr;          /*!< SPIC interrupt status register,        Address offset: 0x030 */
    __IO uint32_t risr;         /*!< SPIC raw interrupt status register,    Address offset: 0x034 */
    __IO uint32_t txoicr;/*!< SPIC transmit FIFO overflow interrupt clear register,offset: 0x038 */
    __IO uint32_t rxoicr;/*!< SPIC receive FIFO overflow interrupt clear register, offset: 0x03C */
    __IO uint32_t rxuicr;/*!< SPIC receive FIFO underflow interrupt clear register,offset: 0x040 */
    __IO uint32_t msticr;/*!< SPIC master error interrupt clear register,   Address offset: 0x044 */
    __IO uint32_t icr;              /*!< SPIC interrupt clear register, Address offset: 0x048 */
    __IO uint32_t dmacr;            /*!< N/A,                   Address offset: 0x04C */
    __IO uint32_t dmatdlr;          /*!< N/A,                   Address offset: 0x050 */
    __IO uint32_t dmardlr;          /*!< N/A,                   Address offset: 0x054 */
    __IO uint32_t idr;              /*!< SPIC Identiation register,     Address offset: 0x058 */
    __IO uint32_t spi_flash_version;    /*!< SPIC version ID register,      Address offset: 0x05C */
    union
    {
        __IO uint8_t  byte;
        __IO uint16_t half;
        __IO uint32_t word;
    } dr[32];              /*!< SPIC data register,                 Address offset: 0x060~0x0DC */
    __IO uint32_t rd_fast_single;   /*!< Fast read data command of SPI Flash, offset: 0x0E0 */
    __IO uint32_t rd_dual_o;        /*!< Dual output read command of SPI Flash, offset: 0x0E4 */
    __IO uint32_t rd_dual_io;       /*!< Dual I/O read command of SPI Flash, offset: 0x0E8 */
    __IO uint32_t rd_quad_o;        /*!< Quad output read command of SPI Flash, offset: 0x0EC */
    __IO uint32_t rd_quad_io;       /*!< Quad I/O read command of SPI Flash, offset: 0x0F0 */
    __IO uint32_t wr_single;            /*!< Page program command of SPI Flash,  offset: 0x0F4 */
    __IO uint32_t wr_dual_i;    /*!< Dual data input program command of SPI Flash, offset: 0x0F8 */
    __IO uint32_t wr_dual_ii;/*!< Dual address&data program command of SPI Flash,offset: 0x0FC */
    __IO uint32_t wr_quad_i;    /*!< Quad data input program command of SPI Flash,offset: 0x100 */
    __IO uint32_t wr_quad_ii;/*!< Quad address&data program command of SPI Flash,offset: 0x104 */
    __IO uint32_t wr_enable;        /*!< Write enabe command of SPI Flash,  Address offset: 0x108 */
    __IO uint32_t rd_status;            /*!< Read status command of SPI Flash,   offset: 0x10C */
    __IO uint32_t ctrlr2;           /*!< SPIC control register2,            Address offset: 0x110 */
    __IO uint32_t fbaudr;           /*!< SPIC fast baudrate select,         Address offset: 0x114 */
    __IO uint32_t addr_length;      /*!< SPIC address length register,      Address offset: 0x118 */
    __IO uint32_t auto_length;      /*!< SPIC auto address length register, Address offset: 0x11C */
    __IO uint32_t valid_cmd;        /*!< SPIC valid command register,       Address offset: 0x120 */
    __IO uint32_t flash_size;   /*!< SPIC flash size register,           offset: 0x124 */
    __IO uint32_t flush_fifo;           /*!< SPIC flush FIFO register,   offset: 0x128 */
} SPIC_TypeDef;

typedef struct
{
    __IO uint32_t ANA_CR0;          /*!< 0x00 */
    __IO uint32_t ANA_CR1;          /*!< 0x04 */
    __IO uint32_t ANA_CR2;          /*!< 0x08 */
} CODEC_AnalogTypeDef;

typedef struct
{
    __IO uint32_t CR0;              /*!< 0x00 */
    __IO uint32_t ANA_CR1;          /*!< 0x04 */
    __IO uint32_t CLK_CR1;          /*!< 0x08 */
    __IO uint32_t CLK_CR2;          /*!< 0x0C */
    __IO uint32_t CLK_CR3;          /*!< 0x10 */
    __IO uint32_t ASRC_CR0;         /*!< 0x14 */
    __IO uint32_t ASRC_CR1;         /*!< 0x18 */
    __IO uint32_t I2S_CTRL;         /*!< 0x1C */
    __IO uint32_t ADC0_CTRL0;       /*!< 0x20 */
    __IO uint32_t ADC0_CTRL1;       /*!< 0x24 */
    __IO uint32_t ADC1_CTRL0;       /*!< 0x28 */
    __IO uint32_t ADC1_CTRL1;       /*!< 0x2C */
    __IO uint32_t DAC_CTRL0;        /*!< 0x30 */
    __IO uint32_t DAC_CTRL1;        /*!< 0x34 */
} CODEC_TypeDef;

typedef struct
{
    __IO uint32_t EQ_H0;            /*!< 0x40 */
    __IO uint32_t EQ_B1;            /*!< 0x44 */
    __IO uint32_t EQ_B2;            /*!< 0x48 */
    __IO uint32_t EQ_A1;            /*!< 0x4C */
    __IO uint32_t EQ_A2;            /*!< 0x50 */
} CODEC_EQTypeDef;

/**
  * @brief I2S
  */
typedef struct
{
    __O  uint32_t TX_DR;            /*!< 0x00 */
    __IO uint32_t CTRL0;            /*!< 0x04 */
    __IO uint32_t CTRL1;            /*!< 0x08 */
    __IO uint32_t DSP_INT_CR;       /*!< 0x0C */
    __I  uint32_t RX_DR;            /*!< 0x10 */
    __I  uint32_t FIFO_SR;          /*!< 0x14 */
    __IO uint32_t ERROR_CNT_SR;     /*!< 0x18 */
    __IO uint32_t BCLK_DIV;         /*!< 0x1C */
    __IO uint32_t DMA_TRDLR;        /*!< 0x20 */
    __I  uint32_t SR;               /*!< 0x24 */
} I2S_TypeDef;

/**
  * @brief IF8080
  */
typedef struct
{
    __IO uint32_t CTRL0;            /*!< 0x00 */
    __IO uint32_t CTRL1;            /*!< 0x04 */
    __IO uint32_t IMR;              /*!< 0x08 */
    __I  uint32_t SR;               /*!< 0x0C */
    __O  uint32_t ICR;              /*!< 0x10 */
    __IO uint32_t CFG;              /*!< 0x14 */
    __IO uint32_t FIFO;             /*!< 0x18 */
    __I  uint32_t RESV0;            /*!< 0x1C */
    __I  uint32_t RXDATA;           /*!< 0x20 */
    __IO uint32_t TX_LEN;           /*!< 0x24 */
    __I  uint32_t TX_CNT;           /*!< 0x28 */
    __I  uint32_t RESV1;            /*!< 0x2C */
    __IO uint32_t RX_LEN;           /*!< 0x30 */
    __I  uint32_t RX_CNT;           /*!< 0x34 */
    __IO uint32_t CMD1;             /*!< 0x38 */
    __IO uint32_t CMD2;             /*!< 0x3C */
    __IO uint32_t CMD3;             /*!< 0x40 */
} IF8080_TypeDef;

typedef struct
{
    __IO uint32_t SAR;
    __IO uint32_t DAR;
    __IO uint32_t LLP;
    __IO uint32_t CTL_LOW;
    __IO uint32_t CTL_HIGH;
} IF8080_GDMALLITypeDef;

typedef struct
{
    __IO uint32_t CR;
    __IO uint32_t LLI;
} IF8080_GDMATypeDef;


typedef struct
{
    __IO uint32_t SAR_OFT;
    __IO uint32_t DAR_OFT;
} IF8080_GDMALLIOFTTypeDef;


/** @} */ /* End of group RTL876X_Peripheral_Registers_Structures */

/*============================================================================*
 *                              Macros
 *============================================================================*/

/** @defgroup RTL876X_Exported_Macros RTL876X  Exported Macros
    * @brief
    * @{
    */

/* ================================================================================ */
/* ================              Peripheral memory map             ================ */
/* ================================================================================ */
#define SYSTEM_REG_BASE             0x40000000UL
#define PERIPH_REG_BASE             0x40000000UL
#define RTC_REG_BASE                0x40000140UL
#define RTC_LP_REG_BASE             0x40000180UL
#define LPC_REG_BASE                0x40000180UL
#define AON_WDG_REG_BASE            0x40000190UL
#define SYSBLKCTRL_REG_BASE         0x40000200UL
#define PINMUX_REG_BASE             0x40000280UL
#define SPIC_DLY_CTRL_BASE          0x40000300UL
#define PERICLKGAT_REG_BASE         0x40000348UL
#define PWM2_REG_BASE               0x40000364UL
#define PWM3_REG_BASE               0x40000368UL
#define RTC_LED_REG_BASE            0x40000400UL

#define GPIO_REG_BASE               0x40001000UL
#define TIM0_REG_BASE               0x40002000UL
#define TIM1_REG_BASE               0x40002014UL
#define TIM2_REG_BASE               0x40002028UL
#define TIM3_REG_BASE               0x4000203CUL
#define TIM4_REG_BASE               0x40002050UL
#define TIM5_REG_BASE               0x40002064UL
#define TIM6_REG_BASE               0x40002078UL
#define TIM7_REG_BASE               0x4000208CUL
#define QDEC_REG_BASE               0x40004000UL
#define SPI2WIRE_REG_BASE           0x40004000UL
#define KEYSCAN_REG_BASE            0x40005000UL
#define VENDOR_REG_BASE             0x40006000UL
#define WDG_REG_BASE                0x40006000UL
#define PERI_INT_REG_BASE           0x40006004UL
#define RANDOM_GEN_REG_BASE         0x40006150UL
#define CACHE_REG_BASE              0x40007000UL
#define ADC_REG_BASE                0x40010000UL
#define UART1_REG_BASE              0x40011000UL
#define UART0_REG_BASE              0x40012000UL
#define SPI0_REG_BASE               0x40013000UL
#define SPI1_REG_BASE               0x40013400UL
#define HW_AES_REG_BASE             0x40014000UL
#define I2C0_REG_BASE               0x40015000UL
#define I2C1_REG_BASE               0x40015400UL
#define IR_REG_BASE                 0x40016000UL
#define SPORT0_REG_BASE             0x40020000UL
#define I2S0_REG_BASE               0x40020000UL
#define SPORT1_REG_BASE             0x40021000UL
#define I2S1_REG_BASE               0x40021000UL
#define SPDIF_REG_BASE              0x40021800UL
#define CODEC_ANA_REG_BASE          0x40022000UL
#define CODEC_EQ1_REG_BASE          0x40022040UL
#define CODEC_EQ2_REG_BASE          0x40022054UL
#define CODEC_EQ3_REG_BASE          0x40022068UL
#define CODEC_EQ4_REG_BASE          0x4002207CUL
#define CODEC_EQ5_REG_BASE          0x40022090UL
#define CODEC_REG_BASE              0x40022100UL
#define CODEC_CH0_CR_REG_BASE       0x40022200UL
#define CODEC_CH0_EQ1_REG_BASE      0x40022204UL
#define CODEC_CH0_EQ2_REG_BASE      0x40022218UL
#define CODEC_CH0_EQ3_REG_BASE      0x4002222CUL
#define CODEC_CH0_EQ4_REG_BASE      0x40022240UL
#define CODEC_CH0_EQ5_REG_BASE      0x40022254UL
#define CODEC_CH1_CR_REG_BASE       0x40022268UL
#define CODEC_CH1_EQ1_REG_BASE      0x40022280UL
#define CODEC_CH1_EQ2_REG_BASE      0x40022294UL
#define CODEC_CH1_EQ3_REG_BASE      0x400222A8UL
#define CODEC_CH1_EQ4_REG_BASE      0x400222BCUL
#define CODEC_CH1_EQ5_REG_BASE      0x400222D0UL
#define H2D_D2H_REG_BASE            0x40023000UL
#define UART2_REG_BASE              0x40024000UL
#define IF8080_REG_BASE             0x40024800UL
#define IF8080_LLI_REG1_BASE        0x40024850UL
#define IF8080_LLI_REG1_GDMA_BASE   0x400248A0UL
#define IF8080_LLI_REG2_BASE        0x40024880UL
#define IF8080_LLI_REG2_GDMA_BASE   0x400248C0UL
#define IF8080_LLI_REG1_OFT_BASE    0x40024870UL
#define IF8080_LLI_REG2_OFT_BASE    0x40024878UL
#define IF8080_LLI_CR_REG_BASE      0x40024894UL
#define GDMA_CHANNEL_REG_BASE       0x40027000UL // for platform_1_1_1_20160323 later
#define GDMA_Channel0_BASE          (GDMA_CHANNEL_REG_BASE + 0x0000)
#define GDMA_Channel1_BASE          (GDMA_CHANNEL_REG_BASE + 0x0058)
#define GDMA_Channel2_BASE          (GDMA_CHANNEL_REG_BASE + 0x00b0)
#define GDMA_Channel3_BASE          (GDMA_CHANNEL_REG_BASE + 0x0108)
#define GDMA_Channel4_BASE          (GDMA_CHANNEL_REG_BASE + 0x0160)
#define GDMA_Channel5_BASE          (GDMA_CHANNEL_REG_BASE + 0x01b8)
#define GDMA_REG_BASE               (GDMA_CHANNEL_REG_BASE + 0x02c0)
#define RTL_SPI0_BASE               0x40042000UL
#define RTL_SPI1_BASE               0x40042400UL
#define BT_BB_REG_BASE              0x40050000UL
#define HCI_DMA_REG_BASE            0x40064000UL
#define HCI_UART_REG_BASE           0x40068000UL
#define BT_VENDOR_REG_BASE          0x40058000UL
#define SECURITY_REG_BASE           0x40058f28UL
#define GDMA0_REG_BASE              0x40060000UL
#define SPIC0_REG_BASE              0x40080000UL
#define SPIC1_REG_BASE              0x40090000UL
#define SPIC2_REG_BASE              0x400A0000UL

#define SPI0_MASTER_MODE_REG *((volatile uint32_t *)0x40000308UL)
#define SPI0_MASTER_MODE_BIT BIT(8)

/** @brief clock divider for peripheral */
#define SYSTEM_CLK_CTRL  *((volatile uint32_t *)0x4000020CUL)
#define CLK_SOURCE_REG_0 *((volatile uint32_t *)0x40000348UL)
#define CLK_SOURCE_REG_1 *((volatile uint32_t *)0x4000035CUL)
#define CLK_SOURCE_REG_2 *((volatile uint32_t *)0x40000360UL)

#define PERI_CLOCKGATE_REG_BASE     0x40000348UL

#define REG_PEON_SYS_CLK_SEL        0x0200
#define REG_PEON_SYS_CLK_SEL_2      0x0208
#define REG_PEON_SYS_CLK_SEL_3      0x020C
#define REG_SOC_FUNC_EN             0x0210
#define REG_SOC_HCI_COM_FUNC_EN     0x0214
#define REG_SOC_PERI_FUNC0_EN       0x0218
#define REG_SOC_PERI_FUNC1_EN       0x021C
#define REG_PESOC_CLK_CTRL          0x0230
#define REG_PESOC_PERI_CLK_CTRL0    0x0234
#define REG_PESOC_PERI_CLK_CTRL1    0x0238
#define REG_PESOC_DSP_SHARE_RAM     0x0250
#define REG_TEST_MODE               0x02a8
#define REG_ANAPAR_PLL1_0           0x0320
#define REG_ANAPAR_PLL3_2           0x0324
#define REG_ANAPAR_PLL5_4           0x0328
#define REG_XTAL_PLL_READY          0x0338

#define GPIO_OUTPUT_OFFSET          0x00
#define GPIO_DIRECTION_OFFSET       0x04

/** @brief AON PAD AREA */
#define REG_PAD_WKEN_ADDRESS        0x20
#define REG_PAD_WK_CTRL_ADDRESS     0x12d
#define REG_PAD_WKPOL_ADDRESS       0x25
#define REG_PAD_O_ADDRESS           0x2A
#define REG_AON_PAD_E_ADDRESS       0x2F
#define REG_AON_PAD_S_ADDRESS       0x34
#define REG_AON_PAD_PU_ADDRESS      0x39
#define REG_AON_PAD_PD_ADDRESS      0x3E
#define REG_AON_PAD_PWRON_ADDRESS   0x4C

#define BIT_WK_INTEN                (1U << 7) /**< BIT7 */
#define BIT_WK_FLAG_GPIO            (1U << 6) /**< BIT6 */

#define BIT_SOC_ACTCK_GPIO_EN       (1U << 24) /**< BIT24 */
#define BIT_PERI_GPIO_EN            (1U << 8) /**< BIT8 */

#define BIT_PERI_KEYSCAN_EN         (1U << 19) /**< BIT19 */
#define BIT_SOC_ACTCK_KEYSCAN_EN    (1U << 6) /**< BIT6 */


/* ================================================================================ */
/* ================             Peripheral declaration             ================ */
/* ================================================================================ */
/** @brief System */
#define PERIPH                          ((PHERIPHERIAL_TypeDef  *) PERIPH_REG_BASE)
#define SYSBLKCTRL                      ((SYS_BLKCTRL_TypeDef   *) SYSBLKCTRL_REG_BASE)
#define PERIPHINT                       ((PERI_INT_TypeDef      *) VENDOR_REG_BASE)
#define SECURITY                        ((SECURITY_TypeDef      *) SECURITY_REG_BASE)

/** @brief IO */
#define PINMUX                          ((PINMUX_TypeDef        *) PINMUX_REG_BASE)
#define KEYSCAN                         ((KEYSCAN_TypeDef       *) KEYSCAN_REG_BASE)
#define GPIO                            ((GPIO_TypeDef          *) GPIO_REG_BASE)
#define QDEC                            ((QDEC_TypeDef          *) QDEC_REG_BASE)
#define I2C0                            ((I2C_TypeDef           *) I2C0_REG_BASE)
#define I2C1                            ((I2C_TypeDef           *) I2C1_REG_BASE)
#define SPI0                            ((SPI_TypeDef           *) SPI0_REG_BASE)
#define SPI1                            ((SPI_TypeDef           *) SPI1_REG_BASE)
#define TIM0                            ((TIM_TypeDef           *) TIM0_REG_BASE)
#define TIM1                            ((TIM_TypeDef           *) TIM1_REG_BASE)
#define TIM2                            ((TIM_TypeDef           *) TIM2_REG_BASE)
#define TIM3                            ((TIM_TypeDef           *) TIM3_REG_BASE)
#define TIM4                            ((TIM_TypeDef           *) TIM4_REG_BASE)
#define TIM5                            ((TIM_TypeDef           *) TIM5_REG_BASE)
#define TIM6                            ((TIM_TypeDef           *) TIM6_REG_BASE)
#define TIM7                            ((TIM_TypeDef           *) TIM7_REG_BASE)
#define PWM2                            ((PWM_TypeDef           *) PWM2_REG_BASE)
#define PWM3                            ((PWM_TypeDef           *) PWM3_REG_BASE)
#define GDMA_BASE                       ((GDMA_TypeDef *) GDMA_REG_BASE)
#define GDMA_Channel0                   ((GDMA_ChannelTypeDef *) GDMA_Channel0_BASE)
#define GDMA_Channel1                   ((GDMA_ChannelTypeDef *) GDMA_Channel1_BASE)
#define GDMA_Channel2                   ((GDMA_ChannelTypeDef *) GDMA_Channel2_BASE)
#define GDMA_Channel3                   ((GDMA_ChannelTypeDef *) GDMA_Channel3_BASE)
#define GDMA_Channel4                   ((GDMA_ChannelTypeDef *) GDMA_Channel4_BASE)
#define GDMA_Channel5                   ((GDMA_ChannelTypeDef *) GDMA_Channel5_BASE)
#define ADC                             ((ADC_TypeDef             *) ADC_REG_BASE)
#define UART0                           ((UART_TypeDef            *) UART0_REG_BASE)
#define UART1                           ((UART_TypeDef            *) UART1_REG_BASE)
#define UART2                           ((UART_TypeDef            *) UART2_REG_BASE)
#define SPI3WIRE                        ((SPI3WIRE_TypeDef        *) SPI2WIRE_REG_BASE)
#define IR                              ((IR_TypeDef              *) IR_REG_BASE)
#define RTC                             ((RTC_TypeDef             *) RTC_REG_BASE)
#define RTC_LP                          ((RTC_LP_TypeDef             *) RTC_LP_REG_BASE)
#define RTC_LED                         ((RTC_LED_TypeDef         *) RTC_LED_REG_BASE)
#define LPC                             ((LPC_TypeDef             *) LPC_REG_BASE)
#define HWAES                           ((HW_AES_TypeDef          *) HW_AES_REG_BASE)
#define WDG                             ((WDG_TypeDef             *) WDG_REG_BASE)
#define AON_WDG                         ((AON_WDG_TypeDef         *) AON_WDG_REG_BASE)
#define RAN_GEN                         ((RAN_GEN_TypeDef         *) RANDOM_GEN_REG_BASE)
#define ICG                             ((ICG_TypeDef             *) ICG_REG_BASE)
#define CACHE                           ((CACHE_TypeDef           *) CACHE_REG_BASE)
#define SPIC                            ((SPIC_TypeDef            *) SPIC0_REG_BASE)
#define SPIC0                           ((SPIC_TypeDef            *) SPIC0_REG_BASE)
#define SPIC1                           ((SPIC_TypeDef            *) SPIC1_REG_BASE)
#define SPIC2                           ((SPIC_TypeDef            *) SPIC2_REG_BASE)
#define CODEC_ANA                       ((CODEC_AnalogTypeDef*)CODEC_ANA_REG_BASE)
#define CODEC                           ((CODEC_TypeDef*) CODEC_REG_BASE)
#define CODEC_CH0_CR                    (*((volatile uint32_t *)CODEC_CH0_CR_REG_BASE))
#define CODEC_CH0_EQ1                   ((CODEC_EQTypeDef*) CODEC_CH0_EQ1_REG_BASE)
#define CODEC_CH0_EQ2                   ((CODEC_EQTypeDef*) CODEC_CH0_EQ2_REG_BASE)
#define CODEC_CH0_EQ3                   ((CODEC_EQTypeDef*) CODEC_CH0_EQ3_REG_BASE)
#define CODEC_CH0_EQ4                   ((CODEC_EQTypeDef*) CODEC_CH0_EQ4_REG_BASE)
#define CODEC_CH0_EQ5                   ((CODEC_EQTypeDef*) CODEC_CH0_EQ5_REG_BASE)
#define CODEC_CH1_CR                    (*((volatile uint32_t *)CODEC_CH1_CR_REG_BASE))
#define CODEC_CH1_EQ1                   ((CODEC_EQTypeDef*) CODEC_CH1_EQ1_REG_BASE)
#define CODEC_CH1_EQ2                   ((CODEC_EQTypeDef*) CODEC_CH1_EQ2_REG_BASE)
#define CODEC_CH1_EQ3                   ((CODEC_EQTypeDef*) CODEC_CH1_EQ3_REG_BASE)
#define CODEC_CH1_EQ4                   ((CODEC_EQTypeDef*) CODEC_CH1_EQ4_REG_BASE)
#define CODEC_CH1_EQ5                   ((CODEC_EQTypeDef*) CODEC_CH1_EQ5_REG_BASE)
#define I2S0                            ((I2S_TypeDef*) I2S0_REG_BASE)
#define I2S1                            ((I2S_TypeDef*) I2S1_REG_BASE)
#define IF8080                          ((IF8080_TypeDef*) IF8080_REG_BASE)
#define IF8080_LLPGROUP1                ((IF8080_GDMALLITypeDef*) IF8080_LLI_REG1_BASE)
#define IF8080_LLPGROUP2                ((IF8080_GDMALLITypeDef*) IF8080_LLI_REG2_BASE)
#define IF8080_LLPGROUP1_GDMA           ((IF8080_GDMALLITypeDef*) IF8080_LLI_REG1_GDMA_BASE)
#define IF8080_LLPGROUP2_GDMA           ((IF8080_GDMALLITypeDef*) IF8080_LLI_REG2_GDMA_BASE)

#define IF8080_LLPGROUP1_OFT            ((IF8080_GDMALLIOFTTypeDef*) IF8080_LLI_REG1_OFT_BASE)
#define IF8080_LLPGROUP2_OFT            ((IF8080_GDMALLIOFTTypeDef*) IF8080_LLI_REG2_OFT_BASE)
#define IF8080_GDMA                     ((IF8080_GDMATypeDef*)IF8080_LLI_CR_REG_BASE)

#define CLK_GATE                        ((Peri_ClockGate_TypeDef  *) PERI_CLOCKGATE_REG_BASE)



//Add by Vendor
#define LITTLE_ENDIAN                         0
#define BIG_ENDIAN                              1
#define SYSTEM_ENDIAN                        LITTLE_ENDIAN

#define SWAP32(x) ((uint32_t)(                         \
                                                       (((uint32_t)(x) & (uint32_t)0x000000ff) << 24) |            \
                                                       (((uint32_t)(x) & (uint32_t)0x0000ff00) <<  8) |            \
                                                       (((uint32_t)(x) & (uint32_t)0x00ff0000) >>  8) |            \
                                                       (((uint32_t)(x) & (uint32_t)0xff000000) >> 24)))

#define WAP16(x) ((uint16_t)(                         \
                                                      (((uint16_t)(x) & (uint16_t)0x00ff) <<  8) |            \
                                                      (((uint16_t)(x) & (uint16_t)0xff00) >>  8)))

#if SYSTEM_ENDIAN == LITTLE_ENDIAN
#ifndef rtk_le16_to_cpu
#define rtk_cpu_to_le32(x)      ((uint32_t)(x))
#define rtk_le32_to_cpu(x)      ((uint32_t)(x))
#define rtk_cpu_to_le16(x)      ((uint16_t)(x))
#define rtk_le16_to_cpu(x)      ((uint16_t)(x))
#define rtk_cpu_to_be32(x)      SWAP32((x))
#define rtk_be32_to_cpu(x)      SWAP32((x))
#define rtk_cpu_to_be16(x)      WAP16((x))
#define rtk_be16_to_cpu(x)      WAP16((x))
#endif

#elif SYSTEM_ENDIAN == BIG_ENDIAN
#ifndef rtk_le16_to_cpu
#define rtk_cpu_to_le32(x)      SWAP32((x))
#define rtk_le32_to_cpu(x)      SWAP32((x))
#define rtk_cpu_to_le16(x)      WAP16((x))
#define rtk_le16_to_cpu(x)      WAP16((x))
#define rtk_cpu_to_be32(x)      ((uint32_t)(x))
#define rtk_be32_to_cpu(x)      ((uint32_t)(x))
#define rtk_cpu_to_be16(x)      ((uint16_t)(x))
#define rtk_be16_to_cpu(x)      ((uint16_t)(x))
#endif
#endif

#define HAL_READ32(base, addr)            \
    rtk_le32_to_cpu(*((volatile uint32_t *)(base + addr)))

#define HAL_WRITE32(base, addr, value32)  \
    ((*((volatile uint32_t *)(base + addr))) = rtk_cpu_to_le32(value32))

#define HAL_UPDATE32(addr, mask, value32)  \
    HAL_WRITE32(0, addr, (HAL_READ32(0, addr) & ~mask) | (value32 & mask))

#define HAL_READ16(base, addr)            \
    rtk_le16_to_cpu(*((volatile uint16_t *)(base + addr)))

#define HAL_WRITE16(base, addr, value)  \
    ((*((volatile uint16_t *)(base + addr))) = rtk_cpu_to_le16(value))

#define HAL_UPDATE16(addr, mask, value16)  \
    HAL_WRITE16(0, addr, (HAL_READ16(0, addr) & ~mask) | (value16 & mask))

#define HAL_READ8(base, addr)            \
    (*((volatile uint8_t *)(base + addr)))

#define HAL_WRITE8(base, addr, value)  \
    ((*((volatile uint8_t *)(base + addr))) = value)

#define HAL_UPDATE8(addr, mask, value8)  \
    HAL_WRITE8(0, addr, (HAL_READ8(0, addr) & ~mask) | (value8 & mask))

#define BIT0        0x00000001
#define BIT1        0x00000002
#define BIT2        0x00000004
#define BIT3        0x00000008
#define BIT4        0x00000010
#define BIT5        0x00000020
#define BIT6        0x00000040
#define BIT7        0x00000080
#define BIT8        0x00000100
#define BIT9        0x00000200
#define BIT10       0x00000400
#define BIT11       0x00000800
#define BIT12       0x00001000
#define BIT13       0x00002000
#define BIT14       0x00004000
#define BIT15       0x00008000
#define BIT16       0x00010000
#define BIT17       0x00020000
#define BIT18       0x00040000
#define BIT19       0x00080000
#define BIT20       0x00100000
#define BIT21       0x00200000
#define BIT22       0x00400000
#define BIT23       0x00800000
#define BIT24       0x01000000
#define BIT25       0x02000000
#define BIT26       0x04000000
#define BIT27       0x08000000
#define BIT28       0x10000000
#define BIT29       0x20000000
#define BIT30       0x40000000
#define BIT31       0x80000000

#define BIT(_n)                  (uint32_t)(1U << (_n))
#define BIT_BAND(reg, bit_pos)   (*((volatile uint32_t*)(0x42000000 + ((uint32_t)&reg - 0x40000000) * 32 + bit_pos * 4)))



/* Uncomment the line below to expanse the "assert_param" macro in the
   Standard Peripheral Library drivers code */
//#define USE_FULL_ASSERT


/** @} */ /* End of group RTL876X_Exported_Macros */


/*============================================================================*
  *                                Functions
  *============================================================================*/
/** @defgroup RTL876X_Exported_Functions RTL876X Sets Exported Functions
    * @brief
    * @{
    */
#ifdef  USE_FULL_ASSERT
/**
  * @brief  The assert_param macro is used for function's parameters check.
  * @param  expr: If expr is false, it calls assert_failed function which reports
  *         the name of the source file and the source line number of the call
  *         that failed. If expr is true, it returns no value.
  * @retval None
  */
#define assert_param(expr) ((expr) ? (void)0 : io_assert_failed((uint8_t *)__FILE__, __LINE__))
void io_assert_failed(uint8_t *file, uint32_t line);
#else
#define assert_param(expr) ((void)0)
#endif /* USE_FULL_ASSERT */




/**
    * @brief    Read data from aon register
    * @param    offset: register address
    * @return   data read from register
    */
extern uint8_t btaon_fast_read(uint16_t offset);

/**
    * @brief    Read data from aon register safely
    * @param    offset: register address
    * @return   data read from register
    */
extern uint8_t btaon_fast_read_safe(uint16_t offset);

/**
    * @brief    Write data to aon register
    * @param    offset:  register address
    * @param    data:  data to be writen to register
    * @return
    */
extern void btaon_fast_write(uint16_t offset, uint8_t data);

/**
    * @brief    Write data to aon egister safely
    * @param    offset:  register address
    * @param    data:  data to be writen to register
    * @return
    */
extern void btaon_fast_write_safe(uint16_t offset, uint8_t data);

/**
    * @brief    Write data to aon egister
    * @param    offset: register address
    * @param    mask:   indicate which bit in data will be updated
    * @param    data:   data to be writen to register
    * @return
    */
extern void btaon_fast_update(uint16_t offset, uint8_t mask, uint8_t data);

/**
    * @brief    Write data to aon egister safely
    * @param    offset: register address
    * @param    mask:   indicate which bit in data will be updated
    * @param    data:   data to be writen to register
    * @return
    */
extern void btaon_fast_update_safe(uint16_t offset, uint8_t mask, uint8_t data);

/** @} */ /* End of RTL876X_Exported_Functions */


/** @} */ /* End of group RTL876X */

#ifdef __cplusplus
}
#endif
#endif  /* RTL876X_H */

