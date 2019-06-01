#include "abstract_game_server.hpp"
#include "player.hpp"
#include "server_map.hpp"

ServerMap::ServerMap(::AbstractGameServer& abstractGameServer,
                     const Dummy::Core::Map& map)
    : m_abstractGameServer(abstractGameServer), m_map(map)
{
}

void ServerMap::addPlayer(std::shared_ptr<::Player> player) {
    if (m_players.find(player->name()) == std::end(m_players)) {
        m_players[player->name()] = player;
    }
}

void ServerMap::removePlayer(std::shared_ptr<::Player> player) {
    if (m_players.find(player->name()) != std::end(m_players)) {
        std::cerr << "Bye bye " << player->name() << std::endl;
        m_players.erase(player->name());
    }
}

bool ServerMap::isBlocking(std::uint16_t x, std::uint16_t y) const {
    return m_map.isBlocking(x, y);
}
