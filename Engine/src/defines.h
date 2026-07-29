#pragma once

#define ARRAY_SIZE(array) sizeof(array)/sizeof(array[0])

// Unsigned int types
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

// Signed int types
typedef signed char s8;
typedef signed short s16;
typedef signed int s32;
typedef signed long long s64;

// Floating point types
typedef float f32;
typedef double f64;

// Boolean types
typedef char b8;
typedef short b16;
typedef int b32;
typedef long long b64;

// Define static asserts
#if defined(__clang__) || defined(__gcc__)
#define STATIC_ASSERT _Static_assert
#else
#define STATIC_ASSERT static_assert
#endif


// Ensure all types are the correct size
STATIC_ASSERT(sizeof(u8)  == 1, "Expected u8 to be 1 byte");
STATIC_ASSERT(sizeof(u16) == 2, "Expected u16 to be 2 bytes");
STATIC_ASSERT(sizeof(u32) == 4, "Expected u32 to be 4 bytes");
STATIC_ASSERT(sizeof(u64) == 8, "Expected u64 to be 8 bytes");

STATIC_ASSERT(sizeof(s8)  == 1, "Expected s8 to be 1 byte");
STATIC_ASSERT(sizeof(s16) == 2, "Expected s16 to be 2 bytes");
STATIC_ASSERT(sizeof(s32) == 4, "Expected s32 to be 4 bytes");
STATIC_ASSERT(sizeof(s64) == 8, "Expected s64 to be 8 bytes");

STATIC_ASSERT(sizeof(f32) == 4, "Expected f32 to be 4 bytes");
STATIC_ASSERT(sizeof(f64) == 8, "Expected f64 to be 8 bytes");

STATIC_ASSERT(sizeof(b8)  == 1, "Expected b8 to be 1 byte");
STATIC_ASSERT(sizeof(b16) == 2, "Expected b16 to be 2 bytes");
STATIC_ASSERT(sizeof(b32) == 4, "Expected b32 to be 4 bytes");
STATIC_ASSERT(sizeof(b64) == 8, "Expected b64 to be 8 bytes");

#define TRUE 1
#define FALSE 0


// Platform detection
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
// Windows
#define SLPLATFORM_WINDOWS
#ifndef _WIN64
#error "This engine only works on 64bit Windows"
#endif
#elif (defined(linux) || defined(__linux) || defined(__linux__)) || defined(__gnu_linux__)
// Linux
#define SLPLATFORM_LINUX 1
#if defined(ANDROID) || defined(__ANDROID__)
// Android
#define SLPLATFORM_ANDROID
#error "Unsupported platform"
#endif
#elif defined(__unix__) || defined(__unix) || defined(unix)
// Unix
#define SLPLATFORM_UNIX
#error "Unsupported platform"
#elif defined(_POSIX_VERSION)
// Posix
#define SLPLATFORM_POSIX
#error "Unsupported platform"
#elif __APPLE
// Apple
#define SLPLATFORM_APPLE
#error "Unsupported platform"
#include <TargetConditionals.h>
#if TARGET_IPHONE_SIMULATOR
// iOS Simulator
#define SLPLATFORM_IOS
#define SLPLATFORM_IOS_SIMULATOR
#elif TARGET_OS_PHONE
// iOS Device
#define SLPLATFORM_IOS
#elif TARGET_OS_MAC
// iOS MAC
#define SLPLATFORM_MAC
#else
#error "Unknown Apple platform"
#endif
#else
#error "Unsupported platform"
#endif

#ifdef SLEXPORT
// Exports
#ifdef _MSC_VER
#define SLAPI __declspec(dllexport)
#else
#define SLAPI __attribute__((visibility("default")))
#endif
#else
// Imports
#ifdef _MSC_VER
#define SLAPI __declspec(dllimport)
#else
#define SLAPI
#endif
#endif