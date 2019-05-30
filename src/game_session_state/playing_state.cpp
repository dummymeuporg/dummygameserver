#include "game_session.hpp"
#include "game_session_state/playing_state.hpp"

namespace GameSessionState {

PlayingState::PlayingState(std::shared_ptr<GameSession> gameSession)
    : GameSessionState(gameSession)
{
}

void PlayingState::resume() {

}

void PlayingState::onRead(Dummy::Protocol::IncomingPacket& pkt) {

}

} // namespace GameSessionState
