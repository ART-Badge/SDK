/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-10-30     supperthomas first version
 */

#include "drv_usart.h"
#include "rtl876x_gdma.h"

#ifdef RT_USING_SERIAL

#define DRV_DEBUG
#define LOG_TAG             "drv.usart"
#include <drv_log.h>

#define UART_RX_GDMA_CHANNEL_NUM            4
#define UART_RX_GDMA_CHANNEL                GDMA_Channel4
#define UART_RX_GDMA_CHANNEL_IRQN           GDMA0_Channel4_IRQn
#define UART_RX_GDMA_Handler                GDMA0_Channel4_Handler

const UART_BaudRate_TypeDef BaudRate_Table[] =
{
    {271, 10, 0x24A}, // BAUD_RATE_9600
    {165, 7,  0x5AD}, // BAUD_RATE_19200
    {20,  12, 0x252}, // BAUD_RATE_115200
    {10,  12, 0x252}, // BAUD_RATE_230400
    {5,   12, 0x252}, // BAUD_RATE_460800
    {4,   5,  0x3F7}, // BAUD_RATE_921600
    {4,   5,  0x000}, // BAUD_RATE_1000000
    {2,   5,  0},     // BAUD_RATE_2000000
    {1,   8,  0x492}, // BAUD_RATE_3000000
    {1,   5,  0},     // BAUD_RATE_4000000
    {1,   1,  0x36D}, // BAUD_RATE_6000000
};

enum
{
#ifdef BSP_USING_UART0
    UART0_INDEX,
#endif
};

static struct rtl_uart_config uart_config[] =
{
#ifdef BSP_USING_UART0
    UART0_CONFIG,
#endif
};

static struct rtl_uart uart_obj[sizeof(uart_config) / sizeof(uart_config[0])] = {0};

#ifdef RT_SERIAL_USING_DMA
static void rtl_dma_config(struct rt_serial_device *serial, rt_ubase_t flag);
#endif

static void uart_isr(struct rt_serial_device *serial)
{
    struct rtl_uart *uart;
    RT_ASSERT(serial != RT_NULL);
    uart = rt_container_of(serial, struct rtl_uart, serial);

#ifdef RT_SERIAL_USING_DMA
    rt_size_t recv_total_index, recv_len;
    rt_base_t level;
    uint32_t data_len = 0;
#endif

    /* Read interrupt type */
    uint32_t int_status = UART_GetIID(uart->config->Instance);

    /* Disable interrupt */
#ifdef RT_SERIAL_USING_DMA
    if (UART_GetFlagStatus(uart->config->Instance, UART_FLAG_RX_IDLE) == SET)
    {
        GDMA_SuspendCmd(UART_RX_GDMA_CHANNEL, ENABLE);
        UART_INTConfig(uart->config->Instance, UART_FLAG_RX_IDLE, DISABLE);
        uint32_t time_out = 0x1f;
        while ((RESET == GDMA_GetSuspendChannelStatus(UART_RX_GDMA_CHANNEL)) && time_out)
        {
            time_out--;
        }
        time_out = 0x0f;
        while ((RESET == GDMA_GetSuspendCmdStatus(UART_RX_GDMA_CHANNEL)) && time_out)
        {
            time_out--;
        }
        level = rt_hw_interrupt_disable();
        data_len = GDMA_GetTransferLen(UART_RX_GDMA_CHANNEL);
        if(data_len)
        {
            GDMA_Cmd(UART_RX_GDMA_CHANNEL_NUM, DISABLE);
            GDMA_SuspendCmd(UART_RX_GDMA_CHANNEL, DISABLE);
            rtl_dma_config(&uart->serial, RT_DEVICE_FLAG_DMA_RX);
        }
        else
        {
            GDMA_SuspendCmd(UART_RX_GDMA_CHANNEL, DISABLE);
        }

        recv_total_index = uart->serial.config.bufsz - data_len;
        recv_len = recv_total_index - uart->last_index;
        uart->last_index = recv_total_index;
        rt_hw_interrupt_enable(level);
        if (recv_len)
        {
            rt_hw_serial_isr(&uart->serial, RT_SERIAL_EVENT_RX_DMADONE | (recv_len << 8));
        }

        UART_INTConfig(uart->config->Instance, UART_INT_RX_IDLE, ENABLE);
        return;
    }
#endif

    UART_INTConfig(uart->config->Instance, UART_INT_RD_AVA | UART_INT_RX_LINE_STS, DISABLE);

    /* Interrupt handle */
    switch (int_status & 0x0E)
    {
    case UART_INT_ID_RX_DATA_TIMEOUT:
    {
        while (UART_GetFlagStatus(uart->config->Instance, UART_FLAG_RX_DATA_AVA) == SET)
        {
            //HAVE RECIVE TIMEOUT THE LEFT
            rt_hw_serial_isr(&uart->serial, RT_SERIAL_EVENT_RX_IND);
        }
        break;
    }
    case UART_INT_ID_LINE_STATUS:
    {
        UART_GetFlagStatus(uart->config->Instance, UART_FLAG_RX_OVERRUN);
        UART_ClearRxFIFO(uart->config->Instance);
        /* Enable interrupt */
        UART_INTConfig(uart->config->Instance, UART_INT_RD_AVA | UART_INT_RX_LINE_STS, ENABLE);
        return ;
    }
    case UART_INT_ID_RX_LEVEL_REACH:
    {
        while (UART_GetFlagStatus(uart->config->Instance, UART_FLAG_RX_DATA_AVA) == SET)
        {
            rt_hw_serial_isr(&uart->serial, RT_SERIAL_EVENT_RX_IND);
        }
        break;
    }
    case UART_INT_ID_TX_EMPTY:
    {
        break;
    }
    case UART_INT_ID_MODEM_STATUS:
    {
        break;
    }
    default:
    {
        break;
    }
    }

    /* Enable interrupt */
    UART_INTConfig(uart->config->Instance, UART_INT_RD_AVA |  UART_INT_RX_LINE_STS, ENABLE);
}

#ifdef RT_SERIAL_USING_DMA
static void dma_isr(struct rt_serial_device *serial)
{
    struct rtl_uart *uart;
    rt_size_t recv_total_index, recv_len;
    rt_base_t level;
    struct rt_serial_rx_fifo *rx_fifo;

    RT_ASSERT(serial != RT_NULL);
    uart = rt_container_of(serial, struct rtl_uart, serial);
    rx_fifo = (struct rt_serial_rx_fifo *)serial->serial_rx;

    /*  Clear interrupt */
    GDMA_Cmd(UART_RX_GDMA_CHANNEL_NUM, DISABLE);
    GDMA_ClearAllTypeINT(UART_RX_GDMA_CHANNEL_NUM);

    level = rt_hw_interrupt_disable();
    recv_total_index = serial->config.bufsz - GDMA_GetTransferLen(UART_RX_GDMA_CHANNEL);
    if (recv_total_index == 0)
    {
        recv_len = serial->config.bufsz - uart->last_index;
    }
    else
    {
        recv_len = recv_total_index - uart->last_index;
    }
    uart->last_index = recv_total_index;
    rt_hw_interrupt_enable(level);

    if (recv_len)
    {
        rt_hw_serial_isr(serial, RT_SERIAL_EVENT_RX_DMADONE | (recv_len << 8));
    }

    GDMA_ClearINTPendingBit(UART_RX_GDMA_CHANNEL_NUM, GDMA_INT_Transfer);
    /*  reset gdma param    */
    GDMA_SetDestinationAddress(UART_RX_GDMA_CHANNEL, (uint32_t)rx_fifo->buffer);
    GDMA_Cmd(UART_RX_GDMA_CHANNEL_NUM, ENABLE);
}
#endif

/**
  * @brief  Initialization of pinmux settings and pad settings.
  * @param  No parameter.
  * @return void
*/

void board_uart_init(uint8_t tx_pin, uint8_t rx_pin, UART_TypeDef *uart_index)
{
    Pad_Config(tx_pin, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE, PAD_OUT_HIGH);
    Pad_Config(rx_pin, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE, PAD_OUT_HIGH);

    if(uart_index == UART0)
    {
        Pinmux_Config(tx_pin, UART0_TX);
        Pinmux_Config(rx_pin, UART0_RX);
    }
    else if(uart_index == UART1)
    {
        Pinmux_Config(tx_pin, UART1_TX);
        Pinmux_Config(rx_pin, UART1_RX);
    }
}

#ifdef RT_SERIAL_USING_DMA
static void rtl_dma_config(struct rt_serial_device *serial, rt_ubase_t flag)
{
    struct rt_serial_rx_fifo *rx_fifo;
    struct rtl_uart *uart;

    RT_ASSERT(serial != RT_NULL);
    uart = rt_container_of(serial, struct rtl_uart, serial);

    if (RT_DEVICE_FLAG_DMA_RX == flag)
    {
        rx_fifo = (struct rt_serial_rx_fifo *)serial->serial_rx;

        /*--------------GDMA init-----------------------------*/
        RCC_PeriphClockCmd(APBPeriph_GDMA, APBPeriph_GDMA_CLOCK, ENABLE);
        GDMA_InitTypeDef GDMA_InitStruct;
        GDMA_StructInit(&GDMA_InitStruct);
        GDMA_InitStruct.GDMA_ChannelNum             = UART_RX_GDMA_CHANNEL_NUM;
        GDMA_InitStruct.GDMA_DIR                    = GDMA_DIR_PeripheralToMemory;
        GDMA_InitStruct.GDMA_BufferSize             = serial->config.bufsz;//determine total transfer size
        GDMA_InitStruct.GDMA_SourceInc              = DMA_SourceInc_Fix;
        GDMA_InitStruct.GDMA_DestinationInc         = DMA_DestinationInc_Inc;
        GDMA_InitStruct.GDMA_SourceDataSize         = GDMA_DataSize_Byte;
        GDMA_InitStruct.GDMA_DestinationDataSize    = GDMA_DataSize_Byte;
        GDMA_InitStruct.GDMA_SourceMsize            = GDMA_Msize_1;
        GDMA_InitStruct.GDMA_DestinationMsize       = GDMA_Msize_1;
        GDMA_InitStruct.GDMA_SourceAddr             = (uint32_t)(&(uart->config->Instance->RB_THR));
        GDMA_InitStruct.GDMA_SourceHandshake        = GDMA_Handshake_UART0_RX;
        GDMA_InitStruct.GDMA_DestinationAddr        = (uint32_t)rx_fifo->buffer;
        GDMA_Init(UART_RX_GDMA_CHANNEL, &GDMA_InitStruct);

        /* enable uart idle interrupt */
        UART_INTConfig(uart->config->Instance, UART_INT_RX_IDLE, ENABLE);

        GDMA_INTConfig(UART_RX_GDMA_CHANNEL_NUM, GDMA_INT_Transfer, ENABLE);
        /*-----------------GDMA IRQ init-------------------*/
        NVIC_InitTypeDef NVIC_InitStruct;
        NVIC_InitStruct.NVIC_IRQChannel = UART_RX_GDMA_CHANNEL_IRQN;
        NVIC_InitStruct.NVIC_IRQChannelPriority = 3;
        NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
        NVIC_Init(&NVIC_InitStruct);

        /*-----------------start to send data-----------*/
        GDMA_Cmd(UART_RX_GDMA_CHANNEL_NUM, ENABLE);
    }
}
#endif

void UART0_Handler(void)
{
    rt_interrupt_enter();
    uart_isr(&uart_obj[UART0_INDEX].serial);
    rt_interrupt_leave();
}

void UART_RX_GDMA_Handler(void)
{
    rt_interrupt_enter();
#ifdef RT_SERIAL_USING_DMA
    dma_isr(&uart_obj[UART0_INDEX].serial);
#endif
    rt_interrupt_leave();
}

static rt_err_t mcu_uart_configure(struct rt_serial_device *serial, struct serial_configure *cfg)
{
    struct rtl_uart *uart;
    RT_ASSERT(serial != RT_NULL);
    RT_ASSERT(cfg != RT_NULL);
    uart = rt_container_of(serial, struct rtl_uart, serial);

    /* pin init */
    Pad_Config(uart->config->tx_pin, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE, PAD_OUT_HIGH);
    Pad_Config(uart->config->rx_pin, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE, PAD_OUT_HIGH);

    Pinmux_Config(uart->config->tx_pin, uart->config->tx_pin_func);
    Pinmux_Config(uart->config->rx_pin, uart->config->rx_pin_func);

    /* clock init */
    UART_DeInit(uart->config->Instance);
    RCC_PeriphClockCmd(uart->config->periph, uart->config->periph_clock, ENABLE);

    /* uart init */
    UART_InitTypeDef UART_InitStruct;
    UART_StructInit(&UART_InitStruct);

    /* Config uart baudrate */
    UART_InitStruct.div            = BaudRate_Table[RTL_BAUD_RATE_1000000].div;
    UART_InitStruct.ovsr           = BaudRate_Table[RTL_BAUD_RATE_1000000].ovsr;
    UART_InitStruct.ovsr_adj       = BaudRate_Table[RTL_BAUD_RATE_1000000].ovsr_adj;

    UART_InitStruct.wordLen        = UART_WORD_LENGTH_8BIT;
    UART_InitStruct.stopBits       = UART_STOP_BITS_1;
    UART_InitStruct.parity         = UART_PARITY_NO_PARTY;
    UART_InitStruct.autoFlowCtrl   = 0;
    UART_InitStruct.txTriggerLevel = 16;                   //1~29
    UART_InitStruct.rxTriggerLevel = 16;                   //1~29
    UART_InitStruct.idle_time      = UART_RX_IDLE_4BYTE;   //idle interrupt wait time
    UART_InitStruct.dmaEn          = UART_DMA_ENABLE;
    UART_InitStruct.TxWaterlevel   = 15;       //Better to equal TX_FIFO_SIZE(16)- GDMA_MSize
    UART_InitStruct.RxWaterlevel   = 1;        //Better to equal GDMA_MSize
    UART_InitStruct.TxDmaEn        = DISABLE;
    UART_InitStruct.RxDmaEn        = ENABLE;
    UART_Init(uart->config->Instance, &UART_InitStruct);

#ifdef RT_SERIAL_USING_DMA
    uart->last_index = 0;
#endif

    return RT_EOK;
}

static rt_err_t mcu_uart_control(struct rt_serial_device *serial, int cmd, void *arg)
{
    struct rtl_uart *uart;

#ifdef RT_SERIAL_USING_DMA
    rt_ubase_t ctrl_arg = (rt_ubase_t)arg;
#endif

    RT_ASSERT(serial != RT_NULL);
    uart = rt_container_of(serial, struct rtl_uart, serial);

    switch(cmd)
    {
    /* enable interrupt */
    case RT_DEVICE_CTRL_SET_INT:
    {
        /* config uart interrupt */
        UART_INTConfig(uart->config->Instance, UART_INT_RD_AVA | UART_INT_RX_LINE_STS, ENABLE);
        NVIC_InitTypeDef NVIC_InitStruct;
        NVIC_InitStruct.NVIC_IRQChannel = uart->config->irq_type;
        NVIC_InitStruct.NVIC_IRQChannelPriority = 3;
        NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
        NVIC_Init(&NVIC_InitStruct);
        break;
    }
    case RT_DEVICE_CTRL_CLR_INT:
    {
        NVIC_DisableIRQ(uart->config->irq_type);
        break;
    }
#ifdef RT_SERIAL_USING_DMA
    case RT_DEVICE_CTRL_CONFIG:
    {
        rtl_dma_config(serial, ctrl_arg);
        break;
    }
#endif
    }

    return RT_EOK;
}

static int mcu_uart_putc(struct rt_serial_device *serial, char c)
{
    struct rtl_uart *uart;
    uart = rt_container_of(serial, struct rtl_uart, serial);

    while (UART_GetFlagState(uart->config->Instance, UART_FLAG_THR_EMPTY) == 0);

    UART_SendByte(uart->config->Instance, c);

    return 1;
}

static int mcu_uart_getc(struct rt_serial_device *serial)
{
    int ch;
    struct rtl_uart *uart;

    RT_ASSERT(serial != RT_NULL);
    uart = rt_container_of(serial, struct rtl_uart, serial);
    if (UART_GetFlagState(uart->config->Instance, UART_FLAG_RX_DATA_RDY) == SET)
    {
        ch = UART_ReceiveByte(uart->config->Instance);
    }
    else
    {
        ch = -1;
    }
    return ch;
}

static void rtl_uart_get_dma_config(void)
{
    uart_obj[0].uart_dma_flag = 0;
#ifdef RT_SERIAL_USING_DMA
    uart_obj[0].uart_dma_flag |= RT_DEVICE_FLAG_DMA_RX;
#endif
}

static const struct rt_uart_ops mcu_uart_ops =
{
    .configure = mcu_uart_configure,
    .control = mcu_uart_control,
    .putc = mcu_uart_putc,
    .getc = mcu_uart_getc,
};

int rt_hw_usart_init(void)
{
    rt_size_t obj_num = sizeof(uart_config) / sizeof(uart_config[0]);
    rt_err_t result = 0;
    struct serial_configure config = RT_SERIAL_CONFIG_DEFAULT;

    rtl_uart_get_dma_config();

    for (int i = 0; i < obj_num; i++)
    {
        uart_obj[i].config = &uart_config[i];
        uart_obj[i].serial.ops = &mcu_uart_ops;
        uart_obj[i].serial.config = config;
        result = rt_hw_serial_register(&uart_obj[i].serial, uart_obj[i].config->name,
                                       RT_DEVICE_FLAG_RDWR
                                       | RT_DEVICE_FLAG_INT_RX
                                       | RT_DEVICE_FLAG_INT_TX
                                       | uart_obj[i].uart_dma_flag
                                       , NULL);
    }
    /* init UART object */
    return result;
}

#endif /* RT_USING_SERIAL */

/************** end of file ********************/
