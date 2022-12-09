
cmake_minimum_required(VERSION 3.12)

# ---- binary ----

set(BINARY_NAME "project-lib")
add_library(${BINARY_NAME} SHARED "")

# ---- compiler targets ----

target_include_directories(${BINARY_NAME} PUBLIC "include")
target_include_directories(${BINARY_NAME} PRIVATE "src")

file(GLOB_RECURSE GLOB_SOURCES CONFIGURE_DEPENDS
  "src/*.c" "src/*.cc" "src/*.cpp" "src/*.cxx")
target_sources(${BINARY_NAME} PRIVATE ${GLOB_SOURCES})

# ---- testing ----

add_subdirectory(tests)
