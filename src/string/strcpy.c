#include <string.h>

char *strcpy(char *dest, const char *src)
{
	char *temp = dest;
	while((*dest++ = *src++) != '\0');
	return temp;
}

