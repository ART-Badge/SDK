/**
*********************************************************************************************************
*               Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* @file      rtl876x_hw_aes.h
* @brief
* @details
* @author    eason li
* @date      2016-01-04
* @version   v0.1
* *********************************************************************************************************
*/

#ifndef __RTL876X_HW_AES_H
#define __RTL876X_HW_AES_H

#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include <stdbool.h>
#include "rtl876x.h"
#include "hw_aes.h"

/**
  * @brief referenc to hw aes register table
  */
typedef struct
{
    union
    {
        __IO uint32_t CTL;
        struct
        {
            __IO uint32_t enc_en:         1; /* aes encryption enable */
            __IO uint32_t dec_en:         1; /* aes decryption enable */
            __IO uint32_t ase256_en:      1; /* aes 256-bit mode enable */
            __IO uint32_t aes_mode_sel:   3; /* aes mode select, see HW_AES_MODE below */
            __IO uint32_t access_mode:    1; /* 0: CPU mode, 1: DMA mode */
            __IO uint32_t scram_en:       1; /* scramble function, 0:disable, 1:enable */
            __IO uint32_t use_hidden_key: 1; /* secure related */
            __IO uint32_t copy_hidden_key: 1; /* secure related */
            __IO uint32_t hidden_256:     1; /* secure related */
            __IO uint32_t poly_en:        1; /* XOR polynomial, 0:disable, 1:enable */
            __IO uint32_t rsvd:           19;
            __IO uint32_t dout_rdy:       1; /* aes data output ready signal used in CPU mode.
                                              when data output is ready, dout_rdy = 1.
                                              if dout_rdy == 1, "CPU reads enc_dout or dec_dout
                                              for 4 times (128 bits)" will clear dout_rdy to 0.
                                            */
        } CTL_BITS;
    };
    __IO uint32_t data_in;
    __I  uint32_t enc_dout;
    __I  uint32_t dec_dout;
    __IO uint32_t IRK[8];
    __IO uint32_t iv[4];
    union
    {
        __O  uint32_t secure_reg[13];
        struct
        {
            __O  uint32_t RPZ0_CMP_ADDR_TOP_H;    /* 0x40 */
            __O  uint32_t RPZ0_CMP_ADDR_TOP_L;    /* 0x44 */
            __O  uint32_t RPZ0_CMP_ADDR_BUTTOM_H; /* 0x48 */
            __O  uint32_t RPZ0_CMP_ADDR_BUTTOM_L; /* 0x4C */
            __O  uint32_t RPZ1_CMP_ADDR_TOP_H;    /* 0x50 */
            __O  uint32_t RPZ1_CMP_ADDR_TOP_L;    /* 0x54 */
            __O  uint32_t RPZ1_CMP_ADDR_BUTTOM_H; /* 0x58 */
            __O  uint32_t RPZ1_CMP_ADDR_BUTTOM_L; /* 0x5C */
            __O  uint32_t RPZ_SWD_CTRL;           /* 0x60 */
            __O  uint32_t DSP_MEM_CTRL_H;         /* 0x64 */
            __O  uint32_t DSP_MEM_CTRL_L;         /* 0x68 */
            __O  uint32_t rsvd;                   /* 0x6C */
            __O  uint32_t reg_buf_cache_sram_en;  /* 0x70 */
        };
    };
    union
    {
        __IO uint32_t secure_status;
        struct
        {
            __IO uint32_t cnt:            4;
            __IO uint32_t S:              1;
            __IO uint32_t irk_cnt:        3;
            __IO uint32_t rsvd:           8;
            __IO uint32_t hash_:          16;
        } SEC_STS;
    };
    __O  uint32_t authentication;
} HW_AES_TypeDef;

#define HWAES_CTL       0x0
#define HWAES_DATAIN    0x4
#define HWAES_ENC_DOUT  0x8
#define HWAES_DEC_DOUT  0xC
#define HWAES_IRK       0x10
#define HWAES_IV        0x30
#define HWAES_SEC_REG   0x40
#define HWAES_SEC_STS   0x74
#define HWAES_AUTHEN    0x78


typedef enum
{
    CPU_MODE,
    DMA_MODE
} HW_AES_ACCESS_MODE;

#define DMA_CH_BASE(ChNum) (GDMA_CHANNEL_REG_BASE + ChNum * 0x0058)
#define DMA_CH_IRQ(ChNum) ((GDMA0_Channel0_IRQn + ChNum))
#define HW_AES_RX_DMA_IO_NUM 14
#define HW_AES_TX_DMA_IO_NUM 13

#define HW_AES_SET_ENC_EN(isEnable)         (HWAES->CTL_BITS.enc_en = isEnable)
#define HW_AES_SET_DEC_EN(isEnable)         (HWAES->CTL_BITS.dec_en = isEnable)
#define HW_AES_SET_256_EN(isEnable)         (HWAES->CTL_BITS.ase256_en = isEnable)
#define HW_AES_GET_256_EN                   (HWAES->CTL_BITS.ase256_en)
#define HW_AES_SET_AES_MODE(mode)           (HWAES->CTL_BITS.aes_mode_sel = (mode & 0x7))
#define HW_AES_SET_WORK_MODE(mode)          (HWAES->CTL_BITS.access_mode = (mode & BIT0))
#define HW_AES_IS_DATA_OUT_READY            (HWAES->CTL_BITS.dout_rdy)
#define HW_AES_SET_INPUT_DATA(data)         (HWAES->data_in = (uint32_t)data)
#define HW_AES_READ_ENC_OUTPUT(Out)         (Out = HWAES->enc_dout)
#define HW_AES_READ_DEC_OUTPUT(Out)         (Out = HWAES->dec_dout)
#define HW_AES_SET_IRK(pIRK, cnt)           for (uint8_t i = 0;i < cnt;i ++) {HWAES->IRK[(cnt - 1) - i] = pIRK[i];}
#define HW_AES_GET_IRK(pIRK, cnt)           for (uint8_t i = 0;i < cnt;i ++) {pIRK[i] = HWAES->IRK[(cnt - 1) - i];}
#define HW_AES_SET_IV(pIV)                  for (uint8_t i = 0;i < 4;i ++) {HWAES->iv[3 - i] = pIV[i];}
#define HW_AES_GET_IV(pIV)                  for (uint8_t i = 0;i < 4;i ++) {pIV[i] = HWAES->iv[3 - i];}
#define HW_AES_SET_SCRAMBLE_EN(isEnable)    (HWAES->CTL_BITS.scram_en = isEnable)
#define HW_AES_USE_HIDDEN_KEY(isEnable)     (HWAES->CTL_BITS.use_hidden_key = isEnable)
#define HW_AES_COPY_HIDDEN_KEY(isEnable)    (HWAES->CTL_BITS.copy_hidden_key = isEnable)
#define HW_AES_SET_HIDDEN_256(isEnable)     (HWAES->CTL_BITS.hidden_256 = isEnable)

#define HWAES_DMA_RX_CH_NUM 3
#define HWAES_DMA_TX_CH_NUM 2
#define HWAES_DMA_RX_CH GDMA_Channel3
#define HWAES_DMA_TX_CH GDMA_Channel2
#define MAX_DMA_BUF_SZ 0xFF0

#if 1
#define AES_INFO(...) DBG_DIRECT(__VA_ARGS__);
#else
#define AES_INFO(...)
#endif
__STATIC_INLINE void hw_aes_clear(void)
{
    HWAES->CTL = 0;
}
__STATIC_INLINE void hw_aes_set_clk(bool is_enable)
{
    /* turn on hw aes clock */
    SYSBLKCTRL->u_238.BITS_238.BIT_SOC_ACTCK_AES_EN = is_enable;

    /* enable hw aes */
    SYSBLKCTRL->u_218.BITS_218.BIT_PERI_AES_EN = is_enable;
}

bool hw_aes_init(const uint32_t *aesKey, uint32_t *iv, T_HW_AES_MODE work_mode, bool isAes256);
bool hw_aes_cpu_operate(uint32_t *in, uint32_t *out, uint32_t word_len, bool isEncrypt);
bool hw_aes_dma_operate(uint32_t *in, uint32_t *out, uint32_t word_len, bool isEncrypt,
                        uint8_t dma_rx_ch_num, uint8_t dma_tx_ch_num, bool isMac);
void hw_aes_set_dma_rx_done(bool isDone);
bool hw_aes_is_dma_rx_done(void);
void hw_aes_set_dma_tx_done(bool isDone);
bool hw_aes_is_dma_tx_done(void);
bool hw_aes_dma_done(void);
void hw_aes_set_dma_move_src(uint32_t src);
void hw_aes_set_dma_move_dst(uint32_t dst);
void hw_aes_set_dma_carry_size(uint32_t size);
bool aes_cmac_inverse(uint8_t *key, uint8_t *input, uint32_t length, uint8_t *mac, int mode);
uint32_t get_secure_reg_cfg_val(uint32_t in_val);

void share_dsp_and_cache_ram_rom(void);

#ifdef __cplusplus
}
#endif
#endif /*__RTL8762X_GDMA_H*/
