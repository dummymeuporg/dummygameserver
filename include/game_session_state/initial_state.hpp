#pragma once

#include "game_session_state/game_session_state.hpp"

namespace GameSessionState {

class InitialState {
public:
    InitialState(std::shared_ptr<GameSession>);
    virtual void onRead(const std::vector<std::uint8_t>&) override;
};

} // namespace GameSessionState
