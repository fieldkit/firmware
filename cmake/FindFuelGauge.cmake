if(TARGET FuelGauge)
  return()
endif()

add_external_arduino_library(FuelGauge)

find_package(Wire)

target_link_libraries(FuelGauge Wire)
