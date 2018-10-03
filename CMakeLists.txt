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
target_include_directories(tipe PUBLIC include)

add_subdirectory(external/CWPack)
target_link_libraries(tipe PUBLIC cwpack)

find_package(LibUV REQUIRED)
if (${LibUV_FOUND})
    message(STATUS "Found LibUV ${LibUV_VERSION}, building UVW")
    add_subdirectory(external/uvw-1.11.1_libuv-v1.23)
    target_link_libraries(tipe PUBLIC ${LibUV_LIBRARY} uvw)
    target_include_directories(tipe PUBLIC ${LibUV_INCLUDE_DIR})
endif()

add_subdirectory(tests)
add_subdirectory(samples)