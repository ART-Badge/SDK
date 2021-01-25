#include "board.h"
#include "app_section.h"
#include "wristband_gui.h"
#include "rtl876x_if8080.h"
#include "trace.h"
#include "wristband_gui.h"
#include "lcd_STUnknow_320_qspi.h"
#include "platform_utils.h"

#define BIT_CMD_CH(x)           (((x) & 0x00000003) << 20)
#define BIT_DATA_CH(x)          (((x) & 0x00000003) << 18)
#define BIT_ADDR_CH(x)          (((x) & 0x00000003) << 16)
#define BIT_TMOD(x)             (((x) & 0x00000003) << 8)

#define BIT_TXSIM               (0x00000001 << 9)
#define BIT_SEQ_EN              (0x00000001 << 3)


DATA_RAM_FUNCTION
static void spic2_spi_write(uint8_t *buf, uint32_t len)
{
    SPIC2->ser = BIT0;
    SPIC2->ssienr = 0x00;//disable SPIC2

    SPIC2->ctrlr0 &= ~(BIT_CMD_CH(3) | BIT_ADDR_CH(3) | BIT_DATA_CH(3));//SET CHANNEL NUM
    SPIC2->ctrlr0 &= ~(BIT_TMOD(3)); //tx mode

    for (uint32_t i = 0; i < len; i++)
    {
        SPIC2->dr[0].byte = buf[i];
    }
    SPIC2->ssienr = 0x01;//enable SPIC2

    while (SPIC2->sr & BIT0); // wait bus busy

    SPIC2->ssienr = 0x00;//disable SPIC2
}



DATA_RAM_FUNCTION
static void rtl_STUnknow_qspi_write_cmd(uint16_t cmd) //total 4 byte, first byte is 0x02
{
    uint8_t sdat[] = {0xDE, 0x00, cmd, 0x00};
    spic2_spi_write(sdat, sizeof(sdat));
}

DATA_RAM_FUNCTION
static void rtl_STUnknow_qspi_write_data(uint16_t data)
{
    uint8_t sdat[] = {data};
    spic2_spi_write(sdat, sizeof(sdat));
}


//DATA_RAM_FUNCTION
//static void rtl_STUnknow_qspi_cmd_param(uint8_t cmd, uint8_t data) //total 5 byte, first byte is 0x02
//{
//    uint8_t sdat[] = {0x02, 0x00, cmd, 0x00, data};
//    spic2_spi_write(sdat, sizeof(sdat));
//}

DATA_RAM_FUNCTION
static void rtl_STUnknow_qspi_cmd_param4(uint8_t cmd,
                                         uint8_t *data) //total 8 byte, first byte is 0x02
{
    uint8_t sdat[] = {0xDE, 0x00, cmd, 0x00, data[0], data[1], data[2], data[3]};
    spic2_spi_write(sdat, sizeof(sdat));
}

DATA_RAM_FUNCTION
static void rtl_STUnknow_qspi_enter_data_output_mode(void) //total 4 byte, first byte is 0x32
{
    SPIC2->ser = BIT0;//select CS0
    SPIC2->ssienr = 0x00;//disable SPIC2

    SPIC2->ctrlr0 &= ~(BIT_CMD_CH(3) | BIT_ADDR_CH(3) | BIT_DATA_CH(3));//SET CHANNEL NUM
    SPIC2->ctrlr0 |= (BIT_CMD_CH(0) | BIT_ADDR_CH(0) | BIT_DATA_CH(2));
    SPIC2->ctrlr0 &= ~(BIT_TMOD(3)); //set tx mode

    SPIC2->imr |= BIT_TXSIM;
    SPIC2->ctrlr2 |= BIT_SEQ_EN;

    /* must push cmd and address before SPIC enable */
    uint32_t first_word = 0xDE | __REV(0x006000);
    SPIC2->dr[0].word = first_word;

    SPIC2->dmacr = 2;

    /* change this value can not influence the result. the wave is split into two periods. the first is 32 bytes. */
    SPIC2->dmatdlr = 4; /* no any influence. */



    SPIC2->ssienr = 0x01;//enable SPIC2
    /*then , we can push data to FIFO*/


    //SPIC2->ctrlr2 &= ~ BIT_SEQ_EN;
    //SPIC2->imr &= ~ BIT_TXSIM;

    //SPIC2->ssienr = 0x00;//disable SPIC2
}

void lcd_STUnknow_qspi_320_power_on(void)
{
    rtl_STUnknow_qspi_write_cmd(0x11);
    rtl_STUnknow_qspi_write_cmd(0x29);
}

void lcd_STUnknow_qspi_320_power_off(void)
{
    rtl_STUnknow_qspi_write_cmd(0x28);
    rtl_STUnknow_qspi_write_cmd(0x10);
}

void lcd_STUnknow_qspi_320_set_window(uint16_t xStart, uint16_t yStart, uint16_t xEnd,
                                      uint16_t yEnd)
{
    uint8_t data[4];
    xStart = xStart + 12;
    xEnd = xEnd + 12;
    yStart = yStart + 0;
    yEnd = yEnd + 0;

    data[0] = xStart >> 8;
    data[1] = xStart & 0xff;
    data[2] = xEnd >> 8;
    data[3] = xEnd & 0xff;
    rtl_STUnknow_qspi_cmd_param4(0x2A, data);


    data[0] = yStart >> 8;
    data[1] = yStart & 0xff;
    data[2] = yEnd >> 8;
    data[3] = yEnd & 0xff;
    rtl_STUnknow_qspi_cmd_param4(0x2B, data);

    rtl_STUnknow_qspi_write_cmd(0x2C);
    rtl_STUnknow_qspi_write_cmd(0x61);
    rtl_STUnknow_qspi_write_cmd(0x60);

    rtl_STUnknow_qspi_enter_data_output_mode();
}

void lcd_STUnknow_qspi_320_init(void)
{
    rtl_STUnknow_qspi_write_cmd(0xF0);
    rtl_STUnknow_qspi_write_data(0xC3);

    rtl_STUnknow_qspi_write_cmd(0xF0);
    rtl_STUnknow_qspi_write_data(0x96);

    rtl_STUnknow_qspi_write_cmd(0xB0);
    rtl_STUnknow_qspi_write_data(0x80);

    rtl_STUnknow_qspi_write_cmd(0x36);
    rtl_STUnknow_qspi_write_data(0x48);

    rtl_STUnknow_qspi_write_cmd(0x3A);
    rtl_STUnknow_qspi_write_data(0x66);

    rtl_STUnknow_qspi_write_cmd(0xB5);
    rtl_STUnknow_qspi_write_data(0x02);
    rtl_STUnknow_qspi_write_data(0x08);
    rtl_STUnknow_qspi_write_data(0x08);
    rtl_STUnknow_qspi_write_data(0x12);

    rtl_STUnknow_qspi_write_cmd(0xB6);
    rtl_STUnknow_qspi_write_data(0x20);
    rtl_STUnknow_qspi_write_data(0x02);
    rtl_STUnknow_qspi_write_data(0x27);

    rtl_STUnknow_qspi_write_cmd(0x11);
    platform_delay_ms(120);
    rtl_STUnknow_qspi_write_cmd(0x29);
    platform_delay_ms(10);
    rtl_STUnknow_qspi_write_cmd(0x21);
    rtl_STUnknow_qspi_write_cmd(0x2C);
}







