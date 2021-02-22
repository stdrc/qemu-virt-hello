#pragma once

extern void enable_irq();
extern void disable_irq();

void interrupt_init();
void print_daif();
