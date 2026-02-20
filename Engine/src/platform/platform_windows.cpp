#include "defines.h"
// if we are on Windows
#ifdef PLATFORM_WINDOWS

#include "includes.h"
#include "platform.h"
#include "core/logger.h"
#include "core/asserts.h"
#include <sys/time.h>
#include <SDL3/SDL_vulkan.h>
#include <SDL3/SDL.h>
#include <vector>
#include <spdlog/spdlog.h>
#include <spdlog/logger.h>
#include "spdlog/sinks/stdout_color_sinks.h"

typedef struct internal_state {
  SDL_Window* Window;
} internal_state;


//STATIC
static f64 clock_frequency;
static LARGE_INTEGER start_time;

b8 platform_startup( platform_state *platform_state, std::string *application_name, s32 x/*unused*/, s32 y/*unused*/, s32 width, s32 height) {
  (void)x;
  (void)y;

  platform_state->internal_state = malloc(sizeof(internal_state));
  memset(platform_state->internal_state, 0, sizeof(internal_state));
  internal_state* state = (internal_state*)platform_state->internal_state;

  if(!SDL_InitSubSystem(SDL_INIT_VIDEO)) {
    CRITICAL("Failed to create window: {}",SDL_GetError());
    return FALSE;
  }

  state->Window = SDL_CreateWindow(application_name->c_str(), width, height, SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE );

  if(!state->Window) {
    CRITICAL("Failed to create window: {}",SDL_GetError());
    return FALSE;
  }

  //clock setup
  LARGE_INTEGER frequency;
  QueryPerformanceFrequency(&frequency);
  clock_frequency = 1.0/(f64)frequency.QuadPart;
  QueryPerformanceCounter(&start_time);
  return TRUE;
}


void platform_shutdown(platform_state* platform_state) {
  (void) platform_state;
}

b8 platform_handle_events(platform_state* platform_state, SDL_Event* event) {
  internal_state* state = (internal_state*)platform_state->internal_state;
  switch (event->type) {
//      case SDL_DISPLAYEVENT:
//        SDL_UpdateWindowSurface(_window);
//        return 0;
    case SDL_EVENT_TERMINATING:
    case SDL_EVENT_QUIT:
      SDL_QuitSubSystem(SDL_INIT_VIDEO);
      SDL_DestroyWindow(state->Window);
      platform_state->running = false;
      return FALSE; // always let it pass through to the rest

      //std::cout << "Event: " << SDL_WindowEventToString(event.window) << std::endl;
      //std::cout << "  \\- TimeStamp: " << (int)event.window.timestamp << std::endl;
      //std::cout << "  |- WindowID: " << (int)event.window.windowID << std::endl;
      //std::cout << "  |- Data1: " << (int)event.window.data1 << std::endl;
      //std::cout << "  |- Data2: " << (int)event.window.data2 << std::endl;

    case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
      SDL_Event quit;
      quit.type = SDL_EVENT_QUIT;
      SDL_PushEvent(&quit);
      return TRUE;

    case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
      SDL_SetWindowSize(state->Window, event->window.data1, event->window.data2);
      return TRUE;

    case SDL_EVENT_WINDOW_MINIMIZED:
        SDL_MinimizeWindow(state->Window);
      return TRUE;

    case SDL_EVENT_WINDOW_MAXIMIZED:
      SDL_MaximizeWindow(state->Window);
      return TRUE;

    case SDL_EVENT_WINDOW_RESTORED:
      SDL_RestoreWindow(state->Window);
      return TRUE;

    case SDL_EVENT_WINDOW_MOUSE_ENTER:
      return TRUE;

    case SDL_EVENT_WINDOW_MOUSE_LEAVE:
      return TRUE;

    case SDL_EVENT_KEY_DOWN:
      if (event->key.key == SDLK_Q) {
        SDL_Event quit;
        quit.type = SDL_EVENT_QUIT;
        SDL_PushEvent(&quit);
      }
      return TRUE;

    default:
      return TRUE;
  }

  return FALSE;
}

void* platform_allocate(u64 size, b8 aligned) {
  (void)aligned;
  return malloc(size);
}
void platform_free(void* block, b8 aligned) {
  (void)aligned;
  free(block);
}
void* platform_zero_memory(void* block, u64 size) {
  return memset(block, 0, size);
}
void* platform_copy_memory(void* dst, const void* src, u64 size) {
  return memcpy(dst, src, size);
}
void* platform_set_memory(void* dst, s32 val, u64 size) {
  return memset(dst, val, size);
}

f64 platform_get_absolute_time() {
  LARGE_INTEGER now_time;
  QueryPerformanceCounter(&now_time);
  return (f64)now_time.QuadPart * clock_frequency;
}

void platorm_sleep_ms(u64 miliseconds) {
  usleep(1000*miliseconds);
}
void platorm_sleep_us(u64 microseconds) {
  usleep(microseconds);
}
#endif