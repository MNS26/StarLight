#include "core/slstring.h"
#include "core/slmemory.h"

#include <string.h>
#include "slstring.h"

u64 string_length(const char *str) {
  return strlen(str);
}

char* string_duplicate(const char* str) {
  u64 length = string_length(str);
  char* copy = slallocate(length + 1, MEMORY_TAG_STRING);
  slcopy_memory(copy, str, length + 1);
  return copy;
}