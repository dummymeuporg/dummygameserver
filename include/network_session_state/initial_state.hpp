#pragma once

#include "network_session_state/state.hpp"

namespace NetworkSessionState {

class InitialState : public State {
public:
    InitialState(::NetworkSession&);

    std::unique_ptr<const Dummy::Protocol::OutgoingPacket>
    serializeResponse(const Dummy::Server::Response::Response&) override;

    virtual
    std::unique_ptr<const Dummy::Server::Command::Command>
    getCommand(Dummy::Protocol::IncomingPacket&) override;
    
};

} // namespace NetworkSessionState
