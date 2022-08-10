cmake_minimum_required(VERSION 3.0)
project(largo_game)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(largo_game src/main.cpp)
target_link_libraries(largo_game ${CONAN_LIBS})
