#include "core/event.h"
#include "core/slmemory.h"
#include "containers/darray.h"
#include "core/logger.h"

#include <SDL3/SDL_events.h>

typedef struct registered_event {
  void* listener;
  PFN_on_event callback;
} registered_event;

typedef struct event_code_entry {
  registered_event* events;
} event_code_entry;

#define MAX_MESSAGE_CODES SDL_EVENT_LAST - SDL_EVENT_USER

// State structure
typedef struct event_system_state {
  event_code_entry registered[MAX_MESSAGE_CODES];
  Uint32 sdl_event_type;
} event_system_state;

// Event system internal state
static b8 is_initialized = false;
static event_system_state state;

b8 event_initialize() {
  if (is_initialized == true)
    return false;

  slzero_memory(&state, sizeof(state));

  state.sdl_event_type = SDL_RegisterEvents(1);
  if (state.sdl_event_type == (Uint32)-1) {
    SLERROR("Failed to register SDL user event type");
    return false;
  }

  is_initialized = true;

  return true;
}

void event_shutdown() {
  for (u32 i = 0; i < MAX_MESSAGE_CODES; i++) {
    if (state.registered[i].events != 0) {
      darray_destroy(state.registered[i].events);
      state.registered[i].events = 0;
    }
  }
}

b8 event_register(u32 code, void* listener, PFN_on_event on_event) {
  if (is_initialized == false)
    return false;

  if (state.registered[code].events == 0)
    state.registered[code].events = darray_create(registered_event);

  u64 registered_count = darray_length(state.registered[code].events);
  for (u64 i = 0; i < registered_count; i++) {
    if (state.registered[code].events[i].listener == listener) {
      SLWARN("Can't register dubplicate listener!")
      return false;
    }
  }
  
  registered_event event;
  event.listener = listener;
  event.callback = on_event;
  darray_push(state.registered[code].events, event);

  return true;
}

b8 event_unregister(u32 code, void* listener, PFN_on_event on_event) {
  if (is_initialized == false)
    return false;

  if (state.registered[code].events == 0) {
    SLWARN("Unable to unregister listener! No listeners found, returning.")
    return false;
  }

  u64 registered_count = darray_length(state.registered[code].events);
  for (u64 i = 0; i < registered_count; i++) {
    registered_event e = state.registered[code].events[i];
    if (e.listener == listener && e.callback == on_event) {
      //SLDEBUG("Found listener, unregistering.")
      registered_event popped_event;
      darray_pop_at(state.registered[code].events, i, &popped_event);
      return true;
    }
  }
  SLDEBUG("No listener found for this code.");
  return false;
}

b8 event_fire(u32 code, void* sender, event_context context) {
  if (is_initialized == false)
    return false;

  SDL_Event event;
  slzero_memory(&event, sizeof(event));
  event.type = state.sdl_event_type;
  event.user.code = (Sint32)code;
  slcopy_memory(&event.user.data1, &context.data.u64[0], sizeof(void*));
  slcopy_memory(&event.user.data2, &context.data.u64[1], sizeof(void*));

  return SDL_PushEvent(&event);
}

void event_process_queue() {
  if (is_initialized == false)
    return;

  SDL_Event event;
  while (SDL_PeepEvents(&event, 1, SDL_GETEVENT, state.sdl_event_type, state.sdl_event_type) > 0) {
    u32 code = (u32)event.user.code;
    if (state.registered[code].events != 0) {
      event_context ctx;
      slcopy_memory(&ctx.data.u64[0], &event.user.data1, sizeof(void*));
      slcopy_memory(&ctx.data.u64[1], &event.user.data2, sizeof(void*));

      u64 registered_count = darray_length(state.registered[code].events);
      for (u64 i = 0; i < registered_count; i++) {
        registered_event e = state.registered[code].events[i];
        if (e.callback(code, NULL, e.listener, ctx))
          break;
      }
    }
  }
}