#include "board.h"
#include "app_section.h"
#include "wristband_gui.h"
#include "rtl876x_spi.h"
#include "rtl876x_gpio.h"
#include "trace.h"
#include "lcd_gc9203_16080.h"
#include "platform_utils.h"
#include "module_lcd_spi.h"

DATA_RAM_FUNCTION
void gc9203_write_cmd(uint8_t command)
{
    while (SPI_GetFlagState(LCD_SPI_BUS, SPI_FLAG_BUSY));
    /* switch to CS0 signal */
    SPI_SetCSNumber(LCD_SPI_BUS, 0);
    GPIO_ResetBits(LCD_SPI_DC_PIN);
    while (SPI_GetFlagState(LCD_SPI_BUS, SPI_FLAG_TFE) == false);
    SPI_SendData(LCD_SPI_BUS, command);

    //platform_delay_us(1);
    while (SPI_GetFlagState(LCD_SPI_BUS, SPI_FLAG_TFE) == false);
    /* Waiting for SPI data transfer to end */
    while (SPI_GetFlagState(LCD_SPI_BUS, SPI_FLAG_BUSY));
    GPIO_SetBits(LCD_SPI_DC_PIN);
}

DATA_RAM_FUNCTION
void gc9203_write_data(uint8_t data)
{
    while (SPI_GetFlagState(LCD_SPI_BUS, SPI_FLAG_TFE) == false);
    SPI_SendData(LCD_SPI_BUS, data);

    /* Waiting for SPI data transfer to end */
    while (SPI_GetFlagState(LCD_SPI_BUS, SPI_FLAG_BUSY)); //Howie todo, remove later
}

void lcd_gc9203_power_on(void)
{
    lcd_set_backlight(100);
    gc9203_write_cmd(0x11);
    gc9203_write_cmd(0x29);
}

void lcd_gc9203_power_off(void)
{
    gc9203_write_cmd(0x28);
    gc9203_write_cmd(0x10);
    platform_delay_ms(150);//we must delay for power by screen factory
    lcd_set_backlight(0);
}


void lcd_gc9203_set_window(uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd)
{
    xStart = xStart + 0;
    xEnd = xEnd + 0;
    yStart = yStart + 18;
    yEnd = yEnd + 18;

    /*this is a bug here if lcd size bigger than uint8_t*/
    gc9203_write_cmd(0x37);
    gc9203_write_data(yStart >> 8);
    gc9203_write_data(yStart);


    gc9203_write_cmd(0x36);
    gc9203_write_data(yEnd >> 8);
    gc9203_write_data(yEnd);


    gc9203_write_cmd(0x39);
    gc9203_write_data(xStart >> 8);
    gc9203_write_data(xStart);

    gc9203_write_cmd(0x38);
    gc9203_write_data(xEnd >> 8);
    gc9203_write_data(xEnd);


    gc9203_write_cmd(0x22);
}



void lcd_gc9203_init(void)
{
    gc9203_write_cmd(0xff);
    gc9203_write_data(0x5a);
    gc9203_write_data(0xa5);
    gc9203_write_cmd(0xf6);
    gc9203_write_data(0x01);
    gc9203_write_data(0x12);
    gc9203_write_cmd(0xef);
    gc9203_write_data(0x10);
    gc9203_write_data(0x52);
    gc9203_write_cmd(0xa5);
    gc9203_write_data(0x07);
    gc9203_write_data(0x80);
    gc9203_write_cmd(0x02);
    gc9203_write_data(0x01);
    gc9203_write_data(0x00);
    gc9203_write_cmd(0x03);
    gc9203_write_data(0x10);
    gc9203_write_data(0x10);

    gc9203_write_cmd(0x01);
    gc9203_write_data(0x03);
    gc9203_write_data(0x1c);

    gc9203_write_cmd(0xf6);
    gc9203_write_data(0x01);
    gc9203_write_data(0x12);

    gc9203_write_cmd(0x11);
    gc9203_write_data(0x10);
    gc9203_write_data(0x00);

    gc9203_write_cmd(0xEB);
    gc9203_write_data(0x0d);
    gc9203_write_data(0x1f);
    gc9203_write_cmd(0xEC);
    gc9203_write_data(0x05);
    gc9203_write_data(0x28);

    gc9203_write_cmd(0x50);
    gc9203_write_data(0xf3);
    gc9203_write_data(0x80);
    gc9203_write_cmd(0x51);
    gc9203_write_data(0x11);
    gc9203_write_data(0x0c);
    gc9203_write_cmd(0x52);
    gc9203_write_data(0x09);
    gc9203_write_data(0x09);
    gc9203_write_cmd(0x53);
    gc9203_write_data(0x37);
    gc9203_write_data(0x06);
    gc9203_write_cmd(0x54);
    gc9203_write_data(0x4e);
    gc9203_write_data(0x1f);
    gc9203_write_cmd(0x55);
    gc9203_write_data(0x12);
    gc9203_write_data(0x12);
    gc9203_write_cmd(0x56);
    gc9203_write_data(0x37);
    gc9203_write_data(0x36);
    gc9203_write_cmd(0x57);
    gc9203_write_data(0xfc);
    gc9203_write_data(0x80);
    gc9203_write_cmd(0x58);
    gc9203_write_data(0x11);
    gc9203_write_data(0x0c);
    gc9203_write_cmd(0x59);
    gc9203_write_data(0x09);
    gc9203_write_data(0x09);
    gc9203_write_cmd(0x5A);
    gc9203_write_data(0x37);
    gc9203_write_data(0x06);
    gc9203_write_cmd(0x5B);
    gc9203_write_data(0x4e);
    gc9203_write_data(0x19);
    gc9203_write_cmd(0x5C);
    gc9203_write_data(0x12);
    gc9203_write_data(0x12);
    gc9203_write_cmd(0x5D);
    gc9203_write_data(0x37);
    gc9203_write_data(0x36);

    gc9203_write_cmd(0x07);
    gc9203_write_data(0x10);
    gc9203_write_data(0x13);
    gc9203_write_cmd(0xfe);
}







