cmake_minimum_required(VERSION 3.7.2)

# ---- project ----

# FIXME: change to match your project definition
project("initial-cpp" VERSION "2.0.0" LANGUAGES "C" "CXX"
  HOMEPAGE_URL "https://github.com/robertefry/initial-cpp"
  DESCRIPTION "An initial C++ project to source other projects from.")

set(CMAKE_CXX_STANDARD "20")

# ---- add dependencies via CPM ----
# see https://github.com/TheLartians/CPM.cmake for more info
include(.cmake/get_cpm.cmake)

# FIXME: change to match your project dependency requirements
CPMAddPackage(NAME "fmt"
  GITHUB_REPOSITORY "fmtlib/fmt" GIT_TAG "9.1.0"
  OPTIONS "FMT_INSTALL NO"
)
CPMAddPackage(NAME "googletest"
  GITHUB_REPOSITORY "google/googletest" GIT_TAG "release-1.12.1"
  OPTIONS "INSTALL_GTEST OFF" "gtest_force_shared_crt"
)

# ---- project units ----
include(.cmake/tools.cmake) # toolkit for buildsystem features

# FIXME: change to match your project compilation units
add_subdirectory(project-lib)
add_subdirectory(project-bin)

# ---- cmake & compiler options ----

# dissallow in-source builds
if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
  message(FATAL_ERROR
    "In-source build are disallowed, please run CMake from a build directory.")
endif()

# set cxx compiler flags
if ((CMAKE_CXX_COMPILER_ID STREQUAL "GNU"  ) OR
    (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
)
  add_compile_options(-Werror)
  add_compile_options(-Wall -Wextra -Wconversion -Wshadow -Wpedantic -Weffc++)
  add_compile_options(-Wno-unused)
endif()
