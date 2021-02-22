#include "uart.h"
#include "tools.h"

void uart_send_string(char *str)
{
	for (int i = 0; str[i] != '\0'; i++)
		uart_putc((char)str[i]);
}

void uart_putc(char ch)
{
	/* Wait until there is space in the FIFO or device is disabled */
	while (get32((unsigned long)PL011_UART0_BASE + UART_FR) &
	       UART_FR_TXFF) {
	}
	/* Send the character */
	put32((unsigned long)PL011_UART0_BASE + UART_DR, (unsigned int)ch);
}

void uart_init(void)
{
	volatile unsigned int *base_rsr_ecr =
		(unsigned int *)(PL011_UART0_BASE + UART_RSR_ECR);
	volatile unsigned int *base_cr =
		(unsigned int *)(PL011_UART0_BASE + UART_CR);
	volatile unsigned int *base_lcr_h =
		(unsigned int *)(PL011_UART0_BASE + UART_LCR_H);

	//Init the uart
	/* Clear all errors */
	*base_rsr_ecr = 0;
	/* Disable everything */
	*base_cr = 0;
	/* Configure TX to 8 bits, 1 stop bit, no parity, fifo disabled. */
	*base_lcr_h = UART_LCRH_WLEN_8;
	/* Enable UART and RX/TX */
	*base_cr = UART_CR_UARTEN | UART_CR_TXE | UART_CR_RXE;
}
