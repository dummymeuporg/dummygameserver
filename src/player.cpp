#include "player.hpp"

Player::Player(std::shared_ptr<::GameSession> gameSession,
               std::shared_ptr<Dummy::Core::Character> character)
    : m_gameSession(gameSession), m_character(character)
{

}
