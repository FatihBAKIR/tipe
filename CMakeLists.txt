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


set(PROJECT_CMAKE_IN ${CMAKE_SOURCE_DIR}/cmake/in)
set(PROJECT_DEPS_DIR ${CMAKE_SOURCE_DIR}/deps)
set(LIBUV_DEPS_DIR ${PROJECT_DEPS_DIR}/libuv)

configure_file(${PROJECT_CMAKE_IN}/deps.in ${PROJECT_DEPS_DIR}/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" . WORKING_DIRECTORY ${PROJECT_DEPS_DIR})
execute_process(COMMAND ${CMAKE_COMMAND} --build . WORKING_DIRECTORY ${PROJECT_DEPS_DIR})
add_subdirectory(${LIBUV_DEPS_DIR})

add_subdirectory(external/uvw-1.11.1_libuv-v1.23)
target_link_libraries(tipe PUBLIC uv_a uvw)
target_include_directories(tipe PUBLIC ${LIBUV_DEPS_DIR}/include)

add_subdirectory(tests)
add_subdirectory(samples)
