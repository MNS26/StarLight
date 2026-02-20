#include "memory.h"
#include "core/logger.h"
#include "platform/platform.h"



static std::string memory_tag_strigns[MEMORY_TAG_MAX] = {
  "UNKNOWN",
  "ARRAY",
  "DARRAY",
  "DICT",
  "RING_QUEUE",
  "BST",
  "STRING",
  "APPLICATION",
  "JOB",
  "TEXTURE",
  "MAT_INST",
  "RENDERER",
//    "ENGINE",
  "GAME",
  "TRANSFORM",
  "ENTITY",
  "ENTITY_NODE",
  "SCENE",
};

struct memoryStats {
  u64 totall_allocated;
  u64 tagged_allocations[MEMORY_TAG_MAX];
};
struct memoryStats stats;

void memory_initialize() {
  platform_zero_memory(&stats, sizeof(stats));
}
void memory_shutdown() {}

void *SLallocate(u64 size, memory_tag tag) {
  if (tag == MEMORY_TAG_UNKNOWN) {
    SLCRITICAL("Allocate called using MEMORY_TAG_UNKNOWN. Check Tag class!");
  }

  stats.totall_allocated += size;
  stats.tagged_allocations[tag] += size;

  // TODO: Inplement memory alignment
  void* block = platform_allocate(size, FALSE);
  platform_zero_memory(block, size);
  return block;

}
void* SLfree(void* block, u64 size, memory_tag tag) {
  if (tag == MEMORY_TAG_UNKNOWN) {
    SLWARNING("Free called using MEMORY_TAG_UNKNOWN. Check Tag class!");
  }
  stats.totall_allocated -= size;
  stats.tagged_allocations[tag] -= size;

  // TODO: Implement memory alignmet
  platform_free(block, FALSE);
  return block;
}
void* SLzeroMemory(void* block, u64 size) {
  return platform_zero_memory(block, size);
}
void* SLcopyMemory(void* dest, const void* source, u64 size) {
  return platform_copy_memory(dest, source, size);
}
void* SLsetMemory(void* dest, s32 value, u64 size){
  return platform_set_memory(dest, value, size);
}

SLAPI std::string get_memory_usage_str() {
  const u64 kib = 1024;
  const u64 mib = kib * 1024;
  const u64 gib = mib * 1024;
  const u64 tib = gib * 1024;

  std::string string;
  string.assign("System memory use (tagged): \n");

  for (u32 i = 0; i < MEMORY_TAG_MAX; i++) {
    if (stats.tagged_allocations[i] >= tib) {
      string.append(std::format(" {:11} {:02.3f}{:3}\n", memory_tag_strigns[i], stats.tagged_allocations[i]/(f32)tib, "TiB"));
    }
    else if (stats.tagged_allocations[i] >= gib) {
      string.append(std::format(" {:11} {:02.3f}{:3}\n", memory_tag_strigns[i], stats.tagged_allocations[i]/(f32)gib, "GiB"));
    }
    else if (stats.tagged_allocations[i] >= mib) {
      string.append(std::format(" {:11} {:02.3f}{:3}\n", memory_tag_strigns[i], stats.tagged_allocations[i]/(f32)mib, "MiB"));
    }
    else if (stats.tagged_allocations[i] >= kib) {
      string.append(std::format(" {:11} {:02.3f}{:3}\n", memory_tag_strigns[i], stats.tagged_allocations[i]/(f32)kib, "KiB"));
    }
    else {
      string.append(std::format(" {:11} {:02.3f}{:3}\n", memory_tag_strigns[i], stats.tagged_allocations[i]/(f32)1, "B"));
    }
  }
  return string;
}
