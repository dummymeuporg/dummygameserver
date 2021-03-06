#pragma once

#include <dummy/protocol/outgoing_packet.hpp>
#include <dummy/server/response/response_visitor.hpp>

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

    void visitResponse(
        const Dummy::Server::Response::ConnectResponse&
    ) override;
    void visitResponse(
        const Dummy::Server::Response::CharactersListResponse&
    ) override;

    void
    visitResponse(const Dummy::Server::Response::CreateCharacter&) override;
    void

    visitResponse(const Dummy::Server::Response::SelectCharacter&) override;
    void visitResponse(const Dummy::Server::Response::TeleportMap&) override;
    void visitResponse(const Dummy::Server::Response::Ping&) override;
    void visitResponse(const Dummy::Server::Response::SetPosition&) override;
    void visitResponse(const Dummy::Server::Response::Message&) override;

};
