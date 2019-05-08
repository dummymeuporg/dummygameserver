cmake_minimum_required(VERSION 3.10.0)
project(dummygameserver)
include_directories(include vendor/libdummy/include)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} \
	-L${CMAKE_CURRENT_SOURCE_DIR}/vendor/libdummy/build \
	-lboost_filesystem -lboost_system -lboost_log -lpthread -lssl -lcrypto")

add_executable(dummygameserver
    src/main.cpp
    src/master_client.cpp
    src/abstract_game_server.cpp
    src/game_session.cpp
    src/test_game_server.cpp
	src/game_session_state/game_session_state.cpp
	src/game_session_state/initial_state.cpp
    src/master_client_state/initial_state.cpp
    src/master_client_state/master_client_state.cpp
)

target_link_libraries(dummygameserver dummy)
