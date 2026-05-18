cmake_minimum_required(VERSION 3.10)
project(DungeonCrawler)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
    src/main.cpp
    src/game.cpp
)

add_executable(dungeon_crawler ${SOURCES})

target_include_directories(dungeon_crawler PRIVATE include)