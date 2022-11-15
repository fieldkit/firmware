if(TARGET Ms5837)
  return()
endif()

add_external_arduino_library(Ms5837)

find_package(Wire)

target_link_libraries(Ms5837 Wire)
