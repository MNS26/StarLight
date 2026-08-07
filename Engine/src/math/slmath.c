#include "slmath.h"
#include "platform/platform.h"

#include <math.h>
#include <stdlib.h>

static b8 rand_seeded = false;

f32 slsin(f32 x) {return sinf(x);}

f32 slcos(f32 x) {return cosf(x);}

f32 sltan(f32 x) {return tanf(x);}

f32 slacos(f32 x) {return acosf(x);}

f32 slsqrt(f32 x) {return sqrtf(x);}

f32 slabs(f32 x) {return fabsf(x);}

s32 slrandom() {
    if (!rand_seeded) {
        srand((u32)platform_get_absolute_time());
        rand_seeded = true;
    }
    return rand();
}

s32 slrandom_in_range(s32 min, s32 max) {
    if (!rand_seeded) {
        srand((u32)platform_get_absolute_time());
        rand_seeded = true;
    }
    return (rand() % (max - min + 1)) + min;
}

f32 fslrandom() {return (float)slrandom() / (f32)RAND_MAX;}

f32 fslrandom_in_range(f32 min, f32 max) {return min + ((float)slrandom() / ((f32)RAND_MAX / (max - min)));}