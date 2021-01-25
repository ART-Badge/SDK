#include <stddef.h>
#include "otp.h"
#include "psram_platform.h"
#include "rtl876x_lib_platform.h"
#include "rtl876x.h"
#include "rtl876x_rcc.h"
#include "rtl876x_pinmux.h"
#include "rtl876x_gdma.h"
#include "rtl876x_spic.h"
#include "flash_device.h"
#include "trace.h"
#include "platform_utils.h"

#define PSRAM_MAGIC_PATTERN        0x5A5A12A5
#define PSRAM_CAL_ADDR             0x6802030
#define DELAY_CNT_MAX              32
#define FMC_MAIN1                  0x6800000

/** @defgroup FLASH_DRIVER_Registers_Definitions Flash Driver Registers Definitions
  * @brief
  * @{
  */


/** End of FLASH_DRIVER_Registers_Definitions
  * @}
  */
typedef enum _ACTIVE_CLK_SRC
{
    CLK_SRC_40M_OSC = 0,
    CLK_SRC_40M_XTAL,
    CLK_SRC_PLL,
    CLK_SRC_DEF_MAX,
} ACTIVE_CLK_SRC_TYPE;

typedef enum
{
    CHANN_SINGLE = 0,
    CHANN_DUAL = 1,
    CHANN_QUAD = 2
} T_CHANN_TYPE;

typedef enum
{
    READ_CMD            = 0x03,
    FAST_READ_QUAD_CMD  = 0xEB,
    WRITE_CMD           = 0x02,
    QUAD_WRITE_CMD      = 0x38,
    RDID_CMD            = 0x9F,
    RESET_EN_CMD        = 0x66,
    RESET_CMD           = 0x99,
    HALF_SLEEP_CMD      = 0xC0,
} PSRAM_CMD;

/**
  * @brief  PSRAM Init structure definition
  */
typedef struct
{
    uint8_t curr_rd_cmd;        /* current used read cmd */
    uint8_t curr_wr_cmd;        /* current used write cmd */
    uint8_t rd_data_ch;         /* read data channel */
    uint8_t rd_addr_ch;         /* read address channel */
    uint8_t rd_st_dummy_cycle;  /* read status dummy cycle */
    uint8_t bit_mode;
} T_PSRAM_DEVICE_INFO;

typedef struct
{
    uint32_t ctrlr0;
    uint32_t auto_length;
    uint32_t valid_cmd;
    uint8_t  rd_st_dummy_cycle;
    uint8_t  bit_mode;
    bool     is_calibrated;
} T_PSRAM_CAL_INFO;

T_PSRAM_CAL_INFO psram_cal_info_20, psram_cal_info_40, psram_cal_info_60,
                 psram_cal_info_80, psram_cal_info_90, psram_cal_info_100;

T_PSRAM_DEVICE_INFO psram_device_info =
{
    .curr_rd_cmd = READ_CMD,
    .curr_wr_cmd = WRITE_CMD,
    .rd_data_ch  = CHANN_SINGLE,
    .rd_addr_ch  = CHANN_SINGLE,
};

extern uint32_t FlashSrcClk;
extern void spic_enable(SPIC_TypeDef *spic, uint32_t enable);
extern void spic_set_rd_dummy_length(SPIC_TypeDef *spic, uint8_t dummy_len);
extern bool spic_wait_busy(SPIC_TypeDef *spic);

SPIC_TypeDef *spic = NULL;

static uint16_t g_def_dummy_len[3] = {0x0, 0x8, 0xC};/* default dummy length base for each bit mode */

static uint32_t convert_psram_addr_to_offset(uint32_t addr)
{
    return (addr - FMC_MAIN1);
}

/**
 * @brief set spic enable or disable.
 *
 * @param enable          DISABLE to disable cache, ENABLE to enable cache.
 * @return
 * @note: spic should be disabled before programming any register in user mode
*/
void spic_enable(SPIC_TypeDef *spic, uint32_t enable)
{
    spic->ssienr = enable;
}

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

static bool spic_cmd_tx(uint8_t cmd, uint8_t data_len, uint8_t *data_buf)
{
    bool retval = true;

    uint32_t ctrlr0 = spic->ctrlr0;
    uint32_t addr_len = spic->addr_length;

    spic_enable(spic, DISABLE);
    spic->ctrlr0 &= ~(BIT_CMD_CH(3) | BIT_ADDR_CH(3) | BIT_DATA_CH(3));
    spic->ctrlr0 &= ~(BIT_TMOD(3)); /* TX Mode */

    spic->addr_length = data_len;
    spic->dr[0].byte = cmd;

    while (data_len--)
    {
        spic->dr[0].byte = *data_buf++;
    }

    spic_enable(spic, ENABLE);

    if (!spic_wait_busy(spic))
    {
        retval = false;
    }

    spic_enable(spic, DISABLE);

    //restore ctrl0 and addr_len register
    spic->ctrlr0 = ctrlr0;
    spic->addr_length = addr_len;

    return retval;
}

void psram_init(void)
{
    RCC_PeriphClockCmd(APBPeriph_FLASH1, APBPeriph_FLASH1_CLOCK, (FunctionalState)ENABLE);

    Pinmux_Config(P1_7, IDLE_MODE); /* SPIC1_CS   */
    Pinmux_Config(P3_6, IDLE_MODE); /* SPIC1_CLK  */
    Pinmux_Config(P3_2, IDLE_MODE); /* SPIC1_SIO0 */
    Pinmux_Config(P3_3, IDLE_MODE); /* SPIC1_SIO1 */
    Pinmux_Config(P3_4, IDLE_MODE); /* SPIC1_SIO2 */
    Pinmux_Config(P3_5, IDLE_MODE); /* SPIC1_SIO3 */

    Pad_Config(P1_7, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE, PAD_OUT_LOW);
    Pad_Config(P3_6, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_NONE, PAD_OUT_DISABLE, PAD_OUT_LOW);
    Pad_Config(P3_2, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE, PAD_OUT_LOW);
    Pad_Config(P3_3, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE, PAD_OUT_LOW);
    Pad_Config(P3_4, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE, PAD_OUT_LOW);
    Pad_Config(P3_5, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE, PAD_OUT_LOW);

    /* spic1 enable controller */
    SYSBLKCTRL->u_210.BITS_210.BIT_SOC_FLASH_EN1 = 1;

    /* spic1 master enable */
    SYSBLKCTRL->u_2A8.BITS_2A8.SPIC1_MASTER_EN = 1;

    /* spic1 clock source enable bit */
    switch (OTP->CLK_SRC_FLASH)
    {
    case CLK_SRC_40M_OSC:
        HAL_UPDATE32(PERIPH_REG_BASE + 0x2D0, 0x1E0, BIT5 | BIT8);
        break;
    case CLK_SRC_40M_XTAL:
        HAL_UPDATE32(PERIPH_REG_BASE + 0x2D0, 0x1E0, BIT5 | BIT6 | BIT8);
        break;
    case CLK_SRC_PLL:
        HAL_UPDATE32(PERIPH_REG_BASE + 0x2D0, 0x1E0, BIT5 | BIT6 | BIT7 | BIT8);
        break;
    }

    /* set cs and baudrate */
    SPIC1->ssienr = 0;
    SPIC1->ser = BIT0;
    SPIC1->baudr = 1;

    /* ctrlr2[17:16] = 2'b11 to remove WREN+RDSR (i.e., only PP cmd issue) */
    SPIC1->ctrlr2 |= (BIT16 | BIT17);

    /* use ctrlr0 CMD_CH/DATA_CH/ADDR_CH in auto mode */
    //BIT_BAND(SPIC1->valid_cmd, 12) = 1; /* CTRLR0_CH */

    spic = SPIC1;
}

/*
What are the differences between 1 bit mode and 4 bit mode?
only 1 bit and 4 bit supported

address length always 3
1 bit:
    1-1-1
4 bit:
    1-4-4


auto mode bit num:
1 bit:
    VALID_CMD: no bits or clear 4 bit bits

4 bit:
    VALID_CMD: BIT_WR_QUAD_II(WR) / RD_QUAD_IO(RD)
    READ_QUAD_ADDR_DATA[0:7]: 0xEB (PRM_EN disable)
    WRITE_QUAD_ADDR_DATA:     0x38

user mode bit num:
    ctrlr0: CMD_CH, DATA_CH, ADDR_CH

*/
static void psram_config(T_FLASH_MODE bit_mode)
{
    if (bit_mode == FLASH_MODE_1BIT)
    {
        spic->ssienr = 0;
        spic->ctrlr0 &= ~(BIT_CMD_CH(3) | BIT_ADDR_CH(3) | BIT_DATA_CH(3));
        spic->ssienr = 1;

        psram_device_info.curr_rd_cmd = READ_CMD;
        psram_device_info.curr_wr_cmd = WRITE_CMD;
        psram_device_info.rd_addr_ch  = CHANN_SINGLE;
        psram_device_info.rd_data_ch  = CHANN_SINGLE;

        psram_device_info.bit_mode = bit_mode;
    }
    else if (bit_mode == FLASH_MODE_4BIT)
    {
        spic->ssienr = 0;
        spic->ctrlr0 &= ~(BIT_CMD_CH(3) | BIT_ADDR_CH(3) | BIT_DATA_CH(3));
        spic->ctrlr0 |= (BIT_ADDR_CH(bit_mode) | BIT_DATA_CH(bit_mode));
        spic->valid_cmd |= (BIT4 | BIT8);
        spic->ssienr = 1;

        psram_device_info.curr_rd_cmd = FAST_READ_QUAD_CMD;
        psram_device_info.curr_wr_cmd = QUAD_WRITE_CMD;
        psram_device_info.rd_addr_ch  = CHANN_QUAD;
        psram_device_info.rd_data_ch  = CHANN_QUAD;

        psram_device_info.bit_mode = bit_mode;
    }
    else
    {
        DBG_DIRECT("PSRAM doesn't support %d bit mode.", 1 << bit_mode);
        while (1);
    }
}

void psram_seq_trans_enable(uint8_t enable)
{
    spic->ssienr = 0;

    if (enable)
    {
        uint8_t bit_mode = psram_device_info.bit_mode;
        uint16_t rd_dummy_len = BIT_RD_DUMMY_LENGTH(spic->auto_length);

        if (rd_dummy_len >= g_def_dummy_len[bit_mode])
        {
            HAL_UPDATE32(&spic->auto_length, (BIT18 | BIT19 | BIT20 | BIT21 | BIT22 | BIT23 | BIT24 | BIT25),
                         BIT_AUTO_DUM_LEN((spic->baudr * 2) * (8 / (1 << bit_mode))));
        }

        BIT_BAND(spic->valid_cmd, 14) = enable; /* BIT_SEQ_TRANS_EN */
    }
    else
    {
        HAL_UPDATE32(&spic->auto_length, (BIT18 | BIT19 | BIT20 | BIT21 | BIT22 | BIT23 | BIT24 | BIT25),
                     BIT_AUTO_DUM_LEN(psram_device_info.rd_st_dummy_cycle));

        BIT_BAND(spic->valid_cmd, 14) = enable; /* BIT_SEQ_TRANS_EN */
    }
}

bool psram_calibration(T_FLASH_MODE bit_mode)
{
    bool ret = false;
    uint16_t dly_cnt = g_def_dummy_len[bit_mode];

    /* adjust AUTO_LENGTH[RD_DUMMY_LENGTH], make sure read data equals magic pattern. */
    for (; dly_cnt < DELAY_CNT_MAX; ++dly_cnt)
    {
        uint32_t rd_data;

        spic_enable(spic, DISABLE);
        spic_set_rd_dummy_length(spic, dly_cnt);
        spic_enable(spic, ENABLE);

        rd_data = HAL_READ32(PSRAM_CAL_ADDR, 0);

        APP_PRINT_INFO3("cnt=%d, [%08X]=%08X", dly_cnt, PSRAM_CAL_ADDR, rd_data);

        if (rd_data == PSRAM_MAGIC_PATTERN)
        {
            /* update to AUTO_DUM_LEN when 1 bit mode */
            if (bit_mode == FLASH_MODE_1BIT)
            {
                spic_enable(spic, DISABLE);
                spic->auto_length &= ~ BIT_AUTO_DUM_LEN(0xFF);
                spic->auto_length |= BIT_AUTO_DUM_LEN(dly_cnt);
                spic_enable(spic, ENABLE);
                psram_device_info.rd_st_dummy_cycle = dly_cnt;
            }

            /* update IN_PHYSICAL_CYC */
            if (dly_cnt >= g_def_dummy_len[bit_mode])
            {
                spic_enable(spic, DISABLE);
                spic->auto_length |= BIT_IN_PHYSICAL_CYC(dly_cnt - g_def_dummy_len[bit_mode]);
                spic_enable(spic, ENABLE);
            }

            ret = true;
            /* get things done, break */
            break;
        }
    }

    return ret;
}

T_FLASH_RET psram_try_high_speed(T_FLASH_MODE bit_mode)
{
    T_PSRAM_CAL_INFO *p_cal_info;
    uint8_t cur_bit_mode = psram_device_info.bit_mode;

    switch (FlashSrcClk)
    {
    case SYSTEM_20MHZ:
        p_cal_info = &psram_cal_info_20;
        break;
    case SYSTEM_40MHZ:
        p_cal_info = &psram_cal_info_40;
        break;
    case SYSTEM_60MHZ:
        p_cal_info = &psram_cal_info_60;
        break;
    case SYSTEM_80MHZ:
        p_cal_info = &psram_cal_info_80;
        break;
    case SYSTEM_90MHZ:
        p_cal_info = &psram_cal_info_90;
        break;
    case SYSTEM_100MHZ:
        p_cal_info = &psram_cal_info_100;
        break;
    }

    if (p_cal_info->is_calibrated && (cur_bit_mode == bit_mode))
    {
        spic_enable(spic, DISABLE);
        spic->ctrlr0      = p_cal_info->ctrlr0;
        spic->auto_length = p_cal_info->auto_length;
        spic->valid_cmd   = p_cal_info->valid_cmd;
        spic_enable(spic, ENABLE);
        psram_device_info.rd_st_dummy_cycle = p_cal_info->rd_st_dummy_cycle;

        return FLASH_SUCCESS;
    }

    if (bit_mode == FLASH_MODE_2BIT)
    {
        DBG_DIRECT("PSRAM doesn't support %d bit mode.", 1 << bit_mode);
        while (1);
    }

    /*
    1 bit calibration: 1 bit and RDSR rd dummy length
    use auto write to write a magic pattern before calibration
    */
    psram_config(FLASH_MODE_1BIT);

    uint32_t *p_magic_pattern = (uint32_t *)PSRAM_CAL_ADDR;

    *p_magic_pattern = PSRAM_MAGIC_PATTERN;

    /* start 1 bit calibration and get 1 bit rd dummy length */
    /* do calibration -->
            get a rd_dummy_len_val
            set AUTO_LENGTH[RD_DUMMY_LENGTH] and AUTO_LENGTH[AUTO_DUM_LEN] = rd_dummy_len_val
            set AUTO_LENGTH[IN_PHYSICAL_CYC] = rd_dummy_len_val - g_def_dummy_len
    */

    if (psram_calibration(FLASH_MODE_1BIT) == true)
    {
        APP_PRINT_INFO2("1 bit calibration pass: [0x%08X]=%08X", p_magic_pattern, *p_magic_pattern);
    }
    else
    {
        APP_PRINT_INFO0("1 bit calibration fail!");
        return FLASH_CAL_FAILED;
    }

    /* start 4 bit calibration and get 4 bit rd dummy length */
    /* switch to 4 bit mode */
    /* do calibration */
    if (bit_mode == FLASH_MODE_4BIT)
    {
        psram_config(FLASH_MODE_4BIT);

        if (psram_calibration(FLASH_MODE_4BIT) == true)
        {
            APP_PRINT_INFO2("4 bit calibration pass: [0x%08X]=%08X", p_magic_pattern, *p_magic_pattern);
        }
        else
        {
            APP_PRINT_INFO0("4 bit calibration fail!");
            return FLASH_CAL_FAILED;
        }
    }

    p_cal_info->ctrlr0      = spic->ctrlr0;
    p_cal_info->auto_length = spic->auto_length;
    p_cal_info->valid_cmd   = spic->valid_cmd;
    p_cal_info->rd_st_dummy_cycle = psram_device_info.rd_st_dummy_cycle;
    p_cal_info->bit_mode = bit_mode;
    p_cal_info->is_calibrated = 1;

    return FLASH_SUCCESS;
}

bool psram_cmd_rx(uint8_t cmd, uint8_t read_len, uint8_t *read_buf)
{
    return true;
}

bool psram_cmd_tx(uint8_t cmd, uint8_t data_len, uint8_t *data_buf)
{
    return true;
}

bool psram_read(uint32_t addr, uint32_t len, uint8_t *data)
{
    return true;
}

/**
 * @brief program data to psram via user mode
 * @param start_addr    start address where is going to be written in flash
 * @param data_len      data length to be written
 * @param data          data buffer to be written
 * @return
 * @note 1. SPIC only supports SPIC_FIFO_SIZE FIFO, so max SPIC_FIFO_SIZE-4 word allowed to be written.
 *       2. start address should be 4 byte align
*/
static bool psram_write_internal(uint32_t start_addr, uint32_t data_len, uint8_t *data)
{
    bool retval = true;
    uint8_t cmd_pp = psram_device_info.curr_wr_cmd, i;
    uint32_t wr_addr;
    uint32_t *data_word = (uint32_t *)data;

    /* the test show that enable spic before push fifo make transfer faster, but we can't push by
    byte. if we want to push by byte, we should disable spic first. */
    spic_enable(spic, ENABLE);
    wr_addr = cmd_pp | __REV(start_addr);
    spic->dr[0].word = wr_addr;

    for (i = 0; i < data_len / 4; i++)
    {
        spic->dr[0].word = data_word[i];
    }

    while (spic->sr & BIT_BUSY);

    spic_enable(spic, DISABLE);
    return retval;
}

/**
 * @brief program data to flash via user mode
 * @param start_addr    start address where is going to be written in flash
 * @param data_len      data length to be written
 * @param data          data buffer to be written
 * @return true if success
 * @note start address should be 4 byte align
*/
bool psram_write(uint32_t addr, uint32_t len, uint8_t *data)
{
    const uint32_t blksize = len; //SPIC_FIFO_SIZE - 4;
    uint32_t tmp_addr = convert_psram_addr_to_offset(addr);
    uint8_t *tmp_data = data;
    int remain_size = len;

    spic_enable(spic, DISABLE);
    spic->ctrlr0 &= ~(BIT_TMOD(3)); /* tx mode */

    while (remain_size > 0)
    {
        uint32_t write_size = remain_size > blksize ? blksize : remain_size;

        extern bool psram_write_internal(uint32_t start_addr, uint32_t data_len, uint8_t *data);
        if (!psram_write_internal(tmp_addr, write_size, tmp_data))
        {
            return false;
        }

        tmp_addr += write_size;
        tmp_data += write_size;
        remain_size -= write_size;
    }

    return true;
}

bool psram_user_dma_write(uint32_t start_addr, uint32_t data_len, uint8_t *data)
{
    bool ret = true;
    uint8_t cmd_pp = psram_device_info.curr_wr_cmd;
    uint32_t wr_addr;
    uint32_t flash_dr_addr;
    uint32_t dma_handshake;

    flash_dr_addr = (uint32_t)&SPIC1->dr[0].word;
    dma_handshake = GDMA_Handshake_SPIC1_TX;

    uint32_t tmp_addr = convert_psram_addr_to_offset(start_addr);

    spic_enable(spic, DISABLE);
    spic->addr_length = 3;

    wr_addr = cmd_pp | __REV(tmp_addr);

    /* SPIC TX basic settings */
    spic->ctrlr0 &= ~(BIT_TMOD(3));

    /* set IMR.TXSIM and CTRLR2.SEQ_EN */
    spic->imr |= BIT_TXSIM;
    spic->ctrlr2 |= BIT_SEQ_EN;

    /* push CMD, ADDR to FIFO */
    spic->dr[0].word = rtk_cpu_to_le32(wr_addr);

    spic->dmacr = 2;

    /* change this value can not influence the result. the wave is split into two periods. the first is 32 bytes. */
    spic->dmatdlr = 4; /* no any influence. */

    /* Init GDMA */
    GDMA_InitTypeDef gdma_init;
    uint8_t dma_ch_num = OTP->flash_dma_ch;
    GDMA_ChannelTypeDef *gdma_channel = (GDMA_ChannelTypeDef *)(GDMA_Channel0_BASE +
                                                                dma_ch_num * sizeof(GDMA_ChannelTypeDef));

    GDMA_StructInit(&gdma_init);
    gdma_init.GDMA_ChannelNum           = dma_ch_num;
    gdma_init.GDMA_DIR                  = GDMA_DIR_MemoryToPeripheral;
    gdma_init.GDMA_BufferSize           = data_len / 4; // div by 4 because DataSize use word
    gdma_init.GDMA_SourceInc            = DMA_SourceInc_Inc;
    gdma_init.GDMA_DestinationInc       = DMA_SourceInc_Fix;
    gdma_init.GDMA_SourceDataSize       = GDMA_DataSize_Word;
    gdma_init.GDMA_DestinationDataSize  = GDMA_DataSize_Word;
    gdma_init.GDMA_SourceMsize          = GDMA_Msize_1;            // dependent to SPIC dmardlr
    gdma_init.GDMA_DestinationMsize     = GDMA_Msize_1;            // dependent to SPIC dmardlr
    gdma_init.GDMA_SourceAddr           = (uint32_t)data;
    gdma_init.GDMA_DestinationAddr      = flash_dr_addr;
    gdma_init.GDMA_ChannelPriority      = 3; //channel prority between 0 to 6
    gdma_init.GDMA_SourceHandshake      = 0;
    gdma_init.GDMA_DestHandshake        = dma_handshake;
    GDMA_Init(gdma_channel, &gdma_init);
    gdma_channel->CFG_HIGH |= 0x02;

    GDMA_INTConfig(dma_ch_num, GDMA_INT_Transfer, (FunctionalState)ENABLE);

    GDMA_Cmd(dma_ch_num, (FunctionalState)ENABLE);
    spic_enable(spic, ENABLE);

    while (GDMA_GetTransferINTStatus(dma_ch_num) != SET)
    {
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
    }
    GDMA_ClearINTPendingBit(dma_ch_num, GDMA_INT_Transfer);

    /* set CTRLR2.SEQ_EN = 0, IMR.TXSIM = 0 */
    spic->ctrlr2 &= ~ BIT_SEQ_EN;
    spic->imr &= ~ BIT_TXSIM;

    /* set SSIENR = 0 */
    spic_enable(spic, DISABLE);
    spic->dmacr = 0;
    //spic->flush_fifo = 1;
    //spic->icr = 1;

    return ret;
}

void psram_deepsleep_enable(uint8_t enable)
{
    if (enable)
    {
        spic_cmd_tx(HALF_SLEEP_CMD, 0, NULL);
        platform_delay_us(1);
    }
    else
    {
        spic->ssienr = 0;
        BIT_BAND(spic->valid_cmd, 14) = 1;
        spic->ssienr = 1;

        /* Half Sleep Exit CE# low to CLK setup time: 60ns(min), 8us(max) */
        platform_delay_us(3);

        spic->ssienr = 0;
        BIT_BAND(spic->valid_cmd, 14) = 0;
        spic->ssienr = 1;
    }
}

void psram_deinit(void)
{
    /* clock disable */
    RCC_PeriphClockCmd(APBPeriph_FLASH1, APBPeriph_FLASH1_CLOCK, (FunctionalState)DISABLE);

    /* spic1 enable controller */
    SYSBLKCTRL->u_210.BITS_210.BIT_SOC_FLASH_EN1 = 0;

    /* spic1 master enable */
    SYSBLKCTRL->u_2A8.BITS_2A8.SPIC1_MASTER_EN = 0;
}
