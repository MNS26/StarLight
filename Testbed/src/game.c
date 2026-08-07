#include "game.h"

#include <core/logger.h>

b8 game_initialize(game* game_inst) {
  (void) game_inst;
  SLDEBUG("game_initialize() called");

  SLFATAL("TEST %f", 0.1);
  SLERROR("TEST %f", 0.12);
  SLWARN("TEST %f", 0.123);
  SLINFO("TEST %f", 0.1234);
  SLDEBUG("TEST %f", 0.12345);
  SLTRACE("TEST %f", 0.123456);

  return true;
}

b8 game_update(game* game_inst, f32 delta_time) {
  (void) game_inst;
  (void) delta_time;
  return true;
}

b8 game_render(game* game_inst, f32 delta_time) {
  (void) game_inst;
  (void) delta_time;
  return true;
}

void game_on_resize(game* game_inst, u32 width, u32 height) {
  (void) game_inst;
  (void) width;
  (void) height;
    
}
