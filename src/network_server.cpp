#include <iostream>

#include "server/game_session.hpp"
#include "network_server.hpp"
#include "errors.hpp"
#include "network_session.hpp"

NetworkServer::NetworkServer(
    boost::asio::io_service& ioService,
    unsigned short port,
    Dummy::Server::AbstractGameServer& server
) : m_acceptor(ioService,
               boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(),
                                              port)),
    m_gameServer(server)
{
}

void NetworkServer::run() {
    _doAccept();
}

void NetworkServer::_doAccept()
{
	m_acceptor.async_accept(
    	[this](boost::system::error_code ec,
               boost::asio::ip::tcp::socket socket)
        {
        	if (!ec) {
				
                auto gameSession = m_gameServer.buildGameSession();
                gameSession->start();
            	std::make_shared<NetworkSession>(
                    std::move(socket),
                    gameSession
                )->start();
				
				std::cerr << "Start a new session." << std::endl;
            }
            _doAccept();
        }
    );
}
