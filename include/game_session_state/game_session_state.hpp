#pragma once

#include <cstdint>
#include <vector>

class GameSession;

namespace GameSessionState {

class GameSessionState {
public:
    GameSessionState(::GameSession&);
    virtual void onRead(const std::vector<std::uint8_t>&) = 0;
protected:
    ::GameSession& m_gameSession;
};

} // namespace GameSessionState
