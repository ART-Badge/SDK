/**
*********************************************************************************************************
*               Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     rtl876x_hw_aes.c
* @brief    This file provides all the hw aes firmware functions.
* @details
* @author   eason li
* @date     2016-01-04
* @version  v0.1
*********************************************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include <limits.h>
#include "rtl876x_hw_aes.h"
#include "rtl876x_nvic.h"
#include "rtl876x_gdma.h"
#include "rtl876x_rcc.h"
#include "trace.h"
#include "otp.h"

const uint32_t secure_key[8][4] =
{
    {0xec4174c2, 0x9dfcf091, 0x6d172e6a, 0x652e168e},
    {0xbbd1a437, 0x585860c5, 0x94fcc87b, 0x449a05c1},
    {0x9fd6a050, 0x699d45c1, 0x74c42d11, 0xd9e98af7},
    {0xf77aa23b, 0xe8be11c2, 0x262a9055, 0x3f03b94c},
    {0x3dc3be67, 0xf7f2fad6, 0xb85fafab, 0x93a8a9a7},
    {0x513ed7b9, 0x297caf85, 0xc3f03ecc, 0x4e0231e7},
    {0x3852a810, 0xcc2d2708, 0x41b9cef2, 0x23e445ca},
    {0x1d633436, 0x0edb2fc8, 0x4fd2ce29, 0x26c98913},
};
static bool aes_dma_rx_done = false, aes_dma_tx_done = false;
static uint32_t dma_move_src = 0, dma_move_dst = 0, dma_carried_size = 0;
static bool aes_dma_isMac = false;

bool hw_aes_init(const uint32_t *aesKey, uint32_t *iv, T_HW_AES_MODE work_mode, bool isAes256)
{
    hw_aes_set_clk(true);

    hw_aes_clear();

    if (aesKey)
    {
        HW_AES_SET_IRK(aesKey, (isAes256 ? 8 : 4));
    }
    else
    {
        HW_AES_USE_HIDDEN_KEY(true);
    }

    if (iv)
    {
        uint32_t iv_temp[4] = {0};

        if (work_mode == AES_MODE_CTR)
        {
            iv_temp[0] = iv[2];
            iv_temp[1] = iv[3];
            iv_temp[2] = 0;
            iv_temp[3] = 0;
        }
        else
        {
            iv_temp[0] = iv[0];
            iv_temp[1] = iv[1];
            iv_temp[2] = iv[2];
            iv_temp[3] = iv[3];
        }

        HW_AES_SET_IV(iv_temp);
    }

    HW_AES_SET_256_EN(isAes256);

    HW_AES_SET_AES_MODE(work_mode);

    return true;
}

bool hw_aes_cpu_mode(uint32_t *in, uint32_t *out, uint32_t word_len, bool isEncrypt, bool isMac)
{
    if (!in || !out || word_len < 4 || word_len % 4)
    {
        return false;
    }

    HW_AES_SET_WORK_MODE(CPU_MODE);

    uint32_t output_addr = HW_AES_REG_BASE + HWAES_ENC_DOUT + !isEncrypt * 0x4;

    HW_AES_SET_ENC_EN(isEncrypt);
    HW_AES_SET_DEC_EN(!isEncrypt);

    uint32_t bit_mask = 0;

    (isMac == true) ? (bit_mask = 0x3) : (bit_mask = UINT_MAX);

    for (uint32_t i = 0; i < word_len; i += 4)
    {
        /* input from MSB */
        HW_AES_SET_INPUT_DATA(in[i + 0]);
        HW_AES_SET_INPUT_DATA(in[i + 1]);
        HW_AES_SET_INPUT_DATA(in[i + 2]);
        HW_AES_SET_INPUT_DATA(in[i + 3]);

        uint32_t j = UINT_MAX;

        do
        {
            if (HW_AES_IS_DATA_OUT_READY)
            {
                break;
            }
        }
        while (-- j > 0);

        if (j == 0)
        {
            return false;
        }
        else
        {
            /* output from MSB */
            out[(i + 0) & bit_mask] = HAL_READ32(0, output_addr);
            out[(i + 1) & bit_mask] = HAL_READ32(0, output_addr);
            out[(i + 2) & bit_mask] = HAL_READ32(0, output_addr);
            out[(i + 3) & bit_mask] = HAL_READ32(0, output_addr);
        }
    }

    return true;
}

bool hw_aes_cpu_operate(uint32_t *in, uint32_t *out, uint32_t word_len, bool isEncrypt)
{
    return hw_aes_cpu_mode(in, out, word_len, isEncrypt, false);
}

bool hw_aes_dma_operate(uint32_t *in, uint32_t *out, uint32_t word_len, bool isEncrypt,
                        uint8_t dma_rx_ch_num, uint8_t dma_tx_ch_num, bool isMac)
{
    if (!in || !out || word_len < 4 || word_len % 4)
    {
        return false;
    }

    uint32_t output_addr = 0;

    HW_AES_SET_ENC_EN(isEncrypt);
    HW_AES_SET_DEC_EN(!isEncrypt);

    /* reset done flag */
    hw_aes_set_dma_rx_done(false);
    hw_aes_set_dma_tx_done(false);

    output_addr = HW_AES_REG_BASE + HWAES_ENC_DOUT + !isEncrypt * 0x4;

    /* turn on gdma clock */
    SYSBLKCTRL->u_230.CLK_CTRL |= (BIT16 | BIT17);
    SYSBLKCTRL->u_210.SOC_FUNC_EN |= BIT13;

    HW_AES_SET_WORK_MODE(DMA_MODE);

    aes_dma_isMac = isMac;

    if (word_len > (MAX_DMA_BUF_SZ / 4))
    {
        word_len = MAX_DMA_BUF_SZ / 4;
    }

    GDMA_InitTypeDef GDMA_InitStruct;
    GDMA_StructInit(&GDMA_InitStruct);

    /* GDMA initial for RX channel */
    GDMA_InitStruct.GDMA_ChannelNum          = dma_rx_ch_num;
    GDMA_InitStruct.GDMA_DIR                 = GDMA_DIR_MemoryToPeripheral;
    GDMA_InitStruct.GDMA_BufferSize          = word_len;
    GDMA_InitStruct.GDMA_SourceInc           = DMA_SourceInc_Inc;
    GDMA_InitStruct.GDMA_DestinationInc      = DMA_DestinationInc_Fix;
    GDMA_InitStruct.GDMA_SourceDataSize      = GDMA_DataSize_Word;
    GDMA_InitStruct.GDMA_DestinationDataSize = GDMA_DataSize_Word;
    GDMA_InitStruct.GDMA_SourceMsize         = GDMA_Msize_4;
    GDMA_InitStruct.GDMA_DestinationMsize    = GDMA_Msize_4;
    GDMA_InitStruct.GDMA_SourceAddr          = (uint32_t)in;
    GDMA_InitStruct.GDMA_DestinationAddr     = HW_AES_REG_BASE + HWAES_DATAIN;
    GDMA_InitStruct.GDMA_ChannelPriority     = 0;// it can be 0 to 6 which 6 is highest priority
    GDMA_InitStruct.GDMA_DestHandshake       = HW_AES_RX_DMA_IO_NUM;
    GDMA_Init((GDMA_ChannelTypeDef *)DMA_CH_BASE(dma_rx_ch_num), &GDMA_InitStruct);

    /* GDMA initial for TX channel */
    GDMA_StructInit(&GDMA_InitStruct);
    GDMA_InitStruct.GDMA_ChannelNum          = dma_tx_ch_num;
    GDMA_InitStruct.GDMA_DIR                 = (isMac == true) ? GDMA_DIR_PeripheralToPeripheral :
                                               GDMA_DIR_PeripheralToMemory;
    GDMA_InitStruct.GDMA_BufferSize          = word_len;
    GDMA_InitStruct.GDMA_SourceInc           = DMA_SourceInc_Fix;
    GDMA_InitStruct.GDMA_SourceDataSize      = GDMA_DataSize_Word;
    GDMA_InitStruct.GDMA_DestinationDataSize = GDMA_DataSize_Word;
    if (isMac)
    {
        GDMA_InitStruct.GDMA_SourceMsize         = GDMA_Msize_1;
        GDMA_InitStruct.GDMA_DestinationMsize    = GDMA_Msize_1;
    }
    else
    {
        GDMA_InitStruct.GDMA_SourceMsize         = GDMA_Msize_4;
        GDMA_InitStruct.GDMA_DestinationMsize    = GDMA_Msize_4;
    }
    GDMA_InitStruct.GDMA_SourceAddr          = output_addr;
    GDMA_InitStruct.GDMA_DestinationAddr     = (uint32_t)out;
    GDMA_InitStruct.GDMA_ChannelPriority     = 0;
    GDMA_InitStruct.GDMA_SourceHandshake     = HW_AES_TX_DMA_IO_NUM;
    GDMA_InitStruct.GDMA_DestinationInc      = (isMac == true) ? DMA_DestinationInc_Fix :
                                               DMA_DestinationInc_Inc;
    GDMA_Init((GDMA_ChannelTypeDef *)DMA_CH_BASE(dma_tx_ch_num), &GDMA_InitStruct);

    ((GDMA_ChannelTypeDef *)DMA_CH_BASE(dma_tx_ch_num))->CFG_LOW &= ~(0x03 << 10);
    ((GDMA_ChannelTypeDef *)DMA_CH_BASE(dma_tx_ch_num))->CFG_LOW |= BIT10;

    /* enable RX channel interrupt */
    GDMA_INTConfig(dma_rx_ch_num, GDMA_INT_Transfer, ENABLE);
    /* enable TX channel interrupt */
    GDMA_INTConfig(dma_tx_ch_num, GDMA_INT_Transfer, ENABLE);

    /* set RX channel IRQ */
    NVIC_InitTypeDef nvic_init_struct;
    nvic_init_struct.NVIC_IRQChannel         = (IRQn_Type)DMA_CH_IRQ(dma_rx_ch_num);
    nvic_init_struct.NVIC_IRQChannelCmd      = (FunctionalState)ENABLE;
    nvic_init_struct.NVIC_IRQChannelPriority = 4;
    NVIC_Init(&nvic_init_struct);

    /* set TX channel IRQ */
    nvic_init_struct.NVIC_IRQChannel         = (IRQn_Type)DMA_CH_IRQ(dma_tx_ch_num);
    nvic_init_struct.NVIC_IRQChannelCmd      = (FunctionalState)ENABLE;
    nvic_init_struct.NVIC_IRQChannelPriority = 4;
    NVIC_Init(&nvic_init_struct);

    /* enable DMA Rx */
    GDMA_Cmd(dma_rx_ch_num, ENABLE);
    /* enable DMA Tx */
    GDMA_Cmd(dma_tx_ch_num, ENABLE);

    return true;
}

void check_hwaes_dma_done(void)
{
    if (aes_dma_tx_done && aes_dma_rx_done)
    {
        if (dma_carried_size > MAX_DMA_BUF_SZ)
        {
            dma_carried_size -= MAX_DMA_BUF_SZ;
            dma_move_src += MAX_DMA_BUF_SZ;

            if (dma_carried_size < MAX_DMA_BUF_SZ)
            {
                GDMA_SetBufferSize(HWAES_DMA_RX_CH, dma_carried_size >> 2);
                GDMA_SetBufferSize(HWAES_DMA_TX_CH, dma_carried_size >> 2);
            }
            if (!aes_dma_isMac)
            {
                dma_move_dst += MAX_DMA_BUF_SZ;
                GDMA_SetDestinationAddress(HWAES_DMA_TX_CH, dma_move_dst);
            }
            GDMA_SetSourceAddress(HWAES_DMA_RX_CH, dma_move_src);
            GDMA_Cmd(HWAES_DMA_RX_CH_NUM, (FunctionalState)ENABLE);
            GDMA_Cmd(HWAES_DMA_TX_CH_NUM, (FunctionalState)ENABLE);
        }
        else //Last DMA action
        {
            dma_carried_size = 0;

            return;
        }

        aes_dma_rx_done = false;
        aes_dma_tx_done = false;
    }
}

void GDMA0_Channel3_Handler(void)
{
    GDMA_ClearAllTypeINT(HWAES_DMA_RX_CH_NUM);
    GDMA_Cmd(HWAES_DMA_RX_CH_NUM, DISABLE);
    aes_dma_rx_done = true;
    check_hwaes_dma_done();
}

void GDMA0_Channel2_Handler(void)
{
    GDMA_ClearAllTypeINT(HWAES_DMA_TX_CH_NUM);
    GDMA_Cmd(HWAES_DMA_TX_CH_NUM, DISABLE);
    aes_dma_tx_done = true;
    check_hwaes_dma_done();
}

void hw_aes_set_dma_rx_done(bool isDone)
{
    aes_dma_rx_done = isDone;
}

bool hw_aes_is_dma_rx_done(void)
{
    return aes_dma_rx_done;
}

void hw_aes_set_dma_tx_done(bool isDone)
{
    aes_dma_tx_done = isDone;
}

bool hw_aes_is_dma_tx_done(void)
{
    return aes_dma_tx_done;
}

bool hw_aes_dma_done(void)
{
    return (aes_dma_tx_done && aes_dma_rx_done);
}

void hw_aes_set_dma_move_src(uint32_t src)
{
    dma_move_src = src;
}

void hw_aes_set_dma_move_dst(uint32_t dst)
{
    dma_move_dst = dst;
}

void hw_aes_set_dma_carry_size(uint32_t size)
{
    dma_carried_size = size;
}

bool hw_aes_cmac128_updata(uint32_t *p_in, uint32_t *p_out, uint32_t word_len,
                           const uint32_t *p_aes_key, int mode)
{
    uint32_t p_iv[4] = {0, 0, 0, 0};

    bool result = hw_aes_init((uint32_t *)p_aes_key, p_iv, AES_MODE_CBC, false);

    if (result != true)
    {
        return false;
    }

    if (mode == CPU_MODE)
    {
        result = hw_aes_cpu_mode(p_in, p_out, word_len, true, true);
    }
    else
    {
        if (word_len > 4)
        {
            word_len -= 4;
            hw_aes_set_dma_move_src((uint32_t)p_in);
            hw_aes_set_dma_move_dst((uint32_t)p_out);
            hw_aes_set_dma_carry_size(word_len * 4);

            result = hw_aes_dma_operate(p_in, p_out, word_len, true, HWAES_DMA_RX_CH_NUM, HWAES_DMA_TX_CH_NUM,
                                        true);

            for (uint32_t i = UINT_MAX; i > 0; i --)
            {
                if (hw_aes_dma_done())
                {
                    //AES_PRINT_INFO0("hw aes dec done!");
                    break;
                }
            }

            hw_aes_set_dma_move_src((uint32_t)(p_in + word_len));
            hw_aes_set_dma_move_dst((uint32_t)p_out);
            hw_aes_set_dma_carry_size(4 * 4);

            result = hw_aes_dma_operate(p_in + word_len, p_out, 4, true, HWAES_DMA_RX_CH_NUM,
                                        HWAES_DMA_TX_CH_NUM,
                                        false);

            for (uint32_t i = UINT_MAX; i > 0; i --)
            {
                if (hw_aes_dma_done())
                {
                    //AES_PRINT_INFO0("hw aes dec done!");
                    break;
                }
            }
        }
        else
        {
            hw_aes_set_dma_move_src((uint32_t)(p_in));
            hw_aes_set_dma_move_dst((uint32_t)p_out);
            hw_aes_set_dma_carry_size(word_len * 4);

            result = hw_aes_dma_operate(p_in, p_out, word_len, true, HWAES_DMA_RX_CH_NUM, HWAES_DMA_TX_CH_NUM,
                                        false);

            for (uint32_t i = UINT_MAX; i > 0; i --)
            {
                if (hw_aes_dma_done())
                {
                    //AES_PRINT_INFO0("hw aes dec done!");
                    break;
                }
            }
        }

    }



    return result;
}

extern void padding(unsigned char *lastb, unsigned char *pad, int length);
extern void generate_subkey(unsigned char *key, unsigned char *K1, unsigned char *K2);
extern void leftshift_onebit(unsigned char *input, unsigned char *output);
extern void xor_128(const unsigned char *a, const unsigned char *b, unsigned char *out);
extern const unsigned char const_Rb[16];

void generate_subkey_inverse(unsigned char *key, unsigned char *K1, unsigned
                             char *K2)
{
    unsigned char L[16];
    unsigned char Z[16];
    unsigned char tmp[16];
    int i;

    for (i = 0; i < 16; i++) { Z[i] = 0; }

    hw_aes_init((uint32_t *)key, NULL, AES_MODE_ECB, false);
    hw_aes_cpu_operate((uint32_t *)Z, (uint32_t *)L, 4, true);

    if ((L[0] & 0x80) == 0)   /* If MSB(L) = 0, then K1 = L << 1 */
    {
        leftshift_onebit(L, K1);
    }
    else      /* Else K1 = ( L << 1 ) (+) Rb */
    {
        leftshift_onebit(L, tmp);
        xor_128(tmp, const_Rb, K1);
    }

    if ((K1[0] & 0x80) == 0)
    {
        leftshift_onebit(K1, K2);
    }
    else
    {
        leftshift_onebit(K1, tmp);
        xor_128(tmp, const_Rb, K2);
    }
    /* UNCLEARED_STACK_KEYS[N,Unconfirmed] in progress_update_012318.pdf from riscure */
    for (i = 0; i < 16; i += 4)
    {
        *(uint32_t *)&L[i] = *(uint32_t *)&tmp[i] = 0;
    }

    return;
}

bool aes_cmac_inverse(uint8_t *key, uint8_t *input, uint32_t length, uint8_t *mac, int mode)
{
    unsigned char       X[16] = {0}, Y[16], M_last[16];
    unsigned char       K1[16], K2[16];
    int         n, i, flag;
    generate_subkey_inverse(key, K1, K2);

    n = (length + 15) / 16;       /* n is number of rounds */

    if (n == 0)
    {
        n = 1;
        flag = 0;
    }
    else
    {
        if ((length % 16) == 0)   /* last block is a complete block */
        {
            flag = 1;
        }
        else   /* last block is not complete block */
        {
            flag = 0;
        }
    }

    if (flag)   /* last block is complete block */
    {
        xor_128(&input[16 * (n - 1)], K1, M_last);
    }
    else
    {
        unsigned char padded[16];
        padding(&input[16 * (n - 1)], padded, length % 16);
        xor_128(padded, K2, M_last);
    }

    if (n > 1)
    {
        if (hw_aes_cmac128_updata((uint32_t *)input, (uint32_t *)X, (n - 1) * 4, (uint32_t *)key,
                                  mode) == false)
        {
            return false;
        }
    }

    xor_128(X, M_last, Y);

    hw_aes_init((uint32_t *)key, NULL, AES_MODE_ECB, false);
    hw_aes_cpu_operate((uint32_t *)Y, (uint32_t *)X, 4, true);

    for (i = 0; i < 16; i++)
    {
        mac[i] = X[i];
        X[i] = Y[i] = M_last[i] = K1[i] = K2[i] = 0;
    }
    //uint32_t *cmac=(uint32_t*)mac;DBG_DIRECT("mac=0x%x%x%x%x",cmac[3],cmac[2],cmac[1],cmac[0]);
    return true;
}

uint32_t get_secure_reg_cfg_val(uint32_t in_val)
{
    uint32_t out_val = 0;
    uint32_t reg[4] = {0};
    uint32_t hash[4] = {0};

    reg[0] = in_val;
    hw_aes_init(secure_key[HWAES->SEC_STS.irk_cnt], NULL, AES_MODE_ECB, false);
    hw_aes_cpu_operate(reg, hash, 4, true);
    out_val = ((hash[0] & 0xffff) << 16) | reg[0];

    return out_val;
}
