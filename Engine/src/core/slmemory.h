#pragma once

#include "defines.h"

typedef enum memory_tag {
  MEMORY_TAG_UNKNOWN,
  MEMORY_TAG_ARRAY,
  MEMORY_TAG_DARRAY,
  MEMORY_TAG_DICT,
  MEMORY_TAG_RING_QUEU,
  MEMORY_TAG_BST,
  MEMORY_TAG_STRING,
  MEMORY_TAG_APPLICATION,
  MEMORY_TAG_JOB,
  MEMORY_TAG_TEXTURE,
  MEMORY_TAG_MATERIAL_INSTANCE,
  MEMORY_TAG_RENDERER,
  MEMORY_TAG_GAME,
  MEMORY_TAG_ENGINE,
  MEMORY_TAG_TRANSFORM,
  MEMORY_TAG_ENTITY,
  MEMORY_TAG_ENTITY_NODE,
  MEMORY_TAG_SCENE,

  MEMORY_TAG_MAX_TAGS,
} memory_tag;

SLAPI void initialize_memory();
SLAPI void shutdown_memory();

SLAPI void* slallocate(u64 size, memory_tag tag);
SLAPI void slfree(void* block, u64 size, memory_tag tag);
SLAPI void* slzero_memory(void* block, u64 size);
SLAPI void* slcopy_memory(void* dest, const void* source, u64 size);
SLAPI void* slset_memory(void* dest, s32 value, u64 size);

SLAPI char* get_memory_usage_str();