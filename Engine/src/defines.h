#pragma once


// unsigned int types
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

// signed int types
typedef signed char s8;
typedef signed short s16;
typedef signed int s32;
typedef signed long long s64;

// float types
typedef float f32;
typedef double f64;

// bool types
typedef char b8;
typedef short b16;
typedef int b32;
typedef long long b64;


// define static asserts
#if defined(__clang__) || defined(__gcc__)
#define STATIC_ASSERT _Static_assert
#else
#define STATIC_ASSERT static_assert
#endif

// make sure all the sizes are correct
STATIC_ASSERT(sizeof(u8 ) == 1, "Expected u8 to be 1 byte!"  );
STATIC_ASSERT(sizeof(u16) == 2, "Expected u16 to be 2 bytes!");
STATIC_ASSERT(sizeof(u32) == 4, "Expected u32 to be 4 bytes!");
STATIC_ASSERT(sizeof(u64) == 8, "Expected u64 to be 8 bytes!");

STATIC_ASSERT(sizeof(s8 ) == 1, "Expected u8 to be 1 byte!"  );
STATIC_ASSERT(sizeof(s16) == 2, "Expected u16 to be 2 bytes!");
STATIC_ASSERT(sizeof(s32) == 4, "Expected u32 to be 4 bytes!");
STATIC_ASSERT(sizeof(s64) == 8, "Expected u64 to be 8 bytes!");

STATIC_ASSERT(sizeof(f32) == 4, "Expected f32 to be 4 bytes!");
STATIC_ASSERT(sizeof(f64) == 8, "Expected f64 to be 8 bytes!");

STATIC_ASSERT(sizeof(b8 ) == 1, "Expected b8 to be 1 byte!"  );
STATIC_ASSERT(sizeof(b16) == 2, "Expected b16 to be 2 bytes!");
STATIC_ASSERT(sizeof(b32) == 4, "Expected b32 to be 4 bytes!");
STATIC_ASSERT(sizeof(b64) == 8, "Expected b64 to be 8 bytes!");

#define TRUE 1
#define FALSE 0

//some platform detection

// WINDOWS
#if defined(WIN32)||defined(_WIN32)||defined(__WIN32__)
#define PLATFORM_WINDOWS
#ifndef _WIN64
#error "Requires 64 bit machine"
#endif
// LINUX/ANDROID/
#elif defined(__linux__)||defined(__gnu_linux__)
#define PLATFORM_LINUX
#if defined(__ANDROID__)
#define PLATFORM_ANDROID
#endif
// UNIX
#elif defined(__unix__)
#define PLATFORM_UNIX
#error "UNIX not supported"
// POSIX
#elif defined(_POSIX_VERSION)
#define PLATFORM_POSIX
#error "POSIX not supported"
// APPLE
#elif __APPLE__
#define PLATFORM_APPLE
#error "APPLE/IOS not supported"
#else
#error "Unknown platform"
#endif


// some import and export templating i found online
#ifdef SLEXPORT
// Exporting
#ifdef _MSC_VER
#define SALPI __declspec(dllexport) // windows
#else
#define SLAPI __attribute__((visibility("default"))) // others
#endif
#else
// Importing
#ifdef _MSC_VER
#define SLAPI __declspec(dllimport) // windows
#else
#define SLAPI // stub
#endif
#endif
//#include "defines.h"