#include "game.h"
#include <entry.h>

#include <core/memory.h>
//extern Memory memory;
b8 create_game(game* game) {  
  game->config.start_pos_x = -1;
  game->config.start_pos_y = -1;
  game->config.start_width = 1000;
  game->config.start_height = 625;
  game->config.name = "StarLight Testbed";
  game->initialize = game_initialize;
  game->update = game_update;
  game->render = game_render;
  game->onResize = game_on_resize;
  game->state = SLallocate(sizeof(game_state), MEMORY_TAG_GAME);
  return TRUE;
}