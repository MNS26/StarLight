#include "application.h"
#include "game_types.h"

#include "logger.h"

#include "platform/platform.h"
#include "core/slmemory.h"
#include "core/event.h"
#include "core/input.h"
#include "core/clock.h"

#include "renderer/renderer_frontend.h"


#include <SDL3/SDL_keyboard.h>
#include <SDL3/SDL_scancode.h>

typedef struct application_state {
  game* game_instance;
  b8 is_running;
  b8 is_suspended;
  platform_state platform_state;
  s16 width;
  s16 height;
  clock clock;
  f64 last_time;
} application_state;

static b8 initialized = false;
static application_state app_state;

// Event handlers
b8 application_on_event(u32 code, void* sender, void* listener_instance, event_context context);
b8 application_on_key(u32 code, void* sender, void* listener_instance, event_context context);
b8 application_on_mouse(u32 code, void* sender, void* listener_instance, event_context context);
b8 application_on_resized(u32 code, void* sender,void* listener_instance, event_context context);
b8 application_on_window_state(u32 code, void* sender, void* listener_instance, event_context context);

b8 application_create(game* game_instance) {
  if (initialized) {
    SLERROR("application_create called more than once.");
    return false;
  }
  app_state.game_instance = game_instance;

  app_state.width = game_instance->config.start_width;
  app_state.height = game_instance->config.start_height;

  input_initialize();

  app_state.is_running = true;
  app_state.is_suspended = false;

  if (!event_initialize()) {
    SLERROR("Event system failed to initialize! Application cannot continue.");
    return false;
  }

  event_register(EVENT_CODE_APPLICATION_QUIT, 0, application_on_event);
  event_register(EVENT_CODE_KEY_PRESSED, 0, application_on_key);
  event_register(EVENT_CODE_KEY_RELEASED, 0, application_on_key);
  event_register(EVENT_CODE_MOUSE_BUTTON_PRESSED, 0, application_on_mouse);
  event_register(EVENT_CODE_MOUSE_BUTTON_RELEASED, 0, application_on_mouse);
  event_register(EVENT_CODE_MOUSE_WHEEL, 0, application_on_mouse);
  event_register(EVENT_CODE_MOUSE_MOVED, 0, application_on_mouse);
  event_register(EVENT_CODE_WINDOW_RESIZED, 0, application_on_resized);
  event_register(EVENT_CODE_WINDOW_MINIMIZED, 0, application_on_window_state);
  event_register(EVENT_CODE_WINDOW_MAXIMIZED, 0, application_on_window_state);
  event_register(EVENT_CODE_WINDOW_RESTORED, 0, application_on_window_state);

  if (!platform_startup(
                        &app_state.platform_state,
                        game_instance->config.name,
                        game_instance->config.start_pos_x,
                        game_instance->config.start_pos_y,
                        game_instance->config.start_width,
                        game_instance->config.start_height
                      )) {
    return false;
  }

  // Renderer startup
  if (!renderer_initialize(game_instance->config.name, &app_state.platform_state)) {
    SLFATAL("Failed to initialize renderer! aborting application");
    return false;
  }

  //Initialize the game
  if (!app_state.game_instance->initialize(app_state.game_instance)) {
    SLFATAL("Unable to initialize game!");
    return false;
  }

  app_state.game_instance->on_resize(app_state.game_instance, app_state.width, app_state.height);

  initialized = true;
  return true;
}

b8 application_run() {
  clock_start(&app_state.clock);
  clock_update(&app_state.clock);
  app_state.last_time = app_state.clock.elapsed;
  f64 running_time = 0;
  u8 frame_count = 0;
  f64 target_frame_count = 1.0f/60;


  SLINFO(get_memory_usage_str());

  while (app_state.is_running) {
    if(!platform_pump_messages(&app_state.platform_state)) {
      app_state.is_running = false;
    }

    event_process_queue();

    if (!app_state.is_suspended) {
      // Update clock and delta time
      clock_update(&app_state.clock);
      f64 current_time = app_state.clock.elapsed;
      f64 delta = (current_time - app_state.last_time);
      f64 frame_start_time = platform_get_absolute_time();

      // Call game update routine
      if (!app_state.game_instance->update(app_state.game_instance,(f32)delta)) {
        SLFATAL("Game update failed, Shutting down.");
        app_state.is_running = false;
        break;
      }

      // Call game render routine
      if (!app_state.game_instance->render(app_state.game_instance,(f32)delta)) {
        SLFATAL("Game render failed, Shutting down.");
        app_state.is_running = false;
        break;
      }
      
      // todo. fix this! this is jank
      render_packet packet;
      packet.delta_time = delta;
      renderer_draw_frame(&packet);

      // Find out how long the frame took
      f64 frame_end_time = platform_get_absolute_time();
      f64 frame_elapsed_time = frame_end_time - frame_start_time;
      running_time += frame_elapsed_time;
      f64 remaining_seconds = target_frame_count - frame_elapsed_time;

      if (remaining_seconds > 0) {
        u64 remaining_ms = (remaining_seconds + 1000);

        b8 limit_frames = false;
        if (remaining_ms > 0 && limit_frames)
          platform_sleep(remaining_ms - 1);
        
        frame_count++;
      }

      // update input  subsystem state
      input_update(delta);

      // Update last time
      app_state.last_time = current_time;
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
  event_unregister(EVENT_CODE_WINDOW_RESIZED, 0, application_on_resized);
  event_unregister(EVENT_CODE_WINDOW_MINIMIZED, 0, application_on_window_state);
  event_unregister(EVENT_CODE_WINDOW_MAXIMIZED, 0, application_on_window_state);
  event_unregister(EVENT_CODE_WINDOW_RESTORED, 0, application_on_window_state);

  event_shutdown();
  input_shutdown();

  renderer_shutdown();

  platform_shutdown(&app_state.platform_state);
  return true;
}

void application_get_framebuffer_size(u32 *width, u32 *height) {
  *width = app_state.width;
  *height = app_state.height;
}

b8 application_on_event(u32 code, void* sender, void* listener_instance, event_context context) {
  switch (code) {
    case EVENT_CODE_APPLICATION_QUIT: {
        SLINFO("EVENT_CODE_APPLICATION_QUIT received, shutting down.\n")
        app_state.is_running = false;
        return true;
      };
    default:
      return false;
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
      return true;
    } else if (scancode == SDL_SCANCODE_Q) {
      SLDEBUG("key: %s",key_name);
      return true;
    } else if (scancode == SDL_SCANCODE_W) {
      SLDEBUG("key: %s",key_name);
      return true;
    } else if (scancode == SDL_SCANCODE_A) {
      SLDEBUG("key: %s",key_name);
      return true;
    } else if (scancode == SDL_SCANCODE_S) {
      SLDEBUG("key: %s",key_name);
      return true;
    } else if (scancode == SDL_SCANCODE_D) {      
      SLDEBUG("key: %s",key_name);
      return true;
    } else {
      SLDEBUG("key: %s",key_name);
      return true;
    }
  }
  return false;
}


b8 application_on_mouse(u32 code, void* sender, void* listener_instance, event_context context) {
  if (code == EVENT_CODE_MOUSE_BUTTON_PRESSED) {
    mouse_buttons button = context.data.u16[0];
    if (button == BUTTON_LEFT) {
      SLDEBUG("mouse button: %i",button);
      return true;
    } else if (button == BUTTON_MIDDLE) {
      SLDEBUG("mouse button: %i",button);
      return true;
    } else if (button == BUTTON_RIGHT) {
      SLDEBUG("mouse button: %i",button);
      return true;
    } else if (button == BUTTON_X1) {
      SLDEBUG("mouse button: %i",button);
      return true;
    } else if (button == BUTTON_X2) {
      SLDEBUG("mouse button: %i",button);
      return true;
    } else {
      SLDEBUG("mouse button: %i",button);
      return true;
    }
  } else if (code == EVENT_CODE_MOUSE_MOVED) {
    s16 x = context.data.s16[0];
    s16 y = context.data.s16[1];
//    SLDEBUG("mouse position: %i, %i",x,y);
    return true;
  } else if (code == EVENT_CODE_MOUSE_WHEEL) {
    f32 x = context.data.f32[0];
    f32 y = context.data.f32[1];
    SLDEBUG("mouse wheel: %.3f.%.3f",x, y);
    return true;

  }
  return false;
}

b8 application_on_resized(u32 code, void *sender, void *listener_instance, event_context context) {
  if (code == EVENT_CODE_WINDOW_RESIZED) {
    u16 width = context.data.u16[0];
    u16 height = context.data.u16[1];
    SLDEBUG("Event data: %i, %i", width, height);

    // Ignore zero-sized updates (some platforms report 0x0 while minimized).
    if (width == 0 || height == 0) {
      return true;
    }

    // Check if different.
    // If so trigger resize event.
    if (width != app_state.width || height != app_state.height) {
      app_state.width = width;
      app_state.height = height;

      SLDEBUG("Window resize event: %i, %i", width, height);

      if (app_state.is_suspended) {
        SLINFO("Window restored, resuming application.");
        app_state.is_suspended = false;
      }
      app_state.game_instance->on_resize(app_state.game_instance, width, height);
      renderer_on_resize(width, height);
    }
  }
  // Purposefully not handled to allow other listeners to also receive it
  return false;
}

b8 application_on_window_state(u32 code, void *sender, void *listener_instance, event_context context) {
  switch (code) {
    case EVENT_CODE_WINDOW_MINIMIZED:
      SLINFO("Window minimized, suspending application.");
      app_state.is_suspended = true;
      return true;
    case EVENT_CODE_WINDOW_MAXIMIZED:
      SLINFO("Window maximized.");
      return true;
    case EVENT_CODE_WINDOW_RESTORED:
      if (app_state.is_suspended) {
        SLINFO("Window restored, resuming application.");
        app_state.is_suspended = false;
      }
      // The swapchain was invalidated while minimized, so force a rebuild
      // even if the window size is unchanged.
      app_state.game_instance->on_resize(app_state.game_instance, app_state.width, app_state.height);
      renderer_on_resize(app_state.width, app_state.height);
      return true;
    default:
      return false;
  }
}
