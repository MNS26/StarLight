Project(StarLightEngine VERSION 0.0.0 LANGUAGES CXX)


## ======================== ##
## 3rd Thridparty libraries ##
## ======================== ##
SET(CMAKE_FIND_LIBRARY_SUFFIXES "" ".so" ".so.1" ".dll" ".lib")
find_package(Vulkan REQUIRED)
find_package(SDL3 REQUIRED)
find_package(PkgConfig REQUIRED)

set(ONLY_LIBS ON)

#add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Thirdparty/VMA)
#add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/extern/spdlog)
#add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Thirdparty/VkBootstrap)


## ===================== ##
## Engine library target ##
## ===================== ##
add_library(${PROJECT_NAME} STATIC)

# Compiler flags
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|CLang")
  target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
  if(CMAKE_BUILD_TYPE MATCHES "Debug|RelWithDebug")
    target_compile_options(${PROJECT_NAME} PRIVATE -g -O0)
  endif()
endif()

if (NOT CMAKE_BUILD_TYPE MATCHES "Debug|RelWithDebug")
  target_compile_definitions(${PROJECT_NAME} PRIVATE -DDISABLE_DEBUG)
  message(STATUS "Building Release version, disabling DEBUG and TRACE debug levels")
endif ()

set_target_properties(${PROJECT_NAME} PROPERTIES
  ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Output/lib
)

target_include_directories(${PROJECT_NAME} PUBLIC ${SDL3_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
file(GLOB_RECURSE STARLIGHT_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")
list(FILTER STARLIGHT_SOURCES EXCLUDE REGEX ".*example.*|.*tests.*")
target_sources(${PROJECT_NAME} PRIVATE ${STARLIGHT_SOURCES})

#target_link_libraries(${PROJECT_NAME} PUBLIC glm)
#target_link_libraries(${PROJECT_NAME} PRIVATE uuid)
target_link_libraries(${PROJECT_NAME} PUBLIC ${SDL3_LIBRARIES})
target_link_libraries(${PROJECT_NAME} PUBLIC ${Vulkan_LIBRARIES})
#target_link_libraries(${PROJECT_NAME} PRIVATE vk-bootstrap::vk-bootstrap)


# If Vulkan::Vulkan target exists use it
if (TARGET Vulkan::Vulkan)
  target_link_libraries(${PROJECT_NAME} PRIVATE Vulkan::Vulkan)
endif()

# If spdlog::spdlog target exists use it
#if (TARGET spdlog::spdlog)
#  target_link_libraries(${PROJECT_NAME} PUBLIC spdlog::spdlog)
#endif()

#target_link_libraries(${PROJECT_NAME} PRIVATE GPUOpen::VulkanMemoryAllocator)

# Find glslangValidator for shader compilation
#find_program(GLSLANG_VALIDATOR glslangValidator)
#if(GLSLANG_VALIDATOR)
#  # Compile shaders to SPIR-V
#  add_custom_command(
#        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/cube.vert.spv
#        COMMAND ${GLSLANG_VALIDATOR} -V ${CMAKE_CURRENT_SOURCE_DIR}/src/Shaders/cube.vert -o ${CMAKE_CURRENT_BINARY_DIR}/cube.vert.spv
#        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/Shaders/cube.vert
#    )
#
#  add_custom_command(
#        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/cube.frag.spv
#        COMMAND ${GLSLANG_VALIDATOR} -V ${CMAKE_CURRENT_SOURCE_DIR}/src/Shaders/cube.frag -o ${CMAKE_CURRENT_BINARY_DIR}/cube.frag.spv
#        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/Shaders/cube.frag
#    )
#
#  add_custom_target(shaders ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/cube.vert.spv ${CMAKE_CURRENT_BINARY_DIR}/cube.frag.spv)
#else()
#  message(WARNING "glslangValidator not found - shaders must be compiled manually")
#endif()

# compile into the same config folder as the editor
set_target_properties(${PROJECT_NAME} PROPERTIES
#  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIG>/runtime
#  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Output/$<CONFIG>
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PLATFORM}/$<CONFIG>
)