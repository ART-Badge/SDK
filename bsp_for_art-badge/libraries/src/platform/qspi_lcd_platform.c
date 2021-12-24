#include "qspi_lcd_platform.h"
#include "rtl876x.h"
#include "rtl876x_rcc.h"
#include "rtl876x_pinmux.h"

void qspi_lcd_init(void)
{
    /* module init */
    RCC_PeriphClockCmd(APBPeriph_FLASH2, APBPeriph_FLASH2_CLOCK, (FunctionalState)ENABLE);

    Pinmux_Config(P4_3, IDLE_MODE);
    Pinmux_Config(P4_0, IDLE_MODE);
    Pinmux_Config(P4_2, IDLE_MODE);
    Pinmux_Config(P4_1, IDLE_MODE);
    Pinmux_Config(P0_7, IDLE_MODE);
    Pinmux_Config(P0_6, IDLE_MODE);

    Pad_Config(P4_3, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_UP,   PAD_OUT_DISABLE, PAD_OUT_LOW);
    Pad_Config(P4_1, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_NONE, PAD_OUT_DISABLE, PAD_OUT_LOW);
    Pad_Config(P0_7, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_UP,   PAD_OUT_DISABLE, PAD_OUT_LOW);
    Pad_Config(P4_2, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_UP,   PAD_OUT_DISABLE, PAD_OUT_LOW);
    Pad_Config(P4_0, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_UP,   PAD_OUT_DISABLE, PAD_OUT_LOW);
    Pad_Config(P0_6, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_UP,   PAD_OUT_DISABLE, PAD_OUT_LOW);

    /* spic2 enable controller */
    SYSBLKCTRL->u_210.BITS_210.BIT_SOC_FLASH_EN2 = 1;
    /* spic2 master enable */
    SYSBLKCTRL->u_2A8.BITS_2A8.SPIC2_MASTER_EN = 1;

    /* SPIC2 TX DMA handshake signal: use spic2_dma_tx_req for spic2_dma_tx_single */
    HAL_UPDATE32(0x40000300, BIT31, BIT31);

    /* set cs and baudrate */
    SPIC2->ssienr = 0;
    SPIC2->ser = BIT0;
    SPIC2->baudr = 1;

    /* ctrlr2[17:16] = 2'b11 to remove WREN+RDSR (i.e., only PP cmd issue) */
    SPIC2->ctrlr2 |= (BIT16 | BIT17);
}
