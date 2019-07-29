#pragma once

#include "network_session_state/state.hpp"

namespace Dummy {
namespace Server {
namespace Command {
class CreateCharacter;
class SelectCharacter;
} // namespace ConnectCommand

namespace Response {
class CreateCharacter;
class SelectCharacter;
} // namespace ConnectResponse

} // namespace Response
} // namespace Dummy

namespace NetworkSessionState {

class ManageCharactersState : public State {
public:
    ManageCharactersState(::NetworkSession&);

    virtual
    std::shared_ptr<const Dummy::Server::Command::Command>
    getCommand(Dummy::Protocol::IncomingPacket&) override;

    virtual void visit(const Dummy::Server::Response::Response&) override;

    virtual void
    visitResponse(const Dummy::Server::Response::CreateCharacter&)
    override;

    virtual void
    visitResponse(const Dummy::Server::Response::SelectCharacter&)
    override;

private:
    std::shared_ptr<const Dummy::Server::Command::CreateCharacter>
    _createCharacter(Dummy::Protocol::IncomingPacket&);

    std::shared_ptr<const Dummy::Server::Command::SelectCharacter>
    _selectCharacter(Dummy::Protocol::IncomingPacket&);

};

} // namespace NetworkSessionState
