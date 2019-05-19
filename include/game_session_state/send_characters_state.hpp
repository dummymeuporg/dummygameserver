#pragma once

#include "game_session_state/game_session_state.hpp"
#include "protocol/outgoing_packet.hpp"

namespace GameSessionState {

class SendCharactersState : public GameSessionState {
public:
    SendCharactersState(std::shared_ptr<GameSession>);
    virtual void resume() override;
    //virtual void onRead(const std::vector<std::uint8_t>&) override;
    virtual void onRead(Dummy::Protocol::IncomingPacket&) override;
private:
    void _answer(const Dummy::Protocol::OutgoingPacket&);
};

} // namespace GameSessionState
