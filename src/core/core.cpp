#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <core/core.hpp>

void die(const char *fmt, ...)
{
    va_list argp;
    va_start(argp, fmt);
    vfprintf(stderr, fmt, argp);
    va_end(argp);
    fputc('\n', stderr);
    abort();
}