cmake_minimum_required(VERSION 3.7.2)

# ---- project ----

project("thread-tools" VERSION "0.2.0" LANGUAGES "C" "CXX"
  HOMEPAGE_URL "https://github.com/robertefry/thread-tools"
  DESCRIPTION "A collection of concurrency tools, spawned from the development of RockHopper.")

set(CMAKE_CXX_STANDARD "20")

# ---- add dependencies via CPM ----
# see https://github.com/TheLartians/CPM.cmake for more info
include(.cmake/get_cpm.cmake)

CPMAddPackage(NAME "googletest"
  GITHUB_REPOSITORY "google/googletest" GIT_TAG "release-1.12.1"
  OPTIONS "INSTALL_GTEST OFF" "gtest_force_shared_crt"
)

# ---- project units ----
include(.cmake/tools.cmake) # toolkit for buildsystem features

add_subdirectory(thread-tools)

# ---- project installable targets ----
# this allows users to install and find the library via `find_package`.
CPMAddPackage("gh:TheLartians/PackageProject.cmake@1.10.0")

# FIXME: change to match your project packages
packageProject(NAME "thread-tools"
  VERSION ${PROJECT_VERSION}
  NAMESPACE ${PROJECT_NAME}
  BINARY_DIR ${PROJECT_BINARY_DIR}
  COMPATIBILITY SameMajorVersion
)

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
