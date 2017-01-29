#pragma once

#include <sys/cdefs.h>

#ifdef __cplusplus
extern "C" {
#endif

__attribute__((__noreturn__))
void abort(void);
char* itoa(int value, char* result, int base);

#ifdef __cplusplus
}
#endif
