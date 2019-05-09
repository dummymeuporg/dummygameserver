#pragma once

#include <map>
#include <memory>

#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/filesystem.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>

#include "core/account.hpp"
#include "core/project.hpp"

namespace fs = boost::filesystem;
using boost::asio::ip::tcp;

class AbstractGameServer {
public:
    AbstractGameServer(boost::asio::io_service&,
                       unsigned short port,
                       const fs::path& projectPath,
                       const fs::path& serverPath);

    const fs::path& serverPath() const {
        return m_serverPath;
    }

    void connect(const boost::uuids::uuid& sessionID,
                 const std::string&);
    bool isPending(const boost::uuids::uuid&) const;
    bool isAuthenticated(const std::string&) const;

    virtual void run() = 0;

protected:
    void _doAccept();

    boost::asio::ip::tcp::acceptor m_acceptor;
    Dummy::Core::Project m_project;
    fs::path m_serverPath;
    std::map<boost::uuids::uuid,
             std::shared_ptr<Dummy::Core::Account>> m_pendingAccounts;
    std::map<std::string,
             std::shared_ptr<Dummy::Core::Account>> m_connectedAccounts;

};
