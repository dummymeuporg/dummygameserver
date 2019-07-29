#include <iostream>

#include <dummy/protocol/bridge.hpp>
#include <dummy/protocol/incoming_packet.hpp>
#include <dummy/protocol/outgoing_packet.hpp>
#include <dummy/server/command/connect_command.hpp>
#include <dummy/server/response/connect_response.hpp>

#include "network_session.hpp"
#include "network_session_state/initial_state.hpp"
#include "network_session_state/send_characters_state.hpp"

namespace NetworkSessionState {

InitialState::InitialState(::NetworkSession& networkSession)
    : State(networkSession)
{

}

std::shared_ptr<const Dummy::Server::Command::Command>
InitialState::getCommand(Dummy::Protocol::IncomingPacket& pkt)
{
    std::uint16_t command;
    pkt >> command;
    switch(command) {
    case Dummy::Protocol::Bridge::CONNECT:
        /* Build connect command. */
        return _getConnectCommand(pkt);
        break;
    default:
        /* throw an exception */
        throw UnknownCommandError();
        break;
    }
}

std::shared_ptr<const Dummy::Server::Command::ConnectCommand>
InitialState::_getConnectCommand(Dummy::Protocol::IncomingPacket& pkt)
{
    std::string account, sessionID;
    pkt >> account >> sessionID;

    std::cerr << "Account is " << account << std::endl;
    std::cerr << "session ID is " << sessionID << std::endl;
    return std::make_shared<Dummy::Server::Command::ConnectCommand>(
        account, sessionID
    );
}

void InitialState::visit(const Dummy::Server::Response::Response& response) {
    response.accept(*this);
}

void
InitialState::visitResponse(
    const Dummy::Server::Response::ConnectResponse& connect
) {
    auto self(shared_from_this());
    if(connect.status() == 0) {
        /* O.K., change the state */
        std::cerr << "Get to next network state" << std::endl;
        m_networkSession.changeState(
            std::make_shared<SendCharactersState>(m_networkSession)
        );
    } else {
        std::cerr << "Error: close the connection." << std::endl;
        m_networkSession.close();
    }
}

} // namespace NetworkSessionState
