#pragma once

#include "defines.h"
#include "includes.h"
#include "core/logger.h"
// enable or disable assertions
#define ASSERTIONS_ENABLED

#ifdef ASSERTIONS_ENABLED
#if _MSC_VER
#include <intrin.h>
#define debugBreak() __debugbreak()
#else
// FIXME: wtf... this is soo stupid... why... just why... 
// gcc/clang doesnt have a proper way to do breakpoints ugh -_-
#if defined(__i386__) || defined(__x86_64__)
#define debugBreak() __asm__ volatile("int $0x03")
#elif defined(__thumb__)
#define debugBreak() __asm__ volatile(".inst 0xde01")
#elif defined(__arm__) && !defined(__thumb__)
#define debugBreak() __asm__ volatile(".inst 0xe7f001f0")
#elif (defined(__aarch64__) && defined(__APPLE__))
#define debugBreak()  __builtin_debugtrap()
#elif defined(__aarch64__)
#define debugBreak() __asm__ volatile(".inst 0xd4200000")
#elif defined(__powerpc__)
#define debugBreak() __asm__ volatile(".4byte 0x7d821008")
#elif defined(__riscv)
#define debugBreak() __asm__ volatile(".4byte 0x00100073")
#else
#warning "THIS ARCHITECTURE DOESN'T SUPPORT BREAKS!!!"
#define debugBreak()
#endif
#endif

//void repor_assert_failure(const char* expr, const char* mesg, const char* file, u32 line) {FATAL("Assertion Failure {} {} {}:{}",expr, mesg, file, line);};

#define ASSERT(expr) {                                 \
  if (expr) {}                                         \
  else {                                               \
FATAL("Assertion Failure {} {} {}:{}",#expr, "", __FILE__, __LINE__);\
    debugBreak();                                      \
  }                                                    \
}

#define ASSERT_MESG(expr, mesg) {                        \
  if (expr) {}                                           \
  else {                                                 \
FATAL("Assertion Failure {} {} {}:{}",#expr, mesg, __FILE__, __LINE__);\
    debugBreak();                                        \
  }                                                      \
}

#ifndef DISABLE_DEBUG
#define ASSERT_DEBUG(expr) {                           \
  if (expr) {}                                         \
  else {                                               \
FATAL("Assertion Failure {} {} {}:{}",#expr, "", __FILE__, __LINE__);\
    debugBreak();                                      \
  }                                                    \
}
#else
#define ASSERT_DEBUG(expr)
#endif
#else
#define ASSERT(expr)
#define ASSERT_MESG(expr, mesg)
#define ASSERT_DEBUG(expr)
#endif