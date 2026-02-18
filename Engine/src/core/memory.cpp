#include "memory.h"
#include "core/logger.h"
#include "platform/platform.h"



static const std::string memoryTagStrigns[Memory::MEMORY_TAG_MAX] = {
  "UNKNOWN    ",
  "ARRAY      ",
  "DARRAY     ",
  "DICT       ",
  "RING_QUEUE ",
  "BST        ",
  "STRING     ",
  "APPLICATION",
  "JOB        ",
  "TEXTURE    ",
  "MAT_INST   ",
  "RENDERER   ",
  "GAME       ",
  "TRANSFORM  ",
  "ENTITY     ",
  "ENTITY_NODE",
  "SCENE      ",

};




void Memory::initialize(Logger* logger) {
  this->logger = logger;
  platformZeroMemory(&stats, sizeof(stats));
}
void Memory::shutdown() {}

void *Memory::allocate(u64 size, memoryTag tag) {
  if (tag == Memory::MEMORY_TAG_UNKNOWN) {
    logger->CRITICAL("Allocate called using MEMORY_TAG_UNKNOWN. Check Tag class!");
  }

  stats.totallAllocated += size;
  stats.taggedAllocations[tag] += size;

  // TODO: Inplement memory alignment
  void* block = platformAllocate(size, FALSE);
  platformZeroMemory(block, size);
  return block;

}
void* Memory::free(void* block, u64 size, memoryTag tag) {
  if (tag == Memory::MEMORY_TAG_UNKNOWN) {
    logger->WARNING("Free called using MEMORY_TAG_UNKNOWN. Check Tag class!");
  }
  stats.totallAllocated -= size;
  stats.taggedAllocations[tag] -= size;

  // TODO: Implement memory alignmet
  platformFree(block, FALSE);
  return block;
}
void* Memory::zeroMemory(void* block, u64 size) {
  return platformZeroMemory(block, size);
}
void* Memory::copyMemory(void* dest, const void* source, u64 size) {
  return platformCopyMemory(dest, source, size);
}
void* Memory::setMemory(void* dest, s32 value, u64 size){
  return platformSetMemory(dest, value, size);
}

SLAPI std::string Memory::getMemoryUsageStr() {
  const u64 kib = 1024;
  const u64 mib = kib * 1024;
  const u64 gib = mib * 1024;
  const u64 tib = tib * 1024;

  std::string string;
  string.assign("System memory use (tagged): \n");

  for (u32 i = 0; i < Memory::MEMORY_TAG_MAX; i++) {
    if (stats.taggedAllocations[i] >= tib) {
      string.append(std::format(" {:11} {:>1.2f}{:3}\n", memoryTagStrigns[i], stats.taggedAllocations[i]/(f64)tib, "TiB"));
    }
    else if (stats.taggedAllocations[i] >= gib) {
      string.append(std::format(" {:11} {:>1.2f}{:3}\n", memoryTagStrigns[i], stats.taggedAllocations[i]/(f64)gib, "GiB"));
    }
    else if (stats.taggedAllocations[i] >= mib) {
      string.append(std::format(" {:11} {:>1.2f}{:3}\n", memoryTagStrigns[i], stats.taggedAllocations[i]/(f64)mib, "MiB"));
    }
    else if (stats.taggedAllocations[i] >= kib) {
      string.append(std::format(" {:11} {:>1.2f}{:3}\n", memoryTagStrigns[i], stats.taggedAllocations[i]/(f64)kib, "KiB"));
    }
    else {
      string.append(std::format(" {:11} {:>3}{:3}\n", memoryTagStrigns[i], stats.taggedAllocations[i], "B"));
    }
  }
  return string;
}
