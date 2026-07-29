#pragma once

#include "core/application.h"

// represents basic game state
typedef struct game {
  application_config config;

  b8 (*initialize)(struct game* game_inst);

  b8 (*update)(struct game* game_inst, f32 delta_time);

  // Will be called with a consistent interval
  b8 (*fixed_update)(struct game* game_inst, f32 delta_time);

  b8 (*render)(struct game* game_isnt, f32 delta_time);

  void (*on_resize)(struct game* game_inst, u32 width, u32 height);

  void* state;
} game;