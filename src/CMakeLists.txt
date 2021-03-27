file(GLOB SOURCES *.c *.cpp)
file(GLOB INCLUDES *.h)

find_package(Nanopb REQUIRED)

add_library(fk_data_protocol STATIC ${SOURCES} ${NANOPB_SRCS})

include_directories(${NANOPB_INCLUDE_DIRS})

install(TARGETS fk_data_protocol DESTINATION lib)
install(FILES ${INCLUDES} DESTINATION include)

add_subdirectory(example)
