#include <iostream>
#include <boost/uuid/uuid_io.hpp>

#include "abstract_game_server.hpp"
#include "game_session.hpp"
#include "game_session_state/initial_state.hpp"
#include "game_session_state/send_characters_state.hpp"

namespace GameSessionState {

InitialState::InitialState(std::shared_ptr<GameSession> gameSession)
    : GameSessionState(gameSession)
{
    std::cerr << "Initial state instantiated." << std::endl;
}

void InitialState::resume() {
    // Nothing to do.
}

void InitialState::onRead(const std::vector<std::uint8_t>& buffer) {
    std::cerr << "Received " << buffer.size() << " bytes." << std::endl;
    // The buffer should contain an account name and an uuid.
    if (buffer.size() < 16 + 2)
    {
        // Throw an exception?
        return;
    }
    const std::uint16_t* accountLength =
        reinterpret_cast<const std::uint16_t*>(buffer.data());

    // Check against the accountLength
    if (*accountLength > buffer.size() - (16 + 2)) {
        // Throm an exception?
        return;
    }

    std::string accountName(
        reinterpret_cast<const char*>(buffer.data()) + sizeof(std::uint16_t),
                            *accountLength);
    boost::uuids::uuid sessionID;
    std::copy(
        buffer.begin() + sizeof(std::uint16_t) + *accountLength,
        buffer.end(),
        sessionID.data
    );

    std::cerr << "Account is: " << accountName << std::endl;
    std::cerr << "Session id is: " << sessionID << std::endl;

    // Check if the account is not connected yet.
    AbstractGameServer& srv(m_gameSession->gameServer());

    if(srv.isAuthenticated(accountName)) {
        // TODO: send an error.
        std::cerr << "Error. " << accountName << " already logged in."
            << std::endl;
        return;
    }

    // Check that the UUID is into the pending accounts.
    if(!srv.isPending(sessionID)) {
        // TODO: send an error.
        std::cerr << "Error. " << sessionID << " is not pending."
            << std::endl;
        return;
    }
    

    // try to connect the account through the session ID.
    std::cerr << "Connect to the server. " << std::endl;
    srv.connect(sessionID, accountName);
    std::shared_ptr<Dummy::Core::Account> account = 
        std::make_shared<Dummy::Core::Account>(accountName,
                                               std::move(sessionID));

    m_gameSession->setAccount(account);
    // Send a positive answer.
    // Send the list of characters maybe.
    _answer(1);
}

void InitialState::_answer(std::uint8_t answer) {
    auto self(m_gameSession->shared_from_this());
    auto selfState(shared_from_this());
    std::array<std::uint8_t, 3> buffer;
    *(reinterpret_cast<std::uint16_t*>(buffer.data())) = sizeof(std::uint8_t);
    buffer[2] = answer;

    boost::asio::async_write(
        m_gameSession->socket(),
        boost::asio::buffer(buffer, buffer.size()),
        [this, self, selfState, answer](boost::system::error_code ec,
                                        std::size_t length)
        {
            if (!ec)
            {
                if (answer) {
                    // send characters info.
                    std::cerr << "Toggle state." << std::endl;
                    m_gameSession->changeState(
                        std::make_shared<SendCharactersState>(m_gameSession)
                    );
                } else {
                    // Toggle another state? Close the connection?
                }
            }
        }
    );
}

} // namespace GameSessionState
