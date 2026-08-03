#pragma once
#include "vulkan_types.inl"

// Returns string representation of result
const char* vulkan_result_string(VkResult result, b8 get_extended);

// Indicates if the passed result is a success or not
b8 vulkan_result_is_success(VkResult result);