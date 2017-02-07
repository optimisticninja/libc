#include <limits.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include <kernel/tty.h>

__attribute((__unused__))
static void print(const char* data, size_t data_length)
{
	for (size_t i = 0; i < data_length; i++)
		putchar((int) ((const unsigned char*) data)[i]);
}

#define INT_HEXSTRING_LENGTH ( sizeof(int) * CHAR_BIT / 4 )

/* Defined if I ever support weird architectures. */
static char const HEXDIGITS[0x10] = {
	 '0', '1', '2', '3', '4', '5', '6', '7',
	 '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
};

void int_to_hexstring(uint64_t value, char result[INT_HEXSTRING_LENGTH+1])
{
	int i;
	result[INT_HEXSTRING_LENGTH] = '\0';

	for (i = INT_HEXSTRING_LENGTH-1; value; i--, value >>= 4) {
		int d  = value & 0xF;
		result[i] = HEXDIGITS[d];
	}

	for ( ; i >= 0; i--)
		result[i] = '0';
}

int printf(const char* restrict format, ...)
{

	int written = 0;
	va_list parameters;
	va_start(parameters, format);	
#if defined(__is_kernel)
	terminal_printf(format, parameters);
#else	
 
	size_t amount;
	bool rejected_bad_specifier = false;
 
	while (*format != '\0') {
		if ( *format != '%' ) {
		print_c:
			amount = 1;
			while (format[amount] && format[amount] != '%')
				amount++;
			print(format, amount);
			format += amount;
			written += amount;
			continue;
		}
 
		const char* format_begun_at = format;
 
		if (*(++format) == '%')
			goto print_c;
 
		if (rejected_bad_specifier) {
		incomprehensible_conversion:
			rejected_bad_specifier = true;
			format = format_begun_at;
			goto print_c;
		}
 
		if (*format == 'c') {
			format++;
			char c = (char) va_arg(parameters, int /* char promotes to int */);
			print(&c, sizeof(c));
		} else if (*format == 's') {
			format++;
			const char* s = va_arg(parameters, const char*);
			print(s, strlen(s));
		} else if (*format == 'd') {
			format++;
			int i = va_arg(parameters, int);
			char* ia = itoa(i, NULL, 10);
			print(ia, strlen(ia));
		} else if (*format == 'u') {
			format++;
			unsigned u = va_arg(parameters, unsigned);
			char* ua = itoa(u, NULL, 10);
			print(ua, strlen(ua));
		} else if (*format == 'X') {
			format++;
			char buf[INT_HEXSTRING_LENGTH + 1];
			int_to_hexstring(va_arg(parameters, uint64_t));
			print(buf, strlen(buf));
		} else {
			goto incomprehensible_conversion;
		}
	}
 
	va_end(parameters);
#endif
 
	return written;
}

