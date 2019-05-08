#include <boost/uuid/uuid_io.hpp>
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
        m_project.projectPath() / "accounts" / testAccountName
    );
    if (!fs::exists(accountPath)) {
        std::cerr << "Test account directory does not exist. Create it."
            << std::endl;
        fs::create_directory(accountPath);
    }

    Dummy::Core::Account account(testAccountName,
                                 boost::uuids::random_generator()());

    std::cout << "Connect to the server using " << account.sessionUUID()
        << std::endl;
}
