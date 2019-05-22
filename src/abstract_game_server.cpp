#include <iostream>

#include "abstract_game_server.hpp"
#include "errors.hpp"
#include "game_session.hpp"

AbstractGameServer::AbstractGameServer(
    boost::asio::io_service& ioService,
    unsigned short port,
    const fs::path& projectPath,
    const fs::path& serverPath
) : m_acceptor(ioService,
               boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(),
                                              port)),
    m_project(projectPath),
    m_serverPath(serverPath)
{
    // Validate config?
    // Load project?
}

void AbstractGameServer::_doAccept()
{
	m_acceptor.async_accept(
    	[this](boost::system::error_code ec,
               boost::asio::ip::tcp::socket socket)
        {
        	if (!ec) {
				
            	std::make_shared<GameSession>(std::move(socket),
                                              *this)->start();
				
				std::cerr << "Start a new session." << std::endl;
            }
            _doAccept();
        }
    );
}

void AbstractGameServer::connect(
    const boost::uuids::uuid& sessionID,
    const std::string& accountName)
{
    if (m_pendingAccounts.find(sessionID) == m_pendingAccounts.end()) {
        return; // Throw an exception?
    }

    if (m_connectedAccounts.find(accountName) == m_connectedAccounts.end()) {
        return; // Throw an exception?
    }

    std::cerr << "Connect account " << accountName << std::endl;
    std::shared_ptr<Dummy::Core::Account> account =
        m_pendingAccounts[sessionID];
    m_connectedAccounts[accountName] = account;
}

bool
AbstractGameServer::isAuthenticated(const std::string& accountName) const {
    return m_connectedAccounts.find(accountName) != m_connectedAccounts.end();
}

bool
AbstractGameServer::isPending(const boost::uuids::uuid& id) const {
    return m_pendingAccounts.find(id) != m_pendingAccounts.end();
}

bool
AbstractGameServer::skinExists(const std::string& skin) const {
    return fs::exists(m_project.projectPath() / "chipsets" / skin);
}

bool
AbstractGameServer::characterExists(
    const Dummy::Core::Character& character
) const {
    // get canonical name, check if file exists.
    // that’s it.
    return fs::exists(m_serverPath / "characters" / character.filename());
}

void AbstractGameServer::saveCharacter(
    const Dummy::Core::Account& account,
    const Dummy::Core::Character& character
) const
{
    fs::path characterFullPath(m_serverPath
        / "accounts"
        / account.name()
        / "characters"
        / character.filename()
    );

    fs::path symlinkPath(m_serverPath / "characters" / character.filename());

    if (!fs::exists(characterFullPath)) {
        //XXX: This should not happen. Throw an exception?
    }
    std::cerr << "Path:" << characterFullPath.string() << std::endl;
    std::ofstream ofs(characterFullPath.string(),
                      std::ofstream::out | std::ios::binary);
    ofs << character;
    ofs.close();

    // Create a (relative) symlink to reference the character.
    fs::create_symlink(
        fs::path("..")
        / "accounts"
        / account.name()
        / "characters"
        / character.filename(),
        symlinkPath
    );
}

Dummy::Core::Character
AbstractGameServer::createCharacter(const Dummy::Core::Account& account,
                                    const std::string& characterName,
                                    const std::string& skin) const
{
    Dummy::Core::Character chr;

    chr.setName(characterName);

    if(characterExists(chr)) {
        throw ::CharacterAlreadyExists();
    }

    chr.setSkin(skin);

    if (!skinExists(chr.skin())) {
        throw ::SkinDoesNotExist();
    }

    // From now, we consider the character being valid.
    chr.setPosition(m_project.startingPosition());
    chr.setMapLocation(m_project.startingMap());

    saveCharacter(account, chr);

    return chr;

}
