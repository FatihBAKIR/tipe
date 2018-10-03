cmake_minimum_required(VERSION 3.5)
project(typeflow)

set(CMAKE_CXX_STANDARD 17)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

cmake_policy(SET CMP0069 NEW)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)

add_library(tipe
    src/stub.cpp
    include/tipe/tipe.hpp
    include/tipe/nodes/split.hpp
    include/tipe/graph.hpp
    include/tipe/meta/list.hpp
    include/tipe/nodes/common.hpp
    include/tipe/nodes/constant.hpp
    include/tipe/node.hpp
    include/tipe/edge.hpp
    include/tipe/node_traits.hpp
    include/tipe/execution.hpp
    include/tipe/nodes/print.hpp
    include/tipe/nodes/msgpack_to_json.hpp
    include/tipe/nodes/tcp.hpp
    include/tipe/nodes/timer.hpp
    include/tipe/nodes/aggregate.hpp)

find_package(Threads REQUIRED)
target_include_directories(tipe PUBLIC include)
target_link_libraries(tipe PUBLIC ${CMAKE_THREAD_LIBS_INIT})

add_subdirectory(external/CWPack)
target_link_libraries(tipe PUBLIC cwpack)

include(ExternalProject)
ExternalProject_Add(libuv
    GIT_REPOSITORY https://github.com/libuv/libuv.git
    GIT_TAG        v1.23.1
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/libuv
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR> -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
)

ExternalProject_Get_Property(libuv install_dir)

add_library(LibUV::LibUV STATIC IMPORTED)
set_property(TARGET LibUV::LibUV PROPERTY IMPORTED_LOCATION ${install_dir}/lib/libuv_a.a)
set_property(TARGET LibUV::LibUV PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${install_dir}/include)
add_subdirectory(external/uvw-1.11.1_libuv-v1.23)
target_link_libraries(tipe PUBLIC LibUV::LibUV uvw)

add_subdirectory(tests)
add_subdirectory(samples)
