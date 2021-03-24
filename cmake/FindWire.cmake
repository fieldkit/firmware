if(TARGET Wire)
  return()
endif()

file(GLOB files ${ARDUINO_BOARD_CORE_LIBRARIES_PATH}/Wire/*.cpp)

add_arduino_library(Wire ${files})

target_include_directories(Wire
    PUBLIC ${ARDUINO_BOARD_CORE_LIBRARIES_PATH}/Wire
    PRIVATE ${ARDUINO_BOARD_CORE_LIBRARIES_PATH}/Wire
)
