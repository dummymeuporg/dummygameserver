#pragma once

#include <cstdlib>
#include <memory>
#include <vector>

#include <boost/asio.hpp>

#include <dummy/server/response/handler.hpp>

namespace Dummy {
namespace Protocol {
class IncomingPacket;
class OutgoingPacket;
} // namespace Protocol
namespace Server {
class GameSessionCommunicator;
} // namespace Server
} // namespace Dummy

class NetworkServer;

namespace NetworkSessionState {
class State;
} // namespace NetworkSessionState

using ResponsePtr = std::shared_ptr<const Dummy::Server::Response::Response>;

class NetworkSession : public Dummy::Server::Response::Handler {
public:
    NetworkSession(boost::asio::ip::tcp::socket,
                   std::shared_ptr<Dummy::Server::GameSessionCommunicator>);
    virtual ~NetworkSession();
    void close();
    void next();
    void start();

    boost::asio::ip::tcp::socket& socket() {
        return m_socket;
    }

    void changeState(std::shared_ptr<NetworkSessionState::State>);
    void handleResponse(ResponsePtr) override;
    void commandHandlerClosed() override;

private:
    boost::asio::ip::tcp::socket m_socket;
    void _doReadHeader();
    void _doReadContent();
    void _handlePacket(Dummy::Protocol::IncomingPacket&);
    void _sendPacket(const Dummy::Protocol::OutgoingPacket&);

    std::uint16_t m_header;
    std::vector<uint8_t> m_payload;
    std::shared_ptr<Dummy::Server::GameSessionCommunicator>
        m_gameSessionCommunicator;

    std::shared_ptr<NetworkSessionState::State> m_state;
    bool m_isRunning;

};
