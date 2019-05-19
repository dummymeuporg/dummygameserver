#include <array>
#include <iostream>
#include <boost/uuid/uuid_io.hpp>

#include "abstract_game_server.hpp"
#include "game_session.hpp"
#include "game_session_state/initial_state.hpp"
#include "game_session_state/send_characters_state.hpp"

#include "protocol/outgoing_packet.hpp"

namespace GameSessionState {

InitialState::InitialState(std::shared_ptr<GameSession> gameSession)
    : GameSessionState(gameSession)
{
    std::cerr << "Initial state instantiated." << std::endl;
}

void InitialState::resume() {
    // Nothing to do.
}

void InitialState::onRead(Dummy::Protocol::IncomingPacket& pkt)
{
    std::string accountName;
    std::array<std::uint8_t, 16> uuid_data;
    boost::uuids::uuid sessionID;

    pkt >> accountName >> uuid_data;
    std::copy(uuid_data.begin(), uuid_data.end(), sessionID.data);

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
    Dummy::Protocol::OutgoingPacket pkt;
    pkt << answer;
    boost::asio::async_write(
        m_gameSession->socket(),
        boost::asio::buffer(pkt.buffer(), pkt.size()),
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
