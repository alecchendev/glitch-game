cmake_minimum_required(VERSION 3.0)
project(glitch_game)

add_executable(main src/main.cpp)

# External dependencies from vcpkg
find_package(glad CONFIG REQUIRED)
target_link_libraries(main PRIVATE glad::glad)

find_path(STB_INCLUDE_DIRS "stb_c_lexer.h")
target_include_directories(main PRIVATE ${STB_INCLUDE_DIRS})

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(main PRIVATE glfw)

find_package(glm CONFIG REQUIRED)
target_link_libraries(main PRIVATE glm::glm)
