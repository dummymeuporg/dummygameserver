#pragma once

#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>

using boost::asio::ip::tcp;

class AbstractGameServer {
public:
    AbstractGameServer(boost::asio::io_service&,
                       unsigned short port,
                       const fs::path& projectPath,
                       const fs::path& accountsPath);

protected:
    boost::asio::ip::tcp::acceptor m_acceptor;
    fs::path m_accountsPath;
    fs::path m_projectPath;

};
