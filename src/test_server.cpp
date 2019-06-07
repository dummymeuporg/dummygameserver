#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include "test_server.hpp"

TestServer::TestServer(
    boost::asio::io_service& ioService,
    unsigned short port,
    Dummy::Server::AbstractGameServer& server
) : NetworkServer(ioService, port, server)
{
    // Instantiate a Test account. Put it in the pending accounts.
}


void TestServer::_instantiateTestAccount(
    const std::string& accountName, const std::string& sessionID
)
{
    std::string testAccountName(accountName);
    fs::path accountPath(
        m_gameServer.serverPath() / "accounts" / accountName
    );
    if (!fs::exists(accountPath)) {
        std::cerr << "Test account directory does not exist. Create it."
            << std::endl;
        fs::create_directory(accountPath);
        fs::create_directory(accountPath / "characters");
    }

    m_gameServer.addPending(sessionID, accountName);
    std::cerr << "Connect to the server using " << testAccountName
        << " and " << sessionID << std::endl;
}

void TestServer::run() {
    _instantiateTestAccount(
        "TEST.0000", "00000000-0000-0000-0000-000000000000"
    );
    _instantiateTestAccount(
        "TEST.1111", "11111111-1111-1111-1111-111111111111"
    );
    _instantiateTestAccount(
        "TEST.2222", "00000000-0000-0000-0000-000000000000"
    );
    _doAccept();
}
