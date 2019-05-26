#pragma once

#include "protocol/outgoing_packet.hpp"
#include "protocol/teleport_request.hpp"
#include "game_session_state/game_session_state.hpp"

namespace GameSessionState {

class LoadingState : public GameSessionState {
public:
    LoadingState(std::shared_ptr<GameSession>,
                 Dummy::Protocol::TeleportRequest&&);
    virtual void onRead(Dummy::Protocol::IncomingPacket&) override;
    virtual void resume() override;
private:
    void _answerSuccess(const Dummy::Protocol::OutgoingPacket&);
    Dummy::Protocol::TeleportRequest m_teleportRequest;
};

} // namespace GameSessionState
