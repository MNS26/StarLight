#pragma once

#include "defines.h"

#define LOG_WARN_ENABLED
#define LOG_INFO_ENABLED
#define LOG_DEBUG_ENABLED
#define LOG_TRACE_ENABLED

// Disable debug and lower levels on release builds
#ifdef SLRELEASE
#undef LOG_DEBUG_ENABLED
#undef LOG_TRACE_ENABLED
#endif

typedef enum log_level {
  LOG_LEVEL_FATAL,
  LOG_LEVEL_ERROR,
#ifdef LOG_WARN_ENABLED
  LOG_LEVEL_WARN,
#endif
#ifdef LOG_INFO_ENABLED
  LOG_LEVEL_INFO,
#endif
#ifdef LOG_DEBUG_ENABLED
  LOG_LEVEL_DEBUG,
#endif
#ifdef LOG_TRACE_ENABLED
  LOG_LEVEL_TRACE,
#endif
  LOG_LEVEL_MAX,
} log_level;

b8 initialize_logging();
void shutdown_logging();

SLAPI void log_output(log_level level, const char* message, ...);

// Logs a fatal message
#define SLFATAL(message, ...) log_output(LOG_LEVEL_FATAL, message, ##__VA_ARGS__);

#ifndef SLERROR
// Logs a error message
#define SLERROR(message, ...) log_output(LOG_LEVEL_ERROR, message, ##__VA_ARGS__);
#endif

#ifdef LOG_WARN_ENABLED
// Logs a warn message
#define SLWARN(message, ...) log_output(LOG_LEVEL_WARN, message, ##__VA_ARGS__);
#else
#define SLWARN(message, ...)
#endif

#ifdef LOG_INFO_ENABLED
// Logs a info message
#define SLINFO(message, ...) log_output(LOG_LEVEL_INFO, message, ##__VA_ARGS__);
#else
#define SLINFO(message, ...)
#endif

#ifdef LOG_DEBUG_ENABLED
// Logs a debug message
#define SLDEBUG(message, ...) log_output(LOG_LEVEL_DEBUG, message, ##__VA_ARGS__);
#else
#define SLDEBUG(message, ...)
#endif

#ifdef LOG_TRACE_ENABLED
// Logs a trace message
#define SLTRACE(message, ...) log_output(LOG_LEVEL_TRACE, message, ##__VA_ARGS__);
#else
#define SLTRACE(message, ...)
#endif
