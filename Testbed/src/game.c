#include "game.h"

#include <core/logger.h>

b8 game_initialize(game* game_inst) {
  (void) game_inst;
  SLDEBUG("game_initialize() called");
  return TRUE;
}

b8 game_update(game* game_inst, f32 delta_time) {
  (void) game_inst;
  (void) delta_time;
  return TRUE;
}

b8 game_render(game* game_inst, f32 delta_time) {
  (void) game_inst;
  (void) delta_time;
  return TRUE;
}

void game_on_resize(game* game_inst, u32 width, u32 height) {
  (void) game_inst;
  (void) width;
  (void) height;
}
