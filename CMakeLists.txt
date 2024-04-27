cmake_minimum_required(VERSION 3.22)
project(game_of_life)

set(CMAKE_CXX_STANDARD 17)
add_compile_options(-fsanitize=address,undefined)
add_link_options(-fsanitize=address,undefined)
add_executable(game_of_life main.cpp Handler.h)
target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-window sfml-system)