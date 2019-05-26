#include <iostream>
#include <fstream>

#include <boost/range/iterator_range.hpp>

#include "core/character.hpp"

#include "abstract_game_server.hpp"

#include "errors.hpp"

#include "game_session.hpp"
#include "game_session_state/manage_characters_state.hpp"
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
        std::shared_ptr<Dummy::Core::Character> chr =
            std::make_shared<Dummy::Core::Character>();
        std::ifstream ifs(entry.path().string(),
                          std::ios::binary | std::ofstream::out);
        ifs >> *chr;
        characters.push_back(chr);
    }
    std::cerr << "There are " << characters.size() << " characters."
        << std::endl;

    pkt << static_cast<std::uint16_t>(characters.size());
    for (const auto& chr: characters) {
        std::cerr << "Name: " << chr->name() << std::endl;
        pkt << *chr;
    }
    _answer(pkt, std::move(characters));
}

void SendCharactersState::_answer(
    const Dummy::Protocol::OutgoingPacket& pkt,
    CharactersList characters
) {
    auto self(m_gameSession->shared_from_this());
    auto selfState(shared_from_this());
                std::cerr << "There are " << characters.size() << " characters."
                    << std::endl;
    boost::asio::async_write(
        m_gameSession->socket(),
        boost::asio::buffer(pkt.buffer(), pkt.size()),
        [self, selfState, characters, this](boost::system::error_code ec,
                                            std::size_t length)
        {
            if (!ec)
            {
                std::cerr << "There are " << characters.size() <<
                    " characters." << std::endl;
                m_gameSession->changeState(
                    std::make_shared<ManageCharactersState>(
                        self,
                        std::move(characters)
                    )
                );
                // We wait for the client's input.
                m_gameSession->next();
            }
        }
    );
}

void SendCharactersState::onRead(Dummy::Protocol::IncomingPacket& pkt) {
    // Nothing to read from here.
}

} // namespace GameSessionState
