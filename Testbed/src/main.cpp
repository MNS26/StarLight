#include <core/logger.h>
#include <core/asserts.h>

#include <platform/platform.h>
#include <SDL3/SDL.h>

Logger logger;

int main(void)  {
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

  platformState state;
  if (platformStartup(&state, "StarLight Testbed", -1, -1, 1000, 625)) {
    state.running = true;
    SDL_Event event;
    SDL_zero(event);
    while (state.running) {
      while (SDL_PollEvent(&event)) { // add all the events in here
        if(platformHandleEvents(&state, &event)) continue;

      }
      // TODO: replace with a proper idle loop
      platormSleepMs(16);
    }
  }
//  ASSERT(1==0);
  return 0;
}