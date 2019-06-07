#pragma once
#include <iostream>
#include <filesystem>

#include <boost/asio.hpp>

#include "network_server.hpp"

using boost::asio::ip::tcp;

namespace Dummy {
namespace Server {
class AbstractGameServer;
} // namespace Server
} // namespace Dummy

class TestServer : public NetworkServer {
public:
    TestServer(boost::asio::io_service&, unsigned short port,
               Dummy::Server::AbstractGameServer&);
    virtual void run() override;
private:
    void _instantiateTestAccount(const std::string&, const std::string&);
};
