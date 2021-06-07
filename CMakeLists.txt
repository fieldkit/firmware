cmake_minimum_required(VERSION 3.17)

project(Sectors)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

find_package(Git)

if(DEFINED ENV{GIT_LOCAL_BRANCH})
message("Using ENV GIT_LOCAL_BRANCH")
set(GIT_BRANCH $ENV{GIT_LOCAL_BRANCH})
else()
execute_process(
  COMMAND ${GIT_EXECUTABLE} rev-parse --abbrev-ref HEAD
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE GIT_BRANCH
  OUTPUT_STRIP_TRAILING_WHITESPACE
  )
endif()

if (DEFINED ENV{GIT_COMMIT})
message("Using ENV GIT_COMMIT")
set(GIT_COMMIT_HASH $ENV{GIT_COMMIT})
else()
execute_process(
  COMMAND ${GIT_EXECUTABLE} log -1 --format=%h
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE GIT_COMMIT_HASH
  OUTPUT_STRIP_TRAILING_WHITESPACE
  )
endif()

message("Branch: ${GIT_BRANCH}")
message("Hash: ${GIT_COMMIT_HASH}")

set(VERSION_MAJOR 0 CACHE STRING "Project major version number.")
set(VERSION_MINOR 1 CACHE STRING "Project minor version number.")
set(VERSION_PATCH 0 CACHE STRING "Project patch version number.")

if(DEFINED ENV{BUILD_NUMBER})
message("Using ENV build number for prerelease.")
set(VERSION_PREL $ENV{BUILD_NUMBER} CACHE STRING "Project prelease version number.")
else()
message("No build prerelease.")
set(VERSION_PREL 0 CACHE STRING "Project prelease version number.")
endif()
mark_as_advanced(VERSION_MAJOR VERSION_MINOR VERSION_PATCH)

set(PHYLUM_VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}-${GIT_BRANCH}.${VERSION_PREL}-${GIT_COMMIT_HASH}")

message("Version: ${PHYLUM_VERSION}")

configure_file(version.txt.in ${CMAKE_BINARY_DIR}/version.txt)

add_subdirectory(src)
add_subdirectory(example)
add_subdirectory(tests)
