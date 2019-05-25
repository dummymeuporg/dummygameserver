#pragma once

#include "game_session_state/game_session_state.hpp"
#include "protocol/outgoing_packet.hpp"

namespace GameSessionState {

class ManageCharactersState : public GameSessionState {
public:
    ManageCharactersState(std::shared_ptr<GameSession>);
    virtual void resume() override;
    virtual void onRead(Dummy::Protocol::IncomingPacket&) override;
private:
    void _answer(const Dummy::Protocol::OutgoingPacket& pkt);
    void _onCreateCharacter(Dummy::Protocol::IncomingPacket&);
    void _onSelectCharacter(Dummy::Protocol::IncomingPacket&);
};

};
