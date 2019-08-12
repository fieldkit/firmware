#
#
#
if(TARGET QrCode)
  return()
endif()

file(GLOB sources ${QRCode_PATH}/src/*.c)

if(${TARGET_ARCH} MATCHES "amd64")
  add_library(QrCode STATIC ${sources})
else()
  add_arduino_library(QrCode "${sources}")
endif()

target_include_directories(QrCode
  PUBLIC ${QRCode_PATH}/src
  PRIVATE ${QRCode_PATH}/src
)
