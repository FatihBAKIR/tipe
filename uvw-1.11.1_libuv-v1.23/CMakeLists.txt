#
# uvw
#

cmake_minimum_required(VERSION 3.2)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#
# CMake configuration
#

set(PROJECT_SRC_DIR ${uvw_SOURCE_DIR}/src)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)

#
# Enable test support using ctest-like interface
#

option(BUILD_TESTING "Enable testing with ctest." OFF)

#
# Referenced packages
#

set(THREADS_PREFER_PTHREAD_FLAG ON)

include(FindThreads)
find_package(Doxygen 1.8)

add_library(uvw INTERFACE)
target_include_directories(uvw INTERFACE "src")
