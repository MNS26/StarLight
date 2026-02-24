
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

//  SLCRITICAL("TEST {}", 0.1);
//  SLERROR("TEST {}", 0.12);
//  SLWARNING("TEST {}", 0.123);
//  SLINFO("TEST {}", 0.1234);
//  SLDEBUG("TEST {}", 0.12345);
//  SLTRACE("TEST {}", 0.123456);
  
//  platform_console_write(LOG_LEVEL_CRITICAL, "TEST platform_console_write {} {} {}", 01, 2,3);
//  platform_console_write(LOG_LEVEL_ERROR, "TEST platform_console_write {} {} {}", 01, 2,3);
//  platform_console_write(LOG_LEVEL_WARN, "TEST platform_console_write {} {} {}", 01, 2,3);
//  platform_console_write(LOG_LEVEL_INFO, "TEST platform_console_write {} {} {}", 01, 2,3);
//  platform_console_write(LOG_LEVEL_DEBUG, "TEST platform_console_write {} {} {}", 01, 2,3);
//  platform_console_write(LOG_LEVEL_TRACE, "TEST platform_console_write {} {} {}", 01, 2,3);

//  platform_console_write_error("TEST platform_console_write_error {} {} {}", 01, 2,3);

  app_state.is_running = TRUE;
  app_state.is_suspended = FALSE;

  if (!platform_startup(
        &app_state.platform, 
        &game_instance->config.name,
        game_instance->config.start_pos_x,
        game_instance->config.start_pos_y,
        game_instance->config.start_width,
        game_instance->config.start_height)) {
    return FALSE;
  }

  //if(!event_initialize()) {
  //  SLCRITICAL("Event system failed to initialize. Application cannot continue");
  //  return FALSE;
  //}
  if(!sdl_event_initialize()) {
    SLCRITICAL("Event system failed to initialize. Application cannot continue");
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
b8 test3(void* data) {
  event_context* ctx = (event_context*)data;
  SLINFO("test3 {}", ctx->data.c);
  return FALSE;
}
b8 test4(void* data) {
  event_context* ctx = (event_context*)data;
  SLINFO("test4 {}", ctx->data.c);
  return FALSE;
}
b8 test5(void* data) {
  event_context* ctx = (event_context*)data;
  SLINFO("test5 {}", ctx->data.c);
  return FALSE;
}
b8 test6(void* data) {
  event_context* ctx = (event_context*)data;
  SLINFO("test6 {}", ctx->data.c);
  return FALSE;
}
b8 test7(void* data) {
  event_context* ctx = (event_context*)data;
  SLINFO("test7 {}", ctx->data.c);
  return FALSE;
}
b8 test8(void* data) {
  event_context* ctx = (event_context*)data;
  SLINFO("test8 {}", ctx->data.c);
  return FALSE;
}
b8 test9(void* data) {
  event_context* ctx = (event_context*)data;
  SLINFO("test9 {}", ctx->data.c);
  return FALSE;
}
b8 test10(void* data) {
  event_context* ctx = (event_context*)data;
  SLINFO("test10 {}", ctx->data.c);
  return FALSE;
}
b8 test11(void* data) {
  event_context* ctx = (event_context*)data;
  SLINFO("test11 {}", ctx->data.c);
  return FALSE;
}
b8 test12(void* data) {
  event_context* ctx = (event_context*)data;
  SLINFO("test12 {}", ctx->data.c);
  return FALSE;
}
b8 test13(void* data) {
  event_context* ctx = (event_context*)data;
  SLINFO("test13 {}", ctx->data.c);
  return FALSE;
}
b8 test14(void* data) {
  event_context* ctx = (event_context*)data;
  SLINFO("test14 {}", ctx->data.c);
  return FALSE;
}
b8 test15(void* data) {
  event_context* ctx = (event_context*)data;
  SLINFO("test15 {}", ctx->data.c);
  return FALSE;
}
b8 test16(void* data) {
  event_context* ctx = (event_context*)data;
  SLINFO("test16 {}", ctx->data.c);
  return FALSE;
}
b8 application_run() {
  SDL_Event event;
  SDL_zero(event);
  //register_listener(SYSTEM_EVENT_TEST,test1);
  //register_listener(SYSTEM_EVENT_TEST,test2);
  //register_listener(SYSTEM_EVENT_TEST,test3);
  //register_listener(SYSTEM_EVENT_TEST,test4);
  //register_listener(SYSTEM_EVENT_TEST,test5);
  //register_listener(SYSTEM_EVENT_TEST,test6);
  //register_listener(SYSTEM_EVENT_TEST,test7);
  //register_listener(SYSTEM_EVENT_TEST,test8);
  //register_listener(SYSTEM_EVENT_TEST,test9);
  //register_listener(SYSTEM_EVENT_TEST,test10);
  //register_listener(SYSTEM_EVENT_TEST,test11);
  //register_listener(SYSTEM_EVENT_TEST,test12);
  //register_listener(SYSTEM_EVENT_TEST,test13);
  //register_listener(SYSTEM_EVENT_TEST,test14);
  //register_listener(SYSTEM_EVENT_TEST,test15);
  //register_listener(SYSTEM_EVENT_TEST,test16);

  sdl_event_register(SYSTEM_EVENT_TEST,test1);
  sdl_event_register(SYSTEM_EVENT_TEST,test2);
  sdl_event_register(SYSTEM_EVENT_TEST,test3);
  sdl_event_register(SYSTEM_EVENT_TEST,test4);
  sdl_event_register(SYSTEM_EVENT_TEST,test5);
  sdl_event_register(SYSTEM_EVENT_TEST,test6);
  sdl_event_register(SYSTEM_EVENT_TEST,test7);
  sdl_event_register(SYSTEM_EVENT_TEST,test8);
  sdl_event_register(SYSTEM_EVENT_TEST,test9);
  sdl_event_register(SYSTEM_EVENT_TEST,test10);
  sdl_event_register(SYSTEM_EVENT_TEST,test11);
  sdl_event_register(SYSTEM_EVENT_TEST,test12);
  sdl_event_register(SYSTEM_EVENT_TEST,test13);
  sdl_event_register(SYSTEM_EVENT_TEST,test14);
  sdl_event_register(SYSTEM_EVENT_TEST,test15);
  sdl_event_register(SYSTEM_EVENT_TEST,test16);

  //scuffed but shoud
  SLINFO("{}",get_memory_usage_str());
  std::string a = "aaaaa";
  while (app_state.is_running) {
    while (SDL_PollEvent(&event)) { 

      // add all the events in here
      if (!platform_handle_events(&app_state.platform, &event)) app_state.is_running = FALSE;

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
        sdl_custom_event_process(&event);

        sdl_event_push(SYSTEM_EVENT_TEST, (event_context*)a.c_str());

//        //      emit_event(SYSTEM_EVENT_TEST, (event_context*)a.c_str());
      }
    }
    // TODO: replace with a proper idle loop
    platorm_sleep_ms(16);
  }
  app_state.is_running = FALSE;
//  event_shutdown();
  platform_shutdown(&app_state.platform);
  return TRUE;
}