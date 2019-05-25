#pragma once

#include "game_session_state/game_session_state.hpp"
#include "protocol/outgoing_packet.hpp"

namespace GameSessionState {

using CharactersList =
    std::vector<std::shared_ptr<Dummy::Core::Character>>;
class ManageCharactersState : public GameSessionState {
public:
    ManageCharactersState(
        std::shared_ptr<GameSession>,
        CharactersList&&);
    virtual void resume() override;
    virtual void onRead(Dummy::Protocol::IncomingPacket&) override;
private:
    CharactersList m_characters;
    void _onCreateCharacter(Dummy::Protocol::IncomingPacket&);
    void _answerOnCreateCharacter(const Dummy::Protocol::OutgoingPacket& pkt);
    void _onSelectCharacter(Dummy::Protocol::IncomingPacket&);
    void _answerOnSelectCharacter(const Dummy::Protocol::OutgoingPacket& pkt);
};

};
