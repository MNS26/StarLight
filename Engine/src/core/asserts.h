#pragma once
#include "defines.h"

#define SLASSERTIONS_ENABLED

#ifdef SLASSERTIONS_ENABLED
#if _MSC_VER
#include <intrin.h>
#define debugBreak() __debugbreak()
#else
#define debugBreak() __builtin_trap()
#endif

SLAPI void report_assertion_failure(const char* expression, const char* message, const char* file, s32 line);

#define SLASSERT(expr) {                                      \
  if (expr) {                                                 \
  } else {                                                    \
    report_assertion_failure(#expr, "", __FILE__, __LINE__);  \
    debugBreak();                                             \
  }                                                           \
}
#define SLASSERT_MSG(expr, msg) {                             \
  if (expr) {                                                 \
  } else {                                                    \
    report_assertion_failure(#expr, msg, __FILE__, __LINE__); \
    debugBreak();                                             \
  }                                                           \
}
#if defined(IS_DEBUG)
#define SLASSERT_DEBUG(expr) {                                \
  if (expr) {                                                 \
  } else {                                                    \
    report_assertion_failure(#expr, "", __FILE__, __LINE__);  \
    debugBreak();                                             \
  }                                                           \
}
#else
#define SLASSERT_DEBUG(expr)
#endif
#else
#define SLASSERT(expr)
#define SLASSERT_MSG(expr, msg)
#define SLASSERT_DEBUG(expr)
#endif