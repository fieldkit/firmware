find_package(Nanopb REQUIRED)

add_executable(example main.cpp)

include_directories(../)

target_link_libraries(example fk_data_protocol)
