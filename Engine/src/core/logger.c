#include "logger.h"
#include "asserts.h"
#include <platform/platform.h>

#include <stdio.h>
#include <string.h>
#include <stdarg.h> 

//#include "spdlog/spdlog.h"

const char* log_level_str[LOG_LEVEL_MAX] = {"[FATAL] ","[ERROR] ","[WARN ] ","[INFO ] ","[DEBUG] ","[TRACE] "};

b8 initialize_logging() {
  return true;
}

void shutdown_logging() {

}
void log_output(log_level level, const char* message, ...) {
  b8 is_error = level < LOG_LEVEL_WARN;

  #warning "replace this!"
  char message_out[32000];
  memset(message_out, 0, ARRAY_SIZE(message_out));

  __builtin_va_list arg_ptr;
  va_start(arg_ptr, message);
  vsnprintf(message_out, ARRAY_SIZE(message_out), message, arg_ptr);
  va_end(arg_ptr);

  char out[32000];
  memset(out, 0, ARRAY_SIZE(out));

  snprintf(out, ARRAY_SIZE(out), "%s%s", log_level_str[level], message_out);
  if (is_error)
    platform_console_write_error(out, level);
  else
    platform_console_write(out, level);

}

void report_assertion_failure(const char* expression, const char* message, const char* file, s32 line) {
  log_output(LOG_LEVEL_FATAL, "Assertion Failure: %s message: '%s', in file: %s:%d\n",expression,message,file,line);
}
