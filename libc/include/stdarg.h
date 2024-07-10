#ifndef STDARG_H
#define STDARG_H

typedef __builtin_va_list va_list;

#define va_start(vl, last) __builtin_va_start(vl, last)
#define va_arg(vl, type) __builtin_va_arg(vl, type)
#define va_end(vl) __builtin_va_end(vl)

#endif