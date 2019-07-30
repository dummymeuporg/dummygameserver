#include <dummy/protocol/bridge.hpp>
#include <dummy/server/response/connect_response.hpp>
#include <dummy/server/response/characters_list_response.hpp>
#include <dummy/server/response/create_character.hpp>
#include <dummy/server/response/message.hpp>
#include <dummy/server/response/ping.hpp>
#include <dummy/server/response/select_character.hpp>
#include <dummy/server/response/set_position.hpp>
#include <dummy/server/response/teleport_map.hpp>

#include "response_packet.hpp"

ResponsePacket::ResponsePacket() { }

void ResponsePacket::visitResponse(
    const Dummy::Server::Response::ConnectResponse& response
) {
    *this << Dummy::Protocol::Bridge::CONNECT;
    response.serializeTo(*this);
}

void ResponsePacket::visitResponse(
    const Dummy::Server::Response::CharactersListResponse& response
) {
    *this << Dummy::Protocol::Bridge::GET_PRIMARY_INFO;
    response.serializeTo(*this);
}

void ResponsePacket::visitResponse(
    const Dummy::Server::Response::CreateCharacter& response
) {
    *this << Dummy::Protocol::Bridge::CREATE_CHARACTER;
    response.serializeTo(*this);
}

void ResponsePacket::visitResponse(
    const Dummy::Server::Response::SelectCharacter& response
) {
    *this << Dummy::Protocol::Bridge::SELECT_CHARACTER;
    response.serializeTo(*this);
}

void ResponsePacket::visitResponse(
    const Dummy::Server::Response::TeleportMap& response
) {
    *this << Dummy::Protocol::Bridge::TELEPORT_MAP;
    response.serializeTo(*this);
}

void ResponsePacket::visitResponse(
    const Dummy::Server::Response::Ping& response
) {
    *this << Dummy::Protocol::Bridge::PING;
    response.serializeTo(*this);
}

void ResponsePacket::visitResponse(
    const Dummy::Server::Response::SetPosition& response
) {
    *this << Dummy::Protocol::Bridge::SET_POSITION;
    response.serializeTo(*this);
}

void ResponsePacket::visitResponse(
    const Dummy::Server::Response::Message& response
) {
    *this << Dummy::Protocol::Bridge::MESSAGE;
    response.serializeTo(*this);
}
