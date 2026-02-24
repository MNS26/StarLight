#pragma once

#include "defines.h"
#include "includes.h"
#include "memory.h"
#include "types/d_array.h"

// List of all system events
typedef enum system_event_code {
  SYSTEM_EVENT_QUIT  = 0,
  SYSTEM_EVENT_RESIZE,
  SYSTEM_EVENT_TEST,

  SYSTEM_EVENT_MAX
} system_event_code;

#define MAX_EVENT_SIZE 3
typedef struct event_context {
  // 196 bytes
  union {
    s64 _s64[MAX_EVENT_SIZE];
    u64 _u64[MAX_EVENT_SIZE];
    f64 _f64[MAX_EVENT_SIZE];
    s32 _s32[MAX_EVENT_SIZE*2];
    u32 _u32[MAX_EVENT_SIZE*2];
    f32 _f32[MAX_EVENT_SIZE*2];
    s16 _s16[MAX_EVENT_SIZE*4];
    u16 _u16[MAX_EVENT_SIZE*4];
    s8 _s8[MAX_EVENT_SIZE*8];
    u8 _u8[MAX_EVENT_SIZE*8];
    char c[MAX_EVENT_SIZE*8];
  } data;
} event_context;

// True if handled
typedef b8 (*event_callback)(void* func);

// creating custom std::vector
using event_listener_list = DArray<event_callback>;

b8 event_initialize();
void event_shutdown();


/// @brief Register for events from event system
/// @param Event Event type to listen for
/// @param listener pointer to instance (Can be set to 0/NULL)
/// @param callback callback function to register
/// @return TRUE if successfully registers, otherwise FALSE
SLAPI b8 register_listener(u32 Event, event_callback callback);

/// @brief Unregister from event system
/// @param Event Event type to stop listen for
/// @param callback callback function to ungerister
/// @return TRUE if successfully registers, otherwise FALSE
SLAPI b8 unregister_listener(u32 Event, event_callback callback);


/// @brief Fires event to listeners of the givven EventCode
/// @param Event Event to fire
/// @param data The event data
/// @return TRUE if handled, otherwise FALSE
SLAPI b8 emit_event(u32 Event, event_context* data);





#include "SDL3/SDL_events.h"

typedef b8 (*sdl_event_callback)(void*);
using sdl_event_listener_list = DArray<sdl_event_callback>;

SLAPI b8 sdl_event_initialize();
SLAPI b8 sdl_event_register(u32 event, sdl_event_callback callback);
SLAPI b8 sdl_event_push(u32 type, event_context* data1, void* data2 = NULL);
SLAPI void sdl_custom_event_process(SDL_Event* event);