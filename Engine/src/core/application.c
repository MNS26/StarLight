#include "application.h"
#include "game_types.h"

#include "logger.h"

#include "platform/platform.h"
#include "core/slmemory.h"
#include "core/event.h"
#include "core/input.h"

#include <SDL3/SDL_keyboard.h>
#include <SDL3/SDL_scancode.h>

typedef struct application_state {
  game* game_instance;
  b8 is_running;
  b8 is_suspended;
  platform_state platform_state;
  s16 width;
  s16 height;
  f64 last_time;
} application_state;

static b8 initialized = FALSE;
static application_state app_state;

// Event handlers
b8 application_on_event(u32 code, void* sender, void* listener_instance, event_context context);
b8 application_on_key(u32 code, void* sender, void* listener_instance, event_context context);
b8 application_on_mouse(u32 code, void* sender, void* listener_instance, event_context context);


b8 application_create(game* game_instance) {
  if (initialized) {
    SLERROR("application_create called more than once.");
    return FALSE;
  }
  app_state.game_instance = game_instance;

  input_initialize();

  app_state.is_running = TRUE;
  app_state.is_suspended = FALSE;

  if (!event_initialize()) {
    SLERROR("Event system failed to initialize! Application cannot continue.");
    return FALSE;
  }

  event_register(EVENT_CODE_APPLICATION_QUIT, 0, application_on_event);
  event_register(EVENT_CODE_KEY_PRESSED, 0, application_on_key);
  event_register(EVENT_CODE_KEY_RELEASED, 0, application_on_key);
  event_register(EVENT_CODE_MOUSE_BUTTON_PRESSED, 0, application_on_mouse);
  event_register(EVENT_CODE_MOUSE_BUTTON_RELEASED, 0, application_on_mouse);
  event_register(EVENT_CODE_MOUSE_WHEEL, 0, application_on_mouse);
  event_register(EVENT_CODE_MOUSE_MOVED, 0, application_on_mouse);

  if (!platform_startup(
                        &app_state.platform_state,
                        game_instance->config.name,
                        game_instance->config.start_pos_x,
                        game_instance->config.start_pos_y,
                        game_instance->config.start_width,
                        game_instance->config.start_height
                      )) {
    return FALSE;
  }

  //Initialize the game
  if (!app_state.game_instance->initialize(app_state.game_instance)) {
    SLFATAL("Unable to initialize game!");
    return FALSE;
  }

  app_state.game_instance->on_resize(app_state.game_instance, app_state.width, app_state.height);

  initialized = TRUE;
  return TRUE;
}

b8 application_run() {
  SLINFO(get_memory_usage_str());
  while (app_state.is_running) {
    if(!platform_pump_messages(&app_state.platform_state)) {
      app_state.is_running = FALSE;
    }

    event_process_queue();

    if (!app_state.is_suspended) {
  
      // Call game update routine
      if (!app_state.game_instance->update(app_state.game_instance,(f32)0)) {
        SLFATAL("Game update failed, Shutting down.");
        app_state.is_running = FALSE;
        break;
      }

      // Call game render routine
      if (!app_state.game_instance->render(app_state.game_instance,(f32)0)) {
        SLFATAL("Game render failed, Shutting down.");
        app_state.is_running = FALSE;
        break;
      }

      // update input subsystem state
      input_update(0);
    }
  }
  // Shutdown
  event_unregister(EVENT_CODE_APPLICATION_QUIT, 0, application_on_event);
  event_unregister(EVENT_CODE_KEY_PRESSED, 0, application_on_key);
  event_unregister(EVENT_CODE_KEY_RELEASED, 0, application_on_key);
  event_unregister(EVENT_CODE_MOUSE_BUTTON_PRESSED, 0, application_on_mouse);
  event_unregister(EVENT_CODE_MOUSE_BUTTON_RELEASED, 0, application_on_mouse);
  event_unregister(EVENT_CODE_MOUSE_WHEEL, 0, application_on_mouse);
  event_unregister(EVENT_CODE_MOUSE_MOVED, 0, application_on_mouse);

  event_shutdown();
  input_shutdown();
  platform_shutdown(&app_state.platform_state);
  return TRUE;
}

b8 application_on_event(u32 code, void* sender, void* listener_instance, event_context context) {
  switch (code) {
    case EVENT_CODE_APPLICATION_QUIT: {
        SLINFO("EVENT_CODE_APPLICATION_QUIT received, shutting down.\n")
        app_state.is_running = FALSE;
        return TRUE;
      };
    default:
      return FALSE;
  }
}

b8 application_on_key(u32 code, void* sender, void* listener_instance, event_context context) {
  if (code == EVENT_CODE_KEY_PRESSED) {
    SDL_Scancode scancode = context.data.u16[0];
    const char* key_name = SDL_GetScancodeName(scancode);
    if (scancode == SDL_SCANCODE_ESCAPE) {
      SLDEBUG("key: %s",key_name);
      event_context data = {};
      event_fire(EVENT_CODE_APPLICATION_QUIT, 0, data);
      return TRUE;
    } else if (scancode == SDL_SCANCODE_Q) {
      SLDEBUG("key: %s",key_name);
      return TRUE;
    } else if (scancode == SDL_SCANCODE_W) {
      SLDEBUG("key: %s",key_name);
      return TRUE;
    } else if (scancode == SDL_SCANCODE_A) {
      SLDEBUG("key: %s",key_name);
      return TRUE;
    } else if (scancode == SDL_SCANCODE_S) {
      SLDEBUG("key: %s",key_name);
      return TRUE;
    } else if (scancode == SDL_SCANCODE_D) {      
      SLDEBUG("key: %s",key_name);
      return TRUE;
    } else {
      SLDEBUG("key: %s",key_name);
      return TRUE;
    }
  }
  return FALSE;
}


b8 application_on_mouse(u32 code, void* sender, void* listener_instance, event_context context) {
  if (code == EVENT_CODE_MOUSE_BUTTON_PRESSED) {
    mouse_buttons button = context.data.u16[0];
    if (button == BUTTON_LEFT) {
      SLDEBUG("mouse button: %i",button);
      return TRUE;
    } else if (button == BUTTON_MIDDLE) {
      SLDEBUG("mouse button: %i",button);
      return TRUE;
    } else if (button == BUTTON_RIGHT) {
      SLDEBUG("mouse button: %i",button);
      return TRUE;
    } else if (button == BUTTON_X1) {
      SLDEBUG("mouse button: %i",button);
      return TRUE;
    } else if (button == BUTTON_X2) {
      SLDEBUG("mouse button: %i",button);
      return TRUE;
    } else {
      SLDEBUG("mouse button: %i",button);
      return TRUE;
    }
  } else if (code == EVENT_CODE_MOUSE_MOVED) {
    s16 x = context.data.s16[0];
    s16 y = context.data.s16[1];
    SLDEBUG("mouse position: %i, %i",x,y);
    return TRUE;
  } else if (code == EVENT_CODE_MOUSE_WHEEL) {
    s8 x = context.data.s8[0];
    SLDEBUG("mouse wheel: %i",x);
    return TRUE;

  }
  return FALSE;
}
