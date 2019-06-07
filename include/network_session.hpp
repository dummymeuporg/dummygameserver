#pragma once

#include <cstdlib>
#include <memory>
#include <vector>

#include <boost/asio.hpp>

class NetworkServer;

class NetworkSession : public std::enable_shared_from_this<NetworkSession> {
public:
    NetworkSession(boost::asio::ip::tcp::socket, AbstractGameServer&);
    void close();
    void next();
    void start();

    boost::asio::ip::tcp::socket& socket() {
        return m_socket;
    }

private:
    boost::asio::ip::tcp::socket m_socket;
    AbstractGameServer& m_gameServer;
    void _doReadHeader();
    void _doReadContent();

    std::uint16_t m_header;
    std::vector<uint8_t> m_payload;
    std::shared_ptr<GameSessionState::GameSessionState> m_state;
};
