/**
*********************************************************************************************************
*               Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* \file     rtl876x_spic.h
* \brief    The header file of the peripheral SPI driver.
* \details  This file provides all SPI firmware functions.
* \author   elliot chen
* \date     2015-5-6
* \version  v1.0
* *********************************************************************************************************
*/

#ifndef _RTL876X_SPIC_H_
#define _RTL876X_SPIC_H_

#ifdef __cplusplus
extern "C" {
#endif


/**
 * \addtogroup  IO          Peripheral Drivers
 * \defgroup    SPIC         SPIC
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
 * \defgroup    SPIC_Exported_Types Init Params Struct
 *
 * \ingroup     SPIC
 */

/**
 * \brief       SPIC init structure definition.
 *
 * \ingroup     SPIC_Exported_Types
 */

typedef struct
{
    uint16_t SPIC_Direction;
    uint16_t SPIC_Mode;
    uint32_t SPIC_BaudRatePrescaler;

} SPIC_InitTypeDef;


/*****************************************************************************************
* Registers Definitions --------------------------------------------------------*
********************  Bits definition for SPIC_CTRLR0 register  *******************/
#define BIT_CK_MTIMES(x)        (((x) & 0x0000001F) << 23)
#define BIT_FAST_RD(x)          (((x) & 0x00000001) << 22)
#define BIT_CMD_CH(x)           (((x) & 0x00000003) << 20)
#define BIT_DATA_CH(x)          (((x) & 0x00000003) << 18)
#define BIT_ADDR_CH(x)          (((x) & 0x00000003) << 16)
#define BIT_TMOD(x)             (((x) & 0x00000003) << 8)
#define BIT_SCPOL               (0x00000001 << 7)
#define BIT_SCPH                (0x00000001 << 6)

/********************  Bits definition for SPIC_CTRLR1 register  *******************/
#define BIT_NDF(x)                  ((x) & 0xfffff)

/********************  Bits definition for SPIC_SSIENR register  *******************/
#define BIT_ATCK_CMD                (0x00000001 << 1)
#define BIT_SPIC_EN             (0x00000001)

/********************  Bits definition for SPIC_BAUDR register  *******************/
#define BIT_SCKDV(x)                ((x) & 0x0fff)

/********************  Bits definition for SPIC_SR register  *******************/
#define BIT_TXE                 (0x00000001 << 5)
#define BIT_RFF                 (0x00000001 << 4)
#define BIT_RFNE                (0x00000001 << 3)
#define BIT_TFE                 (0x00000001 << 2)
#define BIT_TFNF                    (0x00000001 << 1)
#define BIT_BUSY                    (0x00000001)

/********************  Bits definition for SPIC_IMR register  *******************/
#define BIT_ACSIM                               (0x00000001 << 11)
#define BIT_RXSIM                               (0x00000001 << 10)
#define BIT_TXSIM                               (0x00000001 << 9)
#define BIT_ACEIM                               (0x00000001 << 8)
#define BIT_BYEIM                               (0x00000001 << 7)
#define BIT_WBEIM                               (0x00000001 << 6)
#define BIT_FSEIM                               (0x00000001 << 5)
#define BIT_RXFIM                               (0x00000001 << 4)
#define BIT_RXOIM                               (0x00000001 << 3)
#define BIT_RXUIM                               (0x00000001 << 2)
#define BIT_TXOIM                               (0x00000001 << 1)
#define BIT_TXEIM                               (0x00000001)

/********************  Bits definition for SPIC_ISR register  *******************/
#define BIT_ACSIS                               (0x00000001 << 11)
#define BIT_RXSIS                               (0x00000001 << 10)
#define BIT_TXSIS                               (0x00000001 << 9)
#define BIT_ACEIS                               (0x00000001 << 8)
#define BIT_BYEIS                               (0x00000001 << 7)
#define BIT_WBEIS                               (0x00000001 << 6)
#define BIT_FSEIS                               (0x00000001 << 5)
#define BIT_RXFIS                               (0x00000001 << 4)
#define BIT_RXOIS                               (0x00000001 << 3)
#define BIT_RXUIS                               (0x00000001 << 2)
#define BIT_TXOIS                               (0x00000001 << 1)
#define BIT_TXEIS                               (0x00000001)

/********************  Bits definition for SPIC_RISR register  *******************/
#define BIT_ACEIR                               (0x00000001 << 8)
#define BIT_BYEIR                               (0x00000001 << 7)
#define BIT_WBEIR                               (0x00000001 << 6)
#define BIT_FSEIR                               (0x00000001 << 5)
#define BIT_RXFIR                               (0x00000001 << 4)
#define BIT_RXOIR                               (0x00000001 << 3)
#define BIT_RXUIR                               (0x00000001 << 2)
#define BIT_TXOIR                               (0x00000001 << 1)
#define BIT_TXEIR                               (0x00000001)

/********************  Bits definition for SPIC_CTRLR2 register  *******************/
#define BIT_RX_FIFO_ENTRY(x)           (((x) & 0x0000000f) << 8)
#define BIT_FIFO_ENTRY(x)           (((x) & 0x0000000f) << 4)
#define BIT_SEQ_EN              (0x00000001 << 3)
/* Indicate the WPn input pin of SPI Flash is connected to:
   0(default): WP=spi_sout[2], 1:WP=spi_sout[3]. */
#define BIT_WPN_DNUM            (0x00000001 << 2)
/*write protect function.*/
#define BIT_WPN_SET             (0x00000001 << 1)
/* SO pin of SPI Flash.0: SO connects to spi_sout[0]. 1(default): SO connects to spi_sout[1].*/
#define BIT_SO_DUM              (0x00000001)

/********************  Bits definition for SPIC_ADDR_LENGTH register  *******************/
#define BIT_ADDR_PHASE_LENGTH(x)    ((x) & 0x00000003)

/********************  Bits definition for SPIC_AUTO_LENGTH register  *******************/
#define BIT_CS_H_WR_DUM_LEN(x)  (((x) & 0x0000000f) << 28)
#define BIT_CS_H_RD_DUM_LEN(x)  (((x) & 0x00000003) << 26)
#define BIT_AUTO_DUM_LEN(x)     (((x) & 0x000000ff) << 18)
#define BIT_AUTO_ADDR_LENGTH(x) (((x) & 0x00000003) << 16)
#define BIT_IN_PHYSICAL_CYC(x)  (((x) & 0x0000000f) << 12)
#define BIT_RD_DUMMY_LENGTH(x)  (((x) & 0x00000fff))

/********************  Bits definition for SPIC_VALID_CMD register  *******************/
#define BIT_SEQ_TRANS_EN        (0x00000001 << 14)
#define BIT_CTRLR0_CH           (0x00000001 << 12)
#define BIT_PRM_EN              (0x00000001 << 11)
#define BIT_WR_AUTOCHECKSTATUS  (0x00000001 << 10)
#define BIT_WR_BLOCKING         (0x00000001 << 9)
#define BIT_WR_QUAD_II          (0x00000001 << 8)
#define BIT_WR_QUAD_I           (0x00000001 << 7)
#define BIT_WR_DUAL_II          (0x00000001 << 6)
#define BIT_WR_DUAL_I           (0x00000001 << 5)
#define BIT_RD_QUAD_IO          (0x00000001 << 4)
#define BIT_RD_QUAD_O           (0x00000001 << 3)
#define BIT_RD_DUAL_IO          (0x00000001 << 2)
#define BIT_RD_DUAL_I           (0x00000001 << 1)
#define BIT_FRD_SINGEL          (0x00000001)


#define SPIC_FIFO_SIZE              64

typedef enum
{
    SPIC_MODE_1BIT         = 0,
    SPIC_MODE_2BIT         = 1,
    SPIC_MODE_4BIT         = 2
} T_SPIC_BIT_MODE;

/** \} */ /* End of group SPIC_Exported_Functions */

#ifdef __cplusplus
}
#endif

#endif /* _RTL876X_SPIC_H_ */


/******************* (C) COPYRIGHT 2015 Realtek Semiconductor Corporation *****END OF FILE****/

