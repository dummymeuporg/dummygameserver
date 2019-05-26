#pragma once

#include "core/character.hpp"
#include "game_session.hpp"

class Player {
public:
    Player(std::shared_ptr<::GameSession>,
           std::shared_ptr<Dummy::Core::Character>);

    std::shared_ptr<::GameSession> gameSession() {
        return m_gameSession;
    }

    std::shared_ptr<Dummy::Core::Character> character() const {
        return m_character;
    }

private:
    std::shared_ptr<::GameSession> m_gameSession;
    std::shared_ptr<Dummy::Core::Character> m_character;

};
