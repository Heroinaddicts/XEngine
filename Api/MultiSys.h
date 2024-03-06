#ifndef __MultiSys_h__
#define __MultiSys_h__

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdarg.h>
#include <typeinfo>

#pragma execution_character_set("utf-8")

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

#define xmin(x, y) (x) > (y) ? (y) : (x)
#define xmax(x, y) (x) > (y) ? (x) : (y)

#endif //__MultiSys_h__
