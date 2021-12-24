#include "board.h"
#include "trace.h"
#include "string.h"
#include "os_timer.h"
#include "os_mem.h"
#include "os_sync.h"
#include "rtl876x_rcc.h"
#include "rtl876x_tim.h"
#include "rtl876x_gpio.h"
#include "rtl876x_pinmux.h"
#include "rtl876x_spi.h"
#include "rtl876x_gdma.h"
#include "rtl876x_nvic.h"
#include "hub_clock.h"
#include "hub_display.h"
#include "module_font.h"
#include "app_section.h"
#include "flash_device.h"
#include "wristband_picture_res.h"
#include "gui_lcd_callback.h"
#include "gui_core.h"
#include "lcd_st7735_16080.h"
#include "platform_utils.h"
#include "app_msg.h"
#include "app_task.h"
#include "module_lcd_spi.h"
#include "hub_task.h"

GDMA_LLIDef *GDMA_LLIStruct = NULL;

void lcd_set_reset(bool reset)
{
    if (reset)
    {
        Pad_Config(LCD_SPI_RST, PAD_SW_MODE, PAD_IS_PWRON, PAD_PULL_DOWN, PAD_OUT_ENABLE, PAD_OUT_LOW);
    }
    else
    {
        Pad_Config(LCD_SPI_RST, PAD_SW_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_ENABLE, PAD_OUT_HIGH);
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
    platform_delay_ms(50);
    lcd_set_reset(false);
    platform_delay_ms(50);
    lcd_te_device_init();

    user_lcd_power_on_cb = lcd_st7735_power_on;
    user_lcd_power_off_cb = lcd_st7735_power_off;
    user_lcd_set_window_cb = lcd_st7735_set_window;
    user_lcd_init_cb = lcd_st7735_init;


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

    NVIC_InitTypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel = GPIO_FRAME_SYNC_IRQ;
    NVIC_InitStruct.NVIC_IRQChannelPriority = 3;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);

    GPIO_INTConfig(GPIO_GetPin(LCD_FRAME_SYNC), DISABLE);
    GPIO_MaskINTConfig(GPIO_GetPin(LCD_FRAME_SYNC), ENABLE);

    user_lcd_te_enable_cb = lcd_te_enable;
    user_lcd_te_disable_cb = lcd_te_disable;
    gui_set_te_function(false);
}

void lcd_frame_sync_handler(void)
{
    lcd_te_disable();

    T_IO_MSG lcd_sync_msg;
    lcd_sync_msg.type = HUB_MSG_LCD;
    lcd_sync_msg.subtype = LCD_MSG_FRAME_SYNC;
    app_send_msg_to_apptask(&lcd_sync_msg);
}
void lcd_te_enable(void)
{
    return;
}

void lcd_te_disable(void)
{
    return;
}

void lcd_pad_init(void)
{
    Pinmux_Config(LCD_SPI_CLK, LCD_SPI_FUNC_CLK);
    Pinmux_Config(LCD_SPI_MOSI, LCD_SPI_FUNC_MOSI);
    Pinmux_Config(LCD_SPI_MISO, LCD_SPI_FUNC_MISO);
    Pinmux_Config(LCD_SPI_CS, LCD_SPI_FUNC_CS);

    Pinmux_Config(LCD_SPI_DC, DWGPIO);

    Pad_Config(LCD_SPI_CLK, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_NONE, PAD_OUT_ENABLE, PAD_OUT_HIGH);
    Pad_Config(LCD_SPI_MOSI, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_NONE, PAD_OUT_ENABLE,
               PAD_OUT_HIGH);
    Pad_Config(LCD_SPI_MISO, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE, PAD_OUT_HIGH);
    Pad_Config(LCD_SPI_CS, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_ENABLE, PAD_OUT_HIGH);
    Pad_Config(LCD_SPI_DC, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_NONE, PAD_OUT_ENABLE, PAD_OUT_LOW);

    /*BL AND RESET ARE NOT FIX*/
    Pad_Config(LCD_SPI_BL, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_NONE, PAD_OUT_ENABLE, PAD_OUT_LOW);
    Pinmux_Config(LCD_SPI_BL, timer_pwm4);
    Pad_Config(LCD_SPI_EN, PAD_SW_MODE, PAD_IS_PWRON, PAD_PULL_DOWN, PAD_OUT_DISABLE, PAD_OUT_LOW);

    Pad_Config(GSENSOR_SPI_CS, PAD_SW_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_ENABLE, PAD_OUT_HIGH);


}
void lcd_enter_dlps(void)
{
    Pad_Config(LCD_SPI_CLK, PAD_SW_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE, PAD_OUT_LOW);
    Pad_Config(LCD_SPI_MOSI, PAD_SW_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE, PAD_OUT_LOW);

    Pad_Config(LCD_SPI_CS, PAD_SW_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE, PAD_OUT_LOW);
    Pad_Config(LCD_SPI_DC, PAD_SW_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE, PAD_OUT_LOW);

    Pad_Config(LCD_SPI_BL, PAD_SW_MODE, PAD_IS_PWRON, PAD_PULL_DOWN, PAD_OUT_ENABLE, PAD_OUT_LOW);
    Pad_Config(LCD_SPI_EN, PAD_SW_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE, PAD_OUT_LOW);
}
void lcd_exit_dlps(void)
{
    Pad_Config(LCD_SPI_CLK, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_NONE, PAD_OUT_ENABLE, PAD_OUT_LOW);
    Pad_Config(LCD_SPI_MOSI, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_NONE, PAD_OUT_ENABLE,
               PAD_OUT_HIGH);
    Pad_Config(LCD_SPI_CS, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_ENABLE, PAD_OUT_HIGH);
    Pad_Config(LCD_SPI_DC, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_NONE, PAD_OUT_ENABLE, PAD_OUT_LOW);

    Pad_Config(LCD_SPI_BL, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_NONE, PAD_OUT_DISABLE, PAD_OUT_HIGH);
    Pad_Config(LCD_SPI_EN, PAD_SW_MODE, PAD_IS_PWRON, PAD_PULL_DOWN, PAD_OUT_DISABLE, PAD_OUT_LOW);
}


/**
  * @brief  writband lcd device init set IO config here
  * @param  None
  * @retval None
  */
DATA_RAM_FUNCTION
void lcd_device_init(void)
{
    lcd_pad_init();

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
    TIM_InitStruct.TIM_SOURCE_DIV = TIM_CLOCK_DIVIDER_40;
    TIM_TimeBaseInit(BL_PWM_TIM, &TIM_InitStruct);
    TIM_Cmd(BL_PWM_TIM, DISABLE);

    RCC_PeriphClockCmd(APBPeriph_GPIO, APBPeriph_GPIO_CLOCK, ENABLE);
    SPI_DeInit(LCD_SPI_BUS);
    RCC_PeriphClockCmd(LCD_SPI_APBPeriph, LCD_SPI_APBClock, ENABLE);
    SPI_InitTypeDef  SPI_InitStructure;

    SPI_StructInit(&SPI_InitStructure);
    SPI_InitStructure.SPI_Direction   = SPI_Direction_FullDuplex;
    SPI_InitStructure.SPI_Mode        = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize    = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL        = SPI_CPOL_High;
    SPI_InitStructure.SPI_CPHA        = SPI_CPHA_2Edge;
    SPI_InitStructure.SPI_BaudRatePrescaler  = SPI_BaudRatePrescaler_2;
    SPI_InitStructure.SPI_FrameFormat = SPI_Frame_Motorola;
    SPI_InitStructure.SPI_NDF         = 0;
    SPI_InitStructure.SPI_TxWaterlevel = 30;
    SPI_InitStructure.SPI_TxDmaEn = ENABLE;

    SPI_Init(LCD_SPI_BUS, &SPI_InitStructure);
    SPI_Cmd(LCD_SPI_BUS, ENABLE);

    RCC_PeriphClockCmd(APBPeriph_GPIO, APBPeriph_GPIO_CLOCK, ENABLE);

    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_ControlMode = GPIO_SOFTWARE_MODE;
    GPIO_InitStruct.GPIO_Pin  = GPIO_GetPin(LCD_SPI_DC);
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_ITCmd = DISABLE;
    GPIO_Init(&GPIO_InitStruct);
    GPIO_SetBits(GPIO_GetPin(LCD_SPI_BL));
}

void lcd_set_backlight(uint32_t percent)
{
    Pad_Config(LCD_SPI_BL, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE, PAD_OUT_HIGH);
    extern bool allowed_lcd_backlight_enter_dlps;
    if (percent != 0)
    {
        TIM_Cmd(BL_PWM_TIM, DISABLE);
        TIM_PWMChangeFreqAndDuty(BL_PWM_TIM, (percent) * 10, (100 - percent) * 10);
        TIM_Cmd(BL_PWM_TIM, ENABLE);
        Pad_Config(LCD_SPI_BL, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_NONE, PAD_OUT_DISABLE, PAD_OUT_HIGH);
        allowed_lcd_backlight_enter_dlps = false;
    }
    else
    {
        TIM_Cmd(BL_PWM_TIM, DISABLE);
        Pad_Config(LCD_SPI_BL, PAD_SW_MODE, PAD_IS_PWRON, PAD_PULL_DOWN, PAD_OUT_DISABLE, PAD_OUT_LOW);
        allowed_lcd_backlight_enter_dlps = true;
    }
}



DATA_RAM_FUNCTION
void lcd_clear(uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd, uint16_t color)
{
    rtl_gui_lcd_sectionconfig(xStart, yStart, xEnd, yEnd);
    while (SPI_GetFlagState(LCD_SPI_BUS, SPI_FLAG_BUSY));
    for (uint16_t i = xStart; i <= xEnd; i++)
    {
        for (uint16_t j = yStart; j <= yEnd; j++)
        {
            while (!(LCD_SPI_BUS->SR & BIT(1)));
            LCD_SPI_BUS->DR[0] = color >> 8;
            while (!(LCD_SPI_BUS->SR & BIT(1)));
            LCD_SPI_BUS->DR[0] = color;
        }
    }
    while (SPI_GetFlagState(LCD_SPI_BUS, SPI_FLAG_BUSY));
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
    GDMA_InitStruct.GDMA_DIR                 = GDMA_DIR_MemoryToPeripheral;
    GDMA_InitStruct.GDMA_BufferSize          = LCD_WIDTH * LCD_SECTION_HEIGHT * PIXEL_BYTES;
    GDMA_InitStruct.GDMA_SourceInc           = DMA_SourceInc_Inc;
    GDMA_InitStruct.GDMA_DestinationInc      = DMA_DestinationInc_Fix;
    GDMA_InitStruct.GDMA_SourceDataSize      = GDMA_DataSize_Byte;
    GDMA_InitStruct.GDMA_DestinationDataSize = GDMA_DataSize_Byte;
    GDMA_InitStruct.GDMA_SourceMsize         = GDMA_Msize_4;
    GDMA_InitStruct.GDMA_DestinationMsize    = GDMA_Msize_4;
    GDMA_InitStruct.GDMA_SourceAddr          = 0;
    GDMA_InitStruct.GDMA_DestinationAddr     = (uint32_t)(LCD_SPI_BUS->DR);
    GDMA_InitStruct.GDMA_DestHandshake       = LCD_SPI_DMA_TX_HANDSHAKE;

    GDMA_Init(LCD_DMA_CHANNEL_INDEX, &GDMA_InitStruct);
    GDMA_INTConfig(LCD_DMA_CHANNEL_NUM, GDMA_INT_Transfer, ENABLE);
}


void lcd_dma_single_block_start(uint32_t destination_addr, uint32_t source_addr, uint32_t len)
{
    GDMA_SetBufferSize(LCD_DMA_CHANNEL_INDEX, len);
    if (destination_addr != 0)
    {
        //GDMA_SetDestinationAddress(LCD_DMA_CHANNEL_INDEX, destination_addr);
    }
    else
    {
        //GDMA_SetDestinationAddress(LCD_DMA_CHANNEL_INDEX, (uint32_t)(&(LCD->FIFO)));
    }
    GDMA_SetSourceAddress(LCD_DMA_CHANNEL_INDEX, (uint32_t)source_addr);
    GDMA_Cmd(LCD_DMA_CHANNEL_NUM, ENABLE);
    SPI_GDMACmd(LCD_SPI_BUS, SPI_GDMAReq_Tx, ENABLE);
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
}



void lcd_dma_set_multi_block_for_y(uint32_t picture0_addr, uint32_t picture1_addr,
                                   uint32_t offset,
                                   uint32_t yStart, uint32_t yEnd)//offset mean left menu not display in current menu
{

}

void lcd_dma_set_multi_block_for_x(uint32_t picture0_addr, uint32_t picture1_addr,
                                   uint32_t offset,
                                   uint32_t yStart, uint32_t yEnd)//offset mean up menu not display in current menu
{


}

bool get_font_dot(uint16_t unicode, uint32_t *addr, uint8_t *dot_width, uint8_t *font_width)
{
#define CODE_TABLE_SIZE  30808
    const uint16_t *unicode_table = (uint16_t *) 0x900000;
    int first = 0;
    int end = CODE_TABLE_SIZE  - 1;
    int mid = 0;
    if ((unicode < 0x80) && (unicode >= 0x20))
    {
        *font_width = 8;
    }
    else
    {
        *font_width = 16;
    }
    *dot_width = 16;

    while (first <= end)
    {
        mid = (first + end) / 2;
        if (unicode_table[mid] == unicode)
        {
            *addr = UserData_Font_addr + (unicode_table[mid + CODE_TABLE_SIZE] - 1) * 34 + 2;
            return true;
        }
        else if (unicode_table[mid] > unicode)
        {
            end = mid - 1;
        }
        else
        {
            first = mid + 1;
        }
    }
    return false; //can not get the right value
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

