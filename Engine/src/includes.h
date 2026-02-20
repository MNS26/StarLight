#pragma once
#include "defines.h"
#if defined(PLATFORM_WINDOWS)
// C
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>

// C++
#include <cstdio>
#include <string>
#include <format>
#include <iostream>

#elif defined(PLATFORM_LINUX)
// C
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>

// C++
#include <iostream>
#include <cstdint>
#include <cstdio>
#include <format>
#include <string>
#include <vector>
#include <algorithm>
#endif
