if(TARGET SparkFun_MS5803-14BA_Breakout_Arduino_Library)
  return()
endif()

add_external_arduino_library(SparkFun_MS5803-14BA_Breakout_Arduino_Library)

find_package(Wire)

target_link_libraries(SparkFun_MS5803-14BA_Breakout_Arduino_Library Wire)
