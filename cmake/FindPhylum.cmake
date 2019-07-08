if(TARGET phylum)
  return()
endif()

set(phylum_RECURSE True)

add_external_arduino_library(phylum)

find_package(arduino-logging)
target_link_libraries(phylum arduino-logging)

find_package(SPI)
target_link_libraries(phylum SPI)

target_compile_options(phylum PUBLIC -DPHYLUM_ENABLE_SD)

target_include_directories(phylum
    PUBLIC ${phylum_PATH}/src
    PRIVATE ${phylum_PATH}/src
)
