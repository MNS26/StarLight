#include "renderer_backend.h"

#include "vulkan/vulkan_backend.h"

b8 renderer_backend_create(renderer_backend_type type, struct platform_state* platform_state, renderer_backend* out_renderer_backend) {
  out_renderer_backend->platform_state = platform_state;

  if (type == RENDERER_BACKEND_TYPE_VULKAN) {
    out_renderer_backend->initialize = vulkan_renderer_backend_initialize;
    out_renderer_backend->shutdown = vulkan_renderer_backend_shutdown;
    out_renderer_backend->begin_frame = vulkan_renderer_backend_begin_frame;
    out_renderer_backend->end_frame = vulkan_renderer_backend_end_frame;
    out_renderer_backend->resized = vulkan_renderer_backend_on_resized;

    return true;
  } else if (type == RENDERER_BACKEND_TYPE_OPENGL) {
    return false;
  } else if (type == RENDERER_BACKEND_TYPE_DIRECTX) {
    return false;
  }
  return false;
}

void renderer_backend_destroy(renderer_backend* render_backend) {
  render_backend->initialize = 0;
  render_backend->shutdown = 0;
  render_backend->begin_frame = 0;
  render_backend->end_frame = 0;
  render_backend->resized = 0;
}