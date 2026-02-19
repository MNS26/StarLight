#include "game.h"
#include <entry.h>

#include <core/memory.h>
//extern Memory memory;
b8 createGame(game* game) {  
  game->config.startPosX = -1;
  game->config.startPosY = -1;
  game->config.startWidth = 1000;
  game->config.startHeight = 625;
  game->config.name = "StarLight Testbed";
  game->initialize = gameInitialize;
  game->update = gameUpdate;
  game->render = gameRender;
  game->onResize = gameOnResize;
  game->state = SLallocate(sizeof(gameState), MEMORY_TAG_GAME);
  return TRUE;
}