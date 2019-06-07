#pragma once

#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>

#include "server/abstract_game_server.hpp"

using boost::asio::ip::tcp;

class Player;
class ServerMap;

class NetworkServer {
public:
    NetworkServer(boost::asio::io_service&, unsigned short port,
                  Dummy::Server::AbstractGameServer&);

    virtual void run();

protected:
    void _doAccept();

    boost::asio::ip::tcp::acceptor m_acceptor;
    Dummy::Server::AbstractGameServer& m_gameServer;

};
