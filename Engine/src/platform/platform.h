#pragma once

#include "defines.h"

typedef struct platform_state {
  void* internal_state;
} platform_state;

b8 platform_startup(platform_state* platform_state, const char* app_name, s32 x, s32 y, s32 width, s32 height);


void platform_shutdown(platform_state* platform_state);
b8 platform_pump_messages(platform_state* platform_state);

void* platform_allocate(u64 size, b8 alligned);
void platform_free(void* block, b8 alligned);
void* platform_zero_memory(void* block, u64 size);
void* platform_copy_memory(void* dest, const void* source, u64 size);
void* platform_set_memory(void* dest, s32 value, u64 size);

void platform_console_write(const char* message, u8 color);
void platform_console_write_error(const char* message, u8 color);

f64 platform_get_absolute_time();

void platform_sleep(u64 ms);