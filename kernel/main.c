#include "uart.h"
#include "printf.h"
#include "timer.h"
#include "irq.h"
#include "types.h"
#include "tools.h"
#include "machine.h"

char stack[4096];

void main()
{
	uart_init();
	printf("Booting...\n");

	interrupt_init();
	printf("Interrupt inited\n");

	timer_init();
	printf("Timer inited\n");

	enable_irq();
	printf("IRQ enabled\n");
	print_daif();

	while (1)
		;
}
