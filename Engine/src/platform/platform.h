#pragma once

#include <SDL3/SDL.h>

typedef enum LOGLEVEL {
    LOG_LEVEL_TRACE,
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARN,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_CRITICAL,
    LOG_LEVEL_LOG,
    LOG_LEVEL_MAX = LOG_LEVEL_LOG
}LOGLEVEL;


typedef struct platformState {
  b8 running = FALSE;
  void* internalState;
} platformState;

b8 platformStartup( platformState* platformState, std::string *applicationName, s32 x, s32 y, s32 width, s32 height);

void platformShutdown(platformState* platformState);

b8 platformHandleEvents(platformState* platformState, SDL_Event* event);

SLAPI void* platformAllocate(u64 size, b8 aligned);
SLAPI void platformFree(void* block, b8 aligned);
void* platformZeroMemory(void* block, u64 size);
void* platformCopyMemory(void* dst, const void* src, u64 size);
void* platformSetMemory(void* dst, s32 val, u64 size);

// FIXME: im going isane if someone knows a better way PLEASE FIX THIS
#define platformConsoleWrite(level, ...) logger.log_output(level, std::format(__VA_ARGS__))
#define platformConsoleWriteError(...) logger.log_output(logger.LOG_LEVEL_ERROR, std::format(__VA_ARGS__))
//void platformConsoleWrite(u8 level, const char* message, ...);
//void platformConsoleWriteError(const char* message, ...);

f64 platformGetAbsoluteTime();

void platormSleepMs(u64 miliseconds);
void platormSleepUs(u64 microseconds);