#
#
#
project(fk)
cmake_minimum_required(VERSION 3.6)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
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
set(VERSION_MINOR 2 CACHE STRING "Project minor version number.")
set(VERSION_PATCH 0 CACHE STRING "Project patch version number.")

if(DEFINED ENV{BUILD_NUMBER})
message("Using ENV build number for prerelease.")
set(VERSION_PREL $ENV{BUILD_NUMBER} CACHE STRING "Project prelease version number.")
else()
message("No build prerelease.")
set(VERSION_PREL 0 CACHE STRING "Project prelease version number.")
endif()
mark_as_advanced(VERSION_MAJOR VERSION_MINOR VERSION_PATCH)

set(FK_VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}-${GIT_BRANCH}.${VERSION_PREL}-${GIT_COMMIT_HASH}")

message("Version: ${FK_VERSION}")

message("Target Architecture: ${TARGET_ARCH}")

message("FK_UNDERWATER: ${FK_UNDERWATER}")

if("${FK_UNDERWATER}" STREQUAL "")
  set(FIELDKIT_BOARD "fieldkit_core")
else()
  set(FIELDKIT_BOARD "fieldkit_underwater")
endif()

enable_testing()

add_subdirectory(boards)

if(${TARGET_ARCH} MATCHES "samd51")
  if(${FK_TARGET_QSPI_MEMORY})
    add_subdirectory(fk)
  else()
    add_subdirectory(bootloader)
    add_subdirectory(fk)
  endif()
endif()

add_subdirectory(modules)

add_subdirectory(tests)

# Add a target to generate API documentation with Doxygen
find_package(Doxygen)
option(BUILD_DOCUMENTATION "Create and install the HTML based API documentation (requires Doxygen)" ${DOXYGEN_FOUND})

option(FK_PHYLUM "Enable phylum by default" NO)
option(FK_PHYLUM_AMPLIFICATION "Amplify phylum readings" 1)

if(BUILD_DOCUMENTATION)
    if(NOT DOXYGEN_FOUND)
        message(FATAL_ERROR "Doxygen is needed to build the documentation.")
    endif()

    set(doxyfile_in ${CMAKE_CURRENT_SOURCE_DIR}/doc/Doxyfile.in)
    set(doxyfile ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

    configure_file(${doxyfile_in} ${doxyfile} @ONLY)

    add_custom_target(doc
        COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM)

    install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/html DESTINATION share/doc)
endif()
