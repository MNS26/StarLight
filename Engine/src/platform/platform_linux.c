#include "platform/platform.h"

#if defined(SLPLATFORM_LINUX)
#include <core/logger.h>

#include <SDL3/SDL.h>

#if _POSIX_C_SOURCE >= 199309L
#include <time.h>
#else
#include <unistd.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static const char* color_str[] = {"0;41","1;31","1;33","1;32","1;34","1;30"};

void platform_console_write(const char* message, u8 color) {
  printf("\033[%sm%s\033[0m\n", color_str[color], message);
}

void platform_console_write_error(const char* message, u8 color) {
  printf("\033[%sm%s\033[0m\n", color_str[color], message);
}

#endif