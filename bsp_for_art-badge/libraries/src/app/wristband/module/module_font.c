/**
*********************************************************************************************************
*               Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file
* @brief
* @details
* @author
* @date
* @version  v0.1
*********************************************************************************************************
*/

#include "board.h"
#include "trace.h"
#include "flash_device.h"
#include "module_font.h"
#include "app_section.h"
#include "string.h"
#include "rtl876x_gpio.h"
#include "flash_adv_cfg.h"
#include "wristband_gui.h"

#define  MAX_SECTION_NUM        3

uint8_t  section_num;
FONT_SECTION_INFO  section_info[MAX_SECTION_NUM];


/**
 * [Utf8ToUnicode description]
 * @param  utf8         [description]
 * @param  len          [description]
 * @param  unicodeArray [description]
 * @return              [description]
 */
uint32_t utf8_to_unicode(uint8_t *utf8, uint32_t len, uint16_t *unicode_array,
                         uint32_t unicode_buf_len)
{
    int k = 0;

    for (uint32_t i = 0; i < len; i++)
    {
        if (k >= unicode_buf_len)
        {
            return k;
        }
        switch (get_utf8_byte_num((uint8_t)utf8[i]))
        {
        case 0:
            {
                unicode_array[k] = *(utf8 + i);
                k++;
                break;
            }
        case 1:
            {
                //TODO
                //can not enter here
                //k++;
                break;
            }
        case 2:
            {

                //unicodeArray[k] = utf8[i]>>8;
                unicode_array[k] = utf8[i + 1];
                k++;
                i = i + 1;
                break;
            }
        case 3:
            {
                unicode_array[k] = ((utf8[i + 1] & 0x03) << 6) + (utf8[i + 2] & 0x3F);
                unicode_array[k] |= (uint16_t)(((utf8[i] & 0x0F) << 4) | ((utf8[i + 1] >> 2) & 0x0F)) << 8;
                k++;
                i = i + 2;
                break;
            }
        case 4:
            {
                //TODO
                k++;
                i = i + 3;
                break;
            }
        case 5:
            {
                //TODO
                k++;
                i = i + 4;
                break;
            }
        case 6:
            {
                //TODO
                k++;
                i = i + 5;
                break;
            }
        default:
            //DBG_DIRECT("the len is more than 6\n");
            break;
        }
    }
    return k;
}


/**
 * [GetUtf8ByteNumForWord description]
 * @param  firstCh [description]
 * @return         [description]
 */
int get_utf8_byte_num(uint8_t firstCh)
{
    uint8_t temp = 0x80;
    int num = 0;

    while (temp & firstCh)
    {
        num++;
        temp = (temp >> 1);
    }
    return num;
}



DATA_RAM_FUNCTION
uint16_t get_string_width(char *value)
{
    return 0;//howie todo
}

DATA_RAM_FUNCTION
uint8_t get_unicode_width(uint16_t code)
{
    return 0;//howie todo
}
