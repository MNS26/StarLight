#include "defines.h"
#include "includes.h"

#include "logger.h"
#include "asserts.h"

#include <platform/platform.h>
#include <SDL3/SDL.h>

#include "application.h"

typedef struct applicationState {
  b8 isRunning;
  b8 isSuspended;
  platformState platform;
  s32 width;
  s32 height;
  f64 firstTime;
} applicationState;

static b8 initialized = FALSE;
static applicationState appState;

b8 applicationCreate(applicationConfig* config) {
  if (initialized) {
    ERROR("applicationCreate called more than once.");
    return FALSE;
  }
  // Init subsystems
  logger.initialize_logger();

  CRITICAL("TEST {}", 0.1);
  ERROR("TEST {}", 0.12);
  WARNING("TEST {}", 0.123);
  INFO("TEST {}", 0.1234);
  DEBUG("TEST {}", 0.12345);
  TRACE("TEST {}", 0.123456);
  
  platformConsoleWrite(logger.LOG_LEVEL_CRITICAL, "TEST platformConsoleWrite {} {} {}", 01, 2,3);
  platformConsoleWrite(logger.LOG_LEVEL_ERROR, "TEST platformConsoleWrite {} {} {}", 01, 2,3);
  platformConsoleWrite(logger.LOG_LEVEL_WARN, "TEST platformConsoleWrite {} {} {}", 01, 2,3);
  platformConsoleWrite(logger.LOG_LEVEL_INFO, "TEST platformConsoleWrite {} {} {}", 01, 2,3);
  platformConsoleWrite(logger.LOG_LEVEL_DEBUG, "TEST platformConsoleWrite {} {} {}", 01, 2,3);
  platformConsoleWrite(logger.LOG_LEVEL_TRACE, "TEST platformConsoleWrite {} {} {}", 01, 2,3);

  platformConsoleWriteError("TEST platformConsoleWriteError {} {} {}", 01, 2,3);


  appState.isRunning = TRUE;
  appState.isSuspended = FALSE;
  if (!platformStartup(&appState.platform, &config->name, config->startPosX, config->startPosY, config->startWidth, config->startHeight)) {
    return FALSE;
  }
  initialized = TRUE;
  return TRUE;
}

b8 applicationRun() {
  SDL_Event event;
  SDL_zero(event);
  while (appState.isRunning) {
    while (SDL_PollEvent(&event)) { 
      // add all the events in here
      if(platformHandleEvents(&appState.platform, &event)) continue;
        appState.isRunning = FALSE;
    }
    // TODO: replace with a proper idle loop
    platormSleepMs(16);
  }
  appState.isRunning = FALSE;
  platformShutdown(&appState.platform);
  return TRUE;
}