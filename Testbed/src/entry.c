#include "game.h"
#include <entry.h>
#include <core/slmemory.h>

// define the function to create game 
b8 create_game(game* game)  {
  game->config.name = "StarLight engine testbed";
  game->config.start_width = 800;
  game->config.start_height = 480;
  game->config.start_pos_x = 0;
  game->config.start_pos_y = 0;

  game->update = game_update;
  game->render = game_render;
  game->initialize = game_initialize;
  game->on_resize = game_on_resize;

  // Create game state
  game->state = slallocate(sizeof(game_state), MEMORY_TAG_GAME);

  return TRUE;
}