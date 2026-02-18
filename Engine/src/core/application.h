#pragma once
#include "defines.h"
#include "includes.h"

struct game;

//Application Config
typedef struct applicationConfig {
  // Window possiont X (if applicable)
  s16 startPosX;
  // Window possiont Y (if applicable)
  s16 startPosY;
  // Window width (if applicable)
  s16 startWidth;
  // Window height (if applicable)
  s16 startHeight;
  // Window name (if applicable)
  std::string name;
} applicationConfig;

SLAPI b8 applicationCreate(struct game* gameInstance);

SLAPI b8 applicationRun();

