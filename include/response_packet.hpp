#pragma once

#include "protocol/outgoing_packet.hpp"
#include "server/response/response_visitor.hpp"

namespace Dummy {
namespace Server {
namespace Response {
class ConnectResponse;
class CharactersListResponse;
class CreateCharacter;
class SelectCharacter;
class TeleportMap;
} // namespace Response
} // namespace Server
} // namespace Dummy

class ResponsePacket : public Dummy::Protocol::OutgoingPacket,
                       public Dummy::Server::Response::ResponseVisitor {
public:
    ResponsePacket();

    virtual void visitResponse(
        const Dummy::Server::Response::ConnectResponse&
    ) override;
    virtual void visitResponse(
        const Dummy::Server::Response::CharactersListResponse&
    ) override;
    virtual void visitResponse(
        const Dummy::Server::Response::CreateCharacter&
    ) override;
    virtual void visitResponse(
        const Dummy::Server::Response::SelectCharacter&
    ) override;
    virtual void visitResponse(
        const Dummy::Server::Response::TeleportMap&
    ) override;

};
