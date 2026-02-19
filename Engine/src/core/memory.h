#pragma once

#include "defines.h"
#include "includes.h"
#include "logger.h"


typedef enum memoryTag {
  MEMORY_TAG_UNKNOWN,
  MEMORY_TAG_ARRAY,
  MEMORY_TAG_DARRAY,
  MEMORY_TAG_DICT,
  MEMORY_TAG_RING_QUEUE,
  MEMORY_TAG_BST,
  MEMORY_TAG_STRING,
  MEMORY_TAG_APPLICATION,
  MEMORY_TAG_JOB,
  MEMORY_TAG_TEXTURE,
  MEMORY_TAG_MATERIAL_INSTANCE,
  MEMORY_TAG_RENDERER,
//  MEMORY_TAG_ENGINE,
  MEMORY_TAG_GAME,
  MEMORY_TAG_TRANSFORM,
  MEMORY_TAG_ENTITY,
  MEMORY_TAG_ENTITY_NODE,
  MEMORY_TAG_SCENE,

  MEMORY_TAG_MAX,
} memoryTag;

void memoryInitialize();
void memoryShutdown();

SLAPI void* SLallocate(u64 size, memoryTag tag);
SLAPI void* SLfree(void* block, u64 size, memoryTag tag);
SLAPI void* SLzeroMemory(void* block, u64 size);
SLAPI void* SLcopyMemory(void* dest, const void* source, u64 size);
SLAPI void* SLsetMemory(void* dest, s32 value, u64 size);
  
SLAPI std::string getMemoryUsageStr();

