
cmake_minimum_required(VERSION 3.5)

# ---- binary ----

set(BINARY_NAME "project-bin")
add_executable(${BINARY_NAME} "")

target_link_libraries(${BINARY_NAME} PUBLIC project-lib)

# ---- cmake targets ----

add_custom_target(run-${BINARY_NAME}
  COMMAND $<TARGET_FILE:${BINARY_NAME}>
  DEPENDS ${BINARY_NAME})

# ---- compiler targets ----

target_include_directories(${BINARY_NAME} PUBLIC "include")
target_include_directories(${BINARY_NAME} PRIVATE "src")

file(GLOB_RECURSE BINARY_SOURCES
  "src/*.c" "src/*.cc" "src/*.cpp" "src/*.cxx")
target_sources(${BINARY_NAME} PRIVATE ${BINARY_SOURCES})

target_link_libraries(${BINARY_NAME} PRIVATE fmt::fmt)
