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
    m_state(new MasterClientState::InitialState(*this))
{
    _doConnect(endpoints);
}

MasterClient::~MasterClient() {
    m_socket.close();
}

void MasterClient::_doConnect(const tcp::resolver::results_type& endpoints)
{
    boost::asio::async_connect(m_socket, endpoints,
        [this](boost::system::error_code ec, tcp::endpoint)
        {
            if (!ec) {
                // TODO: communicate with the master server.
                _sendPreambleHeader();
            } 
        }
    );
}

void MasterClient::_sendPreambleHeader() {
    std::uint16_t serverNameSize = static_cast<std::uint16_t>(
        m_serverName.size()
    );

    boost::asio::async_write(m_socket, boost::asio::buffer(
        reinterpret_cast<const char*>(&serverNameSize), sizeof(std::uint16_t)),
        [this](boost::system::error_code ec, std::size_t /*length*/)
        {
            _sendPreambleContent();
        }
    );
}

void MasterClient::_sendPreambleContent() {
    boost::asio::async_write(m_socket, boost::asio::buffer(
        m_serverName, m_serverName.size()),
        [this](boost::system::error_code ec, std::size_t /*length*/)
        {
            next();
        }
    );

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
            if (!ec)
            {
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
                m_state->onRead(m_payload);
            }
        }
    );
}


void MasterClient::changeState(MasterClientState::MasterClientState* state) {
    m_state.reset(state);
}
