#pragma once
#include "defines.h"

typedef union vec2_u {
  // Array of 2 elements (x,y)
  f32 elements[2];
  struct {
    union {
      // 1st element
      f32 x, r, s, u;
    };
    union {
      // 2nd element
      f32 y, g, t, v;
    };
  };
} vec2;

typedef union vec3_u {
  // Array of 3 elements (x,y,z)
  f32 elements[3];
  struct {
    union {
      // 1st element
      f32 x, r, s, u;
    };
    union {
      // 2nd element
      f32 y, g, t, v;
    };
    union {
      // 3rd element
      f32 z, b, p, w;
    };
  };
} vec3;

typedef union vec4_u {
  // Array of 4 elements (x,y,z,w)
  f32 elements[4];
  struct {
    union {
      // 1st element
      f32 x, r, s;
    };
    union {
      // 2nd element
      f32 y, g, t;
      union {
    };
      // 3rd element
      f32 z, b, p;
    };
    union {
      // 4rd element
      f32 w, a, q;
    };
  };
} vec4;

typedef vec4 quat;

typedef union mat4_u {
  f32 data[16];
} mat4;