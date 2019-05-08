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
