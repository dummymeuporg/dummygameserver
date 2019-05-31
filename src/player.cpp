#include "server_map.hpp"
#include "player.hpp"

Player::Player(std::shared_ptr<::GameSession> gameSession,
               std::shared_ptr<Dummy::Core::Character> character)
    : m_gameSession(gameSession), m_character(character),
      m_serverMap(nullptr)
{

}

void Player::setServerMap(std::shared_ptr<::ServerMap> serverMap) {
    m_serverMap = serverMap;
}

void
Player::setPosition(const std::pair<std::uint16_t, std::uint16_t>& pos) {
    m_character->setPosition(pos); 
}

void Player::setPosition(std::uint16_t x, std::uint16_t y) {
    setPosition(std::pair<std::uint16_t, std::uint16_t>(x, y));
}
