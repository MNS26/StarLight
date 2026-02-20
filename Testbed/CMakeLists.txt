project(StarLightTestbed VERSION 0.0.0 LANGUAGES CXX)

SET(CMAKE_FIND_LIBRARY_SUFFIXES "" ".so" ".so.1" ".dll" ".lib")
find_package(PkgConfig REQUIRED)

add_executable(${PROJECT_NAME})
file(GLOB_RECURSE EDITOR_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
target_sources(${PROJECT_NAME} PRIVATE ${EDITOR_SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(${PROJECT_NAME} PUBLIC StarLightEngine)

# Compiler flags
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|CLang")
  target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Werror -Wpedantic)
  if(CMAKE_BUILD_TYPE STREQUAL "Debug" OR "RelWithDebug")
    target_compile_options(${PROJECT_NAME} PRIVATE -g -O0)
  endif()

  # Disable Debug and Trace logging in release builds
  if(CMAKE_BUILD_TYPE MATCHES "Release")

    target_compile_definitions(${PROJECT_NAME} PRIVATE -DDISABLE_DEBUG)
    message(STATUS "Building Release version. Disabling DEBUG and TRACE logging")
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
