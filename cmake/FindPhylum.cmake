if(TARGET phylum)
  return()
endif()

set(phylum_RECURSE True)

if(${TARGET_ARCH} MATCHES "amd64")
  file(GLOB_RECURSE sources ${phylum_PATH}/src/*.cpp ${phylum_PATH}/src/*.c)
  add_library(phylum STATIC ${sources})

  find_package(arduino-logging)
  target_link_libraries(phylum arduino-logging)

  find_package(Dhara)
  target_link_libraries(phylum Dhara)
else()
  add_external_arduino_library(phylum)

  find_package(arduino-logging)
  target_link_libraries(phylum arduino-logging)

  find_package(Dhara)
  target_link_libraries(phylum Dhara)

  find_package(SPI)
  target_link_libraries(phylum SPI)

  target_compile_options(phylum PUBLIC -DPHYLUM_LOCAL_EXCHANGE=1 -I${CMAKE_SOURCE_DIR}/fk)
endif()

target_include_directories(phylum
    PUBLIC ${phylum_PATH}/src
    PRIVATE ${phylum_PATH}/src
)
