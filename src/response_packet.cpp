#include "protocol/bridge.hpp"
#include "server/response/connect_response.hpp"
#include "server/response/characters_list_response.hpp"
#include "server/response/create_character.hpp"
#include "server/response/ping.hpp"
#include "server/response/select_character.hpp"
#include "server/response/set_position.hpp"
#include "server/response/teleport_map.hpp"

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
