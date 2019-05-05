# ------------------------------------------------------------------------------
# CMakeLists.txt for aggresss/playground-libre subdirectory
# ------------------------------------------------------------------------------

string(REGEX MATCH "[^/]+$" SUBMODULE ${CMAKE_CURRENT_LIST_DIR})
option(${SUBMODULE} "Build ${SUBMODULE}" OFF)

if(${SUBMODULE} OR BUILD_ALL)
  # Build
  if(WITH_OPENSSL)
    include_directories(${OPENSSL_INCLUDE_DIRS})
    link_directories(${OPENSSL_LIBRARIES})
  endif()
  
  include_directories(${LIBRE_INCLUDE_DIRS})
  set(SRCS ${LIBRE_BASE_SRCS})

  # specific definition for libre
  add_definitions(
    -DUSE_OPENSSL
    -DUSE_TLS
    )
  # specific module for libre
  set(MODULES
    tcp
    udp
    tls/openssl
    hash
    dns
    md5
    httpauth
    http
    )
  foreach(module ${MODULES})
    file(GLOB CUR_SRCS ${LIBRE_SRC_DIRS}/${module}/*.c)
    list(APPEND SRCS ${CUR_SRCS})
  endforeach()
  
  # specific file for libre
  set(LIBRE_MODULE_SPEC_FILE
    main/openssl.c
  )
  foreach(one_file ${LIBRE_MODULE_SPEC_FILE})
    list(APPEND SRCS ${LIBRE_SRC_DIRS}/${one_file})
  endforeach()

  aux_source_directory(${CMAKE_CURRENT_LIST_DIR} SRCS)
  add_executable(${SUBMODULE} ${SRCS})
  target_compile_options(${SUBMODULE} PRIVATE -std=c99)
  target_link_libraries(${SUBMODULE} ssl crypto pthread dl)
endif()

