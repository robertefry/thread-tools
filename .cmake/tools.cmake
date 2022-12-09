
# This file contains a list of tools that can be activated and downloaded
# on-demand. Each tool is enabled during the configuration by passing an
# additional `-DUSE_<TOOL>=<VALUE>` argument to CMake.

include(${CMAKE_CURRENT_LIST_DIR}/get_cpm.cmake)

# ---- clang-tidy ----
# This enables clang-tidy static analysis through the use of the USE_CLANG_TIDY
# CMake variabe as a boolean.

set(USE_CLANG_TIDY ON CACHE BOOL "Use the clang-tidy static analyzer")

find_program(CLANG_TIDY NAMES "clang-tidy")
if (CLANG_TIDY)
  message(STATUS "Found clang-tidy: ${CLANG_TIDY}")
  if (NOT USE_CLANG_TIDY)
    message("clang-tidy NOT ENABLED via 'USE_CLANG_TIDY' variable!")
  endif()
elseif(USE_CLANG_TIDY)
  message(SEND_ERROR "Cannot enable clang-tidy, executable not found!")
endif()

if (USE_CLANG_TIDY AND CLANG_TIDY)
  foreach(LANG "C" "CXX" "OBJC" "OBJCXX")
    set(CMAKE_${LANG}_CLANG_TIDY ${CLANG_TIDY})
  endforeach()
endif()
