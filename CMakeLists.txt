# ------------------------------------------------------------------------------
# CMakeLists.txt for aggresss/playground-libre
# ------------------------------------------------------------------------------

cmake_minimum_required(VERSION 2.8)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/CMake" ${CMAKE_MODULE_PATH})
include(Utilities)
include(GNUInstallDirs)

# ------------------------------------------------------------------------------
# Cross compile
# ------------------------------------------------------------------------------

# MUST before project()
if(CROSS_COMPILE)
  set(CMAKE_C_COMPILER ${CROSS_COMPILE}-gcc)
  set(CMAKE_CXX_COMPILER ${CROSS_COMPILE}-g++)
  set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
  set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
  set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
  set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
endif()

# ------------------------------------------------------------------------------
# Project
# ------------------------------------------------------------------------------

project("playground-libre")

# ------------------------------------------------------------------------------
# Get and Display infomation
# ------------------------------------------------------------------------------

message(STATUS "CMake version: " ${CMAKE_VERSION})
message(STATUS "CMake system name: " ${CMAKE_SYSTEM_NAME})

c_compiler_prefix(C_COMPILER_PREFIX)
if(C_COMPILER_PREFIX)
  message(STATUS "C compiler prefix: " ${C_COMPILER_PREFIX})
endif()

c_compiler_machine(C_COMPILER_MACHINE)
if(C_COMPILER_MACHINE)
  message(STATUS "C compiler machine infomation: " ${C_COMPILER_MACHINE})
endif()

# ------------------------------------------------------------------------------
# Build type
# ------------------------------------------------------------------------------

set(CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE}
  CACHE STRING "Choose the type of build: None Debug Release Coverage" FORCE)

if(CMAKE_BUILD_TYPE)
  message(STATUS "CMake build type: " ${CMAKE_BUILD_TYPE})
endif()

# Default C / CXX flags
if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall ")
  set(CMAKE_CXX_FLAGS ${CMAKE_C_FLAGS})
endif()

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g -ggdb -O0 -Wall")
set(CMAKE_CXX_FLAGS_DEBUG ${CMAKE_C_FLAGS_DEBUG})

set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3 -Wall -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_RELEASE})

set(CMAKE_C_FLAGS_COVERAGE "${CMAKE_C_FLAGS_DEBUG} -fprofile-arcs -ftest-coverage")
set(CMAKE_CXX_FLAGS_COVERAGE ${CMAKE_C_FLAGS_COVERAGE})

# ------------------------------------------------------------------------------
# Test
# ------------------------------------------------------------------------------

find_program(VALGRIND valgrind)
if(VALGRIND)
  # Ctest configuration variables must be set BEFORE include(Ctest)
  set(MEMORYCHECK_COMMAND ${VALGRIND})
  # Check process children and give detail for each leak
  set(MEMORYCHECK_COMMAND_OPTIONS
    "${MEMORYCHECK_COMMAND_OPTIONS} --leak-check=yes --show-reachable=yes")
  set(MEMORYCHECK_COMMAND_OPTIONS
    "${MEMORYCHECK_COMMAND_OPTIONS} --trace-children=yes --track-fds=yes")
  # Set error code.
  set(MEMORYCHECK_COMMAND_OPTIONS
    "${MEMORYCHECK_COMMAND_OPTIONS} --error-exitcode=255")
  # Generate suppression hint.
  set(MEMORYCHECK_COMMAND_OPTIONS
    "${MEMORYCHECK_COMMAND_OPTIONS} --gen-suppressions=all")
  # Set suppression file
  set(MEMORYCHECK_COMMAND_OPTIONS
    "${MEMORYCHECK_COMMAND_OPTIONS} --suppressions=${CMAKE_CURRENT_LIST_DIR}/valgrind.supp")
endif()
include (CTest)
enable_testing()

# ------------------------------------------------------------------------------
# Project options
# ------------------------------------------------------------------------------

option(BUILD_ALL "Build all submodule" OFF)

# ------------------------------------------------------------------------------
# Common directory
# ------------------------------------------------------------------------------

set(TOP_DIRS ${PROJECT_SOURCE_DIR})
set(THIRD_PARTY_DIRS ${PROJECT_SOURCE_DIR}/third_party)

# libre
set(LIBRE_DIRS ${THIRD_PARTY_DIRS}/libre)
set(LIBRE_INCLUDE_DIRS ${LIBRE_DIRS}/include)
set(LIBRE_SRC_DIRS ${LIBRE_DIRS}/src)

# librem
set(LIBREM_DIRS ${THIRD_PARTY_DIRS}/librem)
set(LIBREM_INCLUDE_DIRS ${LIBREM_DIRS}/include)
set(LIBREM_SRC_DIRS ${LIBREM_DIRS}/src)

# ------------------------------------------------------------------------------
# definition for libre
# ------------------------------------------------------------------------------

add_definitions(
  -DHAVE_PTHREAD
  -DHAVE_GETIFADDRS
  -DHAVE_STRERROR_R
  -DHAVE_GETOPT
  -DHAVE_INTTYPES_H
  -DHAVE_NET_ROUTE_H
  -DHAVE_SYS_SYSCTL_H
  -DHAVE_STDBOOL_H
  -DHAVE_INET6
  -DHAVE_RESOLV
  -DHAVE_SYSLOG
  -DHAVE_FORK
  -DHAVE_INET_NTOP
  -DHAVE_PWD_H
  -DHAVE_POLL
  -DHAVE_INET_PTON
  -DHAVE_SELECT
  -DHAVE_SELECT_H
  -DHAVE_SETRLIMIT
  -DHAVE_SIGNAL
  -DHAVE_SYS_TIME_H
  -DHAVE_EPOLL
  -DHAVE_UNAME
  -DHAVE_UNISTD_H
  -DHAVE_STRINGS_H
  -DHAVE_ROUTE_LIST
  )

# ------------------------------------------------------------------------------
# Subdirectory
# ------------------------------------------------------------------------------

SUBDIRLIST(SUB_DIRS ${PROJECT_SOURCE_DIR})
foreach(subdir ${SUB_DIRS})
  if(EXISTS ${CMAKE_CURRENT_LIST_DIR}/${subdir}/CMakeLists.txt)
    add_subdirectory(${subdir})
  endif()
endforeach()
