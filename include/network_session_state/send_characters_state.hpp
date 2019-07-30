#pragma once

#include "network_session_state/state.hpp"

namespace Dummy {
namespace Server {
namespace Command {
class GetPrimaryInfoCommand;
} // namespace ConnectCommand

namespace Response {
class CharactersListResponse;
} // namespace ConnectResponse

} // namespace Response
} // namespace Dummy

namespace NetworkSessionState {

class SendCharactersState : public State {
public:
    SendCharactersState(::NetworkSession&);

    virtual
    std::shared_ptr<const Dummy::Server::Command::Command>
    getCommand(Dummy::Protocol::IncomingPacket&) override;

    virtual void visit(const Dummy::Server::Response::Response&) override;

    virtual void
    visitResponse(const Dummy::Server::Response::CharactersListResponse&)
    override;

private:
    std::shared_ptr<const Dummy::Server::Command::GetPrimaryInfoCommand>
    _getPrimaryInfo(Dummy::Protocol::IncomingPacket&);

};

} // namespace NetworkSessionState
