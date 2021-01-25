;#include "mem_config.h"

                EXPORT __initial_sp
__initial_sp    EQU     0x00203400                ; 8772 is 0x00203C00, but the value will not be updated

                PRESERVE8
                THUMB

; Vector Table Mapped to Address 0 at Reset

                AREA    VECTOR, DATA, READONLY
                EXPORT  __Vectors
                EXPORT  __Vectors_End
                EXPORT  __Vectors_Size

__Vectors       DCD     __initial_sp              ; Top of Stack
                DCD     Reset_Handler             ; Reset Handler
                DCD     NMI_Handler               ; NMI Handler
                DCD     HardFault_Handler         ; Hard Fault Handler
                DCD     MemManage_Handler         ; MPU Fault Handler
                DCD     BusFault_Handler          ; Bus Fault Handler
                DCD     UsageFault_Handler        ; Usage Fault Handler
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     SVC_Handler               ; SVCall Handler
                DCD     DebugMon_Handler          ; Debug Monitor Handler
                DCD     0                         ; Reserved
                DCD     PendSV_Handler            ; PendSV Handler
                DCD     SysTick_Handler           ; SysTick Handler

                ; External Interrupts
                ; sync from Bee2_Address_Mapping_20170526.xls
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
__Vectors_End

__Vectors_Size  EQU     __Vectors_End - __Vectors

                AREA    RESET, CODE, READONLY

; Reset Handler
Reset_Handler   PROC
                EXPORT Reset_Handler               [WEAK]
                IMPORT SystemInit
                LDR    R0, =SystemInit
                BX     R0

                ENDP ; end of Reset_Handler


                AREA    |.text|, CODE, READONLY
Default_Handler PROC
                EXPORT Default_Handler                  [WEAK]
                EXPORT NMI_Handler                      [WEAK]
                EXPORT HardFault_Handler                [WEAK]
                EXPORT MemManage_Handler                [WEAK]
                EXPORT BusFault_Handler                 [WEAK]
                EXPORT UsageFault_Handler               [WEAK]
                EXPORT SVC_Handler                      [WEAK]
                EXPORT DebugMon_Handler                 [WEAK]
                EXPORT PendSV_Handler                   [WEAK]
                EXPORT SysTick_Handler                  [WEAK]
                EXPORT System_Handler                   [WEAK]
                EXPORT WDG_Handler                      [WEAK]
                EXPORT BTMAC_Handler                    [WEAK]
                EXPORT Timer3_Handler                   [WEAK]
                EXPORT Timer2_Handler                   [WEAK]
                EXPORT I2S0_TX_Handler                  [WEAK]
                EXPORT I2S0_RX_Handler                  [WEAK]
                EXPORT Timer4_7_Handler                 [WEAK]
                EXPORT GPIO4_Handler                    [WEAK]
                EXPORT GPIO5_Handler                    [WEAK]
                EXPORT UART1_Handler                    [WEAK]
                EXPORT UART0_Handler                    [WEAK]
                EXPORT RTC_Handler                      [WEAK]
                EXPORT SPI0_Handler                     [WEAK]
                EXPORT SPI1_Handler                     [WEAK]
                EXPORT I2C0_Handler                     [WEAK]
                EXPORT I2C1_Handler                     [WEAK]
                EXPORT ADC_Handler                      [WEAK]
                EXPORT Peripheral_Handler               [WEAK]
                EXPORT GDMA0_Channel0_Handler           [WEAK]
                EXPORT GDMA0_Channel1_Handler           [WEAK]
                EXPORT GDMA0_Channel2_Handler           [WEAK]
                EXPORT GDMA0_Channel3_Handler           [WEAK]
                EXPORT GDMA0_Channel4_Handler           [WEAK]
                EXPORT GDMA0_Channel5_Handler           [WEAK]
                EXPORT GPIO_Group3_Handler              [WEAK]
                EXPORT GPIO_Group2_Handler              [WEAK]
                EXPORT IR_Handler                       [WEAK]
                EXPORT GPIO_Group1_Handler              [WEAK]
                EXPORT GPIO_Group0_Handler              [WEAK]
                EXPORT UART2_Handler                    [WEAK]

                ;Extension Interrupts
                EXPORT Timer4_Handler                   [WEAK]
                EXPORT Timer5_Handler                   [WEAK]
                EXPORT Timer6_Handler                   [WEAK]
                EXPORT Timer7_Handler                   [WEAK]
                EXPORT SPI_Flash_Handler                [WEAK]
                EXPORT Qdecode_Handler                  [WEAK]
                EXPORT Keyscan_Handler                  [WEAK]
                EXPORT SPI2W_Handler                    [WEAK]
                EXPORT LPCOMP_Handler                   [WEAK]
                EXPORT PTA_Mailbox_Handler              [WEAK]
                EXPORT I2S1_TX_Handler                  [WEAK]
                EXPORT I2S1_RX_Handler                  [WEAK]
                EXPORT LCD_Handler                      [WEAK]
                EXPORT GPIO0_Handler                    [WEAK]
                EXPORT GPIO1_Handler                    [WEAK]
                EXPORT GPIO2_Handler                    [WEAK]
                EXPORT GPIO3_Handler                    [WEAK]
                EXPORT GPIO6_Handler                    [WEAK]
                EXPORT GPIO7_Handler                    [WEAK]
                EXPORT GPIO8_Handler                    [WEAK]
                EXPORT GPIO9_Handler                    [WEAK]
                EXPORT GPIO10_Handler                   [WEAK]
                EXPORT GPIO11_Handler                   [WEAK]
                EXPORT GPIO12_Handler                   [WEAK]
                EXPORT GPIO13_Handler                   [WEAK]
                EXPORT GPIO14_Handler                   [WEAK]
                EXPORT GPIO15_Handler                   [WEAK]
                EXPORT GPIO16_Handler                   [WEAK]
                EXPORT GPIO17_Handler                   [WEAK]
                EXPORT GPIO18_Handler                   [WEAK]
                EXPORT GPIO19_Handler                   [WEAK]
                EXPORT GPIO20_Handler                   [WEAK]
                EXPORT GPIO21_Handler                   [WEAK]
                EXPORT GPIO22_Handler                   [WEAK]
                EXPORT GPIO23_Handler                   [WEAK]
                EXPORT GPIO24_Handler                   [WEAK]
                EXPORT GPIO25_Handler                   [WEAK]
                EXPORT GPIO26_Handler                   [WEAK]
                EXPORT GPIO27_Handler                   [WEAK]
                EXPORT GPIO28_Handler                   [WEAK]
                EXPORT GPIO29_Handler                   [WEAK]
                EXPORT GPIO30_Handler                   [WEAK]
                EXPORT GPIO31_Handler                   [WEAK]
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
                IMPORT log_direct
                LDR    R0, =0x20000000
                LDR    R1, =DEFAULT_HANDLER_TXT
                MRS    R2, IPSR
                LDR    R3, =log_direct
                BLX    R3
                B      .

                ENDP


; User Initial Stack
                EXPORT  __user_setup_stackheap
__user_setup_stackheap PROC
                BX      LR
                ENDP

DEFAULT_HANDLER_TXT
                DCB "Error! Please implement your ISR Handler for IRQ %d!\n", 0 ; Null terminated string
                ALIGN

                END
