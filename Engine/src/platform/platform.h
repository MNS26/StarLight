#pragma once
#include "core/logger.h"
#include "core/memory.h"
#include <SDL3/SDL.h>

typedef struct platformState {
  b8 running = FALSE;
  void* internalState;
} platformState;

b8 platformStartup( platformState* platformState, std::string *applicationName, s32 x, s32 y, s32 width, s32 height);

void platformShutdown(platformState* platformState);

b8 platformHandleEvents(platformState* platformState, SDL_Event* event);

void* platformAllocate(u64 size, b8 aligned);
void platformFree(void* block, b8 aligned);
void* platformZeroMemory(void* block, u64 size);
void* platformCopyMemory(void* dst, const void* src, u64 size);
void* platformSetMemory(void* dst, s32 val, u64 size);

template <typename... Args>
void platformConsoleWrite(LOG_LEVEL level, std::format_string<Args...> fmt, Args&&... args) {
  std::string message = std::format(fmt, std::forward<Args>(args)...);
  log_output(level, message);
}

template <typename... Args>
void platformConsoleWriteError(std::format_string<Args...> fmt, Args&&... args) {
  std::string message = std::format(fmt, std::forward<Args>(args)...);
  log_output(LOG_LEVEL_ERROR, message);
}

//void platformConsoleWrite(Logger::LOG_LEVEL level, const char* message, ...);
//void platformConsoleWriteError(const char* message, ...);

f64 platformGetAbsoluteTime();

void platormSleepMs(u64 miliseconds);
void platormSleepUs(u64 microseconds);