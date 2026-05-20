cmake_minimum_required(VERSION 3.10)
project(DungeonCrawler)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SFML_ROOT "C:/SFML")
set(CMAKE_PREFIX_PATH "C:/SFML/lib/cmake/SFML" ${CMAKE_PREFIX_PATH})

find_package(SFML 3.0 COMPONENTS Graphics Window System REQUIRED)

add_executable(dungeon_crawler 
    src/main.cpp 
    src/game.cpp
)

target_link_libraries(dungeon_crawler SFML::Graphics SFML::Window SFML::System)

target_include_directories(dungeon_crawler PRIVATE include "C:/SFML/include")
