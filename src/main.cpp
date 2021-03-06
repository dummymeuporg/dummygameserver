#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <thread>

#include <boost/asio.hpp>

#include "master_client.hpp"
#include "test_server.hpp"

using boost::asio::ip::tcp;
namespace fs = std::filesystem;

int main(int argc, char* argv[])
{
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " PROJECT_DIR SERVER_DIR"
            << std::endl;
        exit(EXIT_FAILURE);
    }

    boost::asio::io_context io_context;


    std::cerr << "Instantiate the game server..." << std::endl;
    Dummy::Server::AbstractGameServer gameServer(io_context, argv[1], argv[2]);
    gameServer.run();
    std::cerr << "Server running." << std::endl;
    tcp::resolver resolver(io_context);
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

    TestServer server(io_context, 6612, gameServer);
    server.run();
    io_context.run();


    return EXIT_SUCCESS;
}
