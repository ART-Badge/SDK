#include "board.h"
#include "app_section.h"
#include "wristband_gui.h"
#include "rtl876x_if8080.h"
#include "trace.h"
#include "drv_lcd_8080.h"
#include "rtl876x_module_lcd_8080.h"
#include "lcd_st7796_320.h"
#include "platform_utils.h"

// DATA_RAM_FUNCTION
void st7796_write_cmd(uint8_t command)
{
    IF8080_SetCS();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    IF8080_ResetCS();
    IF8080_SendCommand(command);
}

// DATA_RAM_FUNCTION
void st7796_write_data(uint8_t data)
{
    IF8080_SendData(&data, 1);
}

void lcd_st7796_power_on(void)
{
    st7796_write_cmd(0x11);
    st7796_write_cmd(0x29);
    lcd_set_backlight(100);
}
MSH_CMD_EXPORT(lcd_st7796_power_on,lcd_st7796_power_on)

void lcd_st7796_power_off(void)
{
    st7796_write_cmd(0x28);
    st7796_write_cmd(0x10);
    lcd_set_backlight(0);
}


void lcd_st7796_set_window(uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd)
{
    IF8080_SwitchMode(IF8080_MODE_MANUAL);

    st7796_write_cmd(0x2a);
    st7796_write_data(xStart >> 8);
    st7796_write_data(xStart & 0xff);
    st7796_write_data(xEnd >> 8);
    st7796_write_data(xEnd & 0xff);

    st7796_write_cmd(0x2b);
    st7796_write_data(yStart >> 8);
    st7796_write_data(yStart & 0xff);
    st7796_write_data(yEnd >> 8);
    st7796_write_data(yEnd & 0xff);
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



void lcd_st7796_init(void)
{
    st7796_write_cmd(0x11);
    platform_delay_ms(120);
    st7796_write_cmd(0x36);
    st7796_write_data(0x48);

    st7796_write_cmd(0x35);//Tearing Effect Line On
    st7796_write_data(0x00);

    st7796_write_cmd(0x3A);
    st7796_write_data(0x05);

    st7796_write_cmd(0xF0);
    st7796_write_data(0xC3);

    st7796_write_cmd(0xF0);
    st7796_write_data(0x96);

    st7796_write_cmd(0xB4);
    st7796_write_data(0x01);
#if 1
    st7796_write_cmd(0xB1);     //aim at gui_temp_test_4 setting mode menu no psram diplay
    st7796_write_data(0x90);    //frame rate 64hz
    st7796_write_data(0x10);

    st7796_write_cmd(0xB5);
    st7796_write_data(0x2D);
    st7796_write_data(0x2D);
    st7796_write_data(0x00);
    st7796_write_data(0x04);

    st7796_write_cmd(0xB6);
    st7796_write_data(0x8A);
    st7796_write_data(0x07);
    st7796_write_data(0x27);
#else
    st7796_write_cmd(0xB1);
    st7796_write_data(0x20);
    st7796_write_data(0x10);

    st7796_write_cmd(0xB5);
    st7796_write_data(0x2D);
    st7796_write_data(0x2D);
    st7796_write_data(0x00);
    st7796_write_data(0x04);

    st7796_write_cmd(0xB6);
    st7796_write_data(0x8A);
    st7796_write_data(0x07);
    st7796_write_data(0x27);
#endif
    st7796_write_cmd(0xB7);
    st7796_write_data(0xC6);

    st7796_write_cmd(0xB9);
    st7796_write_data(0x02);
    st7796_write_data(0xE0);

    st7796_write_cmd(0xC0);
    st7796_write_data(0x80);
    st7796_write_data(0x25);

    st7796_write_cmd(0xC1);
    st7796_write_data(0x13);

    st7796_write_cmd(0xC5);
    st7796_write_data(0x1A);

    st7796_write_cmd(0xE8);
    st7796_write_data(0x40);
    st7796_write_data(0x8A);
    st7796_write_data(0x00);
    st7796_write_data(0x00);
    st7796_write_data(0x29);
    st7796_write_data(0x19);
    st7796_write_data(0xA5);
    st7796_write_data(0x33);

    st7796_write_cmd(0xE0);
    st7796_write_data(0xF0);
    st7796_write_data(0x04);
    st7796_write_data(0x08);
    st7796_write_data(0x09);
    st7796_write_data(0x08);
    st7796_write_data(0x25);
    st7796_write_data(0x2A);
    st7796_write_data(0x44);
    st7796_write_data(0x42);
    st7796_write_data(0x3C);
    st7796_write_data(0x17);
    st7796_write_data(0x17);
    st7796_write_data(0x29);
    st7796_write_data(0x2E);

    st7796_write_cmd(0xE1);
    st7796_write_data(0xF0);
    st7796_write_data(0x01);
    st7796_write_data(0x05);
    st7796_write_data(0x06);
    st7796_write_data(0x06);
    st7796_write_data(0x03);
    st7796_write_data(0x29);
    st7796_write_data(0x43);
    st7796_write_data(0x41);
    st7796_write_data(0x27);
    st7796_write_data(0x13);
    st7796_write_data(0x13);
    st7796_write_data(0x26);
    st7796_write_data(0x2C);

    st7796_write_cmd(0xF0);
    st7796_write_data(0x3C);

    st7796_write_cmd(0xF0);
    st7796_write_data(0x69);

    st7796_write_cmd(0x21);
}

