#include <cstdlib>
#include <iostream>
#include <thread>

#include <boost/asio.hpp>

#include "crypto/rsa_keypair.hpp"
#include "master_client.hpp"

using boost::asio::ip::tcp;

int main(int argc, char* argv[])
{
    if (argc < 4) {
        std::cerr << "Usage: " << argv[0] << " PROJECT_DIR ACCOUNTS_DIR "
            "CONF_DIR SERVER_NAME" << std::endl;
        exit(EXIT_FAILURE);
    }

    try {
        
        boost::asio::io_service io_service;
        tcp::resolver resolver(io_service);
        auto endpoints = resolver.resolve("127.0.0.1", "33338");

        MasterClient masterClient(
            io_service,
            endpoints,
            fs::path(argv[1]),
            fs::path(argv[2]),
            fs::path(argv[3]),
            argv[4]
        );

        io_service.run();

    } catch(const Dummy::Crypto::RSAKeyPairException& e) {
        std::cerr << "Could not run the master client: " 
            << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}
