#define BOOST_LOG_DYN_LINK 1

#include <boost/log/trivial.hpp>

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
    _loadMasterKey();
    _doConnect(endpoints);
}

MasterClient::~MasterClient() {
    ::EVP_PKEY_free(m_masterPublicKey);
    m_socket.close();
}

void MasterClient::_doConnect(const tcp::resolver::results_type& endpoints)
{
    boost::asio::async_connect(m_socket, endpoints,
        [this](boost::system::error_code ec, tcp::endpoint)
        {
            BOOST_LOG_TRIVIAL(debug) << "Error code: " << ec.value();
            if (!ec) {
                // TODO: communicate with the master server.
                _sendPreambleHeader();
            } else {
                BOOST_LOG_TRIVIAL(debug) << "Could not connect to server.";
            }
        }
    );
}

void MasterClient::_sendPreambleHeader() {
    std::uint16_t serverNameSize = static_cast<std::uint16_t>(
        m_serverName.size()
    );
    BOOST_LOG_TRIVIAL(debug) << "Server name size is: " << serverNameSize;

    boost::asio::async_write(m_socket, boost::asio::buffer(
        reinterpret_cast<const char*>(&serverNameSize), sizeof(std::uint16_t)),
        [this](boost::system::error_code ec, std::size_t /*length*/)
        {
            BOOST_LOG_TRIVIAL(debug) << "Wrote preamble header.";
        }
    );
}

void MasterClient::_loadMasterKey() {

    ::FILE* fMasterPub = ::fopen(
        fs::path(m_configDir / "master.pem").string().c_str(), "rb");
    if (nullptr == fMasterPub) {
        throw Dummy::Crypto::PEMFileError();
    }

    m_masterPublicKey = PEM_read_PUBKEY(fMasterPub, NULL, NULL, NULL);
    if (nullptr == m_masterPublicKey) {
        ::fclose(fMasterPub);
        throw Dummy::Crypto::KeyLoadingError();
    }

    ::fclose(fMasterPub);
}

void MasterClient::_encryptMasterKey() {

}

void MasterClient::next() {

}

void MasterClient::changeState(MasterClientState::MasterClientState* state) {
    m_state.reset(state);
}
