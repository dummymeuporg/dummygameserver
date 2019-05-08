#pragma once

#include "game_session_state/game_session_state.hpp"

namespace GameSessionState {

class SendCharactersState : public GameSessionState {
public:
    SendCharactersState(std::shared_ptr<GameSession>);
    virtual void resume() override;
    virtual void onRead(const std::vector<std::uint8_t>&) override;
};

} // namespace GameSessionState
