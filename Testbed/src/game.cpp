#include "game.h"
#include <core/logger.h>

b8 gameInitialize(game* gameInstance) {
  (void)gameInstance;
  return TRUE;
}
b8 gameUpdate(game* gameInstance, f64 deltaTime) {
  (void)gameInstance;
  (void)deltaTime;
  return TRUE;
}
b8 gameRender(game* gameInstance, f64 deltaTime) {
  (void)gameInstance;
  (void)deltaTime;
  return TRUE;
}
void gameOnResize(game* gameInstance, u32 width, u32 height) {
  (void)gameInstance;
  (void)width;
  (void)height;

}