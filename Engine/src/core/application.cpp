
#include "logger.h"
#include "asserts.h"
#include "platform/platform.h"
#include "gameTypes.h"

#include <SDL3/SDL.h>

#include "application.h"

typedef struct applicationState {
  game* gameInstance;
  b8 isRunning;
  b8 isSuspended;
  platformState platform;
  s32 width;
  s32 height;
  f64 firstTime;
} applicationState;

Logger logger;

static b8 initialized = FALSE;
static applicationState appState;

b8 applicationCreate(game* gameInstance) {
  if (initialized) {
    ERROR("applicationCreate called more than once.");
    return FALSE;
  }

  appState.gameInstance = gameInstance;

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
  if (!platformStartup(
        &appState.platform, 
        &gameInstance->config.name,
        gameInstance->config.startPosX,
        gameInstance->config.startPosY,
        gameInstance->config.startWidth,
        gameInstance->config.startHeight)) {
    return FALSE;
  }

  // Initialize the game
  if (!appState.gameInstance->initialize(appState.gameInstance)) {
    CRITICAL("Game failed to intialize!");
    return FALSE;
  }

  appState.gameInstance->onResize(appState.gameInstance, appState.width, appState.height);
  initialized = TRUE;
  return TRUE;
}

b8 applicationRun() {
  SDL_Event event;
  SDL_zero(event);
  while (appState.isRunning) {
    while (SDL_PollEvent(&event)) { 
      // add all the events in here
      if (!platformHandleEvents(&appState.platform, &event))
        appState.isRunning = FALSE;

      if (!appState.isSuspended) {
        // Update call
        if (!appState.gameInstance->update(appState.gameInstance, (f64)0)) {
          CRITICAL("Game update failed, exiting!");
          appState.isRunning = FALSE;
          break;
        }
        // Render call
        if (!appState.gameInstance->render(appState.gameInstance, (f64)0)) {
          CRITICAL("Game render failed, exiting!");
          appState.isRunning = FALSE;
          break;
        }
      }
    }
    // TODO: replace with a proper idle loop
    platormSleepMs(16);
  }
  appState.isRunning = FALSE;
  platformShutdown(&appState.platform);
  return TRUE;
}