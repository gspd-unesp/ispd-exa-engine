#include <core/core.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

void die(const char *fmt, ...)
{
    va_list argp;
    va_start(argp, fmt);
    vfprintf(stderr, fmt, argp);
    va_end(argp);
    fputc('\n', stderr);
    abort();
}
