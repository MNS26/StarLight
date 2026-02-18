#pragma once
#include "core/logger.h"
#include <SDL3/SDL.h>

extern Logger* logger;
typedef struct platformState {
  b8 running = FALSE;
  Logger logger;
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

// FIXME: im going isane if someone knows a better way PLEASE FIX THIS
//#define platformConsoleWrite(level, ...) logger.LOG(level, std::format(__VA_ARGS__))
//#define platformConsoleWriteError(...) logger.ERROR(std::format(__VA_ARGS__))


template <typename... Args>
void platformConsoleWrite(Logger::LOG_LEVEL level, std::format_string<Args...> fmt, Args&&... args) {
  std::string message = std::format(fmt, std::forward<Args>(args)...);
  logger->log_output(level, message);
}

template <typename... Args>
void platformConsoleWriteError(std::format_string<Args...> fmt, Args&&... args) {
  std::string message = std::format(fmt, std::forward<Args>(args)...);
  logger->log_output(logger->LOG_LEVEL_ERROR, message);
}

//void platformConsoleWrite(Logger::LOG_LEVEL level, const char* message, ...);
//void platformConsoleWriteError(const char* message, ...);

f64 platformGetAbsoluteTime();

void platormSleepMs(u64 miliseconds);
void platormSleepUs(u64 microseconds);