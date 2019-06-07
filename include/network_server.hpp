#pragma once

#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>

using boost::asio::ip::tcp;

class Player;
class ServerMap;

class NetworkServer {
public:
    AbstractGameServer(boost::asio::io_service&,
                       unsigned short port);

    virtual void run() = 0;

protected:
    void _doAccept();

    boost::asio::ip::tcp::acceptor m_acceptor;

};
