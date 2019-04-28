#include "master_client.hpp"

namespace fs = boost::filesystem;

MasterClient::MasterClient(boost::asio::io_service& ioService,
                           fs::path projectDir, fs::path accountsDir,
                           fs::path configDir) :
    m_ioService(ioService), m_socket(ioService),
    m_projectDir(projectDir), m_accountsDir(accountsDir),
    m_configDir(configDir), m_masterPublicKey(nullptr),
    m_rsaKeyPair(fs::path(m_configDir / ".conf" / "pub.pem").string(),
                 fs::path(m_configDir / ".conf" / "priv.pem").string())
{
}

void MasterClient::_loadRSAKeyPair() {
}
