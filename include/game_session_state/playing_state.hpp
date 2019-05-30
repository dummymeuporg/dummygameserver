#pragma once

#include "game_session.hpp"
#include "game_session_state/game_session_state.hpp"
#include "protocol/outgoing_packet.hpp"
#include "protocol/living.hpp"

namespace GameSessionState {

class PlayingState : public GameSessionState {
public:
    PlayingState(std::shared_ptr<GameSession>);
    virtual void onRead(Dummy::Protocol::IncomingPacket&) override;
    virtual void resume() override;
private:
    void _onMove(Dummy::Protocol::IncomingPacket&);
    void _answer(const Dummy::Protocol::OutgoingPacket&);
    void _updateLivings(std::shared_ptr<::Player> player,
                        std::shared_ptr<::ServerMap> map,
                        Dummy::Protocol::OutgoingPacket& pkt);

    std::map<std::string, Dummy::Protocol::Living> m_livings;
};

} // namespace GameSessionState
