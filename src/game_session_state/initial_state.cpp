#include "game_session.hpp"

#include "game_session_state/initial_state.hpp"

namespace GameSessionState {

InitialState::InitialState(std::shared_ptr<GameSession> gameSession)
    GameSessionState(gameSession)
{
}

} // namespace GameSessionState
