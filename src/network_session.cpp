#include <iostream>

#include <boost/asio.hpp>

#include "protocol/incoming_packet.hpp"
#include "protocol/outgoing_packet.hpp"
#include "server/game_session.hpp"
#include "server/command/command.hpp"
#include "server/response/response.hpp"
#include "network_session_state/initial_state.hpp"
#include "response_packet.hpp"
#include "network_session.hpp"

NetworkSession::NetworkSession(
    boost::asio::ip::tcp::socket s,
    std::shared_ptr<Dummy::Server::GameSession> gameSession)
    : m_socket(std::move(s)), m_gameSession(gameSession),
      m_state(std::make_shared<NetworkSessionState::InitialState>(*this)),
      m_isRunning(false)
{

}

NetworkSession::~NetworkSession() {
    std::cerr << "Ending network session." << std::endl;
}

void NetworkSession::close() {
    if (m_isRunning) {
        m_socket.close();
        m_gameSession->close();
        m_gameSession.reset();
        m_state.reset();
        m_isRunning = false;
    }
}

void NetworkSession::start()
{
    m_isRunning = true;
    _doReadHeader();
}

void NetworkSession::next() {
    _doReadHeader();
}

void NetworkSession::_doReadHeader() {
    auto self(shared_from_this());
    boost::asio::async_read(
        m_socket,
        boost::asio::buffer(&m_header, sizeof(std::uint16_t)),
        [this, self](boost::system::error_code ec, std::size_t length)
        {
            if (!ec) {
                m_payload.resize(m_header);
                _doReadContent();
            } else {
                std::cerr << "Error while reading size." << std::endl;
                close();
            }
        }
    );
}

void NetworkSession::_handlePacket(Dummy::Protocol::IncomingPacket& pkt) {

    // Convert the incoming packet into command
    std::unique_ptr<const Dummy::Server::Command::Command> cmd =
        m_state->getCommand(pkt);

    // Forward the command into the game session
    m_gameSession->handleCommand(*cmd);

    // Immediatly get the response
    std::unique_ptr<const Dummy::Server::Response::Response> response =
        m_gameSession->getResponse();

    ResponsePacket packet;

    response->accept(packet);

    _sendPacket(packet);

    m_state->visit(*response);
}

void NetworkSession::_doReadContent() {
    auto self(shared_from_this());
    boost::asio::async_read(
        m_socket,
        boost::asio::buffer(m_payload, m_header),
        [this, self](boost::system::error_code ec, std::size_t length)
        {
            if (!ec) {
                try {
                    Dummy::Protocol::IncomingPacket pkt(m_payload);
                    _handlePacket(pkt);
                } catch(const Dummy::Protocol::Error& e) {
                    std::cerr << e.what();
                    close();
                }
            } else {
                std::cerr << "Error while reading content." << std::endl;
                close();
            }
        }
	);
}

void
NetworkSession::changeState(std::shared_ptr<NetworkSessionState::State> state)
{
    m_state = state;
}

void NetworkSession::_sendPacket(const Dummy::Protocol::OutgoingPacket& pkt) {
    auto self(shared_from_this());
    boost::asio::async_write(
        m_socket,
        boost::asio::buffer(pkt.buffer(), pkt.size()),
        [this, self](boost::system::error_code ec, std::size_t size) {
            if (!ec) {
                next();
            }
        }
    );
}
