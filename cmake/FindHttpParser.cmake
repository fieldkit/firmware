#
#
#
if(TARGET HttpParser)
  return()
endif()

file(GLOB sources ${HttpParser_PATH}/http_parser.c)

if(${TARGET_ARCH} MATCHES "amd64")
  add_library(HttpParser STATIC ${sources})
else()
  add_arduino_library(HttpParser "${sources}")
endif()

target_include_directories(HttpParser
  PUBLIC ${HttpParser_PATH}
  PRIVATE ${HttpParser_PATH}
)
