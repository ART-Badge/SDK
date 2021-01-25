#include "board.h"
#include "app_section.h"
#include "wristband_gui.h"
#include "rtl876x_if8080.h"
#include "trace.h"
#include "drv_lcd_8080.h"
#include "lcd_st7789_240.h"
#include "platform_utils.h"
#include "rtl876x_module_lcd_8080.h"

DATA_RAM_FUNCTION
void st7789_write_cmd(uint8_t command)
{
    IF8080_SetCS();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    IF8080_ResetCS();
    IF8080_SendCommand(command);
}

DATA_RAM_FUNCTION
void st7789_write_data(uint8_t data)
{
    IF8080_SendData(&data, 1);
}

void lcd_st7789_power_on(void)
{
//    st7789_write_cmd(0xc6);
//    st7789_write_data(0x1F);
    st7789_write_cmd(0x11);
    platform_delay_ms(10);
    st7789_write_cmd(0x29);
    lcd_set_backlight(100);
}

void lcd_st7789_power_off(void)
{
    st7789_write_cmd(0x28);
    st7789_write_cmd(0x10);
    lcd_set_backlight(0);
}


void lcd_st7789_set_window(uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd)
{
    IF8080_SwitchMode(IF8080_MODE_MANUAL);

    st7789_write_cmd(0x2a);
    st7789_write_data(xStart >> 8);
    st7789_write_data(xStart & 0xff);
    st7789_write_data(xEnd >> 8);
    st7789_write_data(xEnd & 0xff);

    st7789_write_cmd(0x2b);
    st7789_write_data(yStart >> 8);
    st7789_write_data(yStart & 0xff);
    st7789_write_data(yEnd >> 8);
    st7789_write_data(yEnd & 0xff);
    IF8080_SetCS();
    /* Enable Auto mode */
    IF8080_SwitchMode(IF8080_MODE_AUTO);

    IF8080_ClearTxCounter();

    /* Configure command */
    uint8_t cmd[1] = {0x2c};
    IF8080_SetCmdSequence(cmd, 1);

    /* Enable GDMA TX */
    IF8080_GDMACmd(ENABLE);

    /* Configure pixel number */
    uint32_t len = (xEnd - xStart + 1) * (yEnd - yStart + 1) * PIXEL_BYTES;
    IF8080_SetTxDataLen(len);

    /* Start output */
    IF8080_AutoModeCmd(IF8080_Auto_Mode_Direction_WRITE, ENABLE);
}



void lcd_st7789_init(void)
{
#if 0
    st7789_write_cmd(0x35); //Tearing Effect Line On
    st7789_write_data(0x00);

    st7789_write_cmd(0x44); //Set Tear Scanline
    st7789_write_data(0x00);
    st7789_write_data(0x50);
#if FEATURE_PSRAM
    st7789_write_cmd(0x44); //Set Tear Scanline
    st7789_write_data(0x00);
    st7789_write_data(0xA0);// if use psram set tear scanline here
#else
    st7789_write_cmd(0x44); //Set Tear Scanline
    st7789_write_data(0x00);
    st7789_write_data(0x50);
#endif

    st7789_write_cmd(0x36); //Memory Data Access Control --rotation & color order
    st7789_write_data(0x00);

    st7789_write_cmd(0x3A); //Interface Pixel Format --65K/262K color & ctrl interface
    st7789_write_data(0x05);

    st7789_write_cmd(0xB2); //Porch Setting --BP & FP settings
    st7789_write_data(0x0C);
    st7789_write_data(0x0C);
    st7789_write_data(0x00);
    st7789_write_data(0x33);
    st7789_write_data(0x33);

    st7789_write_cmd(0xB7); //Gate Control -- Gate Voltage settings
    st7789_write_data(0x35);

    st7789_write_cmd(0xBB); //VCOM Setting -- VCOM Voltage settings
    st7789_write_data(0x19);

    st7789_write_cmd(0xC0); //LCM Control
    st7789_write_data(0x2C);

    st7789_write_cmd(0xC2); //VDV and VRH Command Enable
    st7789_write_data(0x01);

    st7789_write_cmd(0xC3); //VRH Set
    st7789_write_data(0x12);

    st7789_write_cmd(0xC4); //VDV Set
    st7789_write_data(0x20);

    st7789_write_cmd(0xC6); //Frame Rate Control in Normal Mode -- set frame rate range 39 ~ 119 frames
    st7789_write_data(0x1F);

    st7789_write_cmd(0xD0); //Power Control 1
    st7789_write_data(0xA4);
    st7789_write_data(0xA1);

    st7789_write_cmd(0xE0); //Positive Voltage Gamma Control
    st7789_write_data(0xD0);
    st7789_write_data(0x04);
    st7789_write_data(0x0D);
    st7789_write_data(0x11);
    st7789_write_data(0x13);
    st7789_write_data(0x2B);
    st7789_write_data(0x3F);
    st7789_write_data(0x54);
    st7789_write_data(0x4C);
    st7789_write_data(0x18);
    st7789_write_data(0x0D);
    st7789_write_data(0x0B);
    st7789_write_data(0x1F);
    st7789_write_data(0x23);

    st7789_write_cmd(0xE1); //Negative Voltage Gamma Control
    st7789_write_data(0xD0);
    st7789_write_data(0x04);
    st7789_write_data(0x0C);
    st7789_write_data(0x11);
    st7789_write_data(0x13);
    st7789_write_data(0x2C);
    st7789_write_data(0x3F);
    st7789_write_data(0x44);
    st7789_write_data(0x51);
    st7789_write_data(0x2F);
    st7789_write_data(0x1F);
    st7789_write_data(0x1F);
    st7789_write_data(0x20);
    st7789_write_data(0x23);

    st7789_write_cmd(0xE4); //Gate Control
    st7789_write_data(0x27);
    st7789_write_data(0x00);
    st7789_write_data(0x11);

    st7789_write_cmd(0x21); //Invert the color of screen

    st7789_write_cmd(0x11); //Sleep Out -- wakeup screen
    platform_delay_ms(120);
#endif
#if 1
    platform_delay_ms(120);
    /* Sleep Out */
    st7789_write_cmd(0x11);
    /* wait for power stability */
    platform_delay_ms(120);

    /* Memory Data Access Control */
    st7789_write_cmd(0x36);
    st7789_write_data(0x00);

    /* RGB 5-6-5-bit  */
    st7789_write_cmd(0x3A);
    st7789_write_data(0x05);

    /* Porch Setting */
    st7789_write_cmd(0xB2);
    st7789_write_data(0x0C);
    st7789_write_data(0x0C);
    st7789_write_data(0x00);
    st7789_write_data(0x33);
    st7789_write_data(0x33);

    /*  Gate Control */
    st7789_write_cmd(0xB7);
    st7789_write_data(0x72);

    /* VCOM Setting */
    st7789_write_cmd(0xBB);
    st7789_write_data(0x3D);   //Vcom=1.625V

    /* LCM Control */
    st7789_write_cmd(0xC0);
    st7789_write_data(0x2C);

    /* VDV and VRH Command Enable */
    st7789_write_cmd(0xC2);
    st7789_write_data(0x01);

    /* VRH Set */
    st7789_write_cmd(0xC3);
    st7789_write_data(0x19);

    /* VDV Set */
    st7789_write_cmd(0xC4);
    st7789_write_data(0x20);

    /* Frame Rate Control in Normal Mode */
    st7789_write_cmd(0xC6);
    st7789_write_data(0x0F);	//60MHZ

    /* Power Control 1 */
    st7789_write_cmd(0xD0);
    st7789_write_data(0xA4);
    st7789_write_data(0xA1);

    /* Positive Voltage Gamma Control */
    st7789_write_cmd(0xE0);
    st7789_write_data(0xD0);
    st7789_write_data(0x04);
    st7789_write_data(0x0D);
    st7789_write_data(0x11);
    st7789_write_data(0x13);
    st7789_write_data(0x2B);
    st7789_write_data(0x3F);
    st7789_write_data(0x54);
    st7789_write_data(0x4C);
    st7789_write_data(0x18);
    st7789_write_data(0x0D);
    st7789_write_data(0x0B);
    st7789_write_data(0x1F);
    st7789_write_data(0x23);

    /* Negative Voltage Gamma Control */
    st7789_write_cmd(0xE1);
    st7789_write_data(0xD0);
    st7789_write_data(0x04);
    st7789_write_data(0x0C);
    st7789_write_data(0x11);
    st7789_write_data(0x13);
    st7789_write_data(0x2C);
    st7789_write_data(0x3F);
    st7789_write_data(0x44);
    st7789_write_data(0x51);
    st7789_write_data(0x2F);
    st7789_write_data(0x1F);
    st7789_write_data(0x1F);
    st7789_write_data(0x20);
    st7789_write_data(0x23);

    /* Display Inversion On */
//    st7789_write_cmd(0x21);
#endif
}

void lcd_st7789_frame_rate(uint8_t RTNA, uint8_t FPA, uint8_t BPA)
{
    IF8080_SwitchMode(IF8080_MODE_MANUAL);
    st7789_write_cmd(0xB2);
    st7789_write_data(BPA & 0x3f);
    st7789_write_data(FPA & 0x3f);
    st7789_write_data(0x00);
    st7789_write_data(0x33);
    st7789_write_data(0x33);

    st7789_write_cmd(0xC6);
    st7789_write_data(RTNA & 0x1F);
}
