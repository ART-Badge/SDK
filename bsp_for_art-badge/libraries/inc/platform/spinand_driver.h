#ifndef _SPINAND_DRIVER_H_
#define _SPINAND_DRIVER_H_

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
Support SPI NAND Devices:
    W25M02GV (Stack two W25N01GV die)
*/


/*
support 4 bit user mode read/write

supported api:
select die: die id 0x00, 0x01 --> flash_cmd_tx
get rdid
read/write status register
read/write flash
read/write otp

SR registers:
    write enabled or disabled
    state of write protection
    read modes
    protection register/otp area lock status
    erase/program result
    ECC usage/status

Protection Register/Status Register-1

*/

enum w25xxx_op
{
    W25XXX_OP_RST = 0xff,
    W25MXX_OP_DIE_SELECT = 0xc2,
    W25XXX_OP_READ_JEDEC_ID = 0x9f,
    W25XXX_OP_READ_REG = 0x0F,
    W25XXX_OP_WRITE_REG = 0x1F,
    W25XXX_OP_WRITE_ENABLE = 0x06,
    W25XXX_OP_WRITE_DISABLE = 0x04,
    W25XXX_OP_BBM_SWAP_BLOCKS = 0xa1,
    W25XXX_OP_BBM_READ_LUT = 0xa5,
    W25XXX_OP_BBM_READ_LAST_ECC_FAIL_ADDR = 0xa9,
    W25XXX_OP_PROG_DATA_LOAD = 0x02,
    W25XXX_OP_PROG_RAND_DATA_LOAD = 0x84,
    W25XXX_OP_PROG_QUAD_DATA_LOAD = 0x32,
    W25XXX_OP_PROG_RAND_QUAD_DATA_LOAD = 0x34,
    W25XXX_OP_PROG_EXECUTE = 0x10,
    W25XXX_OP_BLOCK_ERASE = 0xd8,
    W25XXX_OP_PAGE_DATA_READ = 0x13,
    W25XXX_OP_READ = 0x03,
    W25XXX_OP_QUAD_READ = 0xEB,
};

enum w25xxx_reg
{
    W25XXX_REG_PROT = 0xa0, /* Protection register */
    W25XXX_REG_CONF = 0xb0, /* Configuration register */
    W25XXX_REG_STAT = 0xc0, /* Status register */
};

#define W25XXX_REG_PROT_BP0   (1 << 3)
#define W25XXX_REG_PROT_BP1   (1 << 4)
#define W25XXX_REG_PROT_BP2   (1 << 5)
#define W25XXX_REG_PROT_BP3   (1 << 6)
#define W25XXX_REG_PROT_BP    (W25XXX_REG_PROT_BP0 | W25XXX_REG_PROT_BP1 | W25XXX_REG_PROT_BP2 | W25XXX_REG_PROT_BP3)

#define W25XXX_REG_CONF_BUF   (1 << 3)
#define W25XXX_REG_CONF_ECCE  (1 << 4)
#define W25XXX_REG_CONF_OTPE  (1 << 6)

#define W25XXX_REG_STAT_BUSY  (1 << 0)
#define W25XXX_REG_STAT_WEL   (1 << 1)
#define W25XXX_REG_STAT_EFAIL (1 << 2)
#define W25XXX_REG_STAT_PFAIL (1 << 3)
#define W25XXX_REG_STAT_ECC0  (1 << 4)
#define W25XXX_REG_STAT_ECC1  (1 << 5)
#define W25XXX_REG_STAT_LUTF  (1 << 6)

#define W25XXX_MAX_NUM_DIES   2
#define W25XXX_BB_LUT_SIZE    20

#define W25XXX_PAGE_SIZE      2048U
#define W25XXX_BLOCK_SIZE     (64 * W25XXX_PAGE_SIZE)
#define W25XXX_DIE_SIZE       (1024 * W25XXX_BLOCK_SIZE)

#define PARAM_PAGE_SIGNATURE  0x49464E4FU /* ONFI, the word can be used as calibration value */


void spinand_init(void);

uint32_t spinand_get_rdid(void);

bool spinand_block_erase(uint32_t off, uint32_t len);

bool spinand_read(uint32_t start_addr, uint32_t data_len, uint8_t *data);

bool spinand_write(uint32_t start_addr, uint32_t data_len, uint8_t *data);

bool nand_cmd_tx(uint8_t cmd, uint8_t data_len, uint8_t *data_buf);

bool nand_read_sr(uint8_t sr_addr, uint32_t data_len, uint8_t *data);

uint8_t spinand_read_reg(uint8_t reg_addr);

bool nand_read_internal(uint16_t start_addr, uint16_t data_len, uint8_t *data);

bool spinand_page_data_read(uint16_t page_num);

bool spinand_write_enable(void);

bool spinand_pgm_data_load(uint16_t page_off, uint32_t data_len, uint8_t *data);

void spinand_test(void);

#ifdef __cplusplus
}
#endif

#endif /* _SPINAND_DRIVER_H_ */
