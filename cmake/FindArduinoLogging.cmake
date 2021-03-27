if (TARGET ArduinoLogging)
  return()
endif()

if(${TARGET_ARCH} MATCHES "samd51")
  add_external_arduino_library(ArduinoLogging)
else()
  # message(${CMAKE_SOURCE_DIR})
  # message(${CMAKE_CURRENT_SOURCE_DIR})
  # message(${CMAKE_CURRENT_LIST_DIR})

  file(GLOB sources ${CMAKE_CURRENT_LIST_DIR}/../src/*.cpp ${CMAKE_CURRENT_LIST_DIR}/../src/*.h)

  add_library(ArduinoLogging STATIC ${sources})

  target_include_directories(ArduinoLogging PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../src)
endif()
