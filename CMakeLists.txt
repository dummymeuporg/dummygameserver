cmake_minimum_required(VERSION 3.10.0)
project(dummygameserver)
include_directories(include vendor/libdummy/include)

set (CMAKE_CXX_STANDARD 17)
add_subdirectory("vendor/libdummy")

find_package(Boost 1.69.0 REQUIRED)
link_directories(${Boost_LIBRARY_DIRS})

file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/*.hpp")

source_group("include" FILES ${HEADERS} RELATIVE ${CMAKE_CURRENT_SOURCE_DIR})
source_group("src" FILES ${SOURCES} RELATIVE ${CMAKE_CURRENT_SOURCE_DIR})

set(CLIENT_LINK_LIBRARIES
    ${Boost_LIBRARIES}
    dummy
)

# On Linux, explicitly pass libraries.
if (UNIX)
    set(CLIENT_LINK_LIBRARIES
        ${CLIENT_LINK_LIBRARIES}
	boost_system
        boost_filesystem
        stdc++fs
	pthread
    )

endif (UNIX)


add_executable(dummygameserver ${SOURCES} ${HEADERS})

target_include_directories(dummygameserver PUBLIC ${Boost_INCLUDE_DIR})
target_link_libraries(dummygameserver
    dummy
    ${Boost_LIBRARIES}
    ${CLIENT_LINK_LIBRARIES}
)
