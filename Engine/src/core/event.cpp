// even though we are using sdl's event system we do want to track how much each event typse is being used

#include "event.h"
#include "core/logger.h"
b8 initialized = FALSE;
event_listener_list listeners[SYSTEM_EVENT_MAX];

b8 event_initialize()
{
  initialized = TRUE;
  return TRUE;
}

void event_shutdown() {
  for (u32 i = 0; i < SYSTEM_EVENT_MAX; i++) {
    listeners[i].clear();
  }
  initialized = FALSE;
}

SLAPI b8 register_listener(u32 Event, event_callback callback) {
  if (!initialized) return FALSE;
  if (Event >= SYSTEM_EVENT_MAX) return FALSE;
  listeners[Event].push_back(callback);
  return TRUE;
}

SLAPI b8 unregister_listener(u32 Event, event_callback callback) {
  if (!initialized) return FALSE;

  if (Event >= SYSTEM_EVENT_MAX) {
    return FALSE;
  }
  auto& list = listeners[Event];
  auto it = std::remove(list.begin(), list.end(), callback);
  if (it != list.end()) {
    list.erase(it, list.end());
    return TRUE;
  }
  return FALSE;
}

SLAPI b8 emit_event(u32 Event, event_context* data) {
  if (!initialized) return FALSE;
  if (Event >= SYSTEM_EVENT_MAX) {
      return FALSE;
  }
//  SDL_zero(event);
//  event.user.code = Event;
//  event.user.data1 = &data;
  b8 consumed = FALSE;
  for (auto& callback : listeners[Event]) {
    if (callback(data)) {
      //TRUE if consumed
      consumed = TRUE;
      break;
    }
  }
  return consumed;
}

#include "SDL3/SDL_init.h"
#include "SDL3/SDL_events.h"
//u32 base_event_val = SDL_RegisterEvents(SYSTEM_EVENT_MAX);
b8 sdl_initialized = FALSE;
sdl_event_listener_list sdl_listeners[SYSTEM_EVENT_MAX];

SLAPI b8 sdl_event_initialize()
{
  if(!SDL_InitSubSystem(SDL_INIT_EVENTS)) return FALSE;
  sdl_initialized = TRUE;
  return TRUE;
}

SLAPI b8 sdl_event_register(u32 event, sdl_event_callback callback) {
  if (!sdl_initialized) return FALSE;
  if(!callback) return FALSE;
  if (event >= SYSTEM_EVENT_MAX) return FALSE;
  sdl_listeners[event].push_back(callback);
  return TRUE;
}

SLAPI b8 sdl_event_push(u32 type, event_context* data1, void* data2) {
  if (!sdl_initialized) return FALSE;
  if (type >= SYSTEM_EVENT_MAX) return FALSE;
  SDL_Event event;
  event.type = type+SDL_EVENT_USER;
  event.user.data1 = data1;
  event.user.data2 = data2; // by default null if its not used
  return SDL_PushEvent(&event);
}

SLAPI void sdl_custom_event_process(SDL_Event *event) {
  if (!sdl_initialized) return;
//  SDL_Event event;
//  while (SDL_PollEvent(event)) {
    u32 type = event->type;
    if (type >= SDL_EVENT_USER && type < SDL_EVENT_USER + SYSTEM_EVENT_MAX) {
      s32 index = type - SDL_EVENT_USER;
      for (auto& callback : sdl_listeners[index]) {
        if (callback(event->user.data1)) {
          //TRUE if consumed
          break;
        }
      }
    }
//  }
}