
#ifndef __INTERRUPT_H__
#define __INTERRUPT_H__

#include <rthw.h>
#include <board.h>

#define INT_IRQ     0x00
#define INT_FIQ     0x01

void rt_hw_interrupt_init(void);
void rt_hw_interrupt_mask(int vector);
void rt_hw_interrupt_umask(int vector);

rt_isr_handler_t rt_hw_interrupt_install(int vector, rt_isr_handler_t handler,
        void *param, const char *name);

#endif
