#include <iostream>
#include "protocol/bridge.hpp"
#include "protocol/incoming_packet.hpp"
#include "protocol/outgoing_packet.hpp"
#include "server/command/ping.hpp"
#include "server/command/set_position.hpp"
#include "server/response/ping.hpp"
#include "server/response/set_position.hpp"
#include "network_session.hpp"
#include "network_session_state/playing_state.hpp"
#include "network_session_state/loading_state.hpp"

namespace NetworkSessionState {

PlayingState::PlayingState(::NetworkSession& networkSession)
    : State(networkSession)
{
}

std::unique_ptr<const Dummy::Server::Command::Command>
PlayingState::getCommand(Dummy::Protocol::IncomingPacket& pkt) {
    std::uint16_t command;
    pkt >> command;
    switch(command) {
    case Dummy::Protocol::Bridge::PING:
        return _ping(pkt);
    case Dummy::Protocol::Bridge::SET_POSITION:
        return _setPosition(pkt);
    default:
        throw UnknownCommandError();
        break;
    }
}

std::unique_ptr<const Dummy::Server::Command::Ping>
PlayingState::_ping(Dummy::Protocol::IncomingPacket& pkt) {
    // Nothing to extract from the packet (for now).
    return std::make_unique<Dummy::Server::Command::Ping>();
}

std::unique_ptr<const Dummy::Server::Command::SetPosition>
PlayingState::_setPosition(Dummy::Protocol::IncomingPacket& pkt) {
    std::uint16_t x, y;
    pkt >> x >> y;
    return std::make_unique<Dummy::Server::Command::SetPosition>(
        x, y
    );
}

void PlayingState::visit(const Dummy::Server::Response::Response& response) {
    response.accept(*this);
}

void PlayingState::visitResponse(
    const Dummy::Server::Response::Ping& ping
) {
    // Nothing to do, for now!
}

void PlayingState::visitResponse(
    const Dummy::Server::Response::SetPosition& setPosition
) {
    // Nothing to do for now!
}

}
