#include "renderer_frontend.h"
#include "renderer_backend.h"

#include "core/logger.h"
#include "core/slmemory.h"

// Backend render context
static renderer_backend* backend = 0;

b8 renderer_initialize(const char* application_name, struct platform_state* platform_state) {
  backend = slallocate(sizeof(renderer_backend), MEMORY_TAG_RENDERER);

  // todo, make it configurable
  renderer_backend_create(RENDERER_BACKEND_TYPE_VULKAN, platform_state, backend);
  backend->fram_number = 0;
  
  if (!backend->initialize(backend, application_name, platform_state)) {
    SLFATAL("Renderer backend failed to initialize! Shutting down.");
    return false;
  }

  return true;
}

void renderer_shutdown() {
  backend->shutdown(backend);

  slfree(backend, sizeof(renderer_backend), MEMORY_TAG_RENDERER);
}

void renderer_on_resize(u16 width, u16 height) {
  if (backend) {
    backend->resized(backend, width, height);
  } else {
    SLWARN("renderer backend does not exist to accept resize: %ix%i",width, height);
  }
}

b8 renderer_begin_frame(f32 delta_time) {
  return backend->begin_frame(backend, delta_time);
}

b8 renderer_end_frame(f32 delta_time) {
  b8 result = backend->end_frame(backend,delta_time);
  backend->fram_number++;
  return result;
}

b8 renderer_draw_frame(render_packet* packet) {
  // If the begin frame returned true, continue
  if (renderer_begin_frame(packet->delta_time)) {
    // End the frame if this fails. something went VERY wrong in this case.
    b8 result = renderer_end_frame(packet->delta_time);

    if (!result) {
      SLERROR("render_end_frame failed. Application shutting down…");
      return false;
    }
  }

  return true;
}