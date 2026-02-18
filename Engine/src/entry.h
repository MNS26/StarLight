#pragma once

#include "defines.h"
#include "core/application.h"
#include "core/logger.h"
#include "gameTypes.h"


//User function
extern b8 createGame(game* outGame);

/**
 * Main entry point
 */
int main(void)  {

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
    INFO("Application failed to create!");
    return 1;
  }

  // start game loop
  if(!applicationRun()) {
    INFO("Application did not shutdown correctly");
    return 2;
  }

  return 0;
}