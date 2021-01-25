;/*****************************************************************************/
;/*  startup_rtl8762c_gcc.s: Startup file for Realtek RTL8762C device series  */
;/*****************************************************************************/

/**
**===========================================================================
**  Definitions
**===========================================================================
*/
    .syntax unified
    .cpu cortex-m4
    .fpu softvfp
    .thumb

  .global    g_pfnVectors
  .global    Default_Handler
  .type    Default_Handler, %function 


.equ _estack, 0x00203800
/**
**===========================================================================
**  Program - Reset_Handler
**  Abstract: This code gets called after a reset event.  
**===========================================================================
*/
  .section RESET
  .weak    Reset_Handler
  .type    Reset_Handler, %function
Reset_Handler:
  LDR    R0, = SystemInit  
  BX     R0

/**
**===========================================================================
**  Program - Default_Handler
**  Abstract: This code gets called when the processor receives an
**    unexpected interrupt.
**===========================================================================
*/
  .section    .text,"ax",%progbits
  Default_Handler:
    b   .
    .size  Default_Handler, .-Default_Handler  
/**
**===========================================================================
**  Reset, Exception, and Interrupt vectors
**===========================================================================
*/

  .section VECTOR,"a",%progbits
  .type    g_pfnVectors, %object
  .size    g_pfnVectors, .-g_pfnVectors

g_pfnVectors:
  /* Processor exception vectors */
  .word    _estack
  .word    Reset_Handler
  .word    NMI_Handler
  .word    HardFault_Handler
  .word    MemManage_Handler
  .word    BusFault_Handler
  .word    UsageFault_Handler
  .word    0
  .word    0
  .word    0
  .word    0
  .word    SVC_Handler
  .word    DebugMon_Handler
  .word    0
  .word    PendSV_Handler
  .word    SysTick_Handler

  
  /* Interrupt Handlers for RTL8762C Peripherals */
  .word System_Handler                   /*[0]  System On interrupt*/
  .word WDG_Handler                      /*[1]  Watch dog global insterrupt*/
  .word BTMAC_Handler                    /*[2]  See Below Table ( an Extension of interrupt )*/
  .word Timer3_Handler                   /*[3]  Timer3 global interrupt*/
  .word Timer2_Handler                   /*[4]  Timer2 global interrupt*/
  .word HardFault_Handler                /*[5]  Platform interrupt (platfrom error interrupt)*/
  .word I2S0_TX_Handler                  /*[6]  I2S0 TX interrupt*/
  .word I2S0_RX_Handler                  /*[7]  I2S0 RX interrupt*/
  .word Timer4_7_Handler                 /*[8]  Timer[4:7] global interrupt*/
  .word GPIO4_Handler                    /*[9]  GPIO 4 interrupt*/
  .word GPIO5_Handler                    /*[10] GPIO 5 interrupt*/
  .word UART1_Handler                    /*[11] Uart1 interrupt*/
  .word UART0_Handler                    /*[12] Uart0 interrupt*/
  .word RTC_Handler                      /*[13] Realtime counter interrupt*/
  .word SPI0_Handler                     /*[14] SPI0 interrupt*/
  .word SPI1_Handler                     /*[15] SPI1 interrupt*/
  .word I2C0_Handler                     /*[16] I2C0 interrupt*/
  .word I2C1_Handler                     /*[17] I2C1 interrupt*/
  .word ADC_Handler                      /*[18] ADC global interrupt*/
  .word Peripheral_Handler               /*[19] See Below Table ( an Extension of interrupt )*/
  .word GDMA0_Channel0_Handler           /*[20] RTK-DMA0 channel 0 global interrupt*/
  .word GDMA0_Channel1_Handler           /*[21] RTK-DMA0 channel 1 global interrupt*/
  .word GDMA0_Channel2_Handler           /*[22] RTK-DMA0 channel 2 global interrupt*/
  .word GDMA0_Channel3_Handler           /*[23] RTK-DMA0 channel 3 global interrupt*/
  .word GDMA0_Channel4_Handler           /*[24] RTK-DMA0 channel 4 global interrupt*/
  .word GDMA0_Channel5_Handler           /*[25] RTK-DMA0 channel 5 global interrupt (default for log)*/
  .word GPIO_Group3_Handler              /*[26] GPIO(n*4)+3,n={0:7} global interrupt*/
  .word GPIO_Group2_Handler              /*[27] GPIO(n*4)+2,n={0:7} global interrupt*/
  .word IR_Handler                       /*[28] IR module global interrupt*/
  .word GPIO_Group1_Handler              /*[29] GPIO(n*4)+1,n={0:7}-{1} global interrupt*/
  .word GPIO_Group0_Handler              /*[30] GPIO(n*4)+0,n={0:7}-{1} global interrupt*/
  .word UART2_Handler                    /*[31] Uart2 interrupt (default for log)*/

  .word Timer4_Handler                   /* 8, 0, 48 */
  .word Timer5_Handler                   /* 8, 1, 49  */
  .word Timer6_Handler                   /* 8, 2, 50  */
  .word Timer7_Handler                   /* 8, 3, 51 */
  .word SPI_Flash_Handler                /* 19, 0, 52    */
  .word Qdecode_Handler                  /* 9, 1, 53    */
  .word Keyscan_Handler                  /* 19, 2, 54    */
  .word SPI2W_Handler                    /* 19, 3, 55  */
  .word LPCOMP_Handler                   /* 19, 4, 56    */
  .word PTA_Mailbox_Handler              /* 19, 5, 57   */
  .word I2S1_TX_Handler                  /* 19, 6, 58    */
  .word I2S1_RX_Handler                  /* 19, 7, 59   */
  .word LCD_Handler                      /* 19, 8, 60    */



/**
**===========================================================================
** Provide weak aliases for each Exception handler to the Default_Handler.
**===========================================================================
*/
  .weak  NMI_Handler
  .thumb_set NMI_Handler,Default_Handler
  
  .weak  HardFault_Handler
  .thumb_set HardFault_Handler,Default_Handler
  
  .weak  MemManage_Handler
  .thumb_set MemManage_Handler,Default_Handler
  
  .weak  BusFault_Handler
  .thumb_set BusFault_Handler,Default_Handler

  .weak  UsageFault_Handler
  .thumb_set UsageFault_Handler,Default_Handler

  .weak  SVC_Handler
  .thumb_set SVC_Handler,Default_Handler

  .weak  DebugMon_Handler
  .thumb_set DebugMon_Handler,Default_Handler

  .weak  PendSV_Handler
  .thumb_set PendSV_Handler,Default_Handler

  .weak  SysTick_Handler
  .thumb_set SysTick_Handler,Default_Handler

  .weak  System_Handler
  .thumb_set System_Handler,Default_Handler

  .weak  WDG_Handler
  .thumb_set WDG_Handler,Default_Handler

  .weak  BTMAC_Handler
  .thumb_set BTMAC_Handler,Default_Handler

  .weak  Timer3_Handler
  .thumb_set Timer3_Handler,Default_Handler

  .weak  Timer2_Handler
  .thumb_set Timer2_Handler,Default_Handler

  .weak  I2S0_TX_Handler
  .thumb_set I2S0_TX_Handler,Default_Handler

  .weak  I2S0_RX_Handler
  .thumb_set I2S0_RX_Handler,Default_Handler

  .weak  Timer4_7_Handler
  .thumb_set Timer4_7_Handler,Default_Handler

  .weak  GPIO4_Handler
  .thumb_set GPIO4_Handler,Default_Handler

  .weak  GPIO5_Handler
  .thumb_set GPIO5_Handler,Default_Handler

  .weak  UART1_Handler
  .thumb_set UART1_Handler,Default_Handler

  .weak  UART0_Handler
  .thumb_set UART0_Handler,Default_Handler

  .weak  RTC_Handler
  .thumb_set RTC_Handler,Default_Handler

  .weak  SPI0_Handler
  .thumb_set SPI0_Handler,Default_Handler

  .weak  SPI1_Handler
  .thumb_set SPI1_Handler,Default_Handler

  .weak  I2C0_Handler
  .thumb_set I2C0_Handler,Default_Handler

  .weak  I2C1_Handler
  .thumb_set I2C1_Handler,Default_Handler

  .weak  ADC_Handler
  .thumb_set ADC_Handler,Default_Handler

  .weak  Peripheral_Handler
  .thumb_set Peripheral_Handler,Default_Handler

  .weak  GDMA0_Channel0_Handler
  .thumb_set GDMA0_Channel0_Handler,Default_Handler

  .weak  GDMA0_Channel1_Handler
  .thumb_set GDMA0_Channel1_Handler,Default_Handler

  .weak  GDMA0_Channel2_Handler
  .thumb_set GDMA0_Channel2_Handler,Default_Handler

  .weak  GDMA0_Channel3_Handler
  .thumb_set GDMA0_Channel3_Handler,Default_Handler

  .weak  GDMA0_Channel4_Handler
  .thumb_set GDMA0_Channel4_Handler,Default_Handler

  .weak  GDMA0_Channel5_Handler
  .thumb_set GDMA0_Channel5_Handler,Default_Handler

  .weak  GPIO_Group3_Handler
  .thumb_set GPIO_Group3_Handler,Default_Handler

  .weak  GPIO_Group2_Handler
  .thumb_set GPIO_Group2_Handler,Default_Handler

  .weak  IR_Handler
  .thumb_set IR_Handler,Default_Handler

  .weak  GPIO_Group1_Handler
  .thumb_set GPIO_Group1_Handler,Default_Handler

  .weak  GPIO_Group0_Handler
  .thumb_set GPIO_Group0_Handler,Default_Handler

  .weak  UART2_Handler
  .thumb_set UART2_Handler,Default_Handler

  .weak  Timer4_Handler
  .thumb_set Timer4_Handler,Default_Handler

  .weak  Timer5_Handler
  .thumb_set Timer5_Handler,Default_Handler

  .weak  Timer6_Handler
  .thumb_set Timer6_Handler,Default_Handler

  .weak  Timer7_Handler
  .thumb_set Timer7_Handler,Default_Handler

  .weak  SPI_Flash_Handler
  .thumb_set SPI_Flash_Handler,Default_Handler

  .weak  Qdecode_Handler
  .thumb_set Qdecode_Handler,Default_Handler

  .weak  Keyscan_Handler
  .thumb_set Keyscan_Handler,Default_Handler

  .weak  SPI2W_Handler
  .thumb_set SPI2W_Handler,Default_Handler

  .weak  LPCOMP_Handler
  .thumb_set LPCOMP_Handler,Default_Handler

  .weak  PTA_Mailbox_Handler
  .thumb_set PTA_Mailbox_Handler,Default_Handler

  .weak  I2S1_TX_Handler
  .thumb_set I2S1_TX_Handler,Default_Handler

  .weak  I2S1_RX_Handler
  .thumb_set I2S1_RX_Handler,Default_Handler

  .weak  LCD_Handler
  .thumb_set LCD_Handler,Default_Handler
  
  .weak  GPIO0_Handler
  .thumb_set GPIO0_Handler,Default_Handler
  
  .weak  GPIO1_Handler
  .thumb_set GPIO1_Handler,Default_Handler
  
  .weak  GPIO2_Handler
  .thumb_set GPIO2_Handler,Default_Handler
  
  .weak  GPIO3_Handler
  .thumb_set GPIO3_Handler,Default_Handler
  
  .weak  GPIO6_Handler
  .thumb_set GPIO6_Handler,Default_Handler
  
  .weak  GPIO7_Handler
  .thumb_set GPIO7_Handler,Default_Handler
  
  .weak  GPIO8_Handler
  .thumb_set GPIO8_Handler,Default_Handler
  
  .weak  GPIO9_Handler
  .thumb_set GPIO9_Handler,Default_Handler
  
  .weak  GPIO10_Handler
  .thumb_set GPIO10_Handler,Default_Handler
  
  .weak  GPIO11_Handler
  .thumb_set GPIO11_Handler,Default_Handler
  
  .weak  GPIO12_Handler
  .thumb_set GPIO12_Handler,Default_Handler
  
  .weak  GPIO13_Handler
  .thumb_set GPIO13_Handler,Default_Handler
  
  .weak  GPIO14_Handler
  .thumb_set GPIO14_Handler,Default_Handler
  
  .weak  GPIO15_Handler
  .thumb_set GPIO15_Handler,Default_Handler
  
  .weak  GPIO16_Handler
  .thumb_set GPIO16_Handler,Default_Handler
  
  .weak  GPIO17_Handler
  .thumb_set GPIO17_Handler,Default_Handler
  
  .weak  GPIO18_Handler
  .thumb_set GPIO18_Handler,Default_Handler
  
  .weak  GPIO19_Handler
  .thumb_set GPIO19_Handler,Default_Handler
  
  .weak  GPIO20_Handler
  .thumb_set GPIO20_Handler,Default_Handler
  
  .weak  GPIO21_Handler
  .thumb_set GPIO21_Handler,Default_Handler
  
  .weak  GPIO22_Handler
  .thumb_set GPIO22_Handler,Default_Handler
  
  .weak  GPIO23_Handler
  .thumb_set GPIO23_Handler,Default_Handler
  
  .weak  GPIO24_Handler
  .thumb_set GPIO24_Handler,Default_Handler
  
  .weak  GPIO25_Handler
  .thumb_set GPIO25_Handler,Default_Handler
  
  .weak  GPIO26_Handler
  .thumb_set GPIO26_Handler,Default_Handler
  
  .weak  GPIO27_Handler
  .thumb_set GPIO27_Handler,Default_Handler
  
  .weak  GPIO28_Handler
  .thumb_set GPIO28_Handler,Default_Handler
  
  .weak  GPIO29_Handler
  .thumb_set GPIO29_Handler,Default_Handler
  
  .weak  GPIO30_Handler
  .thumb_set GPIO30_Handler,Default_Handler
  
  .weak  GPIO31_Handler
  .thumb_set GPIO31_Handler,Default_Handler

.end
