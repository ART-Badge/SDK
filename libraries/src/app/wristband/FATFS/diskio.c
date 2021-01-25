/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2016        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "diskio.h"     /* FatFs lower layer API */
#include "drivers/fatfs_io_driver.h"

/* Definitions of physical drive number for each drive */
#define DEV_RAM     0   /* Example: Map Ramdisk to physical drive 0 */
#define DEV_MMC     1   /* Example: Map MMC/SD card to physical drive 1 */
#define DEV_USB     2   /* Example: Map USB MSD to physical drive 2 */
#define DEV_RTL8762CK_SPIC_FLASH        3   /* for realtek wristband */


/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status(
    BYTE pdrv       /* Physical drive nmuber to identify the drive */
)
{
    return RES_OK;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize(
    BYTE pdrv               /* Physical drive nmuber to identify the drive */
)
{
    return RES_OK;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read(
    BYTE pdrv,      /* Physical drive nmuber to identify the drive */
    BYTE *buff,     /* Data buffer to store read data */
    DWORD sector,   /* Start sector in LBA */
    UINT count      /* Number of sectors to read */
)
{
    DRESULT res;

    switch (pdrv)
    {
    case DEV_RAM :
        // translate the arguments here

        res = RES_PARERR;

        // translate the reslut code here

        return res;

    case DEV_MMC :
        // translate the arguments here

        res = RES_PARERR;

        // translate the reslut code here

        return res;

    case DEV_USB :
        // translate the arguments here

        res = RES_PARERR;

        // translate the reslut code here

        return res;
    case DEV_RTL8762CK_SPIC_FLASH :
        // translate the arguments here

        res = internal_falsh_read(buff, sector, count);

        // translate the reslut code here

        return res;
    }

    return RES_PARERR;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

DRESULT disk_write(
    BYTE pdrv,          /* Physical drive nmuber to identify the drive */
    const BYTE *buff,   /* Data to be written */
    DWORD sector,       /* Start sector in LBA */
    UINT count          /* Number of sectors to write */
)
{
    DRESULT res;

    switch (pdrv)
    {
    case DEV_RAM :
        // translate the arguments here

        res = RES_PARERR;

        // translate the reslut code here

        return res;

    case DEV_MMC :
        // translate the arguments here

        res = RES_PARERR;

        // translate the reslut code here

        return res;

    case DEV_USB :
        // translate the arguments here

        res = RES_PARERR;

        // translate the reslut code here

        return res;
    case DEV_RTL8762CK_SPIC_FLASH :
        // translate the arguments here

        res = internal_falsh_write(buff, sector, count);

        // translate the reslut code here

        return res;
    }

    return RES_PARERR;
}



/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl(
    BYTE pdrv,      /* Physical drive nmuber (0..) */
    BYTE cmd,       /* Control code */
    void *buff      /* Buffer to send/receive control data */
)
{
    DRESULT res;

    switch (pdrv)
    {
    case DEV_RAM :
    case DEV_MMC :
    case DEV_USB :
        res = RES_PARERR;
        return res;
    case DEV_RTL8762CK_SPIC_FLASH :
        switch (cmd)
        {
        case CTRL_SYNC:
            {
                return RES_OK;
            }
        case GET_SECTOR_COUNT:
            {
                *(WORD *)buff = 64;  //256K have 64 4K
                return RES_OK;
            }
        case GET_SECTOR_SIZE:
            {
                *(WORD *)buff = 4096;
                return RES_OK;
            }
        case GET_BLOCK_SIZE:
            {
                *(WORD *)buff = 1;
                return RES_OK;
            }
        case CTRL_TRIM:
            {
                return RES_PARERR; // no define this option
            }
        default:
            {
                return RES_PARERR;
            }
        }
    }

    return RES_PARERR;
}

DWORD get_fattime(void)
{
    return 0;
}

