#pragma once

#include "network_session_state/state.hpp"

namespace Dummy {
namespace Server {
namespace Command {
class Ping;
} // namespace Command

namespace Response {
class Ping;
} // namespace Response

} // namespase Server
} // namespace Dummy

namespace NetworkSessionState {

class PlayingState : public State {
    PlayingState(::NetworkSession&);

    virtual
    std::unique_ptr<const Dummy::Server::Command::Command>
    getCommand(Dummy::Protocol::IncomingPacket&) override;

    virtual void visit(const Dummy::Server::Response::Response&) override;

    virtual void
    visitResponse(const Dummy::Server::Response::Ping&) override;
private:
    std::unique_ptr<const Dummy::Server::Command::Ping>
    _ping(Dummy::Protocol::IncomingPacket&);
};

}
