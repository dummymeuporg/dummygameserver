#include <iostream>
#include <fstream>

#include <boost/range/iterator_range.hpp>

#include "core/character.hpp"

#include "abstract_game_server.hpp"

#include "errors.hpp"

#include "game_session.hpp"
#include "game_session_state/manage_characters_state.hpp"

#include "protocol/outgoing_packet.hpp"

namespace GameSessionState {

ManageCharactersState::ManageCharactersState(
    std::shared_ptr<GameSession> gameSession
) : GameSessionState(gameSession)
{
}

void ManageCharactersState::resume() {
    // Nothing to do.
}

void ManageCharactersState::_answer(const Dummy::Protocol::OutgoingPacket& pkt)
{
    auto self(m_gameSession->shared_from_this());
    auto selfState(shared_from_this());
    boost::asio::async_write(
        m_gameSession->socket(),
        boost::asio::buffer(pkt.buffer(), pkt.size()),
        [self, selfState, this](boost::system::error_code ec,
                                std::size_t length)
        {
            if (!ec)
            {
                m_gameSession->next();
            }
        }
    );
}

void ManageCharactersState::onRead(Dummy::Protocol::IncomingPacket& pkt)
{
    // From here, the player has either created a character or selected one.
    std::uint8_t command;
    pkt >> command;

    std::cerr << "command: " << command << std::endl;
    switch(command) {
    case 1:
        _onCreateCharacter(pkt);
    break;
    default:
        // Send an error?
    break;
    }
}

void
ManageCharactersState::_onCreateCharacter(Dummy::Protocol::IncomingPacket& pkt)
{
    ::AbstractGameServer& svr(m_gameSession->gameServer());
    std::cerr << "Create character." << std::endl;
    std::string characterName;
    std::string skin;
    pkt >> characterName >> skin;
    std::cerr << "Name is " << characterName << std::endl;
    std::cerr << "Skin is " << skin << std::endl;

    try {
        svr.createCharacter(
            *m_gameSession->account(),
            characterName, 
            skin
        );
    } catch(const ::GameServerError& e) {
        std::cerr << "Could not create character: "
            << e.what() << std::endl;
    }
}

void
ManageCharactersState::_onSelectCharacter(Dummy::Protocol::IncomingPacket& pkt)
{

}

} // namespace GameSessionState
