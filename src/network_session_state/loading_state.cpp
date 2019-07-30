#include <iostream>

#include <dummy/protocol/bridge.hpp>
#include <dummy/protocol/incoming_packet.hpp>
#include <dummy/protocol/outgoing_packet.hpp>
#include <dummy/server/command/teleport_map.hpp>
#include <dummy/server/response/teleport_map.hpp>

#include "network_session.hpp"
#include "network_session_state/loading_state.hpp"
#include "network_session_state/playing_state.hpp"

namespace NetworkSessionState {

LoadingState::LoadingState(::NetworkSession& networkSession)
    : State(networkSession)
{
}

std::shared_ptr<const Dummy::Server::Command::Command>
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

std::shared_ptr<const Dummy::Server::Command::TeleportMap>
LoadingState::_teleportMap(Dummy::Protocol::IncomingPacket& pkt)
{
    std::string mapName, instance;
    std::uint16_t dstX, dstY;
    pkt >> mapName >> dstX >> dstY >> instance;
    return std::make_shared<Dummy::Server::Command::TeleportMap>(
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
    if (response.status() == 0) {
        std::cerr << "Switch to playing state." << std::endl;
        m_networkSession.changeState(
            std::make_shared<PlayingState>(m_networkSession)
        );
    }
}

} // namespace NetworkSessionState
