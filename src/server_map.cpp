#include "abstract_game_server.hpp"
#include "player.hpp"
#include "server_map.hpp"

ServerMap::ServerMap(::AbstractGameServer& abstractGameServer,
                     const Dummy::Core::Map& map)
    : m_abstractGameServer(abstractGameServer), m_map(map)
{
}

void ServerMap::addPlayer(std::shared_ptr<::Player> player) {
    if (m_players.find(player) == std::end(m_players)) {
        m_players.insert(player);
    }
}

void ServerMap::removePlayer(std::shared_ptr<::Player> player) {
    if (m_players.find(player) != std::end(m_players)) {
        m_players.erase(player);
    }
}
