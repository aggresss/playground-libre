# ------------------------------------------------------------------------------
# CMakeLists.txt for aggresss/playground-libre subdirectory
# ------------------------------------------------------------------------------

string(REGEX MATCH "[^/]+$" SUBMODULE ${CMAKE_CURRENT_LIST_DIR})
option(${SUBMODULE} "Build ${SUBMODULE}" OFF)

if(${SUBMODULE} OR BUILD_ALL)
  # Build
  include_directories(${LIBRE_INCLUDE_DIRS})
  set(SRCS ${LIBRE_BASE_SRCS})

  set(MODULES
    tcp
    )
  foreach(module ${MODULES})
    file(GLOB CUR_SRCS ${LIBRE_SRC_DIRS}/${module}/*.c)
    list(APPEND SRCS ${CUR_SRCS})
  endforeach()

  aux_source_directory(${CMAKE_CURRENT_LIST_DIR} SRCS)
  add_executable(${SUBMODULE} ${SRCS})
  target_compile_options(${SUBMODULE} PRIVATE -std=c99)
  target_link_libraries(${SUBMODULE} pthread)
endif()

