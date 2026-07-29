#pragma once

#include "defines.h"
#define SIZE 2
typedef struct event_context {
  // 128 Bytes
  union {
    s64 s64[SIZE*1];
    u64 u64[SIZE*1];
    f64 f64[SIZE*1];

    s32 s32[SIZE*2];
    u32 u32[SIZE*2];
    f32 f32[SIZE*2];

    s16 s16[SIZE*4];
    u16 u16[SIZE*4];

    s8 s8[SIZE*8];
    u8 u8[SIZE*8];

    char c[SIZE*8];
  } data;
  
} event_context;

// Return true if handled
typedef b8 (*PFN_on_event)(u32 code, void* sender, void* listener_instance, event_context data);

b8 event_initialize();
void event_shutdown();


/// @brief Register events to listen for
/// @param code The event to listen for
/// @param listener pointer to the listener instance. Can be 0 or NULL
/// @param on_event The callback function pointer to run when the event is triggered
/// @return TRUE if the event sucessfully registered, otherwise FALSE
SLAPI b8 event_register(u32 code, void* listener, PFN_on_event on_event);

/// @brief Unregister events to listen for
/// @param code the listener to remove
/// @param listener pointer to remove
/// @param on_event the event callback
/// @return TRUE if it sucessfully removed it, otherwise FALSE
SLAPI b8 event_unregister(u32 code, void* listener, PFN_on_event on_event);

/// @brief Fire events to listeners for the even code.
/// @param code The event to fire
/// @param listener pointer to the sender. Can be 0 or NULL
/// @param context The event data
/// @return TRUE if handled, otherwise FALSE
SLAPI b8 event_fire(u32 code, void* sender, event_context context);


typedef enum system_event_code {
  EVENT_CODE_APPLICATION_QUIT = 1,
  EVENT_CODE_KEY_PRESSED,
  EVENT_CODE_KEY_RELEASED,
  EVENT_CODE_MOUSE_BUTTON_PRESSED,
  EVENT_CODE_MOUSE_BUTTON_RELEASED,
  EVENT_CODE_MOUSE_MOVED,
  EVENT_CODE_MOUSE_WHEEL,
  EVENT_CODE_WINDOW_MOVED,
  EVENT_CODE_WINDOW_RESIZED,
  EVENT_CODE_MAX_EVENT = 255
} system_event_code;
