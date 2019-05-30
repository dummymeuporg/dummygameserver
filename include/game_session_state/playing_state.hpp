#pragma once

#include "game_session_state/game_session_state.hpp"

namespace GameSessionState {

class PlayingState : public GameSessionState {
public:
    PlayingState(std::shared_ptr<GameSession>);
    virtual void onRead(Dummy::Protocol::IncomingPacket&) override;
    virtual void resume() override;
};

} // namespace GameSessionState
