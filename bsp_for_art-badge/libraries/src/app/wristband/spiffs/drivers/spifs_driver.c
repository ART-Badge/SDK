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
#include "string.h"
#include "spifs_driver.h"
#include "flash_device.h"
#include "../spiffs.h"


#define WRISTBAND_FATFS_ADDR            0x01F80000
#define WRISTBAND_FATFS_SIZE            0x00040000 //TOTAL 256k
#define FLASH_SECTOR_SIZE               4096

#define  SPIFLASH_CFG_PHYS_SZ               (1024*256)
#define  SPIFLASH_CFG_PHYS_ERASE_SZ         (1024 *4)
#define  SPIFLASH_CFG_PHYS_ADDR             (0)
#define  SPIFLASH_CFG_LOG_PAGE_SZ           (256)
#define  SPIFLASH_CFG_LOG_BLOCK_SZ          (1024 * 4)



s32_t rtk_flash_read(u32_t addr, u32_t size, u8_t *dst)
{
    memcpy(dst, (uint8_t *)(WRISTBAND_FATFS_ADDR + addr), size);
    return 0;
}

s32_t rtk_flash_write(u32_t addr, u32_t size, u8_t *src)
{
    flash_write_locked(WRISTBAND_FATFS_ADDR + addr, size, src);
    return 0;
}

s32_t rtk_flash_erase(u32_t addr, u32_t size)
{
    uint32_t count = size / FLASH_SECTOR_SIZE;

    for (uint32_t i = 0; i < count; i++)
    {
        if (flash_erase_locked(FLASH_ERASE_SECTOR, WRISTBAND_FATFS_ADDR + FLASH_SECTOR_SIZE * i) == false)
        {
            APP_PRINT_ERROR0("fs data erase error!");
        }
    }
    return 0;
}

#if 0
spiffs SPIFlashFS;
static u8_t FS_Work_Buf[SPIFLASH_CFG_LOG_PAGE_SZ * 2];
static u8_t FS_FDS[32 * 4];
static u8_t FS_Cache_Buf[(SPIFLASH_CFG_LOG_PAGE_SZ + 32) * 4];
char const WriteBuf[] = {"Hi,Budy! if you get this Message......Congratulations!You have succeeded!!"};
char ReadBuf[80];

void spifs_test_code(void)
{
    spiffs_config SPIFlashCfg;
    SPIFlashCfg.phys_size        = SPIFLASH_CFG_PHYS_SZ;
    SPIFlashCfg.phys_addr        = SPIFLASH_CFG_PHYS_ADDR;
    SPIFlashCfg.phys_erase_block = SPIFLASH_CFG_PHYS_ERASE_SZ;
    SPIFlashCfg.log_block_size   = SPIFLASH_CFG_LOG_BLOCK_SZ;
    SPIFlashCfg.log_page_size    = SPIFLASH_CFG_LOG_PAGE_SZ;

    SPIFlashCfg.hal_read_f  = rtk_flash_read;
    SPIFlashCfg.hal_write_f = rtk_flash_write;
    SPIFlashCfg.hal_erase_f = rtk_flash_erase;
    int res = SPIFFS_mount(&SPIFlashFS,
                           &SPIFlashCfg,
                           FS_Work_Buf,
                           FS_FDS,
                           sizeof(FS_FDS),
                           FS_Cache_Buf,
                           sizeof(FS_Cache_Buf),
                           0);
    spiffs_file fd = SPIFFS_open(&SPIFlashFS, "my_file", SPIFFS_CREAT | SPIFFS_TRUNC | SPIFFS_RDWR, 0);

    if (SPIFFS_write(&SPIFlashFS, fd, WriteBuf, sizeof(WriteBuf)) < 0)
    {
        APP_PRINT_INFO1("errno %d\n", SPIFFS_errno(&SPIFlashFS));
    }
    SPIFFS_close(&SPIFlashFS, fd);
    fd = SPIFFS_open(&SPIFlashFS, "my_file", SPIFFS_RDWR, 0);
    if (SPIFFS_read(&SPIFlashFS, fd, ReadBuf, sizeof(WriteBuf)) < 0)
    {
        APP_PRINT_INFO1("errno %d\n", SPIFFS_errno(&SPIFlashFS));
    }
    SPIFFS_close(&SPIFlashFS, fd);
    APP_PRINT_INFO1("%s\n", TRACE_STRING(ReadBuf));
}
#endif





