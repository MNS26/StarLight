#include "logger.h"
#include "platform/platform.h"
#include "spdlog/spdlog.h"
#include "spdlog/logger.h"
#include "spdlog/sinks/stdout_color_sinks.h"


b8 Logger::initialize_logger() {
  // TODO: initialize spdlog

//  spdlog::set_level((spdlog::level::level_enum)((LOG_LEVEL_MAX-2)-sizeof(log_level)));
  spdlog::set_level((spdlog::level::level_enum)0);
  spdlog::set_pattern("%^[%-8l] %v%$");
  //TODO: setup log file
  return TRUE;
}

void Logger::shutdown_logger() {
  // TODO: write/close log file
  spdlog::shutdown();
}


SLAPI void Logger::log_output(log_level level, std::string message) {
  switch (level)
  {
    case LOG_LEVEL_TRACE:
#if LOG_TRACE_ENABLED
      spdlog::trace(message.c_str());
#endif
      break;

    case LOG_LEVEL_DEBUG:
#if LOG_DEBUG_ENABLED
      spdlog::debug(message.c_str());
#endif
      break;

    case LOG_LEVEL_INFO:
#if LOG_INFO_ENABLED
      spdlog::info(message.c_str());
#endif    
      break;

    case LOG_LEVEL_WARN:
#if LOG_WARN_ENABLED
      spdlog::warn(message.c_str());
#endif
      break;

    case LOG_LEVEL_ERROR:
#if LOG_ERROR_ENABLED
      spdlog::error(message.c_str());
#endif
      break;

    case LOG_LEVEL_CRITICAL:
#if LOG_CRITICAL_ENABLED
      spdlog::critical(message.c_str());
#endif
      break;

    default:
      printf("%s\n",message.c_str());
      break;
  }
}
