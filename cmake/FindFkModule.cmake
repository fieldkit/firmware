# Get the current working branch
execute_process(
  COMMAND git rev-parse --abbrev-ref HEAD
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} OUTPUT_VARIABLE GIT_BRANCH
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

# Get the latest abbreviated commit hash of the working branch
execute_process(
  COMMAND git log -1 --format=%H
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE GIT_COMMIT_HASH
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

add_definitions("-DFIRMWARE_GIT_HASH=\"${GIT_COMMIT_HASH}\"")
add_definitions("-DFIRMWARE_GIT_BRANCH=\"${GIT_BRANCH}\"")
add_definitions("-DFIRMWARE_BUILD=\"$ENV{BUILD_TAG}\"")

if (NOT TARGET firmware-common)
  if(NOT DEFINED EXTERNAL_DEPENDENCIES)
    include(${CMAKE_CURRENT_SOURCE_DIR}/dependencies.cmake)
  endif()

  set(_module_sources_path ${firmware-common_PATH})

  file(GLOB _module_sources ${_module_sources_path}/src/common/*.cpp ${_module_sources_path}/src/modules/*.cpp)

  add_arduino_library(firmware-common "${_module_sources}")

  target_compile_options(firmware-common PRIVATE -Wall -Werror)

  target_include_directories(firmware-common
    PUBLIC
      ${_module_sources_path}/src/common
      ${_module_sources_path}/src/modules
    PRIVATE
      ${_module_sources_path}/src/common
      ${_module_sources_path}/src/modules
  )

  find_package(nanopb)
  target_link_libraries(firmware-common nanopb)

  find_package(RTCZero)
  target_link_libraries(firmware-common RTCZero)

  find_package(RTClib)
  target_link_libraries(firmware-common RTClib)

  find_package(phylum)
  target_compile_options(phylum PUBLIC -DPHYLUM_ENABLE_SERIAL_FLASH -DPHYLUM_ENABLE_SD)
  target_link_libraries(firmware-common phylum)

  find_package(module-protocol)
  target_link_libraries(firmware-common module-protocol)

  find_package(Adafruit_ASFcore)
  target_link_libraries(firmware-common Adafruit_ASFcore)

  find_package(AtSamd)
  target_link_libraries(firmware-common AtSamd)

  find_package(arduino-logging)
  target_link_libraries(firmware-common arduino-logging)

  find_package(SerialFlash)
  target_link_libraries(firmware-common SerialFlash)

  find_package(Adafruit_NeoPixel)
  target_link_libraries(firmware-common Adafruit_NeoPixel)

  find_package(Wire)
  target_link_libraries(firmware-common Wire)

  find_package(SPI)
  target_link_libraries(firmware-common SPI)
endif()

function(fk_configure_module target_name)
  message(STATUS "Configuring FkModule: ${target_name}")

  target_link_libraries(${target_name} firmware-common)

  target_include_directories(${target_name} PRIVATE ./)
endfunction()
