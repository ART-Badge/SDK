#include "app_section.h"
#include "wristband_gui.h"
#include "rtl876x_if8080.h"
#include "trace.h"
#include "wristband_gui.h"
#include "lcd_rm69330_390_qspi.h"
#include "platform_utils.h"

#define BIT_CMD_CH(x)           (((x) & 0x00000003) << 20)
#define BIT_DATA_CH(x)          (((x) & 0x00000003) << 18)
#define BIT_ADDR_CH(x)          (((x) & 0x00000003) << 16)
#define BIT_TMOD(x)             (((x) & 0x00000003) << 8)

#define BIT_TXSIM               (0x00000001 << 9)
#define BIT_SEQ_EN              (0x00000001 << 3)


DATA_RAM_FUNCTION
void spic2_spi_write(uint8_t *buf, uint32_t len)
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
void rtl_rm69330_qspi_write_cmd(uint16_t cmd) //total 4 byte, first byte is 0x02
{
    uint8_t sdat[] = {0x02, 0x00, cmd, 0x00};
    spic2_spi_write(sdat, sizeof(sdat));
}


DATA_RAM_FUNCTION
void rtl_rm69330_qspi_cmd_param(uint8_t cmd, uint8_t data) //total 5 byte, first byte is 0x02
{
    uint8_t sdat[] = {0x02, 0x00, cmd, 0x00, data};
    spic2_spi_write(sdat, sizeof(sdat));
}

DATA_RAM_FUNCTION
void rtl_rm69330_qspi_cmd_param4(uint8_t cmd, uint8_t *data) //total 8 byte, first byte is 0x02
{
    uint8_t sdat[] = {0x02, 0x00, cmd, 0x00, data[0], data[1], data[2], data[3]};
    spic2_spi_write(sdat, sizeof(sdat));
}

DATA_RAM_FUNCTION
void rtl_rm69330_qspi_enter_data_output_mode(void) //total 4 byte, first byte is 0x32
{
    SPIC2->ser = BIT0;//select CS0
    SPIC2->ssienr = 0x00;//disable SPIC2

    SPIC2->ctrlr0 &= ~(BIT_CMD_CH(3) | BIT_ADDR_CH(3) | BIT_DATA_CH(3));//SET CHANNEL NUM
    SPIC2->ctrlr0 |= (BIT_CMD_CH(0) | BIT_ADDR_CH(0) | BIT_DATA_CH(2));
    SPIC2->ctrlr0 &= ~(BIT_TMOD(3)); //set tx mode

    SPIC2->imr |= BIT_TXSIM;
    SPIC2->ctrlr2 |= BIT_SEQ_EN;

    /* must push cmd and address before SPIC enable */
    uint32_t first_word = 0x32 | __REV(0x002c00);
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

void lcd_rm69330_qspi_390_power_on(void)
{
    rtl_rm69330_qspi_write_cmd(0x11);
    rtl_rm69330_qspi_write_cmd(0x29);
}

void lcd_rm69330_qspi_390_power_off(void)
{
    rtl_rm69330_qspi_write_cmd(0x28);
    rtl_rm69330_qspi_write_cmd(0x10);
}

void lcd_rm69330_qspi_390_set_window(uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd)
{
    uint8_t data[4];
    xStart = xStart + 6;
    xEnd = xEnd + 6;
    yStart = yStart + 0;
    yEnd = yEnd + 0;

    data[0] = xStart >> 8;
    data[1] = xStart & 0xff;
    data[2] = xEnd >> 8;
    data[3] = xEnd & 0xff;
    rtl_rm69330_qspi_cmd_param4(0x2A, data);


    data[0] = yStart >> 8;
    data[1] = yStart & 0xff;
    data[2] = yEnd >> 8;
    data[3] = yEnd & 0xff;
    rtl_rm69330_qspi_cmd_param4(0x2B, data);

    rtl_rm69330_qspi_enter_data_output_mode();
}

void lcd_rm69330_qspi_390_init(void)
{
    rtl_rm69330_qspi_cmd_param(0xFE, 0x01);
    rtl_rm69330_qspi_cmd_param(0x05, 0x10);
    rtl_rm69330_qspi_cmd_param(0x06, 0x62);
    rtl_rm69330_qspi_cmd_param(0x0D, 0x00);
    rtl_rm69330_qspi_cmd_param(0x0E, 0x81);
    rtl_rm69330_qspi_cmd_param(0x0F, 0x81);
    rtl_rm69330_qspi_cmd_param(0x10, 0x11);
    rtl_rm69330_qspi_cmd_param(0x11, 0x81);
    rtl_rm69330_qspi_cmd_param(0x12, 0x81);

    rtl_rm69330_qspi_cmd_param(0x13, 0x80);

    rtl_rm69330_qspi_cmd_param(0x14, 0x80);

    rtl_rm69330_qspi_cmd_param(0x15, 0x81);

    rtl_rm69330_qspi_cmd_param(0x16, 0x81);

    rtl_rm69330_qspi_cmd_param(0x18, 0x66);

    rtl_rm69330_qspi_cmd_param(0x19, 0x88);

    rtl_rm69330_qspi_cmd_param(0x5B, 0x10);

    rtl_rm69330_qspi_cmd_param(0x5C, 0x55);

    rtl_rm69330_qspi_cmd_param(0x62, 0x19);

    rtl_rm69330_qspi_cmd_param(0x63, 0x19);

    rtl_rm69330_qspi_cmd_param(0x70, 0x55);
    rtl_rm69330_qspi_cmd_param(0x74, 0x0C);

    rtl_rm69330_qspi_cmd_param(0xC5, 0x10);

    rtl_rm69330_qspi_cmd_param(0x25, 0x03);

    rtl_rm69330_qspi_cmd_param(0x26, 0x80);

    rtl_rm69330_qspi_cmd_param(0x27, 0x08);

    rtl_rm69330_qspi_cmd_param(0x28, 0x08);

    rtl_rm69330_qspi_cmd_param(0x2A, 0x23);

    rtl_rm69330_qspi_cmd_param(0x2B, 0x80);

    rtl_rm69330_qspi_cmd_param(0x2D, 0x08);

    rtl_rm69330_qspi_cmd_param(0x2F, 0x08);

    rtl_rm69330_qspi_cmd_param(0x30, 0x43);
    rtl_rm69330_qspi_cmd_param(0x66, 0x90);

    rtl_rm69330_qspi_cmd_param(0x72, 0x1A);

    rtl_rm69330_qspi_cmd_param(0x73, 0x13);

    rtl_rm69330_qspi_cmd_param(0xFE, 0x01);

    //rtl_rm69330_qspi_cmd_param(0x6A,0x03);
    rtl_rm69330_qspi_cmd_param(0x6A, 0x17);

    rtl_rm69330_qspi_cmd_param(0x1B, 0x00);

    rtl_rm69330_qspi_cmd_param(0x1D, 0x03);

    rtl_rm69330_qspi_cmd_param(0x1E, 0x03);

    rtl_rm69330_qspi_cmd_param(0x1F, 0x0C);

    rtl_rm69330_qspi_cmd_param(0x20, 0x03);

    rtl_rm69330_qspi_cmd_param(0xFE, 0x01);

    rtl_rm69330_qspi_cmd_param(0x36, 0x00);

    rtl_rm69330_qspi_cmd_param(0x6c, 0x80);
    rtl_rm69330_qspi_cmd_param(0x6d, 0x19);
    rtl_rm69330_qspi_cmd_param(0xfe, 0x04);
    rtl_rm69330_qspi_cmd_param(0x63, 0x00);
    rtl_rm69330_qspi_cmd_param(0x64, 0x0e);
    rtl_rm69330_qspi_cmd_param(0xfe, 0x02);

    {

        const uint8_t gama_value[] =
        {
            0x0, 0x1, 0x0, 0x1, 0x0, 0xF, 0x0, 0x1F,
            0x1, 0x76, 0x1, 0x90, 0x1, 0xA5, 0x1, 0xB5,
            0x1, 0xC2, 0x1, 0xCE, 0x1, 0xD8, 0x1, 0xE2,
            0x1, 0xF3, 0x2, 0x3, 0x2, 0x11, 0x2, 0x2A,
            0x2, 0x40, 0x2, 0x55, 0x2, 0x69, 0x2, 0x7C,
            0x2, 0x8E, 0x2, 0xA0, 0x0, 0x2, 0x0, 0xB2,
            0x2, 0xC4, 0x2, 0xD6, 0x2, 0xE8, 0x2, 0xFA,
            0x0, 0x1, 0x0, 0x1, 0x0, 0x1, 0x0, 0x87, 0x1,
            0x97, 0x1, 0xB5, 0x1, 0xC4, 0x1, 0xD1, 0x1,
            0xDD, 0x1, 0xE7, 0x1, 0xF1, 0x1, 0xFB, 0x2,
            0x4, 0x2, 0x14, 0x2, 0x23, 0x2, 0x0, 0x30, 0x2,
            0x49, 0x2, 0x5F, 0x2, 0x74, 0x2, 0x88, 0x2, 0x9B,
            0x2, 0xAD, 0x2, 0xBF, 0x2, 0xD1, 0x2, 0xE2, 0x2,
            0xF4, 0x3, 0x5, 0x3, 0x17, 0x0, 0x1, 0x0, 0x1, 0x1,
            0xC5, 0x1, 0xD5, 0x1, 0xDE, 0x1, 0xEE, 0x1, 0xFB,
            0x2, 0x7, 0x2, 0x11, 0x2, 0x1C, 0x2, 0x27, 0x2, 0x30,
            0x2, 0x41, 0x2, 0x52, 0x2, 0x61, 0x2, 0x7D, 0x2,
            0x96, 0x2, 0xAE, 0x2, 0xC5, 0x2, 0xDB, 0x2, 0xF0,
            0x3, 0x5, 0x3, 0x1A, 0x3, 0x2F, 0x0, 0x3, 0x44,
            0x3, 0x58, 0x3, 0x6D
        };
        for (uint16_t i = 0; i < sizeof(gama_value); i++)
        {
            rtl_rm69330_qspi_cmd_param(i, gama_value[i]);
        }

    }


    rtl_rm69330_qspi_cmd_param(0xA9, 0x40);

    rtl_rm69330_qspi_cmd_param(0xaa, 0xb8);
    rtl_rm69330_qspi_cmd_param(0xab, 0x01);
    rtl_rm69330_qspi_cmd_param(0xfe, 0x03);
    rtl_rm69330_qspi_cmd_param(0xa9, 0x40);

    rtl_rm69330_qspi_cmd_param(0xAA, 0x90);

    rtl_rm69330_qspi_cmd_param(0xAB, 0x01);

    rtl_rm69330_qspi_cmd_param(0xFE, 0x01);

    rtl_rm69330_qspi_cmd_param(0x3A, 0x00);

    rtl_rm69330_qspi_cmd_param(0x3B, 0x41);

    rtl_rm69330_qspi_cmd_param(0x3D, 0x17);

    rtl_rm69330_qspi_cmd_param(0x3F, 0x42);

    rtl_rm69330_qspi_cmd_param(0x40, 0x17);
    rtl_rm69330_qspi_cmd_param(0x41, 0x06);

    rtl_rm69330_qspi_cmd_param(0x37, 0x0C);

    rtl_rm69330_qspi_cmd_param(0xFE, 0x0C);

    rtl_rm69330_qspi_cmd_param(0x07, 0x1F);

    rtl_rm69330_qspi_cmd_param(0x08, 0x2F);

    rtl_rm69330_qspi_cmd_param(0x09, 0x3F);

    rtl_rm69330_qspi_cmd_param(0x0A, 0x4F);

    rtl_rm69330_qspi_cmd_param(0x0B, 0x5F);

    rtl_rm69330_qspi_cmd_param(0x0C, 0x6F);
    rtl_rm69330_qspi_cmd_param(0x0D, 0xFF);

    rtl_rm69330_qspi_cmd_param(0x0E, 0xFF);

    rtl_rm69330_qspi_cmd_param(0x0F, 0xFF);

    rtl_rm69330_qspi_cmd_param(0x10, 0xFF);

    rtl_rm69330_qspi_cmd_param(0xFE, 0x01);

    rtl_rm69330_qspi_cmd_param(0x42, 0x14);

    rtl_rm69330_qspi_cmd_param(0x43, 0x41);

    rtl_rm69330_qspi_cmd_param(0x44, 0x25);

    rtl_rm69330_qspi_cmd_param(0x45, 0x52);

    rtl_rm69330_qspi_cmd_param(0x46, 0x36);

    rtl_rm69330_qspi_cmd_param(0x47, 0x63);
    rtl_rm69330_qspi_cmd_param(0x48, 0x41);

    rtl_rm69330_qspi_cmd_param(0x49, 0x14);

    rtl_rm69330_qspi_cmd_param(0x4A, 0x52);

    rtl_rm69330_qspi_cmd_param(0x4B, 0x25);

    rtl_rm69330_qspi_cmd_param(0x4C, 0x63);

    rtl_rm69330_qspi_cmd_param(0x4D, 0x36);

    rtl_rm69330_qspi_cmd_param(0x4E, 0x36);
    rtl_rm69330_qspi_cmd_param(0x4F, 0x63);
    rtl_rm69330_qspi_cmd_param(0x50, 0x25);
    rtl_rm69330_qspi_cmd_param(0x51, 0x52);
    rtl_rm69330_qspi_cmd_param(0x52, 0x14);

    rtl_rm69330_qspi_cmd_param(0x53, 0x41);

    rtl_rm69330_qspi_cmd_param(0x54, 0x63);

    rtl_rm69330_qspi_cmd_param(0x55, 0x36);

    rtl_rm69330_qspi_cmd_param(0x56, 0x52);

    rtl_rm69330_qspi_cmd_param(0x57, 0x25);

    rtl_rm69330_qspi_cmd_param(0x58, 0x41);

    rtl_rm69330_qspi_cmd_param(0x59, 0x14);

    rtl_rm69330_qspi_cmd_param(0xFE, 0x04); //
    rtl_rm69330_qspi_cmd_param(0x5D, 0x01); //
    rtl_rm69330_qspi_cmd_param(0x75, 0x08); //

    rtl_rm69330_qspi_cmd_param(0xFE, 0x04);
    rtl_rm69330_qspi_cmd_param(0x5E, 0x9F);

    rtl_rm69330_qspi_cmd_param(0x5F, 0x43);

    rtl_rm69330_qspi_cmd_param(0x60, 0xFF);

    rtl_rm69330_qspi_cmd_param(0x61, 0xFF);

    rtl_rm69330_qspi_cmd_param(0x62, 0xFF);

    rtl_rm69330_qspi_cmd_param(0xFE, 0x04);

    rtl_rm69330_qspi_cmd_param(0x76, 0xFF);

    rtl_rm69330_qspi_cmd_param(0x77, 0xFF);

    rtl_rm69330_qspi_cmd_param(0x78, 0x10);

    rtl_rm69330_qspi_cmd_param(0x79, 0xF2);

    rtl_rm69330_qspi_cmd_param(0x7A, 0xFF);
    rtl_rm69330_qspi_cmd_param(0xFE, 0x04);

    rtl_rm69330_qspi_cmd_param(0x00, 0x8D);

    rtl_rm69330_qspi_cmd_param(0x01, 0x00);

    rtl_rm69330_qspi_cmd_param(0x02, 0x00);

    rtl_rm69330_qspi_cmd_param(0x03, 0x05);

    rtl_rm69330_qspi_cmd_param(0x04, 0x00);

    rtl_rm69330_qspi_cmd_param(0x05, 0x05);

    rtl_rm69330_qspi_cmd_param(0x06, 0x00);

    rtl_rm69330_qspi_cmd_param(0x07, 0x00);

    rtl_rm69330_qspi_cmd_param(0x08, 0x00);

    rtl_rm69330_qspi_cmd_param(0xFE, 0x04);

    rtl_rm69330_qspi_cmd_param(0x09, 0xCC);

    rtl_rm69330_qspi_cmd_param(0x0A, 0x00);
    rtl_rm69330_qspi_cmd_param(0x0B, 0x02);

    rtl_rm69330_qspi_cmd_param(0x0C, 0x00);

    rtl_rm69330_qspi_cmd_param(0x0D, 0x60);

    rtl_rm69330_qspi_cmd_param(0x0E, 0x06);

    rtl_rm69330_qspi_cmd_param(0x0F, 0x2C);

    rtl_rm69330_qspi_cmd_param(0x10, 0x53);

    rtl_rm69330_qspi_cmd_param(0x11, 0x00);

    rtl_rm69330_qspi_cmd_param(0xFE, 0x04);

    rtl_rm69330_qspi_cmd_param(0x12, 0xCC);

    rtl_rm69330_qspi_cmd_param(0x13, 0x00);
    rtl_rm69330_qspi_cmd_param(0x14, 0x02);

    rtl_rm69330_qspi_cmd_param(0x15, 0x00);

    rtl_rm69330_qspi_cmd_param(0x16, 0x60);

    rtl_rm69330_qspi_cmd_param(0x17, 0x05);

    rtl_rm69330_qspi_cmd_param(0x18, 0x2C);

    rtl_rm69330_qspi_cmd_param(0x19, 0x53);

    rtl_rm69330_qspi_cmd_param(0x1A, 0x00);

    rtl_rm69330_qspi_cmd_param(0xFE, 0x04);

    rtl_rm69330_qspi_cmd_param(0x1B, 0xDC);
    rtl_rm69330_qspi_cmd_param(0x1C, 0x00);

    rtl_rm69330_qspi_cmd_param(0x1D, 0x04);

    rtl_rm69330_qspi_cmd_param(0x1E, 0x02);

    rtl_rm69330_qspi_cmd_param(0x1F, 0x18);

    rtl_rm69330_qspi_cmd_param(0x20, 0x06);

    rtl_rm69330_qspi_cmd_param(0x21, 0x3D);

    rtl_rm69330_qspi_cmd_param(0x22, 0x75);

    rtl_rm69330_qspi_cmd_param(0x23, 0x00);
    rtl_rm69330_qspi_cmd_param(0xFE, 0x04);

    rtl_rm69330_qspi_cmd_param(0x24, 0xDC);

    rtl_rm69330_qspi_cmd_param(0x25, 0x00);

    rtl_rm69330_qspi_cmd_param(0x26, 0x04);

    rtl_rm69330_qspi_cmd_param(0x27, 0x02);

    rtl_rm69330_qspi_cmd_param(0x28, 0x18);

    rtl_rm69330_qspi_cmd_param(0x29, 0x04);

    rtl_rm69330_qspi_cmd_param(0x2A, 0x3D);

    rtl_rm69330_qspi_cmd_param(0x2B, 0x75);

    rtl_rm69330_qspi_cmd_param(0x2D, 0x00);

    rtl_rm69330_qspi_cmd_param(0xFE, 0x04);

    rtl_rm69330_qspi_cmd_param(0x53, 0x8A);
    rtl_rm69330_qspi_cmd_param(0x54, 0x78);

    rtl_rm69330_qspi_cmd_param(0x55, 0x08);

    rtl_rm69330_qspi_cmd_param(0x56, 0x0A);

    rtl_rm69330_qspi_cmd_param(0x58, 0x2A);

    rtl_rm69330_qspi_cmd_param(0x59, 0x00);

    rtl_rm69330_qspi_cmd_param(0x65, 0x02);
    rtl_rm69330_qspi_cmd_param(0x66, 0x0A);

    rtl_rm69330_qspi_cmd_param(0x67, 0x00);

    rtl_rm69330_qspi_cmd_param(0xFE, 0x07);

    rtl_rm69330_qspi_cmd_param(0x15, 0x04);

    //rtl_rm69330_qspi_cmd_param(0xFE,0x0a);
    //rtl_rm69330_qspi_cmd_param(0xc6,0x1f);
    //rtl_rm69330_qspi_cmd_param(0xc7,0x80);

    rtl_rm69330_qspi_cmd_param(0xFE, 0x00);

    rtl_rm69330_qspi_cmd_param(0xC4, 0x80);
    rtl_rm69330_qspi_cmd_param(0x35, 0x00);


    rtl_rm69330_qspi_cmd_param(0x51, 0xAF);
    rtl_rm69330_qspi_cmd_param(0x3A, 0x55); //RGB 565
    //rtl_rm69330_qspi_cmd_param(0x3A,0x77);   //RGB888

    rtl_rm69330_qspi_write_cmd(0x11);
    rtl_rm69330_qspi_write_cmd(0x29);
}







