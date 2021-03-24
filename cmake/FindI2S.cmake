if(TARGET I2S)
  return()
endif()

file(GLOB_RECURSE files ${ARDUINO_BOARD_CORE_LIBRARIES_PATH}/I2S/src/*.cpp)

add_arduino_library(I2S "${files}")

target_include_directories(I2S
    PUBLIC ${ARDUINO_BOARD_CORE_LIBRARIES_PATH}/I2S/src
    PRIVATE ${ARDUINO_BOARD_CORE_LIBRARIES_PATH}/I2S/src
)
