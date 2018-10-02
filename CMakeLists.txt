cmake_minimum_required(VERSION 3.5)
project(typeflow)

set(CMAKE_CXX_STANDARD 17)

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
        include/tipe/nodes/msgpack_to_json.hpp)
target_include_directories(tipe PUBLIC include)

add_executable(typeflow src/main.cpp)
target_link_libraries(typeflow tipe pthread cwpack)

add_subdirectory(CWPack)

add_subdirectory(tests)
