#include <iostream>
#include "protocol/bridge.hpp"
#include "protocol/incoming_packet.hpp"
#include "protocol/outgoing_packet.hpp"
#include "server/command/get_primary_info_command.hpp"
#include "server/response/characters_list_response.hpp"
#include "network_session.hpp"
#include "network_session_state/send_characters_state.hpp"
#include "network_session_state/manage_characters_state.hpp"

namespace NetworkSessionState {

SendCharactersState::SendCharactersState(::NetworkSession& networkSession)
    : State(networkSession)
{

}

std::unique_ptr<const Dummy::Protocol::OutgoingPacket>
SendCharactersState::serializeResponse(
    const Dummy::Server::Response::Response& response
)
{
    std::unique_ptr<Dummy::Protocol::OutgoingPacket> outPkt =
        std::make_unique<Dummy::Protocol::OutgoingPacket>();

    *outPkt <<
        static_cast<std::uint16_t>(Dummy::Protocol::Bridge::GET_PRIMARY_INFO);
    response.serializeTo(*outPkt);
    return outPkt;
}

std::unique_ptr<const Dummy::Server::Command::Command>
SendCharactersState::getCommand(Dummy::Protocol::IncomingPacket& pkt)
{
    std::uint16_t command;
    pkt >> command;
    switch(command) {
    case Dummy::Protocol::Bridge::GET_PRIMARY_INFO:
        /* Build connect command. */
        return _getPrimaryInfo(pkt);
        break;
    default:
        /* throw an exception */
        throw UnknownCommandError();
        break;
    }
}

std::unique_ptr<const Dummy::Server::Command::GetPrimaryInfoCommand>
SendCharactersState::_getPrimaryInfo(Dummy::Protocol::IncomingPacket& pkt)
{
    return std::make_unique<Dummy::Server::Command::GetPrimaryInfoCommand>();
}

void SendCharactersState::visit(
const Dummy::Server::Response::Response& response
) {
    response.accept(*this);
}

void
SendCharactersState::visitResponse(
    const Dummy::Server::Response::CharactersListResponse& response
) {
    auto self(shared_from_this());
    std::cerr << "Auto switch to next state" << std::endl;
    m_networkSession.changeState(
        std::make_shared<ManageCharactersState>(m_networkSession)
    );
}

} // namespace NetworkSessionState
