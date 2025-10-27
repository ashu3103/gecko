cmake_minimum_required(VERSION 3.15)

project(GeckoLibrary LANGUAGES CXX VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Prevent in-source builds
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
  message(FATAL_ERROR
    "In-source builds are not allowed. Please use:\n"
    "  mkdir build && cd build && cmake .."
  )
endif()

add_compile_options(-std=c++20)

# Add library subdirectories
add_subdirectory(lib/core)
add_subdirectory(src)
