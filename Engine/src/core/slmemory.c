#include "slmemory.h"

#include "core/logger.h"
#include "core/slstring.h"
#include "platform/platform.h"

#include <stdio.h>

struct memory_stats {
  u64 total_allocated;
  u64 tagged_allocations[MEMORY_TAG_MAX_TAGS];
};

static const char* memory_tag_strings[MEMORY_TAG_MAX_TAGS] = {
  "UNKNOWN      ",
  "ARRAY        ",
  "DARRAY       ",
  "DICT         ",
  "RING_QUEUE   ",
  "BST          ",
  "STRING       ",
  "APPLICATION  ",
  "JOB          ",
  "TEXTURE      ",
  "MATERIAL_INST",
  "RENDERER     ",
  "GAME         ",
  "ENGINE       ",
  "TRANSFORM    ",
  "ENTITY       ",
  "ENTITY_NODE  ",
  "SCENE        ",
};

static struct memory_stats stats;

void initialize_memory() {
  platform_zero_memory(&stats, sizeof(stats));
}
void shutdown_memory() {

}

void* slallocate(u64 size, memory_tag tag) {
  if (tag == MEMORY_TAG_UNKNOWN) {
    SLWARN("slallocate() called using MEMORY_TAG_UNKNOWN. Re-class this allocation");
  };

  stats.total_allocated += size;
  stats.tagged_allocations[tag] += size;

  // Allocate memory block
  void* block = platform_allocate(size, FALSE);
  platform_zero_memory(block, size);
  return block;
}

void slfree(void* block, u64 size, memory_tag tag) {
  if (tag == MEMORY_TAG_UNKNOWN) {
    SLWARN("slfree() called using MEMORY_TAG_UNKNOWN. Re-class this allocation");
  };

  stats.total_allocated -= size;
  stats.tagged_allocations[tag] -= size;

  platform_free(block, FALSE);
}

void* slzero_memory(void* block, u64 size) {
  return platform_zero_memory(block, size);
}

void* slcopy_memory(void* dest, const void* source, u64 size) {
  return platform_copy_memory(dest, source, size);
}

void* slset_memory(void* dest, s32 value, u64 size) {
  return platform_set_memory(dest, value, size);
}

char* get_memory_usage_str() {
  const u64 gib = 1000 * 1000 * 1000;
  const u64 mib = 1000 * 1000;
  const u64 kib = 1000;

  char buff[8000] = "System memory use (tagged):\n";
  u64 offset = string_length(buff);
  for (u32 tag = 0; tag < MEMORY_TAG_MAX_TAGS; tag++) {
    char unit[4] = " iB";
    float amount = 1.0f;
     if (stats.tagged_allocations[tag] >= gib) {
      unit[0] = 'G';
      amount = stats.tagged_allocations[tag] / (float)gib;
    } else if (stats.tagged_allocations[tag] >= mib) {
      unit[0] = 'M';
      amount = stats.tagged_allocations[tag] / (float)mib;
    } else if (stats.tagged_allocations[tag] >= kib) {
      unit[0] = 'K';
      amount = stats.tagged_allocations[tag] / (float)kib;
    } else {
      unit[0] = 'B';
      unit[1] = '\0';
      amount = (float)stats.tagged_allocations[tag];
    } 
    offset += snprintf(buff + offset, 8000, "  %s: %.2f%s\n", memory_tag_strings[tag], amount, unit);
  }
  char* out = string_duplicate(buff);
  return out;
}