#include <iostream>

#include "abstract_game_server.hpp"

AbstractGameServer::AbstractGameServer(
    boost::asio::io_service& ioService,
    unsigned short port,
    const fs::path& projectPath,
    const fs::path& serverPath
) : m_acceptor(ioService,
               boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(),
                                              port)),
    m_project(projectPath),
    m_serverPath(serverPath)
{
    // Validate config?
    // Load project?
}

void AbstractGameServer::_doAccept()
{
	m_acceptor.async_accept(
    	[this](boost::system::error_code ec,
               boost::asio::ip::tcp::socket socket)
        {
        	if (!ec) {
				/*
            	std::make_shared<PlayerSession>(std::move(socket),
                                                *this)->start();
				*/
				std::cerr << "Start a new session." << std::endl;
            }
            _doAccept();
        }
    );
}

void AbstractGameServer::connect(
    const boost::uuids::uuid& sessionID,
    const std::string& accountName)
{
    if (m_pendingAccounts.find(sessionID) == m_pendingAccounts.end()) {
        return; // Throw an exception?
    }

    if (m_connectedAccounts.find(accountName) == m_connectedAccounts.end()) {
        return; // Throw an exception?
    }

    std::cerr << "Connect account " << accountName << std::endl;
    std::shared_ptr<Dummy::Core::Account> account =
        m_pendingAccounts[sessionID];
    m_connectedAccounts[accountName] = account;
}
