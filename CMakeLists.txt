cmake_minimum_required(VERSION 3.26)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(StarLight VERSION 0.0.0)

# Compiler flags
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|CLang")
  # set it to Release if we dont have anything
  if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
    message(STATUS "CMAKE_BUILD_TYPE is empty! using Release by default")
  endif()
endif()

#Platform check
if(NOT PLATFORM)
  message(STATUS "PLATFORM is not set! using \"Linux\" by default")
  set(PLATFORM "Linux")
endif()

## ================== ##
## Global C++ Options ##
## ================== ##
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(Engine)
add_subdirectory(Editor)
add_subdirectory(Runtime)
add_subdirectory(Testbed)

