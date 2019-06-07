#pragma once

#include "network_session_state/state.hpp"

namespace Dummy {
namespace Server {
namespace Command {
class ConnectCommand;
} // namespace ConnectCommand

namespace Response {
class ConnectResponse;
} // namespace ConnectResponse

} // namespace Response
} // namespace Dummy

namespace NetworkSessionState {

class InitialState : public State {
public:
    InitialState(::NetworkSession&);

    virtual
    std::unique_ptr<const Dummy::Protocol::OutgoingPacket>
    serializeResponse(const Dummy::Server::Response::Response&) override;

    virtual
    std::unique_ptr<const Dummy::Server::Command::Command>
    getCommand(Dummy::Protocol::IncomingPacket&) override;

    virtual void visit(const Dummy::Server::Response::Response&) override;

    virtual void
    visitResponse(const Dummy::Server::Response::ConnectResponse&) override;

private:
    std::unique_ptr<const Dummy::Server::Command::ConnectCommand>
    _getConnectCommand(Dummy::Protocol::IncomingPacket&);
};

} // namespace NetworkSessionState
