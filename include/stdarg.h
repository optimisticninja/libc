#pragma once

#pragma GCC diagnostic ignored "-Wint-conversion"

typedef int * va_list;

#define va_start( ap, v ) ((void) (ap = (va_list) &v + sizeof(v)))
#define va_end(ap) ((void) (ap = 0))
#define va_copy(dest, src) (dest = src)

#ifdef ORIGINAL
#define va_arg(ap, type) (*((type *)(ap))++)
#else
#define va_arg(ap, type) (*(ap)++)
#endif

