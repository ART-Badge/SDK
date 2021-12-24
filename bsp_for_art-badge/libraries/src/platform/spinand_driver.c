#include <stddef.h>
#include <string.h>
#include "spinand_driver.h"
#include "rtl876x.h"
#include "rtl876x_spic.h"
#include "flash_device.h"
#include "trace.h"
#include "platform_utils.h"

/** @defgroup FLASH_DRIVER_Registers_Definitions Flash Driver Registers Definitions
  * @brief
  * @{
  */


/** End of FLASH_DRIVER_Registers_Definitions
  * @}
  */

typedef enum
{
    CHANN_SINGLE = 0,
    CHANN_DUAL = 1,
    CHANN_QUAD = 2
} T_CHANN_TYPE;

typedef struct spidnand_dev
{
    uint32_t num_dies;           /* Number of dies, W25M02GV contains two dies of W25N01GV. */
    uint8_t  curr_rd_cmd;        /* current used read cmd */
    uint8_t  curr_wr_cmd;        /* current used write cmd */
    uint8_t  rd_data_ch;         /* read data channel */
    uint8_t  rd_addr_ch;         /* read address channel */
    uint8_t  rd_st_dummy_cycle;  /* read status dummy cycle */
    uint8_t  wr_addr_ch;         /* write address channel */
    uint8_t  wr_addr_byte;       /* address byte */
    uint8_t  wr_data_ch;         /* write data channel */
    uint8_t  bit_mode;           /* bit mode */
    uint8_t  curr_die;           /* only NAND with multiple dies use this field */
} spinand_dev_t;

static spinand_dev_t nand =
{
    .num_dies = 0,
    .curr_rd_cmd = W25XXX_OP_READ,
    .curr_wr_cmd = W25XXX_OP_PROG_RAND_DATA_LOAD,
    .rd_data_ch  = CHANN_SINGLE,
    .rd_addr_ch  = CHANN_SINGLE,
    .rd_st_dummy_cycle = 2,
    .bit_mode    = 0,
    .curr_die    = 0,
};


#define SPIC_PROGRAM_PAGE_SIZE 256

/**
 * @brief set addr channel and data channel in ctrl0 register.
 *
 * @return
*/
__STATIC_INLINE void spic_set_multi_ch(SPIC_TypeDef *spic, uint32_t data_ch, uint32_t addr_ch)
{
    spic->ctrlr0 &= ~(BIT_CMD_CH(3) | BIT_ADDR_CH(3) | BIT_DATA_CH(3));

    spic->ctrlr0 |= (BIT_ADDR_CH(addr_ch) | BIT_DATA_CH(data_ch));
}

void spic_enable(SPIC_TypeDef *spic, uint32_t enable);

/**
 * @brief set dummy_cycle in autolen register[11:0].
 *
 * @return
*/
static void spic_set_rd_dummy_length(SPIC_TypeDef *spic, uint8_t dummy_len)
{
    uint32_t data = BIT_RD_DUMMY_LENGTH(dummy_len);
    HAL_UPDATE32(&spic->auto_length, 0xfff, data);
}

/**
 * @brief clr addr channel and data channel in ctrl0 register.
 *
 * @return
*/
__STATIC_INLINE void spic_clr_multi_ch(SPIC_TypeDef *spic)
{
    spic->ctrlr0 &= ~(BIT_CMD_CH(3) | BIT_ADDR_CH(3) | BIT_DATA_CH(3));
}

/**
 * @brief set RX mode in ctrl0 register in user mode.
 *
 * @return
*/
__STATIC_INLINE void spic_set_rx_mode(SPIC_TypeDef *spic)
{
    spic->ctrlr0 |= BIT_TMOD(3);
}

/**
 * @brief set TX mode in ctrl0 register in user mode.
 *
 * @return
*/
__STATIC_INLINE void spic_set_tx_mode(SPIC_TypeDef *spic)
{
    spic->ctrlr0 &= ~(BIT_TMOD(3));
}

/**
 * @brief wait until busy states of SR register is cleared.
 *
 * @return true if success
*/
__STATIC_INLINE bool spic_wait_busy(SPIC_TypeDef *spic)
{

//    if (otp.flash_setting.parameters.delay_us_before_wait_busy)
//    {
//        platform_delay_us(otp.flash_setting.parameters.delay_us_before_wait_busy);
//    }

    //break only when tx error or busy state is cleared
//    do
//    {
//        if (ctr++ >= otp.flash_cfg.spic_wait_max)
//        {
//            otp.flash_cfg.spic_wait_max = FLASH_TABLE_MAGIC_PATTERN;   // used to notify exceed max
//            return false;
//        }
//    }
    while ((spic->sr & BIT_BUSY) && (!(spic->sr & BIT_TXE)));

//    if (otp.flash_setting.parameters.delay_us_after_wait_busy)
//    {
//        platform_delay_us(otp.flash_setting.parameters.delay_us_after_wait_busy);
//    }

    return true;
}

/* some command has input parameters */
bool nand_cmd_rx(uint8_t cmd, uint8_t tx_len, uint8_t *tx_buf, uint8_t read_len, uint8_t *read_buf)
{
    bool retval = true;

    uint32_t autolen = SPIC2->auto_length;
    uint32_t ctrlr0 = SPIC2->ctrlr0;
    uint32_t ctrlr1 = SPIC2->ctrlr1;
    spic_enable(SPIC2, DISABLE);
    spic_clr_multi_ch(SPIC2);
    spic_set_rx_mode(SPIC2);
    SPIC2->ctrlr1 = read_len;
    SPIC2->dr[0].byte = cmd;

    while (tx_len--)
    {
        SPIC2->dr[0].byte = *tx_buf++;
    }

    spic_set_rd_dummy_length(SPIC2, nand.rd_st_dummy_cycle);

    spic_enable(SPIC2, ENABLE);
    if (!spic_wait_busy(SPIC2))
    {
        retval = false;
    }

    spic_enable(SPIC2, DISABLE);

    if (retval == true)
    {
        while (read_len--)
        {
            *read_buf++ = SPIC2->dr[0].byte;
        }
    }

    SPIC2->auto_length = autolen;
    SPIC2->ctrlr0 = ctrlr0;
    SPIC2->ctrlr1 = ctrlr1;
    return retval;
}

bool nand_cmd_tx(uint8_t cmd, uint8_t data_len, uint8_t *data_buf)
{
    bool retval = true;

    uint32_t ctrlr0 = SPIC2->ctrlr0;
    uint32_t addr_len = SPIC2->addr_length;

    spic_enable(SPIC2, DISABLE);
    spic_clr_multi_ch(SPIC2);
    spic_set_tx_mode(SPIC2);

    SPIC2->addr_length = data_len;

    SPIC2->dr[0].byte = cmd;

    while (data_len--)
    {
        SPIC2->dr[0].byte = *data_buf++;
    }

    spic_enable(SPIC2, ENABLE);

    if (!spic_wait_busy(SPIC2))
    {
        retval = false;
    }

    spic_enable(SPIC2, DISABLE);

    //restore ctrl0 and addr_len register
    SPIC2->ctrlr0 = ctrlr0;
    SPIC2->addr_length = addr_len;

    return retval;
}

/* read status register */
static bool nand_read_sr(uint8_t sr_addr, uint32_t data_len, uint8_t *data)
{
    bool retval = true;
    uint8_t cmd = W25XXX_OP_READ_REG;
    uint8_t rd_num = 0;

    if (data_len > SPIC_FIFO_SIZE - 4) // 4 byte is for command and addr
    {
        return false;
    }

    uint32_t autolen = SPIC2->auto_length;
    spic_enable(SPIC2, DISABLE);

    /* prepare */
    spic_clr_multi_ch(SPIC2);
    spic_set_rx_mode(SPIC2);
    SPIC2->ctrlr1 = data_len; //spic_set_ctrl1(data_len); /* only read a status register once */
    SPIC2->addr_length = BIT_ADDR_PHASE_LENGTH(1); /* status register address length */
    spic_set_rd_dummy_length(SPIC2, nand.rd_st_dummy_cycle);

    /* push cmd and its parameters */
    SPIC2->dr[0].byte = cmd;
    SPIC2->dr[0].byte = sr_addr;

    /* enable */
    spic_enable(SPIC2, ENABLE);
    if (!spic_wait_busy(SPIC2))
    {
        retval = false;
    }
    spic_enable(SPIC2, DISABLE);

    /* read out data */
    if (retval == true)
    {
        while (rd_num < data_len)
        {
            if (SPIC2->sr & BIT_RFNE)
            {
                data[rd_num++] = SPIC2->dr[0].byte;
            }
        }
    }

    SPIC2->auto_length = autolen;
    return retval;
}

static void spic_set_seq_enable(uint8_t enable)
{
    if (enable)
    {
        SPIC2->imr |= BIT_RXSIM;
        SPIC2->ctrlr2 |= BIT_SEQ_EN;
    }
    else
    {
        SPIC2->ctrlr2 &= ~ BIT_SEQ_EN;
        SPIC2->imr &= ~ BIT_RXSIM;
    }
}

bool nand_read_internal(uint16_t start_addr, uint16_t data_len, uint8_t *data)
{
    bool retval = true;
    uint8_t cmd = nand.curr_rd_cmd, split_intr = 0, param_len;

    uint32_t next_addr = start_addr, i = 0;
    uint32_t rd_num = 0, rxflr = 0, split_ctr = 0;
    volatile uint32_t ser;

    spic_enable(SPIC2, DISABLE);

    spic_set_multi_ch(SPIC2, nand.rd_data_ch, nand.rd_addr_ch);
    spic_set_rx_mode(SPIC2);
    //spic_set_ctrl1(data_len);
    SPIC2->ctrlr1 = data_len;
    spic_set_seq_enable(1);
    ser = SPIC2->ser;

    if (cmd == W25XXX_OP_READ)
    {
        SPIC2->addr_length = BIT_ADDR_PHASE_LENGTH(3);
        param_len = 4;
    }
    else
    {
        SPIC2->addr_length = BIT_ADDR_PHASE_LENGTH(2);
        param_len = 3;
    }

    uint8_t cmd_param[4];
    cmd_param[0] = cmd;
//    cmd_param[1] = (start_addr & 0xFF00) >> 8;
//    cmd_param[2] = start_addr & 0x00FF;
    cmd_param[3] = 0x00;

    {
        cmd_param[1] = (start_addr & 0xFF00) >> 8;
        cmd_param[2] = start_addr & 0x00FF;

        for (int i = 0; i < param_len; ++i)
        {
            SPIC2->dr[0].byte = cmd_param[i];
        }
        spic_enable(SPIC2, ENABLE);

        do
        {
            *((uint32_t *)&data[rd_num]) = rtk_le32_to_cpu(SPIC2->dr[0].word);
            rd_num += 4;

            if ((SPIC2->isr & BIT_RXSIS) == BIT_RXSIS)
            {
                SPIC2->icr = 1;        // Clear all the interrupt
                SPIC2->ser = ser;
                split_ctr++;
                split_intr = 1;
            }

            rxflr = SPIC2->rxflr;
            for (i = 0; i < rxflr; i += 4)
            {
                *((uint32_t *)&data[rd_num]) = rtk_le32_to_cpu(SPIC2->dr[0].word);
                rd_num += 4;
            }

            if (split_intr == 1)
            {
                rd_num -= 4;

                split_intr = 0;
                SPIC2->ssienr = 0;
                SPIC2->flush_fifo = 1;
                SPIC2->ctrlr1 = data_len - rd_num;

                next_addr = start_addr + rd_num;
                cmd_param[1] = (next_addr & 0xFF00) >> 8;
                cmd_param[2] = next_addr & 0x00FF;

                for (int i = 0; i < param_len; ++i)
                {
                    SPIC2->dr[0].byte = cmd_param[i];
                }

                SPIC2->ssienr = 1;
            }

        }
        while (rd_num < data_len);
    }

    spic_enable(SPIC2, ENABLE);
    spic_set_seq_enable(0);

    //DBG_DIRECT("intr = %d", split_ctr);

    return retval;
}

uint32_t spinand_get_rdid(void)
{
    uint8_t flash_id[4];
    uint32_t rdid = 0;

    /* return magic pattern if rx cmd fail */
    if (!nand_cmd_rx(W25XXX_OP_READ_JEDEC_ID, 0, NULL, 4, flash_id))
    {
        return 0;
    }

    rdid = flash_id[1] << 16 | flash_id[2] << 8 | flash_id[3];

    return rdid;
}

void spinand_reset(void)
{
    /* reset and delay 500 us by spec */
    nand_cmd_tx(W25XXX_OP_RST, 0, NULL);
    platform_delay_us(500);
}

/*
    die: 0, 1
*/
void spinand_select_die(uint8_t die)
{
    if (nand.curr_die != die)
    {
        nand_cmd_tx(W25MXX_OP_DIE_SELECT, 1, &die);
        nand.curr_die = die;
    }
}

uint8_t spinand_read_reg(uint8_t reg_addr)
{
    uint8_t reg_val = 0;

    nand_read_sr(reg_addr, 1, &reg_val);

    return reg_val;
}

bool spinand_write_reg(uint8_t reg_addr, uint8_t reg_val)
{
    uint8_t buf[2];

    buf[0] = reg_addr, buf[1] = reg_val;
    nand_cmd_tx(W25XXX_OP_WRITE_REG, 2, buf);

    return true;
}

void spinand_unblock(void)
{
    uint8_t reg_val;

    reg_val = spinand_read_reg(W25XXX_REG_PROT);
    reg_val &= (~(BIT3 | BIT4 | BIT5 | BIT6));

    spinand_write_reg(W25XXX_REG_PROT, reg_val);
}

void spinand_buf_mode(uint8_t enable)
{
    uint8_t reg_val = spinand_read_reg(W25XXX_REG_CONF);

    if (enable)
    {
        reg_val |= W25XXX_REG_CONF_BUF;
    }
    else
    {
        reg_val &= ~W25XXX_REG_CONF_BUF;
    }

    spinand_write_reg(W25XXX_REG_CONF, reg_val);
}

bool spinand_write_enable(void)
{
    /* write enable should check WEL bit */
    uint8_t reg_val;

    nand_cmd_tx(W25XXX_OP_WRITE_ENABLE, 0, NULL);
    reg_val = spinand_read_reg(W25XXX_REG_STAT);

    if (reg_val & W25XXX_REG_STAT_WEL)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool spinand_write_disable(void)
{
    /* write enable should check WEL bit */
    uint8_t reg_val;

    nand_cmd_tx(W25XXX_OP_WRITE_DISABLE, 0, NULL);
    reg_val = spinand_read_reg(W25XXX_REG_STAT);

    if (reg_val & W25XXX_REG_STAT_WEL)
    {
        return false;
    }
    else
    {
        return true;
    }
}

static bool w25xxx_map_page_ex(size_t off,
                               uint8_t *die_num, uint16_t *page_num,
                               uint16_t *page_off, uint8_t bb_reserve)
{
    bool res = false;
    size_t die_size = W25XXX_DIE_SIZE;
    size_t orig_off = off;
    die_size -= ((size_t) bb_reserve) * W25XXX_BLOCK_SIZE;
    *die_num = 0;

    while (off >= die_size)
    {
        off -= die_size;
        (*die_num)++;
        if (*die_num >= nand.num_dies) { goto out; }
    }

    *page_num = off / W25XXX_PAGE_SIZE;
    if (page_off != NULL) { *page_off = off % W25XXX_PAGE_SIZE; }
    res = true;

out:
    (void) orig_off;
    return res;
}

bool spinand_block_erase(size_t off, size_t len)
{
    bool ret = true;
    const size_t orig_off = off, orig_len = len;

    if (off % W25XXX_BLOCK_SIZE != 0 || len % W25XXX_BLOCK_SIZE != 0)
    {
        DBG_DIRECT("Addr/Len is not block size aligned.");
        ret = false;
        goto out;
    }

    uint8_t die_num;
    uint16_t page_num, page_off;
    while (len > 0)
    {
        if (!w25xxx_map_page_ex(off, &die_num, &page_num, &page_off, 0))
        {
            ret = false;
            goto out;
        }

        DBG_DIRECT("Page Number: %X", page_num);
        spinand_select_die(die_num);
        spinand_write_enable();

        uint8_t cmd_param[3];
        cmd_param[0] = 0x00;
        cmd_param[1] = (page_num & 0xFF00) >> 8;
        cmd_param[2] = page_num & 0xFF;

        nand_cmd_tx(W25XXX_OP_BLOCK_ERASE, 3, cmd_param);

        uint8_t st;
        while ((st = spinand_read_reg(W25XXX_REG_STAT)) & W25XXX_REG_STAT_BUSY)
        {
            DBG_DIRECT("Erasing %08X...", off);
        }
        if (st & W25XXX_REG_STAT_EFAIL)
        {
            DBG_DIRECT("Erase fail!");
            ret = false;
            goto out;
        }
        off += W25XXX_BLOCK_SIZE;
        len -= W25XXX_BLOCK_SIZE;
    }

out:
    (void) orig_off;
    (void) orig_len;

    return ret;
}

bool spinand_page_data_read(uint16_t page_num)
{
    bool ret = true;
    uint8_t cmd_param[3];
    uint8_t st;

    cmd_param[0] = 0x00;
    cmd_param[1] = (page_num & 0xFF00) >> 8;
    cmd_param[2] = page_num & 0xFF;

    nand_cmd_tx(W25XXX_OP_PAGE_DATA_READ, 3, cmd_param);

    while (spinand_read_reg(W25XXX_REG_STAT) & W25XXX_REG_STAT_BUSY)
    {
        //DBG_DIRECT("Page read preparing: %X", page_num);
    }

    st = spinand_read_reg(W25XXX_REG_STAT) & (W25XXX_REG_STAT_ECC0 | W25XXX_REG_STAT_ECC1);

    switch (st)
    {
    case 0:
        APP_PRINT_INFO0("Page Read Success!");
        ret = true;
        break;
    case 1:
        APP_PRINT_WARN0("Fix by ECC!");
        ret = true;
        break;
    case 2:
        APP_PRINT_ERROR0("Can fix by remapping bad blocks!");
        ret = false;
        break;
    case 3:
        APP_PRINT_ERROR0("Cannot fix!");
        ret = false;
        break;
    }

    return ret;
}

bool spinand_read_internal(uint32_t addr, uint32_t len, uint8_t *buf)
{
    bool ret = true;
    uint8_t die_num;
    uint16_t page_num;
    uint16_t page_off;

    /* See 7.2.5.
    By default, after power up, the data in page 0 will be automatically loaded into the Data Buffer
    and the device is ready to accept any read commands.
    */
    static int8_t last_die_num = 0xFF; /* set to cur_die_num to a invalid die to load the first page*/
    static uint16_t last_page_num = 0;

    w25xxx_map_page_ex(addr, &die_num, &page_num, &page_off, 0);

    if ((die_num == last_die_num) && (page_num == last_page_num))
    {
        ;
    }
    else
    {
        last_die_num = die_num;
        last_page_num = page_num;

        spinand_select_die(die_num);

        /* load data into page buffer, need to check busy bit */
        if (spinand_page_data_read(page_num) == false)
        {
            ret = false;
            goto _end;
        }

        uint8_t st;
        while ((st = spinand_read_reg(W25XXX_REG_STAT)) & W25XXX_REG_STAT_BUSY)
        {
            APP_PRINT_INFO2("Reading page num %08X, st = %02X...", page_num, st);
        }
    }

    /* read out */
    ret = nand_read_internal(page_off, len, buf);

_end:
    return ret;
}

/**
 * @brief read flash data via user mode
 * @param start_addr    start_addr address where is going to be read in flash
 * @param data_len      data length to be read
 * @param data          data buffer to be read into
 * @return true if success
 * @note start address should be 4 byte align
*/
bool spinand_read(uint32_t start_addr, uint32_t data_len, uint8_t *data)
{
    uint32_t tmp_addr = start_addr; //convert_flash_addr_to_offset(start_addr);
    uint8_t *tmp_data = data;
    int remain_size = data_len;

    while (remain_size > 0)
    {
        uint32_t read_size = remain_size >  W25XXX_PAGE_SIZE ? W25XXX_PAGE_SIZE : remain_size;

        if (!spinand_read_internal(tmp_addr, read_size, tmp_data))
        {
            return false;
        }

        tmp_addr += read_size;
        tmp_data += read_size;
        remain_size -= read_size;
    }

    return true;
}

bool spinand_pgm_data_load(uint16_t page_off, uint32_t data_len, uint8_t *data)
{
    bool retval = true;
    uint8_t cmd_pp = nand.curr_wr_cmd;
    uint32_t i;

    spic_enable(SPIC2, DISABLE);
    SPIC2->addr_length = 2;
    spic_set_tx_mode(SPIC2);
    spic_set_multi_ch(SPIC2, nand.wr_data_ch, nand.wr_addr_ch);

    uint8_t cmd_param[3];
    cmd_param[0] = cmd_pp;
    cmd_param[1] = (page_off & 0xFF00) >> 8;
    cmd_param[2] = page_off & 0x00FF;

    for (int i = 0; i < 3; ++i)
    {
        SPIC2->dr[0].byte = cmd_param[i];
    }

    for (i = 0; i < data_len; i++)
    {
        SPIC2->dr[0].byte = *(data + i);
    }

    spic_enable(SPIC2, ENABLE);

    if (!spic_wait_busy(SPIC2))
    {
        retval = false;
    }

    spic_enable(SPIC2, DISABLE);

    return retval;
}

bool spinand_pgm_execute(uint16_t page_num)
{
    bool ret = true;
    uint8_t st = 0;
    uint8_t cmd_param[3];

    cmd_param[0] = 0x00;
    cmd_param[1] = (page_num & 0xFF00) >> 8;
    cmd_param[2] = page_num & 0xFF;

    nand_cmd_tx(W25XXX_OP_PROG_EXECUTE, 3, cmd_param);

    while ((st = spinand_read_reg(W25XXX_REG_STAT)) & W25XXX_REG_STAT_BUSY)
    {
        //DBG_DIRECT("Programming page num %08X...", page_num);
    }

    if (st & W25XXX_REG_STAT_PFAIL)
    {
        ret = false;
    }

    return ret;
}

bool spinand_write_internal(uint32_t addr, uint32_t len, uint8_t *buf)
{
    bool ret = true;
    uint8_t die_num;
    uint16_t page_num;
    uint16_t page_off;

    w25xxx_map_page_ex(addr, &die_num, &page_num, &page_off, 0);
    spinand_select_die(die_num);

    /* load into buffer first */
    spinand_page_data_read(page_num);

    /* write enable.
    PS: Winbond needs enable write before program but GD and EMST don't need. */
    spinand_write_enable();

    /* program into buffer */
    for (uint16_t i = 0; i < len; i += (SPIC_FIFO_SIZE - 4))
    {
        uint8_t fifo_size = (len - i) >= (SPIC_FIFO_SIZE - 4) ? (SPIC_FIFO_SIZE - 4) : (len - i);
        uint32_t wr_off = page_off + i;

        spinand_pgm_data_load(wr_off, fifo_size, buf + i);
    }

    /* update to flash */
    ret = spinand_pgm_execute(page_num);

    return ret;
}

/**
 * @brief program data to flash via user mode
 * @param start_addr    start address where is going to be written in flash
 * @param data_len      data length to be written
 * @param data          data buffer to be written
 * @return true if success
 * @note start address should be 4 byte align
*/
bool spinand_write(uint32_t start_addr, uint32_t data_len, uint8_t *data)
{
    uint32_t wr_addr = start_addr;
    uint32_t wr_size;
    uint8_t *wr_buf = data;
    int remain_size = data_len;

    while (remain_size > 0)
    {
        if (wr_addr % W25XXX_PAGE_SIZE)
        {
            wr_size = W25XXX_PAGE_SIZE - wr_addr % W25XXX_PAGE_SIZE;
        }
        else
        {
            wr_size = W25XXX_PAGE_SIZE;
        }

        if (wr_size > remain_size)
        {
            wr_size = remain_size;
        }

        if (!spinand_write_internal(wr_addr, wr_size, wr_buf))
        {
            return false;
        }

        wr_addr += wr_size;
        wr_buf  += wr_size;

        remain_size -= wr_size;
    }

    return true;
}

void spinand_config(T_FLASH_MODE bit_mode)
{
    if (bit_mode == FLASH_MODE_1BIT)
    {
        SPIC2->ssienr = 0;
        SPIC2->ctrlr0 &= ~(BIT_CMD_CH(3) | BIT_ADDR_CH(3) | BIT_DATA_CH(3));
        SPIC2->ssienr = 1;

        nand.curr_rd_cmd = W25XXX_OP_READ;
        nand.curr_wr_cmd = W25XXX_OP_PROG_RAND_DATA_LOAD;
        nand.rd_addr_ch  = CHANN_SINGLE;
        nand.rd_data_ch  = CHANN_SINGLE;
        nand.wr_addr_ch  = CHANN_SINGLE;
        nand.wr_data_ch  = CHANN_SINGLE;

        nand.bit_mode = bit_mode;
    }
    else if (bit_mode == FLASH_MODE_4BIT)
    {
        SPIC2->ssienr = 0;
        SPIC2->ctrlr0 &= ~(BIT_CMD_CH(3) | BIT_ADDR_CH(3) | BIT_DATA_CH(3));
        SPIC2->ctrlr0 |= (BIT_ADDR_CH(bit_mode) | BIT_DATA_CH(bit_mode));
        SPIC2->valid_cmd |= (BIT4 | BIT8);
        SPIC2->ssienr = 1;

        nand.curr_rd_cmd = W25XXX_OP_QUAD_READ;
        nand.curr_wr_cmd = W25XXX_OP_PROG_RAND_QUAD_DATA_LOAD;
        nand.rd_addr_ch  = CHANN_QUAD;
        nand.rd_data_ch  = CHANN_QUAD;
        nand.wr_addr_ch  = CHANN_SINGLE;
        nand.wr_data_ch  = CHANN_QUAD;

        nand.bit_mode = bit_mode;
    }
    else
    {
        DBG_DIRECT("SPI NAND doesn't support %d bit mode.", 1 << bit_mode);
        while (1);
    }
}

/*
calibration flow:
    enter OTP access mode
       OTP-E = 1; */

/* read parameter page signature. the first 4 bytes of the second page
Page Address: 01h, Data Length: 256-Byte x 3
*/

/* what's the data read flow?
1. Page Data Read
    Send Bytes: 13h xxh PA[15-8] PA[7-0]
    RDSR BUSY, tRD
    BUSY = 0; 2048+64 bytes is loaded into Page Buffer
2. Read Data
   1 bit read
   Buf Mode:
    Send Bytes: 03h PA[15-8] PA[7-0] xxh, read Data
   Cont Mode:
    Send Bytes: 03h xxh xxh xxh, read data
   4 bit read
   Buf Mode:
    Send Bytes: EBh PA[15-8] PA[7-0] xxh xxh, read data
   Cont Mode:
    Send Bytes: EBh xxh xxh xxh xxh xxh xxh, read data
*/

/* exit OTP access mode
   OTP-E = 1;
   */

static uint16_t g_def_dummy_len[3] = {0x0, 0x8, 0xC};/* default dummy length base for each bit mode */

bool nand_calibration(T_FLASH_MODE bit_mode)
{
    bool ret = false;
    uint16_t dly_cnt = 4;

    APP_PRINT_INFO5("Bit: %d, RD CMD: %02X, WR CMD: %02X, RD ADDR CH: %02X, RD DATA CH: %02X",
                    nand.bit_mode, nand.curr_rd_cmd, nand.curr_wr_cmd, nand.rd_addr_ch, nand.rd_data_ch);

    /* adjust AUTO_LENGTH[RD_DUMMY_LENGTH], make sure read data equals magic pattern. */
    for (; dly_cnt < 32; ++dly_cnt)
    {
        uint32_t rd_data;

        spic_enable(SPIC2, DISABLE);
        spic_set_rd_dummy_length(SPIC2, dly_cnt);
        spic_enable(SPIC2, ENABLE);

        spinand_read_internal(0x800, 4, (uint8_t *)&rd_data);
        APP_PRINT_INFO2("cnt=%d, %08X", dly_cnt, rd_data);

        /* TODO: only Winbond SPI NAND has OTP signature, need a general calibration magic number */
        if (rd_data == PARAM_PAGE_SIGNATURE)
        {
            /* update to AUTO_DUM_LEN when 1 bit mode */
            if (bit_mode == FLASH_MODE_1BIT)
            {
                spic_enable(SPIC2, DISABLE);
                SPIC2->auto_length &= ~ BIT_AUTO_DUM_LEN(0xFF);
                SPIC2->auto_length |= BIT_AUTO_DUM_LEN(dly_cnt);
                spic_enable(SPIC2, ENABLE);
                nand.rd_st_dummy_cycle = dly_cnt;
            }

            /* update IN_PHYSICAL_CYC */
            if (dly_cnt >= g_def_dummy_len[bit_mode])
            {
                spic_enable(SPIC2, DISABLE);
                SPIC2->auto_length |= BIT_IN_PHYSICAL_CYC(dly_cnt - g_def_dummy_len[bit_mode]);
                spic_enable(SPIC2, ENABLE);
            }

            ret = true;
            /* get things done, break */
            break;
        }
    }

    return ret;
}

void spinand_calibration(void)
{
    uint8_t conf_reg = spinand_read_reg(W25XXX_REG_CONF);

    /* enter OTP access mode */
    conf_reg |= W25XXX_REG_CONF_OTPE;
    spinand_write_reg(W25XXX_REG_CONF, conf_reg);

    APP_PRINT_INFO1("CONF REG: %02X", spinand_read_reg(W25XXX_REG_CONF));

    spinand_config(FLASH_MODE_4BIT);
    nand_calibration(FLASH_MODE_4BIT);

    /* exit OTP access mode */
    conf_reg &= (~W25XXX_REG_CONF_OTPE);
    spinand_write_reg(W25XXX_REG_CONF, conf_reg);
}

void spinand_init(void)
{
    uint32_t rdid;
    uint8_t dev_id_h;

    spinand_reset();

    rdid = spinand_get_rdid();

    DBG_DIRECT("RDID: %06X", rdid);

    /* detect die number */
    dev_id_h = (rdid & 0xFF00) >> 8;
    if (dev_id_h == 0xAB)  /* 0xAB: W25M02GV */
    {
        nand.num_dies = 2;
    }
    else
    {
        nand.num_dies = 1;    /* 0xAA: W25N01GV */
    }

    /* put all dies into buffer mode, and unblock */
    for (uint8_t die = 0; die < nand.num_dies; die++)
    {
        uint8_t reg_prot, reg_conf, reg_stat;

        spinand_select_die(die);

        reg_prot = spinand_read_reg(W25XXX_REG_PROT);
        reg_conf = spinand_read_reg(W25XXX_REG_CONF);
        reg_stat = spinand_read_reg(W25XXX_REG_STAT);

        DBG_DIRECT("Die: %d, SR: %02X-%02X-%02X", die, reg_prot, reg_conf, reg_stat);

        if ((reg_conf & W25XXX_REG_CONF_BUF) == 0)
        {
            /* enter buf mode */
            spinand_buf_mode(1);
            DBG_DIRECT("  Enable Buf Mode");
        }
        else
        {
            DBG_DIRECT("  Already Buf Mode");
        }

        if (reg_prot & W25XXX_REG_PROT_BP)
        {
            spinand_unblock();
            DBG_DIRECT("  Unlock All");
        }
        else
        {
            DBG_DIRECT("  Already Unlock");
        }
    }

    /* select the first die */
    spinand_select_die(0);

    spinand_calibration();
}

static void dump_buf(uint32_t addr_base, uint8_t *p, uint32_t len)
{
    for (uint32_t i = 0; i < len; i += 16)
    {
        DBG_DIRECT("%08X: %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X",
                   addr_base + i,
                   p[i + 0], p[i + 1], p[i + 2], p[i + 3],
                   p[i + 4], p[i + 5], p[i + 6], p[i + 7],
                   p[i + 8], p[i + 9], p[i + 10], p[i + 11],
                   p[i + 12], p[i + 13], p[i + 14], p[i + 15]);
    }
}

static uint8_t in_buf[W25XXX_PAGE_SIZE];
static uint8_t out_buf[W25XXX_PAGE_SIZE];

#define TEST_BASE_ADDR  (1024 * 1024)
#define TEST_LEN        (32 * 1024)

void spinand_test(void)
{
    bool ret = true;

    spinand_write_enable();
    DBG_DIRECT("WREN, NAND REGS: %02X-%02X-%02X",
               spinand_read_reg(W25XXX_REG_PROT),
               spinand_read_reg(W25XXX_REG_CONF),
               spinand_read_reg(W25XXX_REG_STAT));

    spinand_write_disable();
    DBG_DIRECT("WRDIS NAND REGS: %02X-%02X-%02X",
               spinand_read_reg(W25XXX_REG_PROT),
               spinand_read_reg(W25XXX_REG_CONF),
               spinand_read_reg(W25XXX_REG_STAT));

    /* RW test */
    memset(in_buf, 0xFF, sizeof(in_buf));
    memset(out_buf, 0xFF, sizeof(out_buf));

    spinand_block_erase(TEST_BASE_ADDR, TEST_LEN > W25XXX_BLOCK_SIZE ? TEST_LEN : W25XXX_BLOCK_SIZE);

    for (uint32_t i = 0; i < W25XXX_PAGE_SIZE; i++)
    {
        in_buf[i] = i & 0xFF;
    }

    for (uint32_t addr = TEST_BASE_ADDR; addr < TEST_BASE_ADDR + TEST_LEN; addr += W25XXX_PAGE_SIZE)
    {
        /* program 2K */
        if (spinand_write(addr, W25XXX_PAGE_SIZE, in_buf) == false)
        {
            DBG_DIRECT("write %d bytes to %08X fail!", W25XXX_PAGE_SIZE, addr);
            goto _end;
        }
    }

    for (uint32_t addr = TEST_BASE_ADDR; addr < TEST_BASE_ADDR + TEST_LEN; addr += W25XXX_PAGE_SIZE)
    {
        if (spinand_read(addr, W25XXX_PAGE_SIZE, out_buf) == false)
        {
            DBG_DIRECT("read %d bytes from %08X fail!", W25XXX_PAGE_SIZE, addr);
            goto _end;
        }

        dump_buf(addr, out_buf, W25XXX_PAGE_SIZE);

        if (memcmp(in_buf, out_buf, sizeof(in_buf)) != 0)
        {
            ret = false;
        }
    }

    if (ret == false)
    {
        DBG_DIRECT("SPI NAND Read fail!");
        goto _end;
    }
    else
    {
        DBG_DIRECT("SPI NAND Read pass!");
    }

    DBG_DIRECT("Start Read %d bytes Begin", TEST_LEN);
    for (uint32_t addr = TEST_BASE_ADDR; addr < TEST_BASE_ADDR + TEST_LEN; addr += W25XXX_PAGE_SIZE)
    {
        if (spinand_read(addr, W25XXX_PAGE_SIZE, out_buf) == false)
        {
            DBG_DIRECT("read %d bytes from %08X fail!", W25XXX_PAGE_SIZE, addr);
            return;
        }
    }
    DBG_DIRECT("Start Read %d bytes End", TEST_LEN);

_end:
    return;
}
