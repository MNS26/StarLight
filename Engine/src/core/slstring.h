#pragma once

#include "defines.h"

// Return the length of given string
SLAPI u64 string_length(const char* str);

SLAPI char* string_duplicate(const char* str);

// Case sensitive string compare
SLAPI b8 string_equals(const char* str1, const char* str2);