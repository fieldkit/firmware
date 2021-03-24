if(TARGET u8g2)
  return()
endif()

add_external_arduino_library(u8g2)

find_package(SPI)
target_link_libraries(u8g2 SPI)

find_package(Wire)
target_link_libraries(u8g2 Wire)

target_compile_options(u8g2 PRIVATE -DU8G2_16BIT PUBLIC -DU8G2_16BIT)
