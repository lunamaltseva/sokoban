cmake_minimum_required(VERSION 3.26.4)
project(Catastrophic)

set(CMAKE_CXX_STANDARD 17)

find_package(raylib CONFIG REQUIRED)
if(APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
elseif(UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -fsanitize=address -fsanitize=undefined")
endif()

add_executable(project01 sokoban.cpp globals.h draw.h assets.h levels.h player.h lunalib.h run.h)
target_link_libraries(project01 PRIVATE raylib)
