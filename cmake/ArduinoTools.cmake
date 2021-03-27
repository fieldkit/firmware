if (NOT DEFINED ARDUINO_IDE)
  foreach(path $ENV{HOME}/arduino-1.8.3 $ENV{HOME}/conservify/arduino-1.8.3
               $ENV{HOME}/workspace/arduino-1.8.3
               ${PROJECT_SOURCE_DIR}/../arduino-1.8.3 ${PROJECT_SOURCE_DIR}/../../arduino-1.8.3)
      if (EXISTS ${path})
        set(ARDUINO_IDE ${path})
        break()
    endif()
  endforeach()

  if (NOT DEFINED ARDUINO_IDE)
    message(FATAL_ERROR "Unable to find Arduino IDE")
  endif()
endif()

set(ARDUINO_PACKAGES_PATH "${ARDUINO_IDE}/packages")
set(ARDUINO_TOOLS_PATH "${ARDUINO_PACKAGES_PATH}/arduino/tools")
set(ARM_TOOLS "${ARDUINO_TOOLS_PATH}/arm-none-eabi-gcc/4.8.3-2014q1/bin")

set(ARDUINO_CMSIS_DIRECTORY "${ARDUINO_TOOLS_PATH}/CMSIS/4.5.0/CMSIS")
set(ARDUINO_CMSIS_INCLUDE_DIRECTORY "${ARDUINO_CMSIS_DIRECTORY}/Include/")
set(ARDUINO_DEVICE_DIRECTORY "${ARDUINO_TOOLS_PATH}/CMSIS-Atmel/1.2.0/CMSIS/Device/ATMEL")

set(ARDUINO_OBJCOPY "${ARM_TOOLS}/arm-none-eabi-objcopy")
set(ARDUINO_NM "${ARM_TOOLS}/arm-none-eabi-nm")

set(CMAKE_C_COMPILER ${ARM_TOOLS}/arm-none-eabi-gcc)
set(CMAKE_ASM_COMPILER ${ARM_TOOLS}/arm-none-eabi-gcc)

enable_language(ASM)
