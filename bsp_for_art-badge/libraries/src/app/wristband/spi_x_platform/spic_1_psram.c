#include "rtl876x.h"
#include "rtl876x_rcc.h"
#include "rtl876x_pinmux.h"
#include "spic_1_psram.h"
#include "trace.h"
#include "stdlib.h"
#include "string.h"
#include "rtl876x_gdma.h"
#include "rtl876x_nvic.h"
#include "psram_platform.h"
#include "platform_utils.h"
#include "os_mem.h"
#include "os_sched.h"

#define PSRAM_TEST_BUFFER_SZIE  4096*1024
extern uint32_t SystemCpuClock;

bool allowed_psram_enter_dlps = true;

static const T_IO_MODULE_CBS clock_cbs =
{
    spic_1_psram_init,
    spic_1_psram_enter_dpls,
    spic_1_psram_exit_dlps,
    psram_allowed_enter_dlps_check,
};

void spic_1_psram_enter_dpls(void)
{
    //DBG_DIRECT("[PSRAM][DLPS ENTER] spic1 ->psram enter DLPS");
}

void spic_1_psram_exit_dlps(void)
{
    psram_init();
    psram_try_high_speed(FLASH_MODE_4BIT);
    //psram_config(FLASH_MODE_4BIT);
    //DBG_DIRECT("[PSRAM][DLPS EXIT] spic1 ->psram exit DLPS");
}

bool psram_allowed_enter_dlps_check(void)
{
    return allowed_psram_enter_dlps;
}

void spic_1_psram_init(void)
{
    psram_init();
}

void psram_dma_auto_read_write_test(void)
{
    RCC_PeriphClockCmd(APBPeriph_GDMA, APBPeriph_GDMA_CLOCK, ENABLE);
    NVIC_InitTypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel = LCD_DMA_CHANNEL_IRQ;
    NVIC_InitStruct.NVIC_IRQChannelPriority = 3;
    NVIC_InitStruct.NVIC_IRQChannelCmd = DISABLE;
    NVIC_Init(&NVIC_InitStruct);
    /* Initialize GDMA peripheral */
    GDMA_InitTypeDef GDMA_InitStruct;
    GDMA_StructInit(&GDMA_InitStruct);
    GDMA_InitStruct.GDMA_ChannelNum          = LCD_DMA_CHANNEL_NUM;

    GDMA_InitStruct.GDMA_SourceDataSize      = GDMA_DataSize_Word;
    GDMA_InitStruct.GDMA_DestinationDataSize = GDMA_DataSize_Word;
    GDMA_InitStruct.GDMA_SourceMsize         = GDMA_Msize_1;//here must set Msize == 1
    GDMA_InitStruct.GDMA_DestinationMsize    = GDMA_Msize_1;//here must set Msize == 1

    GDMA_InitStruct.GDMA_DIR                 = GDMA_DIR_MemoryToMemory;
    GDMA_InitStruct.GDMA_SourceInc           = DMA_SourceInc_Inc;
    GDMA_InitStruct.GDMA_DestinationInc      = DMA_SourceInc_Inc;
    GDMA_Init(LCD_DMA_CHANNEL_INDEX, &GDMA_InitStruct);
    GDMA_INTConfig(LCD_DMA_CHANNEL_NUM, GDMA_INT_Transfer, ENABLE);

    uint8_t *p_buf = 0;
    /* malloc 1K buffer */
    p_buf = malloc(sizeof(uint8_t) * PSRAM_TEST_BUFFER_SZIE);
    if (p_buf != NULL)
    {
        DBG_DIRECT("malloc pass");
        DBG_DIRECT("malloc ok -> %x", p_buf);
        for (uint32_t i = 0; i < PSRAM_TEST_BUFFER_SZIE; i++)
        {
            *(uint8_t *)(p_buf + i) = i;
        }

        /* inter ram to psram */
        GDMA_SetBufferSize(LCD_DMA_CHANNEL_INDEX, PSRAM_TEST_BUFFER_SZIE / 4);
        GDMA_SetDestinationAddress(LCD_DMA_CHANNEL_INDEX, 0x6800000);
        GDMA_SetSourceAddress(LCD_DMA_CHANNEL_INDEX, (uint32_t)p_buf);
        GDMA_Cmd(LCD_DMA_CHANNEL_NUM, ENABLE);
        uint32_t old_stamp = ((*(uint32_t *)(0x4005817C)) & 0x3FFFFFF);
        while (GDMA_GetTransferINTStatus(LCD_DMA_CHANNEL_NUM) != SET);
        GDMA_ClearINTPendingBit(LCD_DMA_CHANNEL_NUM, GDMA_INT_Transfer);
        uint32_t new_stamp = ((*(uint32_t *)(0x4005817C)) & 0x3FFFFFF);


        if (memcmp(p_buf, (uint8_t *)0x6800000, PSRAM_TEST_BUFFER_SZIE) == 0)
        {
            DBG_DIRECT("psram dma auto write test pass");
            DBG_DIRECT("[psram dma auto write SystemCpuClock = %dM; efficiency=%ld%%]", \
                       SystemCpuClock / 1000000, (PSRAM_TEST_BUFFER_SZIE * 4 * 100) / (new_stamp - old_stamp));
        }
        else
        {
            DBG_DIRECT("psram dma write test fail!");
        }

        platform_delay_ms(10);
        memset(p_buf, 0x00, PSRAM_TEST_BUFFER_SZIE);
        psram_seq_trans_enable(true);
        GDMA_SetBufferSize(LCD_DMA_CHANNEL_INDEX, PSRAM_TEST_BUFFER_SZIE / 4);
        GDMA_SetDestinationAddress(LCD_DMA_CHANNEL_INDEX, (uint32_t)p_buf);
        GDMA_SetSourceAddress(LCD_DMA_CHANNEL_INDEX, 0x6800000);
        GDMA_Cmd(LCD_DMA_CHANNEL_NUM, ENABLE);
        old_stamp = ((*(uint32_t *)(0x4005817C)) & 0x3FFFFFF);
        while (GDMA_GetTransferINTStatus(LCD_DMA_CHANNEL_NUM) != SET);
        GDMA_ClearINTPendingBit(LCD_DMA_CHANNEL_NUM, GDMA_INT_Transfer);
        new_stamp = ((*(uint32_t *)(0x4005817C)) & 0x3FFFFFF);
        psram_seq_trans_enable(false);
        if (memcmp(p_buf, (uint8_t *)0x6800000, PSRAM_TEST_BUFFER_SZIE) == 0)
        {
            DBG_DIRECT("psram dma auto read test pass");
            DBG_DIRECT("[psram dma auto read SystemCpuClock = %dM; efficiency=%ld%%]", \
                       SystemCpuClock / 1000000, (PSRAM_TEST_BUFFER_SZIE * 4 * 100) / (new_stamp - old_stamp));
        }
        else
        {
            DBG_DIRECT("psram dma read test fail!");
        }
    }
    else
    {
        DBG_DIRECT("malloc fail");
        while (1);
    }


    /* free buffer */
    if (p_buf)
    {
        free(p_buf);
        p_buf = NULL;
    }
}

void psram_cpu_user_read_write_test(void)
{

}

void psram_cpu_read_write_test(void)
{

    uint8_t *p_buf = 0;
    /* malloc 1K buffer */
//    p_buf = malloc(sizeof(uint8_t) * PSRAM_TEST_BUFFER_SZIE);
//    if (p_buf != NULL)
//    {
//        DBG_DIRECT("malloc ok");
//        /* do init inter ram*/
//        for (uint32_t i = 0; i < PSRAM_TEST_BUFFER_SZIE; i++)
//        {
//            *(uint8_t *)(p_buf + i) = i;
//        }
        /* do init psram*/
        uint32_t old_stamp = ((*(uint32_t *)(0x4005817C)) & 0x3FFFFFF);
        for (uint32_t i = 0; i < PSRAM_TEST_BUFFER_SZIE; i++)
        {
            *(uint8_t *)(0x6800000 + i) = i;
        }
        uint32_t new_stamp = ((*(uint32_t *)(0x4005817C)) & 0x3FFFFFF);
//        if (memcmp(p_buf, (uint8_t *)0x6800000, PSRAM_TEST_BUFFER_SZIE) == 0)
//        {
				DBG_DIRECT("psram cpu test pass THOMAS %d, %d RATE:%6u KB/s ", new_stamp,old_stamp, (PSRAM_TEST_BUFFER_SZIE) * 1000*(new_stamp - old_stamp)/1024);
            DBG_DIRECT("[psram cpu read SystemCpuClock = %dM; efficiency=%ld%%]", \
                       SystemCpuClock / 1000000, (PSRAM_TEST_BUFFER_SZIE * 4 * 100) / (new_stamp - old_stamp));
					

//        }
//        else
//        {
//            DBG_DIRECT("psram cpu test fail!");
//        }
//    }
//    else
//    {
//        DBG_DIRECT("malloc fail");
//    }


    /* free buffer */
//    if (p_buf)
//    {
//        free(p_buf);
//        p_buf = NULL;
//    }
}

void psram_dma_user_read_write_test(void)
{
    RCC_PeriphClockCmd(APBPeriph_GDMA, APBPeriph_GDMA_CLOCK, ENABLE);
    NVIC_InitTypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel = LCD_DMA_CHANNEL_IRQ;
    NVIC_InitStruct.NVIC_IRQChannelPriority = 3;
    NVIC_InitStruct.NVIC_IRQChannelCmd = DISABLE;
    NVIC_Init(&NVIC_InitStruct);
    /* Initialize GDMA peripheral */
    GDMA_InitTypeDef GDMA_InitStruct;
    GDMA_StructInit(&GDMA_InitStruct);
    GDMA_InitStruct.GDMA_ChannelNum          = LCD_DMA_CHANNEL_NUM;

    GDMA_InitStruct.GDMA_SourceDataSize      = GDMA_DataSize_Word;
    GDMA_InitStruct.GDMA_DestinationDataSize = GDMA_DataSize_Word;
    GDMA_InitStruct.GDMA_SourceMsize         = GDMA_Msize_1;
    GDMA_InitStruct.GDMA_DestinationMsize    = GDMA_Msize_1;

    GDMA_InitStruct.GDMA_DestHandshake       = GDMA_Handshake_SPIC1_TX;
    GDMA_InitStruct.GDMA_DIR                 = GDMA_DIR_MemoryToPeripheral;
    GDMA_InitStruct.GDMA_SourceInc           = DMA_SourceInc_Inc;
    GDMA_InitStruct.GDMA_DestinationInc      = DMA_DestinationInc_Fix;
    GDMA_Init(LCD_DMA_CHANNEL_INDEX, &GDMA_InitStruct);
    GDMA_INTConfig(LCD_DMA_CHANNEL_NUM, GDMA_INT_Transfer, ENABLE);

    uint8_t *p_buf = 0;
    /* malloc 1K buffer */
    p_buf = malloc(sizeof(uint8_t) * (PSRAM_TEST_BUFFER_SZIE));
    if (p_buf != NULL)
    {
        DBG_DIRECT("malloc ok -> %x", p_buf);
        for (uint32_t i = 0; i < PSRAM_TEST_BUFFER_SZIE; i++)
        {
            *(uint8_t *)(p_buf + i) = i;
        }

        SPIC1->ssienr = DISABLE;
        //SPIC1->ctrlr0 &= ~(BIT_TMOD(3)); /* tx mode */
        SPIC1->ctrlr0 &= ~(BIT8 | BIT9); /* tx mode */
        /* set IMR.TXSIM and CTRLR2.SEQ_EN */
        SPIC1->imr |= BIT10;        //BIT_RXSIM;
        SPIC1->ctrlr2 |= BIT3;      //BIT_SEQ_EN; set this cs alway low

        SPIC1->ctrlr2 |= (0x00000001 << 3); //BIT_SEQ_EN
        SPIC1->dr[0].word = 0x38 | __REV(0);
        SPIC1->dmacr = 2;
        SPIC1->dmatdlr = 4; // no use this register
        SPIC1->ssienr = ENABLE;

        /* inter ram to psram */
        GDMA_SetBufferSize(LCD_DMA_CHANNEL_INDEX, PSRAM_TEST_BUFFER_SZIE / 4);
        GDMA_SetSourceAddress(LCD_DMA_CHANNEL_INDEX, (uint32_t)p_buf);
        GDMA_SetDestinationAddress(LCD_DMA_CHANNEL_INDEX, (uint32_t)(&(SPIC1->dr[0].word)));
        GDMA_Cmd(LCD_DMA_CHANNEL_NUM, ENABLE);

        uint32_t old_stamp = ((*(uint32_t *)(0x4005817C)) & 0x3FFFFFF);
        while (GDMA_GetTransferINTStatus(LCD_DMA_CHANNEL_NUM) != SET);
        GDMA_ClearINTPendingBit(LCD_DMA_CHANNEL_NUM, GDMA_INT_Transfer);
        SPIC1->ssienr = DISABLE;
        uint32_t new_stamp = ((*(uint32_t *)(0x4005817C)) & 0x3FFFFFF);
     DBG_DIRECT("psram dma user write oldl pass:%d", os_sys_tick_get());

        if (memcmp(p_buf, (uint8_t *)0x6800000, PSRAM_TEST_BUFFER_SZIE) == 0)
        {
					DBG_DIRECT("psram dma user write test pass:%d", os_sys_tick_get());
            DBG_DIRECT("[psram dma user write SystemCpuClock = %dM; efficiency=%ld%%]", \
                       SystemCpuClock / 1000000, (PSRAM_TEST_BUFFER_SZIE * 4 * 100) / (new_stamp - old_stamp));
        }
        else
        {
            DBG_DIRECT("psram dma write test fail!");
        }
    }
    else
    {
        DBG_DIRECT("malloc fail");
        return;
    }


    /* free buffer */
    if (p_buf)
    {
        free(p_buf);
        p_buf = NULL;
    }
}

void psram_test_code(void)
{
    psram_init();
    psram_try_high_speed(FLASH_MODE_4BIT);
    *(uint32_t *)0x6800000 = 0x12345678;
    DBG_DIRECT("VALUE = 0x%x", *(uint32_t volatile *)0x6800000);
    *(uint32_t *)0x6800000 = 0x11111111;
    DBG_DIRECT("VALUE = 0x%x", *(uint32_t volatile *)0x6800000);

    memcpy((uint8_t *)0x6800010, (uint8_t *)0x6800000, 4);

    DBG_DIRECT("VALUE = 0x%x", *(uint32_t volatile *)0x6800010);

//    psram_cpu_read_write_test();
//    psram_dma_auto_read_write_test();
//    psram_dma_user_read_write_test();


}

void psram_add_hub_task(void)
{
    spic_1_psram_init();
    driver_modules[wristband_sensor_hub_count++] = (T_IO_MODULE_CBS *)&clock_cbs;
    psram_test_code();
}



