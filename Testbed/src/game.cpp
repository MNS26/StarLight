#include "game.h"
#include <core/logger.h>

b8 gameInitialize(game* gameInstance) {
  (void)gameInstance;
  gameInstance->logger.DEBUG("gameInistialize called");
  return TRUE;
}
b8 gameUpdate(game* gameInstance, f64 deltaTime) {
  (void)gameInstance;
  (void)deltaTime;
  gameInstance->logger.DEBUG("gameUpdate called");
  return TRUE;
}
b8 gameRender(game* gameInstance, f64 deltaTime) {
  (void)gameInstance;
  (void)deltaTime;
  gameInstance->logger.DEBUG("gameRender called");
  return TRUE;
}
void gameOnResize(game* gameInstance, u32 width, u32 height) {
  (void)gameInstance;
  (void)width;
  (void)height;
  gameInstance->logger.DEBUG("gameOnResize called");

}