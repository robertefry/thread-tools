
cmake_minimum_required(VERSION 3.5)

# Define binary
set(BINARY_NAME "thread-tools-tests")
add_executable(${BINARY_NAME} "")

# Define compiler configuration
target_compile_options(${BINARY_NAME} PRIVATE -Wall -Wextra -Weffc++ -Wpedantic)
target_compile_options(${BINARY_NAME} PRIVATE -Wno-unused)

# Define include directories
target_include_directories(${BINARY_NAME} PUBLIC "include")
target_include_directories(${BINARY_NAME} PRIVATE "src")

# Define build sources
file(GLOB_RECURSE BINARY_SOURCES
  "src/*.c" "src/*.cc" "src/*.cpp" "src/*.cxx"
)
target_sources(${BINARY_NAME} PRIVATE ${BINARY_SOURCES})

target_link_libraries(${BINARY_NAME} PRIVATE thread-tools)

# Use GTest
find_package(GTest CONFIG REQUIRED)
target_link_libraries(${BINARY_NAME} PRIVATE GTest::gmock GTest::gtest GTest::gmock_main GTest::gtest_main)

# Add test target
add_test(NAME ${BINARY_NAME} COMMAND $<TARGET_FILE:${BINARY_NAME}>)

# Add launch target
add_custom_target(test-thread-tools
  COMMAND $<TARGET_FILE:${BINARY_NAME}>
  DEPENDS ${BINARY_NAME})
