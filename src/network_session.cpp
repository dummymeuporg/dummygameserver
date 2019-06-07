#include <iostream>

#include <boost/asio.hpp>

#include "network_session.hpp"

NetworkSession::NetworkSession(boost::asio::ip::tcp::socket s,
                               NetworkServer& networkServer)
    : m_socket(std::move(s)),
      m_gameServer(gameServer)
{

}

void NetworkSession::close() {
    m_socket.close();
}

void NetworkSession::start()
{
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
                    m_state->onRead(pkt);
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
