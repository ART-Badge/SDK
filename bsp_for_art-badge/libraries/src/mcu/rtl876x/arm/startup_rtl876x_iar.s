/*****************************************************************************/
/* Startup_rtl8762c.s: Startup file for RTL8762C device series               */
/*****************************************************************************/

        MODULE  ?vector_table

        ;; Forward declaration of sections.
        SECTION CSTACK:DATA:NOROOT(3)

        SECTION .intvec:CODE:NOROOT(2)

        EXTERN  __iar_program_start
        EXTERN  SystemInit  
        PUBLIC  __vector_table

        DATA

; IAR debugger reads initial MSP and PC from APP vector table. In order to debug
; from ROM Reset_Hanlder, both initial MSP and PC should use the ROM values.
__initial_sp      EQU   0x00203800    ; 8772 is 0x00203C00, but the value will not be updated
Reset_Handler_ROM EQU   0x0000107D

__vector_table
    DCD   __initial_sp
    DCD   Reset_Handler_ROM           ; Reset Handler            
    DCD   NMI_Handler                 ; NMI Handler                  
    DCD   HardFault_Handler           ; Hard Fault Handler           
    DCD   MemManage_Handler           ; MPU Fault Handler            
    DCD   BusFault_Handler            ; Bus Fault Handler            
    DCD   UsageFault_Handler          ; Usage Fault Handler          
    DCD   0                           ; Reserved                     
    DCD   0                           ; Reserved                     
    DCD   0                           ; Reserved                     
    DCD   0                           ; Reserved                     
    DCD   SVC_Handler                 ; SVCall Handler               
    DCD   DebugMon_Handler            ; Debug Monitor Handler        
    DCD   0                           ; Reserved                     
    DCD   PendSV_Handler              ; PendSV Handler               
    DCD   SysTick_Handler             ; SysTick Handler              

    ; External Interrupt Handlers
    DCD     System_Handler            ;[0]  System On interrupt
    DCD     WDG_Handler               ;[1]  Watch dog global insterrupt
    DCD     BTMAC_Handler             ;[2]  See Below Table ( an Extension of interrupt )
    DCD     Timer3_Handler            ;[3]  Timer3 global interrupt
    DCD     Timer2_Handler            ;[4]  Timer2 global interrupt
    DCD     HardFault_Handler         ;[5]  Platform interrupt (platfrom error interrupt)
    DCD     I2S0_TX_Handler           ;[6]  I2S0 TX interrupt
    DCD     I2S0_RX_Handler           ;[7]  I2S0 RX interrupt
    DCD     Timer4_7_Handler          ;[8]  Timer[4:7] global interrupt
    DCD     GPIO4_Handler             ;[9]  GPIO 4 interrupt
    DCD     GPIO5_Handler             ;[10] GPIO 5 interrupt
    DCD     UART1_Handler             ;[11] Uart1 interrupt
    DCD     UART0_Handler             ;[12] Uart0 interrupt
    DCD     RTC_Handler               ;[13] Realtime counter interrupt
    DCD     SPI0_Handler              ;[14] SPI0 interrupt
    DCD     SPI1_Handler              ;[15] SPI1 interrupt
    DCD     I2C0_Handler              ;[16] I2C0 interrupt
    DCD     I2C1_Handler              ;[17] I2C1 interrupt
    DCD     ADC_Handler               ;[18] ADC global interrupt
    DCD     Peripheral_Handler        ;[19] See Below Table ( an Extension of interrupt )
    DCD     GDMA0_Channel0_Handler    ;[20] RTK-DMA0 channel 0 global interrupt
    DCD     GDMA0_Channel1_Handler    ;[21] RTK-DMA0 channel 1 global interrupt
    DCD     GDMA0_Channel2_Handler    ;[22] RTK-DMA0 channel 2 global interrupt
    DCD     GDMA0_Channel3_Handler    ;[23] RTK-DMA0 channel 3 global interrupt
    DCD     GDMA0_Channel4_Handler    ;[24] RTK-DMA0 channel 4 global interrupt
    DCD     GDMA0_Channel5_Handler    ;[25] RTK-DMA0 channel 5 global interrupt (default for log)
    DCD     GPIO_Group3_Handler       ;[26] GPIO(n*4)+3,n={0:7} global interrupt
    DCD     GPIO_Group2_Handler       ;[27] GPIO(n*4)+2,n={0:7} global interrupt
    DCD     IR_Handler                ;[28] IR module global interrupt
    DCD     GPIO_Group1_Handler       ;[29] GPIO(n*4)+1,n={0:7}-{1} global interrupt
    DCD     GPIO_Group0_Handler       ;[30] GPIO(n*4)+0,n={0:7}-{1} global interrupt
    DCD     UART2_Handler             ;[31] Uart2 interrupt (default for log)

    ;Timer[4:7] interrupt
    DCD     Timer4_Handler            ;8, 0, 48
    DCD     Timer5_Handler            ;8, 1, 49
    DCD     Timer6_Handler            ;8, 2, 50
    DCD     Timer7_Handler            ;8, 3, 51

    ;Peripheral Interrupts not special interrupt
    ;Interrupt name, Interrupt status bit, Offset in vector
    DCD     SPI_Flash_Handler         ;19, 0, 52
    DCD     Qdecode_Handler           ;19, 1, 53
    DCD     Keyscan_Handler           ;19, 2, 54
    DCD     SPI2W_Handler             ;19, 3, 55
    DCD     LPCOMP_Handler            ;19, 4, 56
    DCD     PTA_Mailbox_Handler       ;19, 5, 57
    DCD     I2S1_TX_Handler           ;19, 6, 58
    DCD     I2S1_RX_Handler           ;19, 7, 59
    DCD     LCD_Handler               ;19, 8, 60                 

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Default interrupt handlers.
;;
        THUMB
        PUBLIC Reset_Handler
        SECTION .reset:CODE:REORDER(2)
Reset_Handler

        LDR     R0, =SystemInit
        BLX     R0
        
        PUBWEAK Default_Handler
        PUBWEAK NMI_Handler
        PUBWEAK HardFault_Handler
        PUBWEAK MemManage_Handler
        PUBWEAK BusFault_Handler
        PUBWEAK UsageFault_Handler
        PUBWEAK SVC_Handler
        PUBWEAK DebugMon_Handler
        PUBWEAK PendSV_Handler
        PUBWEAK SysTick_Handler
        PUBWEAK System_Handler
        PUBWEAK WDG_Handler
        PUBWEAK BTMAC_Handler
        PUBWEAK Timer3_Handler
        PUBWEAK Timer2_Handler
        PUBWEAK I2S0_TX_Handler
        PUBWEAK I2S0_RX_Handler
        PUBWEAK Timer4_7_Handler
        PUBWEAK GPIO4_Handler
        PUBWEAK GPIO5_Handler
        PUBWEAK UART1_Handler
        PUBWEAK UART0_Handler
        PUBWEAK RTC_Handler
        PUBWEAK SPI0_Handler
        PUBWEAK SPI1_Handler
        PUBWEAK I2C0_Handler
        PUBWEAK I2C1_Handler
        PUBWEAK ADC_Handler
        PUBWEAK Peripheral_Handler
        PUBWEAK GDMA0_Channel0_Handler
        PUBWEAK GDMA0_Channel1_Handler
        PUBWEAK GDMA0_Channel2_Handler
        PUBWEAK GDMA0_Channel3_Handler
        PUBWEAK GDMA0_Channel4_Handler
        PUBWEAK GDMA0_Channel5_Handler
        PUBWEAK GPIO_Group3_Handler
        PUBWEAK GPIO_Group2_Handler
        PUBWEAK IR_Handler
        PUBWEAK GPIO_Group1_Handler
        PUBWEAK GPIO_Group0_Handler
        PUBWEAK UART2_Handler

        ;Extension Interrupts
        PUBWEAK Timer4_Handler
        PUBWEAK Timer5_Handler
        PUBWEAK Timer6_Handler
        PUBWEAK Timer7_Handler
        PUBWEAK SPI_Flash_Handler
        PUBWEAK Qdecode_Handler
        PUBWEAK Keyscan_Handler
        PUBWEAK SPI2W_Handler
        PUBWEAK LPCOMP_Handler
        PUBWEAK PTA_Mailbox_Handler
        PUBWEAK I2S1_TX_Handler
        PUBWEAK I2S1_RX_Handler
        PUBWEAK LCD_Handler
        PUBWEAK GPIO0_Handler
        PUBWEAK GPIO1_Handler
        PUBWEAK GPIO2_Handler
        PUBWEAK GPIO3_Handler
        PUBWEAK GPIO6_Handler
        PUBWEAK GPIO7_Handler
        PUBWEAK GPIO8_Handler
        PUBWEAK GPIO9_Handler
        PUBWEAK GPIO10_Handler
        PUBWEAK GPIO11_Handler
        PUBWEAK GPIO12_Handler
        PUBWEAK GPIO13_Handler
        PUBWEAK GPIO14_Handler
        PUBWEAK GPIO15_Handler
        PUBWEAK GPIO16_Handler
        PUBWEAK GPIO17_Handler
        PUBWEAK GPIO18_Handler
        PUBWEAK GPIO19_Handler
        PUBWEAK GPIO20_Handler
        PUBWEAK GPIO21_Handler
        PUBWEAK GPIO22_Handler
        PUBWEAK GPIO23_Handler
        PUBWEAK GPIO24_Handler
        PUBWEAK GPIO25_Handler
        PUBWEAK GPIO26_Handler
        PUBWEAK GPIO27_Handler
        PUBWEAK GPIO28_Handler
        PUBWEAK GPIO29_Handler
        PUBWEAK GPIO30_Handler
        PUBWEAK GPIO31_Handler
        
        SECTION .text:CODE:REORDER:NOROOT(2)
        THUMB
Default_Handler
NMI_Handler
HardFault_Handler
MemManage_Handler
BusFault_Handler
UsageFault_Handler
SVC_Handler
DebugMon_Handler
PendSV_Handler
SysTick_Handler
System_Handler
WDG_Handler
BTMAC_Handler
Timer3_Handler
Timer2_Handler
I2S0_TX_Handler
I2S0_RX_Handler
Timer4_7_Handler
GPIO4_Handler
GPIO5_Handler
UART1_Handler
UART0_Handler
RTC_Handler
SPI0_Handler
SPI1_Handler
I2C0_Handler
I2C1_Handler
ADC_Handler
Peripheral_Handler
GDMA0_Channel0_Handler
GDMA0_Channel1_Handler
GDMA0_Channel2_Handler
GDMA0_Channel3_Handler
GDMA0_Channel4_Handler
GDMA0_Channel5_Handler
GPIO_Group3_Handler
GPIO_Group2_Handler
IR_Handler
GPIO_Group1_Handler
GPIO_Group0_Handler
UART2_Handler

;Extension Interrupts
Timer4_Handler
Timer5_Handler
Timer6_Handler
Timer7_Handler
SPI_Flash_Handler
Qdecode_Handler
Keyscan_Handler
SPI2W_Handler
LPCOMP_Handler
PTA_Mailbox_Handler
I2S1_TX_Handler
I2S1_RX_Handler
LCD_Handler
GPIO0_Handler
GPIO1_Handler
GPIO2_Handler
GPIO3_Handler
GPIO6_Handler
GPIO7_Handler
GPIO8_Handler
GPIO9_Handler
GPIO10_Handler
GPIO11_Handler
GPIO12_Handler
GPIO13_Handler
GPIO14_Handler
GPIO15_Handler
GPIO16_Handler
GPIO17_Handler
GPIO18_Handler
GPIO19_Handler
GPIO20_Handler
GPIO21_Handler
GPIO22_Handler
GPIO23_Handler
GPIO24_Handler
GPIO25_Handler
GPIO26_Handler
GPIO27_Handler
GPIO28_Handler
GPIO29_Handler
GPIO30_Handler
GPIO31_Handler

        EXTERN log_direct
        LDR    R0, =0x20000000
        LDR    R1, =DEFAULT_HANDLER_TXT
        MRS    R2, IPSR
        LDR    R3, =log_direct
        BLX    R3
        B      .

        SECTION rodata:DATA (2)
        DATA
DEFAULT_HANDLER_TXT
        DCB "Error! Please implement your ISR Handler for IRQ %d!\n", 0 ; Null terminated string

        END
