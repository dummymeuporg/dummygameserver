#include <iostream>

#include <dummy/protocol/bridge.hpp>
#include <dummy/protocol/incoming_packet.hpp>
#include <dummy/protocol/outgoing_packet.hpp>

#include <dummy/server/command/message.hpp>
#include <dummy/server/command/ping.hpp>
#include <dummy/server/command/set_position.hpp>
#include <dummy/server/command/teleport_map.hpp>

#include <dummy/server/response/ping.hpp>
#include <dummy/server/response/set_position.hpp>

#include "network_session.hpp"
#include "network_session_state/playing_state.hpp"
#include "network_session_state/loading_state.hpp"

namespace NetworkSessionState {

PlayingState::PlayingState(::NetworkSession& networkSession)
    : State(networkSession)
{
}

std::shared_ptr<const Dummy::Server::Command::Command>
PlayingState::getCommand(Dummy::Protocol::IncomingPacket& pkt) {
    std::uint16_t command;
    pkt >> command;
    switch(command) {
    case Dummy::Protocol::Bridge::PING:
        return _ping(pkt);
        break;
    case Dummy::Protocol::Bridge::SET_POSITION:
        return _setPosition(pkt);
        break;
    case Dummy::Protocol::Bridge::MESSAGE:
        return message(pkt);
        break;
    case Dummy::Protocol::Bridge::PLAYING_TELEPORT_MAP:
        return teleportMap(pkt);
        break;
    default:
        throw UnknownCommandError();
        break;
    }
}

std::shared_ptr<const Dummy::Server::Command::TeleportMap>
PlayingState::teleportMap(Dummy::Protocol::IncomingPacket& pkt) {
    std::string mapName;
    std::uint16_t x, y;
    std::uint8_t floor;
    std::string instance;

    pkt >> mapName >> x >> y >> floor >> instance;
    return std::make_unique<Dummy::Server::Command::TeleportMap>(
        mapName, x, y, floor, instance
    );
}

std::shared_ptr<const Dummy::Server::Command::Ping>
PlayingState::_ping(Dummy::Protocol::IncomingPacket& pkt) {
    // Nothing to extract from the packet (for now).
    return std::make_unique<Dummy::Server::Command::Ping>();
}

std::shared_ptr<const Dummy::Server::Command::SetPosition>
PlayingState::_setPosition(Dummy::Protocol::IncomingPacket& pkt) {
    std::uint16_t x, y;
    pkt >> x >> y;
    return std::make_unique<Dummy::Server::Command::SetPosition>(x, y);
}

std::shared_ptr<const Dummy::Server::Command::Message>
PlayingState::message(Dummy::Protocol::IncomingPacket& pkt) {
    std::string content;
    pkt >> content;
    return std::make_unique<Dummy::Server::Command::Message>(content);
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

void PlayingState::visitResponse(const Dummy::Server::Response::Message&) {
    // Nothing to do for now!

}

void PlayingState::visitResponse(const Dummy::Server::Response::TeleportMap&) {
    // Nothing to do for now!
}

}
