#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include "test_game_server.hpp"

TestGameServer::TestGameServer(
    boost::asio::io_service& ioService,
    unsigned short port,
) : AbstractGameServer(ioService, port, projectPath, serverPath)
{
    // Instantiate a Test account. Put it in the pending accounts.
}

void TestGameServer::_instantiateTestAccount(
    const std::string& accountName, const std::string& sessionID
)
{
    std::string testAccountName(accountName);
    fs::path accountPath(
        m_serverPath / "accounts" / testAccountName
    );
    if (!fs::exists(accountPath)) {
        std::cerr << "Test account directory does not exist. Create it."
            << std::endl;
        fs::create_directory(accountPath);
        fs::create_directory(accountPath / "characters");
    }

    boost::uuids::string_generator gen;
    Dummy::Core::Account account(
        testAccountName,
        gen(sessionID)
    );

    std::cerr << "Connect to the server using " << testAccountName
        << " and " << account.sessionUUID() << std::endl;
    m_pendingAccounts[account.sessionUUID()] =
        std::make_shared<Dummy::Core::Account>(account);
}

void TestGameServer::run() {
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
