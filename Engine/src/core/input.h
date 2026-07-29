#pragma once

#include "defines.h"
#include "SDL3/SDL_scancode.h"

typedef enum mouse_buttons {
  BUTTON_LEFT = 1,
  BUTTON_MIDDLE = 2,
  BUTTON_RIGHT = 3,
  BUTTON_X1 = 4,
  BUTTON_X2 = 5,
  BUTTON_MAX_BUTTONS
} mouse_buttons;

void input_initialize();
void input_shutdown();
void input_update(f64 delta_time);

// Keyboard inputs
SLAPI b8 input_is_key_down(SDL_Scancode key);
SLAPI b8 input_is_key_up(SDL_Scancode key);
SLAPI b8 input_was_key_down(SDL_Scancode key);
SLAPI b8 input_was_key_up(SDL_Scancode key);

void input_process_key(SDL_Scancode key, b8 pressed);

// Mouse input
SLAPI b8 input_is_mouse_button_down(mouse_buttons button);
SLAPI b8 input_is_mouse_button_up(mouse_buttons button);
SLAPI b8 input_was_mouse_button_down(mouse_buttons button);
SLAPI b8 input_was_mouse_button_up(mouse_buttons button);
SLAPI void input_get_mouse_position(s32* x, s32* y);
SLAPI void input_get_previous_mouse_position(s32* x, s32* y);

void input_process_mouse_button(mouse_buttons button, b8 pressed);
void input_process_mouse_move(s16 x, s16 y);
void input_process_mouse_wheel(s8 z_delta);