#pragma once
#include "defines.h"
#include "core/application.h"

typedef struct game {

  // Game config
  application_config config;
  
  // Pointer to game initialization function
  b8 (*initialize)(struct game* game_instance);
  // Pointer to gane update function
  b8 (*update)(struct game* game_instance, f64 delta_time);
  // Pointer to game render function
  b8 (*render)(struct game* game_instance, f64 delta_time);
  // Pointer to game resize function
  void (*onResize)(struct game* game_instance, u32 width, u32 height);
  
  // General game state data
  void* state;
} game;