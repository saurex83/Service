cmake_minimum_required(VERSION 2.8.9)
project (NeocoreService)

# Флаги компиляции
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -O3 ")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
#-D_GLIBCXX_USE_CXX11_ABI=0
include_directories(inc)

add_subdirectory(src)
add_subdirectory(tests)
