#pragma once
#include "defines.h"
#include "includes.h"

struct game;

//Application Config
typedef struct application_config {
  // Window possiont X (if applicable)
  s16 start_pos_x;
  // Window possiont Y (if applicable)
  s16 start_pos_y;
  // Window width (if applicable)
  s16 start_width;
  // Window height (if applicable)
  s16 start_height;
  // Window name (if applicable)
  std::string name;
} application_config;

SLAPI b8 application_create(struct game* game_instance);

SLAPI b8 application_run();

