
cmake_minimum_required(VERSION 3.5)

# Define binary
set(BINARY_NAME "thread-tools")
add_library(${BINARY_NAME} INTERFACE)
add_subdirectory(tests)

# Define include directories
target_include_directories(${BINARY_NAME} INTERFACE "include")
