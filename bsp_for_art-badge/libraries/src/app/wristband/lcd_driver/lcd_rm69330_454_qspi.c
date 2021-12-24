#include "board.h"
#include "app_section.h"
#include "wristband_gui.h"
#include "rtl876x_if8080.h"
#include "trace.h"
#include "wristband_gui.h"
#include "lcd_rm69330_454_qspi.h"
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
static void rtl_rm69330_qspi_write_cmd(uint16_t cmd) //total 4 byte, first byte is 0x02
{
    uint8_t sdat[] = {0x02, 0x00, cmd, 0x00};
    spic2_spi_write(sdat, sizeof(sdat));
}


DATA_RAM_FUNCTION
static void rtl_rm69330_qspi_cmd_param(uint8_t cmd, uint8_t data) //total 5 byte, first byte is 0x02
{
    uint8_t sdat[] = {0x02, 0x00, cmd, 0x00, data};
    spic2_spi_write(sdat, sizeof(sdat));
}

DATA_RAM_FUNCTION
static void rtl_rm69330_qspi_cmd_param4(uint8_t cmd,
                                        uint8_t *data) //total 8 byte, first byte is 0x02
{
    uint8_t sdat[] = {0x02, 0x00, cmd, 0x00, data[0], data[1], data[2], data[3]};
    spic2_spi_write(sdat, sizeof(sdat));
}

DATA_RAM_FUNCTION
static void rtl_rm69330_qspi_enter_data_output_mode(void) //total 4 byte, first byte is 0x32
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

void lcd_rm69330_qspi_454_power_on(void)
{
    rtl_rm69330_qspi_write_cmd(0x11);
    rtl_rm69330_qspi_write_cmd(0x29);
}

void lcd_rm69330_qspi_454_power_off(void)
{
    rtl_rm69330_qspi_write_cmd(0x28);
    rtl_rm69330_qspi_write_cmd(0x10);
}

void lcd_rm69330_qspi_454_set_window(uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd)
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
    rtl_rm69330_qspi_cmd_param4(0x2A, data);


    data[0] = yStart >> 8;
    data[1] = yStart & 0xff;
    data[2] = yEnd >> 8;
    data[3] = yEnd & 0xff;
    rtl_rm69330_qspi_cmd_param4(0x2B, data);

    rtl_rm69330_qspi_enter_data_output_mode();
}

void lcd_rm69330_qspi_454_init(void)
{
    rtl_rm69330_qspi_cmd_param(0xFE, 0x01);
    rtl_rm69330_qspi_cmd_param(0x05, 0x00);
    rtl_rm69330_qspi_cmd_param(0x06, 0x72);
    rtl_rm69330_qspi_cmd_param(0x0D, 0x00);
    rtl_rm69330_qspi_cmd_param(0x0E, 0x81); //AVDD=6V
    rtl_rm69330_qspi_cmd_param(0x0F, 0x81);
    rtl_rm69330_qspi_cmd_param(0x10, 0x11); //AVDD=3VCI
    rtl_rm69330_qspi_cmd_param(0x11, 0x81); //VCL=-VCI
    rtl_rm69330_qspi_cmd_param(0x12, 0x81);
    rtl_rm69330_qspi_cmd_param(0x13, 0x80); //VGH=AVDD
    rtl_rm69330_qspi_cmd_param(0x14, 0x80);
    rtl_rm69330_qspi_cmd_param(0x15, 0x81); //VGL=
    rtl_rm69330_qspi_cmd_param(0x16, 0x81);
    rtl_rm69330_qspi_cmd_param(0x18, 0x66); //VGHR=6V
    rtl_rm69330_qspi_cmd_param(0x19, 0x88); //VGLR=-6V
    rtl_rm69330_qspi_cmd_param(0x5B, 0x10); //VREFPN5 Regulator Enable
    rtl_rm69330_qspi_cmd_param(0x5C, 0x55);
    rtl_rm69330_qspi_cmd_param(0x62, 0x19); //Normal VREFN
    rtl_rm69330_qspi_cmd_param(0x63, 0x19); //Idle VREFN
    rtl_rm69330_qspi_cmd_param(0x70, 0x54); //
    rtl_rm69330_qspi_cmd_param(0x74, 0x0C);
    rtl_rm69330_qspi_cmd_param(0xC5, 0x10); // NOR=IDLE=GAM1 // HBM=GAM2


    rtl_rm69330_qspi_cmd_param(0xFE, 0x01);
    rtl_rm69330_qspi_cmd_param(0x25, 0x03);
    rtl_rm69330_qspi_cmd_param(0x26, 0x32);
    rtl_rm69330_qspi_cmd_param(0x27, 0x0A);
    rtl_rm69330_qspi_cmd_param(0x28, 0x08);
    rtl_rm69330_qspi_cmd_param(0x2A, 0x03);
    rtl_rm69330_qspi_cmd_param(0x2B, 0x32);
    rtl_rm69330_qspi_cmd_param(0x2D, 0x0A);
    rtl_rm69330_qspi_cmd_param(0x2F, 0x08);
    rtl_rm69330_qspi_cmd_param(0x30, 0x43); //43: 15Hz

    rtl_rm69330_qspi_cmd_param(0x66, 0x90);
    rtl_rm69330_qspi_cmd_param(0x72, 0x1A); //OVDD  4.6V
    rtl_rm69330_qspi_cmd_param(0x73, 0x13); //OVSS  -2.2V

    rtl_rm69330_qspi_cmd_param(0xFE, 0x01);
    rtl_rm69330_qspi_cmd_param(0x6A, 0x17); //RT4723  daz20013  0x17=-2.2

    rtl_rm69330_qspi_cmd_param(0x1B, 0x00);

//VSR power saving
    rtl_rm69330_qspi_cmd_param(0x1D, 0x03);
    rtl_rm69330_qspi_cmd_param(0x1E, 0x03);
    rtl_rm69330_qspi_cmd_param(0x1F, 0x03);
    rtl_rm69330_qspi_cmd_param(0x20, 0x03);
    rtl_rm69330_qspi_cmd_param(0xFE, 0x01);
    rtl_rm69330_qspi_cmd_param(0x36, 0x00);
    rtl_rm69330_qspi_cmd_param(0x6C, 0x80);
    rtl_rm69330_qspi_cmd_param(0x6D, 0x19); //VGMP VGSP turn off at idle mode

    rtl_rm69330_qspi_cmd_param(0xFE, 0x04);
    rtl_rm69330_qspi_cmd_param(0x63, 0x00);
    rtl_rm69330_qspi_cmd_param(0x64, 0x0E);

//Gamma1 - AOD/Normal
    rtl_rm69330_qspi_cmd_param(0xFE, 0x02);
    rtl_rm69330_qspi_cmd_param(0xA9, 0x30); //5.8V VGMP
    rtl_rm69330_qspi_cmd_param(0xAA, 0xB9); //2.5V VGSP
    rtl_rm69330_qspi_cmd_param(0xAB, 0x01);


//Gamma2 - HBM
    rtl_rm69330_qspi_cmd_param(0xFE, 0x03); //page2
    rtl_rm69330_qspi_cmd_param(0xA9, 0x30); //5.8V VGMP
    rtl_rm69330_qspi_cmd_param(0xAA, 0x90); //2V VGSP
    rtl_rm69330_qspi_cmd_param(0xAB, 0x01);

//SW MAPPING
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


    rtl_rm69330_qspi_cmd_param(0x4E, 0x16);
    rtl_rm69330_qspi_cmd_param(0x4F, 0x61);
    rtl_rm69330_qspi_cmd_param(0x50, 0x25);
    rtl_rm69330_qspi_cmd_param(0x51, 0x52);
    rtl_rm69330_qspi_cmd_param(0x52, 0x34);
    rtl_rm69330_qspi_cmd_param(0x53, 0x43);

    rtl_rm69330_qspi_cmd_param(0x54, 0x61);
    rtl_rm69330_qspi_cmd_param(0x55, 0x16);
    rtl_rm69330_qspi_cmd_param(0x56, 0x52);
    rtl_rm69330_qspi_cmd_param(0x57, 0x25);
    rtl_rm69330_qspi_cmd_param(0x58, 0x43);
    rtl_rm69330_qspi_cmd_param(0x59, 0x34);
//Switch Timing Control
    rtl_rm69330_qspi_cmd_param(0xFE, 0x01);
    rtl_rm69330_qspi_cmd_param(0x3A, 0x00);
    rtl_rm69330_qspi_cmd_param(0x3B, 0x00);
    rtl_rm69330_qspi_cmd_param(0x3D, 0x12);
    rtl_rm69330_qspi_cmd_param(0x3F, 0x37);
    rtl_rm69330_qspi_cmd_param(0x40, 0x12);
    rtl_rm69330_qspi_cmd_param(0x41, 0x0F);
    rtl_rm69330_qspi_cmd_param(0x37, 0x0C);

    rtl_rm69330_qspi_cmd_param(0xFE, 0x04);
    rtl_rm69330_qspi_cmd_param(0x5D, 0x01);
    rtl_rm69330_qspi_cmd_param(0x75, 0x08);

//VSR Marping command---L
    rtl_rm69330_qspi_cmd_param(0xFE, 0x04);
    rtl_rm69330_qspi_cmd_param(0x5E, 0x0F);
    rtl_rm69330_qspi_cmd_param(0x5F, 0x12);
    rtl_rm69330_qspi_cmd_param(0x60, 0xFF);
    rtl_rm69330_qspi_cmd_param(0x61, 0xFF);
    rtl_rm69330_qspi_cmd_param(0x62, 0xFF);

//   VSR Marping command---R
    rtl_rm69330_qspi_cmd_param(0xFE, 0x04);
    rtl_rm69330_qspi_cmd_param(0x76, 0xFF);
    rtl_rm69330_qspi_cmd_param(0x77, 0xFF);
    rtl_rm69330_qspi_cmd_param(0x78, 0x49);
    rtl_rm69330_qspi_cmd_param(0x79, 0xF3);
    rtl_rm69330_qspi_cmd_param(0x7A, 0xFF);

//VSR1-STV
    rtl_rm69330_qspi_cmd_param(0xFE, 0x04);
    rtl_rm69330_qspi_cmd_param(0x00, 0x9D);
    rtl_rm69330_qspi_cmd_param(0x01, 0x00);
    rtl_rm69330_qspi_cmd_param(0x02, 0x00);
    rtl_rm69330_qspi_cmd_param(0x03, 0x00);
    rtl_rm69330_qspi_cmd_param(0x04, 0x00);
    rtl_rm69330_qspi_cmd_param(0x05, 0x01);
    rtl_rm69330_qspi_cmd_param(0x06, 0x01);
    rtl_rm69330_qspi_cmd_param(0x07, 0x01);
    rtl_rm69330_qspi_cmd_param(0x08, 0x00);

//VSR2-SCK1
    rtl_rm69330_qspi_cmd_param(0xFE, 0x04);
    rtl_rm69330_qspi_cmd_param(0x09, 0xDC);
    rtl_rm69330_qspi_cmd_param(0x0A, 0x00);
    rtl_rm69330_qspi_cmd_param(0x0B, 0x02);
    rtl_rm69330_qspi_cmd_param(0x0C, 0x00);
    rtl_rm69330_qspi_cmd_param(0x0D, 0x08);
    rtl_rm69330_qspi_cmd_param(0x0E, 0x01);
    rtl_rm69330_qspi_cmd_param(0x0F, 0xCE);
    rtl_rm69330_qspi_cmd_param(0x10, 0x16);
    rtl_rm69330_qspi_cmd_param(0x11, 0x00);

//VSR3-SCK2
    rtl_rm69330_qspi_cmd_param(0xFE, 0x04);
    rtl_rm69330_qspi_cmd_param(0x12, 0xDC);
    rtl_rm69330_qspi_cmd_param(0x13, 0x00);
    rtl_rm69330_qspi_cmd_param(0x14, 0x02);
    rtl_rm69330_qspi_cmd_param(0x15, 0x00);
    rtl_rm69330_qspi_cmd_param(0x16, 0x08);
    rtl_rm69330_qspi_cmd_param(0x17, 0x02);
    rtl_rm69330_qspi_cmd_param(0x18, 0xCE);
    rtl_rm69330_qspi_cmd_param(0x19, 0x16);
    rtl_rm69330_qspi_cmd_param(0x1A, 0x00);

//VSR4-ECK1
    rtl_rm69330_qspi_cmd_param(0xFE, 0x04);
    rtl_rm69330_qspi_cmd_param(0x1B, 0xDC);
    rtl_rm69330_qspi_cmd_param(0x1C, 0x00);
    rtl_rm69330_qspi_cmd_param(0x1D, 0x02);
    rtl_rm69330_qspi_cmd_param(0x1E, 0x00);
    rtl_rm69330_qspi_cmd_param(0x1F, 0x08);
    rtl_rm69330_qspi_cmd_param(0x20, 0x01);
    rtl_rm69330_qspi_cmd_param(0x21, 0xCE);
    rtl_rm69330_qspi_cmd_param(0x22, 0x16);
    rtl_rm69330_qspi_cmd_param(0x23, 0x00);

//VSR5-ECK2
    rtl_rm69330_qspi_cmd_param(0xFE, 0x04);
    rtl_rm69330_qspi_cmd_param(0x24, 0xDC);
    rtl_rm69330_qspi_cmd_param(0x25, 0x00);
    rtl_rm69330_qspi_cmd_param(0x26, 0x02);
    rtl_rm69330_qspi_cmd_param(0x27, 0x00);
    rtl_rm69330_qspi_cmd_param(0x28, 0x08);
    rtl_rm69330_qspi_cmd_param(0x29, 0x02);
    rtl_rm69330_qspi_cmd_param(0x2A, 0xCE);
    rtl_rm69330_qspi_cmd_param(0x2B, 0x16);
    rtl_rm69330_qspi_cmd_param(0x2D, 0x00);

//VEN EM-STV
    rtl_rm69330_qspi_cmd_param(0xFE, 0x04);
    rtl_rm69330_qspi_cmd_param(0x53, 0x8a);
    rtl_rm69330_qspi_cmd_param(0x54, 0x00);
    rtl_rm69330_qspi_cmd_param(0x55, 0x03);
    rtl_rm69330_qspi_cmd_param(0x56, 0x01);
    rtl_rm69330_qspi_cmd_param(0x58, 0x01);
    rtl_rm69330_qspi_cmd_param(0x59, 0x00);
    rtl_rm69330_qspi_cmd_param(0x65, 0x76);
    rtl_rm69330_qspi_cmd_param(0x66, 0x19);
    rtl_rm69330_qspi_cmd_param(0x67, 0x00);

    rtl_rm69330_qspi_cmd_param(0xFE, 0x07);
    rtl_rm69330_qspi_cmd_param(0x15, 0x04);

    rtl_rm69330_qspi_cmd_param(0xFE, 0x05);
    rtl_rm69330_qspi_cmd_param(0x4C, 0x01);
    rtl_rm69330_qspi_cmd_param(0x4D, 0x82);
    rtl_rm69330_qspi_cmd_param(0x4E, 0x04);
    rtl_rm69330_qspi_cmd_param(0x4F, 0x00);
    rtl_rm69330_qspi_cmd_param(0x50, 0x20);
    rtl_rm69330_qspi_cmd_param(0x51, 0x10);
    rtl_rm69330_qspi_cmd_param(0x52, 0x04);
    rtl_rm69330_qspi_cmd_param(0x53, 0x41);
    rtl_rm69330_qspi_cmd_param(0x54, 0x0A);
    rtl_rm69330_qspi_cmd_param(0x55, 0x08);
    rtl_rm69330_qspi_cmd_param(0x56, 0x00);
    rtl_rm69330_qspi_cmd_param(0x57, 0x28);
    rtl_rm69330_qspi_cmd_param(0x58, 0x00);
    rtl_rm69330_qspi_cmd_param(0x59, 0x80);
    rtl_rm69330_qspi_cmd_param(0x5A, 0x04);
    rtl_rm69330_qspi_cmd_param(0x5B, 0x10);
    rtl_rm69330_qspi_cmd_param(0x5C, 0x20);
    rtl_rm69330_qspi_cmd_param(0x5D, 0x00);
    rtl_rm69330_qspi_cmd_param(0x5E, 0x04);
    rtl_rm69330_qspi_cmd_param(0x5F, 0x0A);
    rtl_rm69330_qspi_cmd_param(0x60, 0x01);
    rtl_rm69330_qspi_cmd_param(0x61, 0x08);
    rtl_rm69330_qspi_cmd_param(0x62, 0x00);
    rtl_rm69330_qspi_cmd_param(0x63, 0x20);
    rtl_rm69330_qspi_cmd_param(0x64, 0x40);
    rtl_rm69330_qspi_cmd_param(0x65, 0x04);
    rtl_rm69330_qspi_cmd_param(0x66, 0x02);
    rtl_rm69330_qspi_cmd_param(0x67, 0x48);
    rtl_rm69330_qspi_cmd_param(0x68, 0x4C);
    rtl_rm69330_qspi_cmd_param(0x69, 0x02);
    rtl_rm69330_qspi_cmd_param(0x6A, 0x12);
    rtl_rm69330_qspi_cmd_param(0x6B, 0x00);
    rtl_rm69330_qspi_cmd_param(0x6C, 0x48);
    rtl_rm69330_qspi_cmd_param(0x6D, 0xA0);
    rtl_rm69330_qspi_cmd_param(0x6E, 0x08);
    rtl_rm69330_qspi_cmd_param(0x6F, 0x04);
    rtl_rm69330_qspi_cmd_param(0x70, 0x05);
    rtl_rm69330_qspi_cmd_param(0x71, 0x92);
    rtl_rm69330_qspi_cmd_param(0x72, 0x00);
    rtl_rm69330_qspi_cmd_param(0x73, 0x18);
    rtl_rm69330_qspi_cmd_param(0x74, 0xA0);
    rtl_rm69330_qspi_cmd_param(0x75, 0x00);
    rtl_rm69330_qspi_cmd_param(0x76, 0x00);
    rtl_rm69330_qspi_cmd_param(0x77, 0xE4);
    rtl_rm69330_qspi_cmd_param(0x78, 0x00);
    rtl_rm69330_qspi_cmd_param(0x79, 0x04);
    rtl_rm69330_qspi_cmd_param(0x7A, 0x02);
    rtl_rm69330_qspi_cmd_param(0x7B, 0x01);
    rtl_rm69330_qspi_cmd_param(0x7C, 0x00);
    rtl_rm69330_qspi_cmd_param(0x7D, 0x00);
    rtl_rm69330_qspi_cmd_param(0x7E, 0x24);
    rtl_rm69330_qspi_cmd_param(0x7F, 0x4C);
    rtl_rm69330_qspi_cmd_param(0x80, 0x04);
    rtl_rm69330_qspi_cmd_param(0x81, 0x0A);
    rtl_rm69330_qspi_cmd_param(0x82, 0x02);
    rtl_rm69330_qspi_cmd_param(0x83, 0xC1);
    rtl_rm69330_qspi_cmd_param(0x84, 0x02);
    rtl_rm69330_qspi_cmd_param(0x85, 0x18);
    rtl_rm69330_qspi_cmd_param(0x86, 0x90);
    rtl_rm69330_qspi_cmd_param(0x87, 0x60);
    rtl_rm69330_qspi_cmd_param(0x88, 0x88);
    rtl_rm69330_qspi_cmd_param(0x89, 0x02);
    rtl_rm69330_qspi_cmd_param(0x8A, 0x09);
    rtl_rm69330_qspi_cmd_param(0x8B, 0x0C);
    rtl_rm69330_qspi_cmd_param(0x8C, 0x18);
    rtl_rm69330_qspi_cmd_param(0x8D, 0x90);
    rtl_rm69330_qspi_cmd_param(0x8E, 0x10);
    rtl_rm69330_qspi_cmd_param(0x8F, 0x08);
    rtl_rm69330_qspi_cmd_param(0x90, 0x00);
    rtl_rm69330_qspi_cmd_param(0x91, 0x10);
    rtl_rm69330_qspi_cmd_param(0x92, 0xA8);
    rtl_rm69330_qspi_cmd_param(0x93, 0x00);
    rtl_rm69330_qspi_cmd_param(0x94, 0x04);
    rtl_rm69330_qspi_cmd_param(0x95, 0x0A);
    rtl_rm69330_qspi_cmd_param(0x96, 0x00);
    rtl_rm69330_qspi_cmd_param(0x97, 0x08);
    rtl_rm69330_qspi_cmd_param(0x98, 0x10);
    rtl_rm69330_qspi_cmd_param(0x99, 0x28);
    rtl_rm69330_qspi_cmd_param(0x9A, 0x08);
    rtl_rm69330_qspi_cmd_param(0x9B, 0x04);
    rtl_rm69330_qspi_cmd_param(0x9C, 0x02);
    rtl_rm69330_qspi_cmd_param(0x9D, 0x03);

    rtl_rm69330_qspi_cmd_param(0xFE, 0x0C);
    rtl_rm69330_qspi_cmd_param(0x25, 0x00);
    rtl_rm69330_qspi_cmd_param(0x31, 0xEF);
    rtl_rm69330_qspi_cmd_param(0x32, 0xE3);
    rtl_rm69330_qspi_cmd_param(0x33, 0x00);
    rtl_rm69330_qspi_cmd_param(0x34, 0xE3);
    rtl_rm69330_qspi_cmd_param(0x35, 0xE3);
    rtl_rm69330_qspi_cmd_param(0x36, 0x80);
    rtl_rm69330_qspi_cmd_param(0x37, 0x00);
    rtl_rm69330_qspi_cmd_param(0x38, 0x79);
    rtl_rm69330_qspi_cmd_param(0x39, 0x00);
    rtl_rm69330_qspi_cmd_param(0x3A, 0x00);
    rtl_rm69330_qspi_cmd_param(0x3B, 0x00);
    rtl_rm69330_qspi_cmd_param(0x3D, 0x00);
    rtl_rm69330_qspi_cmd_param(0x3F, 0x00);
    rtl_rm69330_qspi_cmd_param(0x40, 0x00);
    rtl_rm69330_qspi_cmd_param(0x41, 0x00);
    rtl_rm69330_qspi_cmd_param(0x42, 0x00);
    rtl_rm69330_qspi_cmd_param(0x43, 0x01);

    rtl_rm69330_qspi_cmd_param(0xFE, 0x00);
    rtl_rm69330_qspi_cmd_param(0x35, 0x00);
    rtl_rm69330_qspi_cmd_param(0xC4, 0x80);

    //set scan line by howie
    rtl_rm69330_qspi_cmd_param(0x44, 0x0C);
    rtl_rm69330_qspi_cmd_param(0x44, 0x00);
//rtl_rm69330_qspi_cmd_param(0x51,0xFF);


    rtl_rm69330_qspi_cmd_param(0x51, 0xAF);
    rtl_rm69330_qspi_cmd_param(0x3A, 0x55); //RGB 565
    //rtl_rm69330_qspi_cmd_param(0x3A,0x77);   //RGB888

    rtl_rm69330_qspi_write_cmd(0x11);
    rtl_rm69330_qspi_write_cmd(0x29);
}







