/**
*****************************************************************************************
*     Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file
  * @brief
  * @details
  * @author
  * @date      2015-11-28
  * @version  v0.1
  * *************************************************************************************
  */

/* Define to prevent recursive inclusion */
#ifndef _RTL876X_MODULE_FONT_H_
#define _RTL876X_MODULE_FONT_H_

#ifdef  __cplusplus
extern "C" {
#endif      /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "rtl876x.h"
#include "mem_config.h"

#define  FONT_HEAD_SIZE     0
#define  FONT_IMG_ADDR      (0x1000400)

typedef struct
{
    uint16_t first_char;
    uint16_t last_char;
    uint32_t addr_offset;
} FONT_SECTION_INFO;

typedef struct
{
    uint32_t addr_offset : 26;
    uint32_t width : 6;
} FONT_CHAR_INDEX;


int get_utf8_byte_num(uint8_t firstCh);
uint32_t utf8_to_unicode(uint8_t *utf8, uint32_t len, uint16_t *unicode_array,
                         uint32_t unicode_buf_len);

uint16_t get_string_width(char *value);
uint8_t get_unicode_width(uint16_t code);




#ifdef  __cplusplus
}
#endif      /*  __cplusplus */


#endif
