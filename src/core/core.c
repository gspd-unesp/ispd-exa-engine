#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <core/core.h>

void die(const char *fmt, ...)
{
    va_list argp;
    va_start(argp, fmt);
    vfprintf(stderr, fmt, argp);
    va_end(argp);
    fputc('\n', stderr);
    abort();
}
