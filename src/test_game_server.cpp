#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include "test_game_server.hpp"

TestGameServer::TestGameServer(
    boost::asio::io_service& ioService,
    unsigned short port,
    const fs::path& projectPath,
    const fs::path& serverPath
) : AbstractGameServer(ioService, port, projectPath, serverPath)
{
    // Instantiate a Test account. Put it in the pending accounts.
}

void TestGameServer::_instantiateTestAccount()
{
    std::string testAccountName("TEST.0000");
    fs::path accountPath(
        m_serverPath / "accounts" / testAccountName
    );
    if (!fs::exists(accountPath)) {
        std::cerr << "Test account directory does not exist. Create it."
            << std::endl;
        fs::create_directory(accountPath);
    }

    boost::uuids::string_generator gen;
    Dummy::Core::Account account(
        testAccountName,
        gen("00000000-0000-0000-0000-000000000000")
    );

    std::cerr << "Connect to the server using " << testAccountName
        << " and " << account.sessionUUID() << std::endl;
    m_pendingAccounts[account.sessionUUID()] =
        std::make_shared<Dummy::Core::Account>(account);
}

void TestGameServer::run() {
    _instantiateTestAccount();
    _doAccept();
}
