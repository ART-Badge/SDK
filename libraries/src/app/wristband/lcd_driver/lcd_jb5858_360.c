#include "board.h"
#include "app_section.h"
#include "wristband_gui.h"
#include "rtl876x_if8080.h"
#include "trace.h"
#include "wristband_gui.h"
#include "lcd_jb5858_360.h"
#include "platform_utils.h"
#include "module_lcd_8080.h"

DATA_RAM_FUNCTION
void jb5858_write_cmd(uint8_t command)
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
void jb5858_write_data(uint8_t data)
{
    IF8080_SendData(&data, 1);
}

DATA_RAM_FUNCTION
void jb5858_read_data(uint8_t cmd, uint8_t *pBuf, uint32_t len)
{
    IF8080_SwitchMode(IF8080_MODE_MANUAL);
    IF8080_ResetCS();

    /* Send command */
    IF8080_SendCommand(cmd);

    /* Read data */
    IF8080_ReceiveData(pBuf, len);

    /* Pull CS up */
    IF8080_SetCS();
}

void lcd_jb5858_power_on(void)
{
    jb5858_write_cmd(0x29);
    lcd_set_backlight(100);
}

void lcd_jb5858_power_off(void)
{
    jb5858_write_cmd(0x28);
    lcd_set_backlight(0);
}

void lcd_jb5858_set_window(uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd)
{
    IF8080_SwitchMode(IF8080_MODE_MANUAL);

    jb5858_write_cmd(0x2a);
    jb5858_write_data(xStart >> 8);
    jb5858_write_data(xStart & 0xff);
    jb5858_write_data(xEnd >> 8);
    jb5858_write_data(xEnd & 0xff);

    jb5858_write_cmd(0x2b);
    jb5858_write_data(yStart >> 8);
    jb5858_write_data(yStart & 0xff);
    jb5858_write_data(yEnd >> 8);
    jb5858_write_data(yEnd & 0xff);
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

void lcd_jb5858_set_window_read(uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd)
{
    IF8080_SwitchMode(IF8080_MODE_MANUAL);

    jb5858_write_cmd(0x2a);
    jb5858_write_data(xStart >> 8);
    jb5858_write_data(xStart & 0xff);
    jb5858_write_data(xEnd >> 8);
    jb5858_write_data(xEnd & 0xff);

    jb5858_write_cmd(0x2b);
    jb5858_write_data(yStart >> 8);
    jb5858_write_data(yStart & 0xff);
    jb5858_write_data(yEnd >> 8);
    jb5858_write_data(yEnd & 0xff);
    IF8080_SetCS();
    /* Enable Auto mode */
    IF8080_SwitchMode(IF8080_MODE_AUTO);

    IF8080_ClearRxCounter();

    /* Configure command */
    uint8_t cmd[1] = {0x2e};
    IF8080_SetCmdSequence(cmd, 1);

    /* Enable GDMA TX */
    IF8080_GDMACmd(ENABLE);

    /* Configure pixel number */
    uint32_t len = (xEnd - xStart + 1) * (yEnd - yStart + 1) * PIXEL_BYTES;
    IF8080_SetRxDataLen(len);

    /* Start output */
    IF8080_AutoModeCmd(IF8080_Auto_Mode_Direction_READ, ENABLE);
}


void lcd_jb5858_init(void)
{
//-----initial JD5858 INX 1p3 360x360 8080 MCU -----//
//PASSWORD
    jb5858_write_cmd(0xDF); //Password
    jb5858_write_data(0x58);
    jb5858_write_data(0x58);
    jb5858_write_data(0xB0);

//---------------- PAGE0 --------------
    jb5858_write_cmd(0xDE);
    jb5858_write_data(0x00);


//VCOM_SET
    jb5858_write_cmd(0xB2);
    jb5858_write_data(0x01);
    jb5858_write_data(0x18); //VCOM

//Gamma_Set
    jb5858_write_cmd(0xB7);
    jb5858_write_data(0x10); //VGMP = +5.3V 0x14A
    jb5858_write_data(0x4A);
    jb5858_write_data(0x00); //VGSP = +0.0V
    jb5858_write_data(0x10); //VGMN = -5.3V 0x14A
    jb5858_write_data(0x4A);
    jb5858_write_data(0x00); //VGSN = -0.0V

//DCDC_SEL
    jb5858_write_cmd(0xBB);
    jb5858_write_data(
        0x01); //VGH = AVDD+VCI = 5.8V+3.1V= 8.9V ;VGL = -1*VGH = -8.9V; AVDD = 2xVCI = 3.1V*2 = 6.2V
    jb5858_write_data(0x1D); //AVDD_S = +5.8V (0x1D) ; AVEE = -1xAVDD_S = -5.8V
    jb5858_write_data(0x43);
    jb5858_write_data(0x43);
    jb5858_write_data(0x21);
    jb5858_write_data(0x21);

//GATE_POWER
    jb5858_write_cmd(0xCF);
    jb5858_write_data(0x20); //VGHO = +8V
    jb5858_write_data(0x50); //VGLO = -8V
    jb5858_write_data(0x30);
    jb5858_write_data(0x0A);


//SET_R_GAMMA
    jb5858_write_cmd(0xC8);
    jb5858_write_data(0x7F);
    jb5858_write_data(0x54);
    jb5858_write_data(0x3C);
    jb5858_write_data(0x2D);
    jb5858_write_data(0x24);
    jb5858_write_data(0x15);
    jb5858_write_data(0x1A);
    jb5858_write_data(0x07);
    jb5858_write_data(0x24);
    jb5858_write_data(0x28);
    jb5858_write_data(0x2B);
    jb5858_write_data(0x4C);
    jb5858_write_data(0x3B);
    jb5858_write_data(0x45);
    jb5858_write_data(0x3A);
    jb5858_write_data(0x35);
    jb5858_write_data(0x2C);
    jb5858_write_data(0x1E);
    jb5858_write_data(0x01);
    jb5858_write_data(0x7F);
    jb5858_write_data(0x54);
    jb5858_write_data(0x3C);
    jb5858_write_data(0x2D);
    jb5858_write_data(0x24);
    jb5858_write_data(0x15);
    jb5858_write_data(0x1A);
    jb5858_write_data(0x07);
    jb5858_write_data(0x24);
    jb5858_write_data(0x28);
    jb5858_write_data(0x2B);
    jb5858_write_data(0x4C);
    jb5858_write_data(0x3B);
    jb5858_write_data(0x45);
    jb5858_write_data(0x3A);
    jb5858_write_data(0x35);
    jb5858_write_data(0x2C);
    jb5858_write_data(0x1E);
    jb5858_write_data(0x01);

//-----------------------------
// SET page4 TCON & GIP
//------------------------------
    jb5858_write_cmd(0xDE);
    jb5858_write_data(0x04);  // page4

//SETSTBA
    jb5858_write_cmd(0xB2);
    jb5858_write_data(0x14); //GAP = 1 ;SAP= 4
    jb5858_write_data(0x14);

//SETSTBA2
    jb5858_write_cmd(0xB3);
    jb5858_write_data(0x00);
    jb5858_write_data(0x40);
    jb5858_write_data(0xD0);
    jb5858_write_data(0x0A);
    jb5858_write_data(0xA0); //SDS[27:25] Res size selection ;3'b000 R=26k
    jb5858_write_data(0x20);
    jb5858_write_data(0x4F);
    jb5858_write_data(0xF1);

//SETRGBCYC1
    jb5858_write_cmd(0xB8);
    jb5858_write_data(0x74); //-   NEQ PEQ[1:0] -  RGB_INV_NP[2:0]
    jb5858_write_data(0x44); //-   RGB_INV_PI[2:0] -   RGB_INV_I[2:0]
    jb5858_write_data(0x00); //RGB_N_T2[11:8],RGB_N_T1[11:8]
    jb5858_write_data(0x01); //RGB_N_T1[7:0],
    jb5858_write_data(0x01); //RGB_N_T2[7:0],
    jb5858_write_data(0x00); //RGB_N_T4[11:8],RGB_N_T3[11:8]
    jb5858_write_data(0x01); //RGB_N_T3[7:0],
    jb5858_write_data(0x01); //RGB_N_T4[7:0],
    jb5858_write_data(0x00); //RGB_N_T6[11:8],RGB_N_T5[11:8]
    jb5858_write_data(0x09); //RGB_N_T5[7:0],
    jb5858_write_data(0x7C); //RGB_N_T6[7:0],
    jb5858_write_data(0x00); //RGB_N_T8[11:8],RGB_N_T7[11:8]
    jb5858_write_data(0x81); //RGB_N_T7[7:0],
    jb5858_write_data(0xF4); //RGB_N_T8[7:0],
    jb5858_write_data(0x10); //RGB_N_T10[11:8],RGB_N_T9[11:8]
    jb5858_write_data(0xF9); //RGB_N_T9[7:0],
    jb5858_write_data(0x6C); //RGB_N_T10[7:0],
    jb5858_write_data(0x11); //RGB_N_T12[11:8],RGB_N_T11[11:8]
    jb5858_write_data(0x71); //RGB_N_T11[7:0],
    jb5858_write_data(0xE4); //RGB_N_T12[7:0],
    jb5858_write_data(0x21); //RGB_N_T14[11:8],RGB_N_T13[11:8]
    jb5858_write_data(0xE9); //RGB_N_T13[7:0],
    jb5858_write_data(0x5C); //RGB_N_T14[7:0],
    jb5858_write_data(0x22); //RGB_N_T16[11:8],RGB_N_T15[11:8]
    jb5858_write_data(0x61); //RGB_N_T15[7:0],
    jb5858_write_data(0xD4); //RGB_N_T16[7:0],
    jb5858_write_data(0x00); //RGB_N_T18[11:8],RGB_N_T17[11:8]
    jb5858_write_data(0x00); //RGB_N_T17[7:0],
    jb5858_write_data(0x00); //RGB_N_T18[7:0],


//SETRGBCYC2
    jb5858_write_cmd(0xB9);
    jb5858_write_data(0x40); //-,ENJDT,RGB_JDT2[2:0],ENP_LINE_INV,ENP_FRM_SEL[1:0],
    jb5858_write_data(0x22); //RGB_N_T20[11:8],RGB_N_T19[11:8],
    jb5858_write_data(0x08); //RGB_N_T19[7:0],
    jb5858_write_data(0x3A); //RGB_N_T20[7:0],
    jb5858_write_data(0x22); //RGB_N_T22[11:8],RGB_N_T21[11:8],
    jb5858_write_data(0x4B); //RGB_N_T21[7:0],
    jb5858_write_data(0x7D); //RGB_N_T22[7:0],
    jb5858_write_data(0x22); //RGB_N_T24[11:8],RGB_N_T23[11:8],
    jb5858_write_data(0x8D); //RGB_N_T23[7:0],
    jb5858_write_data(0xBF); //RGB_N_T24[7:0],
    jb5858_write_data(0x32); //RGB_N_T26[11:8],RGB_N_T25[11:8],
    jb5858_write_data(0xD0); //RGB_N_T25[7:0],
    jb5858_write_data(0x02); //RGB_N_T26[7:0],
    jb5858_write_data(0x33); //RGB_N_T28[11:8],RGB_N_T27[11:8],
    jb5858_write_data(0x12); //RGB_N_T27[7:0],
    jb5858_write_data(0x44); //RGB_N_T28[7:0],
    jb5858_write_data(0x00); //-,-,-,-,RGB_N_TA1[11:8],
    jb5858_write_data(0x0A); //RGB_N_TA1[7:0],
    jb5858_write_data(0x00); //RGB_N_TA3[11:8],RGB_N_TA2[11:8],
    jb5858_write_data(0x0A); //RGB_N_TA2[7:0],
    jb5858_write_data(0x0A); //RGB_N_TA3[7:0],
    jb5858_write_data(0x00); //RGB_N_TA5[11:8],RGB_N_TA4[11:8],
    jb5858_write_data(0x0A); //RGB_N_TA4[7:0],
    jb5858_write_data(0x0A); //RGB_N_TA5[7:0],
    jb5858_write_data(0x00); //RGB_N_TA7[11:8],RGB_N_TA6[11:8],
    jb5858_write_data(0x0A); //RGB_N_TA6[7:0],
    jb5858_write_data(0x0A); //RGB_N_TA7[7:0],


//SETRGBCYC3
    jb5858_write_cmd(0xBA);
    jb5858_write_data(0x00);//-    -   -   -   -   -   -   -
    jb5858_write_data(0x00);//RGB_N_TA9[11:8],RGB_N_TA8[11:8]
    jb5858_write_data(0x07);//RGB_N_TA8[7:0],
    jb5858_write_data(0x07);//RGB_N_TA9[7:0],
    jb5858_write_data(0x00);//RGB_N_TA11[11:8],RGB_N_TA10[11:8]
    jb5858_write_data(0x07);//RGB_N_TA10[7:0],
    jb5858_write_data(0x07);//RGB_N_TA11[7:0],
    jb5858_write_data(0x00);//RGB_N_TA13[11:8],RGB_N_TA12[11:8]
    jb5858_write_data(0x07);//RGB_N_TA12[7:0],
    jb5858_write_data(0x07);//RGB_N_TA13[7:0],
    jb5858_write_data(0x00);//RGB_N_TC[11:8],RGB_N_TB[11:8]
    jb5858_write_data(0x02);//RGB_N_TB[7:0],
    jb5858_write_data(0x0D);//RGB_N_TC[7:0],
    jb5858_write_data(0x00);//RGB_N_TE[11:8],RGB_N_TD[11:8]
    jb5858_write_data(0x0A);//RGB_N_TD[7:0],
    jb5858_write_data(0x01);//RGB_N_TE[7:0],
    jb5858_write_data(0x00);//-    -   -   -   RGB_N_TF[11:8]
    jb5858_write_data(0x01);//RGB_N_TF[7:0],
    jb5858_write_data(0x30);//RGB_CHGEN_OFF[11:8],RGB_CHGEN_ON[11:8]
    jb5858_write_data(0x01);//RGB_CHGEN_ON[7:0],
    jb5858_write_data(0x40);//RGB_CHGEN_OFF[7:0],
    jb5858_write_data(0x30);//RES_MUX_OFF[11:8],RES_MUX_ON[11:8]
    jb5858_write_data(0x01);//RES_MUX_ON[7:0],
    jb5858_write_data(0x3A);//RES_MUX_OFF[7:0],
    jb5858_write_data(0x00);//-    -   -   L2_COND1_INV[12:8],
    jb5858_write_data(0x00);//-    -   -   L2_COND0_INV[12:8],
    jb5858_write_data(0x00);//L2_COND0_INV[7:0],
    jb5858_write_data(0x00);//L2_COND1_INV[7:0],


//SET_TCON
    jb5858_write_cmd(0xBC);
    jb5858_write_data(0x1A);//1  MUX_SEL =1:6 ,RSO = 360H
    jb5858_write_data(0x00);//2  LN_NO_MUL2 = 0:Gate line number=LN[10:0]*2 ,LN[10:8] = 0
    jb5858_write_data(0xB4);//3  LN[7:0] =180*2 = 360
    jb5858_write_data(0x03);//4  PANEL[2:0] = dancing type 2
    jb5858_write_data(0x00);//5  VFP[11:8],SLT[11:8]
    jb5858_write_data(0xD0);//6  SLT[7:0] = 1/(60*(360+10+6))/4OSC(19MHZ)
    jb5858_write_data(0x08);//7  VFP[7:0] = 8
    jb5858_write_data(0x00);//8  HBP[11:8], VBP[11:8]
    jb5858_write_data(0x07);//9  VBP[7:0]
    jb5858_write_data(0x2C);//10 HBP[7:0]
    jb5858_write_data(0x00);//11 VFP_I[11:8],SLT_I[11:8]
    jb5858_write_data(0xD0);//12 SLT_I[7:0]
    jb5858_write_data(0x08);//13 VFP_I[7:0]
    jb5858_write_data(0x00);//14 HBP_I[11:8],VBP_I[11:8]
    jb5858_write_data(0x07);//15 VBP_I[7:0]
    jb5858_write_data(0x2C);//16 HBP_I[7:0]
    jb5858_write_data(0x82);//17 HBP_NCK[3:0],HFP_NCK[3:0]
    jb5858_write_data(0x00);//18 TCON_OPT1[15:8]
    jb5858_write_data(0x03);//19 TCON_OPT1[7:0]
    jb5858_write_data(0x00);//20 VFP_PI[11:8],SLT_PI[11:8]
    jb5858_write_data(0xD0);//21 SLT_PI[7:0]
    jb5858_write_data(0x08);//22 VFP_PI[7:0]
    jb5858_write_data(0x00);//23 HBP_PI[11:8],VBP_PI[11:8]
    jb5858_write_data(0x07);//24 VBP_PI[7:0]
    jb5858_write_data(0x2C);//25 HBP_PI[7:0]


//-------------------GIP----------------------
//SET_GIP_EQ
    jb5858_write_cmd(0xC4);
    jb5858_write_data(0x00);
    jb5858_write_data(0x00);
    jb5858_write_data(0x03);
    jb5858_write_data(0x07);
    jb5858_write_data(0x00);
    jb5858_write_data(0x00);
    jb5858_write_data(0x00);
    jb5858_write_data(0x00);
    jb5858_write_data(0x08);
    jb5858_write_data(0x04);
    jb5858_write_data(0x00);
    jb5858_write_data(0x03);
    jb5858_write_data(0x07);
    jb5858_write_data(0x00);
    jb5858_write_data(0x00);
    jb5858_write_data(0x00);
    jb5858_write_data(0x00);
    jb5858_write_data(0x08);
    jb5858_write_data(0x04);

//SET_GIP_L
    jb5858_write_cmd(0xC5);
    jb5858_write_data(0x00);//DUMMY
    jb5858_write_data(0x1F);//0
    jb5858_write_data(0x1F);//1
    jb5858_write_data(0x1F);//2
    jb5858_write_data(0x1E);//3 GAS
    jb5858_write_data(0xDF);//4 BGAS
    jb5858_write_data(0x1F);//5 RSTV
    jb5858_write_data(0xC7);//6 CKV4
    jb5858_write_data(0xC5);//7 CKV2
    jb5858_write_data(0x1F);//8 SB
    jb5858_write_data(0x1F);//9
    jb5858_write_data(0x1F);//10
    jb5858_write_data(0x1F);//11
    jb5858_write_data(0x1F);//12
    jb5858_write_data(0x1F);//13
    jb5858_write_data(0x1F);//14
    jb5858_write_data(0x1F);//15
    jb5858_write_data(0x1F);//16
    jb5858_write_data(0x1F);//17
    jb5858_write_data(0x1F);//18
    jb5858_write_data(0x1F);//19
    jb5858_write_data(0x1F);//20
    jb5858_write_data(0x1F);//21
    jb5858_write_data(0x1F);//22
    jb5858_write_data(0x1F);//23
    jb5858_write_data(0x1F);//24
    jb5858_write_data(0x1F);//25



//SET_GIP_R
    jb5858_write_cmd(0xC6);
    jb5858_write_data(0x00);//DUMMY
    jb5858_write_data(0x1F);//0
    jb5858_write_data(0x1F);//1
    jb5858_write_data(0x1F);//2
    jb5858_write_data(0x1F);//3
    jb5858_write_data(0x1F);//4
    jb5858_write_data(0x1F);//5
    jb5858_write_data(0x1F);//6
    jb5858_write_data(0x1F);//7
    jb5858_write_data(0x1F);//8
    jb5858_write_data(0x1F);//9   ASB
    jb5858_write_data(0x00);//10  LSTV
    jb5858_write_data(0xC4);//11  CKV1
    jb5858_write_data(0xC6);//12  CKV3
    jb5858_write_data(0xE0);//13  CKH1
    jb5858_write_data(0xE1);//14  CKH2
    jb5858_write_data(0xE2);//15  CKH3
    jb5858_write_data(0xE3);//16  CKH4
    jb5858_write_data(0xE4);//17  CKH5
    jb5858_write_data(0xE5);//18  CKH6
    jb5858_write_data(0x1F);//19
    jb5858_write_data(0x1F);//20
    jb5858_write_data(0x1F);//21
    jb5858_write_data(0x1F);//22
    jb5858_write_data(0x1F);//23
    jb5858_write_data(0x1F);//24
    jb5858_write_data(0x1F);//25



//SET_GIP_L_GS
    jb5858_write_cmd(0xC7);
    jb5858_write_data(0x00);//DUMMY
    jb5858_write_data(0x1F);//0
    jb5858_write_data(0x1F);//1
    jb5858_write_data(0x1F);//2
    jb5858_write_data(0xDE);//3 GAS
    jb5858_write_data(0x1F);//4 BGAS
    jb5858_write_data(0x00);//5 RSTV
    jb5858_write_data(0xC4);//6 CKV4
    jb5858_write_data(0xC6);//7 CKV2
    jb5858_write_data(0x1F);//8 SB
    jb5858_write_data(0x1F);//9
    jb5858_write_data(0x1F);//10
    jb5858_write_data(0x1F);//11
    jb5858_write_data(0x1F);//12
    jb5858_write_data(0x1F);//13
    jb5858_write_data(0x1F);//14
    jb5858_write_data(0x1F);//15
    jb5858_write_data(0x1F);//16
    jb5858_write_data(0x1F);//17
    jb5858_write_data(0x1F);//18
    jb5858_write_data(0x1F);//19
    jb5858_write_data(0x1F);//20
    jb5858_write_data(0x1F);//21
    jb5858_write_data(0x1F);//22
    jb5858_write_data(0x1F);//23
    jb5858_write_data(0x1F);//24
    jb5858_write_data(0x1F);//25



//SET_GIP_R_GS
    jb5858_write_cmd(0xC8);
    jb5858_write_data(0x00);//DUMMY
    jb5858_write_data(0x1F);//0
    jb5858_write_data(0x1F);//1
    jb5858_write_data(0x1F);//2
    jb5858_write_data(0x1F);//3
    jb5858_write_data(0x1F);//4
    jb5858_write_data(0x1F);//5
    jb5858_write_data(0x1F);//6
    jb5858_write_data(0x1F);//7
    jb5858_write_data(0x1F);//8
    jb5858_write_data(0x1F);//9   ASB
    jb5858_write_data(0x1F);//10  LSTV
    jb5858_write_data(0xC7);//11  CKV1
    jb5858_write_data(0xC5);//12  CKV3
    jb5858_write_data(0x20);//13  CKH1
    jb5858_write_data(0x21);//14  CKH2
    jb5858_write_data(0x22);//15  CKH3
    jb5858_write_data(0x23);//16  CKH4
    jb5858_write_data(0x24);//17  CKH5
    jb5858_write_data(0x25);//18  CKH6
    jb5858_write_data(0x1F);//19
    jb5858_write_data(0x1F);//20
    jb5858_write_data(0x1F);//21
    jb5858_write_data(0x1F);//22
    jb5858_write_data(0x1F);//23
    jb5858_write_data(0x1F);//24
    jb5858_write_data(0x1F);//25



//SETGIP1
    jb5858_write_cmd(0xC9);
    jb5858_write_data(0x00);//0
    jb5858_write_data(0x00);//1
    jb5858_write_data(0x00);//2
    jb5858_write_data(0x00);//3   L:GAS
    jb5858_write_data(0x10);//4   L:BGAS :VGH
    jb5858_write_data(0x00);//5   L:RSTV
    jb5858_write_data(0x10);//6   L:CKV4 :VGH
    jb5858_write_data(0x10);//7   L:CKV2 :VGH
    jb5858_write_data(0x00);//8   L:SB
    jb5858_write_data(0x00);//9   R:ASB
    jb5858_write_data(0x00);//10  R:LSTV
    jb5858_write_data(0x20);//11  R:CKV1 :VGH
    jb5858_write_data(0x20);//12  R:CKV3 :VGH
    jb5858_write_data(0x20);//13  R:CKH1 :VGH
    jb5858_write_data(0x20);//14  R:CKH2 :VGH
    jb5858_write_data(0x20);//15  R:CKH3 :VGH
    jb5858_write_data(0x20);//16  R:CKH4 :VGH
    jb5858_write_data(0x20);//17  R:CKH5 :VGH
    jb5858_write_data(0x20);//18  R:CKH6 :VGH
    jb5858_write_data(0x00);//19
    jb5858_write_data(0x00);//20
    jb5858_write_data(0x00);//21
    jb5858_write_data(0x00);//22
    jb5858_write_data(0x00);//23
    jb5858_write_data(0x00);//24
    jb5858_write_data(0x00);//25

//SETGIP2
    jb5858_write_cmd(0xCB);
    jb5858_write_data(0x01);//1  INIT_PORCH
    jb5858_write_data(0x10);//2  INIT_W
    jb5858_write_data(0x00);//3
    jb5858_write_data(0x00);//4
    jb5858_write_data(0x07);//5  STV_S0
    jb5858_write_data(0x01);//6
    jb5858_write_data(0x00);//7
    jb5858_write_data(0x0A);//8
    jb5858_write_data(0x00);//9  STV_NUM = 1 , STV_S1
    jb5858_write_data(0x02);//10
    jb5858_write_data(0x00);//11 STV1/0_W
    jb5858_write_data(0x00);//12 STV3/2_W
    jb5858_write_data(0x00);//13
    jb5858_write_data(0x03);//14
    jb5858_write_data(0x00);//15
    jb5858_write_data(0x00);//16
    jb5858_write_data(0x00);//17
    jb5858_write_data(0x21);//18
    jb5858_write_data(0x23);//19
    jb5858_write_data(0x30);//20 CKV_W
    jb5858_write_data(0x00);//21
    jb5858_write_data(0x08);//22 CKV_S0
    jb5858_write_data(0x04);//23 CKV0_DUM[7:0]
    jb5858_write_data(0x00);//24
    jb5858_write_data(0x00);//25
    jb5858_write_data(0x05);//26
    jb5858_write_data(0x10);//27
    jb5858_write_data(0x01);//28 //END_W
    jb5858_write_data(0x04);//29
    jb5858_write_data(0x06);//30
    jb5858_write_data(0x10);//31
    jb5858_write_data(0x10);//32


//SET_GIP_ONOFF
    jb5858_write_cmd(0xD1);
    jb5858_write_data(0x00);
    jb5858_write_data(0x00);
    jb5858_write_data(0x03);
    jb5858_write_data(0x60);
    jb5858_write_data(0x30);
    jb5858_write_data(0x03);
    jb5858_write_data(0x18);
    jb5858_write_data(0x30);//CKV0_OFF[11:8]
    jb5858_write_data(0x07);//CKV0_ON[7:0]
    jb5858_write_data(0x32);//CKV0_OFF[7:0]
    jb5858_write_data(0x30);
    jb5858_write_data(0x03);
    jb5858_write_data(0x18);
    jb5858_write_data(0x30);
    jb5858_write_data(0x03);
    jb5858_write_data(0x18);
    jb5858_write_data(0x30);
    jb5858_write_data(0x03);
    jb5858_write_data(0x18);

//SET_GIP_ONOFF_WB
    jb5858_write_cmd(0xD2);
    jb5858_write_data(0x00);
    jb5858_write_data(0x30);//STV_OFF[11:8]
    jb5858_write_data(0x07);//STV_ON[7:0]
    jb5858_write_data(0x32);//STV_OFF[7:0]
    jb5858_write_data(0x32);
    jb5858_write_data(0xBC);
    jb5858_write_data(0x20);
    jb5858_write_data(0x32);
    jb5858_write_data(0xBC);
    jb5858_write_data(0x20);
    jb5858_write_data(0x32);
    jb5858_write_data(0xBC);
    jb5858_write_data(0x20);
    jb5858_write_data(0x32);
    jb5858_write_data(0xBC);
    jb5858_write_data(0x20);
    jb5858_write_data(0x30);
    jb5858_write_data(0x10);
    jb5858_write_data(0x20);
    jb5858_write_data(0x30);
    jb5858_write_data(0x10);
    jb5858_write_data(0x20);
    jb5858_write_data(0x30);
    jb5858_write_data(0x10);
    jb5858_write_data(0x20);
    jb5858_write_data(0x30);
    jb5858_write_data(0x10);
    jb5858_write_data(0x20);




//SETGIP8_CKH1 CKH_ON/OFF_CKH0-CKH7_odd
    jb5858_write_cmd(0xD4);
    jb5858_write_data(0x00);
    jb5858_write_data(0x00); //CKH_T2_ODD[11:8],CKH_T1_ODD[11:8]
    jb5858_write_data(0x03); //CKH_T1_ODD[7:0],
    jb5858_write_data(0x14); //CKH_T2_ODD[7:0],
    jb5858_write_data(0x00); //CKH_T4_ODD[11:8],CKH_T3_ODD[11:8]
    jb5858_write_data(0x03); //CKH_T3_ODD[7:0],
    jb5858_write_data(0x20); //CKH_T4_ODD[7:0],
    jb5858_write_data(0x00); //CKH_T6_ODD[11:8],CKH_T5_ODD[11:8]
    jb5858_write_data(0x09); //CKH_T5_ODD[7:0],
    jb5858_write_data(0x7C); //CKH_T6_ODD[7:0],
    jb5858_write_data(0x00); //CKH_T8_ODD[11:8],CKH_T7_ODD[11:8]
    jb5858_write_data(0x81); //CKH_T7_ODD[7:0],
    jb5858_write_data(0xF4); //CKH_T8_ODD[7:0],
    jb5858_write_data(0x10); //CKH_T10_ODD[11:8],CKH_T9_ODD[11:8]
    jb5858_write_data(0xF9); //CKH_T9_ODD[7:0],
    jb5858_write_data(0x6C); //CKH_T10_ODD[7:0],
    jb5858_write_data(0x11); //CKH_T12_ODD[11:8],CKH_T11_ODD[11:8]
    jb5858_write_data(0x71); //CKH_T11_ODD[7:0],
    jb5858_write_data(0xE4); //CKH_T12_ODD[7:0],
    jb5858_write_data(0x21); //CKH_T14_ODD[11:8],CKH_T13_ODD[11:8]
    jb5858_write_data(0xE9); //CKH_T13_ODD[7:0],
    jb5858_write_data(0x5C); //CKH_T14_ODD[7:0],
    jb5858_write_data(0x22); //CKH_T16_ODD[11:8],CKH_T15_ODD[11:8]
    jb5858_write_data(0x61); //CKH_T15_ODD[7:0],
    jb5858_write_data(0xD4); //CKH_T16_ODD[7:0],
    jb5858_write_data(0x00); //CKH_T18_ODD[11:8],CKH_T17_ODD[11:8]
    jb5858_write_data(0x00); //CKH_T17_ODD[7:0],
    jb5858_write_data(0x00); //CKH_T18_ODD[7:0],
    jb5858_write_data(0x00); //CKH_T20_ODD[11:8],CKH_T19_ODD[11:8]
    jb5858_write_data(0x00); //CKH_T19_ODD[7:0],
    jb5858_write_data(0x00); //CKH_T20_ODD[7:0],

    jb5858_write_cmd(0x35);  //for TE ON
    jb5858_write_data(0x00);

///-----------------------------------------------------------------------------------------
//---------------- PAGE1 --------------
    jb5858_write_cmd(0xDE);
    jb5858_write_data(0x01);

////MCMD_CTRL
    jb5858_write_cmd(0xCA);
    jb5858_write_data(0x01);

//---------------- PAGE0 --------------
    jb5858_write_cmd(0xDE);
    jb5858_write_data(0x00);

//Color Pixel Format
    jb5858_write_cmd(0x3A);
    jb5858_write_data(0x05); //565

//PAGE 2
    jb5858_write_cmd(0xDE);
    jb5858_write_data(0x02);

//OSC DIV
    jb5858_write_cmd(0xC5);
    jb5858_write_data(0x0B);  //FPS 60HZ (0x03) to 30HZ (0x0B) ,47HZ(0x0F),42HZ(0x0E)

//SLP OUT
    jb5858_write_cmd(0x11);    // SLPOUT
    platform_delay_ms(120);
}

#if 0
void  test_code(void)
{

    uint8_t test[200];

    jb5858_read_data(0x04, test, 4);

    DBG_DIRECT("[JB5858] READ value = 0x%x", test[0]);
    DBG_DIRECT("[JB5858] READ value = 0x%x", test[1]);
    DBG_DIRECT("[JB5858] READ value = 0x%x", test[2]);
    DBG_DIRECT("[JB5858] READ value = 0x%x", test[3]);

    lcd_jb5858_set_window(180, 180, 189, 189);
    for (uint32_t i = 0; i < 50; i++)
    {
        IF8080->FIFO = 0xF0F0F0F0;
        while (IF8080->SR & IF8080_FLAG_TF_FULL);
    }
    while (IF8080_GetTxCounter() != 200); //LCD_SECTION_HEIGHT

    //lcd_jb5858_set_window_read(180, 180, 189, 189);
    for (uint32_t i = 0; i < 50; i++)
    {
//        while (IF8080->SR & IF8080_FLAG_RF_EMPTY);
//        uint32_t read_value = 0;
//        read_value = IF8080->FIFO;
//        DBG_DIRECT("[JB5858] READ i = %d, value = 0x%x", i, read_value);
    }
    jb5858_read_data(0x2e, test, 200);
    for (uint32_t i = 0; i < 200; i++)
    {
        DBG_DIRECT("[JB5858] READ i = %d, value = 0x%x", i, test[i]);
    }
    while (1);
}
#endif







