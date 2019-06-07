#include <iostream>
#include "protocol/bridge.hpp"
#include "protocol/incoming_packet.hpp"
#include "protocol/outgoing_packet.hpp"
#include "server/command/connect_command.hpp"
#include "server/response/connect_response.hpp"
#include "network_session.hpp"
#include "network_session_state/initial_state.hpp"

namespace NetworkSessionState {

InitialState::InitialState(::NetworkSession& networkSession)
    : State(networkSession)
{

}

std::unique_ptr<const Dummy::Protocol::OutgoingPacket>
InitialState::serializeResponse(
    const Dummy::Server::Response::Response& response
)
{
    std::unique_ptr<Dummy::Protocol::OutgoingPacket> outPkt =
        std::make_unique<Dummy::Protocol::OutgoingPacket>();

    *outPkt << static_cast<std::uint16_t>(Dummy::Protocol::Bridge::CONNECT);
    response.serializeTo(*outPkt);
    return outPkt;
}

std::unique_ptr<const Dummy::Server::Command::Command>
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

std::unique_ptr<const Dummy::Server::Command::ConnectCommand>
InitialState::_getConnectCommand(Dummy::Protocol::IncomingPacket& pkt)
{
    std::string account, sessionID;
    pkt >> account >> sessionID;

    std::cerr << "Account is " << account << std::endl;
    std::cerr << "session ID is " << sessionID << std::endl;
    return std::make_unique<Dummy::Server::Command::ConnectCommand>(
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
    } else {
        std::cerr << "Error: close the connection." << std::endl;
        m_networkSession.close();
    }
}

} // namespace NetworkSessionState
