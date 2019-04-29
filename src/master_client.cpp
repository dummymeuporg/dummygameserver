#include "master_client.hpp"

namespace fs = boost::filesystem;

MasterClient::MasterClient(boost::asio::io_service& ioService,
                           const tcp::resolver::results_type& endpoints,
                           fs::path projectDir, fs::path accountsDir,
                           fs::path configDir, const char* serverName) :
    m_ioService(ioService), m_socket(ioService),
    m_projectDir(projectDir), m_accountsDir(accountsDir),
    m_configDir(configDir), m_serverName(serverName),
    m_masterPublicKey(nullptr),
    m_rsaKeyPair(fs::path(m_configDir / ".conf" / "pub.pem").string(),
                 fs::path(m_configDir / ".conf" / "priv.pem").string()),
    m_state(nullptr)
{
    _doConnect(endpoints);
}

void MasterClient::_doConnect(const tcp::resolver::results_type& endpoints)
{
    boost::asio::async_connect(m_socket, endpoints,
        [this](boost::system::error_code ec, tcp::endpoint)
        {
            if (!ec)
            {
                // TODO: communicate with the master server.
            }
        }
    );
}

void MasterClient::next() {

}

void MasterClient::changeState(MasterClientState::MasterClientState* state) {
    m_state.reset(state);
}
