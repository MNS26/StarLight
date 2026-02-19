#pragma once

#include "defines.h"
#include "core/application.h"
#include "core/logger.h"
#include "core/memory.h"
#include "gameTypes.h"

//User function
extern b8 createGame(game* outGame);

/**
 * Main entry point
 */
int main(void)  {

  initialize_logger();

  memoryInitialize();
  
  // Get game instance from application
  game gameInstance;

  if (!createGame(&gameInstance)) {
    CRITICAL("Could not create game!");
    return -1;
  }

  // Check if pointers arent null
  if (!gameInstance.initialize || !gameInstance.update || !gameInstance.render || !gameInstance.onResize || !gameInstance.state ) {
    CRITICAL("The game's function pointers must be assigned!");
    return -2;
  }

  // Init
  if (!applicationCreate(&gameInstance)) {
    CRITICAL("Application failed to create!");
    return 1;
  }

  // start game loop
  if(!applicationRun()) {
    CRITICAL("Application did not shutdown correctly");
    return 2;
  }

  memoryShutdown();
  return 0;
}