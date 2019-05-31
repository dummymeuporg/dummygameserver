cmake_minimum_required(VERSION 3.10.0)
project(dummygameserver)
include_directories(include vendor/libdummy/include)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} \
	-L${CMAKE_CURRENT_SOURCE_DIR}/vendor/libdummy/build \
	-lboost_filesystem -lboost_system -lboost_log -lpthread -lssl -lcrypto")

set (CMAKE_CXX_STANDARD 17)
add_subdirectory("vendor/libdummy")

file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/*.hpp")

add_executable(dummygameserver ${SOURCES} ${INCLUDE})

target_link_libraries(dummygameserver dummy)
