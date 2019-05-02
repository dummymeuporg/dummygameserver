#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/filesystem.hpp>

#include "game_server.hpp"

GameServer::GameServer(boost::asio::io_service& ioService,
                       unsigned short port,
                       const char* confPath,
                       const char* projectPath)
    : m_acceptor(ioService,
                 boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(),
                                                port)),
      m_confPath(confPath),
      m_projectPath(projectPath)
{
    _doAccept();
}

void GameServer::_doAccept() {
    m_acceptor.async_accept(
        [this](boost::system::error_code ec,
               boost::asio::ip::tcp::socket socket)
        {
            if (!ec) {
                std::make_shared<GameSession>(std::move(socket),
                                              *this)->start();
            }
            _doAccept();
        }
    );
}
