#
#
#
if(NOT TARGET app-protocol)
  if(${TARGET_ARCH} MATCHES "amd64")
    file(GLOB sources ${app-protocol_PATH}/src/*.c)
    add_library(app-protocol STATIC ${sources})
    target_include_directories(app-protocol
      PUBLIC ${app-protocol_PATH}/src
      PRIVATE ${app-protocol_PATH}/src
    )
  else()
    add_external_arduino_library(app-protocol)
  endif()

  find_package(nanopb)
  target_link_libraries(app-protocol nanopb)
endif()

if(NOT TARGET data-protocol)
  if(${TARGET_ARCH} MATCHES "amd64")
    file(GLOB sources ${data-protocol_PATH}/src/*.c)
    add_library(data-protocol STATIC ${sources})
    target_include_directories(data-protocol
      PUBLIC ${data-protocol_PATH}/src
      PRIVATE ${data-protocol_PATH}/src
    )
  else()
    add_external_arduino_library(data-protocol)
  endif()

  find_package(nanopb)
  target_link_libraries(data-protocol nanopb)
endif()

if(NOT TARGET atlas-protocol)
  if(${TARGET_ARCH} MATCHES "amd64")
    file(GLOB sources ${atlas-protocol_PATH}/src/*.c)
    add_library(atlas-protocol STATIC ${sources})
    target_include_directories(atlas-protocol
      PUBLIC ${atlas-protocol_PATH}/src
      PRIVATE ${atlas-protocol_PATH}/src
    )
  else()
    add_external_arduino_library(atlas-protocol)
  endif()

  find_package(nanopb)
  target_link_libraries(atlas-protocol nanopb)
endif()