#pragma once
#include "defines.h"
#include "core/application.h"

typedef struct game {

  // Game config
  applicationConfig config;

  // Pointer to game initialization function
  b8 (*initialize)(struct game* gameInst);
  // Pointer to gane update function
  b8 (*update)(struct game* gameInst, f64 deltaTime);
  // Pointer to game render function
  b8 (*render)(struct game* gameInst, f64 deltaTime);
  // Pointer to game resize function
  void (*onResize)(struct game* gameInst, u32 width, u32 height);

  // General game state data
  void* state;
}game;