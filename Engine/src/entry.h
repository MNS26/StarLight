#pragma once

#include "core/application.h"
#include "core/slmemory.h"
#include "core/logger.h"
#include "game_types.h"

// External function managed by user code
extern b8 create_game(game* game);

// Main enry point of application
int main(void) {
  initialize_memory();
  initialize_logging();

  game game_instance;
  if (!create_game(&game_instance)) {
    SLFATAL("Could not create game!");
    return -1;
  }

  // Enusre they exist
  if (!game_instance.update || !game_instance.state || !game_instance.render || !game_instance.on_resize || !game_instance.initialize) {
    SLFATAL("The game's function pointers must be assigned!");
    return -2;
  }


  //init
  if (!application_create(&game_instance)) {
    SLINFO("Application failed to create!");
    return 1;
  }

  // begin loop
  if (!application_run()) {
    SLINFO("Application did not shut down gracefully");
    return 2;
  }
  shutdown_logging();
  shutdown_memory();
  return 0;
}