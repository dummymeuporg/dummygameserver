#pragma once

#include <cstdint>
#include <memory>
#include <vector>

class GameSession;

namespace GameSessionState {

class GameSessionState {
public:
    GameSessionState(std::shared_ptr<GameSession>);
    virtual void onRead(const std::vector<std::uint8_t>&) = 0;
protected:
    std::shared_ptr<GameSession> m_gameSession;
};

} // namespace GameSessionState
