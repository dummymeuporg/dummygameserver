#include <iostream>
#include <fstream>

#include <boost/range/iterator_range.hpp>

#include "core/character.hpp"
#include "protocol/outgoing_packet.hpp"

#include "abstract_game_server.hpp"
#include "errors.hpp"
#include "game_session.hpp"
#include "game_session_state/loading_state.hpp"
#include "game_session_state/manage_characters_state.hpp"
#include "player.hpp"


namespace GameSessionState {

ManageCharactersState::ManageCharactersState(
    std::shared_ptr<GameSession> gameSession,
    std::vector<std::shared_ptr<Dummy::Core::Character>> characters
) : GameSessionState(gameSession), m_characters(std::move(characters))
{
    // Build the map for look-up.
    for (auto chr: m_characters) {
        m_charactersMap[chr->name()] = chr;
    }
}

void ManageCharactersState::resume() {
    // Nothing to do.
}

void ManageCharactersState::_answerOnCreateCharacter(
    const Dummy::Protocol::OutgoingPacket& pkt
)
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

    std::cerr << "command: " << static_cast<int>(command) << std::endl;
    switch(command) {
    case 1:
        _onCreateCharacter(pkt);
    case 2:
        _onSelectCharacter(pkt);
    break;
    default:
        // Send an error?
    break;
    }
}

void
ManageCharactersState::_onCreateCharacter(Dummy::Protocol::IncomingPacket& pkt)
{
    Dummy::Protocol::OutgoingPacket outPkt;
    std::uint8_t answer;
    // For now, only 5 characters at most.
    std::cerr << "Characters count: " << m_characters.size() << std::endl;
    if (m_characters.size() >= 5)
    {
        std::cerr << "Too much characters." << std::endl;
        answer = 0;
        outPkt << answer;
    } else {
        ::AbstractGameServer& svr(m_gameSession->gameServer());
        std::cerr << "Create character." << std::endl;
        std::string characterName;
        std::string skin;
        pkt >> characterName >> skin;
        std::cerr << "Name is " << characterName << std::endl;
        std::cerr << "Skin is " << skin << std::endl;

        try {
            Dummy::Core::Character chr(
                std::move(
                    svr.createCharacter(
                        *m_gameSession->account(),
                        characterName, 
                        skin
                    )
                )
            );
            answer = 1;
            outPkt << answer << chr;
        } catch(const ::GameServerError& e) {
            std::cerr << "Could not create character: "
                << e.what() << std::endl;
            answer = 0;
            outPkt << answer;
        }
    }
    _answerOnCreateCharacter(outPkt);
}

void
ManageCharactersState::_onSelectCharacter(Dummy::Protocol::IncomingPacket& pkt)
{
    Dummy::Protocol::OutgoingPacket outPkt;
    std::uint8_t answer;
    std::string characterName;
    std::cerr << "Select a character." << std::endl;
    pkt >> characterName;
    std::cerr << "Selected character: " << characterName << std::endl;

    if (m_charactersMap.find(characterName) == std::end(m_charactersMap)) {
        std::cerr << "Character not found." << std::endl;
        answer = 0;
        outPkt << answer;
        _answerFailOnSelectCharacter(outPkt);
    } else {
        /* O.K. */
        answer = 2;
        outPkt << answer;

        std::shared_ptr<::Player> player = std::make_shared<::Player>(
            m_gameSession, m_charactersMap.at(characterName)
        );

        std::cerr << "Set player..." << std::endl;
        m_gameSession->setPlayer(player);

        _answerSuccessOnSelectCharacter(outPkt);
    }
}

void ManageCharactersState::_answerFailOnSelectCharacter(
    const Dummy::Protocol::OutgoingPacket& pkt
) {
    auto self(shared_from_this());
    boost::asio::async_write(
        m_gameSession->socket(),
        boost::asio::buffer(pkt.buffer(), pkt.size()),
        [self, this](boost::system::error_code ec,
                     std::size_t lenght)
        {
            if (!ec)
            {
                std::cerr << "Choose another character." << std::endl;
                m_gameSession->next();
            }
        }
    );
}

void ManageCharactersState::_answerSuccessOnSelectCharacter(
    const Dummy::Protocol::OutgoingPacket& pkt)
{
    auto self(shared_from_this());
    boost::asio::async_write(
        m_gameSession->socket(),
        boost::asio::buffer(pkt.buffer(), pkt.size()),
        [self, this](boost::system::error_code ec,
                     std::size_t lenght)
        {
            if (!ec)
            {
                std::shared_ptr<Dummy::Core::Character> chr =
                    m_gameSession->player()->character();
                std::cerr << "Character chosen. Let's play!" << std::endl;
                // toggle loading state with a "teleport request"
                m_gameSession->changeState(
                    std::make_shared<LoadingState>(
                        m_gameSession,
                        Dummy::Protocol::TeleportRequest(
                            chr->mapLocation(),
                            chr->position()
                        )
                    )
                );
                m_gameSession->next();
            }
        }
    );


}

} // namespace GameSessionState
