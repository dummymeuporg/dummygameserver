#pragma once

#include "network_session_state/state.hpp"

namespace Dummy {
namespace Server {
namespace Command {
class Message;
class Ping;
class SetPosition;
} // namespace Command

namespace Response {
class Ping;
class SetPosition;
} // namespace Response

} // namespase Server
} // namespace Dummy

namespace NetworkSessionState {

class PlayingState : public State {
public:
    PlayingState(::NetworkSession&);

    virtual
    std::unique_ptr<const Dummy::Server::Command::Command>
    getCommand(Dummy::Protocol::IncomingPacket&) override;

    virtual void visit(const Dummy::Server::Response::Response&) override;

    void visitResponse(const Dummy::Server::Response::Ping&) override;
    void visitResponse(const Dummy::Server::Response::SetPosition&) override;
private:
    std::unique_ptr<const Dummy::Server::Command::Ping>
    _ping(Dummy::Protocol::IncomingPacket&);

    std::unique_ptr<const Dummy::Server::Command::SetPosition>
    _setPosition(Dummy::Protocol::IncomingPacket&);

    std::unique_ptr<const Dummy::Server::Command::Message>
    message(Dummy::Protocol::IncomingPacket&);
};

}
