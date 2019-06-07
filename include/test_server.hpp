#pragma once
#include <iostream>

#include <boost/asio.hpp>
#include <boost/filesystem.hpp>

#include "abstract_game_server.hpp"

namespace fs = boost::filesystem;
using boost::asio::ip::tcp;

class TestGameServer : public AbstractGameServer {
public:
    TestGameServer(boost::asio::io_service&,
                   unsigned short port,
                   const fs::path& projectPath,
                   const fs::path& serverPath);
    virtual void run() override;
private:
    void _instantiateTestAccount(const std::string&, const std::string&);
};
