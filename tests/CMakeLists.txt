
project(tos)

add_library(catch_main catch_main.cpp)
target_compile_features(catch_main PUBLIC cxx_std_14)
target_compile_options(catch_main PUBLIC -fexceptions)

add_executable(basic_tests basic_tests.cpp)
target_link_libraries(basic_tests catch_main tipe)

add_executable(msgpack_json_tests msgpack_json_tests.cpp)
target_link_libraries(msgpack_json_tests catch_main tipe cwpack)