#include "core/input.h"
#include "core/event.h"
#include "core/slmemory.h"
#include "core/logger.h"

#include <SDL3/SDL_keyboard.h>

typedef struct keyboard_state {
  b8 keys[SDL_SCANCODE_COUNT];
} keyboard_state;

typedef struct mouse_state {
  s16 x;
  s16 y;
  u8 buttons[BUTTON_MAX_BUTTONS];
} mouse_state;

typedef struct input_state {
  keyboard_state keyboard_current;
  keyboard_state keyboard_previous;
  mouse_state mouse_current;
  mouse_state mouse_previous;
} input_state;

// Internal input state
static b8 initialized = FALSE;
static input_state state = {};

void input_initialize() {
  slzero_memory(&state, sizeof(input_state));
  initialized = TRUE;
  SLINFO("Input subsystem initialized");
}

void input_shutdown() {
  initialized = FALSE;
}

void input_update(f64 delta_time) {
  if (!initialized) {
    SLERROR("Input subsystem not yet initialised!");
    return;
  }
  
  // Copy current states to previous states
  slcopy_memory(&state.keyboard_previous, &state.keyboard_current, sizeof(keyboard_state));
  slcopy_memory(&state.mouse_previous, &state.mouse_current, sizeof(mouse_state));
}

void input_process_key(SDL_Scancode key, b8 pressed) {
  // Only handle if state actually changed
  if (state.keyboard_current.keys[key] != pressed) {
    // Update internal state
    state.keyboard_current.keys[key] = pressed;
    const char* key_name = SDL_GetScancodeName(key);
    //SLDEBUG("key %s: %s ",pressed ?"pressed":"released", key_name);
    // fire event
    event_context context;
    context.data.u16[0]=key;
    event_fire(pressed ? EVENT_CODE_KEY_PRESSED : EVENT_CODE_KEY_RELEASED, 0, context);
  }
}

void input_process_mouse_button(mouse_buttons button, b8 pressed) {
  //If state changed
  if (state.mouse_current.buttons[button] != pressed) {
    state.mouse_current.buttons[button] = pressed;

    // Fire event
    event_context context;
    context.data.u16[0] = button;
    event_fire(pressed ? EVENT_CODE_MOUSE_BUTTON_PRESSED : EVENT_CODE_MOUSE_BUTTON_RELEASED, 0, context);
  }
}

void input_process_mouse_move(s16 x, s16 y) {
  //If state changed
  if (state.mouse_current.x != x || state.mouse_current.y != y) {
    SLDEBUG("Mouse Pos: %i, %i",x,y)
    //Update state
    state.mouse_current.x = x;
    state.mouse_current.y = y;

    //Fire event
    event_context context;
    context.data.u16[0]=x;
    context.data.u16[1]=y;
    event_fire(EVENT_CODE_MOUSE_MOVED, 0, context);
  }
}

void input_process_mouse_wheel(s8 z_delta) {
  event_context context;
  context.data.u8[0] = z_delta;
  event_fire(EVENT_CODE_MOUSE_WHEEL, 0, context);
}

b8 input_is_key_down(SDL_Scancode key) {
  if (!initialized)
    return FALSE;
  return state.keyboard_current.keys[key] == TRUE;
}

b8 input_is_key_up(SDL_Scancode key) {
  if (!initialized)
    return TRUE;
  return state.keyboard_current.keys[key] == FALSE;
}

b8 input_was_key_down(SDL_Scancode key) {
  if (!initialized)
    return FALSE;
  return state.keyboard_previous.keys[key] == TRUE;
}

b8 input_was_key_up(SDL_Scancode key) {
  if (!initialized)
    return TRUE;
  return state.keyboard_previous.keys[key] == FALSE;
}

b8 input_is_mouse_button_down(mouse_buttons button) {
  if (!initialized)
    return FALSE;
  return state.mouse_current.buttons[button] == TRUE;
}

b8 input_is_mouse_button_up(mouse_buttons button) {
  if (!initialized)
    return TRUE;
  return state.mouse_current.buttons[button] == FALSE;
}

b8 input_was_mouse_button_down(mouse_buttons button) {
  if (!initialized)
    return FALSE;
  return state.mouse_previous.buttons[button] == TRUE;
}

b8 input_was_mouse_button_up(mouse_buttons button) {
  if (!initialized)
    return TRUE;
  return state.mouse_previous.buttons[button] == FALSE;
}

void input_get_mouse_position(s32* x, s32* y) {
  if (!initialized) {
    *x = 0;
    *y = 0;
    return;
  }
  *x = state.mouse_current.x;
  *y = state.mouse_current.y;
}

void input_get_previous_mouse_position(s32* x, s32* y) {
  if (!initialized) {
    *x = 0;
    *y = 0;
    return;
  }
  *x = state.mouse_previous.x;
  *y = state.mouse_previous.y;
}