# ------------------------------------------------------------------------------
# CMakeLists.txt for aggresss/playground-libre/examples
# ------------------------------------------------------------------------------

SUBDIRLIST(SUB_DIRS ${PROJECT_SOURCE_DIR})
foreach(subdir ${SUB_DIRS})
  if(EXISTS ${CMAKE_CURRENT_LIST_DIR}/${subdir}/CMakeLists.txt)
    add_subdirectory(${subdir})
  endif()
endforeach()