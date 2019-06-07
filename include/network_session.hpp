#pragma once

#include <cstdlib>
#include <memory>
#include <vector>

#include <boost/asio.hpp>

namespace Dummy {
namespace Protocol {
class IncomingPacket;
class OutgoingPacket;
} // namespace Protocol
namespace Server {
class GameSession;
} // namespace Server
} // namespace Dummy

class NetworkServer;

namespace NetworkSessionState {
class State;
} // namespace NetworkSessionState

class NetworkSession : public std::enable_shared_from_this<NetworkSession> {
public:
    NetworkSession(boost::asio::ip::tcp::socket,
                   std::shared_ptr<Dummy::Server::GameSession>);
    void close();
    void next();
    void start();

    boost::asio::ip::tcp::socket& socket() {
        return m_socket;
    }

    void changeState(std::shared_ptr<NetworkSessionState::State>);

private:
    boost::asio::ip::tcp::socket m_socket;
    void _doReadHeader();
    void _doReadContent();
    void _handlePacket(Dummy::Protocol::IncomingPacket&);
    void _sendPacket(const Dummy::Protocol::OutgoingPacket&);

    std::uint16_t m_header;
    std::vector<uint8_t> m_payload;

    std::shared_ptr<Dummy::Server::GameSession> m_gameSession;
    std::shared_ptr<NetworkSessionState::State> m_state;
};
