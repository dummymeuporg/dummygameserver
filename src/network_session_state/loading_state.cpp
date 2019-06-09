#include <iostream>
#include "protocol/bridge.hpp"
#include "protocol/incoming_packet.hpp"
#include "protocol/outgoing_packet.hpp"
#include "server/command/teleport_map.hpp"
#include "server/response/teleport_map.hpp"
#include "network_session.hpp"
#include "network_session_state/loading_state.hpp"

namespace NetworkSessionState {

LoadingState::LoadingState(::NetworkSession& networkSession)
    : State(networkSession)
{
}

std::unique_ptr<const Dummy::Server::Command::Command>
LoadingState::getCommand(Dummy::Protocol::IncomingPacket& pkt)
{
    std::uint16_t command;
    pkt >> command;
    switch(command) {
    case Dummy::Protocol::Bridge::TELEPORT_MAP:
        return _teleportMap(pkt);
        break;
    default:
        throw UnknownCommandError();
        break;
    }
}

std::unique_ptr<const Dummy::Server::Command::TeleportMap>
LoadingState::_teleportMap(Dummy::Protocol::IncomingPacket& pkt)
{
    std::string mapName, instance;
    std::uint16_t dstX, dstY;
    pkt >> mapName >> dstX >> dstY >> instance;
    return std::make_unique<Dummy::Server::Command::TeleportMap>(
        mapName, dstX, dstY, instance
    );
}

void LoadingState::visit(
    const Dummy::Server::Response::Response& response
) {
    response.accept(*this);
}

void LoadingState::visitResponse(
    const Dummy::Server::Response::TeleportMap& response
) {
    auto self(shared_from_this());
    std::cerr << "Teleport to map" << std::endl;
}

} // namespace NetworkSessionState