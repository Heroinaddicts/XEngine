#include "MultiSys.h"

#include <stdio.h>
#include <assert.h>
#include <stdarg.h>
#include <malloc.h>

void __assert__(const char* file, int line, const char* funname, const char* format, ...) {
#ifdef _DEBUG
    char temp[256] = { 0 };
    va_list args;
    va_start(args, format);
    vsprintf(temp, format, args);
    va_end(args);

    fflush(stdout);
    printf("assert : %s:%s:%d\nassert info:%s\n", file, funname, line, temp);
    fflush(stderr);
    assert(false);
#endif //_DEBUG
}


void * aligned_malloc(size_t size, size_t alignment) {
    #ifdef WIN32
        return _aligned_malloc(size, alignment);
    #else
        return aligned_alloc(alignment, size);
    #endif //WIN32
}

void aligned_free(void * p) {
    #ifdef WIN32
        _aligned_free(p);
    #else
        free(p);
    #endif //WIN32
}
