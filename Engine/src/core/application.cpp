
#include "application.h"
#include "asserts.h"
#include "game_types.h"
#include "logger.h"
#include "memory.h"
#include "event.h"

#include "platform/platform.h"

#include <SDL3/SDL.h>

typedef struct applicationState {
  game* game_instance;
  b8 is_running;
  b8 is_suspended;
  platform_state platform;
  s32 width;
  s32 height;
  f64 first_time;
} applicationState;

static b8 initialized = FALSE;
static applicationState app_state;

b8 application_create(game* game_instance) {

  if (initialized) {
    SLERROR("application_create called more than once.");
    return FALSE;
  }

  app_state.game_instance = game_instance;

  SLCRITICAL("TEST {}", 0.1);
  SLERROR("TEST {}", 0.12);
  SLWARNING("TEST {}", 0.123);
  SLINFO("TEST {}", 0.1234);
  SLDEBUG("TEST {}", 0.12345);
  SLTRACE("TEST {}", 0.123456);
  
  platform_console_write(LOG_LEVEL_CRITICAL, "TEST platform_console_write {} {} {}", 01, 2,3);
  platform_console_write(LOG_LEVEL_ERROR, "TEST platform_console_write {} {} {}", 01, 2,3);
  platform_console_write(LOG_LEVEL_WARN, "TEST platform_console_write {} {} {}", 01, 2,3);
  platform_console_write(LOG_LEVEL_INFO, "TEST platform_console_write {} {} {}", 01, 2,3);
  platform_console_write(LOG_LEVEL_DEBUG, "TEST platform_console_write {} {} {}", 01, 2,3);
  platform_console_write(LOG_LEVEL_TRACE, "TEST platform_console_write {} {} {}", 01, 2,3);

  platform_console_write_error("TEST platform_console_write_error {} {} {}", 01, 2,3);

  app_state.is_running = TRUE;
  app_state.is_suspended = FALSE;

  if(!event_initialize()) {
    SLCRITICAL("Event system failed to initialize. Application cannot continue");
    return FALSE;
  }

  if (!platform_startup(
        &app_state.platform, 
        &game_instance->config.name,
        game_instance->config.start_pos_x,
        game_instance->config.start_pos_y,
        game_instance->config.start_width,
        game_instance->config.start_height)) {
    return FALSE;
  }

  // Initialize the game
  if (!app_state.game_instance->initialize(app_state.game_instance)) {
    platform_console_write(LOG_LEVEL_CRITICAL, "Game failed to intialize!");
    return FALSE;
  }

  app_state.game_instance->onResize(app_state.game_instance, app_state.width, app_state.height);
  initialized = TRUE;
  return TRUE;
}

b8 test1(void* data) {
  event_context* ctx = (event_context*)data;
  SLINFO("test1 {}", ctx->data.c);
  return FALSE;
}
b8 test2(void* data) {
  event_context* ctx = (event_context*)data;
  SLINFO("test2 {}", ctx->data.c);
  return FALSE;
}
b8 application_run() {
  SDL_Event event;
  SDL_zero(event);
  register_listener(SYSTEM_EVENT_TEST,test1);
  register_listener(SYSTEM_EVENT_TEST,test2);

  //scuffed but shoud
  SLINFO("{}",get_memory_usage_str());
  std::string a = "aaaaa";
  while (app_state.is_running) {
    while (SDL_PollEvent(&event)) { 
      emit_event(SYSTEM_EVENT_TEST, (event_context*)a.c_str());
      // add all the events in here
      if (!platform_handle_events(&app_state.platform, &event))
        app_state.is_running = FALSE;

      if (!app_state.is_suspended) {
        // Update call
        if (!app_state.game_instance->update(app_state.game_instance, (f64)0)) {
          SLCRITICAL("Game update failed, exiting!");
          app_state.is_running = FALSE;
          break;
        }
        // Render call
        if (!app_state.game_instance->render(app_state.game_instance, (f64)0)) {
          SLCRITICAL("Game render failed, exiting!");
          app_state.is_running = FALSE;
          break;
        }
      }
    }
    // TODO: replace with a proper idle loop
    platorm_sleep_ms(16);
  }
  app_state.is_running = FALSE;
  event_shutdown();
  platform_shutdown(&app_state.platform);
  return TRUE;
}