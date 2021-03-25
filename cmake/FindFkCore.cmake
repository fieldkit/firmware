function(fk_add_core_library target_name)
  include(${CMAKE_CURRENT_SOURCE_DIR}/dependencies.cmake)

  if(DEFINED firmware-common_PATH)
    set(${target_name}_PATH ${firmware-common_PATH})
    set(sources_path ${firmware-common_PATH})
  else()
    set(${target_name}_PATH ../src)
    set(sources_path ../)
  endif()

  file(GLOB sources ${sources_path}/src/common/*.cpp ${sources_path}/src/core/*.cpp)
  add_arduino_library(${target_name} "${sources}")

  target_compile_options(${target_name} PRIVATE -Wall -Werror)

  target_include_directories(${target_name}
    PUBLIC
      ${sources_path}/src/common
      ${sources_path}/src/core
    PRIVATE
      ${sources_path}/src/common
      ${sources_path}/src/core
  )

  find_package(arduino-logging)
  target_link_libraries(${target_name} arduino-logging)

  find_package(segger)
  target_link_libraries(${target_name} SeggerRTT)

  find_package(Wire)
  target_link_libraries(${target_name} Wire)

  find_package(SPI)
  target_link_libraries(${target_name} SPI)

  find_package(phylum)
  target_link_libraries(${target_name} phylum)
  target_compile_options(phylum PUBLIC -DPHYLUM_ENABLE_SERIAL_FLASH -DPHYLUM_ENABLE_SD)

  find_package(module-protocol)
  target_link_libraries(${target_name} module-protocol)

  find_package(app-protocol)
  target_link_libraries(${target_name} app-protocol)

  find_package(data-protocol)
  target_link_libraries(${target_name} data-protocol)

  find_package(simple-lora-comms)
  target_link_libraries(${target_name} simple-lora-comms)

  find_package(lwcron)
  target_link_libraries(${target_name} lwcron)

  find_package(nanopb)
  target_link_libraries(${target_name} nanopb)

  find_package(RTCZero)
  target_link_libraries(${target_name} RTCZero)

  find_package(RTClib)
  target_link_libraries(${target_name} RTClib)

  find_package(WiFi101)
  target_link_libraries(${target_name} WiFi101)

  find_package(RadioHead)
  target_link_libraries(${target_name} RadioHead)

  find_package(Adafruit_ASFcore)
  target_link_libraries(${target_name} Adafruit_ASFcore)

  find_package(AtSamd)
  target_link_libraries(${target_name} AtSamd)

  find_package(FuelGauge)
  target_link_libraries(${target_name} FuelGauge)

  find_package(TinyGPS)
  target_link_libraries(${target_name} TinyGPS)

  find_package(arduino-base64)
  target_link_libraries(${target_name} arduino-base64)

  find_package(SerialFlash)
  target_link_libraries(${target_name} SerialFlash)

  find_package(Adafruit_NeoPixel)
  target_link_libraries(${target_name} Adafruit_NeoPixel)

  target_include_directories(${target_name}
    PUBLIC ${${target_name}_PATH}/src
  )

  option(FK_ENABLE_RADIO "Enable/disable LoRa radio." OFF)
  if(FK_ENABLE_RADIO)
    target_compile_definitions(${target_name} PUBLIC -DFK_ENABLE_RADIO)
  else()
    target_compile_definitions(${target_name} PUBLIC -DFK_DISABLE_RADIO)
  endif(FK_ENABLE_RADIO)

  target_compile_definitions(${target_name} PUBLIC -DFK_CORE)
  target_compile_definitions(${target_name} PUBLIC -DFK_HARDWARE_SERIAL2_ENABLE)
endfunction()

function(fk_core_apply_configuration target_name)
  set(library_target firmware-common-${target_name})

  option(FK_DEBUG_UART_FALLBACK "On SerialUSB failure use Serial5" ON)
  if(FK_DEBUG_UART_FALLBACK)
    target_compile_options(${library_target} PUBLIC -DFK_DEBUG_UART_FALLBACK)
  endif(FK_DEBUG_UART_FALLBACK)

  option(FK_DEBUG_UART_REQUIRE_CONSOLE "Wait forever for a console connection" OFF)
  if(FK_DEBUG_UART_REQUIRE_CONSOLE)
    target_compile_options(${library_target} PUBLIC -DFK_DEBUG_UART_REQUIRE_CONSOLE)
  endif(FK_DEBUG_UART_REQUIRE_CONSOLE)

  option(FK_DEBUG_MTB_ENABLE "Enable MicroTrace Buffer" OFF)
  if(FK_DEBUG_MTB_ENABLE)
    target_compile_options(${library_target} PUBLIC -DFK_DEBUG_MTB_ENABLE)
  endif(FK_DEBUG_MTB_ENABLE)

  option(FK_API_BASE "Use given API_BASE")
  if(FK_API_BASE)
    target_compile_options(${library_target} PUBLIC -DFK_API_BASE="${API_BASE}")
  endif(FK_API_BASE)

  target_compile_options(${library_target} PUBLIC -DFK_CORE)
  target_compile_options(${library_target} PUBLIC -DFK_RTC_PCF8523)
  target_compile_options(${library_target} PUBLIC -DFK_HARDWARE_SERIAL2_ENABLE)

  option(FK_ENABLE_FLASH "Enable/disable flash memory." ON)
  if(FK_ENABLE_FLASH)
    target_compile_options(${library_target} PUBLIC -DFK_ENABLE_FLASH)
  else()
    target_compile_options(${library_target} PUBLIC -DFK_DISABLE_FLASH)
  endif(FK_ENABLE_FLASH)

  option(FK_ENABLE_FUEL_GAUGE "Enable fuel gauge." ON)
  if(FK_ENABLE_FUEL_GAUGE)
    target_compile_options(${library_target} PUBLIC -DFK_ENABLE_FUEL_GAUGE)
  else()
    target_compile_options(${library_target} PUBLIC -DFK_DISABLE_FUEL_GAUGE)
  endif(FK_ENABLE_FUEL_GAUGE)

  option(FK_ENABLE_LOW_POWER_SLEEP "Enable low power sleep." ON)
  if(FK_ENABLE_LOW_POWER_SLEEP)
    target_compile_options(${library_target} PUBLIC -DFK_ENABLE_LOW_POWER_SLEEP)
  else()
    target_compile_options(${library_target} PUBLIC -DFK_DISABLE_LOW_POWER_SLEEP)
  endif(FK_ENABLE_LOW_POWER_SLEEP)

  option(FK_ENABLE_DEEP_SLEEP "Enable deep sleep." ON)
  if(FK_ENABLE_DEEP_SLEEP)
    target_compile_options(${library_target} PUBLIC -DFK_ENABLE_DEEP_SLEEP)
  else()
    target_compile_options(${library_target} PUBLIC -DFK_DISABLE_DEEP_SLEEP)
  endif(FK_ENABLE_DEEP_SLEEP)

  option(FK_ENABLE_RADIO "Enable/disable LoRa radio." OFF)
  if(FK_ENABLE_RADIO)
    target_compile_options(${library_target} PUBLIC -DFK_ENABLE_RADIO)
  else()
    target_compile_options(${library_target} PUBLIC -DFK_DISABLE_RADIO)
  endif(FK_ENABLE_RADIO)

  option(FK_WIFI_ALWAYS_ON "If ON never turn off Wifi" OFF)
  if(FK_WIFI_ALWAYS_ON)
    target_compile_options(${library_target} PUBLIC -DFK_WIFI_ALWAYS_ON)
  endif(FK_WIFI_ALWAYS_ON)

  option(FK_WIFI_STARTUP_ONLY "Only run Wifi on startup" OFF)
  if(FK_WIFI_STARTUP_ONLY)
    target_compile_options(${library_target} PUBLIC -DFK_WIFI_STARTUP_ONLY)
  endif(FK_WIFI_STARTUP_ONLY)

  option(FK_CORE_REQUIRE_MODULES "Require module, restarting if none are found. " OFF)
  if(FK_CORE_REQUIRE_MODULES)
    target_compile_options(${library_target} PUBLIC -DFK_CORE_REQUIRE_MODULES)
  endif(FK_CORE_REQUIRE_MODULES)
endfunction()

function(fk_write_profile target_names)
  foreach(target_name ${target_names})
    list(APPEND target_names firmware-common-${target_name})
  endforeach()

  foreach(target_name ${target_names})
    get_target_property(options ${target_name} COMPILE_OPTIONS)
    if(NOT "${options}" STREQUAL "options-NOTFOUND")
      message(STATUS "Options: ${target_name}: ${options}")
    endif()
  endforeach()
endfunction()

function(fk_configure_core target_name)
  message(STATUS "Configuring FkCore: ${target_name}")

  fk_add_core_library(firmware-common-${target_name})

  target_link_libraries(${target_name} firmware-common-${target_name})
endfunction()
