cmake_minimum_required(VERSION 3.20)
project(Dx11_SC VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)

add_library(Dx11_SC SHARED)

target_sources(Dx11_SC PRIVATE main.cpp)

add_subdirectory(Utils)
add_subdirectory(Renderer)
add_subdirectory(Includes)

target_link_libraries(Dx11_SC PRIVATE minhook)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)