#include "printf.h"
#include "uart.h"

#define MAX_INT_BUFF_SIZE 80

static void write_string(const char *str)
{
	int i;

	for (i = 0; str[i] != 0; i++)
		uart_putc(str[i]);
}

static void write_num(int base, unsigned long n)
{
	static const char hex[] = "0123456789abcdef";
	char buff[MAX_INT_BUFF_SIZE];
	int k = MAX_INT_BUFF_SIZE - 1;

	/* Special case for "0". */
	if (n == 0) {
		write_string("0");
		return;
	}

	/* NUL-terminate. */
	buff[k--] = 0;

	/* Generate the number. */
	while (n > 0) {
		buff[k] = hex[n % base];
		n /= base;
		k--;
	}

	/* Print the number. */
	write_string(&buff[k + 1]);
}

void tfp_format(char *format, va_list args)
{
	int d, i;
	char c, *s;
	unsigned long p, ul;
	int escape_mode = 0;

	/* Iterate over the format list. */
	for (i = 0; format[i] != 0; i++) {
		/* Handle simple characters. */
		if (!escape_mode && format[i] != '%') {
			if (format[i] == '\n') {
				uart_putc('\r');
			}
			uart_putc(format[i]);
			continue;
		}

		/* Handle the percent escape character. */
		if (format[i] == '%') {
			if (!escape_mode) {
				/* Entering escape mode. */
				escape_mode = 1;
			} else {
				/* Already in escape mode; print a percent. */
				uart_putc(format[i]);
				escape_mode = 0;
			}
			continue;
		}

		/* Handle the modifier. */
		switch (format[i]) {
			/* Ignore printf modifiers we don't support. */
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
		case '-':
		case '.':
			break;

			/* String. */
		case 's':
			s = va_arg(args, char *);
			write_string(s);
			escape_mode = 0;
			break;

			/* Pointers. */
		case 'p':
			p = va_arg(args, unsigned long);
			write_num(16, p);
			escape_mode = 0;
			break;

			/* Hex number. */
		case 'x':
			d = va_arg(args, int);
			write_num(16, (unsigned int)d);
			escape_mode = 0;
			break;

			/* Decimal number. */
		case 'd':
		case 'u':
			d = va_arg(args, int);
			write_num(10, d);
			escape_mode = 0;
			break;

			/* Character. */
		case 'c':
			c = va_arg(args, int);
			uart_putc(c);
			escape_mode = 0;
			break;

			/* Long number. */
		case 'l':
			switch (format[++i]) {
			case 'u':
				ul = va_arg(args, unsigned long);
				write_num(10, ul);
				break;

			case 'x':
				ul = va_arg(args, unsigned long);
				write_num(16, ul);
				break;

			default:
				uart_putc('?');
			}
			escape_mode = 0;
			break;

			/* Unknown. */
		default:
			uart_putc('?');
			escape_mode = 0;
			break;
		}
	}
}

void tfp_printf(char *fmt, ...)
{
	va_list va;

	va_start(va, fmt);
	tfp_format(fmt, va);
	va_end(va);
}
