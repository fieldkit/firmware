if(TARGET phylum)
  return()
endif()

set(phylum_RECURSE True)

if(${TARGET_ARCH} MATCHES "amd64")
  file(GLOB_RECURSE sources ${phylum_PATH}/src/*.cpp ${phylum_PATH}/src/*.c)
  add_library(phylum STATIC ${sources})

  find_package(arduino-logging)
  target_link_libraries(phylum arduino-logging)
else()
  add_external_arduino_library(phylum)

  find_package(arduino-logging)
  target_link_libraries(phylum arduino-logging)

  find_package(SPI)
  target_link_libraries(phylum SPI)

  target_compile_options(phylum PUBLIC -DPHYLUM_ENABLE_SD)
endif()

target_include_directories(phylum
    PUBLIC ${phylum_PATH}/src
    PRIVATE ${phylum_PATH}/src
)
