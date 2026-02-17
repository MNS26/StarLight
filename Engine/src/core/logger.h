#pragma once

#include "defines.h"
#include "includes.h"

//#include "spdlog/spdlog.h"
//#include "spdlog/logger.h"


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

class Logger{
private:
public:
  typedef enum LOG_LEVEL {
    LOG_LEVEL_TRACE,
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARN,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_CRITICAL,
    LOG_LEVEL_LOG,
    LOG_LEVEL_MAX = LOG_LEVEL_LOG
  } log_level;


  b8 initialize_logger();
  void shutdown_logger();

  SLAPI void log_output(log_level level, std::string message);

};

extern Logger logger;

#define LOG(...) logger.log_output(logger.LOG_LEVEL_LOG, std::format(__VA_ARGS__))// Log message

#if LOG_CRITICAL_ENABLED
#define CRITICAL(...) logger.log_output(logger.LOG_LEVEL_CRITICAL, std::format(__VA_ARGS__))// Logs Critical-level message
#else
#define CRITICAL(...)// Logs Critical-level message (DISABLED)
#endif
#if LOG_ERROR_ENABLED
#define ERROR(...) logger.log_output(logger.LOG_LEVEL_ERROR,  std::format(__VA_ARGS__))// Logs Error-level message
#else
#define ERROR(...)// Logs Error-level message (DISABLED)
#endif
#if LOG_WARN_ENABLED
#define WARNING(...) logger.log_output(logger.LOG_LEVEL_WARN,  std::format(__VA_ARGS__))// Logs Warning-level message
#else
#define WARNING(...)// Logs Warning-level message (DISABLED)
#endif
#if LOG_INFO_ENABLED
#define INFO(...) logger.log_output(logger.LOG_LEVEL_INFO,  std::format(__VA_ARGS__))// Logs Info-level message
#else
#define INFO(...)// Logs Info-level message (DISABLED)
#endif
#if LOG_USER_ENABLED
#define USER(...) logger.log_output(logger.LOG_LEVEL_USER,  std::format(__VA_ARGS__))// Logs User-level message
#else
#define USER(...)// Logs User-level message (DISABLED)
#endif
#if LOG_DEBUG_ENABLED
#define DEBUG(...) logger.log_output(logger.LOG_LEVEL_DEBUG,  std::format(__VA_ARGS__))// Logs Debug-level message
#else
#define DEBUG(...)// Logs Debug-level message (DISABLED)
#endif
#if LOG_TRACE_ENABLED
#define TRACE(...) logger.log_output(logger.LOG_LEVEL_TRACE,  std::format(__VA_ARGS__))// Logs Trace-level message
#else
#define TRACE(...)// Logs Trace-level message (DISABLED)
#endif
