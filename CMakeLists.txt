cmake_minimum_required(VERSION 3.26)
project(triforce VERSION 0.0.1 LANGUAGES C CXX ASM)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
set(TRIFORCE_DIST_DIR "${CMAKE_CURRENT_LIST_DIR}/dist")

if(APPLE)
  message(WARNING "TriForce does not support Apple Silicon yet. Forcing x86_64.")
  set(CMAKE_OSX_ARCHITECTURES "x86_64")
endif()

add_subdirectory(src)

option(DISABLE_TESTING "Disable testing" OFF)
if(NOT DISABLE_TESTING)
  message(STATUS "Testing enabled")
  add_subdirectory(test)
endif()
