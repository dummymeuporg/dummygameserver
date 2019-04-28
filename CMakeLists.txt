cmake_minimum_required(VERSION 3.10.0)
project(dummygameserver)
include_directories(include)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} \
    -lboost_filesystem -lboost_system -lboost_log -lpthread")

add_executable(dummygameserver
    src/main.cpp
)
