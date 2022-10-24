#ifndef __MultiSys_h__
#define __MultiSys_h__

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdarg.h>

#pragma execution_character_set("utf-8")

#ifdef WIN32
typedef __int8  int8;
typedef __int16 int16;
typedef __int32 int32;
typedef __int64 int64;

typedef unsigned __int8  unsigned_int8;
typedef unsigned __int16 unsigned_int16;
typedef unsigned __int32 unsigned_int32;
typedef unsigned __int64 unsigned_int64;
#else
typedef char int8;
typedef short int16;
typedef int32_t int32;
typedef int64_t int64;

typedef unsigned char unsigned_int8;
typedef unsigned short unsigned_int16;
typedef uint32_t unsigned_int32;
typedef uint64_t unsigned_int64;

#define __forceinline inline
#define sprintf_s snprintf
#endif //WIN32

#define OUT
#define IN

#ifdef _DEBUG
#define xmalloc malloc
#define xfree free
#define xnew new
#define xdel delete
#else
#define xmalloc malloc
#define xfree free
#define xnew new
#define xdel delete
#endif //defined _DEBUG

void __assert__(const char* file, int line, const char* funname, const char* format, ...);
#define XASSERT(p, format, ...) {\
    (p) ? (void)0 : __assert__(__FILE__, __LINE__, __FUNCTION__, format, ##__VA_ARGS__);\
}

void* aligned_malloc(size_t size, size_t alignment);
void aligned_free(void* p);

#endif //__MultiSys_h__
