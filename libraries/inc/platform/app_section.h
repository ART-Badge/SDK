/**
*****************************************************************************************
*     Copyright(c) 2017, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file    app_section.h
  * @brief
  * @author
  * @date    2017.6.7
  * @version v1.0
   **************************************************************************************
   * @attention
   * <h2><center>&copy; COPYRIGHT 2017 Realtek Semiconductor Corporation</center></h2>
   * *************************************************************************************
  */

/*============================================================================*
 *                      Define to prevent recursive inclusion
 *============================================================================*/
#ifndef _APP_SECTION_H_
#define _APP_SECTION_H_

/** @defgroup APP_SECTION APP Section
  * @brief memory section definition for user application.
  * @{
  */
#if defined (__ICCARM__)
#define UPPERSTACK_FLASH @ ".upperstack.flash"
#define APP_FLASH_HEADER                @ ".app.flash.header"
#define APP_FLASH_HEADER_EXT            @ ".app.flash.header_ext"

#define APP_FLASH_TEXT_SECTION          /* default flash code */

/* ram code */
#define RAM_DATAON_DATA_SECTION         @ ".ram.dataon.data"
#define DATA_RAM_FUNCTION               @ ".app.data_ram.text" /*!< data ram code */

/* overlay section, only support three overlay sections now. */
#define OVERLAY_SECTION_BOOT_ONCE       /* flash code */
#define APP_ENCRYPTION_TEXT_SECTION     @ ".app.encryption.text"
#else
#define UPPERSTACK_FLASH  __attribute__((at(UPPERSTACK_FLASH_ADDR))) __attribute__((section(".upperstack.flash"))) __attribute__((used))

/* app encryption */
#define APP_ENCRYPTION_SIGNATURE   __attribute__((section(".encryption.signature")))
#define APP_ENCRYPION_DUMMY_ALIGN  __attribute__((section(".encryption.dummy.align"))) __attribute__((aligned(16)))
/* app encryption code*/
#define APP_ENCRYPTION_TEXT_SECTION __attribute__((section(".app.encryption.text")))

/*flash const data or flash function */
#define APP_FLASH_HEADER     __attribute__((section(".app.flash.header")))     __attribute__((used))
#define APP_FLASH_HEADER_AUTH     __attribute__((section(".app.flash.header.auth")))     __attribute__((used))
#define APP_FLASH_HEADER_EXT __attribute__((section(".app.flash.header_ext"))) __attribute__((used))
/* flash code */
#define APP_FLASH_TEXT_SECTION     __attribute__((section(".app.flash.text")))
/* flash ro data */
#define APP_FLASH_RODATA_SECTION   __attribute__((section(".app.flash.rodata")))
/* ram code */
#define DATA_RAM_FUNCTION               __attribute__((section(".app.data_ram.text"))) /*!< data ram code */
#define SHARE_CACHE_RAM_SECTION          __attribute__((section(".app.sharecache_ram.text")))  /*!< share cache ram code or data*/

/*  global variable or ram function,  data on (default) */
#define RAM_DATAON_DATA_SECTION    __attribute__((section(".ram.dataon.data")))  /*!< DATA ON */
#define RAM_DATAON_BSS_SECTION     __attribute__((section(".ram.dataon.bss")))  /*!< DATA ON */
#define RAM_DATAON_UNINIT_SECTION  __attribute__((section(".uninit.ram")))  /*!< DATA ON */

/* buffer on */
#define RAM_BUFFERON_DATA_SECTION  __attribute__((section(".ram.bufferon.data")))  /*!< BUFFER ON */
#define RAM_BUFFERON_BSS_SECTION   __attribute__((section(".ram.bufferon.bss")))   /*!< BUFFER ON */


/* overlay section, only support three overlay sections now. */
#define OVERLAY_SECTION_BOOT_ONCE  __attribute__((section(".app.overlay_a")))   /*!< overlay a section */
#define OVERLAY_B_SECTION          __attribute__((section(".app.overlay_b")))   /*!< overlay b section */
#define OVERLAY_C_SECTION          __attribute__((section(".app.overlay_c")))   /*!< overlay c section */




/** @} */ /* End of group APP_SECTION */
#endif
#endif /* _APP_SECTION_H_ */
