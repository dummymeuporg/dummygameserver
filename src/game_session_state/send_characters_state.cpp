#include <iostream>

#include <boost/range/iterator_range.hpp>

#include "core/character.hpp"

#include "abstract_game_server.hpp"

#include "game_session.hpp"
#include "game_session_state/send_characters_state.hpp"

namespace GameSessionState {

SendCharactersState::SendCharactersState(
    std::shared_ptr<GameSession> gameSession
) : GameSessionState(gameSession)
{
}

void SendCharactersState::resume() {
    // Send the list of characters.
    const ::AbstractGameServer& svr(m_gameSession->gameServer());
    fs::path accountPath(svr.serverPath() /
                         "accounts" / m_gameSession->account()->name());
    std::vector<std::shared_ptr<Dummy::Core::Character>> characters;
    std::vector<std::uint8_t> buffer(sizeof(std::uint16_t) * 2);
    std::uint16_t charactersCount = 0;
    for (const auto& entry: boost::make_iterator_range(
                fs::directory_iterator(accountPath))) {
        std::cerr << "found " << entry << std::endl;

        // TODO: load character.
    }
    std::cerr << "There are " << characters.size() << " characters."
        << std::endl;

    *(reinterpret_cast<std::uint16_t*>(buffer.data())) =
        buffer.size() - sizeof(std::uint16_t);
    *(reinterpret_cast<std::uint16_t*>(buffer.data()) + 1) = charactersCount;
    _answer(buffer);
}

void SendCharactersState::_answer(const std::vector<std::uint8_t>& buffer) {
    auto self(m_gameSession->shared_from_this());
    auto selfState(shared_from_this());
    boost::asio::async_write(
        m_gameSession->socket(),
        boost::asio::buffer(buffer, buffer.size()),
        [self, selfState, this](boost::system::error_code ec,
                                std::size_t length)
        {
            if (!ec)
            {
                // wait on the onRead function?
            }
        }
    );
}

void SendCharactersState::onRead(const std::vector<std::uint8_t>& buffer) {
    // From here, the player has either created a character or selected one.
}

} // namespace GameSessionState
