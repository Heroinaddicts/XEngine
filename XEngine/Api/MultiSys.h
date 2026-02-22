#ifndef __MultiSys_h__
#define __MultiSys_h__

#define UsingXEngine using namespace XEngine

#ifdef WIN32
#pragma execution_character_set("utf-8")
#endif //WIN32

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>
#include <malloc.h>
#include <stdarg.h>
#include <typeinfo>
#include <functional>

#ifdef WIN32
typedef __int8  Int8;
typedef __int16 Int16;
typedef __int32 Int32;
typedef __int64 Int64;

typedef unsigned __int8  UInt8;
typedef unsigned __int16 UInt16;
typedef unsigned __int32 UInt32;
typedef unsigned __int64 UInt64;
#else
typedef char Int8;
typedef short Int16;
typedef int32_t Int32;
typedef int64_t Int64;

typedef unsigned char UInt8;
typedef unsigned short UInt16;
typedef uint32_t UInt32;
typedef uint64_t UInt64;

#define __forceinline inline
#define sprintf_s snprintf
#endif //WIN32

#define OUT
#define IN

__forceinline void __assert__(const char* file, int line, const char* funname, const char* format, ...) {
    char temp[256] = { 0 };
    va_list args;
    va_start(args, format);
    vsprintf(temp, format, args);
    va_end(args);

    fflush(stdout);
    printf("assert : %s:%s:%d\nassert info:%s\n", file, funname, line, temp);
    fflush(stderr);
    assert(false);
}

#ifdef _DEBUG
#define txmalloc malloc
#define txfree free

#define txnew new
#define txdel delete

#define XASSERT(p, format, ...) {\
    (p) ? (void)0 : __assert__(__FILE__, __LINE__, __FUNCTION__, format, ##__VA_ARGS__);\
}

#ifdef WIN32
#define tx_aligned_malloc _aligned_malloc
#define tx_aligned_free _aligned_free
#else
#define tx_aligned_malloc malloc
#define tx_aligned_free free
#endif //WIN32

#define ECHO(format, ...) printf(format##"\n", ##__VA_ARGS__);

#else
#define txmalloc malloc
#define txfree free

#define txnew new
#define txdel delete

#ifdef WIN32
#define tx_aligned_malloc _aligned_malloc
#define tx_aligned_free _aligned_free
#else
#define tx_aligned_malloc malloc
#define tx_aligned_free free
#endif //WIN32

#define ECHO(format, ...)
#define XASSERT(p, format, ...)
#endif //defined _DEBUG

#define xmin(x, y) (x) > (y) ? (y) : (x)
#define xmax(x, y) (x) > (y) ? (x) : (y)

#endif //__MultiSys_h__
