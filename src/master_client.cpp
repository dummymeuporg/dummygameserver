#define BOOST_LOG_DYN_LINK 1

#include <boost/log/trivial.hpp>

#include "master_client_state/initial_state.hpp"

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
    m_state(new MasterClientState::InitialState(*this))
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
            _sendPreambleContent();
        }
    );
}

void MasterClient::_sendPreambleContent() {
    boost::asio::async_write(m_socket, boost::asio::buffer(
        m_serverName, m_serverName.size()),
        [this](boost::system::error_code ec, std::size_t /*length*/)
        {
            BOOST_LOG_TRIVIAL(debug) << "Wrote preamble content.";
            next();
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
    _doReadHeader();

}

void MasterClient::_doReadHeader()
{
    boost::asio::async_read(
        m_socket,
        boost::asio::buffer(&m_header, sizeof(std::uint16_t)),
        [this](boost::system::error_code ec, std::size_t length)
        {
            BOOST_LOG_TRIVIAL(debug) << "Read " << length << " bytes.";
            if (!ec)
            {
                BOOST_LOG_TRIVIAL(debug) << "Will read "
                                         << m_header << " more bytes.";
                m_payload.resize(m_header);
                _doReadContent();
            }

        }
    );
}

void MasterClient::_doReadContent()
{
    boost::asio::async_read(
        m_socket,
        boost::asio::buffer(m_payload, m_header),
        [this](boost::system::error_code ec, std::size_t lenght)
        {
            if (!ec)
            {
                BOOST_LOG_TRIVIAL(debug) << "Read " << lenght << " bytes.";
                m_state->onRead(m_payload);
            }
        }
    );
}


void MasterClient::changeState(MasterClientState::MasterClientState* state) {
    m_state.reset(state);
}
