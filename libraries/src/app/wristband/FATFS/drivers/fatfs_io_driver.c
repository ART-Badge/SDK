enum { __FILE_NUM__ = 0 };

/*********************************************************************************************************
*               Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     fatfs_io_driver.c
* @brief
* @details
* @author
* @date
* @version  v0.1
*********************************************************************************************************
*/

#include "board.h"
#include "trace.h"
#include "diskio.h"     /* FatFs lower layer API */
#include "string.h"
#include "fatfs_io_driver.h"
#include "ffconf.h"
#include "flash_device.h"
#include "ff.h"


#define WRISTBAND_FATFS_ADDR            0x01840000
#define WRISTBAND_FATFS_SIZE            0x00040000 //TOTAL 256k
#define FLASH_SECTOR_SIZE               4096

DSTATUS internal_flash_init(void)
{
    return RES_OK;
}


DRESULT internal_falsh_write(
    const BYTE *buff,   /* Data to be written */
    DWORD sector,       /* Start sector in LBA */
    UINT count          /* Number of sectors to write */
)
{
    uint8_t *write = (uint8_t *)buff;

    for (uint32_t i = 0; i < count; i++)
    {
        if (flash_erase_locked(FLASH_ERASE_SECTOR,
                               WRISTBAND_FATFS_ADDR + sector * FLASH_SECTOR_SIZE +  FLASH_SECTOR_SIZE * i) == false)
        {
            APP_PRINT_ERROR0("fs data erase error!");
            return RES_ERROR;
        }
    }
    if (flash_write_locked(WRISTBAND_FATFS_ADDR + sector * FLASH_SECTOR_SIZE, count * FLASH_SECTOR_SIZE,
                           write) == false)
    {
        APP_PRINT_ERROR0("fs data write error!");
        return RES_ERROR;
    }
    else
    {
        return RES_OK;
    }

}

DRESULT internal_falsh_read(
    BYTE *buff,     /* Data buffer to store read data */
    DWORD sector,   /* Sector address (LBA) */
    UINT count      /* Number of sectors to read (1..128) */
)
{
    memcpy(buff, (uint8_t *)(WRISTBAND_FATFS_ADDR + sector * FLASH_SECTOR_SIZE),
           count * FLASH_SECTOR_SIZE);
    return RES_OK;
}

void fasfs_test_code(void)
{
    //res = f_mkfs("0:", FM_ANY, 0, work, sizeof work);
}





