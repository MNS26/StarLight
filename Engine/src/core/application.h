#pragma once

#include "defines.h"

struct game;

//Application config
typedef struct application_config {
  s16 start_pos_x;
  s16 start_pos_y;
  s16 start_width;
  s16 start_height;
  char* name;
} application_config;

SLAPI b8 application_create(struct game* game_instance);

SLAPI b8 application_run();

void application_get_framebuffer_size(u32* width, u32* height);