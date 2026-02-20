#include "game.h"
#include <core/logger.h>

b8 game_initialize(game* game_instance) {
  (void)game_instance;
  SLDEBUG("gameInistialize called");
  return TRUE;
}
b8 game_update(game* game_instance, f64 delta_time) {
  (void)game_instance;
  (void)delta_time;
  SLDEBUG("game_update called");
  return TRUE;
}
b8 game_render(game* game_instance, f64 delta_time) {
  (void)game_instance;
  (void)delta_time;
  SLDEBUG("game_render called");
  return TRUE;
}
void game_on_resize(game* game_instance, u32 width, u32 height) {
  (void)game_instance;
  (void)width;
  (void)height;
  SLDEBUG("game_on_resize called");

}