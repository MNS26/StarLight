#pragma once

#include "defines.h"
#include "includes.h"
#define LOG_CRITICAL_ENABLED 1
#define LOG_ERROR_ENABLED 1
#define LOG_WARN_ENABLED 1
#define LOG_INFO_ENABLED 1

// Enable debug and trace on debug builds
#ifdef DISABLE_DEBUG
#define LOG_DEBUG_ENABLED 0
#define LOG_TRACE_ENABLED 0
#else
#define LOG_DEBUG_ENABLED 1
#define LOG_TRACE_ENABLED 1
#endif

typedef enum LOG_LEVEL {
  LOG_LEVEL_TRACE,
  LOG_LEVEL_DEBUG,
  LOG_LEVEL_USER,
  LOG_LEVEL_INFO,
  LOG_LEVEL_WARN,
  LOG_LEVEL_ERROR,
  LOG_LEVEL_CRITICAL,

  LOG_LEVEL_MAX
} log_level;


b8 logger_initialize();
void shutdown_logger();
template <typename... Args>
void SLLOG(LOG_LEVEL level, std::format_string<Args...> fmt, Args&&... args);

template <typename... Args>
void SLCRITICAL(std::format_string<Args...> fmt, Args&&... args);

template <typename... Args>
void SLERROR(std::format_string<Args...> fmt, Args&&... args);

template <typename... Args>
void SLWARNING(std::format_string<Args...> fmt, Args&&... args);

template <typename... Args>
void SLINFO(std::format_string<Args...> fmt, Args&&... args);

template <typename... Args>
void SLUSER(std::format_string<Args...> fmt, Args&&... args);

template <typename... Args>
void SLDEBUG(std::format_string<Args...> fmt, Args&&... args);

template <typename... Args>
void SLTRACE(std::format_string<Args...> fmt, Args&&... args);

void log_output(log_level level, std::string message);

template <typename... Args>
inline void SLLOG(LOG_LEVEL level, std::format_string<Args...> fmt, Args &&...args) {
  std::string message = std::format(fmt, std::forward<Args>(args)...);
  log_output(level, message);
}

template <typename... Args>
inline void SLCRITICAL(std::format_string<Args...> fmt, Args &&...args) {
  std::string message = std::format(fmt, std::forward<Args>(args)...);
  log_output(LOG_LEVEL_CRITICAL, message);
}

template <typename... Args>
inline void SLERROR(std::format_string<Args...> fmt, Args &&...args) {
  std::string message = std::format(fmt, std::forward<Args>(args)...);
  log_output(LOG_LEVEL_ERROR, message);
}

template <typename... Args>
inline void SLWARNING(std::format_string<Args...> fmt, Args &&...args) {
  std::string message = std::format(fmt, std::forward<Args>(args)...);
  log_output(LOG_LEVEL_WARN, message);
}

template <typename... Args>
inline void SLINFO(std::format_string<Args...> fmt, Args &&...args) {
  std::string message = std::format(fmt, std::forward<Args>(args)...);
  log_output(LOG_LEVEL_INFO, message);
}

template <typename... Args>
inline void SLUSER(std::format_string<Args...> fmt, Args &&...args) {
  std::string message = std::format(fmt, std::forward<Args>(args)...);
  log_output(LOG_LEVEL_USER, message);
}

template <typename... Args>
inline void SLDEBUG(std::format_string<Args...> fmt, Args &&...args) {
  std::string message = std::format(fmt, std::forward<Args>(args)...);
  log_output(LOG_LEVEL_DEBUG, message);
}

template <typename... Args>
inline void SLTRACE(std::format_string<Args...> fmt, Args &&...args) {
  std::string message = std::format(fmt, std::forward<Args>(args)...);
  log_output(LOG_LEVEL_TRACE, message);
}
