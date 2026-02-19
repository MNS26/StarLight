project(StarLightRuntime LANGUAGES C CXX)
add_executable(${PROJECT_NAME})

# Compiler flags
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|CLang")
  target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
  if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(${PROJECT_NAME} PRIVATE -g -O0)
  endif()
endif()

file(GLOB_RECURSE RUNTIME_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
target_sources(${PROJECT_NAME} PRIVATE ${RUNTIME_SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(${PROJECT_NAME} PUBLIC StarLightEngine)

# compile into the same config folder as the editor
set_target_properties(${PROJECT_NAME} PROPERTIES
#  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIG>/runtime
#  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Output/$<CONFIG>
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PLATFORM}/$<CONFIG>
)