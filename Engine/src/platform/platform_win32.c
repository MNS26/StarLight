#include "platform/platform.h"

#if defined(SLPLATFORM_WINDOWS)

#include <core/logger.h>
#include <windows.h>
#include <windowsx.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static u8 levels[6] = {64, 4, 6, 2, 1, 8};

void platform_console_write(const char* message, u8 color) {
  HANDLE console_handle = GetStdHandle(STD_ERROR_HANDLE);
  SetConsoleTextAttribute(console_handle, levels[color]);
  OutputDebugStringA(message+'\n');
  u64 length = strlen(message+'\n');
  DWORD number_written;
  WriteConsoleA(console_handle, message+'\n', (DWORD)length, &number_written, 0);
}

void platform_console_write_error(const char* message, u8 color) {
  HANDLE console_handle = GetStdHandle(STD_ERROR_HANDLE);
  SetConsoleTextAttribute(console_handle, levels[color]);
  OutputDebugStringA(message+'\n');
  u64 length = strlen(message+'\n');
  DWORD number_written;
  WriteConsoleA(console_handle, message+'\n', (DWORD)length, &number_written, 0);

}

#endif