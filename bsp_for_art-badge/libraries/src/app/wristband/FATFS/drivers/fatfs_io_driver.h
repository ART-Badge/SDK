#ifndef __FATFS_IO_DRIVER_H__
#define __FATFS_IO_DRIVER_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "diskio.h"

DRESULT internal_falsh_write(
    const BYTE *buff,   /* Data to be written */
    DWORD sector,       /* Start sector in LBA */
    UINT count          /* Number of sectors to write */
);

DRESULT internal_falsh_read(
    BYTE *buff,     /* Data buffer to store read data */
    DWORD sector,   /* Sector address (LBA) */
    UINT count      /* Number of sectors to read (1..128) */
);


#ifdef __cplusplus
}
#endif

#endif
