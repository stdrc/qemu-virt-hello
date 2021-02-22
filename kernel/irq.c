#include "irq.h"
#include "gic.h"
#include "types.h"
#include "printf.h"

void interrupt_init()
{
	disable_irq();
	gicv2_init();
}

void print_daif()
{
	u32 daif;
	asm volatile("mrs %0, daif" : "=r"(daif) :);
	printf("DAIF: 0x%x\n", daif);
}
