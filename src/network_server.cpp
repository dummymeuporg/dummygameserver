#include <iostream>

#include <dummy/server/game_session.hpp>
#include <dummy/server/game_session_communicator.hpp>
#include "network_server.hpp"
#include "errors.hpp"
#include "network_session.hpp"

NetworkServer::NetworkServer(
    boost::asio::io_context& ioContext,
    unsigned short port,
    Dummy::Server::AbstractGameServer& server
) : m_ioContext(ioContext),
    m_acceptor(m_ioContext,
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
                auto communicator(
                    std::make_shared<Dummy::Server::GameSessionCommunicator>(
                        m_ioContext
                    )
                );
                auto gameSession = m_gameServer.buildGameSession(communicator);
                gameSession->start();
            	std::make_shared<NetworkSession>(
                    std::move(socket),
                    communicator
                )->start();
				
				std::cerr << "Start a new session." << std::endl;
            }
            _doAccept();
        }
    );
}
