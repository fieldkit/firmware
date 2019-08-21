
#
#
#
if(TARGET SdFat)
  return()
endif()

file(GLOB_RECURSE sources ${SdFat_PATH}/src/*.cpp ${SdFat_PATH}/src/*.c)

add_arduino_library(SdFat "${sources}")

target_include_directories(SdFat PUBLIC ${SdFat_PATH}/src)

find_package(SPI)
target_link_libraries(SdFat SPI)
