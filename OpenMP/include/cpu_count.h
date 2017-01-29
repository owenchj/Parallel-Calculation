#ifndef __CPU_COUNT_H__
#define __CPU_COUNT_H__
#include <stdint.h>

//  Windows
#ifdef _WIN32

#include <intrin.h>
uint64_t _rdtsc(){
    return __rdtsc();
}

//  Linux/GCC
#else

uint64_t _rdtsc(){
    unsigned int lo,hi;
    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    return ((uint64_t)hi << 32) | lo;
}

#endif

#endif // __CPU_COUNT_H__
