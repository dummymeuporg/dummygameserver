#include "abstract_game_server.hpp"
#include "player.hpp"
#include "server_map.hpp"

ServerMap::ServerMap(::AbstractGameServer& abstractGameServer,
                     const Dummy::Core::Map& map)
    : m_abstractGameServer(abstractGameServer), m_map(map)
{
}
