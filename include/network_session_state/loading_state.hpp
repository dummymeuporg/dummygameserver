#pragma once

#include "network_session_state/state.hpp"

namespace Dummy {
namespace Server {
namespace Command {
class TeleportMap;
} // namespace Command

namespace Response {
class TeleportMap;
} // namespace Response

} // namespace Server
} // namespace Dummy

namespace NetworkSessionState {

class LoadingState : public State {
public:
    LoadingState(::NetworkSession&);

    virtual
    std::unique_ptr<const Dummy::Server::Command::Command>
    getCommand(Dummy::Protocol::IncomingPacket&) override;

    virtual void visit(const Dummy::Server::Response::Response&) override;

    virtual void
    visitResponse(const Dummy::Server::Response::TeleportMap&) override;

private:
    std::unique_ptr<const Dummy::Server::Command::TeleportMap>
    _teleportMap(Dummy::Protocol::IncomingPacket&);
};

} // namespace NetworkSessionState
