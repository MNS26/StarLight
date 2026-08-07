#include "vulkan_device.h"
#include "vulkan_command_buffer.h"
#include "core/logger.h"
#include "core/slstring.h"
#include "core/slmemory.h"
#include "containers/darray.h"


typedef struct vulkan_physical_device_requirements {
  b8 graphics;
  b8 present;
  b8 compute;
  b8 transfer;
  const char** device_extension_names;
  b8 sampler_anisotropy;
  b8 discrete_gpu;
} vulkan_physical_device_requirements;

typedef struct vulkan_physicial_device_queue_family_info {
  u32 graphics_family_index;
  u32 preset_family_index;
  u32 compute_family_index;
  u32 transfer_family_index;
} vulkan_physicial_device_queue_family_info;

b8 select_physical_device(vulkan_context* context);
b8 physicial_device_meet_requirements(
  VkPhysicalDevice device,
  VkSurfaceKHR surface,
  const VkPhysicalDeviceProperties* properties,
  const VkPhysicalDeviceFeatures* features,
  const vulkan_physical_device_requirements* requirements,
  vulkan_physicial_device_queue_family_info* out_queue_family_info,
  vulkan_swapchain_support_info* out_swapchain_support
);


b8 vulkan_device_create(vulkan_context* context) {
  if (!select_physical_device(context)) {
    return false;
  }
  // for now just return true otherwise

  SLINFO("Creating logical device...");
  // Do not create additional queues for shared queue families
  u32 index_count = 1;
  if (context->device.present_queue_index != context->device.graphics_queue_index)
    index_count++;

  if (context->device.transfer_queue_index != context->device.graphics_queue_index &&
      context->device.transfer_queue_index != context->device.present_queue_index)
    index_count++;

  u32 indices[32];
  u8 index = 0;
  indices[index++] = context->device.graphics_queue_index;

  if (context->device.present_queue_index != context->device.graphics_queue_index)
    indices[index++] = context->device.present_queue_index;

  if (context->device.transfer_queue_index != context->device.graphics_queue_index &&
      context->device.transfer_queue_index != context->device.present_queue_index)
    indices[index++] = context->device.transfer_queue_index;


  VkDeviceQueueCreateInfo queue_create_info[32];
  for (u32 i = 0; i < index_count; ++i) {
    queue_create_info[i].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queue_create_info[i].queueFamilyIndex = indices[i];
    queue_create_info[i].queueCount = 1;
    // some devices dont support it
    // will need to do this properly later
    //if (indices[i] == context->device.graphics_queue_index)
    //  queue_create_info[i].queueCount = 2;
    queue_create_info[i].flags = 0;
    queue_create_info[i].pNext = 0;
    f32 queue_priority = 1.0f;
    queue_create_info[i].pQueuePriorities = &queue_priority;
  }

  // Request device features
  // Should be changed to config driven
  VkPhysicalDeviceFeatures device_features = {};
  device_features.samplerAnisotropy = VK_TRUE;

  VkDeviceCreateInfo device_create_info = {VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO};
  device_create_info.queueCreateInfoCount = index_count;
  device_create_info.pQueueCreateInfos = queue_create_info;
  device_create_info.pEnabledFeatures = &device_features;
  device_create_info.enabledExtensionCount = 1;
  const char* extension_names = VK_KHR_SWAPCHAIN_EXTENSION_NAME;
  device_create_info.ppEnabledExtensionNames= &extension_names;

  // Deprecated and ignored, so pass nothing
  device_create_info.enabledLayerCount = 0;
  device_create_info.ppEnabledLayerNames = 0;

  // Create the device
  VK_CHECK(vkCreateDevice(
    context->device.physical_device,
    &device_create_info,
    context->allocator,
    &context->device.logical_device
  ));
  SLINFO("Logical device created.");

  // Get queues
  vkGetDeviceQueue(
    context->device.logical_device,
    context->device.graphics_queue_index,
    0,
    &context->device.graphics_queue
  );
  vkGetDeviceQueue(
    context->device.logical_device,
    context->device.present_queue_index,
    0,
    &context->device.present_queue
  );
  vkGetDeviceQueue(
    context->device.logical_device,
    context->device.transfer_queue_index,
    0,
    &context->device.transfer_queue
  );
  vkGetDeviceQueue(
    context->device.logical_device,
    context->device.compute_queue_index,
    0,
    &context->device.compute_queue
  );
  SLINFO("Queues obtained.")

  //Create command pool for graphics queue
  VkCommandPoolCreateInfo pool_creat_info = {VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO};
  pool_creat_info.queueFamilyIndex = context->device.graphics_queue_index;
  pool_creat_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
  VK_CHECK(vkCreateCommandPool(
    context->device.logical_device,
    &pool_creat_info,
    context->allocator,
    &context->device.graphics_command_pool
  ));
  SLINFO("Graphics command pool created.");

  return true;
}

void vulkan_device_destroy(vulkan_context* context) {

  // Release queues
  context->device.graphics_queue = 0;
  context->device.present_queue = 0;
  context->device.transfer_queue = 0;
  context->device.compute_queue = 0;

  // Destroy graphics command pool
  if (context->device.graphics_command_pool) {
    vkDestroyCommandPool(context->device.logical_device, context->device.graphics_command_pool, context->allocator);
    context->device.graphics_command_pool = 0;
  }

  // Destroy logical device
  SLINFO("Destroying logical device...");
  if (context->device.logical_device) {
    vkDestroyDevice(context->device.logical_device, context->allocator);
    context->device.logical_device = 0;
  }

  // You cant destroy a physical device (that would get very expensive fast)
  SLINFO("Releasing physical device resource...");
  context->device.physical_device = 0;

  if (context->device.swapchain_support.formats) {
    slfree(
      context->device.swapchain_support.formats,
      sizeof(VkSurfaceFormatKHR) * context->device.swapchain_support.format_count,
      MEMORY_TAG_RENDERER
    );
    context->device.swapchain_support.formats = 0;
    context->device.swapchain_support.format_count = 0;
  }
  slzero_memory(&context->device.swapchain_support.capabilities,
    sizeof(context->device.swapchain_support.capabilities));

  context->device.graphics_queue_index = -1;
  context->device.present_queue_index = -1;
  context->device.transfer_queue_index = -1;
  context->device.compute_queue_index = -1;
  
}

void vulkan_device_query_swapchain_support(
  VkPhysicalDevice physical_device,
  VkSurfaceKHR surface,
  vulkan_swapchain_support_info* out_support_info
) {
  // Surface capabilities
  VK_CHECK(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physical_device, surface, &out_support_info->capabilities));

  // Surface formats
  VK_CHECK(vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, surface, &out_support_info->format_count, 0));

  if (out_support_info->format_count != 0) {
    if(!out_support_info->formats)
      out_support_info->formats = slallocate(sizeof(VkSurfaceFormatKHR) * out_support_info->format_count, MEMORY_TAG_RENDERER);
    VK_CHECK(vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, surface, &out_support_info->format_count, out_support_info->formats));
  }

  // Present modes
  VK_CHECK(vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, surface, &out_support_info->present_mode_count, 0));

  if (out_support_info->present_mode_count != 0) {
    if(!out_support_info->present_mode)
      out_support_info->present_mode = slallocate(sizeof(VkPresentModeKHR) * out_support_info->present_mode_count, MEMORY_TAG_RENDERER);
    VK_CHECK(vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, surface, &out_support_info->present_mode_count, out_support_info->present_mode));
  }
}

b8 vulkan_device_detect_depth_format(vulkan_device* device) {
  // Format indicies
  const u64 candidate_count = 3;
  VkFormat candidates[3] = {
    VK_FORMAT_D32_SFLOAT,
    VK_FORMAT_D32_SFLOAT_S8_UINT,
    VK_FORMAT_D24_UNORM_S8_UINT
  };
  u32 flags = VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT;
  for (u32 i = 0; i < candidate_count; ++i) {
    VkFormatProperties properties;
    vkGetPhysicalDeviceFormatProperties(device->physical_device, candidates[i], &properties);

    if ((properties.linearTilingFeatures & flags) == flags) {
      device->debpth_format = candidates[i];
      return true;
    } else if ((properties.optimalTilingFeatures & flags) == flags) {
      device->debpth_format = candidates[i];
      return true;
    }
  }
  return false;
}


b8 select_physical_device(vulkan_context* context) {
  u32 physical_device_count = 0;
  VK_CHECK(vkEnumeratePhysicalDevices(context->instance, &physical_device_count, 0));
  if (physical_device_count == 0) {
    SLFATAL("No devices which support Vulkan were found!");
    return false;
  }
//  const u32 max_device_count = 32;
  VkPhysicalDevice physical_devices[32];
  VK_CHECK(vkEnumeratePhysicalDevices(context->instance, &physical_device_count, physical_devices));
  
  for (u32 i = 0; i < physical_device_count; ++i) {
    VkPhysicalDeviceProperties properties;
    vkGetPhysicalDeviceProperties(physical_devices[i], &properties);

    VkPhysicalDeviceFeatures features;
    vkGetPhysicalDeviceFeatures(physical_devices[i], &features);

    VkPhysicalDeviceMemoryProperties memory;
    vkGetPhysicalDeviceMemoryProperties(physical_devices[i], &memory);

    // TODO: make it configurable what requirements are needed
    vulkan_physical_device_requirements requirements = {};
    requirements.graphics = true;
    requirements.present = true;
    requirements.transfer = true;
    // Can be optional
    //requirements.compute = true;
    requirements.sampler_anisotropy = true;
    requirements.discrete_gpu = false;
    requirements.device_extension_names = darray_create(const char*);
    darray_push(requirements.device_extension_names, &VK_KHR_SWAPCHAIN_EXTENSION_NAME);

    vulkan_physicial_device_queue_family_info queue_info = {};
    b8 result = physicial_device_meet_requirements(
      physical_devices[i],
      context->surface,
      &properties,
      &features,
      &requirements,
      &queue_info,
      &context->device.swapchain_support
    );

    if (result) {
      SLINFO("Selected device: '%s'.", properties.deviceName);
      // GPU type, etc
      switch (properties.deviceType) {
        default:
        case VK_PHYSICAL_DEVICE_TYPE_OTHER:
          SLINFO("GPU type is Unknown.");
          break;
        case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
          SLINFO("GPU type is Integrated.");
          break;
        case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
          SLINFO("GPU type is Discrete.");
          break;
        case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
          SLINFO("GPU type is Virtual.");
          break;
        case VK_PHYSICAL_DEVICE_TYPE_CPU:
          SLINFO("GPU type is CPU.");
          break;
      }
      // GPU driver version
      SLINFO("GPU Driver version: %d.%d.%d",
        VK_VERSION_MAJOR(properties.driverVersion),
        VK_VERSION_MINOR(properties.driverVersion),
        VK_VERSION_PATCH(properties.driverVersion)
      );

      // Vulkan API version
      SLINFO("Vulkan API version: %d.%d.%d",
        VK_VERSION_MAJOR(properties.apiVersion),
        VK_VERSION_MINOR(properties.apiVersion),
        VK_VERSION_PATCH(properties.apiVersion)
      );

      // Memory information
      for (u32 j =0; j < memory.memoryHeapCount; ++j) {
        f32 memory_size_gib = (((f32)memory.memoryHeaps[j].size) / 1024.0f / 1024.0f / 1024.0f);
        if (memory.memoryHeaps[j].flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT) {
          SLINFO("Local GPU memory: %.2f GiB",memory_size_gib);
        } else {
          SLINFO("Shared System memory: %.2f GiB",memory_size_gib);
        }
      }

      context->device.physical_device = physical_devices[i];
      context->device.graphics_queue_index = queue_info.graphics_family_index;
      context->device.present_queue_index = queue_info.preset_family_index;
      context->device.transfer_queue_index = queue_info.transfer_family_index;

      if (requirements.compute)
        context->device.compute_queue_index = queue_info.compute_family_index;


      // Keep a copy of properties, features and memory for later
      context->device.properties = properties;
      context->device.features = features;
      context->device.memory = memory;
      break;
    }
  }
  // Enusre we have a device
  if (!context->device.physical_device) {
    SLERROR("No physical devices were found that meet requirements.");
    return false;
  }

  SLINFO("Physicial device selected");
  return true;

}


b8 physicial_device_meet_requirements(
  VkPhysicalDevice device,
  VkSurfaceKHR surface,
  const VkPhysicalDeviceProperties* properties,
  const VkPhysicalDeviceFeatures* features,
  const vulkan_physical_device_requirements* requirements,
  vulkan_physicial_device_queue_family_info* out_queue_family_info,
  vulkan_swapchain_support_info* out_swapchain_support
) {
  // Evaluate device properties and if it meets requirements
  out_queue_family_info->graphics_family_index = -1;
  out_queue_family_info->preset_family_index = -1;
  out_queue_family_info->compute_family_index = -1;
  out_queue_family_info->transfer_family_index = -1;

  // Is it a discrete GPU
  if (requirements->discrete_gpu) {
    if (properties->deviceType != VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
      SLINFO("Device is not a discrete GPU, and one is required. skipping.");
      return false;
    }
  }

  u32 queue_family_count = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, 0);
  VkQueueFamilyProperties queue_families[32];
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, queue_families);
  
  //Look at every queue and find what is supported
  SLINFO("Graphics | Present | Compute | Transfer | Name");
  u8 min_trnafer_score = 255;
  for (u32 i = 0; i < queue_family_count; ++i) {
    u8 current_tranfser_score = 0;
    
    //Check graphics queue bit
    if (queue_families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
      out_queue_family_info->graphics_family_index = i;
      ++current_tranfser_score;
    }

    //Check compute queue bit
    if (queue_families[i].queueFlags & VK_QUEUE_COMPUTE_BIT) {
      out_queue_family_info->compute_family_index = i;
      ++current_tranfser_score;
    }

    //Check transfer queue bit
    if (queue_families[i].queueFlags & VK_QUEUE_TRANSFER_BIT) {
      out_queue_family_info->transfer_family_index = i;
      ++current_tranfser_score;
    }

    //Check present queue bit
    if (queue_families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
      // Take current index if its lower
      // Lower = more likely to be dedicated Transfer queue
      if (current_tranfser_score <= min_trnafer_score) {
        min_trnafer_score = current_tranfser_score;
        out_queue_family_info->graphics_family_index = i;
      }
    }

    // check present queue bit
    VkBool32 support_present = VK_FALSE;
    VK_CHECK(vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &support_present));
    if (support_present)
      out_queue_family_info->preset_family_index = i;
  }
  // Print out info about the found devices
  SLINFO("       %d |       %d |       %d |        %d | %s",
    out_queue_family_info->graphics_family_index != -1,
    out_queue_family_info->preset_family_index != -1,
    out_queue_family_info->compute_family_index != -1,
    out_queue_family_info->transfer_family_index != -1,
    properties->deviceName
  );
  if (
    (!requirements->graphics || (requirements->graphics && out_queue_family_info->graphics_family_index != -1)) &&
    (!requirements->present || (requirements->present && out_queue_family_info->preset_family_index != -1)) &&
    (!requirements->compute || (requirements->compute && out_queue_family_info->compute_family_index != -1)) &&
    (!requirements->transfer || (requirements->transfer && out_queue_family_info->transfer_family_index != -1))
  ) {
    SLINFO("Device meets requirements.");
    SLTRACE("Graphics Family Index: %i", out_queue_family_info->graphics_family_index);
    SLTRACE("Present Family Index: %i", out_queue_family_info->preset_family_index);
    SLTRACE("Transfer Family Index: %i", out_queue_family_info->transfer_family_index);
    SLTRACE("Compute Family Index: %i", out_queue_family_info->compute_family_index);

    // Query swapchain support
    vulkan_device_query_swapchain_support(device, surface, out_swapchain_support);

    if (out_swapchain_support->format_count < 1 || out_swapchain_support->present_mode_count < 1) {
      if (out_swapchain_support->formats) {
        slfree(out_swapchain_support->formats, sizeof(VkSurfaceFormatKHR) * out_swapchain_support->format_count, MEMORY_TAG_RENDERER);
      }
      if (out_swapchain_support->present_mode) {
        slfree(out_swapchain_support->present_mode, sizeof(VkSurfacePresentModeKHR) * out_swapchain_support->present_mode_count, MEMORY_TAG_RENDERER);
      }
      SLINFO("Required swapchain support not present, skipping device.");
      return false;
    }

    // Device extension
    if (requirements->device_extension_names) {
      u32 available_extension_count = 0;
      VkExtensionProperties* available_extensions = 0;
      VK_CHECK(vkEnumerateDeviceExtensionProperties(device, 0, &available_extension_count, 0));
      if (available_extension_count != 0) {
        available_extensions = slallocate(sizeof(VkExtensionProperties) * available_extension_count, MEMORY_TAG_RENDERER);
        VK_CHECK(vkEnumerateDeviceExtensionProperties(device, 0, &available_extension_count, available_extensions));


        u32 requred_extension_count = darray_length(requirements->device_extension_names);
        for (u32 i = 0; i <requred_extension_count; ++i) {
          b8 found = false;
          for (u32 j = 0; j < available_extension_count; ++j) {
            if (string_equals(requirements->device_extension_names[i], available_extensions[j].extensionName)) {
              found = true;
              break;
            }
          }
          if (!found) {
            SLINFO("Required extension not found: '%s', skipping device.", requirements->device_extension_names[i]);
            slfree(available_extensions, sizeof(VkExtensionProperties) * available_extension_count, MEMORY_TAG_RENDERER);
            return false;
          }
        }
      }
      slfree(available_extensions, sizeof(VkExtensionProperties) * available_extension_count, MEMORY_TAG_RENDERER);
    }
    // Sampler anisotropy
    if (requirements->sampler_anisotropy && !features->samplerAnisotropy) {
      SLINFO("Device does not support samplerAnisotropy, skipping.")
      return false;
    }

    return true;
  }
  return false;
}

