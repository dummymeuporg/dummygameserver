#include <iostream>

#include <boost/range/iterator_range.hpp>

#include "core/character.hpp"

#include "abstract_game_server.hpp"

#include "errors.hpp"

#include "game_session.hpp"
#include "game_session_state/send_characters_state.hpp"

#include "protocol/outgoing_packet.hpp"

namespace GameSessionState {

SendCharactersState::SendCharactersState(
    std::shared_ptr<GameSession> gameSession
) : GameSessionState(gameSession)
{
}

void SendCharactersState::resume() {
    // Send the list of characters.
    const ::AbstractGameServer& svr(m_gameSession->gameServer());
    fs::path accountPath(svr.serverPath()
        / "accounts" / m_gameSession->account()->name() / "characters");
    std::vector<std::shared_ptr<Dummy::Core::Character>> characters;

    Dummy::Protocol::OutgoingPacket pkt;

    for (const auto& entry: boost::make_iterator_range(
                fs::directory_iterator(accountPath))) {
        std::cerr << "found " << entry << std::endl;
        characters.push_back(nullptr);

        // TODO: load character.
    }
    std::cerr << "There are " << characters.size() << " characters."
        << std::endl;

    pkt << static_cast<std::uint16_t>(characters.size());
    _answer(pkt);
}

void SendCharactersState::_answer(const Dummy::Protocol::OutgoingPacket& pkt) {
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

void SendCharactersState::onRead(Dummy::Protocol::IncomingPacket& pkt) {
    // From here, the player has either created a character or selected one.
    ::AbstractGameServer& svr(m_gameSession->gameServer());
    std::uint8_t command;
    pkt >> command;

    std::cerr << "command: " << command << std::endl;
    if (command == 1) {
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
}

} // namespace GameSessionState
