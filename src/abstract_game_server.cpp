#include "abstract_game_server.hpp"

AbstractGameServer::AbstractGameServer(
    boost::asio::io_service& ioService,
    unsigned short port,
    const fs::path& projectPath,
    const fs::path& accountsPath
) : m_acceptor(ioService,
               boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(),
                                              port)),
    m_confPath(confPath),
    m_projectPath(projectPath)
{
    // Validate config?
    // Load project?
}
