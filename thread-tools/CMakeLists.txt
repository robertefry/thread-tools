
cmake_minimum_required(VERSION 3.12)

# ---- binary ----

set(BINARY_NAME "thread-tools")
add_library(${BINARY_NAME} INTERFACE)

# ---- compiler targets ----

target_include_directories(${BINARY_NAME}
  INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# ---- testing ----

add_subdirectory(tests)
