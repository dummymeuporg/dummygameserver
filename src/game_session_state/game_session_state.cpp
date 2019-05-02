#include "game_session.hpp"

#include "game_session_state/game_session_state.hpp"

namespace GameSessionState {

GameSessionState::GameSessionState(::GameSession& gameSession)
    : m_gameSession(gameSession)
{
}


} // namespace GameSessionState
