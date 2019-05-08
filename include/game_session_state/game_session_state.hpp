#pragma once

#include <cstdint>
#include <memory>
#include <vector>


class GameSession;

namespace GameSessionState {

class GameSessionState : public std::enable_shared_from_this<GameSessionState>
{
public:
    GameSessionState(std::shared_ptr<GameSession>);
    virtual void resume() = 0;
    virtual void onRead(const std::vector<std::uint8_t>&) = 0;
protected:
    std::shared_ptr<GameSession> m_gameSession;
};

} // namespace GameSessionState
