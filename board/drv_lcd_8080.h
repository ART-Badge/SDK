/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-11-11     stackRyan         first version
 */


#ifndef __DRV_LCD_8080_H_
#define __DRV_LCD_8080_H_

//#ifdef __cplusplus
//extern "C" {
//#endif
#include "board.h"
#include "wristband_gui.h"

#define IF8080_PINGROUP_1       0

#define LCD_8080_RST                     H_2
#define LCD_8080_BL                      P1_2 //need R15
/*because most 8080 lcd pins ara fixed*/
#define LCD_8080_D0                      P0_4
#define LCD_8080_D1                      P0_5
#define LCD_8080_D2                      P0_6
#define LCD_8080_D3                      P0_7
#define LCD_8080_D4                      P4_0
#define LCD_8080_D5                      P4_1
#define LCD_8080_D6                      P4_2
#define LCD_8080_D7                      P4_3

#define LCD_8080_CS                      P0_0
#define LCD_8080_DCX                     P1_5
#define LCD_8080_RD                      P1_6
#define LCD_8080_WR                      P0_2
#define LCD_FRAME_SYNC                   P0_1

#define LCD_DMA_CHANNEL_NUM              1
#define LCD_DMA_CHANNEL_INDEX            GDMA_Channel1
#define LCD_DMA_CHANNEL_IRQ              GDMA0_Channel1_IRQn
#define BL_PWM_TIM                       TIM4
#define GPIO_FRAME_SYNC_IRQ              GPIO1_IRQn
#define GPIO_FRAME_SYNC_HANDLER          GPIO1_Handler
// #define QSPI_LCD_POWER                   P2_4

int rt_hw_8080_lcd_init(void);

//#ifdef __cplusplus
//}
//#endif

#endif /* _DRV_LCD_8080_H_ */
