if (TARGET AtSamd)
  return()
endif()

add_external_arduino_library(AtSamd)

find_package(Adafruit_ASFcore)

target_link_libraries(AtSamd Adafruit_ASFcore)
