#include "board.h"
#include "trace.h"
#include "string.h"
#include "os_timer.h"
#include "os_mem.h"
#include "os_sync.h"
#include "rtl876x_rcc.h"
#include "rtl876x_tim.h"
#include "rtl876x_if8080.h"
#include "rtl876x_gpio.h"
#include "rtl876x_pinmux.h"
#include "rtl876x_gdma.h"
#include "rtl876x_nvic.h"

#include "module_lcd_8080.h"
#include "module_font.h"
#include "app_section.h"
#include "flash_device.h"
#include "lcd_rm69330_390_qspi.h"
#include "lcd_rm69330_454_qspi.h"
#include "lcd_STUnknow_320_qspi.h"
#include "platform_utils.h"
#include "app_msg.h"
#include "app_task.h"
#include "hub_task.h"
#include "flash_adv_cfg.h"
#include "spic_2_qspi.h"


void GDMA_Config_LLIStruct(uint32_t g1_addr, uint32_t g2_addr, \
                           uint32_t offset, \
                           GDMA_InitTypeDef *GDMA_InitStruct);


GDMA_LLIDef *GDMA_LLIStruct = NULL;

void lcd_set_reset(bool reset)
{
    if (reset)
    {
        Pad_Config(LCD_8080_RST, PAD_SW_MODE, PAD_IS_PWRON, PAD_PULL_DOWN, PAD_OUT_ENABLE, PAD_OUT_LOW);
    }
    else
    {
        Pad_Config(LCD_8080_RST, PAD_SW_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_ENABLE, PAD_OUT_HIGH);
    }
}

/**
  * @brief  wristband driver init
  * @param  None
  * @retval None
  */
void lcd_driver_init(void)
{
    lcd_device_init();
    lcd_set_reset(true);
    platform_delay_ms(120);
    lcd_set_reset(false);
    platform_delay_ms(50);
    lcd_te_device_init();


//    user_lcd_power_on_cb = lcd_rm69330_power_on;
//    user_lcd_power_off_cb = lcd_rm69330_power_off;
//    user_lcd_set_window_cb = lcd_rm69330_set_window;
//    user_lcd_init_cb = lcd_rm69330_init;

    user_lcd_power_on_cb = lcd_rm69330_qspi_390_power_on;
    user_lcd_power_off_cb = lcd_rm69330_qspi_390_power_off;
    user_lcd_set_window_cb = lcd_rm69330_qspi_390_set_window;
    user_lcd_init_cb = lcd_rm69330_qspi_390_init;

//    user_lcd_power_on_cb = lcd_STUnknow_qspi_320_power_on;
//    user_lcd_power_off_cb = lcd_STUnknow_qspi_320_power_off;
//    user_lcd_set_window_cb = lcd_STUnknow_qspi_320_set_window;
//    user_lcd_init_cb = lcd_STUnknow_qspi_320_init;

    user_lcd_te_enable_cb = lcd_te_enable;
    user_lcd_te_disable_cb = lcd_te_disable;
    user_lcd_clear_cb = lcd_clear;
    user_dma_single_block_init_cb = lcd_dma_single_block_init;
    user_dma_single_block_start_cb = lcd_dma_single_block_start;
    user_wait_dma_transfer_cb = lcd_wait_dma_transfer;
    user_wait_lcd_transfer_cb = lcd_wait_lcd_control_transfer;
    user_dma_set_multi_block_for_x_cb = lcd_dma_set_multi_block_for_x;
    user_dma_set_multi_block_for_y_cb = lcd_dma_set_multi_block_for_y;
    user_lcd_get_font_dot_cb = get_font_dot;
    user_show_char_cb = lcd_show_char;

}

void lcd_te_device_init(void)
{
    Pinmux_Config(LCD_FRAME_SYNC, DWGPIO);
    Pad_Config(LCD_FRAME_SYNC, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_NONE, PAD_OUT_DISABLE,
               PAD_OUT_LOW);
    /* Enable GPIO */
    RCC_PeriphClockCmd(APBPeriph_GPIO,  APBPeriph_GPIO_CLOCK,  ENABLE);

    /* Initialize GPIO as interrupt mode */
    GPIO_InitTypeDef GPIO_Param;
    GPIO_StructInit(&GPIO_Param);
    GPIO_Param.GPIO_Pin = GPIO_GetPin(LCD_FRAME_SYNC);
    GPIO_Param.GPIO_Mode = GPIO_Mode_IN;
    GPIO_Param.GPIO_ITCmd = ENABLE;
    GPIO_Param.GPIO_ITTrigger = GPIO_INT_Trigger_EDGE;
    GPIO_Param.GPIO_ITPolarity = GPIO_INT_POLARITY_ACTIVE_LOW;
    GPIO_Init(&GPIO_Param);


    GPIO_INTConfig(GPIO_GetPin(LCD_FRAME_SYNC), DISABLE);
    GPIO_MaskINTConfig(GPIO_GetPin(LCD_FRAME_SYNC), ENABLE);
    GPIO_ClearINTPendingBit(GPIO_GetPin(LCD_FRAME_SYNC));

    user_lcd_te_enable_cb = lcd_te_enable;
    user_lcd_te_disable_cb = lcd_te_disable;
    gui_set_te_function(true);

    NVIC_InitTypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel = GPIO_FRAME_SYNC_IRQ;
    NVIC_InitStruct.NVIC_IRQChannelPriority = 3;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);
}

void lcd_frame_sync_handler(void)
{
    lcd_te_disable();

    T_IO_MSG lcd_sync_msg;
    lcd_sync_msg.type = HUB_MSG_LCD;
    lcd_sync_msg.subtype = LCD_MSG_FRAME_SYNC;
    send_msg_to_ui_task(&lcd_sync_msg, __LINE__);
}
void lcd_te_enable(void)
{
    GPIO_INTConfig(GPIO_GetPin(LCD_FRAME_SYNC), ENABLE);
    GPIO_MaskINTConfig(GPIO_GetPin(LCD_FRAME_SYNC), DISABLE);
}

void lcd_te_disable(void)
{
    GPIO_INTConfig(GPIO_GetPin(LCD_FRAME_SYNC), DISABLE);
    GPIO_MaskINTConfig(GPIO_GetPin(LCD_FRAME_SYNC), ENABLE);
    GPIO_ClearINTPendingBit(GPIO_GetPin(LCD_FRAME_SYNC));
}

void lcd_pad_init(void)
{
    Pinmux_Config(P4_3, IDLE_MODE); //CS
    Pinmux_Config(P4_0, IDLE_MODE); //CLK
    Pinmux_Config(P4_2, IDLE_MODE); //D0
    Pinmux_Config(P4_1, IDLE_MODE); //D1
    Pinmux_Config(P0_6, IDLE_MODE); //D2
    Pinmux_Config(P0_7, IDLE_MODE); //D3

    Pad_Config(P4_3, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_NONE, PAD_OUT_DISABLE, PAD_OUT_LOW);
    Pad_Config(P4_0, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_NONE, PAD_OUT_DISABLE, PAD_OUT_LOW);
    Pad_Config(P4_2, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_NONE, PAD_OUT_DISABLE, PAD_OUT_LOW);
    Pad_Config(P4_1, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_NONE, PAD_OUT_DISABLE, PAD_OUT_LOW);
    Pad_Config(P0_7, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_NONE, PAD_OUT_DISABLE, PAD_OUT_LOW);
    Pad_Config(P0_6, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_NONE, PAD_OUT_DISABLE, PAD_OUT_LOW);

    /*BL AND RESET ARE NOT FIX*/
    /*BL AND RESET ARE NOT FIX*/

    Pad_Config(QSPI_LCD_POWER, PAD_SW_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_ENABLE, PAD_OUT_HIGH);

}
void lcd_enter_dlps(void)
{
    //DBG_DIRECT("[QSPI][DLPS ENTER] spic2 ->qspi enter DLPS");
    return;
}
void lcd_exit_dlps(void)
{
    lcd_device_init();
    //DBG_DIRECT("[QSPI][DLPS EXIT] spic2 ->qspi exit DLPS");
    //Pad_Config(P4_3, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_NONE, PAD_OUT_DISABLE, PAD_OUT_LOW);
    //Pad_Config(P4_0, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_NONE, PAD_OUT_DISABLE, PAD_OUT_LOW);
    //Pad_Config(P4_2, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_NONE, PAD_OUT_DISABLE, PAD_OUT_LOW);
    //Pad_Config(P4_1, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_NONE, PAD_OUT_DISABLE, PAD_OUT_LOW);
    //Pad_Config(P0_7, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_NONE, PAD_OUT_DISABLE, PAD_OUT_LOW);
    //Pad_Config(P0_6, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_NONE, PAD_OUT_DISABLE, PAD_OUT_LOW);
    //Pad_Config(QSPI_LCD_POWER, PAD_SW_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_ENABLE, PAD_OUT_HIGH);
}


/**
  * @brief  writband lcd device init set IO config here
  * @param  None
  * @retval None
  */
void lcd_device_init(void)
{
    /* Open timer clock */
    RCC_PeriphClockCmd(APBPeriph_TIMER, APBPeriph_TIMER_CLOCK, ENABLE);

    TIM_TimeBaseInitTypeDef TIM_InitStruct;
    TIM_StructInit(&TIM_InitStruct);
    TIM_InitStruct.TIM_PWM_En = PWM_ENABLE;
    /* Set period */
    TIM_InitStruct.TIM_Period = 10 - 1 ;
    /* Set PWM high count and low count */
    TIM_InitStruct.TIM_PWM_High_Count = 900 - 1 ;
    TIM_InitStruct.TIM_PWM_Low_Count = 100 - 1 ;
    /* Set timer mode User_define */
    TIM_InitStruct.TIM_Mode = TIM_Mode_UserDefine;
    /* Set timer clock source divider 40, timer pclk = 40M/4 */
    TIM_InitStruct.TIM_SOURCE_DIV = TIM_CLOCK_DIVIDER_4;
    TIM_TimeBaseInit(BL_PWM_TIM, &TIM_InitStruct);

    TIM_Cmd(BL_PWM_TIM, DISABLE);


    lcd_pad_init();
    //do qspi init here
    spic_2_qspi_init();
    RCC_PeriphClockCmd(APBPeriph_IF8080, APBPeriph_IF8080_CLOCK, DISABLE);
    RCC_PeriphClockCmd(APBPeriph_IF8080, APBPeriph_IF8080_CLOCK, ENABLE);
}

void lcd_set_backlight(uint32_t percent)
{
    Pad_Config(QSPI_LCD_POWER, PAD_SW_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_ENABLE, PAD_OUT_HIGH);
    return;
}


static void lcd_dma_clear(uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd,
                          uint16_t color)
{
#if 1
    static uint32_t color_buf = 0;
    color_buf = (color >> 8) | (color << 8);
    color_buf = color_buf | color_buf << 16;
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

    GDMA_InitStruct.GDMA_DIR                 = GDMA_DIR_MemoryToPeripheral;
    GDMA_InitStruct.GDMA_SourceInc           = DMA_SourceInc_Fix;
    GDMA_InitStruct.GDMA_DestinationInc      = DMA_DestinationInc_Fix;
    GDMA_InitStruct.GDMA_DestHandshake       = GDMA_Handshake_8080_TX;

    GDMA_Init(LCD_DMA_CHANNEL_INDEX, &GDMA_InitStruct);
    GDMA_INTConfig(LCD_DMA_CHANNEL_NUM, GDMA_INT_Transfer, ENABLE);

    rtl_gui_lcd_sectionconfig(xStart, yStart, xEnd, yEnd);
    for (uint8_t i = 0; i < TOTAL_SECTION_COUNT - 1; i++)
    {
        lcd_dma_single_block_start(NULL, (uint32_t)&color_buf,
                                   LCD_WIDTH * LCD_SECTION_HEIGHT * PIXEL_BYTES);
        lcd_wait_dma_transfer();
    }
    uint32_t last_len = 0;
    if (LCD_HIGHT % LCD_SECTION_HEIGHT == 0)
    {
        last_len = LCD_SECTION_HEIGHT * LCD_WIDTH * PIXEL_BYTES;
    }
    else
    {
        last_len = (LCD_HIGHT % LCD_SECTION_HEIGHT) * LCD_WIDTH * PIXEL_BYTES;
    }
    lcd_dma_single_block_start(NULL, (uint32_t)&color_buf, last_len);
    lcd_wait_dma_transfer();
#else
    color = RED;
    platform_delay_ms(10);
    uint32_t tmp_color = 0;
    tmp_color = (color >> 8) | (color << 8);
    tmp_color = tmp_color | tmp_color << 16;

    rtl_gui_lcd_sectionconfig(xStart, yStart, xEnd, yEnd);
    for (uint32_t i = 0; i < (LCD_WIDTH * LCD_HIGHT * 2) / 4; i++)
    {
        while ((SPIC2->sr & BIT1) == 0x00);
        SPIC2->dr[0].word = tmp_color;
    }
    SPIC2->ssienr = 0x00;//disable SPIC2
#endif

}


void lcd_clear(uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd, uint16_t color)
{
    lcd_dma_clear(xStart, yStart, xEnd, yEnd, color);
}

void lcd_dma_single_block_init(uint32_t dir_type)
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

    if (dir_type == GDMA_DIR_MemoryToMemory)
    {
        GDMA_InitStruct.GDMA_DIR                 = dir_type;
        GDMA_InitStruct.GDMA_SourceInc           = DMA_SourceInc_Inc;
        GDMA_InitStruct.GDMA_DestinationInc      = DMA_DestinationInc_Inc;
    }
    else if (dir_type == GDMA_DIR_MemoryToPeripheral)
    {
        GDMA_InitStruct.GDMA_DestHandshake       = GDMA_Handshake_8080_TX;
        GDMA_InitStruct.GDMA_DIR                 = dir_type;
        GDMA_InitStruct.GDMA_SourceInc           = DMA_SourceInc_Inc;
        GDMA_InitStruct.GDMA_DestinationInc      = DMA_DestinationInc_Fix;
    }

    GDMA_Init(LCD_DMA_CHANNEL_INDEX, &GDMA_InitStruct);
    GDMA_INTConfig(LCD_DMA_CHANNEL_NUM, GDMA_INT_Transfer, ENABLE);
}


void lcd_dma_single_block_start(uint32_t destination_addr, uint32_t source_addr, uint32_t len)
{
    GDMA_SetBufferSize(LCD_DMA_CHANNEL_INDEX, len >> 2);
    if (destination_addr != 0)
    {
        GDMA_SetDestinationAddress(LCD_DMA_CHANNEL_INDEX, destination_addr);
    }
    else
    {
        GDMA_SetDestinationAddress(LCD_DMA_CHANNEL_INDEX, (uint32_t)(&(SPIC2->dr[0].word)));
    }
    GDMA_SetSourceAddress(LCD_DMA_CHANNEL_INDEX, (uint32_t)(source_addr));
    GDMA_Cmd(LCD_DMA_CHANNEL_NUM, ENABLE);
}

void lcd_wait_dma_transfer(void)
{
    while (GDMA_GetTransferINTStatus(LCD_DMA_CHANNEL_NUM) != SET);
    GDMA_ClearINTPendingBit(LCD_DMA_CHANNEL_NUM, GDMA_INT_Transfer);
}

void lcd_wait_lcd_control_transfer(uint32_t count)
{
    while (GDMA_GetTransferINTStatus(LCD_DMA_CHANNEL_NUM) != SET);
    GDMA_ClearINTPendingBit(LCD_DMA_CHANNEL_NUM, GDMA_INT_Transfer);
    return; //return for qspi
}



void lcd_dma_set_multi_block_for_y(uint32_t picture0_addr, uint32_t picture1_addr,
                                   uint32_t offset,
                                   uint32_t yStart, uint32_t yEnd)//offset mean left menu not display in current menu
{
    GDMA_LLIStruct = (GDMA_LLIDef *)dma_link_list_buff;
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
    GDMA_InitStruct.GDMA_DIR                 = GDMA_DIR_MemoryToPeripheral;
    GDMA_InitStruct.GDMA_BufferSize          = LCD_WIDTH * PIXEL_BYTES / 4;
    GDMA_InitStruct.GDMA_SourceInc           = DMA_SourceInc_Inc;
    GDMA_InitStruct.GDMA_DestinationInc      = DMA_DestinationInc_Fix;
    GDMA_InitStruct.GDMA_SourceDataSize      = GDMA_DataSize_Word;
    GDMA_InitStruct.GDMA_DestinationDataSize = GDMA_DataSize_Word;
    GDMA_InitStruct.GDMA_SourceMsize         = GDMA_Msize_1;
    GDMA_InitStruct.GDMA_DestinationMsize    = GDMA_Msize_1;
    GDMA_InitStruct.GDMA_SourceAddr          = 0;
    GDMA_InitStruct.GDMA_DestinationAddr     = (uint32_t)(&(SPIC2->dr[0].word));
    GDMA_InitStruct.GDMA_DestHandshake       = GDMA_Handshake_SPIC2_TX;

    GDMA_InitStruct.GDMA_Multi_Block_Mode   = LLI_TRANSFER;//LLI_TRANSFER;
    GDMA_InitStruct.GDMA_Multi_Block_En     = 1;
    GDMA_InitStruct.GDMA_Multi_Block_Struct = (uint32_t)GDMA_LLIStruct;


    for (uint32_t i = 0; i < LCD_HIGHT; i++)
    {
        GDMA_LLIStruct[i].DAR = (uint32_t)(&(SPIC2->dr[0].word));
        GDMA_LLIStruct[i].LLP = (uint32_t)&GDMA_LLIStruct[i + 1];
        /* Configure low 32 bit of CTL register */
        GDMA_LLIStruct[i].CTL_LOW = BIT(0)
                                    | (GDMA_InitStruct.GDMA_DestinationDataSize << 1)
                                    | (GDMA_InitStruct.GDMA_SourceDataSize << 4)
                                    | (GDMA_InitStruct.GDMA_DestinationInc << 7)
                                    | (GDMA_InitStruct.GDMA_SourceInc << 9)
                                    | (GDMA_InitStruct.GDMA_DestinationMsize << 11)
                                    | (GDMA_InitStruct.GDMA_SourceMsize << 14)
                                    | (GDMA_InitStruct.GDMA_DIR << 20)
                                    | (GDMA_InitStruct.GDMA_Multi_Block_Mode & LLP_SELECTED_BIT);
        /* Configure high 32 bit of CTL register */
        GDMA_LLIStruct[i].CTL_HIGH = LCD_WIDTH * PIXEL_BYTES / 4;
    }
    for (uint32_t i = 0; i < LCD_HIGHT; i++)
    {
        if (i < (LCD_HIGHT - offset))
        {
            GDMA_LLIStruct[i].SAR = picture0_addr + offset * PIXEL_BYTES * LCD_WIDTH + i * PIXEL_BYTES *
                                    LCD_WIDTH;
        }
        else
        {
            GDMA_LLIStruct[i].SAR = picture1_addr + (i + offset - LCD_HIGHT) * PIXEL_BYTES *
                                    LCD_WIDTH;
        }
    }
    GDMA_LLIStruct[LCD_HIGHT - 1].LLP = 0;
    GDMA_LLIStruct[LCD_HIGHT - 1].CTL_LOW = BIT(0)
                                            | (GDMA_InitStruct.GDMA_DestinationDataSize << 1)
                                            | (GDMA_InitStruct.GDMA_SourceDataSize << 4)
                                            | (GDMA_InitStruct.GDMA_DestinationInc << 7)
                                            | (GDMA_InitStruct.GDMA_SourceInc << 9)
                                            | (GDMA_InitStruct.GDMA_DestinationMsize << 11)
                                            | (GDMA_InitStruct.GDMA_SourceMsize << 14)
                                            | (GDMA_InitStruct.GDMA_DIR << 20);


    GDMA_Init(LCD_DMA_CHANNEL_INDEX, &GDMA_InitStruct);
    GDMA_INTConfig(LCD_DMA_CHANNEL_NUM, GDMA_INT_Transfer, ENABLE);
    GDMA_Cmd(LCD_DMA_CHANNEL_NUM, ENABLE);
}

void lcd_dma_set_multi_block_for_x(uint32_t picture0_addr, uint32_t picture1_addr,
                                   uint32_t offset,
                                   uint32_t yStart, uint32_t yEnd)//offset mean up menu not display in current menu
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
    GDMA_InitStruct.GDMA_DIR                 = GDMA_DIR_MemoryToPeripheral;
    GDMA_InitStruct.GDMA_BufferSize          = LCD_WIDTH * PIXEL_BYTES / 4;
    GDMA_InitStruct.GDMA_SourceInc           = DMA_SourceInc_Inc;
    GDMA_InitStruct.GDMA_DestinationInc      = DMA_DestinationInc_Fix;
    GDMA_InitStruct.GDMA_SourceDataSize      = GDMA_DataSize_Word;
    GDMA_InitStruct.GDMA_DestinationDataSize = GDMA_DataSize_Word;
    GDMA_InitStruct.GDMA_SourceMsize         = GDMA_Msize_1;
    GDMA_InitStruct.GDMA_DestinationMsize    = GDMA_Msize_1;
    GDMA_InitStruct.GDMA_SourceAddr          = 0;
    GDMA_InitStruct.GDMA_DestinationAddr     = (uint32_t)(&(SPIC2->dr[0].word));
    GDMA_InitStruct.GDMA_DestHandshake       = GDMA_Handshake_SPIC2_TX;

    GDMA_InitStruct.GDMA_Multi_Block_Mode   = LLI_TRANSFER;//LLI_TRANSFER;
    GDMA_InitStruct.GDMA_Multi_Block_En     = 1;

    GDMA_InitStruct.GDMA_Multi_Block_Struct  = (uint32_t)IF8080_LLI_REG1_GDMA_BASE;


    offset = (offset >> 1) << 1;


    GDMA_Init(LCD_DMA_CHANNEL_INDEX, &GDMA_InitStruct);

    GDMA_Config_LLIStruct(picture0_addr, picture1_addr, offset, &GDMA_InitStruct);

    GDMA_INTConfig(LCD_DMA_CHANNEL_NUM, GDMA_INT_Transfer, ENABLE);
    GDMA_Cmd(LCD_DMA_CHANNEL_NUM, ENABLE);
}

static GDMA_LLIDef GDMA_LLIStruct_LAST;
void GDMA_Config_LLIStruct(uint32_t g1_addr, uint32_t g2_addr, \
                           uint32_t offset, \
                           GDMA_InitTypeDef *GDMA_InitStruct)//offset mean up menu not display in current menu
{
    GDMA_LLIDef GDMA_LLIStruct_G1;
    GDMA_LLIDef GDMA_LLIStruct_G2;

    IF8080_GDMALLIOFTTypeDef GDMA_LLIStruct_G1_oft;
    IF8080_GDMALLIOFTTypeDef GDMA_LLIStruct_G2_oft;

    GDMA_LLIStruct_G1_oft.SAR_OFT = LCD_WIDTH * PIXEL_BYTES;
    GDMA_LLIStruct_G1_oft.DAR_OFT = 0;
    GDMA_LLIStruct_G2_oft.SAR_OFT = LCD_WIDTH * PIXEL_BYTES;
    GDMA_LLIStruct_G2_oft.DAR_OFT = 0;

    GDMA_LLIStruct_G1.SAR = (uint32_t)(g1_addr + offset * PIXEL_BYTES);
    GDMA_LLIStruct_G1.DAR = (uint32_t)(&(SPIC2->dr[0].word));
    GDMA_LLIStruct_G1.LLP = 0;
    /* configure low 32 bit of CTL register */
    GDMA_LLIStruct_G1.CTL_LOW = BIT(0)
                                | (GDMA_InitStruct->GDMA_DestinationDataSize << 1)
                                | (GDMA_InitStruct->GDMA_SourceDataSize << 4)
                                | (GDMA_InitStruct->GDMA_DestinationInc << 7)
                                | (GDMA_InitStruct->GDMA_SourceInc << 9)
                                | (GDMA_InitStruct->GDMA_DestinationMsize << 11)
                                | (GDMA_InitStruct->GDMA_SourceMsize << 14)
                                | (GDMA_InitStruct->GDMA_DIR << 20)
                                | (GDMA_InitStruct->GDMA_Multi_Block_Mode & LLP_SELECTED_BIT);
    /* configure high 32 bit of CTL register */ /* this means buffer size */
    GDMA_LLIStruct_G1.CTL_HIGH = LCD_WIDTH * PIXEL_BYTES / 4 - offset * PIXEL_BYTES / 4;

    GDMA_LLIStruct_G2.SAR = (uint32_t)(g2_addr);
    GDMA_LLIStruct_G2.DAR = (uint32_t)(&(SPIC2->dr[0].word));
    GDMA_LLIStruct_G2.LLP = 0;
    /* configure low 32 bit of CTL register */
    GDMA_LLIStruct_G2.CTL_LOW = BIT(0)
                                | (GDMA_InitStruct->GDMA_DestinationDataSize << 1)
                                | (GDMA_InitStruct->GDMA_SourceDataSize << 4)
                                | (GDMA_InitStruct->GDMA_DestinationInc << 7)
                                | (GDMA_InitStruct->GDMA_SourceInc << 9)
                                | (GDMA_InitStruct->GDMA_DestinationMsize << 11)
                                | (GDMA_InitStruct->GDMA_SourceMsize << 14)
                                | (GDMA_InitStruct->GDMA_DIR << 20)
                                | (GDMA_InitStruct->GDMA_Multi_Block_Mode & LLP_SELECTED_BIT);
    /* configure high 32 bit of CTL register */
    GDMA_LLIStruct_G2.CTL_HIGH = offset * PIXEL_BYTES / 4;

    GDMA_LLIStruct_LAST.LLP = 0;
    GDMA_LLIStruct_LAST.SAR = (uint32_t)(g2_addr + LCD_WIDTH * (LCD_HIGHT - 1) * PIXEL_BYTES);
    GDMA_LLIStruct_LAST.DAR = (uint32_t)(&(SPIC2->dr[0].word));
    /* configure low 32 bit of CTL register */
    GDMA_LLIStruct_LAST.CTL_LOW = BIT(0)
                                  | (GDMA_InitStruct->GDMA_DestinationDataSize << 1)
                                  | (GDMA_InitStruct->GDMA_SourceDataSize << 4)
                                  | (GDMA_InitStruct->GDMA_DestinationInc << 7)
                                  | (GDMA_InitStruct->GDMA_SourceInc << 9)
                                  | (GDMA_InitStruct->GDMA_DestinationMsize << 11)
                                  | (GDMA_InitStruct->GDMA_SourceMsize << 14)
                                  | (GDMA_InitStruct->GDMA_DIR << 20);
    /* configure high 32 bit of CTL register */
    GDMA_LLIStruct_LAST.CTL_HIGH = offset * PIXEL_BYTES / 4;

    IF8080_GDMALLIConfig((IF8080_GDMALLITypeDef *)(&GDMA_LLIStruct_G1),
                         (IF8080_GDMALLITypeDef *)(&GDMA_LLIStruct_G2),
                         (IF8080_GDMALLIOFTTypeDef *)(&GDMA_LLIStruct_G1_oft),
                         (IF8080_GDMALLIOFTTypeDef *)(&GDMA_LLIStruct_G2_oft),
                         LCD_HIGHT * 2 - 1,
                         (uint32_t)(&GDMA_LLIStruct_LAST));
}


bool get_font_dot(uint16_t unicode, uint32_t *addr, uint8_t *dot_width, uint8_t *font_width)
{
    uint16_t offset = *(uint16_t *)(unicode * 2 + 0x1400000);

    *addr = offset * 128 + 0x1420000;
    if ((unicode >= 0x20) && (unicode <= 0x80))
    {
        *font_width = 16;
    }
    else
    {
        *font_width = 32;
    }
    *dot_width = 32;
    return true;
}

DATA_RAM_FUNCTION
void lcd_show_char(uint16_t font_width, uint16_t font_height,
                   const unsigned char BMP[], int16_t xs,
                   int16_t ys, \
                   uint32_t colour, int16_t Zs, int16_t Ze, uint8_t *buf)
{
    if ((ys >= Ze) || ((ys + font_height) <= Zs) || (xs >= LCD_WIDTH) || (xs + font_width <= 0))
    {
        return;
    }
    uint8_t *pBMP;
    uint8_t row_bytes = (font_width + 7) >> 3;
    pBMP = (uint8_t *)BMP;
    uint8_t loopx;
    uint8_t loopy;
    uint8_t addr_hs;
    uint8_t addr_hh;
    uint8_t looph;
    uint16_t write_off;

    uint16_t *writebuf = (uint16_t *)buf;

    if (ys > Zs)
    {
        addr_hs = 0;
        addr_hh = Ze - ys;
        addr_hh = (addr_hh > font_height) ? font_height : addr_hh;
        ys = ys - Zs;
    }
    else
    {
        if ((ys + font_height) < Ze)
        {
            addr_hs = Zs - ys;
            addr_hh = (ys + font_height) - Zs;
        }
        else
        {
            addr_hs = Zs - ys;
            addr_hh = Ze - Zs;
        }
        ys = 0;
    }
    looph = ys;
    pBMP = pBMP + row_bytes * addr_hs;

    if (xs < 0)
    {
        if (xs + font_width < 0)
        {
            return;
        }
        int width_cache;
        width_cache = font_width + xs;
        for (loopy = 0; loopy < addr_hh; looph ++, loopy ++)
        {
            write_off = looph * LCD_WIDTH;
            for (loopx = 0; loopx < width_cache; loopx ++)
            {
                if ((pBMP[loopy * row_bytes + (loopx - xs) / 8] >> (7 - (loopx - xs) % 8)) & 0x01)
                {
                    writebuf[write_off + loopx]  = colour;
                }
            }
        }
    }
    else if ((xs + font_width) > LCD_WIDTH)
    {
        int width_cache;
        width_cache = LCD_WIDTH - xs;
        for (loopy = 0; loopy < addr_hh; looph ++, loopy ++)
        {
            write_off = looph * LCD_WIDTH;
            for (loopx = 0; loopx < width_cache; loopx ++)
            {
                if ((pBMP[loopy * row_bytes + loopx / 8] >> (7 - loopx % 8)) & 0x01)
                {
                    writebuf[write_off + xs + loopx]  = colour;
                }
            }
        }
    }
    else
    {
        for (loopy = 0; loopy < addr_hh; loopy ++, looph ++)
        {
            for (loopx = 0; loopx < font_width; loopx ++)
            {
                write_off = looph * LCD_WIDTH;
                if ((pBMP[loopy * row_bytes + loopx / 8] >> (7 - loopx % 8)) & 0x01)
                {
                    writebuf[write_off + xs + loopx]  = colour;
                }
            }
        }
    }
}

