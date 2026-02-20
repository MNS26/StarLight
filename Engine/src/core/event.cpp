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

    if (Event >= SYSTEM_EVENT_MAX) {
    return FALSE;
  }
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
  for (auto& callback : listeners[Event]) {
    if (callback(data)) {
      //TRUE if consumed
      break;
    }
  }
  return TRUE;
}
