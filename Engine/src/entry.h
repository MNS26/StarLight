#pragma once

#include "defines.h"
#include "core/application.h"
#include "core/logger.h"
#include "core/memory.h"
#include "gameTypes.h"

Logger logger;
Memory memory;

//User function
extern b8 createGame(game* outGame);

/**
 * Main entry point
 */
int main(void)  {

  logger.initialize_logger();

  memory.initialize(&logger);
  
  
  // Get game instance from application
  game gameInstance;
  gameInstance.logger = logger;

  if (!createGame(&gameInstance)) {
    logger.CRITICAL("Could not create game!");
    return -1;
  }

  // Check if pointers arent null
  if (!gameInstance.initialize || !gameInstance.update || !gameInstance.render || !gameInstance.onResize || !gameInstance.state ) {
    logger.CRITICAL("The game's function pointers must be assigned!");
    return -2;
  }

  // Init
  if (!applicationCreate(&gameInstance)) {
    logger.CRITICAL("Application failed to create!");
    return 1;
  }

  // start game loop
  if(!applicationRun()) {
    logger.CRITICAL("Application did not shutdown correctly");
    return 2;
  }

  memory.shutdown();
  return 0;
}