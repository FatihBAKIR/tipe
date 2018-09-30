
project(tos)

add_library(catch_main catch_main.cpp)
target_compile_features(catch_main PUBLIC cxx_std_14)

add_executable(basic_tests basic_tests.cpp)
target_link_libraries(basic_tests catch_main tipe)
target_compile_options(basic_tests PUBLIC -fexceptions)
