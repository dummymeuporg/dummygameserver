#pragma once

#include <memory>

namespace Dummy {
namespace Protocol {
class IncomingPacket;
class OutgoingPacket;
} // namespace Protocol

namespace Server {
namespace Command {
class Command;
} // namespace Command

namespace Response {
class Response;
} // namespace Response
} // namespace Server
} // namespace Dummy

class NetworkSession;

namespace NetworkSessionState {

class State : public std::enable_shared_from_this<State> {
public:
    State(::NetworkSession&);

    virtual
    std::unique_ptr<const Dummy::Protocol::OutgoingPacket>
    serializeResponse(const Dummy::Server::Response::Response&) = 0;

    virtual
    std::unique_ptr<const Dummy::Server::Command::Command>
    getCommand(Dummy::Protocol::IncomingPacket&) = 0;

protected:
    ::NetworkSession& m_networkSession;
};

} // namespace NetworkSessionState
