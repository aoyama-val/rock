#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <sdl.h>

template<typename T>
int clamp(T* a, T min, T max)
{
    if (*a < min) {
        *a = min;
        return -1;
    }
    if (*a > max) {
        *a = max;
        return 1;
    }
    return 0;
}

double randDouble(void);
const char* startswith(const char *buf, const char *index);
void Quit(int returnCode);
void printfv(FILE* stream, float *f, int n);
void dprintf(const char *format, ...);
void eprintf(const char *format, ...);
int OpenGLErrorCheck(void);

#endif
