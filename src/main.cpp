#include <cstdlib>
#include <iostream>

#include <boost/asio.hpp>

#include "crypto/rsa_keypair.hpp"
#include "master_client.hpp"

int main(int argc, char* argv[])
{
    if (argc < 4) {
        std::cerr << "Usage: " << argv[0] << " PROJECT_DIR ACCOUNTS_DIR "
            "CONF_DIR" << std::endl;
        exit(EXIT_FAILURE);
    }

    boost::asio::io_service ioservice;

    try {
        MasterClient masterClient(
            ioservice,
            fs::path(argv[1]),
            fs::path(argv[2]),
            fs::path(argv[3])
        );
    } catch(const Dummy::Crypto::RSAKeyPairException& e) {
        std::cerr << "Could not run the master client: " 
            << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }
    //GameServer gameServer(ioservice, 4648, argv[1]);
    //
    ioservice.run();

    return EXIT_SUCCESS;
}
