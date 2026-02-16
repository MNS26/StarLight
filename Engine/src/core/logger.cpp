#include "defines.h"
#include "includes.h"
#include "logger.h"
#include "platform/platform.h"
#include "spdlog/spdlog.h"
#include "spdlog/logger.h"
#include "spdlog/sinks/stdout_color_sinks.h"


b8 Logger::initialize_logger() {
  // TODO: initialize spdlog

  spdlog::set_level((spdlog::level::level_enum)(7-LOG_LEVEL_MAX-1));
  spdlog::set_pattern("%^[%-8l] %v%$");
  //TODO: setup log file
  return TRUE;
}

void Logger::shutdown_logger() {
  // TODO: write/close log file
  spdlog::shutdown();
}


SAPI void Logger::log_output(log_level level, std::string message) {
  b8 is_error = level <= LOG_LEVEL_ERROR;

  switch (level)
  {
    case LOG_LEVEL_TRACE:
      spdlog::trace(message.c_str());
      break;
    
    case LOG_LEVEL_DEBUG:
      spdlog::debug(message.c_str());
      break;
  
    case LOG_LEVEL_INFO:
      spdlog::info(message.c_str());
      break;
    
    case LOG_LEVEL_WARN:
      spdlog::warn(message.c_str());
      break;
  
    case LOG_LEVEL_ERROR:
      spdlog::error(message.c_str());
      break;
    
    case LOG_LEVEL_CRITICAL:
      spdlog::critical(message.c_str());
      break;

    default:
      printf("%s\n",message.c_str());
      break;
  }
}
