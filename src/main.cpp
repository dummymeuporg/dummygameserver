#include <cstdlib>
#include <iostream>
#include <thread>

#include <boost/asio.hpp>

#include "crypto/rsa_keypair.hpp"
#include "master_client.hpp"
#include "test_game_server.hpp"

using boost::asio::ip::tcp;

int main(int argc, char* argv[])
{
    if (argc < 5) {
        std::cerr << "Usage: " << argv[0] << " PROJECT_DIR ACCOUNTS_DIR "
            "CONF_DIR SERVER_NAME" << std::endl;
        exit(EXIT_FAILURE);
    }

        
    boost::asio::io_service io_service;
    tcp::resolver resolver(io_service);
    auto endpoints = resolver.resolve("127.0.0.1", "33338");

    /*
    MasterClient masterClient(
        io_service,
        endpoints,
        fs::path(argv[1]),
        fs::path(argv[2]),
        fs::path(argv[3]),
        argv[4]
    );*/

    TestGameServer server(io_service,
                          6612,
                          fs::path(argv[1]),
                          fs::path(argv[2]));

    io_service.run();


    return EXIT_SUCCESS;
}
