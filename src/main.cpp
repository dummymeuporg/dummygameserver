#include <cstdlib>
#include <iostream>

#include <boost/asio.hpp>

int main(int argc, char* argv[])
{
    if (argc < 4) {
        std::cerr << "Usage: " << argv[0] << " PROJECT_DIR ACCOUNTS_DIR "
            "CONF_DIR" << std::endl;
        exit(EXIT_FAILURE);
    }

    boost::asio::io_service ioservice;
    //GameServer gameServer(ioservice, 4648, argv[1]);
    //
    ioservice.run();

    return EXIT_SUCCESS;
}
