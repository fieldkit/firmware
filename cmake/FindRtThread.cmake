if (TARGET Arduino_RT-Thread)
  return()
endif()

include(${CMAKE_CURRENT_SOURCE_DIR}/dependencies.cmake)

set(Arduino_RT-Thread_RECURSE True)

add_external_arduino_library(Arduino_RT-Thread)

find_package(SPI)

target_include_directories(Arduino_RT-Thread
    PUBLIC ${Arduino_RT-Thread_PATH}/src
    PRIVATE ${Arduino_RT-Thread_PATH}/src
)

target_link_libraries(Arduino_RT-Thread SPI)
