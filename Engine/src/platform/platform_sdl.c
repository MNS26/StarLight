#include "platform/platform.h"

#if defined(SLPLATFORM_LINUX) || defined(SLPLATFORM_WINDOWS)

#include <core/logger.h>
#include <core/slmemory.h>
#include <core/input.h>
#include <SDL3/SDL.h>

//#if defined(SLPLATFORM_LINUX)
//#if _POSIX_C_SOURCE >= 199309L
//#include <time.h>
//#else
//#include <unistd.h>
//#endif
//#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct internal_state {
  SDL_Window* Window;
} internal_state;


b8 platform_startup(platform_state* platform_state, const char* app_name, s32 x, s32 y, s32 width, s32 height) {
  platform_state->internal_state = malloc(sizeof(internal_state));
  internal_state *state = (internal_state*)platform_state->internal_state;

  SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_JOYSTICK | SDL_INIT_GAMEPAD);

  state->Window = SDL_CreateWindow(app_name, width, height, SDL_WINDOW_RESIZABLE | SDL_WINDOW_VULKAN);

  if (state->Window == NULL) {
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create window: %s\n", SDL_GetError());
    return FALSE;
  }
  return TRUE;
}

void platform_shutdown(platform_state* platform_state) {
  internal_state *state = (internal_state*)platform_state->internal_state;
  SDL_DestroyWindow(state->Window);
  SDL_Quit();
}

b8 platform_pump_messages(platform_state* platform_state) {
  SDL_Event event;
  b8 quit_flagged = FALSE;

  SDL_PumpEvents();

  while (SDL_PeepEvents(&event, 1, SDL_GETEVENT, SDL_EVENT_FIRST, SDL_EVENT_USER - 1) > 0) {
    switch (event.type) {
      case SDL_EVENT_QUIT:
        quit_flagged = TRUE;
        break;
      case SDL_EVENT_KEY_UP:
      case SDL_EVENT_KEY_DOWN:
        input_process_key(event.key.scancode, event.key.down);
        break;
      case SDL_EVENT_MOUSE_BUTTON_UP:
      case SDL_EVENT_MOUSE_BUTTON_DOWN:
        input_process_mouse_button(event.button.button, event.button.down);
        break;
      case SDL_EVENT_MOUSE_MOTION:
        input_process_mouse_move(event.motion.x, event.motion.y);
        break;
      case SDL_EVENT_MOUSE_WHEEL:
        input_process_mouse_wheel(event.wheel.y);
        break;
      case SDL_EVENT_WINDOW_RESIZED:
        break;
      case SDL_EVENT_WINDOW_MINIMIZED:
        break;
      case SDL_EVENT_WINDOW_MAXIMIZED:
        break;
      case SDL_EVENT_WINDOW_MOVED:
        break;
      case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
        quit_flagged = TRUE;
        break;
      default:
        continue;
    }
  }
  return !quit_flagged;
}

void* platform_allocate(u64 size, b8 alligned) {
  return malloc(size);
}

void platform_free(void* block, b8 alligned) {
  free(block);
}

void* platform_zero_memory(void* block, u64 size) {
  return memset(block, 0, size);
}

void* platform_copy_memory(void* dest, const void* source, u64 size) {
  return memcpy(dest, source, size);
}

void* platform_set_memory(void* dest, s32 value, u64 size) {
  return memset(dest, value, size);
}

f64 platfor_get_absolute_time() {
  return (f64)SDL_GetPerformanceCounter() / (f64)SDL_GetPerformanceFrequency();
}

void platform_sleep(u64 ms) {
  SDL_Delay((u32)ms);
}

#endif
