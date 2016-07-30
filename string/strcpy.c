#include <string.h>

char *strcpy(char *dest, const char *src)
{
	char *temp = dest;
    while((*dest++ = *src++) != '\0'); // or while((*strDest++=*strSrc++) != '\0');
    return temp;
}

