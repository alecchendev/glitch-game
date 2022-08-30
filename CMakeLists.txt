cmake_minimum_required(VERSION 3.11)
set(CMAKE_CXX_STANDARD 11)
project(glitch_game)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(glitch_game src/main.cpp)
target_link_libraries(glitch_game ${CONAN_LIBS})
target_include_directories(glitch_game PRIVATE include)
