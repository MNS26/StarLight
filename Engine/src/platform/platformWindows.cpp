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

typedef struct internalState {
  SDL_Window* Window;
} internalState;


//STATIC
static f64 clockFrequency;
static LARGE_INTEGER startTime;

b8 platformStartup( platformState *platformState, std::string *applicationName, s32 x/*unused*/, s32 y/*unused*/, s32 width, s32 height) {
  (void)x;
  (void)y;

  platformState->internalState = malloc(sizeof(internalState));
  memset(platformState->internalState, 0, sizeof(internalState));
  internalState* state = (internalState*)platformState->internalState;

  if(!SDL_InitSubSystem(SDL_INIT_VIDEO)) {
    CRITICAL("Failed to initialize sdl3: {}",SDL_GetError());
    return FALSE;
  }

  state->Window = SDL_CreateWindow(applicationName->c_str(), width, height, SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE );

  if(!state->Window) {
    CRITICAL("Failed to create window: {}",SDL_GetError());
    return FALSE;
  }

  //clock setup
  LARGE_INTEGER frequency;
  QueryPerformanceFrequency(&frequency);
  clockFrequency = 1.0/(f64)frequency.QuadPart;
  QueryPerformanceCounter(&startTime);
  return TRUE;
}


void platformShutdown(platformState* platformState) {
  (void) platformState;
}

b8 platformHandleEvents(platformState* platformState, SDL_Event* event) {
  internalState* state = (internalState*)platformState->internalState;
  switch (event->type) {
//      case SDL_DISPLAYEVENT:
//        SDL_UpdateWindowSurface(_window);
//        return 0;
    case SDL_EVENT_TERMINATING:
    case SDL_EVENT_QUIT:
      SDL_QuitSubSystem(SDL_INIT_VIDEO);
      SDL_DestroyWindow(state->Window);
      platformState->running = false;
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

void* platformAllocate(u64 size, b8 aligned) {
  return malloc(size);
}
void platformFree(void* block, b8 aligned) {
  free(block);
}
void* platformZeroMemory(void* block, u64 size) {
  return memset(block, 0, size);
}
void* platformCopyMemory(void* dst, const void* src, u64 size) {
  return memcpy(dst, src, size);
}
void* platformSetMemory(void* dst, s32 val, u64 size) {
  return memset(dst, val, size);
}


// FIXME: I HATE THIS BUT I DONT KNOW ANOTHER SANE WAY
//void platformConsoleWrite(u8 level, const char* message, ...) {
//  char out[UINT64_MAX];
//  memset(out, 0, sizeof(out);
//  __builtin_va_list arg_ptr;
//  va_start(arg_ptr,message);
//  vsnprintf(out,sizeof(out)/sizeof(out[0]), message, arg_ptr);
//  va_end(args_ptr);
//  logger.log_output(level, std::string(out));
//}
//void platformConsoleWriteError(const char* message, ...) {
//  char out[UINT64_MAX];
//  memset(out, 0, sizeof(out);
//  __builtin_va_list arg_ptr;
//  va_start(arg_ptr,message);
//  vsnprintf(out,sizeof(out)/sizeof(out[0]), message, arg_ptr);
//  va_end(args_ptr);
//  logger.log_output(4, std::string(out));
//}


f64 platformGetAbsoluteTime() {
  LARGE_INTEGER nowTime;
  QueryPerformanceCounter(&nowTime);
  return (f64)nowTime.QuadPart * clockFrequency;
}

void platormSleepMs(u64 miliseconds) {
  usleep(1000*miliseconds);
}
void platormSleepUs(u64 microseconds) {
  usleep(microseconds);
}
#endif