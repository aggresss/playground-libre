# ------------------------------------------------------------------------------
# CMakeLists.txt for aggresss/playground-libre subdirectory
# ------------------------------------------------------------------------------

string(REGEX MATCH "[^/]+$" SUBMODULE ${CMAKE_CURRENT_LIST_DIR})
option(${SUBMODULE} "Build ${SUBMODULE}" OFF)

if(${SUBMODULE} OR BUILD_ALL)
  # Build
  include_directories(${LIBRE_INCLUDE_DIRS})
  set(SRCS ${LIBRE_BASE_SRCS})
  list(APPEND SRCS main.c)
  add_executable(${SUBMODULE} ${SRCS})
  target_compile_options(${SUBMODULE} PRIVATE -std=c99)
  target_link_libraries(${SUBMODULE} pthread)
endif()

