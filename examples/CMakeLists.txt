#
#
#

include(Arduino)

enable_arduino_toolchain()

enable_language(ASM)

if(TARGET_M4)
  enable_m4()
endif()

if(TARGET_M0)
  enable_m0()
endif()

if(TARGET_M0 OR TARGET_M4)

file(GLOB osh_sources ../src/*.cpp ../src/*.c ../src/*.s ../src/*.h ../src/segger/*.c)

file(GLOB three_sources three.cpp)
add_arduino_library(osh-three "${osh_sources};${three_sources}")
target_compile_options(osh-three PRIVATE -Wall -Werror)
target_include_directories(osh-three PUBLIC ./ ../src)
add_arduino_firmware(osh-three)

file(GLOB reuse_sources reuse.cpp)
add_arduino_library(osh-reuse "${osh_sources};${reuse_sources}")
target_compile_options(osh-reuse PRIVATE -Wall -Werror)
target_include_directories(osh-reuse PUBLIC ./ ../src)
add_arduino_firmware(osh-reuse)

file(GLOB queues_sources queues.cpp)
add_arduino_library(osh-queues "${osh_sources};${queues_sources}")
target_compile_options(osh-queues PRIVATE -Wall -Werror)
target_include_directories(osh-queues PUBLIC ./ ../src)
add_arduino_firmware(osh-queues)

file(GLOB mutexes_sources mutexes.cpp)
add_arduino_library(osh-mutexes "${osh_sources};${mutexes_sources}")
target_compile_options(osh-mutexes PRIVATE -Wall -Werror)
target_include_directories(osh-mutexes PUBLIC ./ ../src)
add_arduino_firmware(osh-mutexes)

file(GLOB panic_sources panic.cpp)
add_arduino_library(osh-panic "${osh_sources};${panic_sources}")
target_compile_options(osh-panic PRIVATE -Wall -Werror)
target_include_directories(osh-panic PUBLIC ./ ../src)
add_arduino_firmware(osh-panic)

file(GLOB everything_sources everything.cpp)
add_arduino_library(osh-everything "${osh_sources};${everything_sources}")
target_compile_options(osh-everything PRIVATE -Wall -Werror)
target_include_directories(osh-everything PUBLIC ./ ../src)
add_arduino_firmware(osh-everything)

file(GLOB rwlock_sources rwlock.cpp)
add_arduino_library(osh-rwlock "${osh_sources};${rwlock_sources}")
target_compile_options(osh-rwlock PRIVATE -Wall -Werror)
target_include_directories(osh-rwlock PUBLIC ./ ../src)
add_arduino_firmware(osh-rwlock)

endif()
