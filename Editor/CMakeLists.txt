project(StarLightEditor VERSION 0.0.0 LANGUAGES CXX)

add_executable(${PROJECT_NAME})
file(GLOB_RECURSE EDITOR_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
target_sources(${PROJECT_NAME} PRIVATE ${EDITOR_SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(${PROJECT_NAME} PUBLIC StarLightEngine)

# Compiler flags
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|CLang")
  target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
  if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(${PROJECT_NAME} PRIVATE -g -O0)
  endif()
endif()

add_dependencies(${PROJECT_NAME} StarLightRuntime)
set_target_properties(${PROJECT_NAME} PROPERTIES
#  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIG>/editor
#  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Output/$<CONFIG>
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PLATFORM}/$<CONFIG>
)
# Install Step
install(TARGETS ${PROJECT_NAME} # executable
    RUNTIME DESTINATION .
)

#install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/res/ # resources
#        DESTINATION editor_res)
