#pragma once

#include "defines.h"
#include <gameTypes.h>

typedef struct gameState {
  f64 deltaTime;
} gameState;

b8 gameInitialize(game* gameInstance);
b8 gameUpdate(game* gameInstance, f64 deltaTime);
b8 gameRender(game* gameInstance, f64 deltaTime);
void gameOnResize(game* gameInstance, u32 width, u32 height); 