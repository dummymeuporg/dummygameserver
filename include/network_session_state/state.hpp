#pragma once

#include <exception>
#include <memory>
#include <dummy/server/response/response_visitor.hpp>

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

class Error : public std::exception {

};

class UnknownCommandError : public Error {
public:
    virtual const char* what() const noexcept override {
        return "the command code is incorrect";
    };
};

class State : public std::enable_shared_from_this<State>,
              public Dummy::Server::Response::ResponseVisitor {
public:
    State(::NetworkSession&);

    virtual
    std::shared_ptr<const Dummy::Server::Command::Command>
    getCommand(Dummy::Protocol::IncomingPacket&) = 0;

    virtual void visit(const Dummy::Server::Response::Response&) = 0;

protected:
    ::NetworkSession& m_networkSession;
};

} // namespace NetworkSessionState
