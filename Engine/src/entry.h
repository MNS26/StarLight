#pragma once

#include "defines.h"
#include "core/application.h"
#include "core/logger.h"
#include "core/memory.h"
#include "game_types.h"
#include "SDL3/SDL_main.h"

//User function
extern b8 create_game(game* outGame);

/**
 * Main entry point
 */
int main(void)  {

  logger_initialize();

  memory_initialize();
  
  // Get game instance from application
  game game_instance;

  if (!create_game(&game_instance)) {
    SLCRITICAL("Could not create game!");
    return -1;
  }

  // Check if pointers arent null
  if (!game_instance.initialize || !game_instance.update || !game_instance.render || !game_instance.onResize || !game_instance.state ) {
    SLCRITICAL("The game's function pointers must be assigned!");
    return -2;
  }

  // Init
  if (!application_create(&game_instance)) {
    SLCRITICAL("Application failed to create!");
    return 1;
  }

  // start game loop
  if(!application_run()) {
    SLCRITICAL("Application did not shutdown correctly");
    return 2;
  }

  memory_shutdown();
  return 0;
}