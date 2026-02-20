#pragma once
#include "core/logger.h"
#include "core/memory.h"
#include <SDL3/SDL.h>

typedef struct platform_state {
  b8 running = FALSE;
  void* internal_state;
} platform_state;

b8 platform_startup( platform_state* platform_state, std::string *application_name, s32 x, s32 y, s32 width, s32 height);

void platform_shutdown(platform_state* platform_state);

b8 platform_handle_events(platform_state* platform_state, SDL_Event* event);

void* platform_allocate(u64 size, b8 aligned);
void platform_free(void* block, b8 aligned);
void* platform_zero_memory(void* block, u64 size);
void* platform_copy_memory(void* dst, const void* src, u64 size);
void* platform_set_memory(void* dst, s32 val, u64 size);

template <typename... Args>
void platform_console_write(LOG_LEVEL level, std::format_string<Args...> fmt, Args&&... args) {
  std::string message = std::format(fmt, std::forward<Args>(args)...);
  log_output(level, message);
}

template <typename... Args>
void platform_console_write_error(std::format_string<Args...> fmt, Args&&... args) {
  std::string message = std::format(fmt, std::forward<Args>(args)...);
  log_output(LOG_LEVEL_ERROR, message);
}

f64 platform_get_absolute_time();

void platorm_sleep_ms(u64 miliseconds);
void platorm_sleep_us(u64 microseconds);